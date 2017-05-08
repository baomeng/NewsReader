//
//  CMAccountManagerHelper.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol CMAccountManagerHelper <NSObject>

-(void)onGetDeviceTableCellInfo:(int)iResult parentId:(u_int)pId isChildrenZero:(BOOL)zero;

-(void)onlogin:(int)iResult user:(NSString*)strUser;
-(void)onLogout:(int)errorCode;
-(void)onReconnect:(int)count;
-(void)onDevNameChangeNotify:(u_int)uiDevid newName:(NSString*)strName needReload:(BOOL)isReload;
-(void)onAddDevNotify:(u_int)uiDevId devname:(NSString*)strName groupId:(u_int)uiGrpId needReload:(BOOL)isReload;
-(void)onDelDevNotify:(u_int)uiDevId groupId:(u_int)uiGrpId needReload:(BOOL)isReload;
-(void)onPermChangeNotify:(bool)bCanMonitor andPTZCtrlEnable: (bool) bPTZCtrlEnable;
-(void)onSyncAccBalanceInfo:(u_long)ulCHCnt;
@end
