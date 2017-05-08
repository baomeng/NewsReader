//
//  Sqlite.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/19.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "Sqlite.h"

@implementation Sqlite

#import <sqlite3.h>

static sqlite3 *db;//是指向数据库的指针,我们其他操作都是用这个指针来完成

#pragma mark - 2.打开数据库

+ (void)openSqlite {
    //判断数据库是否为空,如果不为空说明已经打开
    if(db != nil) {
        NSLog(@"数据库已经打开");
        return;
    }
    
    //获取文件路径
    NSString *str = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    NSString *strPath = [str stringByAppendingPathComponent:@"my.sqlite"];
    NSLog(@"%@",strPath);
    //打开数据库
    //如果数据库存在就打开,如果不存在就创建一个再打开
    int result = sqlite3_open([strPath UTF8String], &db);
    //判断
    if (result == SQLITE_OK) {
        NSLog(@"数据库打开成功");
    } else {
        NSLog(@"数据库打开失败");
    }
}

#pragma mark - 3.增删改查
//创建表格
+ (void)createTable {
    //1.准备sqlite语句
    NSString *sqlite = [NSString stringWithFormat:@"create table if not exists 'student' ('number' integer primary key autoincrement not null,'name' text,'sex' text,'age'integer)"];
    //2.执行sqlite语句
    char *error = NULL;//执行sqlite语句失败的时候,会把失败的原因存储到里面
    int result = sqlite3_exec(db, [sqlite UTF8String], nil, nil, &error);
    //3.sqlite语句是否执行成功
    
    if (result == SQLITE_OK) {
        NSLog(@"创建表成功");
    } else {
        NSLog(@"创建表失败");
    }
}

//添加数据
+ (void)addStudent:(student *)stu {
    //1.准备sqlite语句
    NSString *sqlite = [NSString stringWithFormat:@"insert into student(number,name,age,sex) values ('%ld','%@','%ld','%@')",stu.number,stu.name,stu.age, stu.sex];
    //2.执行sqlite语句
    char *error = NULL;//执行sqlite语句失败的时候,会把失败的原因存储到里面
    int result = sqlite3_exec(db, [sqlite UTF8String], nil, nil, &error);
    if (result == SQLITE_OK) {
        NSLog(@"添加数据成功");
    } else {
        NSLog(@"添加数据失败");
    }
}

//删除数据
+ (void)delete:(student*)stu {
    //1.准备sqlite语句
    NSString *sqlite = [NSString stringWithFormat:@"delete from student where number = '%ld'",stu.number];
    //2.执行sqlite语句
    char *error = NULL;//执行sqlite语句失败的时候,会把失败的原因存储到里面
    int result = sqlite3_exec(db, [sqlite UTF8String], nil, nil, &error);
    if (result == SQLITE_OK) {
        NSLog(@"删除数据成功");
    } else {
        NSLog(@"删除数据失败%s",error);
    }
}

//修改数据
+ (void)updataWithStu:(student *)stu {
    //1.sqlite语句
    NSString *sqlite = [NSString stringWithFormat:@"update student set name = '%@',sex = '%@',age = '%ld' where number = '%ld'",stu.name,stu.sex,stu.age,stu.number];
    //2.执行sqlite语句
    char *error = NULL;//执行sqlite语句失败的时候,会把失败的原因存储到里面
    int result = sqlite3_exec(db, [sqlite UTF8String], nil, nil, &error);
    if (result == SQLITE_OK) {
        NSLog(@"修改数据成功");
    } else {
        NSLog(@"修改数据失败");
    }
}

//查询所有数据
+ (NSMutableArray*)selectWithStu {
    NSMutableArray *array = [[NSMutableArray alloc] init];
    //1.准备sqlite语句
    NSString *sqlite = [NSString stringWithFormat:@"select * from student"];
    //2.伴随指针
    sqlite3_stmt *stmt = NULL;
    //3.预执行sqlite语句
    int result = sqlite3_prepare(db, sqlite.UTF8String, -1, &stmt, NULL);//第4个参数是一次性返回所有的参数,就用-1
    if (result == SQLITE_OK) {
        NSLog(@"查询成功");
        //4.执行n次
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            student *stu = [[student alloc] init];
            //从伴随指针获取数据,第0列
            stu.number = sqlite3_column_int(stmt, 0);
            //从伴随指针获取数据,第1列
            stu.name = [NSString stringWithUTF8String:(const char *)sqlite3_column_text(stmt, 1)] ;
            //从伴随指针获取数据,第2列
            stu.sex = [NSString stringWithUTF8String:(const char *)sqlite3_column_text(stmt, 2)] ;
            //从伴随指针获取数据,第3列
            stu.age = sqlite3_column_int(stmt, 3);
            [array addObject:stu];
        }
    } else {
        NSLog(@"查询失败");
    }
    //5.关闭伴随指针
    sqlite3_finalize(stmt);
    return array;
}

#pragma mark - 4.关闭数据库
+ (void)closeSqlite {
    
    int result = sqlite3_close(db);
    if (result == SQLITE_OK) {
        NSLog(@"数据库关闭成功");
    } else {
        NSLog(@"数据库关闭失败");
    }
}

@end
