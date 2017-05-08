//
//  KK_Locker.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//



#ifndef __KK_LOCKER_H__
#define __KK_LOCKER_H__


#include "KK_Includes.h"
#include "KK_Templet.h"


#define LOCK() Lock()
#define TRY_LOCK() TryLock()
#define UNLOCK() Unlock()
#define AUTO_LOCK(al, lck_ptr) KK_AutoLock al(lck_ptr)


class KK_Locker
{
public:
	KK_Locker();
	virtual ~KK_Locker();
    
public:
    
	void Lock();
	bool TryLock();
	void Unlock();
    void SetName(NSString *str);
    
private:
    
	NSLock* m_nsLock;
};

class KK_AutoLock
{
public:
	KK_AutoLock(KK_Locker * lck);
	~KK_AutoLock();
	void Release();
    
private:
	KK_Locker * m_lplck;
};


template<class TYPE, class ARG_TYPE>
class KK_LckList : public KK_List<TYPE, ARG_TYPE>, public KK_Locker 
{
public:
	KK_LckList() {}
};

template<class TYPE, class ARG_TYPE>
class KK_LckArray : public KK_Array<TYPE, ARG_TYPE>, public KK_Locker 
{
public:
	KK_LckArray() {}
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class KK_LckMap : public KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>, public KK_Locker
{
public:
	KK_LckMap() {}
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class KK_LckAvlTree : public KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>, public KK_Locker
{
public:
	KK_LckAvlTree() {}
};


#endif // __KK_LOCKER_H__






