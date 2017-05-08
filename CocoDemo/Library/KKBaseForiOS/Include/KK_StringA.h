//
//  KK_StringA.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//



#ifndef __KK_STRING_A_H__
#define __KK_STRING_A_H__

#include "KK_Includes.h"
#include "KK_BaseFunctions.h"

class KK_StringA;
class KK_StringW;
class KK_StringU;

const long NULLSTRING_A = -10001;

//////////////////////////////////////////////////////////////////////
// additional defines
//////////////////////////////////////////////////////////////////////
// 注释［2015.09.07］：考虑到32Bit和64Bit的兼容性，修改KK_StringDataA数据结构的定义
// 注释［2015.09.07］：废弃字符串内存共享模式
struct KK_StringDataA
{
    /*
    long    nRefs;          // reference count
    */
    long    nDataLength;
    long    nAllocLength;
    BYTE*   ptrByte;
    
    KK_StringDataA(void) : // nRefs(NULLSTRING_A),
                           nDataLength(0),
                           nAllocLength(0),
                           ptrByte(NULL)
    {
        // Do Nothing
    }
    
    ~KK_StringDataA(void)
    {
        deleteData();
    }
    
    BYTE* newData(long lByteLen)
    {
        deleteData();
        
        if ( lByteLen > 0 )
        {
            ptrByte = new BYTE[lByteLen + 1];
            if ( NULL != ptrByte )
            {
                memset(ptrByte, 0, sizeof(BYTE) * (lByteLen + 1));
                // nRefs     = 1;
                nDataLength  = 0;
                nAllocLength = lByteLen;
            }
        }
        
        return ptrByte;
    }
    
    BYTE* resizeData(long lByteLen)
    {
        if ( lByteLen > 0 )
        {
            if ( NULL != ptrByte )
            {
                if ( lByteLen > nAllocLength )
                {
                    BYTE* pOldtrByte = ptrByte;
                    
                    ptrByte = new BYTE[lByteLen + 1];
                    if ( NULL != ptrByte )
                    {
                        memset(ptrByte, 0, sizeof(BYTE) * (lByteLen + 1));
                        memmove(ptrByte, pOldtrByte, sizeof(BYTE) * nDataLength);
                        // nRefs     = 1;
                        nDataLength  = lByteLen - 1;
                        nAllocLength = lByteLen;
                    }
                    else
                    {
                        // nRefs     = 1;
                        nDataLength  = 0;
                        nAllocLength = 0;
                    }
                    
                    delete [] pOldtrByte;
                    pOldtrByte = NULL;
                }
                else if ( lByteLen < nAllocLength )
                {
                    ptrByte[lByteLen - 1] = '\0';
                    if ( lByteLen <= nDataLength )
                    {
                        nDataLength = lByteLen - 1;
                    }
                }
            }
            else
            {
                ptrByte = new BYTE[lByteLen + 1];
                if ( NULL != ptrByte )
                {
                    memset(ptrByte, 0, sizeof(BYTE) * (lByteLen + 1));
                    // nRefs     = 1;
                    nDataLength  = lByteLen - 1;
                    nAllocLength = lByteLen;
                }
                else
                {
                    // nRefs     = 1;
                    nDataLength  = 0;
                    nAllocLength = 0;
                }
            }
        }
        else
        {
            deleteData();
        }
        
        return ptrByte;
    }
    
    void deleteData(void)
    {
        // nRefs     = NULLSTRING_A;
        nDataLength  = 0;
        nAllocLength = 0;
        if ( NULL != ptrByte )
        {
            delete [] ptrByte;
            ptrByte = NULL;
        }
    }
    
    CHAR* data() { return (CHAR*)ptrByte; }
    CHAR* dataConst() const { return (CHAR*)ptrByte; };
    // bool IsNullString() { return ( nRefs == NULLSTRING_A); }
};

KK_StringA  operator+(const KK_StringA& string1, const KK_StringA& string2);
KK_StringA  operator+(const KK_StringA& KK_StringA, char ch);
KK_StringA  operator+(const KK_StringA& KK_StringA, WCHAR ch);
KK_StringA  operator+(WCHAR ch, const KK_StringA& KK_StringA);
KK_StringA  operator+(char ch, const KK_StringA& KK_StringA);

/*
static long rgInitDataA[] = { NULLSTRING_A, 0, 0, 0 };
//static LPCSTR _atltmpPchNilA = (LPCSTR)(((BYTE*)&rgInitDataA) + sizeof(KK_StringDataA));
static KK_StringDataA* _atltmpPchNilA = (KK_StringDataA*)rgInitDataA;
*/

class KK_StringA
{
public:
    KK_StringA() { Init(); }
    
	KK_StringA(const KK_StringW& stringSrc);
	KK_StringA(const KK_StringU& stringSrc);
    
    KK_StringA(const KK_StringA& stringSrc)
    {
        /*
        assert(stringSrc.GetData()->nRefs != 0);
        if (stringSrc.GetData()->nRefs >= 0)
        {
            assert(!stringSrc.GetData()->IsNullString());
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
            
        }
        else
        {
            Init();
            *this = stringSrc.m_pchData;
        }
        */
        
        Init();
        *this = stringSrc.GetData()->dataConst();
    }
    
    KK_StringA(CHAR ch, int nRepeat = 1)
    {
        Init();
        /*
        if (nRepeat >= 1)
        {
            if(AllocBuffer(nRepeat))
            {
				int i;
                for (i = 0; i < nRepeat; i++)
                    m_pchData[i] = ch;
            }
        }
        */
        if ( (nRepeat >= 1) && AllocBuffer(nRepeat) )
        {
            memset(m_stStringDataA.data(), ch, sizeof(BYTE) * nRepeat);
        }
    }
    
    KK_StringA(LPCSTR lpsz)
    {
        Init();
        //if (lpsz != NULL && HIWORD(lpsz) == NULL)
        //有警告
		if (lpsz == NULL /*&& HIWORD(lpsz) == 0*/)
        {
			assert(0);
        }
        else
        {
            int nSrcLen = (lpsz != NULL) ? strlen(lpsz) : 0;
			if (nSrcLen != 0 && AllocBuffer(nSrcLen)) memcpy(m_stStringDataA.data(), lpsz, nSrcLen * sizeof(CHAR));
        }
    }
    
