//
//  KK_StringU.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//


//////////////////////////////////////////////////////////////////////
// include guards
//////////////////////////////////////////////////////////////////////

#ifndef __KK_STRING_U_H__
#define __KK_STRING_U_H__

#include "KK_Includes.h"
#include "KK_BaseFunctions.h"

class KK_StringA;
class KK_StringW;
class KK_StringU;

const long NULLSTRING_U = -10001;

//////////////////////////////////////////////////////////////////////
// additional defines
//////////////////////////////////////////////////////////////////////
// 注释［2015.09.07］：考虑到32Bit和64Bit的兼容性，修改KK_StringDataU数据结构的定义
// 注释［2015.09.07］：废弃字符串内存共享模式
struct KK_StringDataU
{
    /*
    long    nRefs;     // reference count
    */
    long    nDataLength;
    long    nAllocLength;
    BYTE*   ptrByte;
    
    KK_StringDataU(void) : // nRefs(NULLSTRING_U),
                           nDataLength(0),
                           nAllocLength(0),
                           ptrByte(NULL)
    {
        // Do Nothing
    }
    
    ~KK_StringDataU(void)
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
        // nRefs     = NULLSTRING_U;
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
	// bool IsNullString() { return ( nRefs == NULLSTRING_U); }
};

KK_StringU __stdcall operator+(const KK_StringU& string1, const KK_StringU& string2);
KK_StringU __stdcall operator+(const KK_StringU& KK_StringA, char ch);
KK_StringU __stdcall operator+(const KK_StringU& KK_StringA, WCHAR ch);
KK_StringU __stdcall operator+(WCHAR ch, const KK_StringU& KK_StringA);
KK_StringU __stdcall operator+(char ch, const KK_StringU& KK_StringA);

//////////////////////////////////////////////////////////////////////
// global data
//////////////////////////////////////////////////////////////////////

// For an empty KK_StringA, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty KK_StringA data (and locked)

// static long rgInitDataU[] = { NULLSTRING_U, 0, 0, 0 };
// static LPCSTR _atltmpPchNilU = (LPCSTR)(((BYTE*)&rgInitDataU) + sizeof(KK_StringDataU));
// static KK_StringDataU* _atltmpPchNilU = (KK_StringDataU*)rgInitDataU;

// static KK_StringDataU s_stShareInitStringDataU;

//////////////////////////////////////////////////////////////////////
// KK_StringU class
//////////////////////////////////////////////////////////////////////

class KK_StringU
{
public:
    // Constructors
    KK_StringU() 
    { Init(); }
    
