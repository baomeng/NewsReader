//
//  CMUtil.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/14.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "CMUtil.h"
#import "CMDataCenter.h"
//#import "CMDefaultSettings.h"
#import "AppDelegate.h"
#import "CMMonitorController.h"
//#import "CMDeviceTableViewController.h"
//#import "CMSettingViewController.h"

@implementation CMUtil

+(CMDataCenter*)getDefaultHistoryDataCenter
{
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    return appDelegate.historyData;
}
+ (int)getUserNameCountsInHistoryDataCenter
{
    NSMutableArray *accounts = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_ACCOUNT];
    return accounts.count;
}
+ (int)getServerIpCountsInHistoryDataCenter
{
    NSMutableArray *ips = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_SERVER_IP];
    return ips.count;
}
+(NSString*)getDefaultUserName
{
    NSMutableArray *accounts = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_ACCOUNT];
    if (accounts.count>0) {
        return [accounts objectAtIndex:0];
    }
    return nil;
}
+(NSString*)getDefaultPassword
{
    NSMutableArray *passwords = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_PASSWORD];
    if (passwords.count>0) {
        return [passwords objectAtIndex:0];
    }
    return nil;
}
+(BOOL)getDefaultRememberPasswordTag
{
    NSMutableArray *rememberPasswords = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_REMEMBER_PASSWORD];
    if (rememberPasswords.count>0) {
        NSNumber *rememberPasswordNumber = [rememberPasswords objectAtIndex:0];
        return [rememberPasswordNumber boolValue];
    }
    return NO;
}
+(BOOL)getDefaultAutoLoginTag
{
    NSMutableArray *autoLogins = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_AUTO_LOGIN];
    if (autoLogins.count>0) {
        NSNumber *autoLoginNumber = [autoLogins objectAtIndex:0];
        return [autoLoginNumber boolValue];
    }
    return NO;
}
+(NSString*)getDefaultServerIp
{
    NSMutableArray *ips = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_SERVER_IP];
    if (ips.count>0) {
        return [ips objectAtIndex:0];
    }
    return nil;
}
+(NSNumber *)getDefaultServerPort
{
    NSMutableArray *ports = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_SERVER_PORT];
    if (ports.count>0) {
        return [ports objectAtIndex:0];
    }
    return nil;
}
+ (NSString*)getUserNameForIndex:(int)index
{
    NSMutableArray *accounts = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_ACCOUNT];
    if (accounts.count>index) {
        return [accounts objectAtIndex:index];
    }
    return nil;
}
+ (NSString*)getServerIpForIndex:(int)index
{
    NSMutableArray *ips = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_SERVER_IP];
    if (ips.count>index) {
        return [ips objectAtIndex:index];
    }
    return nil;
}
+ (BOOL)getRememberPasswordTagForIndex:(int)index
{
    NSMutableArray *rememberPasswords = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_REMEMBER_PASSWORD];
    if (rememberPasswords.count>index) {
        NSNumber *rememberPasswordNumber = [rememberPasswords objectAtIndex:index];
        return [rememberPasswordNumber boolValue];
    }
    return NO;
}
+ (BOOL)getAutoLoginTagForIndex:(int)index
{
    NSMutableArray *autoLogins = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_AUTO_LOGIN];
    if (autoLogins.count>index) {
        NSNumber *autoLoginNumber = [autoLogins objectAtIndex:index];
        return [autoLoginNumber boolValue];
    }
    return NO;
}
+ (NSString*)getPasswordForIndex:(int)index
{
    NSMutableArray *passwords = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_PASSWORD];
    if (passwords.count>index) {
        return [passwords objectAtIndex:index];
    }
    return nil;
}
+ (NSNumber *)getServerPortForIndex:(int)index
{
    NSMutableArray *ports = [[CMUtil getDefaultHistoryDataCenter] objectForKey:KEY_HISTORY_SERVER_PORT];
    if (ports.count>index) {
        return [ports objectAtIndex:index];
    }
    return nil;
}
+ (void)insertUserNameToHistoryDataCenter:(NSString *)name password:(NSString *)pwd rememberPassword:(BOOL)isRemember autoLogin:(BOOL) isAuto
{
    if ([name isEqualToString:@""]) {
        return;
    }
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    NSMutableArray *accounts = [historyData objectForKey:KEY_HISTORY_ACCOUNT];
    NSMutableArray *arrayTmp = [NSMutableArray arrayWithArray:accounts];
    NSMutableArray *passwords = [historyData objectForKey:KEY_HISTORY_PASSWORD];
    NSMutableArray *arrayPassword = [NSMutableArray arrayWithArray:passwords];
    
    NSMutableArray * remembers = [historyData objectForKey:KEY_HISTORY_REMEMBER_PASSWORD];
    NSMutableArray *arrayRememberPassword = [NSMutableArray arrayWithArray:remembers];
    NSMutableArray * autos = [historyData objectForKey:KEY_HISTORY_AUTO_LOGIN];
    NSMutableArray *arrayAutoLogin = [NSMutableArray arrayWithArray:autos];
    NSUInteger uiIndex = [arrayTmp indexOfObject:name];
    if ( uiIndex != NSNotFound )
    {
        [arrayTmp removeObjectAtIndex:uiIndex];
        [arrayPassword removeObjectAtIndex:uiIndex];
        [arrayRememberPassword removeObjectAtIndex:uiIndex];
        [arrayAutoLogin removeObjectAtIndex:uiIndex];
    }
    [arrayTmp insertObject:name atIndex:0];
    [arrayPassword insertObject:pwd atIndex:0];
    
    NSNumber *isRememberNumber = [NSNumber numberWithBool:isRemember];
    [arrayRememberPassword insertObject:isRememberNumber atIndex:0];
    NSNumber *isAutoLoginNumber = [NSNumber numberWithBool:isAuto];
    [arrayAutoLogin insertObject:isAutoLoginNumber atIndex:0];
    while([arrayTmp count] > 5)
    {
        [arrayTmp removeObjectAtIndex:5];
        [arrayPassword removeObjectAtIndex:5];
        [arrayRememberPassword removeObjectAtIndex:5];
        [arrayAutoLogin removeObjectAtIndex:5];
    }
    NSMutableDictionary * dic = [historyData getDictionaryForFile];
    [dic setObject:arrayTmp forKey:KEY_HISTORY_ACCOUNT];
    [dic setObject:arrayPassword forKey:KEY_HISTORY_PASSWORD];
    [dic setObject:arrayRememberPassword forKey:KEY_HISTORY_REMEMBER_PASSWORD];
    [dic setObject:arrayAutoLogin forKey:KEY_HISTORY_AUTO_LOGIN];
    [historyData writeToFile:dic];
}
+ (void)updateAutoLoginTag:(BOOL)isAuto
{
     NSNumber *isAutoLoginNumber = [NSNumber numberWithBool:isAuto];
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    NSMutableArray * autos = [historyData objectForKey:KEY_HISTORY_AUTO_LOGIN];
    NSMutableArray *arrayAutoLogin = [NSMutableArray arrayWithArray:autos];
  //  NSNumber *curAutoTag =  (NSNumber *)[arrayAutoLogin objectAtIndex:0];
  //  curAutoTag = isAutoLoginNumber;
    [arrayAutoLogin removeObjectAtIndex:0];
    [arrayAutoLogin insertObject:isAutoLoginNumber atIndex:0];
    NSMutableDictionary * dic = [historyData getDictionaryForFile];
    [dic setObject:arrayAutoLogin forKey:KEY_HISTORY_AUTO_LOGIN];
    [historyData writeToFile:dic];
}
+ (void)insertServerIPToHistoryDataCenter:(NSString *)ip serverPort:(int)port
{
    if ([ip isEqualToString:@""]) {
        return;
    }
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    NSMutableArray *ips = [historyData objectForKey:KEY_HISTORY_SERVER_IP];
    NSMutableArray *ports = [historyData objectForKey:KEY_HISTORY_SERVER_PORT];
    
    NSMutableArray *arrayTmp = [NSMutableArray arrayWithArray:ips];
    NSMutableArray *arrayServerPort = [NSMutableArray arrayWithArray:ports];
    NSUInteger uiIndex = [arrayTmp indexOfObject:ip];
    if ( NSNotFound != uiIndex )
    {
        [arrayTmp removeObjectAtIndex:uiIndex];
        [arrayServerPort removeObjectAtIndex:uiIndex];
    }
    [arrayTmp insertObject:ip atIndex:0];
    for (NSString* temip in arrayTmp) {
        NSLog(@"insert tem ip = %@",temip);
    }
    NSNumber *NumberPort = [NSNumber numberWithInt:port];
    [arrayServerPort insertObject:NumberPort atIndex:0];
    while([arrayTmp count] > 5)
    {
        [arrayTmp removeObjectAtIndex:5];
        [arrayServerPort removeObjectAtIndex:5];
    }
    NSMutableDictionary * dic = [historyData getDictionaryForFile];
    [dic setObject:arrayTmp forKey:KEY_HISTORY_SERVER_IP];
    [dic setObject:arrayServerPort forKey:KEY_HISTORY_SERVER_PORT];
    [historyData writeToFile:dic];
}
+ (void)removeAllServerIPInfo
{
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    NSMutableDictionary * dic = [historyData getDictionaryForFile];
    [dic setObject:nil forKey:KEY_HISTORY_SERVER_IP];
    [dic setObject:nil forKey:KEY_HISTORY_SERVER_PORT];
    [historyData writeToFile:dic];
}
+ (void)removeServerIPFromHistoryDataCenter:(NSUInteger )rowIndex
{
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    NSMutableArray *ips = [historyData objectForKey:KEY_HISTORY_SERVER_IP];
    NSMutableArray *ports = [historyData objectForKey:KEY_HISTORY_SERVER_PORT];
    NSMutableArray *arrayTmp = [NSMutableArray arrayWithArray:ips];
    NSMutableArray *arrayServerPort = [NSMutableArray arrayWithArray:ports];
    if ([arrayTmp count]>rowIndex && [arrayServerPort count]>rowIndex) {
        [arrayTmp removeObjectAtIndex:rowIndex];
        [arrayServerPort removeObjectAtIndex:rowIndex];
        NSMutableDictionary * dic = [historyData getDictionaryForFile];
        [dic setObject:arrayTmp forKey:KEY_HISTORY_SERVER_IP];
        [dic setObject:arrayServerPort forKey:KEY_HISTORY_SERVER_PORT];
        [historyData writeToFile:dic];
    }
}
+ (void)removeUserNameFromHistoryDataCenter:(NSUInteger )rowIndex
{
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    NSMutableArray *accounts = [historyData objectForKey:KEY_HISTORY_ACCOUNT];
    NSMutableArray *arrayTmp = [NSMutableArray arrayWithArray:accounts];
    NSMutableArray *passwords = [historyData objectForKey:KEY_HISTORY_PASSWORD];
    NSMutableArray *arrayPassword = [NSMutableArray arrayWithArray:passwords];
    
    if ([arrayTmp count]>rowIndex && [arrayPassword count]>rowIndex) {
        [arrayTmp removeObjectAtIndex:rowIndex];
        [arrayPassword removeObjectAtIndex:rowIndex];
        NSMutableDictionary * dic = [historyData getDictionaryForFile];
        [dic setObject:arrayTmp forKey:KEY_HISTORY_ACCOUNT];
        [dic setObject:arrayPassword forKey:KEY_HISTORY_PASSWORD];
        [historyData writeToFile:dic];
    }
}
+ (void)initDefaultServerInfo:(NSArray *)serverInfos serverPort:(int)port
{
    CMDataCenter *historyData = [CMUtil getDefaultHistoryDataCenter];
    for (NSString *ip in serverInfos) {
        NSMutableArray *ips = [historyData objectForKey:KEY_HISTORY_SERVER_IP];
        NSMutableArray *ports = [historyData objectForKey:KEY_HISTORY_SERVER_PORT];
        
        NSMutableArray *arrayTmp = [NSMutableArray arrayWithArray:ips];
        NSMutableArray *arrayServerPort = [NSMutableArray arrayWithArray:ports];
        NSUInteger uiIndex = [arrayTmp indexOfObject:ip];
        if ( NSNotFound != uiIndex )
        {
            [arrayTmp removeObjectAtIndex:uiIndex];
            [arrayServerPort removeObjectAtIndex:uiIndex];
        }
        [arrayTmp insertObject:ip atIndex:0];
        NSNumber *NumberPort = [NSNumber numberWithInt:port];
        [arrayServerPort insertObject:NumberPort atIndex:0];
        while([arrayTmp count] > 5)
        {
            [arrayTmp removeObjectAtIndex:5];
            [arrayServerPort removeObjectAtIndex:5];
        }
        NSMutableDictionary * dic = [historyData getDictionaryForFile];
        [dic setObject:arrayTmp forKey:KEY_HISTORY_SERVER_IP];
        [dic setObject:arrayServerPort forKey:KEY_HISTORY_SERVER_PORT];
        [historyData writeToFile:dic];
    }
}

