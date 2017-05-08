//
//  LoginView.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/12.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <ReactiveCocoa/ReactiveCocoa.h>
#import "LoginView.h"
#import "AppDelegate.h"
#import "CMCenterManager.h"
#import "BViewController.h"
#import "CMUtil.h"
#import "CMSettingViewController.h"

@interface LoginView()

@property (nonatomic, weak) UIImageView *iCon;
@property (nonatomic, weak) UITextField *userName;
@property (nonatomic, weak) UITextField *password;
@property (nonatomic, weak) UIButton    *login_Btn;
@property (nonatomic, weak) UIButton    *set_Btn;
@property (nonatomic, weak) IBOutlet UIAlertView *waittingAlert;

@property (strong, nonatomic) NSString *serverIp;
@property (assign, nonatomic) int serverPort;

@end

@implementation LoginView

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
        
        AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
        centerManager = appDelegate.centerManager;
        
        [self InitUI];
        
        [self RACInit];
        
        //初始化server ip & port
        self.serverPort = [[CMUtil getDefaultServerPort] intValue];
        self.serverIp = [CMUtil getDefaultServerIp];
        
    }
    return self;
}

-(void)InitUI{
    
    UIImageView *iCon = [[UIImageView alloc]init];
    iCon.image = [UIImage imageNamed:@"DefaultProfileHead"];
    self.iCon = iCon;
    
    UITextField *userName = [[UITextField alloc]init];
    userName.placeholder = @"请输入用户名";
    [userName setBackground:[UIImage resizeImageWihtImageName:@"operationbox_text"]];
    userName.leftView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 10, 0)];
    userName.leftViewMode = UITextFieldViewModeAlways;
    userName.clearButtonMode = UITextFieldViewModeWhileEditing;//一键删除按钮
    userName.text = @"demo";
    userName.delegate = (id)self;
    self.userName = userName;
    
    UITextField *password = [[UITextField alloc]init];
    password.placeholder = @"请输入密码";
    password.secureTextEntry = YES;
    [password setBackground:[UIImage resizeImageWihtImageName:@"operationbox_text"]];
    password.clearButtonMode = UITextFieldViewModeWhileEditing;//一键删除按钮
    
    UIImageView *lockView = [[UIImageView alloc]init];
    lockView.image = [UIImage imageNamed:@"Card_Lock"];
    lockView.frame = CGRectMake(0, 0, 30, password.frame.size.height);
    lockView.contentMode = UIViewContentModeCenter;
    
    password.leftView = lockView;
    password.leftViewMode = UITextFieldViewModeAlways;
    password.delegate = (id)self;
    self.password = password;
    
    UIButton *login_Btn = [[UIButton alloc]init];
    login_Btn.enabled = NO;
    [login_Btn setTitle:@"登陆" forState:UIControlStateNormal];
    [login_Btn setBackgroundImage:[UIImage resizeImageWihtImageName:@"fts_green_btn"] forState:UIControlStateNormal];
    //       [login_Btn addTarget:self action:@selector(loginClick) forControlEvents:UIControlEventTouchUpInside];
    self.login_Btn = login_Btn;
    
    UIButton *set_Btn = [[UIButton alloc]init];
    set_Btn.enabled = YES;
    [set_Btn setTitle:@"设置" forState:UIControlStateNormal];
    [set_Btn setBackgroundImage:[UIImage resizeImageWihtImageName:@"fts_green_btn"] forState:UIControlStateNormal];
    [set_Btn addTarget:self action:@selector(showSettingView) forControlEvents:UIControlEventTouchUpInside];
    self.set_Btn = set_Btn;
    
    [self addSubview:iCon];
    [self addSubview:userName];
    [self addSubview:password];
    [self addSubview:login_Btn];
    [self addSubview:set_Btn];
    
    //        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didFinishRegister:) name:@"didFinishRegister" object:nil];
    
    //       [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(textDidChange) name:UITextFieldTextDidChangeNotification object:userName];
    //        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(textDidChange) name:UITextFieldTextDidChangeNotification object:password];
}

-(BOOL)isValidUserName:(NSString *)text{
    
    if ([self.userName hasText]) {
        return YES;
    }
    return NO;
}

-(BOOL)isVailidPassword:(NSString *)text{
    /*    if(6 <= text.length && text.length <= 16){
     return YES;
     }
     return NO;
     */
    return  YES;
}

