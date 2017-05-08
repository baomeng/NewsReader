//
//  PannelView.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/24.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CMMediaManagerHelper.h"
#import "CMMediaModuleProtocolDef.h"

@class CMCenterManager;
@class CMMonitorController;
@interface PannelView : UIView <CMMediaManagerHelper>{
    
    CMCenterManager             *centerManager;
    CMMonitorController         *monitorController;
    _REAL_MONITOR_STATUS_       currentStatus;
    
    UIImageView*                recordingStatusImg;
    UILabel*                    recordingStatusLabel;
    NSTimer*                    recordingStatusTimer;
    NSDate*                     recordingStatusStartDate;
    
}

@end
