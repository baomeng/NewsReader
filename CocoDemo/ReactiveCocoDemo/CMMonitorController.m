//
//  CMMonitorController.m
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "CMMonitorController.h"
#import "CMCenterManager.h"
#import "AppDelegate.h"
#import "CMPlayerData.h"
#import "SystemResourceDef.h"
#import "Masonry.h"

#define     MAX_SCREEN_COUNT                4

#define     PTZ_UP                          1
#define     PTZ_DOWN                        2
#define     PTZ_LEFT                        3
#define     PTZ_RIGHT                       4

#define     RADIO_LC                        1
#define     RADIO_HZ                        2

#define     MP_FRAME                        1
#define     MP_MULTIPLE                     2
#define     MP_FOCUS                        3

#define     ALERTVIEW_ACC_CHANGED_TAG       2
#define     ALERTVIEW_PERM_CHANGED_TAG      3

#define     MAX_SCREEN_COUNT                4

#define     PHOTOS_DIRECTORY                (@"catchAlbum")

@interface CMMonitorController ()

@end

@implementation CMMonitorController

@synthesize leftTop, leftBottom, monitorContainer, panelContainer;
@synthesize curcellInfo;
@synthesize player;
@synthesize catchButton,recordingButton,listeningButton;
@synthesize ptz_bottom,ptz_left,ptz_right,ptz_top,mFocusBtn,mMutipleBtn,pMutipleBtn,pFocusBtn;
@synthesize ptzBackground,frameText,radioHZ,radioLC;
@synthesize currentStatus;
@synthesize infoText,progressInd,waittingAlert;
@synthesize pPannelView;
@synthesize isfullView;
@synthesize fullContainer;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    //InterLog(@"hello, memory leak hapened in monitor View!");
    // Release any cached data, images, etc that aren't in use.
}

-(void)radioButtonUp:(int)controlType{
    
    CChannelID channelID;
    if (curcellInfo!=nil) {
        channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
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

-(void)ptzControlStart:(int)controlType
{
    CChannelID channelID;
    
    switch (controlType) {
        case PTZ_UP:
        {
            if (curcellInfo!=nil) {

                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_UP intensity:0 channelId:channelID Status:currentStatus];
            }
            NSString *path = [[NSBundle mainBundle] pathForResource:@"ptz_up_pressed_background" ofType:@"png"];
            UIImage * background = [UIImage imageWithContentsOfFile:path];
            self.ptzBackground.image = background;
            break;
        }
        case PTZ_DOWN:
        {
            if (curcellInfo!=nil) {

                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_DOWN intensity:0 channelId:channelID Status:currentStatus];
            }
            NSString *path = [[NSBundle mainBundle] pathForResource:@"ptz_down_pressed_background" ofType:@"png"];
            UIImage * background = [UIImage imageWithContentsOfFile:path];
            self.ptzBackground.image = background;
            
            break;
        }
        case PTZ_LEFT:
        {
            if (curcellInfo!=nil) {

                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_LEFT intensity:0 channelId:channelID Status:currentStatus];
            }
            NSString *path = [[NSBundle mainBundle] pathForResource:@"ptz_left_pressed_background" ofType:@"png"];
            UIImage * background = [UIImage imageWithContentsOfFile:path];
            self.ptzBackground.image = background;
            
            break;
        }
        case PTZ_RIGHT:
        {
            if (curcellInfo!=nil) {

                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_RIGHT intensity:0 channelId:channelID Status:currentStatus];
            }
            NSString *path = [[NSBundle mainBundle] pathForResource:@"ptz_right_pressed_background" ofType:@"png"];
            UIImage * background = [UIImage imageWithContentsOfFile:path];
            self.ptzBackground.image = background;
            
            break;
        }
        default:
            break;
    }
}

