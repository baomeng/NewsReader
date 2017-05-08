//
//  CMModuleCreatorProtocolDef.h
//  CMModuleCreator
//
//  Created by Kai Liu on 12-6-6.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#ifndef CMModuleCreator_CMModuleCreatorProtocolDef_h
#define CMModuleCreator_CMModuleCreatorProtocolDef_h

#import "KK_Includes.h"
#import "CMAccountModuleProtocolDef.h"
#import "CMDeviceModuleProtocolDef.h"
#import "CMMediaModuleProtocolDef.h"

////////////////////////////////////////////////////////////////
//  CMModuleCreator的委托协议定义
@protocol PTModuleCreatorDelegate

@end    // PTModuleCreatorDelegate

////////////////////////////////////////////////////////////////
//  CMModuleCreator的协议定义
@protocol PTModuleCreator

- (id<PTAccountModule>) getAccountModule;
- (id<PTDeviceModule>) getDeviceModule;
- (id<PTMediaModule>) getMediaModule;

@end    // PTModuleCreator

////////////////////////////////////////////////////////////////
//  CMModuleCreatorSingleton : 创建维护CMModuleCreator的唯一实例
@interface CMModuleCreatorSingleton : NSObject

+ (BOOL) initializeModuleCreator;
+ (void) unInitializeModuleCreator;

+ (id<PTModuleCreator>) getModuleCreator;

@end    // CMNetDataTransferCreator

#endif
