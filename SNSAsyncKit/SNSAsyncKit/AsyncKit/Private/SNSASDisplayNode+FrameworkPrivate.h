//
//  SNSASDisplayNode+FrameworkPrivate.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASDisplayNode.h"
#import "SNSASObjectDescriptionHelpers.h"

/**
 Hierarchy state is propagated from nodes to all of their children when certain behaviors are required from the subtree.
 Examples include rasterization and external driving of the .interfaceState property.
 By passing this information explicitly, performance is optimized by avoiding iteration up the supernode chain.
 Lastly, this avoidance of supernode traversal protects against the possibility of deadlocks when a supernode is
 simultaneously attempting to materialize views / layers for its subtree (as many related methods require property locking)
 
 Note: as the hierarchy deepens, more state properties may be enabled.  However, state properties may never be disabled /
 cancelled below the point they are enabled.  They continue to the leaves of the hierarchy.
 */

typedef NS_OPTIONS(NSUInteger, ASHierarchyState)
{
    /** The node may or may not have a supernode, but no supernode has a special hierarchy-influencing option enabled. */
    ASHierarchyStateNormal                  = 0,
    /** The node has a supernode with .rasterizesSubtree = YES.
     Note: the root node of the rasterized subtree (the one with the property set on it) will NOT have this state set. */
    ASHierarchyStateRasterized              = 1 << 0,
    /** The node or one of its supernodes is managed by a class like ASRangeController.  Most commonly, these nodes are
     ASCellNode objects or a subnode of one, and are used in ASTableView or ASCollectionView.
     These nodes also receive regular updates to the .interfaceState property with more detailed status information. */
    ASHierarchyStateRangeManaged            = 1 << 1,
    /** Down-propagated version of _flags.visibilityNotificationsDisabled.  This flag is very rarely set, but by having it
     locally available to nodes, they do not have to walk up supernodes at the critical points it is checked. */
    ASHierarchyStateTransitioningSupernodes = 1 << 2,
    /** One of the supernodes of this node is performing a transition.
     Any layout calculated during this state should not be applied immediately, but pending until later. */
    ASHierarchyStateLayoutPending           = 1 << 3,
};

@interface SNSASDisplayNode () <ASDescriptionProvider, ASDebugDescriptionProvider>
{
@protected
    ASHierarchyState _hierarchyState;
}

// Thread safe way to access the bounds of the node
@property (nonatomic) CGRect threadSafeBounds;

// Returns the bounds of the node without reaching the view or layer
- (CGRect)_locked_threadSafeBounds;

/**
 * @abstract Returns the Hierarchy State of the node.
 *
 * @return The current ASHierarchyState of the node, indicating whether it is rasterized or managed by a range controller.
 *
 * @see ASInterfaceState
 */
@property (nonatomic) ASHierarchyState hierarchyState;

@end

@interface UIView (ASDisplayNodeInternal)
@property (nullable, weak) SNSASDisplayNode *asyncdisplaykit_node;
@end

@interface CALayer (ASDisplayNodeInternal)
@property (nullable, weak) SNSASDisplayNode *asyncdisplaykit_node;
@end
