//
//  ModalAlert.h
//  SKInfoPlatform
//
//  Created by yinxs on 13-3-17.
//  Copyright (c) 2013年 seegle. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ModalAlert : NSObject
+ (BOOL) ask: (NSString *) question;
+ (BOOL) confirm:(NSString *) statement;
+ (BOOL)MessageBox:(NSString*)msg;
@end
