//
//  ModalAlertDelegate.h
//  SKInfoPlatform
//
//  Created by yinxs on 13-3-17.
//  Copyright (c) 2013年 seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ModalAlert.h"

@interface ModalAlertDelegate :  NSObject <UIAlertViewDelegate>
{
    CFRunLoopRef currentLoop;
    NSUInteger index;
}
@property (readonly) NSUInteger index;
-(id) initWithRunLoop: (CFRunLoopRef)runLoop;
@end
