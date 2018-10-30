//
//  SNSASAsyncTransactionContainer+Private.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

NS_ASSUME_NONNULL_BEGIN

@class SNSASTransaction;

@interface CALayer (ASAsyncTransactionContainerTransactions)
@property (nonatomic, nullable, setter=asyncdisplaykit_setAsyncLayerTransactions:) NSHashTable<SNSASTransaction *> *asyncdisplaykit_asyncLayerTransactions;

- (void)asyncdisplaykit_asyncTransactionContainerWillBeginTransaction:(SNSASTransaction *)transaction;
- (void)asyncdisplaykit_asyncTransactionContainerDidCompleteTransaction:(SNSASTransaction *)transaction;
@end

NS_ASSUME_NONNULL_END
