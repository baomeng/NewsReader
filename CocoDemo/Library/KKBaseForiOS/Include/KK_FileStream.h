
/*-------------------------------------------------------------------------*/
/*																		   */
/*	KK_FileStream.h														   */
/*																		   */
/*	Copyright (c) 2007 Seegle Soft Ltd.       							   */
/*  All rights reserved													   */
/*																		   */
/*-------------------------------------------------------------------------*/

#ifndef __KK_FILE_STREAM_H__
#define __KK_FILE_STREAM_H__

#include "KK_Includes.h"
#include "KK_File.h"
#include "KK_ByteStream.h"

#define _LMAX 0X7FFFFFFF
#define _MEMPAGESIZE (4*1024)
#define _NEWMAXBUFSIZE (1024*1024)

class KK_FileStream : public KK_ByteStream
{
/*
unsigned char* m_pBuf; // IO CACHE
long m_lCurPos; // 当前读写位置相对于文件首字节的偏移量
long m_lBufLen; // 文件的可用长度(注:此处有问题,新增m_lFileLen)
long m_lFileLen // 文件的可用长度 
*/

protected:
	KK_File m_file;  
	long m_lBufBeginPos; // IO CACHE相对于文件首字节的偏移量
	long m_lFileLen;

public: 
	KK_FileStream() { m_lBufBeginPos = 0; }
	KK_FileStream(const KK_StringW& strFile, bool bLoad) { m_lBufBeginPos = 0; Init(strFile, bLoad); }
	KK_FileStream(const KK_StringA& strFile, bool bLoad) { m_lBufBeginPos = 0; Init(KK_StringW(strFile), bLoad); }
	KK_FileStream(LPCTSTR szFile, bool bLoad)
	{
		m_lBufBeginPos = 0; 
		Init(KK_StringW(szFile), bLoad); 
	}

	virtual ~KK_FileStream() { Close(); }

public:
	virtual unsigned char* GetData() { throw (long)ERROR_READ_BUF; }
	virtual void Init(unsigned char* pBuf, long lLen, bool bLoad) { throw bLoad ? (long)ERROR_READ_BUF : (long)ERROR_WRITE_BUF; }

	bool Init(const KK_StringA& strFile, bool bLoad) { return Init(KK_StringW(strFile), bLoad); }
	bool Init(LPCTSTR strFile, bool bLoad) { return Init(KK_StringW(strFile), bLoad); }
	bool Init(const KK_StringW& strFile, bool bLoad) {
		if (m_pBuf) return false;

		u_int uOpenFlag = (bLoad ? KK_FileEx::modeRead : KK_FileEx::modeReadWrite) | KK_FileEx::OpenAlways;
		if (!m_file.Open(strFile, uOpenFlag)) return false;

		m_bLoad = bLoad;
		/*m_lBufLen*/ m_lFileLen = m_bLoad ? m_file.GetFileLength() : _LMAX;
		m_lBufLen = m_lFileLen;
		m_pBuf = new u_char[_MEMPAGESIZE];

		if (m_bLoad) 
		{
			long lRead = min(/*m_lBufLen*/m_lFileLen, _MEMPAGESIZE);
			if (m_file.Read(m_pBuf, lRead) != lRead) 
			{
				Close();
				return false;
			}
		}

		return true;
	}