-(void)ptzControlStop:(int)controlType
{
    CChannelID channelID;
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"pannel_control_background" ofType:@"png"];
    UIImage * background = [UIImage imageWithContentsOfFile:path];
    self.ptzBackground.image = background;
    switch (controlType) {
        case PTZ_UP:
        {
            if (curcellInfo!=nil) {
                
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_UP_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        case PTZ_DOWN:
        {
            if (curcellInfo!=nil) {

                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_DOWN_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            
            break;
        }
        case PTZ_LEFT:
        {
            if (curcellInfo!=nil) {
                
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_LEFT_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            
            break;
        }
        case PTZ_RIGHT:
        {
            if (curcellInfo!=nil) {
                
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_RIGHT_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            
            break;
        }
        default:
            break;
    }
}

#pragma mark - View lifecycle

-(void) dealloc{
    
    if (self.ptz_left!=nil) {
        [self.ptz_left release];
    }
    if (self.ptz_bottom!=nil) {
        [self.ptz_bottom release];
    }
    if (self.ptz_right!=nil) {
        [self.ptz_right release];
    }
    if (self.ptz_top!=nil) {
        [self.ptz_top release];
    }
    if (self.mMutipleBtn!=nil) {
        [self.mMutipleBtn release];
    }
    if (self.mFocusBtn!=nil) {
        [self.mFocusBtn release];
    }
    if (self.pMutipleBtn!=nil) {
        [self.pMutipleBtn release];
    }
    if (self.pFocusBtn!=nil) {
        [self.pFocusBtn release];
    }
    
    if (player!=nil) {
        [player release];
    }

    if (curcellInfo!=nil) {
        [curcellInfo release];
    }
    
    if (self.catchButton!=nil) {
        [self.catchButton release];
    }
    if (self.recordingButton!=nil) {
        [self.recordingButton release];
    }
    if (self.listeningButton!=nil) {
        [self.listeningButton release];
    }
    if (self.ptzBackground!=nil) {
        [self.ptzBackground release];
    }
    if (self.frameText!=nil) {
        [self.frameText release];
    }
    if (self.radioLC!=nil) {
        [self.radioLC release];
    }
    if (self.radioHZ!=nil) {
        [self.radioHZ release];
    }
    
    if(infoText!=nil){
        [infoText release];
    }
    if(progressInd!=nil){
        [progressInd release];
    }
    if(panelContainer!=nil){
        [panelContainer release];
    }

    if(pPannelView!=nil){
        [pPannelView release];
    }
    
    if(fullContainer!=nil){
        [fullContainer release];
    }
    [super dealloc];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    self.player = nil;
    self.curcellInfo = nil;
    self.waittingAlert = nil;
    
    self.infoText=nil;
    self.progressInd = nil;

    self.catchButton = nil;
    self.recordingButton = nil;
    self.listeningButton = nil;
    
    self.ptz_bottom = nil;
    self.ptz_left = nil;
    self.ptz_right = nil;
    self.ptz_top = nil;
    self.mFocusBtn = nil;
    self.mMutipleBtn = nil;
    self.pFocusBtn = nil;
    self.pMutipleBtn = nil;
    
    self.ptzBackground = nil;
    self.frameText = nil;
    self.radioHZ = nil;
    self.radioLC = nil;
    
    self.panelContainer = nil;

    self.pPannelView = nil;
    self.fullContainer = nil;
    
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    CMDeviceTableCellInfo* info = [appDelegate.playerData GetCurCellInfo];
    curcellInfo = info;
    
    [centerManager regAccountHelper:self];
    [centerManager regMediaManagerHelper:self];
    
    [centerManager playMonitorWithChannelInfo:curcellInfo isRemoteDevice:true];

    if (curcellInfo!=nil) {
        [self.player.layer setBorderColor:[UIColor blueColor].CGColor];
        self.navigationItem.title = curcellInfo.name;
    }else{
        self.navigationItem.title = @"";
    }

    [self updateNavigationBarContent];
    self.navigationController.navigationBarHidden = NO;
}

-(void)updateNavigationBarContent
{
    if (self.navigationItem.leftBarButtonItem==nil) {
        UIBarButtonItem *leftbtn = [[UIBarButtonItem alloc]initWithTitle:NSLocalizedString(@"返回", nil)
                                                                   style:UIBarButtonItemStylePlain
                                                                  target:self
                                                                  action:@selector(backPressed:)];
        self.navigationItem.leftBarButtonItem = leftbtn;
        [leftbtn release];
    }
}

-  (IBAction)backPressed:(id)sender
{
    CChannelID channelID;
    channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
    [centerManager stopMonitorWithChannelInfo:&channelID];

    [self.navigationController popViewControllerAnimated:YES];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [centerManager unregAccountHelper:self];
    [centerManager unregMediaManagerHelper:self];
    
    //
    [pPannelView unregmediahelper];
    
    [super viewDidDisappear:animated];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [self.navigationController.navigationBar setTranslucent:NO];
    
    appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    centerManager = appDelegate.centerManager;

    [self initMonitorViews];
    
    isfullView = false;
}

- (void) initMonitorViews
{
    fullContainer = [[UIView alloc] init];
    [self.view addSubview:fullContainer];
    
    player = [[UIImageView alloc] init];
    [self.view addSubview:self.player];
    
    self.player.userInteractionEnabled     = YES;
    [self.player.layer setMasksToBounds:YES];
    [self.player.layer setBorderColor:[UIColor blackColor].CGColor];
    [self.player.layer setBorderWidth:0.5f];
    
    //monitorContainer = [[UIView alloc]init];
    //[self.view addSubview:monitorContainer];
//    [self.monitorContainer.layer setMasksToBounds:YES];
//    [self.monitorContainer.layer setBorderColor:[UIColor grayColor].CGColor];
//    [self.monitorContainer.layer setBorderWidth:2.0f];
    
    // 单击
    UITapGestureRecognizer *singleTapLP = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnLeftTop)];
    [self.player addGestureRecognizer:singleTapLP];
    [singleTapLP release];
    
    // 双击
    singleTapLP = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleClickOnLeftTop)];
    singleTapLP.numberOfTapsRequired = 2;
    [self.player addGestureRecognizer:singleTapLP];
    [singleTapLP release];
    
    //监控画面接受双手捏合手势
    UIPinchGestureRecognizer *pinchGesture;
    pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchOnLT:)];
    [self.player addGestureRecognizer:pinchGesture];
    [pinchGesture release];
    
    /*添加拖动手势*/
    UIPanGestureRecognizer *panGesture=[[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panOnLT:)];
    [player addGestureRecognizer:panGesture];
    
    //监控画面接受单手滑动手势四个方向
    UISwipeGestureRecognizer *swipeGesture;
    swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeOnMonitorView:)];
    [swipeGesture setDirection:(UISwipeGestureRecognizerDirectionRight)];
    [self.player addGestureRecognizer:swipeGesture];
    [swipeGesture release];
    
    swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeOnMonitorView:)];
    [swipeGesture setDirection:(UISwipeGestureRecognizerDirectionUp)];
    [self.player addGestureRecognizer:swipeGesture];
    [swipeGesture release];
    
    swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeOnMonitorView:)];
    [swipeGesture setDirection:(UISwipeGestureRecognizerDirectionDown)];
    [self.player addGestureRecognizer:swipeGesture];
    [swipeGesture release];
    
    swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeOnMonitorView:)];
    [swipeGesture setDirection:(UISwipeGestureRecognizerDirectionLeft)];
    [self.player addGestureRecognizer:swipeGesture];
    [swipeGesture release];

    
    pPannelView = [[PannelView alloc] init];
    [self.view addSubview:self.pPannelView];
    
    player.translatesAutoresizingMaskIntoConstraints = NO;
    pPannelView.translatesAutoresizingMaskIntoConstraints = NO;
    
    //__weak __typeof(self) weakSelf = self;//这里用一个弱引用来表示self，用于下面的Block中
    [fullContainer mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.view.mas_top).with.offset(0);
        make.left.equalTo(self.view.mas_left).with.offset(0);
        make.right.equalTo(self.view.mas_right).with.offset(0);
        make.bottom.equalTo(self.view.mas_bottom).with.offset(0);
    }];
    
