//
//  MeViewControllerDemo.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/5/6.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MeViewControllerDemo : UIViewController<UITableViewDelegate, UITableViewDataSource>
{
    UIImageView *imageBG;
    UIView *BGView;
}

@property (strong, nonatomic) IBOutlet UITableView *tableview;

@end
