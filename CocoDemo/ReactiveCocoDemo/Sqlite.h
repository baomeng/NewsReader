//
//  Sqlite.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/19.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "student.h"

@interface Sqlite : NSObject

+ (void)openSqlite;
+ (void)createTable;
+ (void)addStudent:(student *)stu;
+ (void)delete:(student*)stu;
+ (void)updataWithStu:(student *)stu;
+ (NSMutableArray*)selectWithStu;
+ (void)closeSqlite;

@end