/*    [monitorContainer mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.view.mas_top).with.offset(0);
        make.left.equalTo(self.view.mas_left).with.offset(0);
        make.right.equalTo(self.view.mas_right).with.offset(0);
        make.height.equalTo(self.view.mas_height).multipliedBy(0.65);
    }];
   */
    [player mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.view.mas_top).with.offset(0);
        make.left.equalTo(self.view.mas_left).with.offset(0);
        make.right.equalTo(self.view.mas_right).with.offset(0);
        make.height.equalTo(self.view.mas_height).multipliedBy(0.65);
    }];
    [pPannelView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(player.mas_bottom).with.offset(0);
        make.left.equalTo(self.view.mas_left).with.offset(0);
        make.right.equalTo(self.view.mas_right).with.offset(0);
        make.height.equalTo(self.view.mas_height).multipliedBy(0.35);
    }];
    
    [self.player setHidden:NO];
    [self.pPannelView setHidden:NO];
}

- (void) tapOnLeftTop
{
    [self tapOnMonitorView:0];
}
-(void) tapOnMonitorView:(int)index{

    if (curcellInfo!=nil) {
        //[self.player.layer setBorderColor:[UIColor redColor].CGColor];
        self.navigationItem.title = curcellInfo.name;
    }
}

-(void) doubleClickOnLeftTop{
    
    if(!isfullView){
        [self.player setHidden:NO];
        [self.pPannelView setHidden:YES];
        
        [self.player setFrame:self.fullContainer.frame];
        [self.player.layer setBorderColor:[UIColor blueColor].CGColor];
        [self.player.layer setBorderWidth:1.0f];
        [self.player setNeedsDisplay];
        
        isfullView = true;
    }else{
        
        [self.player setHidden:NO];
        [self.pPannelView setHidden:NO];
        
        CGRect defaultViewLeftTopFrame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height*13/20);
        [self.player setFrame:defaultViewLeftTopFrame];
        [self.player.layer setBorderColor:[UIColor blueColor].CGColor];
        [self.player.layer setBorderWidth:1.0f];
        [self.player setNeedsDisplay];
        
        isfullView = false;
    }

    
    return;
}