    KK_StringA(LPCSTR lpsz, int nLength)
    {
        Init();
		if (nLength != 0 && AllocBuffer(nLength)) memcpy(m_stStringDataA.data(), lpsz, nLength * sizeof(CHAR));
    }
    
    KK_StringA(LPCWSTR lpsz)
    {
        Init();
        int nSrcLen = (lpsz != NULL) ? _getmbslen((const WCHAR*)lpsz) : 0;
		if (nSrcLen != 0 && AllocBuffer(nSrcLen))
		{ 
			_wcstombsz(m_stStringDataA.data(), (const WCHAR*)lpsz, nSrcLen + 1);
            ReleaseBuffer();
		}
    }
    
    KK_StringA(LPCWSTR lpch, int nLength)
    {
        Init();
		if (nLength != 0 && AllocBuffer(nLength))
        {
			_wcstombsz(m_stStringDataA.data(), (const WCHAR*)lpch, nLength);
            ReleaseBuffer();
        }
    }
    
    KK_StringA(const unsigned char* psz) 
    {
		Init();
		*this = (LPCSTR)psz; 
    }
    
    const KK_StringA& Assign(LPCSTR lpszSrcData, int nSrcLen)
    { 
		AssignCopy(nSrcLen, lpszSrcData);
		return *this; 
	}
    
    int GetLength() const { return GetData()->nDataLength; }
    
    BOOL IsEmpty() const { return GetData()->nDataLength == 0; }
    
    void Empty() 
    {
        /*
        if (GetData()->nDataLength == 0) return;           
        
        if (GetData()->nRefs >= 0)
            Release();
        else
            //*this = L"";
            
            *this = "";//？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
        
        assert(GetData()->nDataLength == 0);
        assert(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
        */
        
        if ( 0 == GetData()->nDataLength )
        {
            return;
        }
        
        *this = "";
        
        assert( (0 == GetData()->nDataLength) && (0 == GetData()->nAllocLength) );
    }
    
