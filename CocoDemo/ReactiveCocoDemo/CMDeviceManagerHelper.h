//
//  CMDeviceManagerHelper.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SystemResourceDef.h"

@protocol CMDeviceManagerHelper <NSObject>
-(void)onGetDeviceTableCellInfo:(int)iResult parentId:(u_int)pId isChildrenZero:(BOOL)zero;
-(void)onDevStatusNotify:(int)uiDevId status:(_CM_DEVICE_STATUS_)eStatus needReload:(BOOL)isReload;
-(void)onChannelNameChangeNotify:(int)uiDevId channelId: (int)uiChannelId newName:(NSString*)strName needReload:(BOOL)isReload;
//-(int)ptzCtrl:(CSubDevId)subDevId ctrltype:(int)iCtrlType intensity:(int)iIntensity;
@end
