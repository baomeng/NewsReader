//
//  DetailViewController.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/5/6.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "DetailViewController.h"

@interface DetailViewController ()

@end

@implementation DetailViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    [self loadUI];
}
- (void) loadUI
{
    if (_type == myaction) {
        _button1.hidden = NO;
        _label1.hidden = NO;
    }
    else if (_type == no){
        _button1.hidden = YES;
        _label1.hidden = YES;
    }
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
