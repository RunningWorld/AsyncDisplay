//
//  SNSASAysncTransactionContainer.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SNSASTransaction;


NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ASAsyncTransactionContainerState) {
    /**
     The async container has no outstanding transactions.
     Whatever it is displaying is up-to-date.
     */
    ASAsyncTransactionContainerStateNoTransactions = 0,
    /**
     The async container has one or more outstanding async transactions.
     Its contents may be out of date or showing a placeholder, depending on the configuration of the contained ASDisplayLayers.
     */
    ASAsyncTransactionContainerStatePendingTransactions,
};

@protocol ASAsyncTransactionContainer

/**
 @summary If YES, the receiver is marked as a container for async transactions, grouping all of the transactions
 in the container hierarchy below the receiver together in a single ASAsyncTransaction.
 
 @default NO
 */
@property (nonatomic, getter=asyncdisplaykit_isAsyncTransactionContainer, setter=asyncdisplaykit_setAsyncTransactionContainer:) BOOL asyncdisplaykit_asyncTransactionContainer;

/**
 @summary The current state of the receiver; indicates if it is currently performing asynchronous operations or if all operations have finished/canceled.
 */
@property (nonatomic, readonly) ASAsyncTransactionContainerState asyncdisplaykit_asyncTransactionContainerState;

/**
 @summary Cancels all async transactions on the receiver.
 */
- (void)asyncdisplaykit_cancelAsyncTransactions;

@property (nullable, nonatomic, setter=asyncdisplaykit_setCurrentAsyncTransaction:) SNSASTransaction *asyncdisplaykit_currentAsyncTransaction;

@end

@interface CALayer (ASAsyncTransactionContainer) <ASAsyncTransactionContainer>
/**
 @summary Returns the current async transaction for this layer. A new transaction is created if one
 did not already exist. This method will always return an open, uncommitted transaction.
 @desc asyncdisplaykit_asyncTransactionContainer does not need to be YES for this to return a transaction.
 Defaults to nil.
 */
@property (nullable, nonatomic, readonly) SNSASTransaction *asyncdisplaykit_asyncTransaction;

/**
 @summary Goes up the superlayer chain until it finds the first layer with asyncdisplaykit_asyncTransactionContainer=YES (including the receiver) and returns it.
 Returns nil if no parent container is found.
 */
@property (nullable, nonatomic, readonly) CALayer *asyncdisplaykit_parentTransactionContainer;

/**
 @summary Whether or not this layer should serve as a transaction container.
 Defaults to NO.
 */
@property (nonatomic, getter=asyncdisplaykit_isAsyncTransactionContainer, setter = asyncdisplaykit_setAsyncTransactionContainer:) BOOL asyncdisplaykit_asyncTransactionContainer;

@end

@interface UIView (ASAsyncTransactionContainer) <ASAsyncTransactionContainer>
@end

NS_ASSUME_NONNULL_END
