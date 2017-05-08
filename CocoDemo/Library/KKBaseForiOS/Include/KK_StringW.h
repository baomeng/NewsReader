//
//  KK_StringW.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

//////////////////////////////////////////////////////////////////////
// include guards
//////////////////////////////////////////////////////////////////////

#ifndef __KK_STRING_W_H__
#define __KK_STRING_W_H__

#include "KK_Includes.h"
#include "KK_BaseFunctions.h"

class KK_StringA;
class KK_StringW;
class KK_StringU;

const long NULLSTRING_W = -10001;

//////////////////////////////////////////////////////////////////////
// additional defines
//////////////////////////////////////////////////////////////////////
// 注释［2015.09.07］：考虑到32Bit和64Bit的兼容性，修改KK_StringDataW数据结构的定义
// 注释［2015.09.07］：废弃字符串内存共享模式
struct KK_StringDataW
{
    /*
    long    nRefs;          // reference count
    */
    long    nDataLength;
    long    nAllocLength;
    WCHAR*  ptrByte;
    
    KK_StringDataW(void) : // nRefs(NULLSTRING_A),
                           nDataLength(0),
                           nAllocLength(0),
                           ptrByte(NULL)
    {
        // Do Nothing
    }
    
    ~KK_StringDataW(void)
    {
        deleteData();
    }
    
    WCHAR* newData(long lByteLen)
    {
        deleteData();
        
        if ( lByteLen > 0 )
        {
            ptrByte = new WCHAR[lByteLen + 1];
            if ( NULL != ptrByte )
            {
                memset(ptrByte, 0, sizeof(WCHAR) * (lByteLen + 1));
                // nRefs     = 1;
                nDataLength  = 0;
                nAllocLength = lByteLen;
            }
        }
        
        return ptrByte;
    }
    
