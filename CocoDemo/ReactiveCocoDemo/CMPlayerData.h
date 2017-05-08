//
//  CMPlayerData.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CMMediaModuleProtocolDef.h"

@class CMDeviceTableCellInfo;
@class CMMonitorPlayer;

typedef enum{
    IDLE,
    INIT,
    PAUSE
}PlayerStatus;    //播放器状态

@interface CMPlayerData : NSObject 

@property (strong,nonatomic)        CMDeviceTableCellInfo       *cellInfo;
@property (assign,nonatomic)        PlayerStatus                status;
@property (strong,nonatomic)        CMMonitorPlayer             *player;
@property (assign, nonatomic)       int                         currentFrameRate;
@property (assign, nonatomic)       _REAL_MONITOR_MODEL_        currentMonitorModel;

- (void) setPlayer:(CMMonitorPlayer *)p;
- (void) SetCurCellInfo:(CMDeviceTableCellInfo*)info;
- (id) GetCurCellInfo;

@end
