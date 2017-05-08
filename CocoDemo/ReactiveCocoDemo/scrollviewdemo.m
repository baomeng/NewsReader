//
//  scrollviewdemo.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/5/5.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "scrollviewdemo.h"

@interface scrollviewdemo ()

@end

@implementation scrollviewdemo
@synthesize scrollview1;
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    scrollview1.backgroundColor = [UIColor whiteColor];
    self.scrollview1.contentSize = CGSizeMake(320*4, 480);
    //scrollview1.contentInset = UIEdgeInsetsMake(10, 10, 10, 10);
    //scrollview1.contentOffset = CGPointMake(10, 10);
    [scrollview1 setIndicatorStyle:UIScrollViewIndicatorStyleWhite];
    scrollview1.delegate = self;
    
    
    UIView* v1 = [[UIView alloc] initWithFrame:self.view.bounds];
    [v1 setFrame:CGRectMake(0, 0, 320, 480)];
    v1.backgroundColor = [UIColor redColor];
    [scrollview1 addSubview:v1];
    UIView* v2 = [[UIView alloc] initWithFrame:self.view.bounds];
    [v2 setFrame:CGRectMake(320*1, 0, 320, 480)];
    v2.backgroundColor = [UIColor yellowColor];
    [scrollview1 addSubview:v2];
    UIView* v3 = [[UIView alloc] initWithFrame:self.view.bounds];
    [v3 setFrame:CGRectMake(320*2, 0, 320, 480)];
    v3.backgroundColor = [UIColor blueColor];
    [scrollview1 addSubview:v3];
    UIView* v4 = [[UIView alloc] initWithFrame:self.view.bounds];
    [v4 setFrame:CGRectMake(320*3, 0, 320, 480)];
    v4.backgroundColor = [UIColor greenColor];
    [scrollview1 addSubview:v4];
    
    [self.pagecontrol addTarget:self action:@selector(pageTurn:) forControlEvents:UIControlEventValueChanged];
}

- (void)pageTurn:(UIPageControl*)sender
{
    int icurPage = self.pagecontrol.currentPage;
    CGRect rect = CGRectMake(320*icurPage, 0, 320, 480);
    [scrollview1 scrollRectToVisible:rect animated:YES];
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

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView{
    int ipage = scrollView.contentOffset.x/320;
    self.pagecontrol.currentPage = ipage;
}
@end
