//
//  DetailViewController.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/5/6.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum : NSUInteger {
    no,
    myaction,
    monthsale
} Type;

@interface DetailViewController : UIViewController
@property (strong, nonatomic) IBOutlet UIButton *button1;
@property (strong, nonatomic) IBOutlet UILabel *label1;
@property (nonatomic, assign) Type type;
@end
