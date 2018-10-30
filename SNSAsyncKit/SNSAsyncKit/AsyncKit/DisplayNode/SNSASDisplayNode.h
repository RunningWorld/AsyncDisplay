//
//  SNSASDisplayNode.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SNSASLocking.h"
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

#define VISIBILITY_NOTIFICATIONS_DISABLED_BITS 4

/**
 * ASDisplayNode will / did render node content in context.
 */
typedef void (^ASDisplayNodeContextModifier)(CGContextRef context, id _Nullable drawParameters);

typedef NS_ENUM(NSInteger, ASCornerRoundingType) {
    ASCornerRoundingTypeDefaultSlowCALayer,
    ASCornerRoundingTypePrecomposited,
    ASCornerRoundingTypeClipping
};

@interface SNSASDisplayNode : NSObject <SNSASLocking>

/**
 * @abstract The receiver's immediate subnodes.
 */
@property (nullable, readonly, copy) NSArray<SNSASDisplayNode *> *subnodes;

/**
 * @abstract The receiver's supernode.
 */
@property (nullable, readonly, weak) SNSASDisplayNode *supernode;

@end

@interface SNSASDisplayNode (UIViewBridge)

@property           CGRect frame;                             // default=CGRectZero
@property           CGRect bounds;                            // default=CGRectZero
@property           CGPoint position;                         // default=CGPointZero
@property           CGFloat alpha;                            // default=1.0f

@property           ASCornerRoundingType cornerRoundingType;  // default=ASCornerRoundingTypeDefaultSlowCALayer .cornerRadius (offscreen rendering)

/** @abstract The radius to use when rounding corners of the ASDisplayNode.
 *
 * @discussion This property is thread-safe and should always be preferred over CALayer's cornerRadius property,
 * even if corner rounding type is ASCornerRoundingTypeDefaultSlowCALayer.
 */
@property           CGFloat cornerRadius;                     // default=0.0

@property           BOOL clipsToBounds;                       // default==NO
@property (getter=isHidden)  BOOL hidden;                     // default==NO
@property (getter=isOpaque)  BOOL opaque;                     // default==YES

@property           CGFloat contentsScale;                    // default=1.0f. See @contentsScaleForDisplay for details.
@property           CGFloat rasterizationScale;               // default=1.0f.

@property           CGPoint anchorPoint;                      // default={0.5, 0.5}
@property           CGFloat zPosition;                        // default=0.0
@property           CATransform3D transform;                  // default=CATransform3DIdentity
@property           CATransform3D subnodeTransform;           // default=CATransform3DIdentity

@property (nullable) id contents;

/**
 * @abstract The node view's background color.
 *
 * @discussion In contrast to UIView, setting a transparent color will not set opaque = NO.
 * This only affects nodes that implement +drawRect like ASTextNode.
 */
@property (nullable, copy) UIColor *backgroundColor;           // default=nil

@property (nullable) CGColorRef shadowColor;                // default=opaque rgb black
@property            CGFloat shadowOpacity;                 // default=0.0
@property            CGSize shadowOffset;                   // default=(0, -3)
@property            CGFloat shadowRadius;                  // default=3
@property            CGFloat borderWidth;                   // default=0
@property (nullable) CGColorRef borderColor;                // default=opaque rgb black

/**
 * @abstract Determines drawing priority of the node. Nodes with higher priority will be drawn earlier.
 *
 * @discussion Defaults to ASDefaultDrawingPriority. There may be multiple drawing threads, and some of them may
 * decide to perform operations in queued order (regardless of drawingPriority)
 */
@property NSInteger drawingPriority;

/**
 * Marks the view as needing display. Convenience for use whether the view / layer is loaded or not. Safe to call from a background thread.
 */
- (void)setNeedsDisplay;

/**
 * Marks the node as needing layout. Convenience for use whether the view / layer is loaded or not. Safe to call from a background thread.
 *
 * If the node determines its own desired layout size will change in the next layout pass, it will propagate this
 * information up the tree so its parents can have a chance to consider and apply if necessary the new size onto the node.
 *
 * Note: ASCellNode has special behavior in that calling this method will automatically notify
 * the containing ASTableView / ASCollectionView that the cell should be resized, if necessary.
 */
- (void)setNeedsLayout;

/**
 * Performs a layout pass on the node. Convenience for use whether the view / layer is loaded or not. Safe to call from a background thread.
 */
- (void)layoutIfNeeded;

@end

NS_ASSUME_NONNULL_END

