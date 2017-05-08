//
//  CMDeviceTableCellInfo.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SystemResourceDef.h"

typedef enum {
    GROUP,
    DEVICE, 
    CHANNEL
}DEVICE_TYPE;

@interface CMDeviceTableCellInfo : NSObject <NSCopying>

@property (retain,nonatomic) NSString               *name;
@property (assign,nonatomic) DEVICE_TYPE            type;
@property (assign,nonatomic) u_int                  self_id;
@property (assign,nonatomic) u_int                  parent_id;
@property (assign,nonatomic) _CM_DEVICE_STATUS_     status;         //only for DEVICE type
@end
