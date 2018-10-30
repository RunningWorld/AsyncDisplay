//
//  SNSASDisplayNode.m
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASDisplayNode.h"
#import "SNSASDisplayNodeInternal.h"
#import "SNSASDisplayLayer.h"

@implementation SNSASDisplayNode

- (BOOL)tryLock
{
    return NO;
}

#pragma mark Layout Pass

// Returns nil if the layer is not an _ASDisplayLayer; will not create the layer if nil.
- (SNSASDisplayLayer *)asyncLayer
{
    ASDN::MutexLocker l(__instanceLock__);
    return [self _locked_asyncLayer];
}

- (SNSASDisplayLayer *)_locked_asyncLayer
{
    ASAssertLocked(__instanceLock__);
    return [_layer isKindOfClass:[SNSASDisplayLayer class]] ? (SNSASDisplayLayer *)_layer : nil;
}


- (void)__layout
{
    
}

- (NSArray *)subnodes
{
    ASDN::MutexLocker l(__instanceLock__);
    if (_cachedSubnodes == nil) {
        _cachedSubnodes = [_subnodes copy];
    } else {
//        ASDisplayNodeAssert(ASObjectIsEqual(_cachedSubnodes, _subnodes), @"Expected _subnodes and _cachedSubnodes to have the same contents.");
    }
    return _cachedSubnodes ?: @[];
}

@end
