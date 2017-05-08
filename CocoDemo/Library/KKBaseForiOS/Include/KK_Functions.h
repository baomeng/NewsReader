//
//  KK_Functions.h
//  SeegleConference
//
//  Created by YuanJun on 11-11-27.
//  Copyright 2011年 seegle.com. All rights reserved.
//


#import "KK_Includes.h"
#import <SystemConfiguration/SystemConfiguration.h>

typedef enum {
	NotReachable = 0,
	ReachableViaWiFi,
	ReachableViaWWAN
} NetworkStatus;

@interface KK_Functions : NSObject {

}

+ (NSString *) getUUID;

+ (NSString *) getMacAddress;

+ (NSString *) bytesToString:(unsigned char *)src len:(int)l;

+ (NetworkStatus) currentNetworkStatus;
@end

