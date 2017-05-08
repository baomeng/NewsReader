//
//  CMMediaModuleProtocolDef.h
//  CMMediaModule
//
//  Created by Kai Liu on 12-6-7.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#ifndef CMMediaModule_CMMediaModuleProtocolDef_h
#define CMMediaModule_CMMediaModuleProtocolDef_h

#import "KK_Includes.h"
#import "SystemErrorCodeDef.h"
#import "SystemResourceDef.h"
#import <UIKit/UIKit.h>

////////////////////////////////////////////////////////////////
//  媒体相关的基本数据结构定义
const u_short MAX_FRAMERATE     = 30;       // 支持的最大帧率
const u_short MIN_FRAMERATE     = 1;        // 支持的最小帧率
const int     DEFAULT_FRAMERATE = 5;        // 用户设定默认帧率
const int     MIN_CIF_WIDTH     = 320;      // CIF宽
const int     MIN_CIF_HEIGHT    = 240;      // CIF高

enum _REAL_MONITOR_STATUS_
{
	MSTATUS_WAIT_INDEXSRV	= (0),          // 等待中心应答
	MSTATUS_WAIT_DEVICE		= (1),          // 等待设备应答
	MSTATUS_DEV_READY		= (2),          // 设备已就绪
	MSTATUS_BUFF_FRAME		= (3),          // 缓冲视频数据：wParam[缓冲进度]
	MSTATUS_SLOW_PLAY		= (4),          // 慢速播放：暂时不使用
	MSTATUS_NORMAL_PLAY		= (5),          // 正常播放
	MSTATUS_FAST_PLAY		= (6),          // 快速播放
    MSTATUS_PTZ_PLAY        = (7),          // 云台转动播放(只有关键帧，停止智能缓冲机制)
    MSTATUS_OVER_PLAY       = (8),          // 停止播放
};

enum _AUDIO_STATUS_
{
	ASTATUS_WAIT_INDEXSRV	= (0),          // 等待中心应答
	ASTATUS_WAIT_DEVICE		= (1),          // 等待设备应答
	ASTATUS_DEV_READY		= (2),          // 设备已就绪
	ASTATUS_NORMAL_PLAY		= (3),          // 正常播放
    ASTATUS_OVER_PLAY       = (4),          // 停止播放
};
////////////////////////////////////////////////////////////////
//  Static Func
_REAL_MONITOR_MODEL_ convertModelFromWH(int iWidth, int iHeight);

////////////////////////////////////////////////////////////////
//  MediaModule的委托协议定义
@protocol PTMediaModuleDelegate

- (BOOL) startRealMonitorRlt: (const CChannelID*) pclChannelID
                     withRlt: (int) iResult;

// 注释[2013.05.31] ：pclImage为共享的托管对象，使用完毕后，需要release
- (BOOL) drawChannelVideo: (const CChannelID*) pclChannelID
                withImage: (UIImage*) pclImage
               imageWidth: (int) iWidth
              imageHeight: (int) iHeight;

- (BOOL) changeRealMonitorModel: (const CChannelID*) pclChannelID
                      withModel: (_REAL_MONITOR_MODEL_) eRMModel
                  withFramerate: (int) iFramerate;

- (BOOL) notifyPlayStatus: (const CChannelID*) pclChannelID
             onPlayStatus: (_REAL_MONITOR_STATUS_) eRMStatus;

@end    // PTMediaModuleDelegate

////////////////////////////////////////////////////////////////
//  AudioModule的委托协议定义
@protocol PTAudioDelegate

- (BOOL) notifyListenStatus: (const CChannelID*) pclChannelID
             onListenStatus: (_AUDIO_STATUS_) audioStatus;

@end    // PTAudioDelegate

////////////////////////////////////////////////////////////////
//  MediaModule的协议定义
@protocol PTMediaModule<NSObject>

// 注册&反注册PTMediaModuleDelegate
- SYNCHRONISM (void) regMediaModuleDelegate: (id<PTMediaModuleDelegate>) idDelegate;
- SYNCHRONISM (void) unRegMediaModuleDelegate: (id<PTMediaModuleDelegate>) idDelegate;

- ASYNCHRONISM (int) startRealMonitor: (const CChannelID*) pclChannelID
                       withRemoteFlag: (bool) bRemoteFlag;

- SYNCHRONISM (int) pauseRealMonitor: (const CChannelID*) pclChannelID;
- SYNCHRONISM (int) resumeRealMonitor: (const CChannelID*) pclChannelID;
- SYNCHRONISM (int) stopRealMonitor: (const CChannelID*) pclChannelID;
- SYNCHRONISM (int) stopAllRealMonitor;

- SYNCHRONISM (BOOL) isRealMonitor: (const CChannelID*) pclChannelID;

- SYNCHRONISM (BOOL) getRealMonitorStatus: (const CChannelID*) pclChannelID
                           outputRMStatus: (_REAL_MONITOR_STATUS_&) eRMStatus;

- SYNCHRONISM (BOOL) setRealMonitorModel: (const CChannelID*) pclChannelID
                               withModel: (_REAL_MONITOR_MODEL_) eRMModel
                           withFramerate: (int) iFramerate;
@optional
// 注册&反注册PTAudioDelegate
- SYNCHRONISM (void) regAudioDelegate: (id<PTAudioDelegate>) idDelegate;
- SYNCHRONISM (void) unRegAudioDelegate: (id<PTAudioDelegate>) idDelegate;
- (int) startListen: (const CChannelID*) pclChannelID withRemoteFlag: (bool) bRemoteFlag;
- SYNCHRONISM (BOOL) stopListen: (u_longlong) ullChannelKey;
- SYNCHRONISM (u_longlong) listenningChannelKey;
- SYNCHRONISM (BOOL) isListenning;
- (BOOL) SYNCHRONISM startRecordering: (const CChannelID*) pclChannelID withOutPath:(NSString*)path withFPS:(int)fps ;
- SYNCHRONISM (BOOL) stopRecordering: (const CChannelID*) pclChannelID ;
- SYNCHRONISM (BOOL) isRecordering: (const CChannelID*) pclChannelID;
@end    // PTMediaModule

////////////////////////////////////////////////////////////////
//  创建MediaModule的类
@protocol PTIndexServiceImpl;
@protocol PTMediaServiceImpl;

@interface CMMediaModuleCreator : NSObject

+ (BOOL) initializeCreator;
+ (void) unInitializeCreator;

// 创建MediaModule实例
+ (id<PTMediaModule>) allocedMediaModule: (id<PTIndexServiceImpl>) idIndexService
                        withMediaService: (id<PTMediaServiceImpl>) idMediaService;

// 销毁MediaModule实例
+ (void) deallocMediaModule: (id<PTMediaModule>) idInstance;

@end    // CMMediaModuleCreator

////////////////////////////////////////////////////////////////
//  解决UIImage内存共享问题，增加UIImageEx类
@interface UIImageEx : UIImage
{
@private
    u_char* m_paucRGBA32Buf;
}

- (id) initWithCGImage: (CGImageRef) cgImage;

// paucRGBA32Buf，采用malloc分配内存，初始化结束后，将拖管给UIImageEx的对象
- (id) initWithCGImage: (CGImageRef) cgImage
         withRGBA32Buf: (u_char*&) paucRGBA32Buf;

- (void) dealloc;

@end

#endif
