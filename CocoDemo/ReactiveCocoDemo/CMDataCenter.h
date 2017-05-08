//
//  CMDataCenter.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/14.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CMDataCenter : NSObject

@property (strong, nonatomic) NSString * propertyFilePath;

- (id) initWithPlistFile:(NSString *)fileName extendString:extend;
- (id) objectForKey:(id)aKey;
- (id) getDictionaryForFile;
-(void)writeToFile:(NSMutableDictionary *)dictionary;
@end
