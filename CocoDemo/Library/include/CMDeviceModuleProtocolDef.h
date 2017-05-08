//
//  CMDeviceModuleProtocolDef.h
//  CMDeviceModule
//
//  Created by cz l on 12-6-20.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#ifndef CMDeviceModule_CMDeviceModuleProtocolDef_h
#define CMDeviceModule_CMDeviceModuleProtocolDef_h

#import "KK_Includes.h"
#import "SystemErrorCodeDef.h"
#import "SystemResourceDef.h"
#import "CMNTDataTransferDef.h"
#import <UIKit/UIKit.h>

////////////////////////////////////////////////////////////////
// PTDeviceModule的委托协议定义
@protocol PTDeviceModuleDelegate

- (void) onGetInfoByID: (u_int) uiDevID
            withResult: (int) iResult 
         andChannelLst: ( CChannelGetInfoList&) clChannelGetInfoLst 
              andDevIP: (NSString*) pclDevIP 
              andDevSN: (NSString*) pclDevSN;

- (void) onDevStatusNotify: (u_int) uiDevID 
                  onStatus: (_CM_DEVICE_STATUS_) eStatus;

- (void) onChannelNameChangeNotify: (const CChannelID&) clChannelID
                           newName: (NSString*) pclNewName;

- (void) onPTZCtrl: (const CChannelID&) clChannelID 
        withResult: (int) iResult
      withCtrlType: (int) iCtrlType 
     withIntensity: (int) iIntensity;

@end

////////////////////////////////////////////////////////////////
// 协议定义
@protocol PTDeviceModule<NSObject>

// 注册&反注册
- SYNCHRONISM (void) regDeviceModuleDelegate: (id<PTDeviceModuleDelegate>) idDelegate;
- SYNCHRONISM (void) unDeviceModuleDelegate: (id<PTDeviceModuleDelegate>) idDelegate;

- ASYNCHRONISM (int) getInfoById: (u_int) uiDevID;

- SYNCHRONISM (bool) isRemoteDevById: (u_int) uiDevID;

// 是否纳入公网并发控制，true为纳入公网控制
- SYNCHRONISM (bool) isUnderWCHCtrl: (u_int) uiDevID;

- ASYNCHRONISM (int) ptzCtrl: (const CChannelID&) clChannelID
                    ctrltype: (int) iCtrlType
                   intensity: (int) iIntensity;

@end

//  创建DeviceModule的类
@interface CMDeviceModuleCreator : NSObject

+ (BOOL) initializeCreator;
+ (void) unInitializeCreator;

// 创建DeviceModule实例
+ (id<PTDeviceModule>) allocedDeviceModule: (id<PTIndexServiceImpl>)idIndexServiceImpl
                         withLocDeviceImpl: (id<PTLocDeviceImpl>) idLocDeviceImpl;

// 销毁DeviceModule实例
+ (void) deallocDeviceModule: (id<PTDeviceModule>) idInstance;

@end    // CMDeviceModuleCreator

#endif
