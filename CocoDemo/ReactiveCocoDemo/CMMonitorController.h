//
//  CMMonitorController.h
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CMAccountManagerHelper.h"
#import "CMMediaManagerHelper.h"
#import "CMMediaModuleProtocolDef.h"
#import "PannelView.h"

@class AppDelegate;
@class CMDeviceTableCellInfo;
@class CMCenterManager;

@interface CMMonitorController : UIViewController  <UIAlertViewDelegate,CMAccountManagerHelper, CMMediaManagerHelper>
{
    AppDelegate *               appDelegate ;
    CMCenterManager             *centerManager;
    _REAL_MONITOR_STATUS_       currentStatus;
    BOOL                        isMutipleMonitorView;
    CGFloat                     fDefRMViewBorderWidth;
    UIImageView*                    recordingStatusImg;
    UILabel*                        recordingStatusLabel;
    NSTimer*                        recordingStatusTimer;
    NSDate*                         recordingStatusStartDate;
}
@property (assign, nonatomic) BOOL                          isfullView;
@property (assign, nonatomic) _REAL_MONITOR_STATUS_         currentStatus;
@property (copy,nonatomic) CMDeviceTableCellInfo            *curcellInfo;
@property (retain, nonatomic) IBOutlet UIImageView          *player;

@property (retain, nonatomic) IBOutlet  UIView              *fullContainer;
@property (retain, nonatomic) IBOutlet  UIView              *monitorContainer;
@property (retain, nonatomic) IBOutlet UIImageView          *leftTop;
@property (retain, nonatomic) IBOutlet UIImageView          *leftBottom;
@property (retain, nonatomic) IBOutlet UIImageView          *rightTop;
@property (retain, nonatomic) IBOutlet UIImageView          *rightBottom;

@property (retain, nonatomic) UILabel                  *infoText;
@property (retain, nonatomic) UIActivityIndicatorView  *progressInd;
@property (assign ,nonatomic) IBOutlet UIAlertView     *waittingAlert;

@property (retain, nonatomic) IBOutlet  UIView              *panelContainer;
@property (retain, nonatomic) IBOutlet UIImageView          *ptzBackground;

@property (retain, nonatomic) IBOutlet UILabel              *frameText;
@property (retain, nonatomic) IBOutlet UIButton             *radioLC;
@property (retain, nonatomic) IBOutlet UIButton             *radioHZ;

@property (retain, nonatomic) IBOutlet UIButton             *catchButton;
@property (retain, nonatomic) IBOutlet UIButton             *recordingButton;
@property (retain, nonatomic) IBOutlet UIButton             *listeningButton;

@property (retain, nonatomic) IBOutlet UIButton             *ptz_left;
@property (retain, nonatomic) IBOutlet UIButton             *ptz_top;
@property (retain, nonatomic) IBOutlet UIButton             *ptz_right;
@property (retain, nonatomic) IBOutlet UIButton             *ptz_bottom;

@property (retain, nonatomic) IBOutlet UIButton             *mMutipleBtn;
@property (retain, nonatomic) IBOutlet UIButton             *pMutipleBtn;

@property (retain, nonatomic) IBOutlet UIButton             *mFocusBtn;
@property (retain, nonatomic) IBOutlet UIButton             *pFocusBtn;

@property (retain, nonatomic) IBOutlet PannelView           *pPannelView;

-  (IBAction)catchImageButtonPressed:(id)sender;                //点击抓拍按钮
-  (IBAction)listenButtonPressed:(id)sender;                    //点击监听按钮
-  (IBAction)recorderButtonPressed:(id)sender;                  //点击录像按钮
-  (IBAction)mutipleScreenButtonPressed:(id)sender;             //点击分屏按钮

-  (IBAction)ptzButtonPressedDown:(id)sender;             //ptz控制按钮被按下
-  (IBAction)ptzButtonPressedUp:(id)sender;               //ptz控制按钮被抬起
-  (IBAction)radioButtonPressedUp:(id)sender;             //流畅，画质控制按钮被抬起
-  (IBAction)pButtonPressedDown:(id)sender;               //+  类按钮被控制按钮被按下
-  (IBAction)mButtonPressedDown:(id)sender;               // - 类按钮被控制按钮被按下
-  (IBAction)pButtonPressedUp:(id)sender;                 //+  类按钮被控制按钮被抬起
-  (IBAction)mButtonPressedUp:(id)sender;                 // - 类按钮被控制按钮被抬起

- (void) initMonitorViews;
- (void) updateNavigationBarContent;
- (void) tapOnMonitorView:(int)index;
- (void) showDeviceTableView;

-(void) showInfoText :(NSString *)info interval:(NSTimeInterval)timeInterval;
-(void) hideInfoText:(NSString *)info;
-(void) showWaiting;
-(void) hideWaiting;

-(void) catchImage;
-(void) listen;
-(void) record;

// panel接口
-(void)ptzControlStart:(int)controlType;
-(void)ptzControlStop:(int)controlType;
-(void)radioButtonUp:(int)controlType;
@end
