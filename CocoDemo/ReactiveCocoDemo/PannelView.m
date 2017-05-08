//
//  PannelView.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/24.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "PannelView.h"
#import "CMMonitorController.h"
#import "AppDelegate.h"
#import "CMCenterManager.h"
#import "CMMonitorController.h"

#define     RADIO_LC                        1
#define     RADIO_HZ                        2

@interface PannelView()

@property (weak, nonatomic) UILabel                      *infoText;

@property (weak, nonatomic) IBOutlet UIImageView          *panelBackground;

@property (weak, nonatomic) IBOutlet UIButton             *catchButton;
@property (weak, nonatomic) IBOutlet UIButton             *recordingButton;
@property (weak, nonatomic) IBOutlet UIButton             *listeningButton;

@property (weak, nonatomic) IBOutlet UIImageView          *frameBackground;
@property (weak, nonatomic) IBOutlet UILabel              *LabelFR;

@property (weak, nonatomic) IBOutlet UILabel              *LabelLC;
@property (weak, nonatomic) IBOutlet UILabel              *LabelHZ;
@property (weak, nonatomic) IBOutlet UIButton             *radioLC;
@property (weak, nonatomic) IBOutlet UIButton             *radioHZ;

@property (weak, nonatomic) IBOutlet UIImageView          *ptzBackground;
@property (weak, nonatomic) IBOutlet UIButton             *ptz_left;
@property (weak, nonatomic) IBOutlet UIButton             *ptz_top;
@property (weak, nonatomic) IBOutlet UIButton             *ptz_right;
@property (weak, nonatomic) IBOutlet UIButton             *ptz_bottom;

@property (weak, nonatomic) IBOutlet UIImageView          *bianbeiBackground;
@property (weak, nonatomic) IBOutlet UIImageView          *bianjiaoBackground;
@property (weak, nonatomic) IBOutlet UILabel              *LabelBB;
@property (weak, nonatomic) IBOutlet UILabel              *LabelBJ;
@property (weak, nonatomic) IBOutlet UIButton             *mutipleButton;

@property (retain, nonatomic) IBOutlet UIButton             *mFrameBtn;
@property (retain, nonatomic) IBOutlet UIButton             *pFrameBtn;
@property (retain, nonatomic) IBOutlet UIButton             *mMutipleBtn;
@property (retain, nonatomic) IBOutlet UIButton             *pMutipleBtn;
@property (retain, nonatomic) IBOutlet UIButton             *mFocusBtn;
@property (retain, nonatomic) IBOutlet UIButton             *pFocusBtn;

-(void) regmediahelper;
-(void) unregmediahelper;

@end

@implementation PannelView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

-(id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if(self){
        [self InitUI];
        
        AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
        centerManager = appDelegate.centerManager;
        monitorController = appDelegate.monitorController;
        
        [self regmediahelper];
    }
    return self;
}

-(void) regmediahelper{
    [centerManager regMediaManagerHelper:self];
}

-(void) unregmediahelper{
    [centerManager unregMediaManagerHelper:self];
}

- (void) handlerDrawChannelVideo:(id)arg{
    
}
- (void) handlerNotifyPlayStatus:(id)arg{
    
}
- (void) handlerStartRealMonitorRlt:(id)arg{
    
}
- (void) handlerChangeRealMonitorModel:(id)arg{
    
    NSArray *param = arg;
    _REAL_MONITOR_MODEL_ model = (_REAL_MONITOR_MODEL_)[[param objectAtIndex:1] intValue];
    int frame = [[param objectAtIndex:2] intValue];

    switch (model) {
        case RMMD_FLUENT:
            [self setRadioButtonImage:YES theRadioButton:self.radioLC];
            [self setRadioButtonImage:NO theRadioButton:self.radioHZ];
            break;
        case RMMD_LEGIBLE:
            [self setRadioButtonImage:YES theRadioButton:self.radioHZ];
            [self setRadioButtonImage:NO theRadioButton:self.radioLC];
            break;
        default:
            break;
    }
    
    NSString *temText = [NSString stringWithFormat:@"%dF",frame];
    self.LabelFR.text = temText;
}

