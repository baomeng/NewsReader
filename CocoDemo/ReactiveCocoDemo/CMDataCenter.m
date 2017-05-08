//
//  CMDataCenter.m
//  CMMonitorUI
//
//  Created by xs yin on 12-6-6.
//  Copyright (c) 2012年 seegle. All rights reserved.
//

#import "CMDataCenter.h"

@implementation CMDataCenter

@synthesize propertyFilePath;
- (id) initWithPlistFile:(NSString *)fileName extendString:extend
{
    self = [super init];
    if (self) {
        // Initialize self.
        NSString * propertyFileName=@"";
        propertyFileName = [propertyFileName stringByAppendingFormat:@"%@.%@",fileName,extend];
        NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDir = [documentPaths objectAtIndex:0];	
        NSString *filePath = [documentsDir stringByAppendingPathComponent:propertyFileName];
        NSFileManager *filemgr = [NSFileManager defaultManager];
        if ([filemgr fileExistsAtPath: filePath ] == NO) {
            NSString *sourcePath = [[NSBundle mainBundle] pathForResource:fileName ofType:extend];
            [filemgr copyItemAtPath:sourcePath toPath:filePath error:NULL];
            //            NSAssert(copyResult,@"Failed to copy historyInfo file"); 			
        }
        self.propertyFilePath = filePath;
    }
    return self;
}

- (id) objectForKey:(id)aKey
{
    if (self.propertyFilePath!=nil) {
        NSFileManager *filemgr = [NSFileManager defaultManager];
        if ([filemgr fileExistsAtPath: self.propertyFilePath ] == NO) {
            return nil;
        }else{
            NSMutableDictionary *dicHistoryInfo = [NSMutableDictionary dictionaryWithContentsOfFile:self.propertyFilePath];
            return [dicHistoryInfo objectForKey:aKey];
        }
    }
    return nil;
}

- (id)getDictionaryForFile
{
    if (self.propertyFilePath!=nil) 
    {
        return [NSMutableDictionary dictionaryWithContentsOfFile:self.propertyFilePath];
    }
    return nil;
}

-(void)writeToFile:(NSMutableDictionary *)dictionary
{
    if (self.propertyFilePath!=nil)
        [dictionary writeToFile:self.propertyFilePath atomically:YES];
}

@end
