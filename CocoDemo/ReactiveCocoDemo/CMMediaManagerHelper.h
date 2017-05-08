//
//  CMAccountManagerHelper.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SystemResourceDef.h"

@protocol CMMediaManagerHelper <NSObject>

- (void) handlerDrawChannelVideo:(id)arg;
- (void) handlerNotifyPlayStatus:(id)arg;
- (void) handlerStartRealMonitorRlt:(id)arg;
- (void) handlerChangeRealMonitorModel:(id)arg;

@end