-(void)InitUI{
    
    UIImageView* panelBackground = [[UIImageView alloc] init];
    self.panelBackground = panelBackground;
    NSString *pathPanel = [[NSBundle mainBundle] pathForResource:@"default_bg" ofType:@"png"];
    UIImage * backgroundPanel = [UIImage imageWithContentsOfFile:pathPanel];
    self.panelBackground.image = backgroundPanel;
    [self addSubview:panelBackground];
    
    UIButton* catchButton = [[UIButton alloc]init];
    [catchButton setBackgroundImage:[UIImage resizeImageWihtImageName:@"catch_normal"] forState:UIControlStateNormal];
    [catchButton addTarget:self action:@selector(catchImageButtonPressed) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:catchButton];
    self.catchButton = catchButton;
    
    UIButton* recordingButton = [[UIButton alloc]init];
    [recordingButton setBackgroundImage:[UIImage resizeImageWihtImageName:@"recording_normal"] forState:UIControlStateNormal];
    [recordingButton addTarget:self action:@selector(recorderButtonPressed) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:recordingButton];
    self.recordingButton = recordingButton;
    
    UIButton* listeningButton = [[UIButton alloc]init];
    [listeningButton setBackgroundImage:[UIImage resizeImageWihtImageName:@"listen_normal"] forState:UIControlStateNormal];
    [listeningButton addTarget:self action:@selector(listenButtonPressed) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:listeningButton];
    self.listeningButton = listeningButton;
    
    UIImageView* ptzBackground = [[UIImageView alloc] init];
    self.ptzBackground = ptzBackground;
    NSString *path = [[NSBundle mainBundle] pathForResource:@"pannel_control_background" ofType:@"png"];
    UIImage * background = [UIImage imageWithContentsOfFile:path];
    self.ptzBackground.image = background;
    [self addSubview:ptzBackground];
    
    UIButton* ptz_left = [[UIButton alloc]init];
    self.ptz_left = ptz_left;
    [ptz_left setBackgroundImage:[UIImage resizeImageWihtImageName:@"left_normal"] forState:UIControlStateNormal];
    [self addSubview:ptz_left];
    [self.ptz_left addTarget:self action:@selector(ptz_left_down)forControlEvents: UIControlEventTouchDown];
    [self.ptz_left addTarget:self action:@selector(ptz_left_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* ptz_top = [[UIButton alloc]init];
    self.ptz_top = ptz_top;
    [ptz_top setBackgroundImage:[UIImage resizeImageWihtImageName:@"up_normal"] forState:UIControlStateNormal];
    [self addSubview:ptz_top];
    [self.ptz_top addTarget:self action:@selector(ptz_top_down)forControlEvents: UIControlEventTouchDown];
    [self.ptz_top addTarget:self action:@selector(ptz_top_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* ptz_right = [[UIButton alloc]init];
    self.ptz_right = ptz_right;
    [ptz_right setBackgroundImage:[UIImage resizeImageWihtImageName:@"right_normal"] forState:UIControlStateNormal];
    [self addSubview:ptz_right];
    [self.ptz_right addTarget:self action:@selector(ptz_right_down)forControlEvents: UIControlEventTouchDown];
    [self.ptz_right addTarget:self action:@selector(ptz_right_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* ptz_bottom = [[UIButton alloc]init];
    self.ptz_bottom = ptz_bottom;
    [ptz_bottom setBackgroundImage:[UIImage resizeImageWihtImageName:@"down_normal"] forState:UIControlStateNormal];
    [self addSubview:ptz_bottom];
    [self.ptz_bottom addTarget:self action:@selector(ptz_down_down)forControlEvents: UIControlEventTouchDown];
    [self.ptz_bottom addTarget:self action:@selector(ptz_down_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* mutipleButton = [[UIButton alloc]init];
    self.mutipleButton = mutipleButton;
    [mutipleButton setBackgroundImage:[UIImage resizeImageWihtImageName:@"divider_screen_mormal"] forState:UIControlStateNormal];
    [self addSubview:mutipleButton];
    
    UIImageView* bianbeiBackground = [[UIImageView alloc] init];
    self.bianbeiBackground = bianbeiBackground;
    NSString *path1 = [[NSBundle mainBundle] pathForResource:@"mp_background" ofType:@"png"];
    UIImage * background1 = [UIImage imageWithContentsOfFile:path1];
    self.bianbeiBackground.image = background1;
    [self addSubview:bianbeiBackground];
    
    UIImageView* bianjiaoBackground = [[UIImageView alloc] init];
    self.bianjiaoBackground = bianjiaoBackground;
    NSString *path2 = [[NSBundle mainBundle] pathForResource:@"mp_background" ofType:@"png"];
    UIImage * background2 = [UIImage imageWithContentsOfFile:path2];
    self.bianjiaoBackground.image = background2;
    [self addSubview:bianjiaoBackground];
    
    UILabel* LabelBB = [[UILabel alloc] init];
    LabelBB.text = @"变倍";
    LabelBB.textColor = [UIColor whiteColor];
    LabelBB.font = [UIFont systemFontOfSize:10];
    self.LabelBB = LabelBB;
    [self addSubview:LabelBB];
    
    UILabel* LabelBJ = [[UILabel alloc] init];
    LabelBJ.text = @"变焦";
    LabelBJ.textColor = [UIColor whiteColor];
    LabelBJ.font = [UIFont systemFontOfSize:10];
    self.LabelBJ = LabelBJ;
    [self addSubview:LabelBJ];
    
    UIImageView* frameBackground = [[UIImageView alloc] init];
    self.frameBackground = frameBackground;
    NSString *path3 = [[NSBundle mainBundle] pathForResource:@"mp_background" ofType:@"png"];
    UIImage * background3 = [UIImage imageWithContentsOfFile:path3];
    self.frameBackground.image = background3;
    [self addSubview:frameBackground];
    
    UILabel* LabelFR = [[UILabel alloc] init];
    LabelFR.text = @"8F";
    LabelFR.textColor = [UIColor whiteColor];
    LabelFR.font = [UIFont systemFontOfSize:12];
    self.LabelFR = LabelFR;
    [self addSubview:LabelFR];
    
    UILabel* LabelLC = [[UILabel alloc] init];
    LabelLC.text = @"流畅优先";
    LabelLC.textColor = [UIColor whiteColor];
    LabelLC.font = [UIFont systemFontOfSize:12];
    self.LabelLC = LabelLC;
    [self addSubview:LabelLC];
    
    UILabel* LabelHZ = [[UILabel alloc] init];
    LabelHZ.font = [UIFont systemFontOfSize:12];
    LabelHZ.text = @"画质优先";
    LabelHZ.textColor = [UIColor whiteColor];
    self.LabelHZ = LabelHZ;
    [self addSubview:LabelHZ];
    
    UIButton* radioLC = [[UIButton alloc]init];
    self.radioLC = radioLC;
    [self addSubview:radioLC];
    [self.radioLC addTarget:self action:@selector(radio_LC_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* radioHZ = [[UIButton alloc]init];
    self.radioHZ = radioHZ;
    [self addSubview:radioHZ];
    [self.radioHZ addTarget:self action:@selector(radio_HZ_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    [self setRadioButtonImage:NO theRadioButton:self.radioLC];
    [self setRadioButtonImage:NO theRadioButton:self.radioHZ];
    
    UIButton* mFrameBtn = [[UIButton alloc]init];
    self.mFrameBtn = mFrameBtn;
    [mFrameBtn setBackgroundImage:[UIImage resizeImageWihtImageName:@"minus_normal"] forState:UIControlStateNormal];
    [self addSubview:mFrameBtn];
    [self.mFrameBtn addTarget:self action:@selector(mFrame_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* pFrameBtn = [[UIButton alloc]init];
    self.pFrameBtn = pFrameBtn;
    [pFrameBtn setBackgroundImage:[UIImage resizeImageWihtImageName:@"plus_normal"] forState:UIControlStateNormal];
    [self addSubview:pFrameBtn];
    [self.pFrameBtn addTarget:self action:@selector(pFrame_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* mMutipleBtn = [[UIButton alloc]init];
    self.mMutipleBtn = mMutipleBtn;
    [mMutipleBtn setBackgroundImage:[UIImage resizeImageWihtImageName:@"minus_normal"] forState:UIControlStateNormal];
    [self addSubview:mMutipleBtn];
    [self.mMutipleBtn addTarget:self action:@selector(mMutiple_down)forControlEvents: UIControlEventTouchDown];
    [self.mMutipleBtn addTarget:self action:@selector(mMutiple_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* pMutipleBtn = [[UIButton alloc]init];
    self.pMutipleBtn = pMutipleBtn;
    [pMutipleBtn setBackgroundImage:[UIImage resizeImageWihtImageName:@"plus_normal"] forState:UIControlStateNormal];
    [self addSubview:pMutipleBtn];
    [self.pMutipleBtn addTarget:self action:@selector(pMutiple_down)forControlEvents: UIControlEventTouchDown];
    [self.pMutipleBtn addTarget:self action:@selector(pMutiple_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
    
    UIButton* mFocusBtn = [[UIButton alloc]init];
    self.mFocusBtn = mFocusBtn;
    [mFocusBtn setBackgroundImage:[UIImage resizeImageWihtImageName:@"minus_normal"] forState:UIControlStateNormal];
    [self addSubview:mFocusBtn];
    [self.mFocusBtn addTarget:self action:@selector(mFocus_down)forControlEvents: UIControlEventTouchDown];
    [self.mFocusBtn addTarget:self action:@selector(mFocus_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];

    UIButton* pFocusBtn = [[UIButton alloc]init];
    self.pFocusBtn = pFocusBtn;
    [pFocusBtn setBackgroundImage:[UIImage resizeImageWihtImageName:@"plus_normal"] forState:UIControlStateNormal];
    [self addSubview:pFocusBtn];
    [self.pFocusBtn addTarget:self action:@selector(pFocus_down)forControlEvents: UIControlEventTouchDown];
    [self.pFocusBtn addTarget:self action:@selector(pFocus_up)forControlEvents: UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
}

-  (void)catchImageButtonPressed{

    if (!monitorController)
        return;
    
    [monitorController catchImage];
    
    NSLog(@"catchImageButtonPressed!");
}
-  (void)recorderButtonPressed{
    
    if (!monitorController)
        return;
    
    if (monitorController.curcellInfo==nil) {
        return;
    }
    
    CChannelID channelID;
    channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
    
    NSString *btn_img_path = nil;
    UIImage *backgroundImg=nil;
    NSString* imgName = nil;
    
    if ([centerManager isRealMonitor:&channelID]) {
        
        if (![centerManager isRecordering:&channelID]) {
            BOOL result = [centerManager startRecorderingWithOutPath:&channelID withOutPath:nil withFPS:10];
            if (result) {
                imgName = NSLocalizedString(@"recording_cancel_normal", nil);
                [self showRecordingStatus:YES];
            }else{
                NSString*    pclMessage = NSLocalizedString(@"录像失败.", nil);
                UIAlertView* alertView  = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"提示信息", nil)
                                                                     message:pclMessage
                                                                    delegate:nil
                                                           cancelButtonTitle:NSLocalizedString(@"确定", nil)
                                                           otherButtonTitles:nil, nil];
                [alertView show];
                //[alertView release];
            }
        }else{
            [centerManager stopRecordering:&channelID];
            imgName = NSLocalizedString(@"recording_normal", nil);
            [self showRecordingStatus:NO];
        }
        if (imgName!=nil) {
            btn_img_path = [[NSBundle mainBundle] pathForResource:imgName ofType:@"png"];
            backgroundImg = [UIImage imageWithContentsOfFile:btn_img_path];
            [_recordingButton setImage:backgroundImg forState:UIControlStateNormal];
            [_recordingButton setImage:backgroundImg forState:UIControlStateHighlighted];
            [_recordingButton setImage:backgroundImg forState:UIControlStateDisabled];
            [_recordingButton setImage:backgroundImg forState:UIControlStateSelected];
        }
    }

    NSLog(@"recorderButtonPressed!");
}

-(void)showRecordingStatus:(BOOL)isShow
{
    if (!monitorController)
        return;
    
    const int marginTop = 10;
    const int marginImgLeft = 5;
    const int marginLabelLeft = 10;
    const int imgSZ = 20;
    const int labelWidth = 150;
    const int labelHeight = 30;
    const int playerHeight = monitorController.player.frame.size.height;
    if (recordingStatusImg == nil) {
        recordingStatusImg = [[UIImageView alloc] initWithFrame:CGRectMake(marginImgLeft, playerHeight - marginTop - imgSZ, imgSZ, imgSZ)];
        recordingStatusImg.image = [UIImage imageNamed:@"recording_status"];
        [monitorController.player addSubview:recordingStatusImg];
        //[recordingStatusImg release];
    }
    if (recordingStatusLabel == nil) {
        recordingStatusLabel = [[UILabel alloc] initWithFrame:CGRectMake(marginImgLeft+imgSZ+marginLabelLeft, playerHeight - marginTop - labelHeight, labelWidth, labelHeight)];
        recordingStatusLabel.backgroundColor = [UIColor clearColor];
        recordingStatusLabel.textColor = [UIColor whiteColor];
        [monitorController.player addSubview:recordingStatusLabel];
        //[recordingStatusLabel release];
        CGFloat centerY = recordingStatusImg.center.y;
        recordingStatusLabel.center=CGPointMake(recordingStatusLabel.center.x,centerY);
    }
    if (isShow) {
        recordingStatusImg.hidden = NO;
        recordingStatusLabel.hidden = NO;
        if (recordingStatusTimer==nil) {
            recordingStatusTimer = [NSTimer scheduledTimerWithTimeInterval:0
                                                                    target:self
                                                                  selector:@selector(updateRecordingStatusTimer:)
                                                                  userInfo:nil
                                                                   repeats:YES];
            //[recordingStatusTimer retain];
           // if (recordingStatusStartDate != nil) {
                //[recordingStatusStartDate release];
           // }
            recordingStatusStartDate = [NSDate date];
            [recordingStatusStartDate copy];
        }
    }else{
        recordingStatusImg.hidden = YES;
        recordingStatusLabel.hidden = YES;
        if (recordingStatusTimer!=nil) {
            [recordingStatusTimer invalidate];
            //[recordingStatusTimer release];
            recordingStatusTimer = nil;
        }
    }
}
- (void)updateRecordingStatusTimer:(NSTimer *)theTimer
{
    NSDate* currentDate = [NSDate date];
    NSCalendar* cal = [NSCalendar currentCalendar];
    unsigned int unitFlags = NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit;
    NSDateComponents *d = [cal components:unitFlags fromDate:recordingStatusStartDate toDate:currentDate options:0];
    int hour = [d hour];
    int minute = [d minute];
    int second = [d second];
    NSString* hourFormatString = nil;
    NSString* minuteFormatString = nil;
    NSString* secondFormatString = nil;
    if (hour<10) {
        hourFormatString = @"0%d";
    }else{
        hourFormatString = @"%d";
    }
    if (minute<10) {
        minuteFormatString = @"0%d";
    }else{
        minuteFormatString = @"%d";
    }
    if (second<10) {
        secondFormatString = @"0%d";
    }else{
        secondFormatString = @"%d";
    }
    NSString* formatString = [NSString stringWithFormat:@"%@:%@:%@",hourFormatString,minuteFormatString,secondFormatString ];
    NSString* info = [NSString stringWithFormat:formatString, hour, minute ,second ];
    recordingStatusLabel.text = info;
}

-  (void)listenButtonPressed{

    if (!monitorController)
        return;
    
    //[controller listen];
    
    if (monitorController.curcellInfo==nil) {
        return;
    }
    
    CChannelID channelID;
    channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
    
    NSString *path = nil;
    UIImage *backgroundImg=nil;
    NSString* imgName = nil;
    if ([centerManager isListenning]) {
        
        u_longlong currentListenningKey = [centerManager listenningChannelKey];
        if (currentListenningKey>0) {
            [centerManager stopListenWithChannelKey:currentListenningKey];
        }
        imgName = NSLocalizedString(@"listen_normal", nil);
        
    }
    else
    {
        if ([centerManager isRealMonitor:&channelID]) {
            BOOL bRemoteDevice = true;
            BOOL result = [centerManager startListen:&channelID withRemoteFlag:bRemoteDevice];
            if (result) {
                imgName = NSLocalizedString(@"listen_cancel_normal", nil);
            }else{
                NSString*    pclMessage = NSLocalizedString(@"发起监听失败", nil);
                UIAlertView* alertView  = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"提示信息", nil)
                                                                     message:pclMessage
                                                                    delegate:nil
                                                           cancelButtonTitle:NSLocalizedString(@"确定", nil)
                                                           otherButtonTitles:nil, nil];
                [alertView show];
                //[alertView release];
            }
        }
    }
    if (imgName!=nil) {
        path = [[NSBundle mainBundle] pathForResource:imgName ofType:@"png"];
        backgroundImg = [UIImage imageWithContentsOfFile:path];
        [_listeningButton setImage:backgroundImg forState:UIControlStateNormal];
        [_listeningButton setImage:backgroundImg forState:UIControlStateHighlighted];
        [_listeningButton setImage:backgroundImg forState:UIControlStateDisabled];
        [_listeningButton setImage:backgroundImg forState:UIControlStateSelected];
    }

    NSLog(@"listenButtonPressed!");
}

-(void) ptz_top_down{
    
    if (!monitorController)
        return;
    [monitorController ptzControlStart:1];
    
    NSLog(@"ptz_top_down!");
}
-(void) ptz_top_up{

    if (!monitorController)
        return;
    [monitorController ptzControlStop:1];
    
    NSLog(@"ptz_top_up!");
}

-(void) ptz_down_down{

    if (!monitorController)
        return;
    [monitorController ptzControlStart:2];
    
    NSLog(@"ptz_down_down!");
}
-(void) ptz_down_up{

    if (!monitorController)
        return;
    [monitorController ptzControlStop:2];
    
    NSLog(@"ptz_down_up!");
}

-(void) ptz_left_down{

    if (!monitorController)
        return;
    [monitorController ptzControlStart:3];
    
    NSLog(@"ptz_left_down!");
}
-(void) ptz_left_up{
    if (!monitorController)
        return;
    [monitorController ptzControlStop:3];
    
    NSLog(@"ptz_left_up!");
}

-(void) ptz_right_down{
    if (!monitorController)
        return;
    [monitorController ptzControlStart:4];
    
    NSLog(@"ptz_right_down!");
}
-(void) ptz_right_up{
    if (!monitorController)
        return;
    [monitorController ptzControlStop:4];
    
    NSLog(@"ptz_right_up!");
}

-(void) radio_LC_up{
    [self radioButtonUp:RADIO_LC];
    
    NSLog(@"radio_LC_up!");
}

-(void) radio_HZ_up{
    [self radioButtonUp:RADIO_HZ];
    
    NSLog(@"radio_HZ_up!");
}

-(void)radioButtonUp:(int)controlType{
    
    if (!monitorController)
        return;
    
    CChannelID channelID;
    if (monitorController.curcellInfo!=nil) {
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        switch (controlType) {
            case RADIO_LC:
            {
                if([centerManager setRealMonitorModel:RMMD_FLUENT channelId:channelID])
                {
                    [self setRadioButtonImage:YES theRadioButton:self.radioLC];
                    [self setRadioButtonImage:NO theRadioButton:self.radioHZ];
                }
                break;
            }
            case RADIO_HZ:
            {
                if([centerManager setRealMonitorModel:RMMD_LEGIBLE channelId:channelID])
                {
                    [self setRadioButtonImage:NO theRadioButton:self.radioLC];
                    [self setRadioButtonImage:YES theRadioButton:self.radioHZ];
                }
                break;
            }
            default:
                break;
        }
    }
    
}

- (void) setRadioButtonImage:(BOOL)isSelected theRadioButton:(UIButton *)button
{
    NSString *pathDefault ,*pathHighlighted, *pathSelected;
    UIImage *defaultBackground, *highlightedBackground, *selectedBackground;
    if (isSelected) {
        pathDefault = [[NSBundle mainBundle] pathForResource:@"btn_radio_on" ofType:@"png"];
        defaultBackground = [UIImage imageWithContentsOfFile:pathDefault];
        
        pathHighlighted = [[NSBundle mainBundle] pathForResource:@"btn_radio_on_pressed" ofType:@"png"];
        highlightedBackground = [UIImage imageWithContentsOfFile:pathHighlighted];
        
        pathSelected = [[NSBundle mainBundle] pathForResource:@"btn_radio_on_selected" ofType:@"png"];
        selectedBackground = [UIImage imageWithContentsOfFile:pathSelected];
    }else{
        pathDefault = [[NSBundle mainBundle] pathForResource:@"btn_radio_off" ofType:@"png"];
        defaultBackground = [UIImage imageWithContentsOfFile:pathDefault];
        
        pathHighlighted = [[NSBundle mainBundle] pathForResource:@"btn_radio_off_pressed" ofType:@"png"];
        highlightedBackground = [UIImage imageWithContentsOfFile:pathHighlighted];
        
        pathSelected = [[NSBundle mainBundle] pathForResource:@"btn_radio_off_selected" ofType:@"png"];
        selectedBackground = [UIImage imageWithContentsOfFile:pathSelected];
    }
    [button setImage:defaultBackground forState:UIControlStateNormal];
    [button  setImage:highlightedBackground forState:UIControlStateHighlighted];
    [button  setImage:selectedBackground forState:UIControlStateSelected];
}

-(void)mFrame_up{
    
    if (!monitorController)
        return;
    
    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID( monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        int currentFrameRate = centerManager.currentFrameRate;
        if ([centerManager setRealMonitorFrame:(currentFrameRate-1) channelId:channelID]) {
            NSString *temText = [NSString stringWithFormat:@"%dF",(currentFrameRate-1)];
            self.LabelFR.text = temText;
        }
    }
    
    NSLog(@"mFrame_up!");
}

-(void)pFrame_up{
    if (!monitorController)
        return;
    
    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        int currentFrameRate = centerManager.currentFrameRate;
        if ([centerManager setRealMonitorFrame:(currentFrameRate+1) channelId:channelID]) {
            NSString *temText = [NSString stringWithFormat:@"%dF",(currentFrameRate+1)];
            self.LabelFR.text = temText;
        }
    }
    NSLog(@"pFrame_up!");
}

-(void)mMutiple_down{
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_ZOOMSUB intensity:0 channelId:channelID Status:currentStatus];
    }
    
    NSLog(@"mMutiple_down!");
}

-(void)mMutiple_up{
    
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_ZOOMSUB_STOP intensity:0 channelId:channelID Status:currentStatus];
    }
    
    NSLog(@"mMutiple_up!");
}

-(void)pMutiple_down{
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_ZOOMADD intensity:0 channelId:channelID Status:currentStatus];
    }
    NSLog(@"pMutiple_down!");
}

-(void)pMutiple_up{
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_ZOOMADD_STOP intensity:0 channelId:channelID Status:currentStatus];
    }
    NSLog(@"pMutiple_up!");
}
-(void)mFocus_down{
    
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_FOCUSSUB intensity:0 channelId:channelID Status:currentStatus];
    }
    NSLog(@"mFocus_down!");
}
-(void)mFocus_up{
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_FOCUSSUB_STOP intensity:0 channelId:channelID Status:currentStatus];
    }
    NSLog(@"mFocus_up!");
}
-(void)pFocus_down{
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_FOCUSADD intensity:0 channelId:channelID Status:currentStatus];
    }
    NSLog(@"pFocus_down!");
}
-(void)pFocus_up{
    if (!monitorController)
        return;

    if (monitorController.curcellInfo!=nil) {
        CChannelID channelID;
        channelID.SetChannelID(monitorController.curcellInfo.parent_id, monitorController.curcellInfo.self_id);
        [centerManager controlPTZ:PTZ_CTL_FOCUSADD_STOP intensity:0 channelId:channelID Status:currentStatus];
    }
    NSLog(@"pFocus_up!");
}

-(void)layoutSubviews
{
    [super layoutSubviews];
    
    self.panelBackground.width = self.frame.size.width;
    self.panelBackground.height = self.frame.size.height;
    self.panelBackground.x = 0;
    self.panelBackground.y = 0;
    
    self.frameBackground.width = 80;
    self.frameBackground.height = 25;
    self.frameBackground.x = (self.width - self.frameBackground.width) / 2;
    self.frameBackground.y = 5;
    
    self.mFrameBtn.width = 28;
    self.mFrameBtn.height = 25;
    self.mFrameBtn.x = self.frameBackground.x;
    self.mFrameBtn.y = self.frameBackground.y;
    
    self.LabelFR.width = 24;
    self.LabelFR.height = 20;
    self.LabelFR.x = self.frameBackground.x + self.mFrameBtn.width + 5;
    self.LabelFR.y = self.frameBackground.y;
    
    self.pFrameBtn.width = 28;
    self.pFrameBtn.height = 25;
    self.pFrameBtn.x = self.frameBackground.x + self.mFrameBtn.width + self.LabelFR.width;
    self.pFrameBtn.y = self.frameBackground.y;
    
    self.radioLC.width = 30;
    self.radioLC.height = 30;
    self.radioLC.x = 10;
    self.radioLC.y = 5;

    self.LabelLC.width = 80;
    self.LabelLC.height = 30;
    self.LabelLC.x = self.radioLC.x + self.radioLC.width + 5;
    self.LabelLC.y = self.radioLC.y;
    
    self.radioHZ.width = 30;
    self.radioHZ.height = 30;
    self.radioHZ.x = self.bianbeiBackground.x;
    self.radioHZ.y = 5;
    
    self.LabelHZ.width = 80;
    self.LabelHZ.height = 30;
    self.LabelHZ.x = self.radioHZ.x + self.radioHZ.width + 5;
    self.LabelHZ.y = self.frameBackground.y;
    
    self.ptzBackground.width = 90;
    self.ptzBackground.height = 90;
    self.ptzBackground.x = (self.width - self.ptzBackground.width) / 2;
    self.ptzBackground.y = self.frameBackground.y + self.frameBackground.height + 7.5;
    
    self.ptz_left.width = 30;
    self.ptz_left.height = 30;
    self.ptz_left.x = self.ptzBackground.x;
    self.ptz_left.y = self.ptzBackground.y + self.ptz_top.height;
    
    self.ptz_top.width = 30;
    self.ptz_top.height = 30;
    self.ptz_top.x = self.ptzBackground.x+self.ptz_top.width;
    self.ptz_top.y = self.ptzBackground.y;
    
    self.ptz_right.width = 30;
    self.ptz_right.height = 30;
    self.ptz_right.x = self.ptzBackground.x + 2 * self.ptz_right.width;
    self.ptz_right.y = self.ptzBackground.y + self.ptz_right.height;

    self.ptz_bottom.width = 30;
    self.ptz_bottom.height = 30;
    self.ptz_bottom.x = self.ptzBackground.x + self.ptz_bottom.width;
    self.ptz_bottom.y = self.ptzBackground.y + 2 * self.ptz_bottom.height;
    
    self.catchButton.width = 80;
    self.catchButton.height = 25;
    self.catchButton.x = 15;
    self.catchButton.y = self.ptzBackground.y;
    
    self.recordingButton.width = 80;
    self.recordingButton.height = 25;
    self.recordingButton.x = 15;
    self.recordingButton.y = self.catchButton.y + self.catchButton.height + 7.5;
    
    self.listeningButton.width = 80;
    self.listeningButton.height = 25;
    self.listeningButton.x = 15;
    self.listeningButton.y = self. recordingButton.y + self.recordingButton.height + 7.5;
    
    self.bianbeiBackground.width = 80;
    self.bianbeiBackground.height = 25;
    self.bianbeiBackground.x = self.frame.size.width - 15 - self.bianbeiBackground.width;
    self.bianbeiBackground.y = self.ptzBackground.y;
    
    self.mMutipleBtn.width = 28;
    self.mMutipleBtn.height = 25;
    self.mMutipleBtn.x = self.bianbeiBackground.x;
    self.mMutipleBtn.y = self.bianbeiBackground.y;
    
    self.LabelBB.width = 25;
    self.LabelBB.height = 20;
    self.LabelBB.x = self.bianbeiBackground.x + self.mMutipleBtn.width + 2;
    self.LabelBB.y = self.bianbeiBackground.y + 2;
    
    self.pMutipleBtn.width = 28;
    self.pMutipleBtn.height = 25;
    self.pMutipleBtn.x = self.bianbeiBackground.x + self.mMutipleBtn.width + self.LabelBB.width;
    self.pMutipleBtn.y = self.bianbeiBackground.y;

    self.bianjiaoBackground.width = 80;
    self.bianjiaoBackground.height = 25;
    self.bianjiaoBackground.x = self.frame.size.width - 15 - self.bianjiaoBackground.width;
    self.bianjiaoBackground.y = self.recordingButton.y;
    
    self.mFocusBtn.width = 28;
    self.mFocusBtn.height = 25;
    self.mFocusBtn.x = self.bianjiaoBackground.x;
    self.mFocusBtn.y = self.bianjiaoBackground.y;
    
    self.LabelBJ.width = 25;
    self.LabelBJ.height = 20;
    self.LabelBJ.x = self.bianjiaoBackground.x + self.mFocusBtn.width + 2;
    self.LabelBJ.y = self.bianjiaoBackground.y + 2;
    
    self.pFocusBtn.width = 28;
    self.pFocusBtn.height = 25;
    self.pFocusBtn.x = self.bianjiaoBackground.x + self.mFocusBtn.width + self.LabelBJ.width;
    self.pFocusBtn.y = self.bianjiaoBackground.y;
    
    self.mutipleButton.width = 80;
    self.mutipleButton.height = 25;
    self.mutipleButton.x = self.frame.size.width - 15 - self.mutipleButton.width;
    self.mutipleButton.y = self.listeningButton.y;
}

@end
