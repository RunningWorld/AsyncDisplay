//
//  SNSASObjectDescriptionHelpers.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/29.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifndef SNSASObjectDescriptionHelpers_h
#define SNSASObjectDescriptionHelpers_h

/**
 * Your base class should conform to this and override `-debugDescription`
 * to call `[self propertiesForDebugDescription]` and use `ASObjectDescriptionMake`
 * to return a string. Subclasses of this base class just need to override
 * `propertiesForDebugDescription`, call super, and modify the result as needed.
 */
@protocol ASDebugDescriptionProvider
@required
- (NSMutableArray<NSDictionary *> *)propertiesForDebugDescription;
@end

/**
 * Your base class should conform to this and override `-description`
 * to call `[self propertiesForDescription]` and use `ASObjectDescriptionMake`
 * to return a string. Subclasses of this base class just need to override
 * `propertiesForDescription`, call super, and modify the result as needed.
 */
@protocol ASDescriptionProvider
@required
- (NSMutableArray<NSDictionary *> *)propertiesForDescription;
@end

#endif /* SNSASObjectDescriptionHelpers_h */
