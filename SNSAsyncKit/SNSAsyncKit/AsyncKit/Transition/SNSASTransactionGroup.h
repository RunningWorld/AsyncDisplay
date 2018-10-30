//
//  SNSASTransactionGroup.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SNSASBaseDefines.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ASAsyncTransactionContainer;

/// A group of transaction containers, for which the current transactions are committed together at the end of the next runloop tick.
AS_SUBCLASSING_RESTRICTED
@interface SNSASTransactionGroup : NSObject

/// The main transaction group is scheduled to commit on every tick of the main runloop.
/// Access from the main thread only.
@property (class, nonatomic, readonly) SNSASTransactionGroup *mainTransactionGroup;

- (void)commit;

/// Add a transaction container to be committed.
- (void)addTransactionContainer:(id<ASAsyncTransactionContainer>)container;

/// Use the main group.
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
