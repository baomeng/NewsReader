
/*-------------------------------------------------------------------------*/
/*																		   */
/*	KK_ByteStream.h														   */
/*																		   */
/*	Copyright (c) 2003 Seegle Soft Ltd.       							   */
/*  All rights reserved													   */
/*																		   */
/*-------------------------------------------------------------------------*/

#ifndef __KK_BYTE_STREAM_H__
#define __KK_BYTE_STREAM_H__

#include "KK_Includes.h"
#include "KK_Utf8_Unicode.h"
#include "KK_String.h"
#include "KK_DateTimeEx.h"


#define ERROR_BASE				0x81000000
#define ERROR_READ_BUF			ERROR_BASE + 0x01
#define ERROR_WRITE_BUF			ERROR_BASE + 0x02
#define ERROR_ALLOC_BUF			ERROR_BASE + 0x03
#define ERROR_READ_DATA			ERROR_BASE + 0x04
#define ERROR_READ_FILE         ERROR_BASE + 0x05
#define ERROR_WRITE_FILE        ERROR_BASE + 0x06

class KK_ByteStream;

class KK_StreamObject  
{
public:
	virtual void SerializeFrom(KK_ByteStream& is) = 0;
	virtual void SerializeTo(KK_ByteStream& os) const = 0;
};  

class KK_StreamObjectEx
{
public:
	virtual void SerializeFrom(KK_ByteStream& is) = 0;
	virtual void SerializeTo(KK_ByteStream& os) const = 0;
};

class KK_ByteStream
{
public :
	KK_ByteStream() 
	{
		m_pBuf = NULL;
		m_lCurPos = 0;
		m_lBufLen = 0;
		m_bLoad = false;
	}
    
	KK_ByteStream(unsigned char* pBuf, long lLen, bool bLoad) 
	{ 
		Init(pBuf, lLen, bLoad); 
	}
    
	virtual void Init(unsigned char* pBuf, long lLen, bool bLoad) 
	{
		KK_Assert(!bLoad || pBuf);
		KK_Assert(lLen >= 0);
		m_pBuf = pBuf;
		m_lCurPos = 0;
		m_lBufLen = lLen;
		m_bLoad = bLoad;
	}
    
	virtual inline unsigned char* GetData() { return m_pBuf; }
    
	inline bool IsLoading() const { return m_bLoad; }
	inline bool IsStoring() const {	return !m_bLoad; }
    
protected:
	void CheckBuffer(long lNewPos, u_long ulSize)
	{
		if (lNewPos < 0 || lNewPos + (long)ulSize > m_lBufLen) 
		{
			long lError = m_bLoad ? ERROR_READ_BUF : ERROR_WRITE_BUF;
			throw lError;
		}
	}
    
	virtual void CopyBuffer(long lOffset, u_long ulSize, void * pIOData = NULL) 
	{
		long lNewPos = m_lCurPos + lOffset;
		CheckBuffer(lNewPos, ulSize);
        
		m_lCurPos = lNewPos + ulSize;
		if (pIOData == NULL) return;
        
		if (m_bLoad) memcpy(pIOData, m_pBuf + lNewPos, ulSize);
		else if (m_pBuf) memcpy(m_pBuf + lNewPos, pIOData, ulSize);
	}
    
public :
	inline void ReadBytes(void *pData, u_long ulSize) 
	{
		KK_Assert(m_bLoad && pData);
		if (ulSize == 0) return;
		CopyBuffer(0, ulSize, pData);
	}
    
	inline void WriteBytes(const void *pData, u_long ulSize) 
	{
		KK_Assert(!m_bLoad && pData);
		if (ulSize == 0) return;
		CopyBuffer(0, ulSize, (void*)pData);
	}
    
	inline long Seek(long lNewPos)
	{
		long lOldPos = m_lCurPos;
		CopyBuffer(lNewPos - m_lCurPos, 0);
		return lOldPos;
	}
    
	inline void Skip(long lOffset) 
	{
		CopyBuffer(lOffset, 0);
	}
    
	inline long Tell() { return m_lCurPos; }
	inline long GetUnusedBufSize() { return m_lBufLen - m_lCurPos; }
    
	int ReadString(KK_StringA & str, bool bFixedLen = false)
	{
		KK_StringU strUtf8;
		ReadString(strUtf8, bFixedLen);
		str = strUtf8;
		return str.GetLength()+1;
	}
    
	int ReadString(KK_StringW & str, bool bFixedLen = false) 
	{
		KK_StringU strUtf8;
		ReadString(strUtf8, bFixedLen);
		str = strUtf8;
		return str.GetLength()+1;
	}
    
	int ReadString(KK_StringU & str, bool bFixedLen = false) 
	{
		long lLen = bFixedLen ? ReadULong() : ReadLength();
		CheckBuffer(m_lCurPos, lLen);
		ReadBytes(str.GetBufferSetLength(lLen), lLen);
		return str.GetLength()+1;
	}
    
