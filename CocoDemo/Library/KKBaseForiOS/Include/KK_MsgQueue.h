//
//  KK_MsgQueue.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "KK_Includes.h"
#include "KK_Templet.h"

#define KK_MSG_QUIT 1

typedef struct __KK_MSG__{
    
    u_int       message;
    u_longlong  wparam;
    u_longlong  lparam;
    id          wobject;
    id          lobject;
    
}KK_MSG;


@protocol KK_MsgQueueDelegate 

- (void) deleteQueueMsg:(KK_MSG *)lpMsg;

@end

@interface KK_MsgQueue : NSObject {
    
@private
    
    KK_List<KK_MSG, const KK_MSG&> queueMsg;
    id <KK_MsgQueueDelegate>    delegate;
    NSConditionLock *msgLock;
}

- (id)   initWithDelegate:(id <KK_MsgQueueDelegate>)dele;

- (BOOL) postMsg:(const KK_MSG *)msg;
- (BOOL) postMsg:(u_int)msg wParam:(u_longlong)w lParam:(u_longlong)l object:(id)object;
- (BOOL) postMultiMsg:(const KK_MSG *)ar size:(int)nSize;

- (BOOL) postQuitMsg;
- (BOOL) postQuitMsgWithFlag:(BOOL)isClear;
- (BOOL) postQuitMsgWithCount:(int)count;
- (BOOL) postQuitMsgWithCount:(int)count flag:(BOOL)isClear; 

- (BOOL) getMsg:(KK_MSG *)msg;
- (BOOL) peekMsg:(KK_MSG *)msg;

- (void) clearQueue;

@end
