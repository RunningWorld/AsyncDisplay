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
#import "SNSASDisplayNode+FrameworkPrivate.h"
#import "SNSASEqualityHelpers.h"

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
        ASDisplayNodeAssert(ASObjectIsEqual(_cachedSubnodes, _subnodes), @"Expected _subnodes and _cachedSubnodes to have the same contents.");
    }
    return _cachedSubnodes ?: @[];
}

- (BOOL)rasterizesSubtree
{
    ASDN::MutexLocker l(__instanceLock__);
    return _flags.rasterizesSubtree;
}

- (ASHierarchyState)hierarchyState
{
    ASDN::MutexLocker l(__instanceLock__);
    return _hierarchyState;
}

- (void)setHierarchyState:(ASHierarchyState)newState
{
    ASHierarchyState oldState = ASHierarchyStateNormal;
    {
        ASDN::MutexLocker l(__instanceLock__);
        if (_hierarchyState == newState) {
            return;
        }
        oldState = _hierarchyState;
        _hierarchyState = newState;
    }
    
    // Entered rasterization state.
//    if (newState & ASHierarchyStateRasterized) {
//        ASDisplayNodeAssert(checkFlag(Synchronous) == NO, @"Node created using -initWithViewBlock:/-initWithLayerBlock: cannot be added to subtree of node with subtree rasterization enabled. Node: %@", self);
//    }
//
//    // Entered or exited range managed state.
//    if ((newState & ASHierarchyStateRangeManaged) != (oldState & ASHierarchyStateRangeManaged)) {
//        if (newState & ASHierarchyStateRangeManaged) {
//            [self enterInterfaceState:self.supernode.pendingInterfaceState];
//        } else {
//            // The case of exiting a range-managed state should be fairly rare.  Adding or removing the node
//            // to a view hierarchy will cause its interfaceState to be either fully set or unset (all fields),
//            // but because we might be about to be added to a view hierarchy, exiting the interface state now
//            // would cause inefficient churn.  The tradeoff is that we may not clear contents / fetched data
//            // for nodes that are removed from a managed state and then retained but not used (bad idea anyway!)
//        }
//    }
//
//    if ((newState & ASHierarchyStateLayoutPending) != (oldState & ASHierarchyStateLayoutPending)) {
//        if (newState & ASHierarchyStateLayoutPending) {
//            // Entering layout pending state
//        } else {
//            // Leaving layout pending state, reset related properties
//            ASDN::MutexLocker l(__instanceLock__);
//            _pendingTransitionID = ASLayoutElementContextInvalidTransitionID;
//            _pendingLayoutTransition = nil;
//        }
//    }
//
//    ASDisplayNodeLogEvent(self, @"setHierarchyState: %@", NSStringFromASHierarchyStateChange(oldState, newState));
//    as_log_verbose(ASNodeLog(), "%s%@ %@", sel_getName(_cmd), NSStringFromASHierarchyStateChange(oldState, newState), self);
}

@end