	inline void WriteString(const char* str, bool bFixedLen = false) 
	{
        // #ifdef _UTF8
        // 
        // 		unsigned int len = 0;
        // 		if (str) len = strlen(str);
        // 		bFixedLen ? WriteULong(len) : WriteLength(len);
        // 		if (len > 0) WriteBytes(str, len); 
        // 
        // #else // end of define _UTF8
        // 
		WriteString(KK_StringW(str), bFixedLen);
        // 
        // #endif // end of not define _UTF8
	}
    
	//inline void WriteString(const wchar_t * str, bool bFixedLen = false) { WriteString(KK_StringU(str), bFixedLen); }
	inline void WriteString(const WCHAR* str, bool bFixedLen = false) { WriteString(KK_StringU(str), bFixedLen); }
	inline void WriteString(const KK_StringA & str, bool bFixedLen = false) { WriteString(KK_StringU(str), bFixedLen); }
	inline void WriteString(const KK_StringW & str, bool bFixedLen = false) { WriteString(KK_StringU(str), bFixedLen); }
    
	inline void WriteString(const KK_StringU & strUtf8, bool bFixedLen = false)
	{
		if (bFixedLen)
			WriteULong(strUtf8.GetLength());
		else
            WriteLength(strUtf8.GetLength());
        
		if (!strUtf8.IsEmpty())	WriteBytes(strUtf8, strUtf8.GetLength()); 
		return;
	}
    
	unsigned long ReadLength() 
	{
		unsigned char lenBytes[5];
        
		ReadBytes(lenBytes, 1);
        
		if (lenBytes[0] <= 0x7f) 
			return (unsigned long)lenBytes[0];
		else if (lenBytes[0] <= 0xbf) {
			ReadBytes(lenBytes+1, 1);
			
			unsigned long len;
			len = (lenBytes[0] & 0x3f);
			len <<= 8;
			len += lenBytes[1];
			
			return len;
		}
		else if (lenBytes[0] <= 0xdf) {
			ReadBytes(lenBytes+1, 2);
            
			unsigned long len;
  			len = (lenBytes[0] & 0x1f);
			len <<= 8;
			len += lenBytes[1];
			len <<= 8;
			len += lenBytes[2];
			
			return len;
		}
		else if (lenBytes[0] <= 0xef) {
			ReadBytes(lenBytes+1, 3);
            
			unsigned long len;
			len = (lenBytes[0] & 0x0f);
			len <<= 8;
			len += lenBytes[1];
			len <<= 8;
			len += lenBytes[2];
			len <<= 8;
			len += lenBytes[3];
			
			return len;
		}
		else if (lenBytes[0] == 0xf0) {
			ReadBytes(lenBytes+1, 4);
            
			unsigned long len;
			len = lenBytes[1];
			len <<= 8;
			len += lenBytes[2];
			len <<= 8;
			len += lenBytes[3];
			len <<= 8;
			len += lenBytes[4];
			
			return len;
		}
		else {
			long lError = ERROR_READ_BUF;
			throw lError;
			return 0;
		}
	}
    
	int WriteLength(unsigned long len) {
		int count = 0;
		unsigned char lenBytes[5];
        
		if (len <= 0x7f) {
			lenBytes[count++] = (unsigned char)len;
		}
		else if (len <= 0x3fff) {
			lenBytes[count++] = (unsigned char)((len & 0x0000ff00) >> 8) + 0x80;
			lenBytes[count++] = (unsigned char)(len & 0x000000ff);
		}
		else if (len <= 0x1fffff) {
			lenBytes[count++] = (unsigned char)((len & 0x00ff0000) >> 16) + 0xc0;
			lenBytes[count++] = (unsigned char)((len & 0x0000ff00) >> 8);
			lenBytes[count++] = (unsigned char)(len & 0x000000ff);
		}
		else if (len <= 0x0fffffff) {
			lenBytes[count++] = (unsigned char)((len & 0xff000000) >> 24) + 0xe0;
			lenBytes[count++] = (unsigned char)((len & 0x00ff0000) >> 16);
			lenBytes[count++] = (unsigned char)((len & 0x0000ff00) >> 8);
			lenBytes[count++] = (unsigned char)(len & 0x000000ff);
		}
		else {
			lenBytes[count++] = (unsigned char)0xf0;
			lenBytes[count++] = (unsigned char)((len & 0xff000000) >> 24);
			lenBytes[count++] = (unsigned char)((len & 0x00ff0000) >> 16);
			lenBytes[count++] = (unsigned char)((len & 0x0000ff00) >> 8);
			lenBytes[count++] = (unsigned char)(len & 0x000000ff);
		}
        
		WriteBytes(lenBytes, count);
		return count;
	}
    
    inline void WriteKKDateEx(const KK_DateEx& date)
	{
		WriteLong(date.GetDate());
	}
    
