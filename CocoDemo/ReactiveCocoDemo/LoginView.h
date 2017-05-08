//
//  LoginView.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/12.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CMAccountManagerHelper.h"

@class CMCenterManager;
@interface LoginView : UIView <CMAccountManagerHelper>
{
    CMCenterManager* centerManager;
}

@end
