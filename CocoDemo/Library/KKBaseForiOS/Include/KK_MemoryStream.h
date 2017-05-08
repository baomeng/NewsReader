//
//  KK_MemoryStream.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_MEMORY_STREAM_H__
#define __KK_MEMORY_STREAM_H__

#include "KK_Includes.h"
#include "KK_Templet.h"
#include "KK_ByteStream.h"


#define KK_MEMORY_STREAM_MEMPAGESIZE   4096     // 4*1024
#define KK_MEMORY_STREAM_NEWMAXBUFSIZE 4194304  // 4*1024*1024

class KK_MemoryStream : public KK_ByteStream
{
public:
	KK_MemoryStream(); 
	KK_MemoryStream(unsigned char* pBuf, long lLen, bool bLoad); 
    virtual ~KK_MemoryStream();
    
public:
	virtual void Init(u_long ulBlockSize = KK_MEMORY_STREAM_MEMPAGESIZE, bool bLoad = false);
	virtual void Init(unsigned char* pBuf, long lLen, bool bLoad);
	void FreeBlockBuf();
	inline void Free(unsigned char* pBuf) { KK_Assert(0); } 
    
protected:
	virtual void CheckBuffer(long lNewPos, u_long ulSize); 
	virtual void CopyBuffer(long lOffset, u_long ulSize, void* pIOData = NULL);
    
	bool ReallocBlockBuf(unsigned int nBlockCount = 1);
    
protected:
	u_long m_ulBlockSize;
	bool   m_bIsInit;
};

class KK_StorePduStream : public KK_MemoryStream
{
public:
	KK_StorePduStream();
	KK_StorePduStream(u_short usCmd);
	virtual ~KK_StorePduStream();
    
	virtual unsigned char* GetData();
    
public:
	void SetPduCmd(u_short usCmd);
};

#endif