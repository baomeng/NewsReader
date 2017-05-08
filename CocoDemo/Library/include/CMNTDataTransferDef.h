//
//  CMNTDataTransferDef.h
//  CMNetDataTransfer
//
//  Created by Kai Liu on 12-6-6.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#ifndef CMNetDataTransfer_CMNTDataTransferDef_h
#define CMNetDataTransfer_CMNTDataTransferDef_h

#import "KK_Includes.h"
#import "SystemErrorCodeDef.h"

////////////////////////////////////////////////////////////////
//  各个Session中，各个Module固有的EVENTID区间
//     1、CMAccountModule    : 1~99
//     2、CMDeviceModule     : 101~199
//     3、CMMediaModule      : 201~299
//     4、CMNetDataTransfer  : 301~399
//     5、TempEvent          : 401+
enum _MODUEL_EVENTID_BASE_
{
    MCE_ACCOUNT_EVENTID_BASE = (0),
    MCE_DEVICE_EVENTID_BASE  = (100),
    MCE_MEDIA_EVENTID_BASE   = (200),
    MCE_NET_EVENTID_BASE     = (300),
    MCE_TEMP_EVENTID_BASE    = (400)
};

////////////////////////////////////////////////////////////////
//  常量定义
const int MAX_THREAD_COUNT  = 5;    // 监看最大并发数+1
const int PDU_HEAD_LEN      = 4;
const int PDU_BASE_LEN      = 6;

////////////////////////////////////////////////////////////////
//  中心和媒体通用的常量定义
const u_long CLIENT_HEARTBEAT_EVENT_ID = MCE_NET_EVENTID_BASE + 1;
const u_long CLIENT_RECONNECT_EVENT_ID = MCE_NET_EVENTID_BASE + 2;
const u_long CLIENT_HEARTBEAT_INTERVAL = 2000;      // ms
const u_long CLIENT_UNRECV_TIME        = 60000;     // ms
const u_long CLIENT_UNSEND_TIME        = 60000;     // ms
const u_long RECONNECT_INTERVAL        = 1000;      // ms
const u_long RECONNECT_MAX_CNT         = 10;

////////////////////////////////////////////////////////////////
//  媒体Session的Event定义
const u_long MDS_CONNECT_ERROR_EVENT_ID = MCE_MEDIA_EVENTID_BASE + 1;
const u_long MDS_UNRECV_TIME            = 30000;     // ms
const u_long MDS_UNSEND_TIME            = 30000;     // ms

////////////////////////////////////////////////////////////////
//  CMPDUData
@interface CMPDUData : NSObject
{
@private
    const u_char* m_paucData;
    u_short       m_usDataLen;
    bool          m_bCopyFlag;
}

@property(nonatomic, assign) const u_char* pduDataBuf;
@property(nonatomic, assign) u_short dataLen;

- (id) init;
- (id) initWithDataBuf: (const u_char*) paucDataBuf
           withDataLen: (u_long) ulDataLen
          withCopyFlag: (bool) bCopyFlag;

- (void) dealloc;

@end    // CMPDUData

////////////////////////////////////////////////////////////////
//  NetDataTransfer的协议定义
@protocol PTIndexServiceImpl;
@protocol PTMediaServiceImpl;
@protocol PTLocDeviceImpl;
@protocol PTNetDataTransfer<NSObject>

- SYNCHRONISM (BOOL) startNetDataTransfer: (int) iThreadCount;
- SYNCHRONISM (BOOL) stopNetDataTransfer;

- SYNCHRONISM (id<PTIndexServiceImpl>) getIndexService;
- SYNCHRONISM (id<PTMediaServiceImpl>) getMediaService;
- SYNCHRONISM (id<PTLocDeviceImpl>)    getLocDeviceService;

@end    // PTNetDataTransfer

////////////////////////////////////////////////////////////////
//  PTIndexServiceImpl
@protocol PTIndexServiceDelegate;
@protocol PTIndexServiceImpl

- SYNCHRONISM (BOOL) addDelegate: (id<PTIndexServiceDelegate>) idDelegate;
- SYNCHRONISM (BOOL) delDelegate: (id<PTIndexServiceDelegate>) idDelegate;
 
- ASYNCHRONISM (BOOL) connectToIndexSrv: (NSString*) pclHost
                               withPort: (int) iPort
                              withDelay: (u_long) ulDelay;

- ASYNCHRONISM (BOOL) disconnectFromIndexSrv;

////////////////////////////////////////////////////////////////
//  注释[2012.06.25] : 
//      1、PDU处理方法(selHandleFun)的参数为 : 
//          id idPDUData : CMPDUData*
- SYNCHRONISM (BOOL) regHandlePDU: (u_short) usModuleID
                        withMDPDU: (u_short) usMDPDU
                       withObject: (id) idHandler
                     withSelecter: (SEL) selHandleFun;

- SYNCHRONISM (BOOL) unregHandlePDU: (u_short) usModuleID
                          withMDPDU: (u_short) usMDPDU;