	inline void WriteKKTimeEx(const KK_TimeEx& time)
	{
		WriteLong(time.GetTime());
	}
    
    inline void WriteKKDatetimeEx(const KK_DatetimeEx& stamptime)
	{
		WriteLong(stamptime.GetDate());
        WriteLong(stamptime.GetTime());
	}
    
	inline KK_ByteStream& operator << (char ch)
	{ 
		WriteBytes(&ch, sizeof(char)); 
		return *this; 
	}
    
	inline void WriteChar(char ch)
	{ 
		WriteBytes(&ch, sizeof(char)); 
	}
    
    //+++++++++++++++++++++++++++++++++++++++++++
	inline KK_ByteStream& operator << (wchar_t ch)
	{ 
		WriteBytes(&ch, sizeof(wchar_t)); 
		return *this; 
	}
    
	inline void WriteWChar(wchar_t ch)
	{ 
		WriteBytes(&ch, sizeof(wchar_t)); 
	}
    /*
     inline KK_ByteStream& operator << (unsigned wchar_t ch)
     { 
     WriteBytes(&ch, sizeof(unsigned wchar_t)); 
     return *this; 
     }
     
     inline void WriteWChar(unsigned wchar_t ch)
     { 
     WriteBytes(&ch, sizeof(unsigned wchar_t)); 
     }*/
    //+++++++++++++++++++++++++++++++++++++++++++
    
	inline KK_ByteStream& operator << (unsigned char ch)
	{ 
		WriteBytes(&ch, sizeof(unsigned char)); 
		return *this; 
	}
    
	inline void WriteUChar(unsigned char ch)
	{ 
		WriteBytes(&ch, sizeof(unsigned char)); 
	}
    
	inline KK_ByteStream& operator << (bool b)
	{
		b = b?1:0; WriteBytes(&b, sizeof(bool)); 
		return *this; 
	}
    
	inline void WriteBool(bool b)
	{ 
		b = b?1:0; 
		WriteBytes(&b, sizeof(bool)); 
	}
    
	inline void WriteBoolArray(bool b0 = false, bool b1 = false, bool b2 = false, bool b3 = false,
                               bool b4 = false, bool b5 = false, bool b6 = false, bool b7 = false)
	{ 
        unsigned char ch = (b0 ? 0x1 : 0) |
        (b1 ? 0x2 : 0) |
        (b2 ? 0x4 : 0) |
        (b3 ? 0x8 : 0) |
        (b4 ? 0x10 : 0) |
        (b5 ? 0x20 : 0) |
        (b6 ? 0x40 : 0) |
        (b7 ? 0x80 : 0);
        WriteBytes(&ch, 1);
	}
    
	inline KK_ByteStream& operator << (short s)
	{ 
		Swap(&s, sizeof(short)); 
		WriteBytes(&s, sizeof(short)); 
		return *this; 
	}
    
	inline void WriteShort(short s)
	{ 
		Swap(&s, sizeof(short)); 
		WriteBytes(&s, sizeof(short)); 
	}
    
	inline KK_ByteStream& operator << (unsigned short s)
	{ 
		Swap(&s, sizeof(unsigned short)); 
		WriteBytes(&s, sizeof(unsigned short));
		return *this; 
	}
    
	inline void WriteUShort(unsigned short s)
	{ 
		Swap(&s, sizeof(unsigned short)); 
		WriteBytes(&s, sizeof(unsigned short));	
	}
    
	inline KK_ByteStream& operator << (int i)
	{ 
		Swap(&i, sizeof(int)); 
		WriteBytes(&i, sizeof(int)); 
		return *this; 
	}
    
	inline void WriteInt(int i)
	{ 
		Swap(&i, sizeof(int)); 
		WriteBytes(&i, sizeof(int));
	}
    
	inline KK_ByteStream& operator << (unsigned int i)
	{ 
		Swap(&i, sizeof(unsigned int)); 
		WriteBytes(&i, sizeof(unsigned int));
		return *this;
	}
    
	inline void WriteUInt(unsigned int i)
	{ 
		Swap(&i, sizeof(unsigned int));
		WriteBytes(&i, sizeof(unsigned int));
	}
    
	inline KK_ByteStream& operator << (long l)
	{ 
		Swap(&l, sizeof(long)); 
		WriteBytes(&l, sizeof(long)); 
		return *this; 
	}
    
	inline void WriteLong(long l)
	{ 
		Swap(&l, sizeof(long)); 
		WriteBytes(&l, sizeof(long)); 
	}
    
	inline KK_ByteStream& operator << (unsigned long l)
	{ 
		Swap(&l, sizeof(unsigned long)); 
		WriteBytes(&l, sizeof(unsigned long)); 
		return *this; 
	}
    
	inline void WriteULong(unsigned long l)
	{ 
		Swap(&l, sizeof(unsigned long)); 
		WriteBytes(&l, sizeof(unsigned long)); 
	}
    
