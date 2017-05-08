//
//  CMCenterManager.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CMAccountModuleProtocolDef.h"
#import "CMDeviceModuleProtocolDef.h"
#import "CMMediaModuleProtocolDef.h"
#import "CMAccountManagerHelper.h"
#import "CMDeviceManagerHelper.h"
#import "CMDeviceTableCellInfo.h"

#import "CMMediaManagerHelper.h"

typedef void (^loginBlock)(int, NSString*);

@interface CMCenterManager : NSObject <PTAccountModuleDelegate,PTDeviceModuleDelegate, PTMediaModuleDelegate, PTAudioDelegate>

@property (assign, nonatomic) id<PTAccountModule>               idAccountModule;
@property (assign, nonatomic) id<PTDeviceModule>                idDeviceModule;
@property (assign, nonatomic) id<PTMediaModule>                 idMediaModule;

@property (retain, nonatomic) NSString                          *serverIp;
@property (assign, nonatomic) u_short                           serverPort;
@property (retain, nonatomic) NSString                          *userName;
@property (retain, nonatomic) NSString                          *password;
//@property (retain, nonatomic) NSMutableDictionary               *recorder;
@property (retain, nonatomic) NSMutableArray                    *accountHelpers;
@property (retain, nonatomic) NSMutableArray                    *mediaHelpers;
@property (retain, nonatomic) NSMutableArray                    *deviceHelpers;
@property (retain, atomic) NSMutableArray                    *currentCellList;
@property (retain, atomic) NSMutableArray                    *currentChannelList;
@property (retain, atomic) NSMutableArray                    *videoList;
@property (assign, atomic) int                               currentGroupId;
@property (assign, atomic) int                               currentGroupParentId;
@property (assign, atomic) u_longlong                               requestGroupId;
@property (assign, atomic) u_longlong                               requestDeviceId;

@property (assign, nonatomic)       _REAL_MONITOR_MODEL_            currentMonitorModel;
@property (assign, nonatomic)       int                             currentFrameRate;

- (int) getDeviceTableCellInfoById:(u_int) theId;
- (int) getDeviceTableCellChildrenInfo: (CMDeviceTableCellInfo *)theCell;

- (void) handlerOnLogin:(id)arg;
- (void) handlerOnLogout:(id)arg;
- (void) handlerOnReconnect:(id)arg;
- (void) handlerOnGetGroupInfo:(id)arg;
- (void) handlerOnGetDeviceInfo:(id)arg;
- (void) handlerOnDevStatusNotify:(id)arg;
- (void) handlerOnDevNameChangeNotify:(id)arg;
- (void) handlerOnChannelNameChangeNotify:(id)arg;
- (void) handlerOnAddDevNotify:(id)arg;
- (void) handlerOnDelDevNotify:(id)arg;

- (void) handlerDrawChannelVideo:(id)arg;
- (void) handlerNotifyPlayStatus:(id)arg;

- (int)login:(NSString*) strSrvIp port:(u_short) usPort user:(NSString*)strUser password:(NSString*)strPassword withBlock:(loginBlock)block;

- (bool)isRealMonitor:(CChannelID*)channelID;
- (bool)isCanMonitor;
- (bool)isPtzEnable;

- (void) regMediaManagerHelper: (id<CMMediaManagerHelper>) helper;
- (void) unregMediaManagerHelper: (id<CMMediaManagerHelper>) helper;

-  (void) regAccountHelper: (id<CMAccountManagerHelper>) helper;
-  (void) unregAccountHelper: (id<CMAccountManagerHelper>) helper;

-  (void) regDeviceHelper: (id<CMDeviceManagerHelper>) helper;
-  (void) unregDeviceHelper: (id<CMDeviceManagerHelper>) helper;

//一些帮助函数
-(BOOL) setCellName:(NSString *)newName cellId:(int*)cId type:(DEVICE_TYPE)cellType;             //设置当前显示列表某一项 的名称,其中cId为设备本身id与父亲id组成的数组，第一位为自身id,第二位可无（仅当类型为通道时有数据）
-(BOOL) setCellStatus:(_CM_DEVICE_STATUS_)eStatus cellId:(int*)cId type:(DEVICE_TYPE)cellType;   //设置当前显示列表某一项的状态
-(CMDeviceTableCellInfo *) findCellInCurrentListById:(int*)cId type:(DEVICE_TYPE)cellType;
-(void) showInfoText :(NSString *)info interval:(NSTimeInterval)timeInterval;

- (BOOL) addNewDevice:(NSString *) devName selfId:(int)devId parentId:(int)groupId;
- (BOOL) delDevice:(int)devId parentId:(int)groupId;
- (void) cancelLogin;
- (void) cancelRequestDevInfo;
- (BOOL) isWaitingResponse;
- (void) getRootDevGroupInfo;

- (int)playMonitorWithChannelInfo:(CMDeviceTableCellInfo *)info isRemoteDevice:(BOOL)bRemoteFlag;
//- (void) playMonitorWithChannelInfo:(CChannelID*)clChannelID isRemoteDevice:(BOOL)bRemoteFlag;
- (void) stopMonitorWithChannelInfo:(CChannelID*)clChannelID ;
- (void) stopAllMonitor;

- (u_longlong) listenningChannelKey;
- (BOOL) isListenning;
- (BOOL) stopListen:(CChannelID *)channelID;
- (int)  startListen: (const CChannelID*) pclChannelID withRemoteFlag: (bool) bRemoteFlag;
- (BOOL) stopListenWithChannelKey:(u_longlong)channelKey;

- (BOOL) isRecordering:(CChannelID *)channelID;
- (BOOL) startRecorderingWithOutPath:(CChannelID*)channelID withOutPath:(NSString*)path withFPS:(int)fps;
- (BOOL) stopRecordering:(CChannelID*)channelID;

- (BOOL) setRealMonitorModel:(_REAL_MONITOR_MODEL_) eRMModel channelId:(CChannelID&)channelID;

- (void) controlPTZ :(_PTZ_CTL_TYPE_)ptzType intensity:(int)iIntensity channelId:(CChannelID&)channelID Status:(_REAL_MONITOR_STATUS_)currentStatus;
- (BOOL) setRealMonitorFrame:(int) iFramerate channelId:(CChannelID&)channelID;

- (NSMutableArray *)videoLists;

@property (nonatomic,copy)loginBlock logincallBackBlock;

-(void) getDevLst;

@end
