//
//  SNSASInternalHelpers.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/11.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreFoundation/CFBase.h>
#import "SNSASBaseDefines.h"

NS_ASSUME_NONNULL_BEGIN

/// For deallocation of objects on a background thread without GCD overhead / thread explosion
AS_EXTERN void ASPerformBackgroundDeallocation(id __strong _Nullable * _Nonnull object);

ASDISPLAYNODE_INLINE BOOL ASImageAlphaInfoIsOpaque(CGImageAlphaInfo info) {
    switch (info) {
        case kCGImageAlphaNone:
        case kCGImageAlphaNoneSkipLast:
        case kCGImageAlphaNoneSkipFirst:
            return YES;
        default:
            return NO;
    }
}

NS_ASSUME_NONNULL_END
