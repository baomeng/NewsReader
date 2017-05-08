//
//  ModalAlertDelegate.m
//  SKInfoPlatform
//
//  Created by yinxs on 13-3-17.
//  Copyright (c) 2013年 seegle. All rights reserved.
//

#import "ModalAlertDelegate.h"

@implementation ModalAlertDelegate
@synthesize index;

// Initialize with the supplied run loop
-(id) initWithRunLoop: (CFRunLoopRef)runLoop
{
    if (self = [super init]) currentLoop = runLoop;
    return self;
}

// User pressed button. Retrieve results
-(void) alertView: (UIAlertView*)aView clickedButtonAtIndex: (NSInteger)anIndex
{
    index = anIndex;
//    CFRunLoopStop(currentLoop);
}
@end
