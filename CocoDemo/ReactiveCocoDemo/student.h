//
//  student.h
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/19.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface student : NSObject

@property(nonatomic, assign) long number;
@property(nonatomic, strong) NSString* name;
@property(nonatomic, strong) NSString* sex;
@property(nonatomic, assign) long age;

@end
