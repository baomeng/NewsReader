//
//  BViewController.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "CMAccountManagerHelper.h"
#import "CMDeviceManagerHelper.h"

@class CMCenterManager;
@class CMDeviceTableCellInfo;
@interface BViewController : UIViewController<UICollectionViewDataSource, UICollectionViewDelegate, UICollectionViewDelegateFlowLayout, CMAccountManagerHelper, CMDeviceManagerHelper>
{
    CMCenterManager *centerManager;
}
@property (nonatomic, strong) UICollectionView             *collectionView;
@property (nonatomic, strong) UICollectionViewFlowLayout   *collectionViewFlowLayout;
@property (nonatomic, copy)   CMDeviceTableCellInfo        *curcellInfo;
@property (nonatomic, weak) IBOutlet UIAlertView         *waittingAlert1;
@property (nonatomic, assign) BOOL                         isOnline;
@property (nonatomic, assign) BOOL                         bPlay;

- (void)showMonitorView:(CMDeviceTableCellInfo *)cellInfo;

@end