    KK_StringU(const KK_StringA& stringSrc);
    KK_StringU(const KK_StringW& stringSrc);
	KK_StringU(const KK_StringU& stringSrc)
    {
        /*
        assert(stringSrc.GetData()->nRefs != 0);
        if ( stringSrc.GetData()->nRefs >= 0 )
        {
            //            assert(stringSrc.GetData() != _atltmpDataNilU);
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
    
    KK_StringU(CHAR ch, int nRepeat = 1)
    {
        Init();
		if ( nRepeat >= 1 && AllocBuffer(nRepeat) )
        {
            /*
			int i;
			for (i = 0; i < nRepeat; i++)
				m_pchData[i] = ch;
            */
            
            memset(m_stStringDataU.data(), ch, sizeof(CHAR) * nRepeat);
        }
    }
    
    KK_StringU(LPCSTR lpsz)
    {
        Init();
        if ( lpsz == NULL /*&& HIWORD(lpsz) == 0*/)
        {
			assert(0);
        }
        else
        {
            /*
            int nSrcLen = (lpsz != NULL) ? strlen(lpsz) : 0;
			if (nSrcLen != 0 && AllocBuffer(nSrcLen)) memcpy(m_pchData, lpsz, nSrcLen * sizeof(CHAR));
            */
            
            int nSrcLen = strlen(lpsz);
            if ( TRUE == AllocBuffer(nSrcLen) )
            {
                memcpy(m_stStringDataU.data(), lpsz, nSrcLen * sizeof(CHAR));
            }
        }
    }
    
    KK_StringU(LPCSTR lpsz, int nLength)
    {
        Init();
		// if (nLength != 0 && AllocBuffer(nLength)) memcpy(m_pchData, lpsz, nLength * sizeof(CHAR));
        if ( (NULL != lpsz) && (nLength > 0) && (strlen(lpsz) >= nLength) )
        {
            if ( TRUE == AllocBuffer(nLength) )
            {
                memcpy(m_stStringDataU.data(), lpsz, nLength * sizeof(CHAR));
            }
        }
    }
    
    KK_StringU(LPCWSTR lpsz)
    {
        Init();
        int nSrcLen = (lpsz != NULL) ? _getutf8len(lpsz) : 0;
		if (nSrcLen != 0 && AllocBuffer(nSrcLen))
        {
			_wcstoutf8z(m_stStringDataU.data(), lpsz, nSrcLen + 1);
			ReleaseBuffer();            
        }
    }
    
    KK_StringU(LPCWSTR lpch, int nLength)
    {
        Init();
		if (nLength != 0 && AllocBuffer(nLength))
        {
			_wcstoutf8z(m_stStringDataU.data(), lpch, nLength);
			ReleaseBuffer();
        }
    }
    
    KK_StringU(const unsigned char* psz)
	{ 
		Init();
		*this = (LPCSTR)psz; 
	}
    
    const KK_StringU& Assign(LPCSTR lpszSrcData, int nSrcLen)
	{
		AssignCopy(nSrcLen, lpszSrcData);
		return *this; 
	}
    
    // Attributes & Operations
    // as an array of characters
    int GetLength() const { return GetData()->nDataLength; }
    
    BOOL IsEmpty() const { return GetData()->nDataLength == 0; }
    
    void Empty()                       // free up the data
    {
        /*
        if (GetData()->nDataLength == 0) return;
        
        if (GetData()->nRefs >= 0)
            Release();
        else
			*this = "";
        //*this = L"";
        
        assert(GetData()->nDataLength == 0);
        assert(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
        */
        
        if (GetData()->nDataLength == 0) return;
        
        *this = "";
        
        assert(GetData()->nDataLength == 0);
        assert(GetData()->nAllocLength == 0);
    }
    
    CHAR GetAt(int nIndex) const       // 0 based
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        return (m_stStringDataU.dataConst())[nIndex];
    }
    
    CHAR & operator[](int nIndex) const  // same as GetAt
    {
        // same as GetAt
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        return (m_stStringDataU.dataConst())[nIndex];
    }
    
    void SetAt(int nIndex, CHAR ch)
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        
        CopyBeforeWrite();
        (m_stStringDataU.data())[nIndex] = ch;
    }
    
    operator LPCSTR() const { return m_stStringDataU.dataConst(); } // as a C KK_StringA
    
    
    const KK_StringU& operator=(const KK_StringA& stringSrc);
    const KK_StringU& operator=(const KK_StringW& stringSrc);
    
    // overloaded assignment
    const KK_StringU& operator=(const KK_StringU& stringSrc)
    {
        /*
        if (m_pchData != stringSrc.m_pchData)
        {
            //            if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNilU) || stringSrc.GetData()->nRefs < 0)
            if ((GetData()->nRefs < 0 && !GetData()->IsNullString()) || stringSrc.GetData()->nRefs < 0)
            {
                // actual copy necessary since one of the strings is locked
                AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
            }
            else
            {
                // can just copy references around
                Release();
                //                assert(stringSrc.GetData() != _atltmpDataNilU);
                assert(!stringSrc.GetData()->IsNullString());
                m_pchData = stringSrc.m_pchData;
                InterlockedIncrement(&GetData()->nRefs);
            }
        }
        return *this;
        */
        
        if ( this != &stringSrc )
        {
            AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.GetData()->dataConst());
        }
        
        return *this;
    }
    
    const KK_StringU& operator=(WCHAR wch)
    {
        KK_StringU ch((LPCWSTR)wch, 1);
        AssignCopy(1, ch);
        return *this;
    }
    
    const KK_StringU& operator=(CHAR ch)
    {
        AssignCopy(1, &ch);
        return *this;
    }
    
    const KK_StringU& operator=(LPCSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        AssignCopy(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    
    const KK_StringU& operator=(LPCWSTR lpsz)
    {
        int nSrcLen = (lpsz != NULL) ? _getutf8len(lpsz) : 0;
        if ( AllocBeforeWrite(nSrcLen) )
        {
            if ( (nSrcLen > 0) && (NULL != lpsz) )
            {
                _wcstoutf8z(m_stStringDataU.data(), lpsz, nSrcLen + 1);
            }
            ReleaseBuffer();
        }
        return *this;
    }
    
    const KK_StringU& operator=(const unsigned char* psz)
	{ 
		*this = (LPCSTR)psz;
		return *this; 
	}
    
    // KK_StringA concatenation
    const KK_StringU& operator+=(LPCWSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        int nSrcLen = (lpsz != NULL) ? _getutf8len(lpsz) : 0;
        ConcatInPlace(nSrcLen, KK_StringU(lpsz));
        return *this;
    }
    
    const KK_StringU& operator+=(CHAR ch)
    {
        ConcatInPlace(1, &ch);
        return *this;
    }
    
    const KK_StringU& operator+=(LPCSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        ConcatInPlace(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    
    // Concatenation
    // NOTE: "operator+" is done as friend functions for simplicity
    
    friend KK_StringU __stdcall operator+(const KK_StringU& string1, const KK_StringU& string2);
    friend KK_StringU __stdcall operator+(const KK_StringU& KK_StringA, char ch);
    friend KK_StringU __stdcall operator+(const KK_StringU& KK_StringA, WCHAR ch);
    friend KK_StringU __stdcall operator+(WCHAR ch, const KK_StringU& KK_StringA);
    friend KK_StringU __stdcall operator+(char ch, const KK_StringU& KK_StringA);
    
    // KK_StringA comparison
    int Compare(LPCSTR lpsz) const { return strcmp(m_stStringDataU.dataConst(), lpsz); }    // MBCS/Unicode aware
    
    int CompareNoCase(LPCSTR lpsz) const { return strcasecmp(m_stStringDataU.dataConst(), lpsz); }   // MBCS/Unicode aware // ignore case
    
    
    int Compare(LPCSTR lpsz, size_t size) const { return strncmp(m_stStringDataU.dataConst(), lpsz, size); }    // MBCS/Unicode aware
    
    int CompareNoCase(LPCSTR lpsz, size_t size) const { return strncasecmp(m_stStringDataU.dataConst(), lpsz, size); } // MBCS/Unicode aware // ignore case
    
    
    // simple sub-KK_StringA extraction 
    KK_StringU Mid(int nFirst, int nCount) const
    {
        // out-of-bounds requests return sensible things
        if (nFirst < 0) nFirst = 0;            
        if (nCount < 0) nCount = 0;            
        
        if (nFirst + nCount > GetData()->nDataLength) nCount = GetData()->nDataLength - nFirst;            
        if (nFirst > GetData()->nDataLength) nCount = 0;  
        
        /* 
         if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
         return *this;
         */
        if (nFirst == 0 && nCount == GetData()->nDataLength) return *this;
        
        KK_StringU dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }
    
    KK_StringU Mid(int nFirst) const
    {
        return Mid(nFirst, GetData()->nDataLength - nFirst);
    }
    
    KK_StringU Left(int nCount) const
    {
        /*
         if (nCount < 0)
         nCount = 0;
         else if (nCount > GetData()->nDataLength)
         nCount = GetData()->nDataLength;*/
        if (nCount < 0)
            nCount = 0;
        else if (nCount >= GetData()->nDataLength)
            return *this;
        
        KK_StringU dest;
        AllocCopy(dest, nCount, 0, 0);
        return dest;
    }
    
    KK_StringU Right(int nCount) const
    {
        /*
         if (nCount < 0)
         nCount = 0;
         else if (nCount > GetData()->nDataLength)
         nCount = GetData()->nDataLength;*/
        if (nCount < 0)
            nCount = 0;
        else if (nCount >= GetData()->nDataLength)
            return *this;
        
        KK_StringU dest;
        AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
        return dest;
    }
    
    KK_StringU SpanIncluding(LPCSTR lpszCharSet) const
    {
        // strspn equivalent
        assert(_IsValidString(lpszCharSet));
        return Left(strspn(GetData()->dataConst(), lpszCharSet));
        
    }
    
    KK_StringU SpanExcluding(LPCSTR lpszCharSet) const
    {
        // strcspn equivalent
        assert(_IsValidString(lpszCharSet));
        return Left(strcspn(GetData()->dataConst(), lpszCharSet));
    }
    
    // upper/lower/reverse conversion
    void MakeUpper()
    {
        CopyBeforeWrite();
        CharUpperA(m_stStringDataU.data());
    }
    
    void MakeLower()
    {
        CopyBeforeWrite();
        CharLowerA(m_stStringDataU.data());
    }
    
    void MakeReverse()
    {
        CopyBeforeWrite();
        strrev(m_stStringDataU.data());
    }
    
    // trimming whitespace (either side)
    void TrimRight()
    {
        CopyBeforeWrite();
        
        // find beginning of trailing spaces by starting at beginning (DBCS aware)
        LPSTR lpsz = m_stStringDataU.data();
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
            lpsz = ::CharNextA(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            // truncate at trailing space start
            *lpszLast = '\0';
            m_stStringDataU.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataU.data());
        }
    }
    
    void TrimLeft()
    {
        CopyBeforeWrite();
        
        // find first non-space character
        LPCSTR lpsz = m_stStringDataU.data();
        while (isspace(*lpsz))
            lpsz = ::CharNextA(lpsz);
        
        // fix up data and length
        int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_stStringDataU.data());
        memmove(m_stStringDataU.data(), lpsz, (nDataLength + 1) * sizeof(CHAR));
        m_stStringDataU.nDataLength = nDataLength;
    }
    
    // remove continuous occurrences of chTarget starting from right
    void TrimRight(CHAR chTarget)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)
        
        CopyBeforeWrite();
        LPSTR lpsz = m_stStringDataU.data();
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
            lpsz = ::CharNextA(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = '\0';
            m_stStringDataU.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataU.data());
        }
    }
    
    // remove continuous occcurrences of characters in passed KK_StringA,
    // starting from right
    void TrimRight(LPCSTR lpszTargets)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)
        
        CopyBeforeWrite();
        LPSTR lpsz = m_stStringDataU.data();
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
            lpsz = ::CharNextA(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = '\0';
            m_stStringDataU.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataU.data());
        }
    }
    
    
    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(CHAR chTarget)
    {
        // find first non-matching character
        
        CopyBeforeWrite();
        LPCSTR lpsz = m_stStringDataU.data();
        
        while (chTarget == *lpsz)
            lpsz = ::CharNextA(lpsz);
        
        if (lpsz != m_stStringDataU.data())
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_stStringDataU.data());
            memmove(m_stStringDataU.data(), lpsz, (nDataLength + 1) * sizeof(CHAR));
            m_stStringDataU.nDataLength = nDataLength;
        }
    }
    
    // remove continuous occcurrences of characters in
    // passed KK_StringA, starting from left
    void TrimLeft(LPCSTR lpszTargets)
    {
        // if we're not trimming anything, we're not doing any work
        if (SafeStrlen(lpszTargets) == 0) return;
        
        CopyBeforeWrite();
        LPCSTR lpsz = m_stStringDataU.data();
        
        while (*lpsz != '\0')
        {
            if (strchr(lpszTargets, *lpsz) == NULL) break;
            lpsz = ::CharNextA(lpsz);
        }
        
        if (lpsz != m_stStringDataU.data())
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_stStringDataU.data());
            memmove(m_stStringDataU.data(), lpsz, (nDataLength + 1) * sizeof(CHAR));
            m_stStringDataU.nDataLength = nDataLength;
        }
    }
    
    // advanced manipulation
    // replace occurrences of chOld with chNew
    int Replace(CHAR chOld, CHAR chNew)
    {
        int nCount = 0;
        
        // short-circuit the nop case
        if (chOld != chNew)
        {
            // otherwise modify each character that matches in the KK_StringA
            CopyBeforeWrite();
            LPSTR psz = m_stStringDataU.data();
            LPSTR pszEnd = psz + GetData()->nDataLength;
            while (psz < pszEnd)
            {
                // replace instances of the specified character only
                if (*psz == chOld)
                {
                    *psz = chNew;
                    nCount++;
                }
                psz = ::CharNextA(psz);
            }
        }
        return nCount;
    }
    
    // replace occurrences of substring lpszOld with lpszNew;
    // empty lpszNew removes instances of lpszOld
    int Replace(LPCSTR lpszOld, LPCSTR lpszNew)
    {
        // can't have empty or NULL lpszOld
        
        int nSourceLen = SafeStrlen(lpszOld);
        if (nSourceLen == 0) return 0;
        int nReplacementLen = SafeStrlen(lpszNew);
        
        // loop once to figure out the size of the result KK_StringA
        int nCount = 0;
        LPSTR lpszStart = m_stStringDataU.data();
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
            //   allocate a new buffer (slow but sure)
            int nOldLength = GetData()->nDataLength;
            int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;
            /*
            if (GetData()->nAllocLength < nNewLength)
            {
                KK_StringDataU* pOldData = GetData();
                LPSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength)) return -1;
                memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(CHAR));
                Release(pOldData);
                
                
            }
            */
            m_stStringDataU.resizeData(nNewLength + 1);
            
            // else, we just do it in-place
            lpszStart = m_stStringDataU.data();
            lpszEnd   = lpszStart + GetData()->nDataLength;
            
            // loop again to actually do the work
            while (lpszStart < lpszEnd)
            {
                while ( (lpszTarget = strstr(lpszStart, lpszOld)) != NULL)
                {
                    int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_stStringDataU.data()) + nSourceLen);
                    memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(CHAR));
                    memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(CHAR));
                    lpszStart = lpszTarget + nReplacementLen;
                    lpszStart[nBalance] = '\0';
                    nOldLength += (nReplacementLen - nSourceLen);
                }
                lpszStart += strlen(lpszStart) + 1;
            }
            m_stStringDataU.nDataLength = nNewLength;
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
    
    // remove occurrences of chRemove
    int Remove(CHAR chRemove)
    {
        CopyBeforeWrite();
        
        LPSTR pstrSource = m_stStringDataU.data();
        LPSTR pstrDest   = pstrSource;
        LPSTR pstrEnd    = pstrSource + GetData()->nDataLength;
        
        while (pstrSource < pstrEnd)
        {
            if (*pstrSource != chRemove)
            {
                *pstrDest = *pstrSource;
                pstrDest = ::CharNextA(pstrDest);
            }
            pstrSource = ::CharNextA(pstrSource);
        }
        *pstrDest = '\0';
        int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
        m_stStringDataU.nDataLength -= nCount;
        
        return nCount;
    }
    
    // insert character at zero-based index; concatenates
    // if index is past end of KK_StringA
    int Insert(int nIndex, CHAR ch)
    {
        CopyBeforeWrite();
        
        if (nIndex < 0)
            nIndex = 0;
        
        int nNewLength = GetData()->nDataLength;
        if (nIndex > nNewLength) nIndex = nNewLength;
        nNewLength++;
        /*
        if (GetData()->nAllocLength < nNewLength)
        {
            KK_StringDataU* pOldData = GetData();
            LPSTR pstr = m_pchData;
            if(!AllocBuffer(nNewLength)) return -1;
            memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(CHAR));
            Release(pOldData);
        }
        */
        m_stStringDataU.resizeData(nNewLength);
        
        // move existing bytes down
        LPSTR lpStart = m_stStringDataU.data();
        memmove(lpStart + nIndex + 1, lpStart + nIndex, (nNewLength - nIndex) * sizeof(CHAR));
        lpStart[nIndex] = ch;
        m_stStringDataU.nDataLength = nNewLength;
        
        return nNewLength;
    }
    
    // insert substring at zero-based index; concatenates
    // if index is past end of KK_StringA
    int Insert(int nIndex, LPCSTR pstr)
    {
        if (nIndex < 0) nIndex = 0;
        
        int nInsertLength = SafeStrlen(pstr);
        int nNewLength = GetData()->nDataLength;
        if (nInsertLength > 0)
        {
            CopyBeforeWrite();
            if (nIndex > nNewLength) nIndex = nNewLength;
            nNewLength += nInsertLength;
            /*
            if (GetData()->nAllocLength < nNewLength)
            {
                KK_StringDataU* pOldData = GetData();
                LPSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength)) return -1;
                memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(CHAR));
                Release(pOldData);
            }
            */
            
            m_stStringDataU.resizeData(nNewLength + 1);
            
            // move existing bytes down
            LPSTR lpStart = m_stStringDataU.data();
            memmove(lpStart + nIndex + nInsertLength, lpStart + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(CHAR));
            memcpy(lpStart + nIndex, pstr, nInsertLength * sizeof(CHAR));
            m_stStringDataU.nDataLength = nNewLength;
        }
        
        return nNewLength;
    }
    
    // delete nCount characters starting at zero-based index
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
        
        LPSTR lpStart = m_stStringDataU.data();
		if (nCount == -1 && nIndex < nNewLength)
		{
            CopyBeforeWrite();
			lpStart[nIndex] = 0;
            m_stStringDataU.nDataLength = nIndex;
			nCount = 0;
			nNewLength = nIndex;
		}
        else if (nCount > 0 && nIndex < nNewLength)
        {
            CopyBeforeWrite();
            int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
            
            memmove(lpStart + nIndex, lpStart + nIndex + nCount, nBytesToCopy * sizeof(CHAR));
            m_stStringDataU.nDataLength = nNewLength - nCount;
        }
        
        return nNewLength - nCount;
    }
    
	int Erase(int nIndex, int nCount = -1) { return Delete(nIndex, nCount); }
    
    // searching (return starting index, or -1 if not found)
    // look for a single character match
    int Find(CHAR ch) const { return Find(ch, 0); }   // like "C" strchr
    
    
    int ReverseFind(CHAR ch) const
    {
        // find last single character
        LPSTR lpStart = GetData()->dataConst();
        LPSTR lpsz    = strrchr(lpStart, ch);
        
        // return -1 if not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - lpStart);
    }
    
    int Find(CHAR ch, int nStart) const          // starting at index
    {
        int nLength = GetData()->nDataLength;
        if (nStart >= nLength) return -1;
        
        // find first single character
        LPSTR lpStart = GetData()->dataConst();
        LPSTR lpsz    = strchr(lpStart + nStart, ch);
        
        // return -1 if not found and index otherwise
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
    
    // look for a specific sub-KK_StringA
    int Find(LPCSTR lpszSub) const { return Find(lpszSub, 0); }  // like "C" strstr        
    
    int Find(LPCSTR lpszSub, int nStart) const  // starting at index
    {
        assert(_IsValidString(lpszSub));
        
        int nLength = GetData()->nDataLength;
        if (nStart > nLength) return -1;
        
        // find first matching substring
        LPSTR lpStart = GetData()->dataConst();
        LPSTR lpsz    = strstr(lpStart + nStart, lpszSub);
        
        // return -1 for not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - lpStart);
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(char n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(short n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(int n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(long n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ltoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(__int64 n, int radix = 10)
    {
        CHAR szBuffer[80];
        _i64toa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(unsigned char n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(unsigned short n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(unsigned int n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(unsigned long n, int radix = 10)
    {
        CHAR szBuffer[40];
        _ultoa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringU& Append(unsigned __int64 n, int radix = 10)
    {
        CHAR szBuffer[80];
        _ui64toa(n, szBuffer, radix);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
	
    // Concatentation for non strings
    const KK_StringU& Append(double n, int digits = 20)
    {
        CHAR szBuffer[80];
		gcvt(n, digits, szBuffer);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    BOOL FormatV(LPCSTR lpszFormat, va_list argList)
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
	    for (lpsz = lpszFormat; *lpsz != '\0'; lpsz = ::CharNextA(lpsz))
	    {
		    // handle '%' character, but watch out for '%%'
		    if (*lpsz != '%' || *(lpsz = ::CharNextA(lpsz)) == '%')
		    {
			    // this is instead of _tclen()
			    nMaxLen += (int)(::CharNextA(lpsz) - lpsz);
			    continue;
		    }
            
		    int nItemLen = 0;
            
		    // handle '%' character with format
		    int nWidth = 0;
		    for (; *lpsz != '\0'; lpsz = ::CharNextA(lpsz))
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
			    for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = ::CharNextA(lpsz))
				    ;
		    }
		    assert(nWidth >= 0);
            
		    int nPrecision = 0;
		    if (*lpsz == '.')
		    {
			    // skip past '.' separator (width.precision)
			    lpsz = ::CharNextA(lpsz);
                
			    // get precision and skip it
			    if (*lpsz == '*')
			    {
				    nPrecision = va_arg(argList, int);
				    lpsz = ::CharNextA(lpsz);
			    }
			    else
			    {
				    nPrecision = atoi(lpsz);
				    for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = ::CharNextA(lpsz))
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
                        lpsz = ::CharNextA(lpsz);
                        break;
                    case 'l':
                        nModifier = FORCE_UNICODE;
                        lpsz = ::CharNextA(lpsz);
                        break;
                        
                        // modifiers that do not affect size
                    case 'F':
                    case 'N':
                    case 'L':
                        lpsz = ::CharNextA(lpsz);
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
                    LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
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
        
	    if(GetBuffer(nMaxLen) == NULL)
		    return FALSE;
	    int nRet = vsprintf(m_stStringDataU.data(), lpszFormat, argListSave);
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
    
    // Access to KK_StringA implementation buffer as "C" character array
    LPSTR GetBuffer(int nMinBufLength)
    {
        assert(nMinBufLength >= 0);
        /*
        if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
        {
            // we have to grow the buffer
            KK_StringDataU* pOldData = GetData();
            int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
            if (nMinBufLength < nOldLen) nMinBufLength = nOldLen;
            
            if(!AllocBuffer(nMinBufLength)) return NULL;
            
            memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(CHAR));
            GetData()->nDataLength = nOldLen;
            KK_StringU::Release(pOldData);
        }
        assert(GetData()->nRefs <= 1);
        
        // return a pointer to the character storage for this KK_StringA
        assert(m_pchData != NULL);
        */
        m_stStringDataU.resizeData(nMinBufLength + 1);
        return m_stStringDataU.data();
    }
    
    void ReleaseBuffer(int nNewLength = -1)
    {
        // CopyBeforeWrite();  // just in case GetBuffer was not called
        
        CHAR* pacData = m_stStringDataU.data();
        if ( NULL != pacData )
        {
            if (nNewLength == -1) nNewLength = strlen(pacData); // zero terminated
            
            assert(nNewLength <= GetData()->nAllocLength);
            m_stStringDataU.nDataLength = nNewLength;
            pacData[nNewLength] = '\0';
        }
    }
    
    LPSTR GetBufferSetLength(int nNewLength)
    {
        assert(nNewLength >= 0);
        
        if(GetBuffer(nNewLength) == NULL) return NULL;
        
        LPSTR lpStart = m_stStringDataU.data();
        
        m_stStringDataU.nDataLength = nNewLength;
        lpStart[nNewLength]    = '\0';
        
        return lpStart;
    }
    
    void FreeExtra()
    {
        assert(GetData()->nDataLength <= GetData()->nAllocLength);
        if (GetData()->nDataLength != GetData()->nAllocLength)
        {
            /*
            KK_StringDataU* pOldData = GetData();
            if(AllocBuffer(GetData()->nDataLength))
            {
                memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(CHAR));
                assert(m_pchData[GetData()->nDataLength] == '\0');
                KK_StringU::Release(pOldData);
            }
            */
            m_stStringDataU.resizeData(GetData()->nDataLength + 1);
        }
        // assert(GetData() != NULL);
    }
    
    /*
    // Use LockBuffer/UnlockBuffer to turn refcounting off
    LPSTR LockBuffer()
    {
        LPSTR lpsz = GetBuffer(0);
        if(lpsz != NULL) GetData()->nRefs = -1;
        return lpsz;
    }
    
    void UnlockBuffer()
    {
        assert(GetData()->nRefs == -1);
        //        if (GetData() != _atltmpDataNilU)
        if (!GetData()->IsNullString()) GetData()->nRefs = 1;
    }
    */
    // Implementation
public:
    ~KK_StringU()
    {
        // free any attached data
        //        if (GetData() != _atltmpDataNilU)
        /*
        if (!GetData()->IsNullString() && InterlockedDecrement(&GetData()->nRefs) <= 0) //::HeapFree(::GetProcessHeap(), 0, GetData());
            delete[] (BYTE*)GetData();
        */
        m_stStringDataU.deleteData();
    }
    
    int GetAllocLength() const { return GetData()->nAllocLength; }
    
    static BOOL __stdcall _IsValidString(LPCWSTR lpwsz, int nLength = -1) 
	{
		return lpwsz == NULL ? FALSE : !IsBadStringPtrW(lpwsz, nLength); 
	}
    
    static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1) 
	{
		return (lpsz == NULL) ? FALSE : !IsBadStringPtrA(lpsz, nLength); 
	}
    
protected:
    // LPSTR m_pchData;   // pointer to ref counted KK_StringA data
    KK_StringDataU m_stStringDataU;
    
    // implementation helpers
    const KK_StringDataU* GetData() const
	{
		// assert(m_pchData != NULL);
		return &m_stStringDataU;
	}
    
    // void Init() { m_pchData = _GetEmptyString().m_pchData; }
    void Init(void)
    {
        m_stStringDataU.deleteData();
    }
    
    void AllocCopy(KK_StringU& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
    {
        // will clone the data attached to this KK_StringA
        // allocating 'nExtraLen' characters
        // Places results in uninitialized KK_StringA 'dest'
        // Will copy the part or all of original data to start of new KK_StringA
        
        int nNewLen = nCopyLen + nExtraLen;
        if (nNewLen == 0)
        {
            dest.Init();
        }
        else
        {
            if(dest.AllocBuffer(nNewLen)) memcpy(dest.m_stStringDataU.data(), GetData()->dataConst() + nCopyIndex, nCopyLen * sizeof(CHAR));
        }
    }
    
    
    BOOL AllocBuffer(int nLen)
    {
        // always allocate one extra character for '\0' termination
        // assumes [optimistically] that data length will equal allocation length
        
        assert(nLen >= 0);
        assert(nLen <= INT_MAX - 1);    // max size (enough room for 1 extra)
        
        /*
        if (nLen == 0)
        {
            Init();
        }
        else
        {
            
            KK_StringDataU* pData = NULL;
			//pData = (KK_StringDataU*)::HeapAlloc(::GetProcessHeap(), 0, sizeof(KK_StringDataU) + (nLen + 1) * sizeof(CHAR));
            pData = (KK_StringDataU*)new BYTE[sizeof(KK_StringDataU) + (nLen + 1) * sizeof(CHAR)];
            if(pData == NULL) return FALSE;
            
            pData->nRefs = 1;
            pData->data()[nLen] = '\0';
            pData->nDataLength = nLen;
            pData->nAllocLength = nLen;
            m_pchData = pData->data();
        }
        */
        if ( NULL != m_stStringDataU.resizeData(nLen + 1) )
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
                memcpy(m_stStringDataU.data(), lpszSrcData, nSrcLen * sizeof(CHAR));
                m_stStringDataU.nDataLength = nSrcLen;
            }
        }
    }
    
    
    BOOL ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data, int nSrc2Len, LPCSTR lpszSrc2Data)
    {
        // -- master concatenation routine
        // Concatenate two sources
        // -- assume that 'this' is a new KK_String object
        
        BOOL bRet = TRUE;
        int nNewLen = nSrc1Len + nSrc2Len;
        if ( nNewLen > 0 )
        {
            bRet = AllocBuffer(nNewLen);
            if ( bRet )
            {
                memcpy(m_stStringDataU.data(), lpszSrc1Data, nSrc1Len * sizeof(CHAR));
                memcpy(m_stStringDataU.data() + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(CHAR));
            }
        }
        return bRet;
    }
    
    void ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData)
    {
        /*
        //  -- the main routine for += operators
        
        // concatenating an empty KK_StringA is a no-op!
        if (nSrcLen == 0) return;
        
        // if the buffer is too small, or we have a width mis-match, just
        //   allocate a new buffer (slow but sure)
        if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
        {
            // we have to grow the buffer, use the ConcatCopy routine
            KK_StringDataU* pOldData = GetData();
            if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
            {
                assert(pOldData != NULL);
                KK_StringU::Release(pOldData);
            }
        }
        else
        {
            // fast concatenation when buffer big enough
            memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(CHAR));
            GetData()->nDataLength += nSrcLen;
            assert(GetData()->nDataLength <= GetData()->nAllocLength);
            m_pchData[GetData()->nDataLength] = '\0';
        }
        */
        //  -- the main routine for += operators
        
        if ( (nSrcLen <= 0) || (NULL == lpszSrcData) )
        {
            return;
        }
        
        // if the buffer is too small, or we have a width mis-match, just
        //   allocate a new buffer (slow but sure)
        if ( GetData()->nDataLength + nSrcLen > GetData()->nAllocLength )
        {
            // we have to grow the buffer, use the ConcatCopy routine
            ConcatCopy(GetData()->nDataLength, GetData()->dataConst(), nSrcLen, lpszSrcData);
        }
        else
        {
            // fast concatenation when buffer big enough
            memcpy(m_stStringDataU.data() + m_stStringDataU.nDataLength, lpszSrcData, nSrcLen * sizeof(CHAR));
            m_stStringDataU.nDataLength += nSrcLen;
            assert(m_stStringDataU.nDataLength <= m_stStringDataU.nAllocLength);
            (m_stStringDataU.data())[m_stStringDataU.nDataLength] = '\0';
        }
    }
    
    void CopyBeforeWrite()
    {
        /*
        if (GetData()->nRefs > 1)
        {
            KK_StringDataU* pData = GetData();
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
        //        if (GetData() != _atltmpDataNilU)
        if (!GetData()->IsNullString())
        {
            assert(GetData()->nRefs != 0);
            if (InterlockedDecrement(&GetData()->nRefs) <= 0) //::HeapFree(::GetProcessHeap(), 0, GetData());
                delete[] (BYTE*)GetData();
            Init();
        }
    }
    
    static void PASCAL Release(KK_StringDataU* pData)
    {
        //        if (pData != _atltmpDataNilU)
        if (!pData->IsNullString())
        {
            assert(pData->nRefs != 0);
            if (InterlockedDecrement(&pData->nRefs) <= 0) //::HeapFree(::GetProcessHeap(), 0, pData);
			    delete[] (BYTE*)pData;
        }
    }
    */
    
    static int PASCAL SafeStrlen(LPCSTR lpsz) { return (lpsz == NULL) ? 0 : strlen(lpsz); }
    
    // static const KK_StringU& __stdcall _GetEmptyString() { return *(KK_StringU*)&_atltmpPchNilU; }
    
    // KK_StringU conversion helpers
    static int __cdecl _wcstombsz(char* mbstr, const WCHAR* wcstr, size_t count)
    {
        if (count == 0 && mbstr != NULL) return 0;
        
        //int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
		int result = wcstombs(mbstr, wcstr, count);
        assert(mbstr == NULL || result <= (int)count);
        if (result > 0) mbstr[result] = 0;
        return result;
    }
    
	static int __cdecl _wcstoutf8z(char* utf8str, const WCHAR* wcstr, size_t count)
	{
        if (count == 0 && wcstr != NULL) return 0;
		if (utf8str == NULL) return 0;
		
		int len = wcslen(wcstr);
		unsigned char* utf8 = (unsigned char*)utf8str;
		int i, result;
		for (i=0, result=0; i<len; i++)
		{
			if (wcstr[i] < 0x80)
			{
				utf8[result] = (unsigned char) wcstr[i];
				result++;
				if (result >= (int)count-1) break;
			}
			else if (wcstr[i] < 0x800)
			{
				utf8[result] = 192 + (wcstr[i]>>6);
				result++;
				if (result >= (int)count-1) break;
				utf8[result] = 128 + (wcstr[i]&0x3F);
				result++;
				if (result >= (int)count-1) break;
			}
			else if (wcstr[i] < 0x10000)
			{
				utf8[result] = 224 + (wcstr[i]>>12);
				result++;
				if (result >= (int)count-1) break;
				utf8[result] = 128 + ((wcstr[i]&0xFFF)>>6);
				result++;
				if (result >= (int)count-1) break;
				utf8[result] = 128 + (wcstr[i]&0x3F);
				result++;
				if (result >= (int)count-1) break;
			}
		}
        assert(result < (int)count);
        if (result > 0) utf8[result] = 0;
		return result+1;
	}
    
    /*
     U-00000000 - U-0000007F: 0xxxxxxx  
     U-00000080 - U-000007FF: 110xxxxx 10xxxxxx  
     U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx  
     U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
     U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
     U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
     */
    static int __cdecl _getutf8len(const WCHAR* wcstr)
    {
		int len = wcslen(wcstr);
		int i, result;
		for (i=0, result=0; i<len; i++)
		{
			if (wcstr[i] < 0x00000080)
				result++;
			else if (wcstr[i] < 0x00000800)
				result+=2;
			else if (wcstr[i] < 0x00010000)
				result+=3;
			/*else if (wcstr[i] < 0x00200000)//++
             result+=4;
             else if (wcstr[i] < 0x04000000)//++
             result+=5;*/
		}
        
        return result;
    }
    
    static int __cdecl _mbstowcsz(WCHAR* wcstr, const char* mbstr, size_t count)
    {
        if (count == 0 && wcstr != NULL) return 0;
        
        //int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
		int result = mbstowcs(wcstr, mbstr, count);
        assert(wcstr == NULL || result <= (int)count);
        if (result > 0) wcstr[result] = 0;
        return result;
    }
};

// Compare helpers
inline bool __stdcall operator==(const KK_StringU& s1, const KK_StringU& s2) { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(const KK_StringU& s1, LPCSTR s2) { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(LPCSTR s1, const KK_StringU& s2) { return s2.Compare(s1) == 0; }
inline bool __stdcall operator!=(const KK_StringU& s1, const KK_StringU& s2) { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(const KK_StringU& s1, LPCSTR s2) { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(LPCSTR s1, const KK_StringU& s2) { return s2.Compare(s1) != 0; }
inline bool __stdcall operator<(const KK_StringU& s1, const KK_StringU& s2) { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(const KK_StringU& s1, LPCSTR s2) { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(LPCSTR s1, const KK_StringU& s2) { return s2.Compare(s1) > 0; }
inline bool __stdcall operator>(const KK_StringU& s1, const KK_StringU& s2) { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(const KK_StringU& s1, LPCSTR s2) { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(LPCSTR s1, const KK_StringU& s2) { return s2.Compare(s1) < 0; }
inline bool __stdcall operator<=(const KK_StringU& s1, const KK_StringU& s2) { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(const KK_StringU& s1, LPCSTR s2) { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(LPCSTR s1, const KK_StringU& s2) { return s2.Compare(s1) >= 0; }
inline bool __stdcall operator>=(const KK_StringU& s1, const KK_StringU& s2) { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(const KK_StringU& s1, LPCSTR s2) { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(LPCSTR s1, const KK_StringU& s2) { return s2.Compare(s1) <= 0; }

// friend functions
inline KK_StringU __stdcall operator+(const KK_StringU& string1, const KK_StringU& string2)
{
    KK_StringU s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), string2.GetData()->nDataLength, string2.GetData()->dataConst());
    return s;
}

inline KK_StringU __stdcall operator+(const KK_StringU& string1, char ch)
{
    KK_StringU s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), 1, &ch);
    return s;
}

inline KK_StringU __stdcall operator+(const KK_StringU& string1, WCHAR wch)
{
    KK_StringU s;
    KK_StringU ch((LPCWSTR)&wch, 1);
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), 1, ch);
    return s;
}

inline KK_StringU __stdcall operator+(WCHAR wch, const KK_StringU& string1)
{
    KK_StringU s;
    KK_StringU ch((LPCWSTR)&wch, 1);
    s.ConcatCopy(1, ch, string1.GetData()->nDataLength, string1.GetData()->dataConst());
    return s;
}

inline KK_StringU __stdcall operator+(char ch, const KK_StringU& string1)
{
    KK_StringU s;
    s.ConcatCopy(1, &ch, string1.GetData()->nDataLength, string1.GetData()->dataConst());
    return s;
}


#endif  // __KK_STRING_U_H__