- SYNCHRONISM (BOOL) unregAllHandlePDU;

- SYNCHRONISM (BOOL) sendData: (u_short) usModuleID
                  withDataLen: (u_short) usDataLen
                  withDataBuf: (u_char*) puacBuf;

- SYNCHRONISM (u_long) getTempEventID;

- SYNCHRONISM (BOOL) setTimer: (u_long) ulEventID
                    withAfter: (u_long) ulMilliseconds
                     withData: (u_longlong) ullData
                   withObject: (id) idObject
                  withOneShot: (BOOL) bOneShot;

- SYNCHRONISM (BOOL) killTimer: (u_long) ulEventID;

- SYNCHRONISM (BOOL) getLocalAddress: (struct sockaddr_in*) pstLocalAddress;

@end    // PTIndexServiceImpl

////////////////////////////////////////////////////////////////
//  PTMediaSessionMgr
@protocol PTMediaSessionMgrDelegate;
@protocol PTMediaSessionMgr<NSObject>

- ASYNCHRONISM (BOOL) connectToMediaSrv: (NSString*) pclHost
                               withPort: (int) iPort
                              withDelay: (u_long) ulDelay;

- ASYNCHRONISM (BOOL) disconnectFromMediaSrv;

- SYNCHRONISM (BOOL) regHandlePDU: (u_short) usMediaPDU
                       withObject: (id) idHandler
                     withSelecter: (SEL) selHandleFun;

- SYNCHRONISM (BOOL) unregHandlePDU: (u_short) usMediaPDU;
- SYNCHRONISM (BOOL) unregAllHandlePDU;

- SYNCHRONISM (BOOL) sendData: (u_short) usMediaPDU
                  withDataLen: (u_short) usDataLen
                  withDataBuf: (u_char*) puacBuf;

- SYNCHRONISM (u_long) getTempEventID;

- SYNCHRONISM (BOOL) setTimer: (u_long) ulEventID
                    withAfter: (u_long) ulMilliseconds
                     withData: (u_longlong) ullData
                   withObject: (id) idObject
                  withOneShot: (BOOL) bOneShot;

- SYNCHRONISM (BOOL) killTimer: (u_long) ulEventID;

@end    // PTMediaSessionMgr

////////////////////////////////////////////////////////////////
//  PTMediaServiceImpl
@protocol PTMediaServiceImpl<NSObject>

- SYNCHRONISM (id<PTMediaSessionMgr>) allocMediaSessionMgr: (id<PTMediaSessionMgrDelegate>) idDelegate;

- SYNCHRONISM (void) deallocMediaSessionMgr: (id) idSessionMgr;

@end    // PTMediaServiceImpl

////////////////////////////////////////////////////////////////
//  PTLocDeviceImpl
@protocol PTLocDeviceDelegate;
@protocol PTLocDeviceImpl

- SYNCHRONISM (BOOL) addLocDeviceDelegate: (id<PTLocDeviceDelegate>) idDelegate;
- SYNCHRONISM (BOOL) delLocDeviceDelegate: (id<PTLocDeviceDelegate>) idDelegate;

@end    // PTLocDeviceImpl

////////////////////////////////////////////////////////////////
//  NetDataTransfer的委托协议定义
@protocol PTIndexServiceDelegate

- (BOOL) onConnectToIndexSrv: (int) iResult;
- (BOOL) onDisconnectFromIndexSrv: (int) iErrorCode;

- (BOOL) onTimer: (u_long) ulEventID
        withData: (u_longlong) ullData
      withObject: (id) idObject;

- (BOOL) onReconnect: (u_long) ulReconCnt;

@end    // PTIndexServiceDelegate

////////////////////////////////////////////////////////////////
//  PTMediaSessionMgrDelegate
@protocol PTMediaSessionMgrDelegate<NSObject>

- (BOOL) onConnectToMediaSrv: (int) iResult;

- (BOOL) onTimer: (u_long) ulEventID
        withData: (u_longlong) ullData
      withObject: (id) idObject;

@end    // PTMediaServiceDelegate

////////////////////////////////////////////////////////////////
//  PTLocDeviceDelegate
@protocol PTLocDeviceDelegate

- (BOOL) clearOldLocalDeviceInfo;
- (BOOL) updateLocalDeviceInfo: (NSString*) pclDeviceSN;

@end    // PTLocDeviceDelegate

////////////////////////////////////////////////////////////////
//  创建NetDataTransfer的类
@interface CMNetDataTransferCreator : NSObject

+ (BOOL) initializeCreator;
+ (void) unInitializeCreator;

// 创建NetDataTransfer实例
+ (id<PTNetDataTransfer>) allocedNetDataTransfer;

// 销毁NetDataTransfer实例
+ (void) deallocNetDataTransfer: (id<PTNetDataTransfer>) idInstance;

@end    // CMNetDataTransferCreator

#endif
