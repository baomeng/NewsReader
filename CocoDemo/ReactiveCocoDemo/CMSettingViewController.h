//
//  CMSettingViewController.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/14.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "student.h"

typedef void (^settingBlock)(NSString*, int);

@interface CMSettingViewController : UIViewController<UITableViewDataSource, UITableViewDelegate>

@property (strong, nonatomic) IBOutlet UITextField  *serverIP;
@property (strong, nonatomic) IBOutlet UITextField  *serverPort;
@property (strong, nonatomic) IBOutlet UIButton*    pclButtonOK;
@property (strong, nonatomic) IBOutlet UIButton*    pclButtonDB;
@property (strong, nonatomic) IBOutlet UIButton*    pclBtnScroll;
@property (strong, nonatomic) IBOutlet UIButton*    pclBtnMe;
@property (strong, nonatomic) UITableView* table;

@property (strong, nonatomic) NSMutableArray* arrStudent;
//@property (nonatomic,copy) void (^settingBlock)(NSString*, int);

@property (nonatomic,copy) settingBlock mysettingBlock;

@end
