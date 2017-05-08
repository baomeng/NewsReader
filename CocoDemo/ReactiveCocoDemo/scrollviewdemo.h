//
//  scrollviewdemo.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/5/5.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface scrollviewdemo : UIViewController<UIScrollViewDelegate>
@property (strong, nonatomic) IBOutlet UIScrollView *scrollview1;
@property (strong, nonatomic) IBOutlet UIPageControl *pagecontrol;

@end