- (void) pinchOnLT:(UIPinchGestureRecognizer *)pinch
{
/*    if (pinch.state == UIGestureRecognizerStateEnded) {
        if ((pinch.scale >=1.4 && isMutipleMonitorView) || (pinch.scale <=0.6 && !isMutipleMonitorView)) {
            [self doubleClickOnLeftTop];
        }
    }
*/
    if (pinch.state==UIGestureRecognizerStateChanged) {
        //捏合手势中scale属性记录的缩放比例
        player.transform = CGAffineTransformMakeScale(pinch.scale, pinch.scale);
        //_imageView.transform=CGAffineTransformMakeScale(gesture.scale, gesture.scale);
    }else if(pinch.state==UIGestureRecognizerStateEnded){//结束后恢复
        [UIView animateWithDuration:.5 animations:^{
            player.transform=CGAffineTransformIdentity;//取消一切形变
        }];
    }
}

-(void)panOnLT:(UIPanGestureRecognizer *)gesture{
/*    if (gesture.state==UIGestureRecognizerStateChanged) {
        CGPoint translation=[gesture translationInView:self.view];//利用拖动手势的translationInView:方法取得在相对指定视图（这里是控制器根视图）的移动
        player.transform=CGAffineTransformMakeTranslation(translation.x, translation.y);
    }else if(gesture.state==UIGestureRecognizerStateEnded){
        [UIView animateWithDuration:0.5 animations:^{
            player.transform=CGAffineTransformIdentity;
        }];
    }
 */
}

