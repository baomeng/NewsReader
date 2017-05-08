//
//  CMDeviceTableCellInfo.m
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.


#import "CMDeviceTableCellInfo.h"

@implementation CMDeviceTableCellInfo

@synthesize self_id,parent_id,name,status,type;

- (id)init
{
    self = [super init];
    if (self) {
        // Initialize self.
        
    }
    return self;
}

- (void)dealloc{
    if (name!=nil) {
        [name release];
    }
    [super dealloc];
}
- (id)copyWithZone:(NSZone *)zone
{
    CMDeviceTableCellInfo *copyObj = [[[self class] allocWithZone:zone] init];
    copyObj.self_id = self.self_id;
    copyObj.parent_id = self.parent_id;
    copyObj.name = [self.name copy];
    copyObj.status = self.status;
    copyObj.type = self.type;
    return copyObj;
}
@end
