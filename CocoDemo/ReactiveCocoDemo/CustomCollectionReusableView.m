//
//  CustomCollectionReusableView.m
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "CustomCollectionReusableView.h"

@implementation CustomCollectionReusableView

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self creatUI];
    }
    return self;
}

- (void)creatUI{
    _label = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, self.bounds.size.width, self.bounds.size.height)];
    _label.backgroundColor = [UIColor blackColor];
    _label.alpha = 1.0f;
    _label.textAlignment = NSTextAlignmentCenter;
    _label.font = [UIFont systemFontOfSize:12.0];
    _label.textColor = [UIColor whiteColor];
    [self addSubview:_label];
}

#pragma mark *** Getters ***
/*
 - (UILabel *)displayLabel {
    if (!_displayLabel) {
        _displayLabel = [[UILabel alloc] initWithFrame:self.bounds];
        _displayLabel.backgroundColor = [UIColor redColor];
        _displayLabel.textColor = [UIColor whiteColor];
        _displayLabel.textAlignment = NSTextAlignmentCenter;
    }
    return _displayLabel;
}
*/

@end