- (void) swipeOnMonitorView:(UISwipeGestureRecognizer *)swipeGesture
{
    if (swipeGesture.state == UIGestureRecognizerStateEnded) {
        switch (swipeGesture.direction) {
            case UISwipeGestureRecognizerDirectionRight:{
                [self ptzControlStart:PTZ_RIGHT];
                [self ptzControlStop:PTZ_RIGHT];
                break;
            }
            case UISwipeGestureRecognizerDirectionLeft:{
                [self ptzControlStart:PTZ_LEFT];
                [self ptzControlStop:PTZ_LEFT];
                break;
            }
            case UISwipeGestureRecognizerDirectionUp:{
                [self ptzControlStart:PTZ_UP];
                [self ptzControlStop:PTZ_UP];
                break;
            }
            case UISwipeGestureRecognizerDirectionDown:{
                [self ptzControlStart:PTZ_DOWN];
                [self ptzControlStop:PTZ_DOWN];
                break;
            }
            default:
                break;
        }
    }
}

-  (IBAction)catchImageButtonPressed:(id)sender
{
    [self catchImage];
}

-(void) catchImage{
    if (player.image!=nil) {
        
        UIImageWriteToSavedPhotosAlbum(player.image, self, @selector(catchImageCompleteDelegate:didFinishSavingWithError:contextInfo:), player);
    }
}

- (void) catchImageCompleteDelegate: (UIImage *) image didFinishSavingWithError: (NSError *) error contextInfo: (void *) contextInfo
{
    if (error.localizedDescription!=nil) {
        
        [self showInfoText:error.localizedDescription interval:2];
        InterLog(@"error string is %@",error.localizedDescription);
    }else{
        //CMMonitorPlayer *player = (CMMonitorPlayer *)contextInfo;
        [self showInfoText:NSLocalizedString(@"抓拍成功,图片已存入相册.", nil) interval:2];
    }
}

-  (IBAction)listenButtonPressed:(id)sender
{
    [self listen];
}

-(void) listen{
    if (curcellInfo==nil) {
        return;
    }
    
    CChannelID channelID;
    channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
    
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
                [alertView release];
            }
        }
    }
    if (imgName!=nil) {
        path = [[NSBundle mainBundle] pathForResource:imgName ofType:@"png"];
        backgroundImg = [UIImage imageWithContentsOfFile:path];
        [listeningButton setImage:backgroundImg forState:UIControlStateNormal];
        [listeningButton setImage:backgroundImg forState:UIControlStateHighlighted];
        [listeningButton setImage:backgroundImg forState:UIControlStateDisabled];
        [listeningButton setImage:backgroundImg forState:UIControlStateSelected];
    }
    
}

-  (IBAction)recorderButtonPressed:(id)sender
{
    [self record];
}

-(void) record{
    if (curcellInfo==nil) {
        return;
    }
    
    CChannelID channelID;
    channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
    
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
                [alertView release];
            }
        }else{
            [centerManager stopRecordering:&channelID];
            imgName = NSLocalizedString(@"recording_normal", nil);
            [self showRecordingStatus:NO];
        }
        if (imgName!=nil) {
            btn_img_path = [[NSBundle mainBundle] pathForResource:imgName ofType:@"png"];
            backgroundImg = [UIImage imageWithContentsOfFile:btn_img_path];
            [recordingButton setImage:backgroundImg forState:UIControlStateNormal];
            [recordingButton setImage:backgroundImg forState:UIControlStateHighlighted];
            [recordingButton setImage:backgroundImg forState:UIControlStateDisabled];
            [recordingButton setImage:backgroundImg forState:UIControlStateSelected];
        }
    }
    
}