	inline KK_ByteStream& operator << (float f)
	{ 
		Swap(&f, sizeof(float)); 
		WriteBytes(&f, sizeof(float)); 
		return *this; 
	}
    
	inline void WriteFloat(float f)
	{ 
		Swap(&f, sizeof(float)); 
		WriteBytes(&f, sizeof(float)); 
	}
    
	inline KK_ByteStream& operator << (double d)
	{ 
		Swap(&d, sizeof(double)); 
		WriteBytes(&d, sizeof(double));
		return *this; 
	}
    
	inline void WriteDouble(double d)
	{ 
		Swap(&d, sizeof(double)); 
		WriteBytes(&d, sizeof(double));
	}
    
	inline KK_ByteStream& operator << (__int64 d)
	{ 
		Swap(&d, sizeof(__int64)); 
		WriteBytes(&d, sizeof(__int64)); 
		return *this;
	}
	
	inline void WriteInt64(__int64 d)
	{ 
		Swap(&d, sizeof(__int64)); 
		WriteBytes(&d, sizeof(__int64)); 
	}
    
	inline KK_ByteStream& operator << (unsigned __int64 d)
	{ 
		Swap(&d, sizeof(unsigned __int64)); 
		WriteBytes(&d, sizeof(unsigned __int64)); 
		return *this; 
	}
    
	inline void WriteUInt64(unsigned __int64 d)
	{ 
		Swap(&d, sizeof(unsigned __int64)); 
		WriteBytes(&d, sizeof(unsigned __int64)); 
	}
    
	inline KK_ByteStream& operator << (const KK_StreamObject& so)
	{ 
		so.SerializeTo(*this); 
		return *this; 
	}
    
	inline KK_ByteStream& operator << (const KK_StreamObjectEx& so)
	{
		long posStart, posEnd;
		posStart = Tell();
		Skip(4);
		so.SerializeTo(*this);
		posEnd = Tell();
		Seek(posStart);
		WriteULong(posEnd-posStart);
		Seek(posEnd);
		return *this;
	}
    
	inline KK_ByteStream& operator << (const KK_StringA& str)
	{
		WriteString(str, false); 
		return *this; 
	}
    
	inline KK_ByteStream& operator << (const KK_StringW& str)
	{ 
		WriteString(str, false); 
		return *this; 
	}
    
	inline KK_ByteStream& operator << (const KK_StringU& str)
	{ 
		WriteString(str, false); 
		return *this; 
	}
    
    inline KK_ByteStream& operator << (const KK_DateEx& date)
	{
        WriteLong(date.GetDate());
        return *this;
	}
	
    inline KK_ByteStream& operator << (const KK_TimeEx& time)
	{
        WriteLong(time.GetTime());
        return *this;
	}
    
    inline KK_ByteStream& operator << (const KK_DatetimeEx& stamptime)
	{
		WriteLong(stamptime.GetDate());
        WriteLong(stamptime.GetTime());
		return *this;
	}
    
	inline KK_ByteStream& operator >> (char& ch)
	{ 
		ReadBytes(&ch, sizeof(char)); 
		return *this; 
	}
    
	inline char ReadChar()
	{ 
		char ch;
		ReadBytes(&ch, sizeof(char));
		return ch;
	}
    
	inline KK_ByteStream& operator >> (unsigned char& ch)
	{ 
		ReadBytes(&ch, sizeof(unsigned char)); 
		return *this; 
	}
    
	inline unsigned char ReadUChar( )
	{ 
		unsigned char ch;
		ReadBytes(&ch, sizeof(unsigned char)); 
		return ch;
	}
    
    //++++++++++++++++++++++++++++++++++++++++++++
	inline KK_ByteStream& operator >> (wchar_t& ch)
	{ 
		ReadBytes(&ch, sizeof(wchar_t)); 
		return *this; 
	}
    
	inline char ReadWChar()
	{ 
		wchar_t ch;
		ReadBytes(&ch, sizeof(wchar_t));
		return ch;
	}
    /*
     inline KK_ByteStream& operator >> (unsigned wchar_t& ch)
     { 
     ReadBytes(&ch, sizeof(unsigned wchar_t)); 
     return *this; 
     }
     
     inline unsigned char ReadUWChar( )
     { 
     unsigned wchar_t ch;
     ReadBytes(&ch, sizeof(unsigned wchar_t)); 
     return ch;
     }*/
    //++++++++++++++++++++++++++++++++++++++++++++
    
	inline KK_ByteStream& operator >> (bool& b)
	{ 
		ReadBytes(&b, 1); b=(b==0)?false:true; 
		return *this; 
	}
    
	inline bool ReadBool()
	{ 
		bool b;
		ReadBytes(&b, 1); b=(b==0)?false:true;
		return b; 
	}
    
