//
//  SNSASAysncTransactionContainer.m
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASAysncTransactionContainer.h"
#import "SNSASAsyncTransactionContainer+Private.h"
#import "SNSASTransaction.h"
#import "SNSASTransactionGroup.h"

@implementation CALayer (ASAsyncTransactionContainerTransactions)
@dynamic asyncdisplaykit_asyncLayerTransactions;

// No-ops in the base class. Mostly exposed for testing.
- (void)asyncdisplaykit_asyncTransactionContainerWillBeginTransaction:(SNSASTransaction *)transaction {}
- (void)asyncdisplaykit_asyncTransactionContainerDidCompleteTransaction:(SNSASTransaction *)transaction {}
@end

@implementation CALayer (ASAsyncTransactionContainer)
@dynamic asyncdisplaykit_currentAsyncTransaction;
@dynamic asyncdisplaykit_asyncTransactionContainer;

- (ASAsyncTransactionContainerState)asyncdisplaykit_asyncTransactionContainerState
{
    return ([self.asyncdisplaykit_asyncLayerTransactions count] == 0) ? ASAsyncTransactionContainerStateNoTransactions : ASAsyncTransactionContainerStatePendingTransactions;
}

- (void)asyncdisplaykit_cancelAsyncTransactions
{
    // If there was an open transaction, commit and clear the current transaction. Otherwise:
    // (1) The run loop observer will try to commit a canceled transaction which is not allowed
    // (2) We leave the canceled transaction attached to the layer, dooming future operations
    SNSASTransaction *currentTransaction = self.asyncdisplaykit_currentAsyncTransaction;
    [currentTransaction commit];
    self.asyncdisplaykit_currentAsyncTransaction = nil;
    
    for (SNSASTransaction *transaction in [self.asyncdisplaykit_asyncLayerTransactions copy]) {
        [transaction cancel];
    }
}

- (SNSASTransaction *)asyncdisplaykit_asyncTransaction
{
    SNSASTransaction *transaction = self.asyncdisplaykit_currentAsyncTransaction;
    if (transaction == nil) {
        NSHashTable *transactions = self.asyncdisplaykit_asyncLayerTransactions;
        if (transactions == nil) {
            transactions = [NSHashTable hashTableWithOptions:NSHashTableObjectPointerPersonality];
            self.asyncdisplaykit_asyncLayerTransactions = transactions;
        }
        __weak CALayer *weakSelf = self;
        transaction = [[SNSASTransaction alloc] initWithCompletionBlock:^(SNSASTransaction *completedTransaction, BOOL cancelled) {
            __strong CALayer *self = weakSelf;
            if (self == nil) {
                return;
            }
            [transactions removeObject:completedTransaction];
            [self asyncdisplaykit_asyncTransactionContainerDidCompleteTransaction:completedTransaction];
        }];
        [transactions addObject:transaction];
        self.asyncdisplaykit_currentAsyncTransaction = transaction;
        [self asyncdisplaykit_asyncTransactionContainerWillBeginTransaction:transaction];
    }
    [SNSASTransactionGroup.mainTransactionGroup addTransactionContainer:self];
    return transaction;
}

- (CALayer *)asyncdisplaykit_parentTransactionContainer
{
    CALayer *containerLayer = self;
    while (containerLayer && !containerLayer.asyncdisplaykit_isAsyncTransactionContainer) {
        containerLayer = containerLayer.superlayer;
    }
    return containerLayer;
}

@end

@implementation UIView (ASAsyncTransactionContainer)

- (BOOL)asyncdisplaykit_isAsyncTransactionContainer
{
    return self.layer.asyncdisplaykit_isAsyncTransactionContainer;
}

- (void)asyncdisplaykit_setAsyncTransactionContainer:(BOOL)asyncTransactionContainer
{
    self.layer.asyncdisplaykit_asyncTransactionContainer = asyncTransactionContainer;
}

- (ASAsyncTransactionContainerState)asyncdisplaykit_asyncTransactionContainerState
{
    return self.layer.asyncdisplaykit_asyncTransactionContainerState;
}

- (void)asyncdisplaykit_cancelAsyncTransactions
{
    [self.layer asyncdisplaykit_cancelAsyncTransactions];
}

- (void)asyncdisplaykit_asyncTransactionContainerStateDidChange
{
    // No-op in the base class.
}

- (void)asyncdisplaykit_setCurrentAsyncTransaction:(SNSASTransaction *)transaction
{
    self.layer.asyncdisplaykit_currentAsyncTransaction = transaction;
}

- (SNSASTransaction *)asyncdisplaykit_currentAsyncTransaction
{
    return self.layer.asyncdisplaykit_currentAsyncTransaction;
}

@end