-(void)showRecordingStatus:(BOOL)isShow
{
    const int marginTop = 10;
    const int marginImgLeft = 5;
    const int marginLabelLeft = 10;
    const int imgSZ = 20;
    const int labelWidth = 150;
    const int labelHeight = 30;
    const int playerHeight = self.player.frame.size.height;
    if (recordingStatusImg == nil) {
        recordingStatusImg = [[UIImageView alloc] initWithFrame:CGRectMake(marginImgLeft, playerHeight - marginTop - imgSZ, imgSZ, imgSZ)];
        recordingStatusImg.image = [UIImage imageNamed:@"recording_status"];
        [self.player addSubview:recordingStatusImg];
        [recordingStatusImg release];
    }
    if (recordingStatusLabel == nil) {
        recordingStatusLabel = [[UILabel alloc] initWithFrame:CGRectMake(marginImgLeft+imgSZ+marginLabelLeft, playerHeight - marginTop - labelHeight, labelWidth, labelHeight)];
        recordingStatusLabel.backgroundColor = [UIColor clearColor];
        recordingStatusLabel.textColor = [UIColor whiteColor];
        [self.player addSubview:recordingStatusLabel];
        [recordingStatusLabel release];
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
            [recordingStatusTimer retain];
            if (recordingStatusStartDate != nil) {
                [recordingStatusStartDate release];
            }
            recordingStatusStartDate = [NSDate date];
            [recordingStatusStartDate retain];
        }
    }else{
        recordingStatusImg.hidden = YES;
        recordingStatusLabel.hidden = YES;
        if (recordingStatusTimer!=nil) {
            [recordingStatusTimer invalidate];
            [recordingStatusTimer release];
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

-  (IBAction)mutipleScreenButtonPressed:(id)sender
{
    
}

-  (IBAction)ptzButtonPressedDown:(id)sender
{
    UIButton *btnPressed = sender;
    [self ptzControlStart:btnPressed.tag];
}

- (IBAction)ptzButtonPressedUp:(id)sender
{
    UIButton *btnPressed = sender;
    [self ptzControlStop:btnPressed.tag];
    
}

-  (IBAction)radioButtonPressedUp:(id)sender               //流畅，画质控制按钮被抬起
{
    UIButton *btnPressed = sender;
    [self radioButtonUp:btnPressed.tag];
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


-  (IBAction)pButtonPressedDown:(id)sender
{
    UIButton *btnPressed = sender;
    //CMPlayerData *data = [playerDataCenter currentPlayerData];
    switch (btnPressed.tag) {
        case MP_FRAME:
        {
            break;
        }
        case MP_MULTIPLE:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_ZOOMADD intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        case MP_FOCUS:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_FOCUSADD intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        default:
            break;
    }
}
-  (IBAction)mButtonPressedDown:(id)sender
{
    //CMPlayerData *data = [playerDataCenter currentPlayerData];
    UIButton *btnPressed = sender;
    switch (btnPressed.tag) {
        case MP_FRAME:
        {
            break;
        }
        case MP_MULTIPLE:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_ZOOMSUB intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        case MP_FOCUS:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_FOCUSSUB intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        default:
            break;
    }
}

-  (IBAction)pButtonPressedUp:(id)sender                 //+  类按钮被控制按钮被抬起
{
    UIButton *btnPressed = sender;
    //CMPlayerData *data = [playerDataCenter currentPlayerData];
    switch (btnPressed.tag) {
        case MP_FRAME:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                int currentFrameRate = centerManager.currentFrameRate;
                if ([centerManager setRealMonitorFrame:(currentFrameRate+1) channelId:channelID]) {
                    NSString *temText = [NSString stringWithFormat:@"%dF",(currentFrameRate+1)];
                    self.frameText.text = temText;
                }
            }
            break;
        }
        case MP_MULTIPLE:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_ZOOMADD_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        case MP_FOCUS:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_FOCUSADD_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        default:
            break;
    }
}

-  (IBAction)mButtonPressedUp:(id)sender                 // - 类按钮被控制按钮被抬起
{
    //CMPlayerData *data = [playerDataCenter currentPlayerData];
    UIButton *btnPressed = sender;
    switch (btnPressed.tag) {
        case MP_FRAME:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                int currentFrameRate = centerManager.currentFrameRate;
                if ([centerManager setRealMonitorFrame:(currentFrameRate-1) channelId:channelID]) {
                    NSString *temText = [NSString stringWithFormat:@"%dF",(currentFrameRate-1)];
                    self.frameText.text = temText;
                }
            }
            break;
        }
        case MP_MULTIPLE:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_ZOOMSUB_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        case MP_FOCUS:
        {
            if (curcellInfo!=nil) {
                CChannelID channelID;
                channelID.SetChannelID(curcellInfo.parent_id, curcellInfo.self_id);
                [centerManager controlPTZ:PTZ_CTL_FOCUSSUB_STOP intensity:0 channelId:channelID Status:currentStatus];
            }
            break;
        }
        default:
            break;
    }
}

