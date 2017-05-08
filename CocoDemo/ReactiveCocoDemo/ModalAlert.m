//
//  ModalAlert.m
//  SKInfoPlatform
//
//  Created by yinxs on 13-3-17.
//  Copyright (c) 2013年 seegle. All rights reserved.
//

#import "ModalAlert.h"
#import "ModalAlertDelegate.h"
@implementation ModalAlert
+(NSUInteger) queryWith: (NSString *)question button1: (NSString *)button1 button2: (NSString *)button2
{
//    CFRunLoopRef currentLoop = CFRunLoopGetCurrent();
    
    // Create Alert
    ModalAlertDelegate *madelegate = [[ModalAlertDelegate alloc] initWithRunLoop:nil];
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:question message:nil delegate:madelegate cancelButtonTitle:button1 otherButtonTitles:button2, nil];
    [alertView show];
    
    // Wait for response
//    CFRunLoopRun();
    
    // Retrieve answer
    NSUInteger answer = madelegate.index;
  //  [alertView release];
 //   [madelegate release];
    return answer;
}

+ (BOOL) confirm: (NSString *) statement
{
    return  [ModalAlert queryWith:statement button1: @"取消" button2: @"确定"];
}
+ (BOOL)ask:(NSString*)msg
{
    return  [ModalAlert queryWith:msg button1: NSLocalizedString(@"取消", nil) button2: NSLocalizedString(@"确定", nil)];
}
+ (BOOL)MessageBox:(NSString*)msg
{
    return  [ModalAlert queryWith:msg button1: NSLocalizedString(@"确定", nil) button2: nil];
}
@end