-(void)RACInit{
    //new 信号，并且判断是否合法。
    RACSignal *validUserNameSignal = [self.userName.rac_textSignal map:^id(NSString *value) {
        return @([self isValidUserName:value]);
    }];
    RACSignal *validPasswordSignal = [self.password.rac_textSignal map:^id(NSString *value) {
        return @([self isVailidPassword:value]);
    }];
    //RAC(需要改变的控件名字,需要改变控件的什么属性) = [信号 map:^id(NSNumber *number/*信号返回的值*/){return 需要改变的属性的值}];//绑定信号
    RAC(self.userName,backgroundColor) = [validUserNameSignal map:^id(NSNumber *userNameValid) {
        return [userNameValid boolValue]?[UIColor clearColor]:[UIColor redColor];
    }];
    RAC(self.password,backgroundColor) = [validPasswordSignal map:^id(NSNumber *passwordValid) {
        return [passwordValid boolValue]?[UIColor clearColor]:[UIColor redColor];
    }];
    //聚合信号,将userNameTextField和passwordTextField聚合在一起。
    RACSignal *signUpActiveSignal = [RACSignal combineLatest:@[validUserNameSignal,validPasswordSignal] reduce:(id)^id(NSNumber *usernameValid,NSNumber *passwordValid){
        return @([usernameValid boolValue] && [passwordValid boolValue]);//两边都是 yes && yes == yes
    }];
    
    [signUpActiveSignal subscribeNext:^(NSNumber *signupActive) {
        self.login_Btn.enabled = [signupActive boolValue];
        if([signupActive boolValue]){
            self.login_Btn.backgroundColor = [UIColor clearColor];
        }else{
            self.login_Btn.backgroundColor = [UIColor redColor];
        }
    }];
    
    //    登录方法
    [[self.login_Btn rac_signalForControlEvents:UIControlEventTouchUpInside] subscribeNext:^(id x) {
        [self signButtonAction];//登录
    }];
    
}

-(void)signButtonAction
{
    [centerManager regAccountHelper:self];
    
//   self.serverPort = [[CMUtil getDefaultServerPort] intValue];
//   self.serverIp =[CMUtil getDefaultServerIp];
    
    if ([centerManager login:self.serverIp port:self.serverPort user:self.userName.text password:self.password.text withBlock:^(int iRet, NSString* user){}]!=CMFC_FAILED)
    {
        _waittingAlert = [self showWaitingAlert];
    }
    else
    {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"错误信息", nil)
                                                            message:NSLocalizedString(@"登录失败，连接服务器出错！", nil)
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@"确定", nil)
                                                  otherButtonTitles:nil, nil];
        [alertView show];
        //[alertView release];
    }
    
}

-(UIAlertView *)showWaitingAlert{
    UIAlertView *waittingAlert = [[UIAlertView alloc] initWithTitle: @"登录获取数据中"
                                                            message: @"请稍候..."
                                                           delegate: nil
                                                  cancelButtonTitle: nil
                                                  otherButtonTitles: nil];
    
    
    UIActivityIndicatorView *activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    activityView.frame = CGRectMake(139.0f-18.0f, 80.0f, 37.0f, 37.0f);
    [waittingAlert addSubview:activityView];
    [activityView startAnimating];
    
    [waittingAlert show];
    
    return waittingAlert;
    
}
-(void) dismissWaitingAlert:(UIAlertView*) waittingAlert{
    
    if (waittingAlert != nil) {
        [waittingAlert dismissWithClickedButtonIndex:0 animated:YES];
        // [waittingAlert release];
        waittingAlert =nil;
    }
}
- (void)showSettingView
{
    CMSettingViewController *settingController;
    settingController = [[CMSettingViewController alloc] initWithNibName:@"CMSettingViewController" bundle:nil];
    settingController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
    
    settingController.mysettingBlock = ^(NSString* ip, int port) {
        self.serverIp = ip;
        self.serverPort = port;
    };
    
    UIViewController *viewController = ((AppDelegate *)[UIApplication sharedApplication].delegate).navController.visibleViewController;
    [viewController.navigationController pushViewController:settingController animated:YES];
}

/*-(void)didFinishRegister:(NSNotification *)notification
{
//    self.userName.text = [UserInfo sharedUserInfo].registerUserName;
    self.userName.textAlignment = NSTextAlignmentCenter;
    self.userName.enabled = NO;
    self.userName.leftView = nil;
    [self.userName setBackground:nil];
//    [MBProgressHUD showSuccess:@"注册成功，请输入密码登陆" toView:self.superview];
}

-(void)textDidChange
{
    if ([self.userName hasText] && [self.password hasText]) {
        self.login_Btn.enabled = YES;
    }
    else
    {
        self.login_Btn.enabled = NO;
    }
}
*/

