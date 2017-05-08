//
//  AppDelegate.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CMCenterManager;
@class CMMonitorController;
@class CMPlayerData;
@class CMDataCenter;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navController;
@property (readonly, nonatomic) CMDataCenter                  *historyData;
@property (readonly, nonatomic) CMDataCenter                  *updateData;
@property (readonly, nonatomic) CMCenterManager               *centerManager;
@property (readonly, nonatomic) CMMonitorController           *monitorController;
@property (readonly, nonatomic) CMPlayerData                  *playerData;

-(void) popToLoginViewController;
- (NSString *) updateIdentifier;
- (NSString *) defaultServertifier;

@end

