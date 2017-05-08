//
//  User+CoreDataProperties.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/21.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "User+CoreDataProperties.h"

@implementation User (CoreDataProperties)

+ (NSFetchRequest<User *> *)fetchRequest {
	return [[NSFetchRequest alloc] initWithEntityName:@"User"];
}

@dynamic name;
@dynamic age;
@dynamic sex;

@end
