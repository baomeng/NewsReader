//
//  SGMultipleTimer.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "KK_Includes.h"
#import "KK_MultipleThread.h"
#import "KK_MsgQueue.h"

@protocol KK_MultipleTimerDelegate

-(void) timerEvent:(u_longlong)eventID withData:(u_longlong)argument withObject:(id)object;

@end


#define TVN_BITS 6
#define TVR_BITS 8
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

struct TIMER_NODE{
    
    struct TIMER_NODE * next;
    struct TIMER_NODE * prev;
    u_longlong      event;      // 定时器ID
    unsigned long   period;		// 定时周期。
    unsigned long   expires;	// 下次定时器到期的时间
    u_longlong      data;       // 用户数据
    id              object;
    id <KK_MultipleTimerDelegate> delegate;
    BOOL            bOneShot;   // 循环定时标志
};


@interface KK_MultipleTimer : NSObject <KK_ThreadDelegate> {
    
@private
    
    BOOL isStarting;
	id <KK_MultipleTimerDelegate>  multiListener;
	NSMutableDictionary *dicTask;
    NSLock *dicLock;
    
    KK_MultipleThread* multipleThread;
    
	KK_MsgQueue *msgQueue;                          // 消息队列。
	u_long nLatestTick;                             // 最近的时间，单位毫秒
	int iMsgThreadCount;
    
	struct TIMER_NODE* arrayTimerVec1[TVR_SIZE];	//no more than 256ms
	struct TIMER_NODE*  arrayTimerVec2[TVN_SIZE];	//0x100≤interval≤0x3fff的定时器			1<<(TVR_BITS+TVN_BITS)		(interval>>8)一组
	struct TIMER_NODE*  arrayTimerVec3[TVN_SIZE];	//0x4000≤interval≤0xfffff的定时器			1<<(TVR_BITS+2*TVN_BITS)	(interval>>8＋6)一组
	struct TIMER_NODE*  arrayTimerVec4[TVN_SIZE];	//0x100000≤interval≤0x3ffffff的定时器		1<<(TVR_BITS+3*TVN_BITS)	(interval>>8＋6＋6)一组
	struct TIMER_NODE*  arrayTimerVec5[TVN_SIZE];	//0x4000000≤interval≤0xffffffff的定时器	其他的						(interval>>8＋6＋6＋6)一组
	struct TIMER_NODE** arrayTimerVecs[5];          //指针数组tvecs[],来分别指向tv1、tv2、…、tv5结构变量
}

-(void) setListenter:(id <KK_MultipleTimerDelegate>) listener;
-(BOOL) beginTimer;
-(BOOL) beginTimerWithCount:(NSUInteger)count;
-(void) endTimer;

-(BOOL) setTimerWithID:(u_longlong)eventId after:(long)delay withData:(u_longlong)argument withObject:(id)object flag:(BOOL)repeat withDelegate:(id <KK_MultipleTimerDelegate>)delegate;

-(BOOL) killTimer:(u_longlong) eventId;
-(BOOL)	killTimerWithHiByte:(unsigned int)eventIdHiByte;
-(BOOL) killTimerWithHighOrLowByte:(unsigned  int)eventId flag:(BOOL)isHiByte;



@end
