//
//  CMDefaultSettingViewController.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/18.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CMDefaultSettingViewController : UIViewController <UITableViewDataSource,UITableViewDelegate>

@property (strong ,nonatomic) IBOutlet UITableView      *table;

@end