	inline void ReadBool(bool& b0, bool& b1)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
	}
    
	inline void ReadBool(bool& b0, bool& b1, bool& b2)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
		b2 = ((ch & 0x04) != 0);
	}
    
	inline void ReadBool(bool& b0, bool& b1, bool& b2, bool& b3)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
		b2 = ((ch & 0x04) != 0);
		b3 = ((ch & 0x08) != 0);
	}
	
	inline void ReadBool(bool& b0, bool& b1, bool& b2, bool& b3,
                         bool& b4)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
		b2 = ((ch & 0x04) != 0);
		b3 = ((ch & 0x08) != 0);
		b4 = ((ch & 0x10) != 0);
	}
    
	inline void ReadBool(bool& b0, bool& b1, bool& b2, bool& b3,
                         bool& b4, bool& b5)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
		b2 = ((ch & 0x04) != 0);
		b3 = ((ch & 0x08) != 0);
		b4 = ((ch & 0x10) != 0);
		b5 = ((ch & 0x20) != 0);
	}
	
	inline void ReadBool(bool& b0, bool& b1, bool& b2, bool& b3,
                         bool& b4, bool& b5, bool& b6)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
		b2 = ((ch & 0x04) != 0);
		b3 = ((ch & 0x08) != 0);
		b4 = ((ch & 0x10) != 0);
		b5 = ((ch & 0x20) != 0);
		b6 = ((ch & 0x40) != 0);
		
	}
    
	inline void ReadBool(bool& b0, bool& b1, bool& b2, bool& b3,
                         bool& b4, bool& b5, bool& b6, bool& b7)
	{ 
		unsigned char ch;
		ReadBytes(&ch, 1);
		b0 = ((ch & 0x01) != 0);
		b1 = ((ch & 0x02) != 0);
		b2 = ((ch & 0x04) != 0);
		b3 = ((ch & 0x08) != 0);
		b4 = ((ch & 0x10) != 0);
		b5 = ((ch & 0x20) != 0);
		b6 = ((ch & 0x40) != 0);
		b7 = ((ch & 0x80) != 0);
	}
    
	inline KK_ByteStream& operator >> (short& s)
	{ 
		ReadBytes(&s, sizeof(short)); 
		Swap(&s, sizeof(short)); 
		return *this; 
	}
    
	inline short ReadShort()
	{ 
		short s; 
		ReadBytes(&s, sizeof(short)); 
		Swap(&s, sizeof(short)); 
		return s; 
	}
    
	inline KK_ByteStream& operator >> (unsigned short& s)
	{ 
		ReadBytes(&s, sizeof(unsigned short)); 
		Swap(&s, sizeof(unsigned short));
		return *this; 
	}
	
	inline unsigned short ReadUShort()
	{ 
		unsigned short s; 
		ReadBytes(&s, sizeof(unsigned short)); 
		Swap(&s, sizeof(unsigned short)); 
		return s; 
	}
    
	inline KK_ByteStream& operator >> (int& i)
	{ 
		ReadBytes(&i, sizeof(int)); 
		Swap(&i, sizeof(int)); 
		return *this; 
	}
	
	inline int ReadInt()
	{ 
		int i; 
		ReadBytes(&i, sizeof(int)); 
		Swap(&i, sizeof(int)); 
		return i; 
	}
    
	inline KK_ByteStream& operator >> (unsigned int& i)
	{ 
		ReadBytes(&i, sizeof(unsigned int)); 
		Swap(&i, sizeof(unsigned int)); 
		return *this; 
	}
    
	inline unsigned int ReadUInt()
	{ 
		unsigned int i; 
		ReadBytes(&i, sizeof(unsigned int)); 
		Swap(&i, sizeof(unsigned int)); 
		return i; 
	}
    
	inline KK_ByteStream& operator >> (long& l)
	{ 
		ReadBytes(&l, sizeof(long)); 
		Swap(&l, sizeof(long)); 
		return *this; 
	}
    
	inline long ReadLong()
	{ 
		long l; 
		ReadBytes(&l, sizeof(long)); 
		Swap(&l, sizeof(long)); 
		return l; 
	}
    
	inline KK_ByteStream& operator >> (unsigned long& l)
	{ 
		ReadBytes(&l, sizeof(unsigned long)); 
		Swap(&l, sizeof(unsigned long)); 
		return *this; 
	}
	
	inline unsigned long ReadULong()
	{ 
		unsigned long l; 
		ReadBytes(&l, sizeof(unsigned long)); 
		Swap(&l, sizeof(unsigned long)); 
		return l; 
	}
    
	inline KK_ByteStream& operator >> (float& f)
	{ 
		ReadBytes(&f, sizeof(float)); 
		Swap(&f, sizeof(float)); 
		return *this; 
	}
    
	inline float ReadFloat()
	{ 
		float f; 
		ReadBytes(&f, sizeof(float)); 
		Swap(&f, sizeof(float)); 
		return f; 
	}
    
	inline KK_ByteStream& operator >> (double& d)
	{ 
		ReadBytes(&d, sizeof(double)); 
		Swap(&d, sizeof(double)); 
		return *this; 
	}
    
	inline double ReadDouble()
	{ 
		double d; 
		ReadBytes(&d, sizeof(double)); 
		Swap(&d, sizeof(double)); 
		return d; 
	}
    
	inline KK_ByteStream& operator >> (__int64& d)
	{ 
		ReadBytes(&d, sizeof(__int64)); 
		Swap(&d, sizeof(__int64)); 
		return *this; 
	}
    
	inline __int64 ReadInt64()
	{
		__int64 n; 
		ReadBytes(&n, sizeof(__int64)); 
		Swap(&n, sizeof(__int64)); 
		return n; 
	}
    
	inline KK_ByteStream& operator >> (unsigned __int64& d)
	{ 
		ReadBytes(&d, sizeof(unsigned __int64)); 
		Swap(&d, sizeof(unsigned __int64));
		return *this; 
	}
    
	inline unsigned __int64 ReadUInt64()
	{ 
		unsigned __int64 n; 
		ReadBytes(&n, sizeof(unsigned __int64)); 
		Swap(&n, sizeof(unsigned __int64)); 
		return n; 
	}
    
	inline KK_ByteStream& operator >> (KK_StreamObject& so)
	{ 
		so.SerializeFrom(*this); 
		return *this; 
	}
    
	inline KK_ByteStream& operator >> (KK_StreamObjectEx& so)
	{
		long posStart, len;
		posStart = Tell();
		len = ReadULong();	
        
		CheckBuffer(posStart, len);
		long lBufLen = m_lBufLen;
		m_lBufLen = posStart+len;
        
		so.SerializeFrom(*this);
        
		m_lBufLen = lBufLen;
		Seek(posStart + len);
        
		return *this;
	}
    
	inline KK_ByteStream& operator >> (KK_StringA& str)
	{ 
		ReadString(str, false); 
		return *this; 
	}
    
	inline KK_ByteStream& operator >> (KK_StringW& str)
	{ 
		ReadString(str, false);
		return *this; 
	}
    
	inline KK_ByteStream& operator >> (KK_StringU& str)
	{ 
		ReadString(str, false); 
		return *this; 
	}
    
	inline KK_ByteStream& operator >> (KK_DateEx& date)
	{
        long ldate = ReadLong();
		date.SetDate(ldate);
		return *this;
	}
    
	inline bool ReadKKDateEx(KK_DateEx& date)
	{
		return date.SetDate(ReadLong());
	}
    
	inline KK_ByteStream& operator >> (KK_TimeEx& time)
	{
        long ltime = ReadLong();
		time.SetTime(ltime);
		return *this;
	}
    
    inline bool ReadKKTimeEx(KK_TimeEx& time)
	{
		return time.SetTime(ReadLong());
	}
    
	inline KK_ByteStream& operator >> (KK_DatetimeEx& stamptime)
	{
        long ldate = ReadLong();
		long ltime = ReadLong();
		stamptime.SetDatetime(ldate, ltime);
		return *this;
	}
    
	inline bool ReadKKDatetimeEx(KK_DatetimeEx& stamptime)
	{
		long ldate = ReadLong();
		long ltime = ReadLong();
		
		return stamptime.SetDatetime(ldate, ltime);
	}
    
	inline static void ReadStreamObject(unsigned char* pBuf, long off, KK_StreamObject & so)
	{ 
		KK_ByteStream is(pBuf+off, 0x7fffffff, true); 
		is >> so; 
	}
    
	inline static unsigned char ReadUChar(unsigned char* pBuf, long off)
	{ 
		return pBuf[off]; 
	}
	
	inline static char ReadChar(unsigned char* pBuf, long off)
	{ 
		return (char)pBuf[off]; 
	}
    
    //+++++++++++++++++++++++++++++++++++++++++++++++
	inline static unsigned wchar_t ReadUWChar(unsigned char* pBuf, long off)
	{ 
		return (unsigned wchar_t)pBuf[off]; 
	}
	
    /*	inline static wchar_t ReadWChar(unsigned char* pBuf, long off)
     { 
     return (wchar_t)pBuf[off]; 
     }*/
    //+++++++++++++++++++++++++++++++++++++++++++++++
	
	inline static bool ReadBool(unsigned char* pBuf, long off)
	{ 
		return (pBuf[off] == 0)?false:true; 
	}
    
	inline static unsigned short ReadUShort(unsigned char* pBuf, long off)
	{ 
		unsigned short s; memcpy(&s, pBuf + off, sizeof(unsigned short)); 
		Swap(&s, sizeof(unsigned short)); return s; 
	}
    
	inline static short ReadShort(unsigned char* pBuf, long off)
	{ 
		short s; 
		memcpy(&s, pBuf + off, sizeof(short)); Swap(&s, sizeof(short)); 
		return s; 
	}
    
	inline static unsigned int ReadUInt(unsigned char* pBuf, long off)
	{ 
		unsigned int i; 
		memcpy(&i, pBuf + off, sizeof(unsigned int)); 
		Swap(&i, sizeof(unsigned int)); 
		return i; 
	}
    
	inline static int ReadInt(unsigned char* pBuf, long off)
	{ 
		int i; 
		memcpy(&i, pBuf + off, sizeof(int)); 
		Swap(&i, sizeof(int)); 
		return i; 
	}
    
	inline static unsigned long ReadULong(unsigned char* pBuf, long off)
	{ 
		unsigned long l; 
		memcpy(&l, pBuf + off, sizeof(unsigned long)); 
		Swap(&l, sizeof(unsigned long)); 
		return l; 
	}
	
	inline static long ReadLong(unsigned char* pBuf, long off)
	{ 
		long l; 
		memcpy(&l, pBuf + off, sizeof(long)); 
		Swap(&l, sizeof(long)); 
		return l; 
	}
    
	inline static float ReadFloat(unsigned char* pBuf, long off)
	{ 
		float f; 
		memcpy(&f, pBuf + off, sizeof(float)); 
		Swap(&f, sizeof(float)); 
		return f; 
	}
    
	inline static double ReadDouble(unsigned char* pBuf, long off)
	{ 
		double d; 
		memcpy(&d, pBuf + off, sizeof(double)); 
		Swap(&d, sizeof(double)); 
		return d; 
	}
    
	static unsigned long ReadLength(unsigned char* pBuf, long off)
	{
		unsigned char length[5];
        
		length[0] = pBuf[off++];
        
		if (length[0] <= 0x7f)
		{
			return (unsigned long)length[0];
		}
		else if (length[0] <= 0xbf)
		{
			unsigned long len;
			length[1] = pBuf[off++];
			len = (length[0] & 0x3f);
			len <<= 8;
			len += length[1];
			
			return len;
		}
		else if (length[0] <= 0xdf)
		{
			unsigned long len;
			length[1] = pBuf[off++];
			length[2] = pBuf[off++];
  			len = (length[0] & 0x1f);
			len <<= 8;
			len += length[1];
			len <<= 8;
			len += length[2];
			
			return len;
		}
		else if (length[0] <= 0xef)
		{
			unsigned long len;
			length[1] = pBuf[off++];
			length[2] = pBuf[off++];
			length[3] = pBuf[off++];
			len = (length[0] & 0x0f);
			len <<= 8;
			len += length[1];
			len <<= 8;
			len += length[2];
			len <<= 8;
			len += length[3];
			
			return len;
		}
		else if (length[0] == 0xf0)
		{
			unsigned long len;
			length[1] = pBuf[off++];
			length[2] = pBuf[off++];
			length[3] = pBuf[off++];
			length[4] = pBuf[off++];
			len = length[1];
			len <<= 8;
			len += length[2];
			len <<= 8;
			len += length[3];
			len <<= 8;
			len += length[4];
			
			return len;
		}
        else {
            
            return 0;
        }
	}
    
	inline static int WriteStreamObject(unsigned char* pBuf, long off, const KK_StreamObject & so)
	{ 
		KK_ByteStream os(pBuf+off, 0x7fffffff, false); 
		os << so; return os.Tell(); 
	}
    
	inline static int WriteUChar(unsigned char* pBuf, long off, unsigned char ch)
	{ 
		Swap(&ch, sizeof(unsigned char)); 
		memcpy(pBuf + off, &ch, sizeof(unsigned char));
		return sizeof(unsigned char); 
	}
	
	inline static int WriteChar(unsigned char* pBuf, long off, char ch)
	{ 
		Swap(&ch, sizeof(char)); 
		memcpy(pBuf + off, &ch, sizeof(char)); 
		return sizeof(char); 
	}
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++
	inline static int WriteUWChar(unsigned char* pBuf, long off, unsigned wchar_t ch)
	{ 
		Swap(&ch, sizeof(unsigned wchar_t)); 
		memcpy(pBuf + off, &ch, sizeof(unsigned wchar_t));
		return sizeof(unsigned wchar_t); 
	}
    /*	
     inline static int WriteWChar(unsigned char* pBuf, long off, wchar_t ch)
     { 
     Swap(&ch, sizeof(wchar_t)); 
     memcpy(pBuf + off, &ch, sizeof(wchar_t)); 
     return sizeof(wchar_t); 
     }*/
    //++++++++++++++++++++++++++++++++++++++++++++++++++
    
	inline static int WriteBool(unsigned char* pBuf, long off, bool b)
	{ 
		pBuf[off] = (unsigned char)b?1:0; 
		return 1; 
	}
	
	inline static int WriteUShort(unsigned char* pBuf, long off, unsigned short s)
	{ 
		Swap(&s, sizeof(unsigned short)); 
		memcpy(pBuf + off, &s, sizeof(unsigned short)); 
		return sizeof(unsigned short); 
	}
	
	inline static int WriteShort(unsigned char* pBuf, long off, short s)
	{ 
		Swap(&s, sizeof(short));
		memcpy(pBuf + off, &s, sizeof(short)); 
		return sizeof(short); 
	}
	
	inline static int WriteUInt(unsigned char* pBuf, long off, unsigned int i)
	{ 
		Swap(&i, sizeof(unsigned int)); 
		memcpy(pBuf + off, &i, sizeof(unsigned int)); 
		return sizeof(unsigned int); 
	}
	
	inline static int WriteInt(unsigned char* pBuf, long off, int i)
	{ 
		Swap(&i, sizeof(int)); 
		memcpy(pBuf + off, &i, sizeof(int)); 
		return sizeof(int); 
	}
    
	inline static int WriteULong(unsigned char* pBuf, long off, unsigned long l)
	{ 
		Swap(&l, sizeof(unsigned long)); 
		memcpy(pBuf + off, &l, sizeof(long)); 
		return sizeof(long); 
	}
    
	inline static int WriteLong(unsigned char* pBuf, long off, long l)
	{ 
		Swap(&l, sizeof(long)); 
		memcpy(pBuf + off, &l, sizeof(long)); 
		return sizeof(long); 
	}
	
	inline static int WriteFloat(unsigned char* pBuf, long off, float f)
	{ 
		Swap(&f, sizeof(float)); 
		memcpy(pBuf + off, &f, sizeof(float)); 
		return sizeof(float); 
	}
	
	inline static int WriteDouble(unsigned char* pBuf, long off, double d)
	{ 
		Swap(&d, sizeof(double)); 
		memcpy(pBuf + off, &d, sizeof(double)); 
		return sizeof(double); 
	}
	
	static int WriteLength(unsigned char* pBuf, long off, unsigned long len)
	{
		if (len <= 0x7f)
		{
			pBuf[0] = (unsigned char)len;
			return 1;
		}
		else if (len <= 0x3fff)
		{
			pBuf[off+0] = (unsigned char)((len & 0x0000ff00) >> 8) + 0x80;
			pBuf[off+1] = (unsigned char)(len & 0x000000ff);
			return 2;
		}
		else if (len <= 0x1fffff)
		{
			pBuf[off+0] = (unsigned char)((len & 0x00ff0000) >> 16) + 0xc0;
			pBuf[off+1] = (unsigned char)((len & 0x0000ff00) >> 8);
			pBuf[off+2] = (unsigned char)(len & 0x000000ff);
			return 3;
		}
		else if (len <= 0x0fffffff)
		{
			pBuf[off+0] = (unsigned char)((len & 0xff000000) >> 24) + 0xe0;
			pBuf[off+1] = (unsigned char)((len & 0x00ff0000) >> 16);
			pBuf[off+2] = (unsigned char)((len & 0x0000ff00) >> 8);
			pBuf[off+3] = (unsigned char)(len & 0x000000ff);
			return 4;
		}
		else
		{
			pBuf[off+0] = (unsigned char)0xf0;
			pBuf[off+1] = (unsigned char)((len & 0xff000000) >> 24);
			pBuf[off+2] = (unsigned char)((len & 0x00ff0000) >> 16);
			pBuf[off+3] = (unsigned char)((len & 0x0000ff00) >> 8);
			pBuf[off+4] = (unsigned char)(len & 0x000000ff);
			return 5;
		}
	}
    
	static inline void Swap(void* pData, int size)
	{
        static int s_nDummy = 1;
        
        if(*(char*)(&s_nDummy) == 0)
        {
		    unsigned char* pch = (unsigned char*)pData;
            
		    KK_Assert(size > 0);
            
			int i;
		    for(i = 0; i < size / 2; i++)
		    {
			    unsigned char chTemp;
			    chTemp = pch[i];
			    pch[i] = pch[size-1-i];
			    pch[size-1-i] = chTemp;
		    }
        }
	}
    
	// SBC: Stream bytes count
	static inline int GetLengthSBC(unsigned long len)
	{
		if (len <= 0x7f)
			return 1;
		else if (len <= 0x3fff)
			return 2;
		else if (len <= 0x1fffff)
			return 3;
		else if (len <= 0x0fffffff)
			return 4;
		else
			return 5;
	}
	static inline int GetStringSBC(const KK_StringU & strUtf8, bool bFixedLen = false)
	{
		int nStringLen = strUtf8.GetLength();
		return bFixedLen ? nStringLen + 4 : nStringLen + GetLengthSBC(nStringLen);
	}
    
protected:
	unsigned char* m_pBuf;
	long m_lCurPos;
	long m_lBufLen;
	bool m_bLoad;
    
public :
	inline void Free(unsigned char * buf) {	if (buf) delete [] buf; }
	virtual ~KK_ByteStream() {}
};


#endif // __KK_BYTE_STREAM_H__
