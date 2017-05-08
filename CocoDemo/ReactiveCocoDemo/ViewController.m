//
//  ViewController.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "ViewController.h"
#import "LoginView.h"
#import "Masonry.h"

@interface ViewController ()
@property (nonatomic,weak) UIView*  loginView;
@end

@implementation ViewController

- (void)viewWillAppear:(BOOL)animated
{
    self.navigationController.navigationBarHidden = YES;
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
//    [centerManager regAccountHelper:self];
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
 // [centerManager unregAccountHelper:self];
    [super viewDidDisappear:animated];
}

-(void)setInit // loginview
{
    self.view.backgroundColor = [UIColor whiteColor];
    self.title = @"登陆";
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(keyboardDismiss)];
    [self.view addGestureRecognizer:tapGesture];
    
    LoginView * loginView = [[LoginView alloc]init];
    self.loginView = loginView;
    [self.view addSubview:loginView];
    
    self.loginView.translatesAutoresizingMaskIntoConstraints = NO;
    __weak __typeof(self) weakSelf = self;
    [self.loginView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.center.equalTo(self.view);
        make.size.mas_equalTo(CGSizeMake(300,600));
    }];
    
//    self.loginView.width = 300;
//    self.loginView.height = 600;
//    self.loginView.x = (ScreenWidth - self.loginView.width) / 2;
//    self.loginView.y = 200;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [self setInit];
}

-(void)keyboardDismiss
{
    [self.view endEditing:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