	bool Close() {
		if (!m_pBuf) return false;

		if (IsStoring()) {
			long lWrite = m_lCurPos - m_lBufBeginPos;
			if (m_file.Write(m_pBuf, lWrite) != lWrite)
				throw (long)ERROR_WRITE_FILE;
		}

		SAFE_ARRAY_DELETE(m_pBuf);
		m_lCurPos = 0;
		m_lBufLen = 0;
		m_lFileLen = 0;
		m_bLoad = false;
		m_lBufBeginPos = 0;
		m_file.Close();
		
		return true;
	}

protected:
	virtual void CopyBuffer(long lOffset, u_long ulSize, void * pIOData = NULL) {
		long lNewPos = m_lCurPos + lOffset;
		CheckBuffer(lNewPos, ulSize);

		if (IsStoring()) {
			KK_Assert(m_file.GetPosition() == m_lBufBeginPos);

			if (lOffset != 0) {
				if (m_lCurPos != m_lBufBeginPos) {
					KK_Assert(m_lCurPos > m_lBufBeginPos && m_lCurPos <= GetBufEndPos());
					long lWrite = m_lCurPos - m_lBufBeginPos;
					if (m_file.Write(m_pBuf, lWrite) != lWrite)
						throw (long)ERROR_WRITE_FILE;
				}
				m_lCurPos = lNewPos;
				m_lBufBeginPos = lNewPos;

				if (m_file.Seek(lNewPos, KK_FileEx::begin) != lNewPos)
					throw (long)ERROR_WRITE_FILE;
			}

			if (pIOData && ulSize > 0) {
				if (GetBufEndPos() - m_lCurPos >= ulSize) { // 有足够的BUF
					memcpy(m_pBuf + m_lCurPos - m_lBufBeginPos, pIOData, ulSize);
				}
				else if (m_lCurPos - m_lBufBeginPos + ulSize >= _MEMPAGESIZE*2) { // 全部写入
					long lWrite = m_lCurPos - m_lBufBeginPos;
					if (m_file.Write(m_pBuf, lWrite) != lWrite)
						throw (long)ERROR_WRITE_FILE;
					if (m_file.Write(pIOData, ulSize) != ulSize)
						throw (long)ERROR_WRITE_FILE;

					m_lBufBeginPos = m_lCurPos + ulSize;
				}
				else { // 写入_MEMPAGESIZE
					long lCopy = GetBufEndPos() - m_lCurPos;
					memcpy(m_pBuf + m_lCurPos - m_lBufBeginPos, pIOData, lCopy);
					if (m_file.Write(m_pBuf, _MEMPAGESIZE) != _MEMPAGESIZE)
						throw (long)ERROR_WRITE_FILE;

					m_lBufBeginPos += _MEMPAGESIZE;
					memcpy(m_pBuf, (u_char*)pIOData + lCopy, ulSize - lCopy);
				}
			}

			m_lCurPos = lNewPos + ulSize;
		}

		else // if (IsLoading()) 
		{
			//    -----------------********************----------------------------  m_pBuf
			// 1. -----*******-----------------------------------------------------  pIOData
			// 2. -------------********--------------------------------------------  pIOData
			// 3. ----------------------*******------------------------------------  pIOData
			// 4. ---------------------------------*********-----------------------  pIOData
			// 5. --------------**************************-------------------------  pIOData
			// 6. -------------------------------------------*****-----------------  pIOData

			m_lCurPos = lNewPos + ulSize;
			if (ulSize == 0 || pIOData == NULL) return;

			if (lNewPos >= m_lBufBeginPos && m_lCurPos <= GetBufEndPos()) { // 3
				memcpy(pIOData, m_pBuf + lNewPos - m_lBufBeginPos, ulSize);
			}
			else { // 1、2、4、5、6
				if (lNewPos >= m_lBufBeginPos && lNewPos < GetBufEndPos()) { // 4
					long lCopy = GetBufEndPos() - lNewPos;
					memcpy(pIOData, m_pBuf + lNewPos - m_lBufBeginPos, lCopy);
					lNewPos += lCopy;
					ulSize -= lCopy;
					pIOData = (u_char*)pIOData + lCopy;
				}

				if (lNewPos != GetBufEndPos()) {
					if (m_file.Seek(lNewPos, KK_FileEx::begin) != lNewPos)
						throw (long)ERROR_READ_FILE;
				}

				if (ulSize > _MEMPAGESIZE) {
					if (m_file.Read(pIOData, ulSize) != ulSize)
						throw (long)ERROR_READ_FILE;
					m_lBufBeginPos = lNewPos+ulSize-_MEMPAGESIZE;
					memcpy(m_pBuf, (u_char*)pIOData+ulSize-_MEMPAGESIZE, _MEMPAGESIZE);
				}
				else {
					long lRead = min(_MEMPAGESIZE, /*m_lBufLen*/m_lFileLen-lNewPos);
					if (m_file.Read(m_pBuf, lRead) != lRead)
						throw (long)ERROR_READ_FILE;
					m_lBufBeginPos = lNewPos;
					memcpy(pIOData, m_pBuf, ulSize);
				}
			}
		}
	}

	inline long GetBufEndPos() { return min(m_lBufLen, m_lBufBeginPos + _MEMPAGESIZE); }
};


#endif // __KK_FILE_STREAM_H__
