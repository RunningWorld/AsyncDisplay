//
//  SNSASDisplayNodeInternal.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASDisplayNode.h"
#import "SNSASThread.h"
#import "SNSASTransactionContext.h"

@class SNSASDisplayLayer;

#define VISIBILITY_NOTIFICATIONS_DISABLED_BITS 4

@interface SNSASDisplayNode () <SNSASTransitionContextCompletionDelegate>
{
@package
    ASDN::RecursiveMutex __instanceLock__;
    UIView *_view;
    CALayer *_layer;
    
    struct ASDisplayNodeFlags {
        // public properties
        unsigned viewEverHadAGestureRecognizerAttached:1;
        unsigned layerBacked:1;
        unsigned displaysAsynchronously:1;
        unsigned rasterizesSubtree:1;
        unsigned shouldBypassEnsureDisplay:1;
        unsigned displaySuspended:1;
        unsigned shouldAnimateSizeChanges:1;

        // Wrapped view handling

        // The layer contents should not be cleared in case the node is wrapping a UIImageView.UIImageView is specifically
        // optimized for performance and does not use the usual way to provide the contents of the CALayer via the
        // CALayerDelegate method that backs the UIImageView.
        unsigned canClearContentsOfLayer:1;

        // Prevent calling setNeedsDisplay on a layer that backs a UIImageView. Usually calling setNeedsDisplay on a CALayer
        // triggers a recreation of the contents of layer unfortunately calling it on a CALayer that backs a UIImageView
        // it goes through the normal flow to assign the contents to a layer via the CALayerDelegate methods. Unfortunately
        // UIImageView does not do recreate the layer contents the usual way, it actually does not implement some of the
        // methods at all instead it throws away the contents of the layer and nothing will show up.
        unsigned canCallSetNeedsDisplayOfLayer:1;

        unsigned implementsDrawRect:1;
        unsigned implementsImageDisplay:1;
        unsigned implementsDrawParameters:1;

        // internal state
        unsigned isEnteringHierarchy:1;
        unsigned isExitingHierarchy:1;
        unsigned isInHierarchy:1;
        unsigned visibilityNotificationsDisabled:VISIBILITY_NOTIFICATIONS_DISABLED_BITS;
        unsigned isDeallocating:1;
    } _flags;
@protected
    SNSASDisplayNode * __weak _supernode;
    NSMutableArray<SNSASDisplayNode *> *_subnodes;
    std::atomic_uint _displaySentinel;
    // Set this to nil whenever you modify _subnodes
    NSArray<SNSASDisplayNode *> *_cachedSubnodes;
    // This is the desired contentsScale, not the scale at which the layer's contents should be displayed
    CGFloat _contentsScaleForDisplay;
    CGFloat _cornerRadius;
    ASCornerRoundingType _cornerRoundingType;
    ASDisplayNodeContextModifier _willDisplayNodeContentWithRenderingContext;
    ASDisplayNodeContextModifier _didDisplayNodeContentWithRenderingContext;
}

/// The _ASDisplayLayer backing the node, if any.
@property (nullable, nonatomic, readonly) SNSASDisplayLayer *asyncLayer;

/**
 * Whether this node rasterizes its descendants. See -enableSubtreeRasterization.
 */
@property (readonly) BOOL rasterizesSubtree;

/**
 * Called whenever the node needs to layout its subnodes and, if it's already loaded, its subviews. Executes the layout pass for the node
 *
 * This method is thread-safe but asserts thread affinity.
 */
- (void)__layout;


@end