//CMCallBackHelper
- (void) handlerChangeRealMonitorModel:(id)arg
{
    NSArray *param = arg;
    _REAL_MONITOR_MODEL_ model = (_REAL_MONITOR_MODEL_)[[param objectAtIndex:1] intValue];
    int frame = [[param objectAtIndex:2] intValue];
    //CMMonitorController * controller = context;
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
    self.frameText.text = temText;
}

- (void) handlerStartRealMonitorRlt:(id)arg
{
    NSArray *param = arg;
    NSNumber *theKey = [param objectAtIndex:0];
    NSString *strResult = [param objectAtIndex:1];
    int iResult = [strResult intValue];
    if (iResult!=CMED_OPERATE_SUCCESS) {
        NSString* pclMessage = [[NSString alloc] initWithString:NSLocalizedString(@"因为网络原因监看失败,请重新尝试。", nil)];
        
        //[self setBackground];
        [self hideWaiting];
        [self hideInfoText:self.infoText.text];
        [self showInfoText:pclMessage interval:0];
        
    }else{
        self.player.image = nil;
        [self updateNavigationBarContent];
    }
    
}
- (void) handlerDrawChannelVideo:(id)arg
{
    UIImage *img = arg;
    self.player.image = img;
    [img release];
}

- (void) handlerNotifyPlayStatus:(id)arg
{
    NSNumber *param  = arg;
    int tem = [param intValue];
    _REAL_MONITOR_STATUS_ eRMStatus = (_REAL_MONITOR_STATUS_)tem;
    currentStatus = eRMStatus;
    switch (eRMStatus) {
        case MSTATUS_WAIT_INDEXSRV:
        {
            self.player.image = nil;
            [self showWaiting];
            [self showInfoText:NSLocalizedString(@"等待中心应答", nil) interval:0];
            break;
        }
        case MSTATUS_WAIT_DEVICE:
        {
            [self showWaiting];
            [self showInfoText:NSLocalizedString(@"等待设备应答", nil) interval:0];
            break;
        }
        case MSTATUS_DEV_READY:
        {
            [self showWaiting];
            [self showInfoText:NSLocalizedString(@"设备已就绪", nil) interval:0];
            break;
        }
        case MSTATUS_BUFF_FRAME:
        {
//               [self showWaiting];
//               [self showInfoText:NSLocalizedString(@"缓冲视频数据", nil) interval:0];
            break;
        }
        case MSTATUS_SLOW_PLAY:
        {
            [self hideWaiting];
            [self hideInfoText:self.infoText.text];
            break;
        }
        case MSTATUS_NORMAL_PLAY:
        {
            [self hideWaiting];
            [self hideInfoText:self.infoText.text];
            break;
        }
        case MSTATUS_FAST_PLAY:
        {
            [self hideWaiting];
            [self hideInfoText:self.infoText.text];
            break;
        }
        default:
            break;
    }
}