    WCHAR* resizeData(long lByteLen)
    {
        if ( lByteLen > 0 )
        {
            if ( NULL != ptrByte )
            {
                if ( lByteLen > nAllocLength )
                {
                    WCHAR* pOldtrByte = ptrByte;
                    
                    ptrByte = new WCHAR[lByteLen + 1];
                    if ( NULL != ptrByte )
                    {
                        memset(ptrByte, 0, sizeof(WCHAR) * (lByteLen + 1));
                        memmove(ptrByte, pOldtrByte, sizeof(WCHAR) * nDataLength);
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
                    ptrByte[lByteLen - 1] = L'\0';
                    if ( lByteLen <= nDataLength )
                    {
                        nDataLength = lByteLen - 1;
                    }
                }
            }
            else
            {
                ptrByte = new WCHAR[lByteLen + 1];
                if ( NULL != ptrByte )
                {
                    memset(ptrByte, 0, sizeof(WCHAR) * (lByteLen + 1));
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
        // nRefs     = NULLSTRING_W;
        nDataLength  = 0;
        nAllocLength = 0;
        if ( NULL != ptrByte )
        {
            delete [] ptrByte;
            ptrByte = NULL;
        }
    }
    
    WCHAR* data() { return ptrByte; }
    WCHAR* dataConst() const { return ptrByte; };
    // bool IsNullString() { return ( nRefs == NULLSTRING_W); }
};

KK_StringW __stdcall operator+(const KK_StringW& string1, const KK_StringW& string2);
KK_StringW __stdcall operator+(const KK_StringW& KK_StringA, char ch);
KK_StringW __stdcall operator+(const KK_StringW& KK_StringA, wchar_t ch);
KK_StringW __stdcall operator+(wchar_t ch, const KK_StringW& KK_StringA);
KK_StringW __stdcall operator+(char ch, const KK_StringW& KK_StringA);

/*
static long rgInitDataW[] = { NULLSTRING_W, 0, 0, 0 };
// static const wchar_t* _atltmpPchNilW = (const wchar_t*)(((BYTE*)&rgInitDataW) + sizeof(KK_StringDataW));
static KK_StringDataW* _atltmpPchNilW = (KK_StringDataW*)rgInitDataW;
*/

//////////////////////////////////////////////////////////////////////
// KK_StringW class
//////////////////////////////////////////////////////////////////////

class KK_StringW
{
public:
    // Constructors
    KK_StringW() { Init(); }
    
    KK_StringW(const KK_StringA& stringSrc);
    KK_StringW(const KK_StringU& stringSrc);
	KK_StringW(const KK_StringW& stringSrc)
    {
        /*
        assert(stringSrc.GetData()->nRefs != 0);
        if (stringSrc.GetData()->nRefs >= 0)
        {
            //            assert(stringSrc.GetData() != _atltmpDataNilW);
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
    
    KK_StringW(wchar_t ch, int nRepeat = 1)
    {
        Init();
        if ( nRepeat >= 1 && AllocBuffer(nRepeat) )
        {
            /*
            LPWSTR lpStart = m_stStringDataW.data();
			for (int i = 0; i < nRepeat; ++i)
            {
                lpStart[i] = ch;
            }
            */
            wmemset(m_stStringDataW.data(), ch, nRepeat);
        }
    }
    
    KK_StringW(LPCSTR lpsz)
    {
        Init();
        if (lpsz == 0)
        {
			assert(0);
        }
        else
        {
            int nSrcLen = (lpsz != 0) ? _getwcslen(lpsz) : 0;
            if (nSrcLen != 0 && AllocBuffer(nSrcLen))
            {
				//mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
				//mbrtowc(m_pchData, lpsz, nSrcLen + 1, NULL);
				//mbstowcs(m_pchData, lpsz, nSrcLen + 1);
				mbstowcs(m_stStringDataW.data(), lpsz, nSrcLen + 1);
                
				ReleaseBuffer();
            }
        }
    }
    
    KK_StringW(LPCSTR lpsz, int nLength)
    {
        Init();
        if (nLength != 0 && AllocBuffer(nLength))
        {
			//mbstowcsz(m_pchData, lpsz, nLength);
			//mbrtowc(m_pchData, lpsz, nLength, NULL);
			mbstowcs(m_stStringDataW.data(), lpsz, nLength);
			ReleaseBuffer();
        }
    }
    
    KK_StringW(const wchar_t* lpsz)
    {
        Init();
        int nSrcLen = (lpsz != NULL) ? wcslen(lpsz) : 0;
        if (nSrcLen != 0 && AllocBuffer(nSrcLen)) wmemcpy(m_stStringDataW.data(), lpsz, nSrcLen);//memcpy(m_pchData, lpsz, nSrcLen * sizeof(wchar_t));
    }
    
    KK_StringW(const wchar_t* lpch, int nLength)
    {
        Init();
        if (nLength != 0 && AllocBuffer(nLength))  wmemcpy(m_stStringDataW.data(), lpch, nLength);//memcpy(m_pchData, lpch, nLength * sizeof(wchar_t));
    }
    
    KK_StringW(const unsigned char* psz) 
	{ 
		Init();
		*this = (LPCSTR)psz; 
	}
    
    const KK_StringW& Assign(const wchar_t* lpszSrcData, int nSrcLen)
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
            *this = L"";
        
        assert(GetData()->nDataLength == 0);
        assert(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
        */
        
        if ( 0 == GetData()->nDataLength )
        {
            return;
        }
        
        *this = L"";
        
        assert( (0 == GetData()->nDataLength) && (0 == GetData()->nAllocLength) );
    }
    
    wchar_t GetAt(int nIndex) const       // 0 based
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        return (GetData()->dataConst())[nIndex];
    }
    
    wchar_t & operator[](int nIndex) const  // same as GetAt
    {
        // same as GetAt
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        return (GetData()->dataConst())[nIndex];
    }
    
    void SetAt(int nIndex, wchar_t ch)
    {
        assert(nIndex >= 0);
        assert(nIndex < GetData()->nDataLength);
        
        CopyBeforeWrite();
        (m_stStringDataW.data())[nIndex] = ch;
    }
    
    operator const wchar_t*() const { return GetData()->dataConst(); } // as a C KK_StringA
    
    
	const KK_StringW& operator=(const KK_StringA& stringSrc);
	const KK_StringW& operator=(const KK_StringU& stringSrc);	
	// overloaded assignment
    const KK_StringW& operator=(const KK_StringW& stringSrc)
    {
        /*
        if (m_pchData != stringSrc.m_pchData)
        {
            //            if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNilW) || stringSrc.GetData()->nRefs < 0)
            if ((GetData()->nRefs < 0 && !GetData()->IsNullString()) || stringSrc.GetData()->nRefs < 0)
            {
                // actual copy necessary since one of the strings is locked
                AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
            }
            else
            {
                // can just copy references around
                Release();
                //                assert(stringSrc.GetData() != _atltmpDataNilW);
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
    
    const KK_StringW& operator=(wchar_t ch)
    {
        AssignCopy(1, &ch);
        return *this;
    }
    
    const KK_StringW& operator=(LPCSTR lpsz)
    {
        int nSrcLen = (lpsz != NULL) ? _getwcslen(lpsz) : 0;
        if ( AllocBeforeWrite(nSrcLen) )
        {
            //_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);			
			//mbrtowc(m_pchData, lpsz, nSrcLen + 1, NULL);
            if ( (nSrcLen > 0) && (NULL != lpsz) )
            {
                mbstowcs(m_stStringDataW.data(), lpsz, nSrcLen + 1);
            }
            ReleaseBuffer();
        }
        
        return *this;
    }

    const KK_StringW& operator=(const wchar_t* lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        AssignCopy(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    
    const KK_StringW& operator=(const unsigned char* psz)
	{ 
		*this = (LPCSTR)psz;
		return *this; 
	}
    
    // KK_StringA concatenation
    const KK_StringW& operator+=(const wchar_t* lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        ConcatInPlace(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    
    const KK_StringW& operator+=(wchar_t ch)
    {
        ConcatInPlace(1, &ch);
        return *this;
    }
    
    const KK_StringW& operator+=(LPCSTR lpsz)
    {
        assert(lpsz == NULL || _IsValidString(lpsz));
        int nSrcLen = (lpsz != NULL) ? _getwcslen(lpsz) : 0;
        ConcatInPlace(nSrcLen, KK_StringW(lpsz));
        return *this;
    }
    
    // Concatenation
    // NOTE: "operator+" is done as friend functions for simplicity
    
    friend KK_StringW __stdcall operator+(const KK_StringW& string1, const KK_StringW& string2);
    friend KK_StringW __stdcall operator+(const KK_StringW& KK_StringA, char ch);
    friend KK_StringW __stdcall operator+(const KK_StringW& KK_StringA, wchar_t ch);
    friend KK_StringW __stdcall operator+(wchar_t ch, const KK_StringW& KK_StringA);
    friend KK_StringW __stdcall operator+(char ch, const KK_StringW& KK_StringA);
    
    // KK_StringA comparison
    int Compare(const wchar_t* lpsz) const {return wcscmp(GetData()->dataConst(), lpsz); }    // MBCS/Unicode aware
    
    int CompareNoCase(const wchar_t* lpsz) const { return wcscasecmp(GetData()->dataConst(), lpsz); }//{ return _wcsicmp(m_pchData, lpsz); }    // MBCS/Unicode aware // ignore case
    
    int Compare(const wchar_t* lpsz, size_t size) const { return wcsncmp(GetData()->dataConst(), lpsz, size); } //{ return wcsncmp(m_pchData, lpsz, size); }    // MBCS/Unicode aware
    
    int CompareNoCase(const wchar_t* lpsz, size_t size) const { return wcsncasecmp(GetData()->dataConst(), lpsz, size); }//{ return _wcsnicmp(m_pchData, lpsz, size); }    // MBCS/Unicode aware // ignore case
    
    // simple sub-KK_StringA extraction
    KK_StringW Mid(int nFirst, int nCount) const
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
        
        KK_StringW dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }
    
    KK_StringW Mid(int nFirst) const { return Mid(nFirst, GetData()->nDataLength - nFirst); }
    
    KK_StringW Left(int nCount) const
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
        
        KK_StringW dest;
        AllocCopy(dest, nCount, 0, 0);
        return dest;
    }
    
    KK_StringW Right(int nCount) const
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
        
        KK_StringW dest;
        AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
        return dest;
    }
    
    KK_StringW SpanIncluding(const wchar_t* lpszCharSet) const
    {
        // strspn equivalent
        assert(_IsValidString(lpszCharSet));
        return Left(wcsspn(GetData()->dataConst(), lpszCharSet));
        
    }
    
    KK_StringW SpanExcluding(const wchar_t* lpszCharSet) const
    {
        // strcspn equivalent
        assert(_IsValidString(lpszCharSet));
        return Left(wcscspn(GetData()->dataConst(), lpszCharSet));
    }
    
    // upper/lower/reverse conversion
    void MakeUpper()
    {
        CopyBeforeWrite();
        CharUpperW(m_stStringDataW.data());
		//CharUpperW(m_pchData);
    }
    
    void MakeLower()
    {
        CopyBeforeWrite();
        CharLowerW(m_stStringDataW.data());
		//CharLowerW(m_pchData);
    }
    
    void MakeReverse()
    {
        CopyBeforeWrite();
        wcsrev(m_stStringDataW.data());
    }
    
    // trimming whitespace (either side)
    void TrimRight()
    {
        CopyBeforeWrite();
        
        // find beginning of trailing spaces by starting at beginning (DBCS aware)
        wchar_t* lpsz     = m_stStringDataW.data();
        wchar_t* lpszLast = NULL;
        while (*lpsz != L'\0')
        {
            if (iswspace(*lpsz))
            {
                if (lpszLast == NULL) lpszLast = lpsz;
            }
            else
            {
                lpszLast = NULL;
            }
            lpsz = CharNextW(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            // truncate at trailing space start
            *lpszLast = L'\0';
            m_stStringDataW.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataW.data());
        }
    }
    
    void TrimLeft()
    {
        CopyBeforeWrite();
        
        // find first non-space character
        const wchar_t* lpsz = GetData()->dataConst();
        while (iswspace(*lpsz))
            lpsz = CharNextW(lpsz);
        
        // fix up data and length
        LPWSTR lpStart = m_stStringDataW.data();
        int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - lpStart);
        memmove(lpStart, lpsz, (nDataLength + 1) * sizeof(wchar_t));
        m_stStringDataW.nDataLength = nDataLength;
    }
    
    // remove continuous occurrences of chTarget starting from right
    void TrimRight(wchar_t chTarget)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)
        
        CopyBeforeWrite();
        wchar_t* lpsz     = m_stStringDataW.data();
        wchar_t* lpszLast = NULL;
        
        while (*lpsz != L'\0')
        {
            if (*lpsz == chTarget)
            {
                if (lpszLast == NULL) lpszLast = lpsz;
            }
            else
			{
				lpszLast = NULL;
			}
            lpsz = CharNextW(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = L'\0';
            m_stStringDataW.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataW.data());
        }
    }
    
    // remove continuous occcurrences of characters in passed KK_StringA,
    // starting from right
    void TrimRight(const wchar_t* lpszTargets)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)
        
        CopyBeforeWrite();
        wchar_t* lpsz     = m_stStringDataW.data();
        wchar_t* lpszLast = NULL;
        
        while (*lpsz != L'\0')
        {
            if (wcschr(lpszTargets, *lpsz) != NULL)
            {
                if (lpszLast == NULL) lpszLast = lpsz;
            }
            else
			{
				lpszLast = NULL;
			}
            lpsz = CharNextW(lpsz);
        }
        
        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = L'\0';
            m_stStringDataW.nDataLength = (int)(DWORD_PTR)(lpszLast - m_stStringDataW.data());
        }
    }
    
    
    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(wchar_t chTarget)
    {
        // find first non-matching character
        
        CopyBeforeWrite();
        const wchar_t* lpsz = GetData()->dataConst();
        
        while (chTarget == *lpsz)
            lpsz = CharNextW(lpsz);
        
        LPWSTR lpStart = m_stStringDataW.data();
        if (lpsz != lpStart)
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - lpStart);
            memmove(lpStart, lpsz, (nDataLength + 1) * sizeof(wchar_t));
            m_stStringDataW.nDataLength = nDataLength;
        }
    }
    
    // remove continuous occcurrences of characters in
    // passed KK_StringA, starting from left
    void TrimLeft(const wchar_t* lpszTargets)
    {
        // if we're not trimming anything, we're not doing any work
        if (SafeStrlen(lpszTargets) == 0) return;
        
        CopyBeforeWrite();
        const wchar_t* lpsz = GetData()->dataConst();
        
        while (*lpsz != L'\0')
        {
            if (wcschr(lpszTargets, *lpsz) == NULL) break;
            lpsz = CharNextW(lpsz);
        }
        
        LPWSTR lpStart = m_stStringDataW.data();
        if ( lpsz != lpStart )
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - lpStart);
            memmove(lpStart, lpsz, (nDataLength + 1) * sizeof(wchar_t));
            m_stStringDataW.nDataLength = nDataLength;
        }
    }
    
    // advanced manipulation
    // replace occurrences of chOld with chNew
    int Replace(wchar_t chOld, wchar_t chNew)
    {
        int nCount = 0;
        
        // short-circuit the nop case
        if (chOld != chNew)
        {
            // otherwise modify each character that matches in the KK_StringA
            CopyBeforeWrite();
            wchar_t* psz    = m_stStringDataW.data();
            wchar_t* pszEnd = psz + GetData()->nDataLength;
            while (psz < pszEnd)
            {
                // replace instances of the specified character only
                if (*psz == chOld)
                {
                    *psz = chNew;
                    nCount++;
                }
                psz = CharNextW(psz);
            }
        }
        return nCount;
    }
    
    // replace occurrences of substring lpszOld with lpszNew;
    // empty lpszNew removes instances of lpszOld
    int Replace(const wchar_t* lpszOld, const wchar_t* lpszNew)
    {
        // can't have empty or NULL lpszOld
        
        int nSourceLen = SafeStrlen(lpszOld);
        if (nSourceLen == 0) return 0;
        int nReplacementLen = SafeStrlen(lpszNew);
        
        // loop once to figure out the size of the result KK_StringA
        int nCount = 0;
        wchar_t* lpszStart = m_stStringDataW.data();
        wchar_t* lpszEnd   = lpszStart + GetData()->nDataLength;
        wchar_t* lpszTarget;
        while (lpszStart < lpszEnd)
        {
            while ((lpszTarget = wcsstr(lpszStart, lpszOld)) != NULL)
            {
                nCount++;
                lpszStart = lpszTarget + nSourceLen;
            }
            lpszStart += wcslen(lpszStart) + 1;
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
            if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
            {
                KK_StringDataW* pOldData = GetData();
                wchar_t* pstr = m_pchData;
                if(!AllocBuffer(nNewLength)) return -1;
                memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(wchar_t));
                Release(pOldData);
            }
            */
            m_stStringDataW.resizeData(nNewLength + 1);
            
            // else, we just do it in-place
            lpszStart = m_stStringDataW.data();
            lpszEnd   = lpszStart + GetData()->nDataLength;
            
            // loop again to actually do the work
            while (lpszStart < lpszEnd)
            {
                while ( (lpszTarget = wcsstr(lpszStart, lpszOld)) != NULL)
                {
                    int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_stStringDataW.data()) + nSourceLen);
                    memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(wchar_t));
                    memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(wchar_t));
                    lpszStart = lpszTarget + nReplacementLen;
                    lpszStart[nBalance] = L'\0';
                    nOldLength += (nReplacementLen - nSourceLen);
                }
                lpszStart += wcslen(lpszStart) + 1;
            }
            
            m_stStringDataW.nDataLength = nNewLength;
        }
        
        return nCount;
    }
    
    // replace nCount characters starting at zero-based index with lpszNew;
    int Replace(int nIndex, int nCount, const wchar_t* lpszNew)
    { // 可以优化
		Delete(nIndex, nCount);
		return Insert(nIndex, lpszNew);
    }
    
    // replace nCount characters starting at zero-based index with chNew;
    int Replace(int nIndex, int nCount, wchar_t chNew)
    { // 可以优化
		Delete(nIndex, nCount);
		return Insert(nIndex, chNew);
    }
    
    // remove occurrences of chRemove
    int Remove(wchar_t chRemove)
    {
        CopyBeforeWrite();
        
        wchar_t* pstrSource = m_stStringDataW.data();
        wchar_t* pstrDest   = pstrSource;
        wchar_t* pstrEnd    = pstrSource + GetData()->nDataLength;
        
        while (pstrSource < pstrEnd)
        {
            if (*pstrSource != chRemove)
            {
                *pstrDest = *pstrSource;
                pstrDest = CharNextW(pstrDest);
            }
            pstrSource = CharNextW(pstrSource);
        }
        *pstrDest = L'\0';
        int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
        m_stStringDataW.nDataLength -= nCount;
        
        return nCount;
    }
    
    // insert character at zero-based index; concatenates
    // if index is past end of KK_StringA
    int Insert(int nIndex, wchar_t ch)
    {
        CopyBeforeWrite();
        
        if (nIndex < 0) nIndex = 0;
        
        int nNewLength = GetData()->nDataLength;
        if (nIndex > nNewLength) nIndex = nNewLength;
        nNewLength++;
        /*
        if (GetData()->nAllocLength < nNewLength)
        {
            KK_StringDataW* pOldData = GetData();
            wchar_t* pstr = m_pchData;
            if(!AllocBuffer(nNewLength)) return -1;
            memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(wchar_t));
            Release(pOldData);
        }
        */
        m_stStringDataW.resizeData(nNewLength);
        
        // move existing bytes down
        LPWSTR lpStart = m_stStringDataW.data();
        memmove(lpStart + nIndex + 1, lpStart + nIndex, (nNewLength - nIndex) * sizeof(wchar_t));
        lpStart[nIndex] = ch;
        m_stStringDataW.nDataLength = nNewLength;
        
        return nNewLength;
    }
    
    // insert substring at zero-based index; concatenates
    // if index is past end of KK_StringA
    int Insert(int nIndex, const wchar_t* pstr)
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
                KK_StringDataW* pOldData = GetData();
                wchar_t* pstr = m_pchData;
                if(!AllocBuffer(nNewLength)) return -1;
                memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(wchar_t));
                Release(pOldData);
            }
            */
            
            m_stStringDataW.resizeData(nNewLength + 1);
            
            // move existing bytes down
            LPWSTR lpStart = m_stStringDataW.data();
            memmove(lpStart + nIndex + nInsertLength, lpStart + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(wchar_t));
            memcpy(lpStart + nIndex, pstr, nInsertLength * sizeof(wchar_t));
            m_stStringDataW.nDataLength = nNewLength;
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
        
        LPWSTR lpStart = m_stStringDataW.data();
		if (nCount == -1 && nIndex < nNewLength)
		{
            CopyBeforeWrite();
			lpStart[nIndex] = 0;
            m_stStringDataW.nDataLength = nIndex;
			nCount = 0;
			nNewLength = nIndex;
		}
        else if (nCount > 0 && nIndex < nNewLength)
        {
            CopyBeforeWrite();
            int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
            
            memmove(lpStart + nIndex, lpStart + nIndex + nCount, nBytesToCopy * sizeof(wchar_t));
            m_stStringDataW.nDataLength = nNewLength - nCount;
        }
        
        return nNewLength - nCount;
    }
    
	int Erase(int nIndex, int nCount = -1) { return Delete(nIndex, nCount); }
    
    // searching (return starting index, or -1 if not found)
    // look for a single character match
    int Find(wchar_t ch) const { return Find(ch, 0); }    // like "C" strchr
    
    
    int ReverseFind(wchar_t ch) const
    {
        // find last single character
        wchar_t* lpsz = wcsrchr(GetData()->dataConst(), ch);
        
        // return -1 if not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - GetData()->dataConst());
    }
    
    int Find(wchar_t ch, int nStart) const          // starting at index
    {
        int nLength = GetData()->nDataLength;
        if (nStart >= nLength || nStart < 0) return -1;//if (nStart >= nLength) return -1;
        
        // find first single character
        wchar_t* lpsz = wcschr(GetData()->dataConst() + nStart, ch);
        
        // return -1 if not found and index otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - GetData()->dataConst());
    }
    
    int FindOneOf(const wchar_t* lpszCharSet) const
    {
        assert(_IsValidString(lpszCharSet));
        wchar_t* lpsz = wcspbrk(GetData()->dataConst(), lpszCharSet);
        return (lpsz == NULL) ? -1 : (int)(lpsz - GetData()->dataConst());
    }
    
    int FindOneNotOf(const wchar_t* lpszCharSet) const
    {
        assert(_IsValidString(lpszCharSet));
        int size = wcsspn(GetData()->dataConst(), lpszCharSet);
		return (size >= GetData()->nDataLength) ? -1 : size;
    }
    
    // look for a specific sub-KK_StringA
    int Find(const wchar_t* lpszSub) const { return Find(lpszSub, 0); }    // like "C" strstr
    
    
    int Find(const wchar_t* lpszSub, int nStart) const  // starting at index
    {
        assert(_IsValidString(lpszSub));
        
        int nLength = GetData()->nDataLength;
        if (nStart >= nLength || nStart < 0) return -1;//if (nStart > nLength) return -1;
        
        // find first matching substring
		
        wchar_t* lpsz = wcsstr(GetData()->dataConst() + nStart, lpszSub);
        
        // return -1 for not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - GetData()->dataConst());
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(char n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ltow(n, szBuffer, radix);
		//swprintf(szBuffer, 40, L"%d", n);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(short n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ltow(n, szBuffer, radix);
        //swprintf(szBuffer, 40, L"%d", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(int n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ltow(n, szBuffer, radix);
        //swprintf(szBuffer, 40, L"%d", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(long n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ltow(n, szBuffer, radix);
        //swprintf(szBuffer, 40, L"%l", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(__int64 n, int radix = 10)
    {
        wchar_t szBuffer[80];
        _i64tow(n, szBuffer, radix);
        //swprintf(szBuffer, 80, L"%lld", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(unsigned char n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ultow(n, szBuffer, radix);
        //swprintf(szBuffer, 40, L"%ud", n);		
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(unsigned short n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ultow(n, szBuffer, radix);
        //swprintf(szBuffer, 40, L"%ud", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(unsigned int n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ultow(n, szBuffer, radix);
		//swprintf(szBuffer, 40, L"%ud", n);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(unsigned long n, int radix = 10)
    {
        wchar_t szBuffer[40];
        _ultow(n, szBuffer, radix);
        //swprintf(szBuffer, 40, L"%ul", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(unsigned __int64 n, int radix = 10)
    {
        wchar_t szBuffer[80];
        _ui64tow(n, szBuffer, radix);
        //swprintf(szBuffer, 80, L"%ulld", n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }
    
    // Concatentation for non strings
    const KK_StringW& Append(double n, int digits = 20)
    {
		char szBuffer[80];
		wchar_t szBufferW[80];
		gcvt(n, digits, szBuffer);
		_mbstowcsz(szBufferW, szBuffer, 80);
		ConcatInPlace(SafeStrlen(szBufferW), szBufferW);
        
		return *this;
    }
    
	BOOL FormatV(LPCWSTR lpszFormat, va_list argList)
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
	    for (LPCWSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = ::CharNextW(lpsz))
	    {
		    // handle '%' character, but watch out for '%%'
		    if (*lpsz != '%' || *(lpsz = ::CharNextW(lpsz)) == '%')
		    {
			    // this is instead of _tclen()
			    nMaxLen++;
			    continue;
		    }
            
		    int nItemLen = 0;
            
		    // handle '%' character with format
		    int nWidth = 0;
		    for (; *lpsz != '\0'; lpsz = ::CharNextW(lpsz))
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
			    nWidth = wtoi(lpsz);
			    for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = ::CharNextW(lpsz))
				    ;
		    }
		    assert(nWidth >= 0);
            
		    int nPrecision = 0;
		    if (*lpsz == '.')
		    {
			    // skip past '.' separator (width.precision)
			    lpsz = ::CharNextW(lpsz);
                
			    // get precision and skip it
			    if (*lpsz == '*')
			    {
				    nPrecision = va_arg(argList, int);
				    lpsz = ::CharNextW(lpsz);
			    }
			    else
			    {
				    nPrecision = wtoi(lpsz);
				    for (; *lpsz != '\0' && iswdigit(*lpsz); lpsz = ::CharNextW(lpsz))
					    ;
			    }
			    assert(nPrecision >= 0);
		    }
            
		    // should be on type modifier or specifier
		    int nModifier = 0;
		    if(lpsz[0] == L'I' && lpsz[1] == L'6' && lpsz[2] == L'4')
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
                        lpsz = ::CharNextW(lpsz);
                        break;
                    case 'l':
                        nModifier = FORCE_UNICODE;
                        lpsz = ::CharNextW(lpsz);
                        break;
                        
                        // modifiers that do not affect size
                    case 'F':
                    case 'N':
                    case 'L':
                        lpsz = ::CharNextW(lpsz);
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
                    LPCWSTR pstrNextArg = va_arg(argList, LPCWSTR);
                    if (pstrNextArg == NULL)
                    {
                        nItemLen = 6;  // "(null)"
                    }
                    else
                    {
                        nItemLen = lstrlenW(pstrNextArg);
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
                        nItemLen = lstrlenA(pstrNextArg);
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
                        nItemLen = lstrlenA(pstrNextArg);
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
					    LPWSTR pszTemp;
                        
					    // 312 == strlen("-1+(309 zeroes).")
					    // 309 zeroes == max precision of a double
					    // 6 == adjustment in case precision is not specified,
					    //   which means that the precision defaults to 6
					    pszTemp = (LPWSTR)_alloca(Max(nWidth, 312 + nPrecision + 6));
                        
					    f = va_arg(argList, double);
					    //swprintf(pszTemp, L"%*.*f", nWidth, nPrecision + 6, f);
						swprintf(pszTemp, nWidth, L"%*.*f", nPrecision + 6, f);
					    nItemLen = wcslen(pszTemp);
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
		int nRet = vswprintf(m_stStringDataW.data(), nMaxLen, lpszFormat, argListSave);
	    //int nRet = vswprintf(m_pchData, lpszFormat, argListSave);
	    //nRet;	// ref
	    assert(nRet <= GetAllocLength());
	    ReleaseBuffer();
        
	    va_end(argListSave);
	    return TRUE;
    }
    
    // formatting for localization (uses FormatMessage API)
    BOOL __cdecl Format(const wchar_t* lpszFormat, ...)
    {
	    assert(_IsValidString(lpszFormat));
        
	    va_list argList;
	    va_start(argList, lpszFormat);
	    BOOL bRet = FormatV(lpszFormat, argList);
	    va_end(argList);
	    return bRet;
    }
    
    // Access to KK_StringA implementation buffer as "C" character array
    wchar_t* GetBuffer(int nMinBufLength)
    {
        assert(nMinBufLength >= 0);
        /*
        if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
        {
            // we have to grow the buffer
            KK_StringDataW* pOldData = GetData();
            int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
            if (nMinBufLength < nOldLen) nMinBufLength = nOldLen;
            
            if(!AllocBuffer(nMinBufLength)) return NULL;
            
            memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(wchar_t));
            GetData()->nDataLength = nOldLen;
            Release(pOldData);
        }
        assert(GetData()->nRefs <= 1);
        
        // return a pointer to the character storage for this KK_StringA
        assert(m_pchData != NULL);
        return m_pchData;
        */
        m_stStringDataW.resizeData(nMinBufLength + 1);
        return m_stStringDataW.data();
    }
    
    void ReleaseBuffer(int nNewLength = -1)
    {
        // CopyBeforeWrite();  // just in case GetBuffer was not called
        
        LPWSTR lpStart = m_stStringDataW.data();
        if ( NULL != lpStart )
        {
            if (nNewLength == -1) nNewLength = wcslen(lpStart); // zero terminated
            
            assert(nNewLength <= GetData()->nAllocLength);
            m_stStringDataW.nDataLength = nNewLength;
            lpStart[nNewLength] = L'\0';
        }
    }
    
    wchar_t* GetBufferSetLength(int nNewLength)
    {
        assert(nNewLength >= 0);
        
        if(GetBuffer(nNewLength) == NULL) return NULL;
        
        LPWSTR lpStart = m_stStringDataW.data();
        
        m_stStringDataW.nDataLength = nNewLength;
        lpStart[nNewLength]         = L'\0';
        
        return lpStart;
    }
    
    void FreeExtra()
    {
        assert(GetData()->nDataLength <= GetData()->nAllocLength);
        if ( GetData()->nDataLength != GetData()->nAllocLength )
        {
            /*
            KK_StringDataW* pOldData = GetData();
            if(AllocBuffer(GetData()->nDataLength))
            {
                memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(wchar_t));
                assert(m_pchData[GetData()->nDataLength] == L'\0');
                KK_StringW::Release(pOldData);
            }
            */
            m_stStringDataW.resizeData(GetData()->nDataLength + 1);
        }
        // assert(GetData() != NULL);
    }
    
    /*
    // Use LockBuffer/UnlockBuffer to turn refcounting off
    wchar_t* LockBuffer()
    {
        wchar_t* lpsz = GetBuffer(0);
        if(lpsz != NULL) GetData()->nRefs = -1;
        return lpsz;
    }
    
    void UnlockBuffer()
    {
        assert(GetData()->nRefs == -1);
        //        if (GetData() != _atltmpDataNilW)
        if (!GetData()->IsNullString()) GetData()->nRefs = 1;
    }
    */
    
	const KK_StringW& SetUtf8String(LPCSTR lpsz)
	{
        int nSrcLen = (lpsz != NULL) ? _getwcslenofutf8(lpsz) : 0;
		if ( AllocBeforeWrite(nSrcLen) )
        {
            if ( (nSrcLen > 0) && (NULL != lpsz) )
            {
                _utf8towcsz(m_stStringDataW.data(), lpsz, nSrcLen + 1);
            }
            ReleaseBuffer();
        }
        return *this;
	}
    
    // Implementation
public:
    ~KK_StringW()
    {
        // free any attached data
        //        if (GetData() != _atltmpDataNilW)
        /*
        if (!GetData()->IsNullString())
        {
            if (InterlockedDecrement(&GetData()->nRefs) <= 0) //::HeapFree(::GetProcessHeap(), 0, GetData());
			    delete[] (BYTE*)GetData();
        }
        */
        m_stStringDataW.deleteData();
    }
    
    int GetAllocLength() const { return GetData()->nAllocLength; }
    
    static BOOL __stdcall _IsValidString(const wchar_t* lpsz, int nLength = -1)
    {
        if(lpsz == NULL) return FALSE;
        return !::IsBadStringPtrW(lpsz, nLength);
    }
    
    static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)
    {
        if(lpsz == NULL) return FALSE;
        return !::IsBadStringPtrA(lpsz, nLength);
    }
    
protected:
    // wchar_t* m_pchData;   // pointer to ref counted KK_StringA data
    KK_StringDataW m_stStringDataW;
    
    // implementation helpers
    const KK_StringDataW* GetData() const
	{ 
		// assert(m_pchData != NULL);
        return &m_stStringDataW;
	}
    
    void Init()
    {
        m_stStringDataW.deleteData();
    }
    
    void AllocCopy(KK_StringW& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
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
            if(dest.AllocBuffer(nNewLen)) memcpy(dest.m_stStringDataW.data(), GetData()->dataConst() + nCopyIndex, nCopyLen * sizeof(wchar_t));
        }
    }
    
    
    BOOL AllocBuffer(int nLen)
    {
        // always allocate one extra character for L'\0' termination
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
            KK_StringDataW* pData = NULL;
			//pData = (KK_StringDataW*)::HeapAlloc(::GetProcessHeap(), 0, sizeof(KK_StringDataW) + (nLen + 1) * sizeof(wchar_t));
            pData = (KK_StringDataW*)new BYTE[sizeof(KK_StringDataW) + (nLen + 1) * sizeof(wchar_t)];
            if(pData == NULL) return FALSE;
            
            pData->nRefs = 1;
            pData->data()[nLen] = L'\0';
            pData->nDataLength = nLen;
            pData->nAllocLength = nLen;
            m_pchData = pData->data();
        }
        
        return TRUE;
        */
        
        if ( NULL != m_stStringDataW.resizeData(nLen + 1) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    
    void AssignCopy(int nSrcLen, const wchar_t* lpszSrcData)
    {
        if ( AllocBeforeWrite(nSrcLen) )
        {
            if ( (nSrcLen > 0) && (NULL != lpszSrcData) )
            {
                wmemcpy(m_stStringDataW.data(), lpszSrcData, nSrcLen);
                m_stStringDataW.nDataLength = nSrcLen;
            }
        }
    }
    
    
    BOOL ConcatCopy(int nSrc1Len, const wchar_t* lpszSrc1Data, int nSrc2Len, const wchar_t* lpszSrc2Data)
    {
        // -- master concatenation routine
        // Concatenate two sources
        // -- assume that 'this' is a new KK_String object
        
        BOOL bRet = TRUE;
        int nNewLen = nSrc1Len + nSrc2Len;
        if (nNewLen != 0)
        {
            bRet = AllocBuffer(nNewLen);
            if (bRet)
            {
                wmemcpy(m_stStringDataW.data(), lpszSrc1Data, nSrc1Len);
                wmemcpy(m_stStringDataW.data() + nSrc1Len, lpszSrc2Data, nSrc2Len);
            }
        }
        return bRet;
    }
    
    void ConcatInPlace(int nSrcLen, const wchar_t* lpszSrcData)
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
            KK_StringDataW* pOldData = GetData();
            if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
            {
                assert(pOldData != NULL);
                KK_StringW::Release(pOldData);
            }
        }
        else
        {
            // fast concatenation when buffer big enough
            memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(wchar_t));
            GetData()->nDataLength += nSrcLen;
            assert(GetData()->nDataLength <= GetData()->nAllocLength);
            m_pchData[GetData()->nDataLength] = L'\0';
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
            // fast concatenation when buffer big enough
            LPWSTR lpStart = m_stStringDataW.data();
            memcpy(lpStart + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(wchar_t));
            m_stStringDataW.nDataLength += nSrcLen;
            assert(GetData()->nDataLength <= GetData()->nAllocLength);
            lpStart[GetData()->nDataLength] = L'\0';
        }
    }
    
    void CopyBeforeWrite()
    {
        /*
        if (GetData()->nRefs > 1)
        {
            KK_StringDataW* pData = GetData();
            Release();
            if(AllocBuffer(pData->nDataLength)) memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(wchar_t));
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
        //        if (GetData() != _atltmpDataNilW)
        if (!GetData()->IsNullString())
        {
            assert(GetData()->nRefs != 0);
            if (InterlockedDecrement(&GetData()->nRefs) <= 0) //::HeapFree(::GetProcessHeap(), 0, GetData());
			    delete[] (BYTE*)GetData();
            Init();
        }
    }
    
    static void PASCAL Release(KK_StringDataW* pData)
    {
        //        if (pData != _atltmpDataNilW)
        if (!pData->IsNullString())
        {
            assert(pData->nRefs != 0);
            if (InterlockedDecrement(&pData->nRefs) <= 0) //::HeapFree(::GetProcessHeap(), 0, pData);
			    delete[] (BYTE*)pData;
        }
    }
    */
    
    static int PASCAL SafeStrlen(const wchar_t* lpsz) { return (lpsz == NULL) ? 0 : wcslen(lpsz); }
    
    // static const KK_StringW& __stdcall _GetEmptyString() { return *(KK_StringW*)&_atltmpPchNilW; }
    
    // KK_StringW conversion helpers
    static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
    {
        if (count == 0 && mbstr != NULL) return 0;
        
        //int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
		//int result = wcrtomb(mbstr, wcstr, NULL);		
		//int result = wcrtomb(mbstr, wcstr[0], NULL);
		int result = wcstombs(mbstr, wcstr, count);
        assert(mbstr == NULL || result <= (int)count);
        if (result > 0) mbstr[result - 1] = 0;
        return result;
    }
    
    static int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
    {
        if (count == 0 && wcstr != NULL) return 0;
        
        //int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
		//int result = mbrtowc(wcstr, mbstr, count, NULL);
		int result = mbstowcs(wcstr, mbstr, count);
        assert(wcstr == NULL || result <= (int)count);
        if (result > 0) wcstr[result - 1] = 0;
        return result;
    }
    
    static int __cdecl _getwcslen(const char* mbstr)
    {
        //int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, NULL, 0);
		//int result = mbrtowc(NULL, mbstr, 0, NULL);
		int result = mbstowcs(NULL, mbstr, 0);
        return result;
    }
    
    /*
     U-00000000 - U-0000007F: 0xxxxxxx  
     U-00000080 - U-000007FF: 110xxxxx 10xxxxxx  
     U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx  
     U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
     U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
     U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
     */
    
	static int __cdecl _utf8towcsz(wchar_t* wcstr, const char* utf8str, size_t count)
    {
        if (count == 0 && wcstr != NULL) return 0;
		if (wcstr == NULL) return 0;
		
		int len = strlen(utf8str);
		int i=0, result=0;
		for (i=0, result=0; i<len; i++)
		{
			if (!(utf8str[i]&128))
			{
				// < 0x80 1bytes
				wcstr[result] = utf8str[i];
				result++;
			}
			else if (!(utf8str[i]&32))
			{
				// < 0x800 2bytes
				wcstr[result] = ((utf8str[i]&0x1F)<<6);
				i++;
				wcstr[result] = wcstr[result] + (utf8str[i]&0x3F);
				result++;
			}
			else if (!(utf8str[i]&16))
			{
				// <0x10000 3bytes
				wcstr[result] = ((utf8str[i]&0xF)<<12);
				i++;
				wcstr[result] = wcstr[result] + ((utf8str[i]&0x3F)<<6);
				i++;
				wcstr[result] = wcstr[result] + (utf8str[i]&0x3F);
				result++;
			}			
			if (result >= (int)count-1) break;
		}
        
		assert(result < (int)count);
        if (result > 0) wcstr[result] = 0;
        return result+1;
    }
	
    static int __cdecl _getwcslenofutf8(const char* utf8str)
    {
		int len = strlen(utf8str);
		int i=0, result=0;
		for (i=0, result=0; i<len; i++)
		{
			if (!(utf8str[i]&128))
			{
				// < 0x80 1bytes
				result++;
			}
			else if (!(utf8str[i]&32))
			{
				// < 0x800 2bytes
				i++;
				result++;
			}
			else if (!(utf8str[i]&16))
			{
				// <0x10000 3bytes
				i++;
				i++;
				result++;
			}
		}
        return result;
    }
};

// Compare helpers
inline bool __stdcall operator==(const KK_StringW& s1, const KK_StringW& s2) { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(const KK_StringW& s1, const wchar_t* s2) { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(const wchar_t* s1, const KK_StringW& s2) { return s2.Compare(s1) == 0; }
inline bool __stdcall operator!=(const KK_StringW& s1, const KK_StringW& s2) { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(const KK_StringW& s1, const wchar_t* s2) { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(const wchar_t* s1, const KK_StringW& s2) { return s2.Compare(s1) != 0; }
inline bool __stdcall operator<(const KK_StringW& s1, const KK_StringW& s2) { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(const KK_StringW& s1, const wchar_t* s2) { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(const wchar_t* s1, const KK_StringW& s2) { return s2.Compare(s1) > 0; }
inline bool __stdcall operator>(const KK_StringW& s1, const KK_StringW& s2) { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(const KK_StringW& s1, const wchar_t* s2) { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(const wchar_t* s1, const KK_StringW& s2) { return s2.Compare(s1) < 0; }
inline bool __stdcall operator<=(const KK_StringW& s1, const KK_StringW& s2) { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(const KK_StringW& s1, const wchar_t* s2) { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(const wchar_t* s1, const KK_StringW& s2) { return s2.Compare(s1) >= 0; }
inline bool __stdcall operator>=(const KK_StringW& s1, const KK_StringW& s2) { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(const KK_StringW& s1, const wchar_t* s2) { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(const wchar_t* s1, const KK_StringW& s2) { return s2.Compare(s1) <= 0; }

// friend functions
inline KK_StringW __stdcall operator+(const KK_StringW& string1, const KK_StringW& string2)
{
    KK_StringW s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), string2.GetData()->nDataLength, string2.GetData()->dataConst());
    return s;
}

inline KK_StringW __stdcall operator+(const KK_StringW& string1, char ch)
{
    KK_StringW s;
    wchar_t wch(ch);
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), 1, &wch);
    return s;
}

inline KK_StringW __stdcall operator+(const KK_StringW& string1, wchar_t ch)
{
    KK_StringW s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.GetData()->dataConst(), 1, &ch);
    return s;
}

inline KK_StringW __stdcall operator+(wchar_t ch, const KK_StringW& string1)
{
    KK_StringW s;
    s.ConcatCopy(1, &ch, string1.GetData()->nDataLength, string1.GetData()->dataConst());
    return s;
}

inline KK_StringW __stdcall operator+(char ch, const KK_StringW& string1)
{
    KK_StringW s;
    wchar_t wch(ch);
    s.ConcatCopy(1, &wch, string1.GetData()->nDataLength, string1.GetData()->dataConst());
    return s;
}


#endif  // __KK_STRING_W_H__
