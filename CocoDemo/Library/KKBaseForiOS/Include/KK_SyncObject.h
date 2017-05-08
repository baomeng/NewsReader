//
//  KK_SyncObject.h
//  SeegleConference
//
//  Created by  yuanjun on 11-11-24.
//  Copyright 2011年 seegle.com. All rights reserved.
//

#ifndef __KK_SYNCOBJECT_H__
#define __KK_SYNCOBJECT_H__

#include "KK_Includes.h"

class KK_Event {
    
    NSCondition         *mlocker;
    bool                mIsSignal;
    bool                mIsAuto;
public:
    
    KK_Event(bool isAuto = false);
    ~KK_Event();
    
    void SetAutoMode(bool isAuto);
    
    void SetEvent(bool isSignal);
    void WaitSignal();
};

#endif //__KK_SYNCOBJECT_H__