-(void)hideWaiting
{
    if (![self.progressInd isDescendantOfView:self.player]) {
        [self.player addSubview:progressInd];
    }
    if ([progressInd isAnimating]) {
        [progressInd stopAnimating];
    }
}

-(void)showWaiting {
    
    if (progressInd==nil) {
        UIActivityIndicatorView *temIndicator = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:
                                                 UIActivityIndicatorViewStyleWhite];
        self.progressInd= temIndicator;
        [temIndicator release];
        CGFloat centerX = self.player.frame.size.width/2;
        CGFloat centerY = self.player.frame.size.height/2;
        self.progressInd.center=CGPointMake(centerX,centerY);
        
    }else{
        CGFloat centerX = self.player.frame.size.width/2;
        CGFloat centerY = self.player.frame.size.height/2;
        self.progressInd.center=CGPointMake(centerX,centerY);
    }
    if (![self.progressInd isDescendantOfView:self.player]) {
        [self.player addSubview:progressInd];
    }
    if (![progressInd isAnimating]) {
        [progressInd startAnimating];
    }
}

-(void) hideInfoText:(NSString *)info {
    if (![self.infoText isDescendantOfView:self.player]) {
        [self.player addSubview:infoText];
    }
    if ([info isEqualToString:self.infoText.text]) {
        self.infoText.hidden = YES;
    }
}

-(void) showInfoText :(NSString *)info interval:(NSTimeInterval)timeInterval
{
    if (infoText==nil) {
        CGFloat centerX =self.player.frame.size.width/2;
        CGFloat centerY =self.player.frame.size.height/2;
        CGRect labelFrame = CGRectMake(centerX, centerY, 150, 30);
        UILabel *temLabel = [[UILabel alloc] initWithFrame:labelFrame];
        self.infoText = temLabel;
        [temLabel release];
        self.infoText.center = CGPointMake(centerX,centerY+30);
        self.infoText.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
        self.infoText.textColor = [UIColor greenColor];
        self.infoText.numberOfLines = 0;
        self.infoText.textAlignment = NSTextAlignmentCenter;
        self.infoText.font = [UIFont systemFontOfSize:10];
    }else{
        CGFloat centerX =self.player.frame.size.width/2;
        CGFloat centerY =self.player.frame.size.height/2;
        self.infoText.center = CGPointMake(centerX,centerY+30);
    }
    if (![self.infoText isDescendantOfView:self.player]) {
        [self.player addSubview:infoText];
    }
    self.infoText.text= info;
    self.infoText.hidden = NO;
    if (timeInterval!=0) {
        [NSTimer scheduledTimerWithTimeInterval:timeInterval
                                         target:self
                                       selector:@selector(handleShowTextInfoTimer:)
                                       userInfo:info
                                        repeats:NO];
    }
}
- (void)handleShowTextInfoTimer:(NSTimer *)theTimer
{
    NSString *info = theTimer.userInfo;
    [self hideInfoText:info];
}

-(void)onGetDeviceTableCellInfo:(int)iResult parentId:(u_int)pId isChildrenZero:(BOOL)zero{
    
}

-(void)onlogin:(int)iResult user:(NSString*)strUser{
    
}
-(void)onLogout:(int)errorCode{
    
}
-(void)onReconnect:(int)count{
    
}
-(void)onDevNameChangeNotify:(u_int)uiDevid newName:(NSString*)strName needReload:(BOOL)isReload{
    
}
-(void)onAddDevNotify:(u_int)uiDevId devname:(NSString*)strName groupId:(u_int)uiGrpId needReload:(BOOL)isReload{
    
}
-(void)onDelDevNotify:(u_int)uiDevId groupId:(u_int)uiGrpId needReload:(BOOL)isReload{
    
}
-(void)onPermChangeNotify:(bool)bCanMonitor andPTZCtrlEnable: (bool) bPTZCtrlEnable{
    
}
-(void)onSyncAccBalanceInfo:(u_long)ulCHCnt{
    
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
