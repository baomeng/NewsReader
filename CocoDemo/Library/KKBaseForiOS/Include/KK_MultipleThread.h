//
//  KK_MultipleThread.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

@protocol KK_ThreadDelegate

- (void) threadMainWithType:(NSInteger)type object:(id)argument;

@end


@interface KK_Thread : NSThread {
    
@private
    
    id <KK_ThreadDelegate>  delegate;
    NSInteger               threadType;
    id                      parameter;
    CFRunLoopRef selfRunLoop;
    NSThread *__unsafe_unretained selfThread;
}
@property(nonatomic, readonly) CFRunLoopRef selfRunLoop;
@property(unsafe_unretained, nonatomic, readonly) NSThread *selfThread;

- (id) initWithDelegate:(id <KK_ThreadDelegate>)dele type:(NSInteger)type name:(NSString *)name object:(id)argument;

- (BOOL) startWork;

- (BOOL) stopWork;


- (NSInteger) getThreadType;


@end


@interface KK_MultipleThread : NSObject {
    
@private
    
    NSMutableArray *lstThread;
    NSLock *lstLock;
    
@protected
    
    id <KK_ThreadDelegate>  __unsafe_unretained delegate;
}

@property(unsafe_unretained) id <KK_ThreadDelegate>  delegate;

- (BOOL) beginOneThreadWithType:(NSInteger)type name:(NSString *)name object:(id)argument;

- (NSInteger) beginSomeThreadWithType:(NSInteger)type threadCount:(NSInteger)count name:(NSString *)name object:(id)argument;    

- (void) endAllThread;

- (void) endSomeThreadWithType:(NSInteger)type;

- (void) endOneThread:(NSThread *) thread;

- (NSInteger) getThreadCount; 

- (NSInteger) getThreadCountWithType:(NSInteger)type;

- (NSThread *)getThreadWithType:(NSInteger)type atIndex:(NSUInteger)xh;
- (CFRunLoopRef)getRunLoopRefWithType:(NSInteger)type atIndex:(NSUInteger)xh;

- (BOOL) isThreadStop:(NSThread *) thread;

@end