-(void)layoutSubviews
{
    [super layoutSubviews];
    
    self.iCon.width = 100;
    self.iCon.height = 100;
    self.iCon.x =  (self.width - self.iCon.width) / 2;
    self.iCon.y = 50;
    
    self.userName.width = self.frame.size.width;
    self.userName.height = 40;
    self.userName.x = (self.width - self.userName.width) / 2;
    self.userName.y = self.iCon.y + self.iCon.height + 10;
    
    self.password.width = self.userName.width;
    self.password.height = 40;
    self.password.x = (self.width - self.password.width) / 2;
    self.password.y = self.userName.y + self.userName.height + 10;
    
    self.login_Btn.width = self.password.width;
    self.login_Btn.height = 40;
    self.login_Btn.x = (self.width - self.login_Btn.width) / 2;
    self.login_Btn.y = self.password.y + self.password.height + 10;
 
    self.set_Btn.width = self.password.width;
    self.set_Btn.height = 40;
    self.set_Btn.x = (self.width - self.set_Btn.width) / 2;
    self.set_Btn.y = self.login_Btn.y + self.login_Btn.height + 10;
 
}


-(void)onGetDeviceTableCellInfo:(int)iResult parentId:(u_int)pId isChildrenZero:(BOOL)zero{
    
}

-(void)onlogin:(int)iResult user:(NSString*)strUser{
    NSString * resultString = convertErrorCodeToString((_CM_ERROR_CODE_)iResult);
    
    if (iResult!=CMED_OPERATE_SUCCESS) {
        
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"错误信息", nil)
                                                            message:resultString
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@"确定", nil)
                                                  otherButtonTitles:nil, nil];
        [alertView show];
        InterLog(@"onlogin false isCancelAutoLogin is YES");
        [centerManager cancelLogin];
        
    }else{
        
        // 登录成功 跳转
        
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
        BViewController *bVC = [storyboard instantiateViewControllerWithIdentifier:@"bViewController"];
        
        UIViewController *viewController = ((AppDelegate *)[UIApplication sharedApplication].delegate).navController.visibleViewController;
        
        [viewController.navigationController pushViewController:bVC animated:YES];
        //[self.navigationController pushViewController:bVC animated:YES];
        
        InterLog(@"onlogin isCancelAutoLogin is YES");
    }
    
    [self dismissWaitingAlert:_waittingAlert];
    
    [centerManager unregAccountHelper:self];
    
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
-(void)dele{
    [[self.userName.rac_textSignal filter:^BOOL(id value) {
        return YES;
    }]subscribeNext:^(id x) {
        
    }];
    //打印所有
    //    [self.userNameTextField.rac_textSignal subscribeNext:^(id x) {
    //        NSLog(@"%@",x);
    //    }];
    
    //只显示3以上的。一步到位
    //    [[self.userNameTextField.rac_textSignal filter:^BOOL(id value) {
    //        NSString *text = value;
    //        return text.length >3;
    //    }]subscribeNext:^(id x) {
    //        NSLog(@"%@",x);
    //    }];
    //
    //    //只显示3以上的。分步
    //    //注册订阅
    //    RACSignal *usernameSourceSignal = self.userNameTextField.rac_textSignal;
    //
    //    //设置一个过滤器，如果return true 那么就通过，执行到subscribeNext。
    //    RACSignal *fileteredUserName = [usernameSourceSignal filter:^BOOL(id value) {
    //        NSString *text = value;
    //        return text.length>3;
    //    }];
    //    //过滤器通过，执行下一步
    //    [fileteredUserName subscribeNext:^(id x) {
    //        NSLog(@"%@",x);
    //    }];
    
    
    //通过map修改，返回长度
    //    [[[self.userNameTextField.rac_textSignal map:^id(NSString *text){
    //        return @(text.length);
    //    }]filter:^BOOL(NSNumber *length){
    //        return [length integerValue]>3;
    //    }]subscribeNext:^(id x){
    //        NSLog(@"%@",x);
    //    }];
    
    //已经合法，接受信号，改变颜色
    //    [[validUserNameSignal map:^id(NSNumber *value) {
    //        return [value boolValue]?[UIColor clearColor]:[UIColor yellowColor];
    //    }]subscribeNext:^(UIColor *color) {
    //        self.userNameTextField.backgroundColor = color;
    //    }];
    //
    //    [[validPasswordSignal map:^id(NSNumber *value) {
    //        return [value boolValue]?[UIColor clearColor]:[UIColor yellowColor];
    //    }]subscribeNext:^(UIColor *color) {
    //        self.passWordTextField.backgroundColor = color;
    //    }];
}

@end