    CHAR GetAt(int nIndex) const 
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        return GetData()->dataConst()[nIndex];
    }
    
    CHAR & operator[](int nIndex) const
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        return GetData()->dataConst()[nIndex];
    }
    
    void SetAt(int nIndex, CHAR ch)
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        
        CopyBeforeWrite();
        m_stStringDataA.data()[nIndex] = ch;
    }
    
    operator LPCSTR() const {return GetData()->dataConst();}
	
	const KK_StringA& operator=(const KK_StringW& stringSrc);
	const KK_StringA& operator=(const KK_StringU& stringSrc);
    const KK_StringA& operator=(const KK_StringA& stringSrc)
    {
        /*
        if (m_pchData != stringSrc.m_pchData)
        {
            if ((GetData()->nRefs < 0 && !GetData()->IsNullString()) || stringSrc.GetData()->nRefs < 0)
            {
                AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
            }
            else
            {
                Release();
                assert(!stringSrc.GetData()->IsNullString());
                m_pchData = stringSrc.m_pchData;
                InterlockedIncrement(&GetData()->nRefs);
                
            }
        }
        */
        
        if ( this != &stringSrc )
        {
            AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.GetData()->dataConst());
        }
        
        return *this;
    }
    
    
    const KK_StringA& operator=(WCHAR wch)
    {
        KK_StringA ch((LPCWSTR)&wch, 1);
        AssignCopy(1, ch);
        return *this;
    }
	/* ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？*/
    
    const KK_StringA& operator=(CHAR ch)
    {
        AssignCopy(1, &ch);
        return *this;
    }
    
    const KK_StringA& operator=(LPCSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        AssignCopy(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    /*
     const KK_StringA& operator=(LPCWSTR lpsz)
     {
     int nSrcLen = (lpsz != NULL) ? _getmbslen((const WCHAR*)lpsz) : 0;        
     if(AllocBeforeWrite(nSrcLen))
     {
     _wcstombsz(m_pchData, (const WCHAR*)lpsz, nSrcLen + 1);
     ReleaseBuffer();
     }
     return *this;
     }
     */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	const KK_StringA& operator=(const wchar_t* lpsz)
	{
		int nSrcLen = (lpsz != NULL) ? wcslen((const wchar_t*)lpsz) : 0;
		if ( AllocBeforeWrite(nSrcLen) )
        {
            if ( NULL != lpsz )
            {
                wcstombs(m_stStringDataA.data(), (const wchar_t*)lpsz, nSrcLen + 1);
            }
            ReleaseBuffer();
        }
        return *this;
	}
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/    
    const KK_StringA& operator=(const unsigned char* psz)
    {
		*this = (LPCSTR)psz;
		return *this; 
	}
    
    // KK_StringA concatenation
    const KK_StringA& operator+=(LPCWSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        int nSrcLen = (lpsz != NULL) ? _getmbslen(lpsz) : 0;
        ConcatInPlace(nSrcLen, KK_StringA(lpsz));
        return *this;
    }
    
    const KK_StringA& operator+=(CHAR ch)
    {
        ConcatInPlace(1, &ch);
        return *this;
    }
    
    const KK_StringA& operator+=(LPCSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        ConcatInPlace(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    
    friend KK_StringA __stdcall operator+(const KK_StringA& string1, const KK_StringA& string2);
    friend KK_StringA __stdcall operator+(const KK_StringA& KK_StringA, char ch);
    friend KK_StringA __stdcall operator+(const KK_StringA& KK_StringA, WCHAR ch);
    friend KK_StringA __stdcall operator+(WCHAR ch, const KK_StringA& KK_StringA);
    friend KK_StringA __stdcall operator+(char ch, const KK_StringA& KK_StringA);
    
    
    int Compare(LPCSTR lpsz) const { return strcmp(GetData()->dataConst(), lpsz); } // MBCS/Unicode aware
    
    int CompareNoCase(LPCSTR lpsz) const { return strcasecmp(GetData()->dataConst(), lpsz); } // ignore case// MBCS/Unicode aware
    
    int Compare(LPCSTR lpsz, size_t size) const { return strncmp(GetData()->dataConst(), lpsz, size); } // MBCS/Unicode aware
    
    int CompareNoCase(LPCSTR lpsz, size_t size) const { return strncasecmp(GetData()->dataConst(), lpsz, size); } // MBCS/Unicode aware // ignore case
    
    
    
    KK_StringA Mid(int nFirst, int nCount) const
    {
        
        if (nFirst < 0) nFirst = 0;            
        if (nCount < 0) nCount = 0;            
        
        if (nFirst + nCount > GetData()->nDataLength) nCount = GetData()->nDataLength - nFirst;            
        if (nFirst > GetData()->nDataLength) nCount = 0;  
        
        if (nFirst == 0 && nCount == GetData()->nDataLength) return *this;
        
        KK_StringA dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }
    
    
    KK_StringA Mid(int nFirst) const { return Mid(nFirst, GetData()->nDataLength - nFirst); }
    
    KK_StringA Left(int nCount) const
    {
        
        if (nCount < 0)
            nCount = 0;
        else if (nCount >= GetData()->nDataLength)
            return *this;
        
        KK_StringA dest;
        AllocCopy(dest, nCount, 0, 0);
        return dest;
    }
    
    KK_StringA Right(int nCount) const
    {
        if (nCount < 0)
            nCount = 0;
        else if (nCount >= GetData()->nDataLength)
            return *this;
        
        KK_StringA dest;
        AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
        return dest;
    }
    
    KK_StringA SpanIncluding(LPCSTR lpszCharSet) const
    {
        assert(_IsValidString(lpszCharSet));
        return Left(strspn(GetData()->dataConst(), lpszCharSet));
    }
    
    KK_StringA SpanExcluding(LPCSTR lpszCharSet) const
    {
        assert(_IsValidString(lpszCharSet));
        return Left(strcspn(GetData()->dataConst(), lpszCharSet));
    }
    
    void MakeUpper()
    {
        CopyBeforeWrite();
		CharUpperA(m_stStringDataA.data());
    }
    
    void MakeLower()
    {
        CopyBeforeWrite();
        CharLowerA(m_stStringDataA.data());
    }
    
    void MakeReverse()
    {
        CopyBeforeWrite();
        strrev(m_stStringDataA.data());
    }
    
    void TrimRight()
    {
        CopyBeforeWrite();
        
        LPSTR lpsz     = m_stStringDataA.data();
        LPSTR lpszLast = NULL;
        while (*lpsz != '\0')
        {
            if (isspace(*lpsz))
            {
                if (lpszLast == NULL) lpszLast = lpsz;                    
            }
            else
            {
                lpszLast = NULL;
            }
            lpsz = CharNextA(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            *lpszLast = '\0';
            m_stStringDataA.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataA.data());
        }
    }
    
    void TrimLeft()
    {
        CopyBeforeWrite();
        
        LPCSTR lpsz = m_stStringDataA.data();
        while (isspace(*lpsz))
            lpsz = CharNextA(lpsz);
        
        LPSTR lpStart     = m_stStringDataA.data();
        int   nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - lpStart);
        memmove(lpStart, lpsz, (nDataLength + 1) * sizeof(CHAR));
        m_stStringDataA.nDataLength = nDataLength;
    }
    
    void TrimRight(CHAR chTarget)
    {
        CopyBeforeWrite();
        LPSTR lpsz = m_stStringDataA.data();
        LPSTR lpszLast = NULL;
        
        while (*lpsz != '\0')
        {
            if (*lpsz == chTarget)
            {
                if (lpszLast == NULL) lpszLast = lpsz;                    
            }
            else
			{
				lpszLast = NULL;
			}
            lpsz = CharNextA(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            *lpszLast = '\0';
            m_stStringDataA.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataA.data());
        }
    }
    
    void TrimRight(LPCSTR lpszTargets)
    {
        CopyBeforeWrite();
        LPSTR lpsz = m_stStringDataA.data();
        LPSTR lpszLast = NULL;
        
        while (*lpsz != '\0')
        {
            if (strchr(lpszTargets, *lpsz) != NULL)
            {
                if (lpszLast == NULL) lpszLast = lpsz;                    
            }
            else
			{
				lpszLast = NULL;
			}
            lpsz = CharNextA(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            *lpszLast = '\0';
            m_stStringDataA.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataA.data());
        }
    }
    
    void TrimLeft(CHAR chTarget)
    {
        CopyBeforeWrite();
        LPCSTR lpsz     = m_stStringDataA.data();
        LPSTR  lpStart  = m_stStringDataA.data();
        
        while (chTarget == *lpsz)
            lpsz = CharNextA(lpsz);
        
        if ( lpsz != lpStart )
        {
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - lpStart);
            memmove(lpStart, lpsz, (nDataLength + 1) * sizeof(CHAR));
            m_stStringDataA.nDataLength = nDataLength;
        }
    }
    
    void TrimLeft(LPCSTR lpszTargets)
    {
        if (SafeStrlen(lpszTargets) == 0) return;
        
        CopyBeforeWrite();
        LPCSTR lpsz     = m_stStringDataA.data();
        LPSTR  lpStart  = m_stStringDataA.data();
        
        while (*lpsz != '\0')
        {
            if (strchr(lpszTargets, *lpsz) == NULL) break;                
            lpsz = CharNextA(lpsz);
        }
        
        if ( lpsz != lpStart )
        {
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - lpStart);
            memmove(lpStart, lpsz, (nDataLength + 1) * sizeof(CHAR));
            m_stStringDataA.nDataLength = nDataLength;
        }
    }
    
    int Replace(CHAR chOld, CHAR chNew)
    {
        int nCount = 0;
        
        if (chOld != chNew)
        {
            CopyBeforeWrite();
            LPSTR psz    = m_stStringDataA.data();
            LPSTR pszEnd = psz + GetData()->nDataLength;
            while (psz < pszEnd)
            {
                if (*psz == chOld)
                {
                    *psz = chNew;
                    nCount++;
                }
                psz = CharNextA(psz);
            }
        }
		
        return nCount;
    }
    
    
    int Replace(LPCSTR lpszOld, LPCSTR lpszNew)
    {
        // can't have empty or NULL lpszOld
        
        int nSourceLen = SafeStrlen(lpszOld);
        if (nSourceLen == 0) return 0;            
        int nReplacementLen = SafeStrlen(lpszNew);
        
        // loop once to figure out the size of the result KK_StringA
        int nCount = 0;
        LPSTR lpszStart = m_stStringDataA.data();
        LPSTR lpszEnd   = lpszStart + GetData()->nDataLength;
        LPSTR lpszTarget;
        while (lpszStart < lpszEnd)
        {
            while ((lpszTarget = strstr(lpszStart, lpszOld)) != NULL)
            {
                nCount++;
                lpszStart = lpszTarget + nSourceLen;
            }
            lpszStart += strlen(lpszStart) + 1;
        }
        
        // if any changes were made, make them
        if (nCount > 0)
        {
            CopyBeforeWrite();
            
            // if the buffer is too small, just
            // allocate a new buffer (slow but sure)
            int nOldLength = GetData()->nDataLength;
            int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;
            /*
            if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
            {
                KK_StringDataA* pOldData = GetData();
                LPSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength)) return -1;  
				
                memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(CHAR));
                Release(pOldData);
            }
            */
            m_stStringDataA.resizeData(nNewLength + 1);
            
            // else, we just do it in-place
            lpszStart = m_stStringDataA.data();
            lpszEnd   = lpszStart + GetData()->nDataLength;
            
            // loop again to actually do the work
            while (lpszStart < lpszEnd)
            {
                while ( (lpszTarget = strstr(lpszStart, lpszOld)) != NULL)
                {
                    int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_stStringDataA.data()) + nSourceLen);
                    memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(CHAR));
                    memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(CHAR));
                    lpszStart = lpszTarget + nReplacementLen;
                    lpszStart[nBalance] = '\0';
                    nOldLength += (nReplacementLen - nSourceLen);
                }
                lpszStart += strlen(lpszStart) + 1;
            }
            m_stStringDataA.nDataLength = nNewLength;
        }
        
        return nCount;
    }
    
    // replace nCount characters starting at zero-based index with lpszNew;
    int Replace(int nIndex, int nCount, LPCSTR lpszNew)
    { // 可以优化
		Delete(nIndex, nCount);
		return Insert(nIndex, lpszNew);
    }
    
    // replace nCount characters starting at zero-based index with chNew;
    int Replace(int nIndex, int nCount, CHAR chNew)
    { // 可以优化
		Delete(nIndex, nCount);
		return Insert(nIndex, chNew);
    }
    
    int Remove(CHAR chRemove)
    {
        CopyBeforeWrite();
        
        LPSTR pstrSource = m_stStringDataA.data();
        LPSTR pstrDest   = pstrSource;
        LPSTR pstrEnd    = pstrSource + GetData()->nDataLength;
        
        while (pstrSource < pstrEnd)
        {
            if (*pstrSource != chRemove)
            {
                *pstrDest = *pstrSource;
                pstrDest = CharNextA(pstrDest);
            }
            pstrSource = CharNextA(pstrSource);
        }
        *pstrDest = '\0';
        int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
        m_stStringDataA.nDataLength -= nCount;
        
        return nCount;
    }
    //Insert应该可以优化
    int Insert(int nIndex, CHAR ch)
    {
        CopyBeforeWrite();
        
        if (nIndex < 0) nIndex = 0;
        int nNewLength = GetData()->nDataLength;
        if (nIndex > nNewLength) nIndex = nNewLength;            
        nNewLength++;
        /*
        if (GetData()->nAllocLength < nNewLength)
        {
            KK_StringDataA* pOldData = GetData();
            LPSTR pstr = m_pchData;
            if(!AllocBuffer(nNewLength)) return -1;
			
            memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(CHAR));
            Release(pOldData);
        }
        */
        m_stStringDataA.resizeData(nNewLength);
        
        LPSTR lpStart = m_stStringDataA.data();
        memmove(lpStart + nIndex + 1, lpStart + nIndex, (nNewLength - nIndex) * sizeof(CHAR));
        lpStart[nIndex] = ch;
        m_stStringDataA.nDataLength = nNewLength;
        
        return nNewLength;
    }
    
    int Insert(int nIndex, LPCSTR pstr)
    {
        if (nIndex < 0) nIndex = 0;            
        
        int nInsertLength = SafeStrlen(pstr);
        int nNewLength    = GetData()->nDataLength;
        if (nInsertLength > 0)
        {
            CopyBeforeWrite();
            if (nIndex > nNewLength) nIndex = nNewLength;                
            nNewLength += nInsertLength;
            /*
            if (GetData()->nAllocLength < nNewLength)
            {
                KK_StringDataA* pOldData = GetData();
                LPSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength)) return -1;
                
                memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(CHAR));
                Release(pOldData);
            }
            */
            m_stStringDataA.resizeData(nNewLength + 1);
            
            // move existing bytes down
            LPSTR lpStart = m_stStringDataA.data();
            memmove(lpStart + nIndex + nInsertLength, lpStart + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(CHAR));
            memcpy(lpStart + nIndex, pstr, nInsertLength * sizeof(CHAR));
            m_stStringDataA.nDataLength = nNewLength;
        }
        
        return nNewLength;
    }
    
    //应该可以简洁点
    int Delete(int nIndex, int nCount = 1)
    {
		int nNewLength = GetData()->nDataLength;
        if (nIndex < 0)
		{
			if (nCount > nNewLength) 
				nCount = -1;
			else if (nCount < 0 && nCount!= -1)
				nCount = 0;
			nIndex = 0;
		}
		else if (nIndex > 0 && nIndex < nNewLength)
		{
			if (nCount > nNewLength) 
				nCount = -1;
			else if (nCount < 0 && nCount!= -1)
				nCount = 0;
		}
		else if (nIndex >= nNewLength)
		{
			nIndex = nNewLength;
			nCount = 0;
		}
        
        LPSTR lpStart = m_stStringDataA.data();
		if (nCount == -1 && nIndex < nNewLength)
		{
            CopyBeforeWrite();
			lpStart[nIndex] = 0;
            m_stStringDataA.nDataLength = nIndex;
			nCount = 0;
			nNewLength = nIndex;
		}
        else if (nCount > 0 && nIndex < nNewLength)
        {
            CopyBeforeWrite();
            int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
            
            memmove(lpStart + nIndex, lpStart + nIndex + nCount, nBytesToCopy * sizeof(CHAR));
            m_stStringDataA.nDataLength = nNewLength - nCount;
        }
        
        return nNewLength - nCount;
    }
    
	int Erase(int nIndex, int nCount = -1) { return Delete(nIndex, nCount); }
	
    int Find(CHAR ch) const { return Find(ch, 0); }
    
    int ReverseFind(CHAR ch) const
    {
        LPSTR lpsz = strrchr(GetData()->dataConst(), ch);
        
        return (lpsz == NULL) ? -1 : (int)(lpsz - GetData()->dataConst());
    }
    
    int ReverseFind(LPCSTR lpszSub) const
    {
		if (lpszSub == NULL) return -1;
        
		int sublen = SafeStrlen(lpszSub);
        LPSTR lpStart = GetData()->dataConst();
        LPSTR pstrEnd = lpStart + GetData()->nDataLength - sublen;
        
        while (pstrEnd >= lpStart)
        {
            if (strncmp(lpszSub, pstrEnd, sublen) == 0) return (int)(pstrEnd - lpStart);
            pstrEnd--;
        }
        
        return -1;
    }
    
    int Find(CHAR ch, int nStart) const 
    {
        int nLength = GetData()->nDataLength;
        if (nStart >= nLength) return -1;            
        LPSTR lpStart = GetData()->dataConst();
        LPSTR lpsz    = strchr(lpStart + nStart, ch);
        
        return (lpsz == NULL) ? -1 : (int)(lpsz - lpStart);
    }
    
    int FindOneOf(LPCSTR lpszCharSet) const
    {
        assert(_IsValidString(lpszCharSet));
        LPSTR lpStart = GetData()->dataConst();
        LPSTR lpsz    = strpbrk(lpStart, lpszCharSet);
        return (lpsz == NULL) ? -1 : (int)(lpsz - lpStart);
    }
    
    int FindOneNotOf(LPCSTR lpszCharSet) const
    {
        assert(_IsValidString(lpszCharSet));
        int size = strspn(GetData()->dataConst(), lpszCharSet);
		return (size >= GetData()->nDataLength) ? -1 : size;
    }
    
    int Find(LPCSTR lpszSub) const { return Find(lpszSub, 0); } 
    
    int Find(LPCSTR lpszSub, int nStart) const 
    {
        assert(_IsValidString(lpszSub));
        
        int nLength = GetData()->nDataLength;
        if (nStart > nLength) return -1;            
        
        LPSTR lpStart = GetData()->dataConst();
        LPSTR lpsz    = strstr(lpStart + nStart, lpszSub);
        
        return (lpsz == NULL) ? -1 : (int)(lpsz - lpStart);
    }
    
    const KK_StringA& Append(char n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        //////////////////////////////////////////////// 
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(short n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        //////////////////////////////////////////////// 
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(int n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        //////////////////////////////////////////////// 
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(long n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        //////////////////////////////////////////////// 
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(__int64 n, int radix = 10)
    {
        CHAR szBuffer[80];
        _i64toa(n, szBuffer, radix);
        //////////////////////////////////////////////// 
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(unsigned char n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ////////////////////////////////////////////////
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(unsigned short n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ////////////////////////////////////////////////
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(unsigned int n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ////////////////////////////////////////////////
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(unsigned long n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ////////////////////////////////////////////////
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(unsigned __int64 n, int radix = 10)
    {
        CHAR szBuffer[80];
        _ui64toa(n, szBuffer, radix);
        ////////////////////////////////////////////////
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    const KK_StringA& Append(double n, int digits = 20)
    {
        CHAR szBuffer[80];
		gcvt(n, digits, szBuffer);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    BOOL FormatV(LPCSTR lpszFormat, va_list argList)  // lstrlenA == strlen CharNextA == CharNextA
    {
	    assert(_IsValidString(lpszFormat));
        
	    enum _FormatModifiers
	    {
		    FORCE_ANSI =	0x10000,
		    FORCE_UNICODE =	0x20000,
		    FORCE_INT64 =	0x40000
	    };
        
        // Test
        va_list argListSave;
        va_copy(argListSave, argList);
        
        
	    // make a guess at the maximum length of the resulting KK_StringA
	    int nMaxLen = 0;
		LPCSTR lpsz;
	    for (lpsz = lpszFormat; *lpsz != '\0'; lpsz = (CharNextA(lpsz)))
	    {
		    // handle '%' character, but watch out for '%%'
		    if (*lpsz != '%' || *(lpsz = CharNextA(lpsz)) == '%')
		    {
				nMaxLen += (int)(CharNextA(lpsz) - lpsz);
			    continue;
		    }
            
		    int nItemLen = 0;
            
		    // handle '%' character with format
		    int nWidth = 0;
		    for (; *lpsz != '\0'; lpsz = CharNextA(lpsz))
		    {
			    // check for valid flags
			    if (*lpsz == '#')
				    nMaxLen += 2;   // for '0x'
			    else if (*lpsz == '*')
				    nWidth = va_arg(argList, int);
			    else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				    ;
			    else // hit non-flag character
				    break;
		    }
		    // get width and skip it
		    if (nWidth == 0)
		    {
			    // width indicated by
			    nWidth = atoi(lpsz);
			    for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = CharNextA(lpsz))
				    ;
		    }
		    assert(nWidth >= 0);
            
		    int nPrecision = 0;
		    if (*lpsz == '.')
		    {
			    // skip past '.' separator (width.precision)
			    lpsz = CharNextA(lpsz);
                
			    // get precision and skip it
			    if (*lpsz == '*')
			    {
				    nPrecision = va_arg(argList, int);
				    lpsz = CharNextA(lpsz);
			    }
			    else
			    {
				    nPrecision = atoi(lpsz);
				    for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = CharNextA(lpsz))
					    ;
			    }
			    assert(nPrecision >= 0);
		    }
            
		    // should be on type modifier or specifier
		    int nModifier = 0;
		    if(lpsz[0] == 'I' && lpsz[1] == '6' && lpsz[2] == '4')
		    {
			    lpsz += 3;
			    nModifier = FORCE_INT64;
		    }
		    else
		    {
			    switch (*lpsz)
			    {
                        // modifiers that affect size
                    case 'h':
                        nModifier = FORCE_ANSI;
                        lpsz = CharNextA(lpsz);
                        break;
                    case 'l':
                        nModifier = FORCE_UNICODE;
                        lpsz = CharNextA(lpsz);
                        break;
                        
                        // modifiers that do not affect size
                    case 'F':
                    case 'N':
                    case 'L':
                        lpsz = CharNextA(lpsz);
                        break;
			    }
		    }
            
		    // now should be on specifier
		    switch (*lpsz | nModifier)
		    {
                    // single characters
                case 'c':
                case 'C':
                    nItemLen = 2;
                    // Test
                    //argList = (va_list)((char*)argList + ((sizeof(TCHAR) + sizeof(int) - 1)&~(sizeof(int) - 1)));
                    va_arg(argList, TCHAR);
                    break;
                case 'c' | FORCE_ANSI:
                case 'C' | FORCE_ANSI:
                    nItemLen = 2;
                    //argList = (va_list)((char*)argList + ((sizeof(char) + sizeof(int) - 1)&~(sizeof(int) - 1)));
                    va_arg(argList, char);
                    break;
                case 'c' | FORCE_UNICODE:
                case 'C' | FORCE_UNICODE:
                    nItemLen = 4;
                    //argList = (va_list)((char*)argList + ((sizeof(WCHAR) + sizeof(int) - 1)&~(sizeof(int) - 1)));
                    va_arg(argList, WCHAR);
                    break;
                    
                    // strings
                case 's':
                {
                    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
                    if (pstrNextArg == NULL)
                    {
                        nItemLen = 6;  // "(null)"
                    }
                    else
                    {
                        nItemLen = strlen(pstrNextArg);
                        nItemLen = Max(1, nItemLen);
                    }
                    break;
                }
                    
                case 'S':
                {
#ifndef _UNICODE
                    LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
                    
                    /*const wchar_t* pstrNextArg = (wchar_t*)va_arg(argList, LPWSTR);//??????????????????????????????????????????*/
                    if (pstrNextArg == NULL)
                    {
                        nItemLen = 6;  // "(null)"
                    }
                    else
                    {
                        nItemLen = (int)wcslen(pstrNextArg);
                        nItemLen = Max(1, nItemLen);
                    }
#else
                    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
                    if (pstrNextArg == NULL)
                    {
                        nItemLen = 6; // "(null)"
                    }
                    else
                    {
                        nItemLen = strlen(pstrNextArg);
                        nItemLen = Max(1, nItemLen);
                    }
#endif
                    break;
                }
                    
                case 's' | FORCE_ANSI:
                case 'S' | FORCE_ANSI:
                {
                    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
                    if (pstrNextArg == NULL)
                    {
                        nItemLen = 6; // "(null)"
                    }
                    else
                    {
                        nItemLen = strlen(pstrNextArg);
                        nItemLen = Max(1, nItemLen);
                    }
                    break;
                }
                    
                case 's' | FORCE_UNICODE:
                case 'S' | FORCE_UNICODE:
                {
                    //LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
                    const wchar_t* pstrNextArg = (wchar_t*)va_arg(argList, LPWSTR);
                    if (pstrNextArg == NULL)
                    {
                        nItemLen = 6; // "(null)"
                    }
                    else
                    {
                        nItemLen = (int)wcslen(pstrNextArg);
                        nItemLen = Max(1, nItemLen);
                    }
                    break;
                }
		    }
            
		    // adjust nItemLen for strings
		    if (nItemLen != 0)
		    {
			    nItemLen = Max(nItemLen, nWidth);
			    if (nPrecision != 0) nItemLen = Min(nItemLen, nPrecision);				    
		    }
		    else
		    {
			    switch (*lpsz)
			    {
                        // integers
                    case 'd':
                    case 'i':
                    case 'u':
                    case 'x':
                    case 'X':
                    case 'o':
                        nModifier & FORCE_INT64 ? va_arg(argList, __int64) : va_arg(argList, int);
                        nItemLen = 32;
                        nItemLen = Max(nItemLen, nWidth + nPrecision);
                        break;
                        
                    case 'e':
                    case 'g':
                    case 'G':
                        va_arg(argList, double);
                        nItemLen = 128;
                        nItemLen = Max(nItemLen, nWidth + nPrecision);
                        break;
                    case 'f':
				    {
					    double f;
					    LPSTR pszTemp;
                        
					    // 312 == strlen("-1+(309 zeroes).")
					    // 309 zeroes == max precision of a double
					    // 6 == adjustment in case precision is not specified,
					    //   which means that the precision defaults to 6
					    pszTemp = (LPSTR)_alloca(Max(nWidth, 312 + nPrecision + 6));
                        
					    f = va_arg(argList, double);
					    sprintf(pszTemp, "%*.*f", nWidth, nPrecision + 6, f);
					    nItemLen = strlen(pszTemp);
				    }
                        break;
                        
                    case 'p':
                        va_arg(argList, void*);
                        nItemLen = 32;
                        nItemLen = Max(nItemLen, nWidth + nPrecision);
                        break;
                        
                        // no output
                    case 'n':
                        va_arg(argList, int*);
                        break;
                        
                    default:
                        assert(FALSE);  // unknown formatting option
			    }
		    }
            
		    // adjust nMaxLen for output nItemLen
		    nMaxLen += nItemLen;
	    }
        
	    if(GetBuffer(nMaxLen) == NULL) return FALSE;
        
	    int nRet = vsprintf(m_stStringDataA.data(), lpszFormat, argListSave);
	    //nRet;	// ref
	    assert(nRet <= GetAllocLength());
	    ReleaseBuffer();
        
	    va_end(argListSave);
	    return TRUE;
    }
    
    // formatting for localization (uses FormatMessage API)
    BOOL __cdecl Format(LPCSTR lpszFormat, ...)
    {
		assert(_IsValidString(lpszFormat));
        
	    va_list argList;
	    va_start(argList, lpszFormat);
	    BOOL bRet = FormatV(lpszFormat, argList);
	    va_end(argList);
	    return bRet;
    }
    
    LPSTR GetBuffer(int nMinBufLength)
    {
        assert(nMinBufLength >= 0);
        /*
        if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
        {
            KK_StringDataA* pOldData = GetData();
            int nOldLen = GetData()->nDataLength;
            if (nMinBufLength < nOldLen) nMinBufLength = nOldLen;                
            
            if(!AllocBuffer(nMinBufLength)) return NULL;                
            
            memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(CHAR));
            GetData()->nDataLength = nOldLen;
            Release(pOldData);
        }
        assert(GetData()->nRefs <= 1);
        
        assert(m_pchData != NULL);
        return m_pchData;
        */
        m_stStringDataA.resizeData(nMinBufLength + 1);
        return m_stStringDataA.data();
    }
    
    void ReleaseBuffer(int nNewLength = -1)
    {
        // CopyBeforeWrite();
        
        LPSTR lpStart = m_stStringDataA.data();
        if ( NULL != lpStart )
        {
            if ( nNewLength == -1 )
            {
                nNewLength = strlen(lpStart);
            }
            
            assert(nNewLength <= GetData()->nAllocLength);
            m_stStringDataA.nDataLength = nNewLength;
            lpStart[nNewLength] = '\0';
        }
    }
    
    LPSTR GetBufferSetLength(int nNewLength)
    {
        assert(nNewLength >= 0);
        
        if(GetBuffer(nNewLength) == NULL) return NULL;            
        
        LPSTR lpStart = m_stStringDataA.data();
        m_stStringDataA.nDataLength = nNewLength;
        lpStart[nNewLength] = '\0';
        return lpStart;
    }
    
    void FreeExtra()
    {
        assert(GetData()->nDataLength <= GetData()->nAllocLength);
        if ( GetData()->nDataLength != GetData()->nAllocLength )
        {
            /*
            KK_StringDataA* pOldData = GetData();
            if(AllocBuffer(GetData()->nDataLength))
            {
                memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(CHAR));
                assert(m_pchData[GetData()->nDataLength] == '\0');
                Release(pOldData);
            }
            */
            m_stStringDataA.resizeData(GetData()->nDataLength + 1);
        }
        // assert(GetData() != NULL);
    }
    
    /*
    LPSTR LockBuffer()
    {
        LPSTR lpsz = GetBuffer(0);
        if(lpsz != NULL) GetData()->nRefs = -1;
		
        return lpsz;
    }
    
    void UnlockBuffer()
    {
        assert(GetData()->nRefs == -1);
        if (!GetData()->IsNullString()) GetData()->nRefs = 1;            
    }
    */
    
public:
    ~KK_StringA()
    {
        /*
		if (!GetData()->IsNullString() && InterlockedDecrement(&GetData()->nRefs) <= 0)	
			delete[] (BYTE*)GetData();		
        //::HeapFree(::GetProcessHeap(), 0, GetData());
        */
        m_stStringDataA.deleteData();
    }  
    
    int GetAllocLength() const { return GetData()->nAllocLength; }
    
	static BOOL __stdcall _IsValidString(LPCWSTR lpwsz, int nLength = -1)//??????????????????????????????
    { return lpwsz == NULL ? false : !IsBadStringPtrW(lpwsz, nLength);}
    
	static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)//???????????????????????????????
    { return lpsz == NULL ? false : !IsBadStringPtrA(lpsz, nLength);}
    
protected:
    // LPSTR m_pchData;
    KK_StringDataA m_stStringDataA;
    
    const KK_StringDataA* GetData() const
    { 
		// assert(m_pchData != NULL);
		return &m_stStringDataA;
	}
    
    void Init()
    {
        // m_pchData = _GetEmptyString().m_pchData;
        m_stStringDataA.deleteData();
    }
    
    void AllocCopy(KK_StringA& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
    {
        int nNewLen = nCopyLen + nExtraLen;
        if ( nNewLen == 0 )
        {
            dest.Init();
        }
        else
        {
            if(dest.AllocBuffer(nNewLen)) memcpy(dest.m_stStringDataA.data(), GetData()->dataConst() + nCopyIndex, nCopyLen * sizeof(CHAR));
        }
    }
    
    
    BOOL AllocBuffer(int nLen)
    {
        assert(nLen >= 0);
        assert(nLen <= INT_MAX - 1); 
        
        /*
        if (nLen == 0)
        {
            Init();
        }
        else
        {
            KK_StringDataA* pData = NULL;
			pData = (KK_StringDataA*)new BYTE[sizeof(KK_StringDataA) + (nLen + 1) * sizeof(CHAR)];
            if(pData == NULL) return FALSE;                
            
            pData->nRefs = 1;
            pData->data()[nLen] = '\0';
            pData->nDataLength = nLen;
            pData->nAllocLength = nLen;
            m_pchData = pData->data();
        }
        
        return TRUE;
        */
        
        if ( NULL != m_stStringDataA.resizeData(nLen + 1) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    
    void AssignCopy(int nSrcLen, LPCSTR lpszSrcData)
    {
        if ( AllocBeforeWrite(nSrcLen) )
        {
            if ( (nSrcLen > 0) && (NULL != lpszSrcData) )
            {
                memcpy(m_stStringDataA.data(), lpszSrcData, nSrcLen * sizeof(CHAR));
                m_stStringDataA.nDataLength = nSrcLen;
            }
        }
    }
    
    
    BOOL ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data, int nSrc2Len, LPCSTR lpszSrc2Data)
    {
        BOOL bRet = TRUE;
        int nNewLen = nSrc1Len + nSrc2Len;
        if (nNewLen != 0)
        {
            bRet = AllocBuffer(nNewLen);
            if ( bRet )
            {
                memcpy(m_stStringDataA.data(), lpszSrc1Data, nSrc1Len * sizeof(CHAR));
                memcpy(m_stStringDataA.data() + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(CHAR));
            }
        }
        return bRet;
    }
    
    void ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData)
    {
        /*
        if (nSrcLen == 0) return;
        
        if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
        {
            KK_StringDataA* pOldData = GetData();
            if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
            {
                assert(pOldData != NULL);
                KK_StringA::Release(pOldData);
            }
        }
        else
        {
            memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(CHAR));
            GetData()->nDataLength += nSrcLen;
            assert(GetData()->nDataLength <= GetData()->nAllocLength);
            m_pchData[GetData()->nDataLength] = '\0';
        }
        */
        
        if ( (nSrcLen <= 0) || (NULL == lpszSrcData) )
        {
            return;
        }
        
        if ( GetData()->nDataLength + nSrcLen > GetData()->nAllocLength )
        {
            ConcatCopy(GetData()->nDataLength, GetData()->dataConst(), nSrcLen, lpszSrcData);
        }
        else
        {
            LPSTR lpStart = m_stStringDataA.data();
            memcpy(lpStart + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(CHAR));
            m_stStringDataA.nDataLength += nSrcLen;
            assert(GetData()->nDataLength <= GetData()->nAllocLength);
            lpStart[GetData()->nDataLength] = '\0';
        }
    }
    
    void CopyBeforeWrite()
    {
        /*
        if (GetData()->nRefs > 1)
        {
            KK_StringDataA* pData = GetData();
            Release();
            if(AllocBuffer(pData->nDataLength)) memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(CHAR));
        }
        assert(GetData()->nRefs <= 1);
        */
    }
    
    BOOL AllocBeforeWrite(int nLen)
    {
        /*
        BOOL bRet = TRUE;
        if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
        {
            Release();
            bRet = AllocBuffer(nLen);
        }
        assert(GetData()->nRefs <= 1);
        return bRet;
        */
        
        BOOL bRet = TRUE;
        
        if ( nLen > GetData()->nAllocLength )
        {
            bRet = AllocBuffer(nLen);
        }
        
        return bRet;
    }
    
    /*
    void Release()
    {
        if (!GetData()->IsNullString())
        {
            assert(GetData()->nRefs != 0);
            if (InterlockedDecrement(&GetData()->nRefs) <= 0) 
				delete[] (BYTE*)GetData();				
            //::HeapFree(::GetProcessHeap(), 0, GetData());
            
            
            Init();
        }
    }
    
    static void PASCAL Release(KK_StringDataA* pData)
    {
        if (!pData->IsNullString())
        {
            assert(pData->nRefs != 0);
            if (InterlockedDecrement(&pData->nRefs) <= 0) 	
				delete[] (BYTE*)pData;			
            //::HeapFree(::GetProcessHeap(), 0, pData);
            
        }
    }
    */
    
    static int PASCAL SafeStrlen(LPCSTR lpsz) { return (lpsz == NULL) ? 0 : strlen(lpsz); }
    
    // static const KK_StringA& __stdcall _GetEmptyString() { return *(KK_StringA*)&_atltmpPchNilA; }
    
    
    static int __cdecl _wcstombsz(char* mbstr, const WCHAR* wcstr, size_t count)
    {
        if (count == 0 && mbstr != NULL) return 0;
		
        //int result = WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);//
		int result = wcstombs(mbstr, wcstr, count);//::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
		assert(mbstr == NULL || result <= (int)count);
        if (result > 0) mbstr[result] = 0;
        
        return result;
    }
    
	static int __cdecl _getmbslen(const WCHAR* wcstr) 
	{ 
		return  wcstombs(NULL, wcstr, 0);//::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, NULL, 0, NULL, NULL) - 1;////::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, NULL, 0, NULL, NULL) - 1;
	}
    
	static int __cdecl _mbstowcsz(WCHAR* wcstr, const char* mbstr, size_t count)
    { 
		if (count == 0 && wcstr != NULL) return 0;          
        
        int result = mbstowcs(wcstr, mbstr, count);//MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);//;//::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
        assert(wcstr == NULL || result <= (int)count);
        if (result > 0) wcstr[result] = 0;
        
        return result;
	}
    
};

inline bool __stdcall operator==(const KK_StringA& s1, const KK_StringA& s2) { return s1.Compare(s2) == 0; }

inline bool __stdcall operator==(const KK_StringA& s1, LPCSTR s2) { return s1.Compare(s2) == 0; }

inline bool __stdcall operator==(LPCSTR s1, const KK_StringA& s2) { return s2.Compare(s1) == 0; }

inline bool __stdcall operator!=(const KK_StringA& s1, const KK_StringA& s2) { return s1.Compare(s2) != 0; }

inline bool __stdcall operator!=(const KK_StringA& s1, LPCSTR s2) { return s1.Compare(s2) != 0; }

inline bool __stdcall operator!=(LPCSTR s1, const KK_StringA& s2) { return s2.Compare(s1) != 0; }

inline bool __stdcall operator<(const KK_StringA& s1, const KK_StringA& s2) { return s1.Compare(s2) < 0; }

inline bool __stdcall operator<(const KK_StringA& s1, LPCSTR s2) { return s1.Compare(s2) < 0; }

inline bool __stdcall operator<(LPCSTR s1, const KK_StringA& s2) { return s2.Compare(s1) > 0; }

inline bool __stdcall operator>(const KK_StringA& s1, const KK_StringA& s2) { return s1.Compare(s2) > 0; }

inline bool __stdcall operator>(const KK_StringA& s1, LPCSTR s2) { return s1.Compare(s2) > 0; }

inline bool __stdcall operator>(LPCSTR s1, const KK_StringA& s2) { return s2.Compare(s1) < 0; }

inline bool __stdcall operator<=(const KK_StringA& s1, const KK_StringA& s2) { return s1.Compare(s2) <= 0; }

inline bool __stdcall operator<=(const KK_StringA& s1, LPCSTR s2) { return s1.Compare(s2) <= 0; }

inline bool __stdcall operator<=(LPCSTR s1, const KK_StringA& s2) { return s2.Compare(s1) >= 0; }

inline bool __stdcall operator>=(const KK_StringA& s1, const KK_StringA& s2) { return s1.Compare(s2) >= 0; }

inline bool __stdcall operator>=(const KK_StringA& s1, LPCSTR s2) { return s1.Compare(s2) >= 0; }

inline bool __stdcall operator>=(LPCSTR s1, const KK_StringA& s2) { return s2.Compare(s1) <= 0; }

inline KK_StringA __stdcall operator+(const KK_StringA& string1, const KK_StringA& string2)
{
    KK_StringA s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), string2.GetData()->nDataLength, string2.GetData()->dataConst());
    return s;
}

inline KK_StringA __stdcall operator+(const KK_StringA& string1, char ch)
{
    KK_StringA s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), 1, &ch);
    return s;
}

inline KK_StringA __stdcall operator+(const KK_StringA& string1, WCHAR wch)
{
    KK_StringA s;
    KK_StringA ch((LPCWSTR)&wch, 1);
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), 1, ch);
    return s;
}

inline KK_StringA __stdcall operator+(WCHAR wch, const KK_StringA& string1)
{
    KK_StringA s;
    KK_StringA ch((LPCWSTR)&wch, 1);
    s.ConcatCopy(1, ch, string1.GetData()->nDataLength, string1.GetData()->dataConst());
    return s;
}

inline KK_StringA __stdcall operator+(char ch, const KK_StringA& string1)
{
    KK_StringA s;
    s.ConcatCopy(1, &ch, string1.GetData()->nDataLength, string1.GetData()->dataConst());
    return s;
}

#endif
