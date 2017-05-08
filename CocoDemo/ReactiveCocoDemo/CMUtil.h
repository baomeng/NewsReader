//
//  CMUtil.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/14.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CMDataCenter;

typedef enum{
    DeviceModel_iPhone,
    DeviceModel_iPad,
    DeviceModel_iPodTouch,
    DeviceModel_Unknow,
}DeviceModel;  //设备模型

@interface CMUtil : NSObject
//数据相关
+(CMDataCenter*)getDefaultHistoryDataCenter;
+(NSString*)getDefaultUserName;
+(NSString*)getDefaultPassword;
+(BOOL)getDefaultRememberPasswordTag;
+(BOOL)getDefaultAutoLoginTag;
+(NSString*)getDefaultServerIp;
+(NSNumber *)getDefaultServerPort;
+ (void)insertUserNameToHistoryDataCenter:(NSString *)name password:(NSString *)pwd rememberPassword:(BOOL)isRemember autoLogin:(BOOL) isAuto;
+ (void)insertServerIPToHistoryDataCenter:(NSString *)ip serverPort:(int)port;
+ (void)removeServerIPFromHistoryDataCenter:(NSUInteger )rowIndex;
+ (void)removeUserNameFromHistoryDataCenter:(NSUInteger )rowIndex;
+ (void)initDefaultServerInfo:(NSArray *)serverInfos serverPort:(int)port;
+ (int)getUserNameCountsInHistoryDataCenter;
+ (int)getServerIpCountsInHistoryDataCenter;
+ (NSString*)getUserNameForIndex:(int)index;
+ (NSString*)getServerIpForIndex:(int)index;
+ (BOOL)getRememberPasswordTagForIndex:(int)index;
+ (BOOL)getAutoLoginTagForIndex:(int)index;
+ (NSString*)getPasswordForIndex:(int)index;
+ (NSNumber *)getServerPortForIndex:(int)index;
+ (void)updateAutoLoginTag:(BOOL)isAuto;
+ (void)removeAllServerIPInfo;
+ (UIImage *)generatePhotoThumbnail:(UIImage *)image ;
+(NSString *)findDirectory:(NSString *)dir;
+(void)deleteAllFilesFromDir:(NSString *)dir;
+(BOOL)deleteFileFromDir:(NSString *)dir withFileName:(NSString *)name;
+(NSString *)createDirectory:(NSString *)dir;
+  (BOOL)saveImageToFile:(UIImage *)image directionary:(NSString *)dir;
//取设备模型编码
+ (DeviceModel) getDeviceModel;

+ (NSArray *) getCenterIpInfo;
@end
