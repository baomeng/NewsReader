//
//  CMPlayerData.m
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "CMPlayerData.h"

@implementation CMPlayerData
@synthesize player,status,cellInfo;
@synthesize currentFrameRate,currentMonitorModel;

- (id)init
{
    self = [super init];
    if (self) {
        // Initialize self.
        
    }
    return self;
}


-(void)setPlayer:(CMMonitorPlayer *)p
{
    player = p;
}

-(void)SetCurCellInfo:(CMDeviceTableCellInfo*)info
{
    cellInfo = info;
}

-(id)GetCurCellInfo
{
    return cellInfo;
}

@end
