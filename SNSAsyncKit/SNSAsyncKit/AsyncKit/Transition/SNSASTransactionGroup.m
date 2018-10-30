//
//  SNSASTransactionGroup.m
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASTransactionGroup.h"
#import "SNSASAysncTransactionContainer.h"
#import "SNSASAssert.h"
#import "SNSASTransaction.h"
#import "SNSASAsyncTransactionContainer+Private.h"

@implementation SNSASTransactionGroup
{
    NSHashTable <id<ASAsyncTransactionContainer>> *_containers;
}

+ (SNSASTransactionGroup *)mainTransactionGroup
{
    ASDisplayNodeAssertMainThread();
    static SNSASTransactionGroup *mainTransactionGroup;
    
    if (mainTransactionGroup == nil) {
        mainTransactionGroup = [[SNSASTransactionGroup alloc] _init];
        [mainTransactionGroup registerAsMainRunloopObserver];
    }
    return mainTransactionGroup;
}

- (void)registerAsMainRunloopObserver
{
    ASDisplayNodeAssertMainThread();
    static CFRunLoopObserverRef observer;
    ASDisplayNodeAssert(observer == NULL, @"A _ASAsyncTransactionGroup should not be registered on the main runloop twice");
    // defer the commit of the transaction so we can add more during the current runloop iteration
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFOptionFlags activities = (kCFRunLoopBeforeWaiting | // before the run loop starts sleeping
                                kCFRunLoopExit);          // before exiting a runloop run
    
    observer = CFRunLoopObserverCreateWithHandler(NULL,        // allocator
                                                  activities,  // activities
                                                  YES,         // repeats
                                                  INT_MAX,     // order after CA transaction commits
                                                  ^(CFRunLoopObserverRef observer, CFRunLoopActivity activity) {
                                                      ASDisplayNodeCAssertMainThread();
                                                      [self commit];
                                                  });
    CFRunLoopAddObserver(runLoop, observer, kCFRunLoopCommonModes);
    CFRelease(observer);
}

- (instancetype)_init
{
    if ((self = [super init])) {
        _containers = [NSHashTable hashTableWithOptions:NSHashTableObjectPointerPersonality];
    }
    return self;
}

- (void)addTransactionContainer:(id<ASAsyncTransactionContainer>)container
{
    ASDisplayNodeAssertMainThread();
    ASDisplayNodeAssert(container != nil, @"No container");
    [_containers addObject:container];
}

- (void)commit
{
    ASDisplayNodeAssertMainThread();
    
    if ([_containers count]) {
        NSHashTable *containersToCommit = _containers;
        _containers = [NSHashTable hashTableWithOptions:NSHashTableObjectPointerPersonality];
        
        for (id<ASAsyncTransactionContainer> container in containersToCommit) {
            // Note that the act of committing a transaction may open a new transaction,
            // so we must nil out the transaction we're committing first.
            SNSASTransaction *transaction = container.asyncdisplaykit_currentAsyncTransaction;
            container.asyncdisplaykit_currentAsyncTransaction = nil;
            [transaction commit];
        }
    }
}

@end
