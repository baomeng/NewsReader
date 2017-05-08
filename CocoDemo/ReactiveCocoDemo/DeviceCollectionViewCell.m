//
//  PictureCollectionViewCell.m
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "DeviceCollectionViewCell.h"

@implementation DeviceCollectionViewCell

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        
        NSArray *arrayOfViews = [[NSBundle mainBundle] loadNibNamed:@"devCell-ipad" owner:nil options:nil];
        if (arrayOfViews.count < 1)
        {
            return nil;
        }
        if (![[arrayOfViews objectAtIndex:0] isKindOfClass:[UIView class]])
        {
            return nil;
        }
        UIView *view = [arrayOfViews objectAtIndex:0];
        [self.contentView addSubview:view];
        
        _mainImageView = (UIImageView *)[view viewWithTag:1];
        _titleLabel = (UILabel *)[view viewWithTag:2];
        
        _playButton = (UIButton *)[view viewWithTag:3];
    }
    return self;
}

@end
