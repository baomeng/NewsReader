//
//  CMAccountModuleProtocolDef.h
//  CMAccountModule
//
//  Created by cz l on 12-6-20.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#ifndef CMAccountModule_CMAccountModuleProtocolDef_h
#define CMAccountModule_CMAccountModuleProtocolDef_h

#import "KK_Includes.h"
#import "SystemErrorCodeDef.h"
#import "SystemResourceDef.h"
#import "CMNTDataTransferDef.h"
#import <UIKit/UIKit.h>

////////////////////////////////////////////////////////////////
//  委托协议定义
@protocol PTAccountModuleDelegate

- (void) onlogin: (int) iResult user: (NSString*) strUser;

- (void) onGetInfoByGroupID: (u_int) uiGroupID
                    withRlt: (int) iResult
              andDeviceInfo: (CGetInfoList&) clDeviceGetInfoLst
            andSubGroupInfo: (CGetInfoList&) clSubGroupGetInfoLst;

- (void) onlogout: (int) iErrCode;

- (BOOL) onReconnect: (u_long) ulReconCnt;

- (void) onDevNameChangeNotify: (u_int) uiDevid newName: (NSString*) strName;

- (void) onAddDevNotify: (u_int) uiDevId devname: (NSString*) strName groupId: (u_int) uiGrpId;

- (void) onDelDevNotify: (u_int) uiDevId groupId: (u_int) uiGrpId;

- (void) onPermChangeNotify: (bool) bCanMonitor andPTZCtrlEnable: (bool) bPTZCtrlEnable;

- (void) onSyncAccBalanceInfo: (u_long) ulCHCnt;

@end

////////////////////////////////////////////////////////////////
//  协议定义
@protocol PTAccountModule<NSObject>

// 注册&反注册
- SYNCHRONISM (void) regAccountModuleDelegate: (id<PTAccountModuleDelegate>) idDelegate;
- SYNCHRONISM (void) unAccountModuleDelegate: (id<PTAccountModuleDelegate>) idDelegate;

- ASYNCHRONISM (int) login: (NSString*) strSrvIp 
                      port: (u_short) usPort 
                      user: (NSString*) strUser 
                  password: (NSString*) strPassword;

- ASYNCHRONISM (int) getRootDevGroupInfo;

- ASYNCHRONISM (int) getDevInfoByGroupId: (u_int) uiGroupId;

- SYNCHRONISM (bool) isCanMonitor;

- SYNCHRONISM (bool) isPTZCtrlEnable;

- SYNCHRONISM (bool) isUpdateEnable;

- SYNCHRONISM (void) cancelLogin;

- SYNCHRONISM (bool) getFatherGroupId: (u_int) uiGroupId fatgerId: (u_int&) uiFatherId;

- SYNCHRONISM (bool) getGroupIdByDevId: (u_int) uiDevId groupId: (u_int&) uiGroupId;

- ASYNCHRONISM (u_long) getAccBalanceInfo;

@end


//  创建AccountModule的类
@interface CMAccountModuleCreator : NSObject

+ (BOOL) initializeCreator;
+ (void) unInitializeCreator;

// 创建AccountModule实例
+ (id<PTAccountModule>) allocedAccountModule: (id<PTIndexServiceImpl>)idIndexServiceImpl;

// 销毁AccountModule实例
+ (void) deallocAccountModule: (id<PTAccountModule>) idInstance;

@end    // CMAccountModuleCreator

#endif
