//
//  AppDelegate.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"

#import "CMModuleCreatorProtocolDef.h"
#import "CMCenterManager.h"
#import "CMPlayerData.h"
#import "CMMonitorController.h"
#import "CMDataCenter.h"
#import "CMUtil.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

@synthesize centerManager, monitorController, playerData, navController, historyData, updateData;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
   
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    if ([prefs boolForKey:@"hasRunBefore"] != YES)
    {
        // Set flag so we know not to run this next time
        [prefs setBool:YES forKey:@"hasRunBefore"];
        [prefs synchronize];
        // init default server ip & port
        NSArray * servers = nil;
        servers = [CMUtil getCenterIpInfo];
        if (servers!=nil) {
//          [CMUtil initDefaultServerInfo:servers serverPort:DEFAULT_SERVER_PORT];
            int sz=servers.count;
            for (int i = sz-1; i>=0; i--) {
                NSString* ip = [servers objectAtIndex:i];
                NSLog(@"insert ip = %@ index = %d",ip,i);
                [CMUtil insertServerIPToHistoryDataCenter:ip serverPort:DEFAULT_SERVER_PORT];
            }
            [CMUtil initDefaultServerInfo:servers serverPort:DEFAULT_SERVER_PORT];
            [CMUtil insertUserNameToHistoryDataCenter:@"demo" password:@"" rememberPassword:NO autoLogin:NO];
        }
    }
    
    self.window = [[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    ViewController *vc = [[ViewController alloc]init];
    navController = [[UINavigationController alloc]initWithRootViewController:vc];
    self.window.rootViewController = navController;
    [self.window makeKeyAndVisible];

    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    
    InterLog(@"applicationDidEnterBackground");
    [self.centerManager stopAllMonitor];
    [self.centerManager cancelLogin];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    [self popToLoginViewController];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

-(void) popToLoginViewController
{
    if (![self.navController.topViewController isMemberOfClass:[ViewController class]]) {
        ViewController *theLoginController = nil;
        for (UIViewController* controller in self.navController.viewControllers) {
            if ([controller isMemberOfClass:[ViewController class]]) {
                theLoginController = (ViewController *)controller;
                break;
            }
        }
        if (theLoginController!=nil) {
            [self.navController popToViewController:theLoginController animated:YES];
        }else{
            ViewController *loginController ;
            UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
            loginController = [storyboard instantiateViewControllerWithIdentifier:@"ViewController"];
            
  //          loginController = [[ViewController alloc] initWithNibName:@"loginviewcontroller-ipad"
  //                                                                  bundle:nil];
            [self.navController pushViewController:loginController animated:YES];
            //[loginController release];
        }
    }
    else{
        ViewController* loginController = (ViewController*) self.navController.topViewController;
        //[loginController showLoginView];
    }
}

- (CMCenterManager *)centerManager
{
    if (centerManager==nil) {
        [CMModuleCreatorSingleton initializeModuleCreator];
        centerManager = [[CMCenterManager alloc] init];
    }
    return centerManager;
}
- (CMPlayerData *)playerData
{
    if (playerData==nil) {
        playerData = [[CMPlayerData alloc] init];
    }
    return playerData;
}

- (CMDataCenter *) historyData
{
    if (historyData==nil) {
        historyData = [[CMDataCenter alloc] initWithPlistFile:HISTORY_PROPERTY_LIST_MAIN extendString:HISTORY_PROPERTY_LIST_EXTEND];
    }
    return historyData;
}
- (CMDataCenter *) updateData
{
    if (updateData==nil) {
        updateData = [[CMDataCenter alloc] initWithPlistFile:UPDATE_PROPERTY_LIST_MAIN extendString:UPDATE_PROPERTY_LIST_EXTEND];
    }
    return updateData;
}
- (NSString *) updateIdentifier
{
    return [self.updateData objectForKey:@"UpdateIdentifier"];
}
- (NSString *) defaultServertifier
{
    return [self.updateData objectForKey:@"DefaultServer"];
}
- (CMMonitorController *) monitorController
{
 /*   if (monitorController==nil) {
        switch ([self getDeviceModel]) {
            case DeviceModel_iPhone:
            {
                monitorController = [[CMMonitorController alloc] initWithNibName:@"CMMonitorController"
                                                                          bundle:nil];
                break;
            }
            case DeviceModel_iPad:
            {
                monitorController = [[CMMonitorController alloc] initWithNibName:@"CMMonitorController-ipad"
                                                                          bundle:nil];
                break;
            }
            default:
                monitorController = [[CMMonitorController alloc] initWithNibName:@"CMMonitorController-ipod"
                                                                          bundle:nil];
                break;
        }
    }
  */
    if (monitorController==nil)
        monitorController = [[CMMonitorController alloc] initWithNibName:@"CMMonitorController_null"
                                                              bundle:nil];
    return monitorController;
}
-(DeviceModel) getDeviceModel
{
    
    NSString *strModel = [UIDevice currentDevice].model;
    if ( strModel == nil )
    {
        return DeviceModel_Unknow;
    }
    else
    {
        if ( [strModel rangeOfString:@"iPhone" options:NSCaseInsensitiveSearch].length > 0 )
        {
            return DeviceModel_iPhone;
        }
        else
        {
            if ( [strModel rangeOfString:@"iPad" options:NSCaseInsensitiveSearch].length > 0 )
            {
                return DeviceModel_iPad;
            }
            else
            {
                return DeviceModel_iPodTouch;
            }
        }
    }
}

@end