//取设备模型编码
+ (DeviceModel) getDeviceModel
{
    return DeviceModel_iPhone;
//    
//    NSString *strModel = [UIDevice currentDevice].model;
//    if ( strModel == nil )
//    {
//        return DeviceModel_Unknow;
//    }
//    else
//    {
//        if ( [strModel rangeOfString:@"iPhone" options:NSCaseInsensitiveSearch].length > 0 )
//        {
//            return DeviceModel_iPhone;
//        }
//        else
//        {
//            if ( [strModel rangeOfString:@"iPad" options:NSCaseInsensitiveSearch].length > 0 )
//            {
//                return DeviceModel_iPad;
//            }
//            else
//            {
//                return DeviceModel_iPodTouch;
//            }
//        }
//    }
}
+ (UIImage *)generatePhotoThumbnail:(UIImage *)image 
{  
    // Create a thumbnail version of the image for the event object.  
    CGSize size = image.size;  
    CGSize croppedSize;  
    CGFloat ratio = 64.0;  
    CGFloat offsetX = 0.0;  
    CGFloat offsetY = 0.0;  
    
    // check the size of the image, we want to make it   
    // a square with sides the size of the smallest dimension  
    if (size.width > size.height) {  
        offsetX = (size.height - size.width) / 2;  
        croppedSize = CGSizeMake(size.height, size.height);  
    } else {  
        offsetY = (size.width - size.height) / 2;  
        croppedSize = CGSizeMake(size.width, size.width);  
    }  
    
    // Crop the image before resize  
    CGRect clippedRect = CGRectMake(offsetX * -1, offsetY * -1, croppedSize.width, croppedSize.height);  
    CGImageRef imageRef = CGImageCreateWithImageInRect([image CGImage], clippedRect);  
    // Done cropping  
    
    // Resize the image  
    CGRect rect = CGRectMake(0.0, 0.0, ratio, ratio);  
    
    UIGraphicsBeginImageContext(rect.size);  
    [[UIImage imageWithCGImage:imageRef] drawInRect:rect];  
    UIImage *thumbnail = UIGraphicsGetImageFromCurrentImageContext();  
    UIGraphicsEndImageContext();  
    // Done Resizing  
    
    return thumbnail;  
}  
//检索目录
+(NSString *)findDirectory:(NSString *)dir
{    
    NSString *appDocDir = [[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] relativePath];
    NSString *path = [appDocDir stringByAppendingPathComponent:dir];
    BOOL isDire = YES;
    if (([[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDire])) {
        return path;
    }else{
        return nil;
    }
}

//删除目录下 所有文件
+(void)deleteAllFilesFromDir:(NSString *)dir
{
    NSString *appDocDir = [[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] relativePath];
    NSString *photoDir = [appDocDir stringByAppendingPathComponent:dir];    
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:photoDir error:NULL];
    if ([files count] == 0) {
        NSLog(@"Not file in  photos directory");
        return;
    }
    for (int i=0; i<[files count]; i++) {   
        NSString *filePath = [photoDir stringByAppendingPathComponent:[files objectAtIndex:i]];  
        [[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
    }
}
//删除目录下 指定文件
+(BOOL)deleteFileFromDir:(NSString *)dir withFileName:(NSString *)name
{
    NSString *appDocDir = [[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] relativePath];
    NSString *photoDir = [appDocDir stringByAppendingPathComponent:dir];    
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:photoDir error:NULL];
    if ([files count] == 0) {
        NSLog(@"Not file in  photos directory");
        return NO;
    }
    for (int i=0; i<[files count]; i++) {   
        NSString *curName = [files objectAtIndex:i];
        if (name!=nil && [curName compare:name]==NSOrderedSame) {
            NSString *filePath = [photoDir stringByAppendingPathComponent:name];  
            return  [[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
        }
    }
    return NO;
}
//创建目录
+(NSString *)createDirectory:(NSString *)dir
{
    NSString *appDocDir = [[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] relativePath];
    
    NSString *path = [appDocDir stringByAppendingPathComponent:dir];
    
    BOOL photo = [[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    
    if (photo) {
        NSLog(@"creat photo directory success");
        return path;
    }else{
        return nil;
    }
    //    return photo;
}
+  (BOOL)saveImageToFile:(UIImage *)image directionary:(NSString *)dir
{
    NSString *dirName = [CMUtil findDirectory:dir];
    if (dirName==nil) {
        dirName = [CMUtil createDirectory:dir];
    }
    if (dirName!=nil) {
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];
        NSString *strDate = [dateFormatter stringFromDate:[NSDate date]];
        //[dateFormatter release];
        NSString *fileName = [NSString stringWithFormat:@"%@.jpg",strDate];
        NSString  *jpgPath = [dirName stringByAppendingPathComponent:fileName];
        
        NSData * data = UIImageJPEGRepresentation(image, 1.0);
        BOOL result = YES;
        if (data!=nil) {
            result= [data writeToFile:jpgPath atomically:YES];
            return result;
        }
    }
    return NO;
}
+ (NSArray *) getCenterIpInfo
{
    NSArray * servers = nil;
    CM_DEALER_VERSION_TYPE dealerVersion = CM_DVT_SALE;
    
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    NSString* defaultServer = [appDelegate defaultServertifier];
    
    dealerVersion = (CM_DEALER_VERSION_TYPE)defaultServer.intValue;
    switch (dealerVersion) {
        case CM_DVT_SALE:
        {
            servers = [NSArray arrayWithObjects:@"58.57.65.85",@"222.173.162.199", nil];
            break;   
        }
        case CM_DVT_SHANDONG:
        {
            servers = [NSArray arrayWithObjects:@"218.56.183.69",@"58.57.65.69", nil];
            break;   
        }
        case CM_DVT_WUXI:
        {
            servers = [NSArray arrayWithObjects:@"58.215.79.227", nil];
            break;   
        }
        case CM_DVT_SHANGHAI:
        {
            servers = [NSArray arrayWithObjects:@"222.73.247.188", @"61.172.249.105", nil];
            break;
        }
        case CM_DVT_EWT:
        {
            servers = [NSArray arrayWithObjects:@"59.45.151.7", nil];
            break;
        }
        case CM_DVT_ZTY:
        {
            servers = [NSArray arrayWithObjects:@"218.56.183.69", nil];
            break;
        }
        case CM_DVT_REMOTE_EYE:
        {
            servers = [NSArray arrayWithObjects:@"58.57.65.69", nil];
            break;
        }
        case CM_DVT_SALE_OLD:
        {
            servers = [NSArray arrayWithObjects:@"110.ip98.net", nil];
            break;
        }
        case CM_DVT_EBAISHI:
        {
            servers = [NSArray arrayWithObjects:@"www.e-baijia.net", nil];
            break;
        }
        case CM_DVT_ASIA_TEK:
        {
            servers = [NSArray arrayWithObjects:@"218.56.183.69",@"58.57.65.69", nil];
            break;
        }
        case CM_DVT_CLOUND_VIEW:
        {
            servers = [NSArray arrayWithObjects:@"61.172.249.105", nil];
            break;
        }
        case CM_DVT_CHINATEL_CLOUND:
        {
            servers = [NSArray arrayWithObjects:@"222.88.82.90", nil];
            break;
        }
        case CM_DVT_GXJG:
        {
            servers = [NSArray arrayWithObjects:@"113.12.83.73", nil];
            break;
        }
        case CM_DVT_XYJK:
        {
            servers = [NSArray arrayWithObjects:@"vip.ip98.net",@"110.ip98.net", nil];
            break;
        }
        case CM_DVT_DIANSHIJIE:
        {
            servers = [NSArray arrayWithObjects:@"vip.ip98.net",@"110.ip98.net", nil];
            break;
        }
        case CM_DVT_ONLYBABY:
        {
            servers = [NSArray arrayWithObjects:@"vip.ip98.net",@"110.ip98.net", nil];
            break;
        }
        default:
            break;
    }
    return servers;
}
@end
