//
//  KK_Buffer.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef _KK_BUFFER_H_
#define _KK_BUFFER_H_

#include "KK_ByteStream.h"

#define MAX_BUFFER_SIZE (4*1024*1024)
#define BUF_GROW_LEN 	(4*1024)

class KK_Buffer : public KK_StreamObjectEx
{
public:
	KK_Buffer();
	KK_Buffer(const KK_Buffer& pBuf);
	KK_Buffer(const u_char* lpBuf, int nDataLen);
	virtual ~KK_Buffer();
	
public:
	void Release();
	inline void Clean() { m_nDataLen = 0; }
    
	inline u_int GetBufSize() const { return m_nBufSize; }	
	inline u_int GetDataLen() const { return m_nDataLen; }
	inline u_char* GetBuffer() const { return m_lpBuf; }
	inline u_char* GetWritePos() { return m_lpBuf + m_nDataLen; }
	
	bool SetBufSize(int nSize);
	bool SetDataLen(int nLen);
	bool AppendDataLen(int nLen);
    
	bool ResetData(const u_char* lpBuf, int nDataLen);
	bool ResetData(const KK_Buffer& pBuf);
	
	bool WriteData(int nStart, const u_char* lpBuf, int nDataLen);
	bool WriteData(int nStart, const KK_Buffer& pBuf);
	
	bool InsertData(int nStart, const u_char* lpBuf, int nDataLen);
	bool InsertData(int nStart, const KK_Buffer& pBuf);
	
	bool AppendData(const u_char* lpBuf, int nDataLen);
	bool AppendData(const KK_Buffer& pBuf);
    
	bool DeleteData(int nStart, int nDelLen);
    
	inline operator void* () const { return m_lpBuf; }
    
	const KK_Buffer& operator = (const KK_Buffer& pBuf);
	KK_Buffer& operator += (const KK_Buffer& pBuf);
	friend KK_Buffer operator + (const KK_Buffer& pBufHead, const KK_Buffer& pBufTail);
    
	virtual void SerializeFrom(KK_ByteStream& is);
	virtual void SerializeTo(KK_ByteStream& os) const;
    
public:
	int Find(const void *lpBuf, int nDataLen);
    
public:
	static int Find(const void *lpBuf1, int nDataLen1, const void *lpBuf2, int nDataLen2);
    
protected:
	u_char* m_lpBuf;
	u_int m_nBufSize;
	u_int m_nDataLen;
};


#endif	