//
//  KK_Templet.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//


#ifndef __KK_TEMPL_H__
#define __KK_TEMPL_H__


#include "KK_Includes.h"
#include "KK_String.h"
#include "KK_Plex.h"
#include "KK_ByteStream.h"
#include "new"

/////////////////////////////////////////////////////////////////////////////
// global helpers (can be overridden)

template<class TYPE>
inline void KK_ConstructElements(TYPE* pElements, int nCount)
{
	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

    
	// then call the constructor(s)
	for (; nCount--; pElements++)
        ::new ((void*)pElements) TYPE();
}

template<class TYPE>
inline void KK_DestructElements(TYPE* pElements, int nCount)
{
	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}

template<class TYPE>
inline void KK_CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	// default is element-copy using assignment
	while (nCount--)
		*pDest++ = *pSrc++;
}

// < 0 pElement1 less than pElement2 
// = 0 pElement1 identical to pElement2 
// > 0 pElement1 greater than pElement2 
template<class TYPE, class ARG_TYPE>
inline int KK_CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{ return memcmp(pElement1, pElement2, sizeof(TYPE)); }

template<> inline int KK_CompareElements(const u_longlong* pElement1, const u_longlong* pElement2)
{ return *pElement1 - *pElement2 == 0 ? 0 : (*pElement1 > *pElement2 ? 1 : -1); }
template<> inline int KK_CompareElements(const longlong* pElement1, const longlong* pElement2)
{ return *pElement1 - *pElement2 == 0 ? 0 : (*pElement1 > *pElement2 ? 1 : -1); }
template<> inline int KK_CompareElements(const u_long* pElement1, const u_long* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const long* pElement1, const long* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const u_int* pElement1, const u_int* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const int* pElement1, const int* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const u_short* pElement1, const u_short* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const short* pElement1, const short* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const u_char* pElement1, const u_char* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const char* pElement1, const char* pElement2)
{ return *pElement1 - *pElement2; }
template<> inline int KK_CompareElements(const KK_StringA* pElement1, const KK_StringA* pElement2)
{ return pElement1->CompareNoCase(*pElement2); }
template<> inline int KK_CompareElements(const KK_StringW* pElement1, const KK_StringW* pElement2)
{ return pElement1->CompareNoCase(*pElement2); }
template<> inline int KK_CompareElements(const KK_StringU* pElement1, const KK_StringU* pElement2)
{ return pElement1->CompareNoCase(*pElement2); }

const int bytebitcount[256] = {
    
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

inline u_long KK_HashKey(const u_long * key, int len)
{
	u_long nHash = 0xa317def2;
	u_char * pHash = (u_char*)&nHash;
	char bitcount;
    
	for (int i = 0; i < len; i++) {
		nHash ^= key[i];
		bitcount = (bytebitcount[pHash[0]] + bytebitcount[pHash[1]] + bytebitcount[pHash[2]] + bytebitcount[pHash[3]]);
        
        __asm__ __volatile__ ("movb bitcount %cl, ");
        __asm__ __volatile__ ("rol %cl nHash, ");
        
        //nHash = (nHash << bitcount) | (nHash >> (32 - bitcount));
        
	}
    
	return nHash;
}


template<class ARG_KEY> inline u_int KK_HashKey(ARG_KEY key)
{ // default identity hash - works for most primitive values
	return ((u_int)(void*)(u_long)key) >> 4;
}


template<> inline u_int KK_HashKey(u_longlong key)
{ 
    u_int l, h; 
    l = (u_int)(key & 0x00000000ffffffff); 
    h = (u_int)((key & 0x00000000ffffffff) >> 32); 
    return h^l;
}

template<> inline u_int KK_HashKey(long long key)
{ 
    u_int l, h; 
    l = (u_int)(key & 0x00000000ffffffff); 
    h = (u_int)((key & 0x00000000ffffffff) >> 32); 
    return h^l; 
}

template<> inline u_int KK_HashKey(u_long key)
{ return key; }

template<> inline u_int KK_HashKey(long key)
{ return key; }

template<> inline u_int KK_HashKey(u_int key)
{ return key; }

template<> inline u_int KK_HashKey(int key)
{ return key; }

template<> inline u_int KK_HashKey(u_short key)
{ return key; }

template<> inline u_int KK_HashKey(short key)
{ return key; }

template<> inline u_int KK_HashKey(u_char key)
{ return key; }

template<> inline u_int KK_HashKey(char key)
{ return key; }

template<> inline u_int KK_HashKey(const wchar_t* key)
{
	u_int nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

template<> inline u_int KK_HashKey(const char* key)
{
	u_int nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

template<> inline u_int KK_HashKey(const KK_StringA& key)
{
	u_int nHash = 0;
	for (int i=0; i<key.GetLength(); i++)
	{
		nHash = (nHash<<5) + nHash + key.GetAt(i);
	}
	return nHash;
}

template<> inline u_int KK_HashKey(const KK_StringW& key)
{
	u_int nHash = 0;
	for (int i=0; i<key.GetLength(); i++)
	{
		nHash = (nHash<<5) + nHash + key.GetAt(i);
	}
	return nHash;
}

template<> inline u_int KK_HashKey(const KK_StringU& key)
{
	u_int nHash = 0;
	for (int i=0; i<key.GetLength(); i++)
	{
		nHash = (nHash<<5) + nHash + key.GetAt(i);
	}
	return nHash;
}

template<> inline u_int KK_HashKey(const void* key)
{
	return *(u_int*)key;
}


/////////////////////////////////////////////////////////////////////////////
// KK_Pair<T1, T2> 
template<class T1, class T2>
class KK_Pair
{
public:
	KK_Pair()
    : first(T1()), second(T2()) {}
	KK_Pair(const T1& V1, const T2& V2)
    : first(V1), second(V2) {}
	KK_Pair(const KK_Pair &p)
    : first(p.first), second(p.second) {}
    
	const KK_Pair& operator = (const KK_Pair& src)
	{
		first = src.GetFirst();
		second = src.GetSecond();
		return *this;
	}
    
public:
	inline void Set(const T1& V1, const T2& V2) { first = V1; second = V2; }
	inline void SetFirst(const T1 & value) { first = value; }
	inline void SetSecond(const T2 & value) { second = value; }
    
	inline const T1& GetFirst() const { return first; }
	inline const T2& GetSecond() const { return second; }
    
protected:
	T1 first;
	T2 second;
};

template<class T1, class T2>
KK_ByteStream& operator >> (KK_ByteStream & is, KK_Pair<T1, T2> & pair)
{
	T1 t1;
	T2 t2;
	is >> t1;
	is >> t2;
	pair.Set(t1, t2);
	return is;
}

template<class T1, class T2>
KK_ByteStream& operator << (KK_ByteStream & os, const KK_Pair<T1, T2> & pair)
{
	os << pair.GetFirst();
	os << pair.GetSecond();
	return os;
}

/////////////////////////////////////////////////////////////////////////////
// KK_Array<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class KK_Array
{
public:
    // Construction
	KK_Array();
    
    // Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);
    
    // Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();
    
	// Accessing elements
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);
    
	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();
    
	// Potentially growing the array
	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const KK_Array& src);
	void Copy(const KK_Array& src);
    
	// overloaded operator helpers
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);
    
	// Operations that move elements around
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, KK_Array* pNewArray);
    
	// Find
	int Find(ARG_TYPE searchValue, int nIndex = 0) const;
	// Sort
	void Sort(bool bAsc = true);
	// Unique
	void Unique(bool bSorted = true);
    
protected:
	const KK_Array& operator = (const KK_Array &src)
	{	
		KK_Assert(false);
		return *this;
	}
	
	KK_Array(const KK_Array &src) 
	{
		KK_Assert(false);
		*this = src;
	}
    
protected:
	static int SortAscCompare(const void *pElement1, const void *pElement2);
	static int SortDesCompare(const void *pElement1, const void *pElement2);
    
    // Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
    
public:
	~KK_Array();	
    
};

/////////////////////////////////////////////////////////////////////////////
// KK_Array<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
inline int KK_Array<TYPE, ARG_TYPE>::GetSize() const
{ return m_nSize; }
template<class TYPE, class ARG_TYPE>
inline int KK_Array<TYPE, ARG_TYPE>::GetUpperBound() const
{ return m_nSize-1; }
template<class TYPE, class ARG_TYPE>
inline void KK_Array<TYPE, ARG_TYPE>::RemoveAll()
{ SetSize(0, -1); }
template<class TYPE, class ARG_TYPE>
inline TYPE KK_Array<TYPE, ARG_TYPE>::GetAt(int nIndex) const
{ assert(nIndex >= 0 && nIndex < m_nSize);
    return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline void KK_Array<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
{ assert(nIndex >= 0 && nIndex < m_nSize);
    m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_Array<TYPE, ARG_TYPE>::ElementAt(int nIndex)
{ assert(nIndex >= 0 && nIndex < m_nSize);
    return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline const TYPE* KK_Array<TYPE, ARG_TYPE>::GetData() const
{ return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
inline TYPE* KK_Array<TYPE, ARG_TYPE>::GetData()
{ return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
inline int KK_Array<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
{ int nIndex = m_nSize;
    SetAtGrow(nIndex, newElement);
    return nIndex; }
template<class TYPE, class ARG_TYPE>
inline TYPE KK_Array<TYPE, ARG_TYPE>::operator[](int nIndex) const
{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_Array<TYPE, ARG_TYPE>::operator[](int nIndex)
{ return ElementAt(nIndex); }
template<class TYPE, class ARG_TYPE>
inline int KK_Array<TYPE, ARG_TYPE>::SortAscCompare(const void *pElement1, const void *pElement2)
{ return KK_CompareElements((TYPE*)pElement1, (TYPE*)pElement2); }
template<class TYPE, class ARG_TYPE>
inline int KK_Array<TYPE, ARG_TYPE>::SortDesCompare(const void *pElement1, const void *pElement2)
{ return KK_CompareElements((TYPE*)pElement2, (TYPE*)pElement1); }

/////////////////////////////////////////////////////////////////////////////
// KK_Array<TYPE, ARG_TYPE> out-of-line functions
template<class TYPE, class ARG_TYPE>
KK_Array<TYPE, ARG_TYPE>::KK_Array()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

template<class TYPE, class ARG_TYPE>
KK_Array<TYPE, ARG_TYPE>::~KK_Array()
{
	if (m_pData != NULL)
	{
		KK_DestructElements<TYPE>(m_pData, m_nSize);
		delete[] (u_char*)m_pData;
	}
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
	assert(nNewSize >= 0);
    
	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size
    
	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			KK_DestructElements<TYPE>(m_pData, m_nSize);
			delete[] (u_char*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		assert(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new u_char[nNewSize * sizeof(TYPE)];
		KK_ConstructElements<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			KK_ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			KK_DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush
        
		assert(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		assert(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new u_char[nNewMax * sizeof(TYPE)];
        
		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
        
		// construct remaining elements
		assert(nNewSize > m_nSize);
		KK_ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);
        
		// get rid of old stuff (note: no destructors called)
		delete[] (u_char*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class ARG_TYPE>
int KK_Array<TYPE, ARG_TYPE>::Append(const KK_Array& src)
{
	//ASSERT_VALID(this);
	assert(this != &src);   // cannot append to itself
    
	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	KK_CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::Copy(const KK_Array& src)
{
	//ASSERT_VALID(this);
	assert(this != &src);   // cannot append to itself
    
	SetSize(src.m_nSize);
	KK_CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		assert(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) new u_char[m_nSize * sizeof(TYPE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}
        
		// get rid of old stuff (note: no destructors called)
		delete[] (u_char*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	assert(nIndex >= 0);
    
	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
	assert(nIndex >= 0);    // will expand to meet need
	assert(nCount > 0);     // zero or negative size not allowed
    
	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		KK_DestructElements<TYPE>(&m_pData[nOldSize], nCount);
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
                (nOldSize-nIndex) * sizeof(TYPE));
        
		// re-init slots we copied from
		KK_ConstructElements<TYPE>(&m_pData[nIndex], nCount);
	}
    
	// insert new value in the gap
	assert(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	assert(nIndex >= 0);
	assert(nCount >= 0);
	assert(nIndex + nCount <= m_nSize);
    
	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	KK_DestructElements<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
                nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, KK_Array* pNewArray)
{
	assert(pNewArray != NULL);
	//ASSERT_VALID(pNewArray);
	assert(nStartIndex >= 0);
    
	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

template<class TYPE, class ARG_TYPE>
int KK_Array<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, int nIndex /*= 0*/) const
{
	if (nIndex >= m_nSize || nIndex < 0)
		return -1;  // went too far
	
	for (int i=nIndex; i < m_nSize; i++)
	{
		if (KK_CompareElements(m_pData + i, &searchValue) == 0) return i;
	}
    
	return -1;
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::Sort(bool bAsc /*= true*/)
{
	if (m_nSize > 0)
	{
		if (bAsc)
			qsort((void *)m_pData, (size_t)m_nSize, sizeof(TYPE), SortAscCompare);
		else
			qsort((void *)m_pData, (size_t)m_nSize, sizeof(TYPE), SortDesCompare);
	}
}

template<class TYPE, class ARG_TYPE>
void KK_Array<TYPE, ARG_TYPE>::Unique(bool bSorted /*= true*/)
{	
	if (bSorted)
	{
		int nRepeatCount = 0;
        int i = 0;
		for (i=m_nSize-1; i>0; i--)
		{		
			
			if (KK_CompareElements(m_pData+i, m_pData+i-1) == 0)
			{
				nRepeatCount++;
			}
			else if (nRepeatCount > 0) 
			{
				RemoveAt(i, nRepeatCount);
				nRepeatCount = 0;
			}
		}
        
		if(nRepeatCount)
			RemoveAt(i, nRepeatCount);
	}
	else
	{
		for(int j=0; j < m_nSize; j++)
		{
			for(int i=j+1; i< m_nSize; i++)
			{		
                if (KK_CompareElements(m_pData+j, m_pData+i) == 0)
                {
                    RemoveAt(i);
                    i--;
                }
			}
		}
	}
    
}


template<class TYPE, class ARG_TYPE>
KK_ByteStream& operator >> (KK_ByteStream & is, KK_Array<TYPE, ARG_TYPE> & array)
{
	int nCount = is.ReadLength();
	array.SetSize(nCount);
	TYPE* pElements = array.GetData();
	for (; nCount-- ; ++pElements)
		is >> *pElements;
	return is;
}

template<class TYPE, class ARG_TYPE>
KK_ByteStream& operator << (KK_ByteStream & os, const KK_Array<TYPE, ARG_TYPE> & array)
{
	int nCount = array.GetSize();
	os.WriteLength(nCount);
    
	const TYPE* pElements = array.GetData();
	for (; nCount-- ; ++pElements)
		os << *pElements;
	return os;
}

/////////////////////////////////////////////////////////////////////////////
// KK_List<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class KK_List
{
protected:
	struct KK_Node
	{
		KK_Node* pNext;
		KK_Node* pPrev;
		TYPE data;
	};
public:
    // Construction
	KK_List(int nBlockSize = 10);
    
    // Attributes (head and tail)
	// count of elements
	int GetCount() const;
	bool IsEmpty() const;
    
	// peek at head or tail
	TYPE& GetHead();
	TYPE GetHead() const;
	TYPE& GetTail();
	TYPE GetTail() const;
    
    // Operations
	// get head or tail (and remove it) - don't call on empty list !
	TYPE RemoveHead();
	TYPE RemoveTail();
    
	// add before head or after tail
	KK_POSITION AddHead(ARG_TYPE newElement);
	KK_POSITION AddTail(ARG_TYPE newElement);
    
	// add another list of elements before head or after tail
	void AddHead(KK_List* pNewList);
	void AddTail(KK_List* pNewList);
    
	// remove all elements
	void RemoveAll();
    
	// iteration
	KK_POSITION GetHeadPosition() const;
	KK_POSITION GetTailPosition() const;
	TYPE& GetNext(KK_POSITION& rPosition); // return *Position++
	TYPE GetNext(KK_POSITION& rPosition) const; // return *Position++
	TYPE& GetPrev(KK_POSITION& rPosition); // return *Position--
	TYPE GetPrev(KK_POSITION& rPosition) const; // return *Position--
    
	// getting/modifying an element at a given position
	TYPE& GetAt(KK_POSITION position);
	TYPE GetAt(KK_POSITION position) const;
	void SetAt(KK_POSITION pos, ARG_TYPE newElement);
	void RemoveAt(KK_POSITION position);
    
	// inserting before or after a given position
	KK_POSITION InsertBefore(KK_POSITION position, ARG_TYPE newElement);
	KK_POSITION InsertAfter(KK_POSITION position, ARG_TYPE newElement);
    
	// helper functions (note: O(n) speed)
	KK_POSITION Find(ARG_TYPE searchValue, KK_POSITION startAfter = NULL) const;
    // defaults to starting at the HEAD, return NULL if not found
	KK_POSITION FindIndex(int nIndex) const;
    // get the 'nIndex'th element (may return NULL)
    
private:
	const KK_List& operator = (const KK_List &src)
	{	
		KK_Assert(false);
		return *this;
	}
    
	KK_List(const KK_List &src) 
	{
		KK_Assert(false);
		*this = src;
	}
    
    // Implementation
protected:
	KK_Node* m_pNodeHead;
	KK_Node* m_pNodeTail;
	int m_nCount;
	KK_Node* m_pNodeFree;
	struct KK_Plex* m_pBlocks;
	int m_nBlockSize;
    
	KK_Node* NewNode(KK_Node*, KK_Node*);
	void FreeNode(KK_Node*);
    
public:
	~KK_List();
};

/////////////////////////////////////////////////////////////////////////////
// KK_List<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
inline int KK_List<TYPE, ARG_TYPE>::GetCount() const
{ return m_nCount; }
template<class TYPE, class ARG_TYPE>
inline bool KK_List<TYPE, ARG_TYPE>::IsEmpty() const
{ return m_nCount == 0; }
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_List<TYPE, ARG_TYPE>::GetHead()
{ assert(m_pNodeHead != NULL);
    return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE KK_List<TYPE, ARG_TYPE>::GetHead() const
{ assert(m_pNodeHead != NULL);
    return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_List<TYPE, ARG_TYPE>::GetTail()
{ assert(m_pNodeTail != NULL);
    return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE KK_List<TYPE, ARG_TYPE>::GetTail() const
{ assert(m_pNodeTail != NULL);
    return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
inline KK_POSITION KK_List<TYPE, ARG_TYPE>::GetHeadPosition() const
{ return (KK_POSITION) m_pNodeHead; }
template<class TYPE, class ARG_TYPE>
inline KK_POSITION KK_List<TYPE, ARG_TYPE>::GetTailPosition() const
{ return (KK_POSITION) m_pNodeTail; }
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_List<TYPE, ARG_TYPE>::GetNext(KK_POSITION& rPosition) // return *Position++
{ 
    KK_Node* pNode = (KK_Node*) rPosition;
    rPosition = (KK_POSITION) pNode->pNext;
    return pNode->data; 
}
template<class TYPE, class ARG_TYPE>
inline TYPE KK_List<TYPE, ARG_TYPE>::GetNext(KK_POSITION& rPosition) const // return *Position++
{ 
    KK_Node* pNode = (KK_Node*) rPosition;
    rPosition = (KK_POSITION) pNode->pNext;
    return pNode->data; 
}
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_List<TYPE, ARG_TYPE>::GetPrev(KK_POSITION& rPosition) // return *Position--
{ 
    KK_Node* pNode = (KK_Node*) rPosition;
    rPosition = (KK_POSITION) pNode->pPrev;
    return pNode->data; 
}
template<class TYPE, class ARG_TYPE>
inline TYPE KK_List<TYPE, ARG_TYPE>::GetPrev(KK_POSITION& rPosition) const // return *Position--
{
    KK_Node* pNode = (KK_Node*) rPosition;
    rPosition = (KK_POSITION) pNode->pPrev;
    return pNode->data; 
}
template<class TYPE, class ARG_TYPE>
inline TYPE& KK_List<TYPE, ARG_TYPE>::GetAt(KK_POSITION position)
{ KK_Node* pNode = (KK_Node*) position;
    return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE KK_List<TYPE, ARG_TYPE>::GetAt(KK_POSITION position) const
{ KK_Node* pNode = (KK_Node*) position;
    return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline void KK_List<TYPE, ARG_TYPE>::SetAt(KK_POSITION pos, ARG_TYPE newElement)
{
    assert(pos != NULL);
    if (pos == NULL) return;
    KK_Node* pNode = (KK_Node*) pos;
    pNode->data = newElement;
}

template<class TYPE, class ARG_TYPE>
KK_List<TYPE, ARG_TYPE>::KK_List(int nBlockSize)
{
	assert(nBlockSize > 0);
    
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class TYPE, class ARG_TYPE>
void KK_List<TYPE, ARG_TYPE>::RemoveAll()
{
	// destroy elements
	KK_Node* pNode;
	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		KK_DestructElements<TYPE>(&pNode->data, 1);
    
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class TYPE, class ARG_TYPE>
KK_List<TYPE, ARG_TYPE>::~KK_List()
{
	RemoveAll();
	assert(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//
// Implementation note: KK_Node's are stored in KK_Plex blocks and
//  chained together. Free blocks are maintained in a singly linked list
//  using the 'pNext' member of KK_Node with 'm_pNodeFree' as the head.
//  Used blocks are maintained in a doubly linked list using both 'pNext'
//  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
//   as the head/tail.
//
// We never free a KK_Plex block unless the List is destroyed or RemoveAll()
//  is used - so the total number of KK_Plex blocks may grow large depending
//  on the maximum past size of the list.
//

template<class TYPE, class ARG_TYPE>
typename KK_List<TYPE, ARG_TYPE>::KK_Node*
KK_List<TYPE, ARG_TYPE>::NewNode(typename KK_List::KK_Node* pPrev, typename KK_List::KK_Node* pNext)
{
	if (m_pNodeFree == NULL)
	{
		// add another block
		KK_Plex* pNewBlock = KK_Plex::Create(m_pBlocks, m_nBlockSize,
                                             sizeof(KK_Node));
        
		// chain them into free list
		KK_Node* pNode = (KK_Node*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
	assert(m_pNodeFree != NULL);  // we must have something
    
	KK_Node* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	assert(m_nCount > 0);  // make sure we don't overflow
    
	KK_ConstructElements<TYPE>(&pNode->data, 1);
	return pNode;
}

template<class TYPE, class ARG_TYPE>
void KK_List<TYPE, ARG_TYPE>::FreeNode(typename KK_List::KK_Node* pNode)
{
	KK_DestructElements<TYPE>(&pNode->data, 1);
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
	assert(m_nCount >= 0);  // make sure we don't underflow
    
	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class TYPE, class ARG_TYPE>
KK_POSITION KK_List<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement)
{
	KK_Node* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (KK_POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
KK_POSITION KK_List<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement)
{
	KK_Node* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (KK_POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void KK_List<TYPE, ARG_TYPE>::AddHead(KK_List* pNewList)
{
	assert(pNewList != NULL);
    
	// add a list of same elements to head (maintain order)
	KK_POSITION pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
}

template<class TYPE, class ARG_TYPE>
void KK_List<TYPE, ARG_TYPE>::AddTail(KK_List* pNewList)
{
	assert(pNewList != NULL);
    
	// add a list of same elements
	KK_POSITION pos = pNewList->GetHeadPosition();
	while (pos != NULL)
		AddTail(pNewList->GetNext(pos));
}

template<class TYPE, class ARG_TYPE>
TYPE KK_List<TYPE, ARG_TYPE>::RemoveHead()
{
	assert(m_pNodeHead != NULL);  // don't call on empty list !!!
    
	KK_Node* pOldNode = m_pNodeHead;
	TYPE returnValue = pOldNode->data;
    
	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
TYPE KK_List<TYPE, ARG_TYPE>::RemoveTail()
{
	assert(m_pNodeTail != NULL);  // don't call on empty list !!!
    
	KK_Node* pOldNode = m_pNodeTail;
	TYPE returnValue = pOldNode->data;
    
	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
KK_POSITION KK_List<TYPE, ARG_TYPE>::InsertBefore(KK_POSITION position, ARG_TYPE newElement)
{
	if (position == NULL)
		return AddHead(newElement); // insert before nothing -> head of the list
    
	// Insert it before position
	KK_Node* pOldNode = (KK_Node*) position;
	KK_Node* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;
    
	if (pOldNode->pPrev != NULL)
	{
		//assert(AfxIsValidAddress(pOldNode->pPrev, sizeof(KK_Node)));
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		assert(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (KK_POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
KK_POSITION KK_List<TYPE, ARG_TYPE>::InsertAfter(KK_POSITION position, ARG_TYPE newElement)
{
	if (position == NULL)
		return AddTail(newElement); // insert after nothing -> tail of the list
    
	// Insert it before position
	KK_Node* pOldNode = (KK_Node*) position;
	KK_Node* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;
    
	if (pOldNode->pNext != NULL)
	{
		//assert(AfxIsValidAddress(pOldNode->pNext, sizeof(KK_Node)));
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		assert(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (KK_POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void KK_List<TYPE, ARG_TYPE>::RemoveAt(KK_POSITION position)
{
	assert(position != NULL);
	if (position == NULL) return;
	KK_Node* pOldNode = (KK_Node*) position;
    
	// remove pOldNode from list
    pOldNode == m_pNodeHead ? m_pNodeHead = pOldNode->pNext : pOldNode->pPrev->pNext = pOldNode->pNext;
	
    pOldNode == m_pNodeTail ? m_pNodeTail = pOldNode->pPrev : pOldNode->pNext->pPrev = pOldNode->pPrev;
	
	FreeNode(pOldNode);
}

template<class TYPE, class ARG_TYPE>
KK_POSITION KK_List<TYPE, ARG_TYPE>::FindIndex(int nIndex) const
{
	if (nIndex >= m_nCount || nIndex < 0)
		return NULL;  // went too far
    
	KK_Node* pNode = m_pNodeHead;
	while (nIndex--)
	{
		pNode = pNode->pNext;
	}
	return (KK_POSITION) pNode;
}

template<class TYPE, class ARG_TYPE>
KK_POSITION KK_List<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, KK_POSITION startAfter) const
{
	KK_Node* pNode = (KK_Node*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		pNode = pNode->pNext;  // start after the one specified
	}
    
	for (; pNode != NULL; pNode = pNode->pNext)
		if (KK_CompareElements(&pNode->data, &searchValue) == 0)
			return (KK_POSITION)pNode;
	return NULL;
}


template<class TYPE, class ARG_TYPE>
KK_ByteStream& operator >> (KK_ByteStream & is, KK_List<TYPE, ARG_TYPE> & list)
{
	int nCount = is.ReadLength();
	while (nCount--)
	{
		TYPE newData;
		is >> newData;
		list.AddTail(newData);
	}
	return is;
}

template<class TYPE, class ARG_TYPE>
KK_ByteStream& operator << (KK_ByteStream & os, const KK_List<TYPE, ARG_TYPE> & list) 
{
	int nCount = list.GetCount();
	os.WriteLength(nCount);
	for (KK_POSITION pos=list.GetHeadPosition(); pos!=NULL; )
	{
		os << list.GetNext(pos);
	}
	return os;
}

/////////////////////////////////////////////////////////////////////////////
// KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class KK_Map
{
protected:
	// Association
	struct KK_Assoc
	{
		KK_Assoc* pNext;
		u_int nHashValue;  // needed for efficient iteration
		KEY key;
		VALUE value;
	};
public:
    // Construction
	KK_Map(int nBlockSize = 10);
    
    // Attributes
	// number of elements
	int GetCount() const;
	bool IsEmpty() const;
    
	// Lookup
	bool Lookup(ARG_KEY key, VALUE& rValue) const;
	bool Lookup(ARG_KEY key) const;
    
    // Operations
	// Lookup and add if not there
	VALUE& operator[](ARG_KEY key);
    
	// add a new (key, value) pair
	void SetAt(ARG_KEY key, ARG_VALUE newValue);
    
	// removing existing (key, ?) pair
	bool RemoveKey(ARG_KEY key);
	bool RemoveKey(ARG_KEY key, VALUE& rValue);
	void RemoveAll();
    
	// iterating all (key, value) pairs
	KK_POSITION GetStartPosition() const;
	VALUE& GetNextAssoc(KK_POSITION& rNextPosition) const;
	void GetNextAssoc(KK_POSITION& rNextPosition, KEY& rKey) const;
	void GetNextAssoc(KK_POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;
	void GetNextAssocEx(KK_POSITION& rNextPosition, KEY& rKey, VALUE& rValue, 
                        u_int nMinBucket, u_int nMaxBucket) const;
    
	// advanced features for derived classes
	u_int GetHashTableSize() const;
	void InitHashTable(u_int hashSize, bool bAllocNow = true);
    
private:
	const KK_Map& operator = (const KK_Map &src)
	{	
		KK_Assert(false);
		return *this;
	}
	
	KK_Map(const KK_Map &src) 
	{
		KK_Assert(false);
		*this = src;
	}
    
    // Implementation
protected:
	KK_Assoc** m_pHashTable;
	u_int m_nHashTableSize;
	int m_nCount;
	KK_Assoc* m_pFreeList;
	struct KK_Plex* m_pBlocks;
	int m_nBlockSize;
    
	KK_Assoc* NewAssoc();
	void FreeAssoc(KK_Assoc*);
	KK_Assoc* GetAssocAt(ARG_KEY, u_int&) const;
    
public:
	~KK_Map();                                                                                                                                              
};

/////////////////////////////////////////////////////////////////////////////
// KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE> inline functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline int KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetCount() const
{ return m_nCount; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline bool KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::IsEmpty() const
{ return m_nCount == 0; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
{ (*this)[key] = newValue; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline KK_POSITION KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition() const
{ return (m_nCount == 0) ? NULL : KK_BEFORE_START_POSITION; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline u_int KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetHashTableSize() const
{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////
// KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE> out-of-line functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Map(int nBlockSize)
{
	assert(nBlockSize > 0);
    
	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::InitHashTable(
                                                           u_int nHashSize, bool bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{
	assert(m_nCount == 0);
	assert(nHashSize > 0);
    
	if (m_pHashTable != NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}
    
	if (bAllocNow)
	{
		m_pHashTable = new KK_Assoc* [nHashSize];
		memset(m_pHashTable, 0, sizeof(KK_Assoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll()
{
	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (u_int nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			KK_Assoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
                 pAssoc = pAssoc->pNext)
			{
				KK_DestructElements<VALUE>(&pAssoc->value, 1);
				KK_DestructElements<KEY>(&pAssoc->key, 1);
			}
		}
	}
    
	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;
    
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::~KK_Map()
{
	RemoveAll();
	assert(m_nCount == 0);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Assoc*
KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewAssoc()
{
	if (m_pFreeList == NULL)
	{
		// add another block
		KK_Plex* newBlock = KK_Plex::Create(m_pBlocks, m_nBlockSize, sizeof(KK_Assoc));
		// chain them into free list
		KK_Assoc* pAssoc = (KK_Assoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	assert(m_pFreeList != NULL);  // we must have something
    
	KK_Assoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	assert(m_nCount > 0);  // make sure we don't overflow
	KK_ConstructElements<KEY>(&pAssoc->key, 1);
	KK_ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::FreeAssoc(typename KK_Map::KK_Assoc* pAssoc)
{
	KK_DestructElements<VALUE>(&pAssoc->value, 1);
	KK_DestructElements<KEY>(&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	assert(m_nCount >= 0);  // make sure we don't underflow
    
	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Assoc*
KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAt(ARG_KEY key, u_int& nHash) const
// find association (or return NULL)
{
	nHash = KK_HashKey<ARG_KEY>(key) % m_nHashTableSize;
    
	if (m_pHashTable == NULL) return NULL;
    
	// see if it exists
	int nBucketSize = 0;
	KK_Assoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (++nBucketSize > 30)
		{
            printf("warning: unreasonable hash key!\r\n");
		}
		if (KK_CompareElements(&pAssoc->key, &key) == 0)
			return pAssoc;
	}
	return NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const
{
	u_int nHash;
	KK_Assoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return false;  // not in map
    
	rValue = pAssoc->value;
	return true;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key) const
{
	u_int nHash;
	return (GetAssocAt(key, nHash) != NULL);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key)
{
	u_int nHash;
	KK_Assoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);
        
		// it doesn't exist, add a new Association
		pAssoc = NewAssoc();
		pAssoc->nHashValue = nHash;
		pAssoc->key = key;
		// 'pAssoc->value' is a constructed object, nothing more
        
		// put into hash table
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key)
// remove key - return true if removed
{
	if (m_pHashTable == NULL)
		return false;  // nothing in the table
    
	KK_Assoc** ppAssocPrev;
	ppAssocPrev = &m_pHashTable[KK_HashKey<ARG_KEY>(key) % m_nHashTableSize];
    
	KK_Assoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (KK_CompareElements(&pAssoc->key, &key) == 0)
		{
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return true;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return false;  // not found
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key, VALUE& rValue)
// remove key - return true if removed
{
	if (m_pHashTable == NULL) return false;  // nothing in the table
    
	KK_Assoc** ppAssocPrev;
	ppAssocPrev = &m_pHashTable[KK_HashKey<ARG_KEY>(key) % m_nHashTableSize];
    
	KK_Assoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (KK_CompareElements(&pAssoc->key, &key) == 0)
		{
			rValue = pAssoc->value;
            
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return true;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return false;  // not found
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(KK_POSITION& rNextPosition) const
{
	assert(m_pHashTable != NULL);  // never call on empty map
    
	KK_Assoc* pAssocRet = (KK_Assoc*)rNextPosition;
	assert(pAssocRet != NULL);
    
	if (pAssocRet == (KK_Assoc*) KK_BEFORE_START_POSITION)
	{
		// find the first association
		for (u_int nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		assert(pAssocRet != NULL);  // must find something
	}
    
	// find next association
	KK_Assoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (u_int nBucket = pAssocRet->nHashValue + 1;
             nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}
    
	rNextPosition = (KK_POSITION) pAssocNext;
    
	// fill in return data
	return pAssocRet->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(KK_POSITION& rNextPosition, KEY& rKey) const
{
	assert(m_pHashTable != NULL);  // never call on empty map
    
	KK_Assoc* pAssocRet = (KK_Assoc*)rNextPosition;
	assert(pAssocRet != NULL);
    
	if (pAssocRet == (KK_Assoc*) KK_BEFORE_START_POSITION)
	{
		// find the first association
		for (u_int nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		assert(pAssocRet != NULL);  // must find something
	}
    
	// find next association
	KK_Assoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (u_int nBucket = pAssocRet->nHashValue + 1;
             nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}
    
	rNextPosition = (KK_POSITION) pAssocNext;
    
	// fill in return data
	rKey = pAssocRet->key;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(KK_POSITION& rNextPosition,
                                                          KEY& rKey, VALUE& rValue) const
{
	assert(m_pHashTable != NULL);  // never call on empty map
    
	KK_Assoc* pAssocRet = (KK_Assoc*)rNextPosition;
	assert(pAssocRet != NULL);
    
	if (pAssocRet == (KK_Assoc*) KK_BEFORE_START_POSITION)
	{
		// find the first association
		for (u_int nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		assert(pAssocRet != NULL);  // must find something
	}
    
	// find next association
	KK_Assoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (u_int nBucket = pAssocRet->nHashValue + 1;
             nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}
    
	rNextPosition = (KK_POSITION) pAssocNext;
    
	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssocEx(KK_POSITION& rNextPosition
                                                            , KEY& rKey, VALUE& rValue, u_int nMinBucket, u_int nMaxBucket) const
{
    //	ASSERT_VALID(this);
	assert(m_pHashTable != NULL);  // never call on empty map
	assert(nMaxBucket <= m_nHashTableSize && nMinBucket >= 0 && nMinBucket <= nMaxBucket);
    
	KK_Assoc* pAssocRet = (KK_Assoc*)rNextPosition;
	assert(pAssocRet != NULL);
    
	if (pAssocRet == (KK_Assoc*) KK_BEFORE_START_POSITION)
	{
		// find the first association
		for (u_int nBucket = nMinBucket; nBucket < nMaxBucket; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
        
		if (pAssocRet == NULL || pAssocRet == (KK_Assoc *)KK_BEFORE_START_POSITION)
		{
			rNextPosition = NULL;
            pAssocRet = NULL;
			return;
		}
	}
    
	// find next association
    //	assert(AfxIsValidAddress(pAssocRet, sizeof(KK_Assoc)));
	KK_Assoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (u_int nBucket = pAssocRet->nHashValue + 1;
             nBucket < nMaxBucket; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}
    
	rNextPosition = (KK_POSITION) pAssocNext;
    
	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}



template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_ByteStream& operator >> (KK_ByteStream & is, KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE> & map)
{
	int nCount = is.ReadLength();
	while (nCount--)
	{
		KEY newKey;
		VALUE newValue;
		is >> newKey;
		is >> newValue;
		map.SetAt(newKey, newValue);
	}
	return is;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_ByteStream& operator << (KK_ByteStream & os, const KK_Map<KEY, ARG_KEY, VALUE, ARG_VALUE> & map)
{
	int nCount = map.GetCount();
	os.WriteLength(nCount);
	for (KK_POSITION pos=map.GetStartPosition(); pos!=NULL; )
	{
		KEY key;
		VALUE value;
		map.GetNextAssoc(pos, key, value);
		os << key;
		os << value;
	}
	return os;
}



/////////////////////////////////////////////////////////////////////////////
// KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class KK_AvlTree
{
protected:
	enum ChangedCode
	{
		HeightUnChanged = 0,
		HeightChanged = 1
	};
    
	struct KK_Node 
	{
		KK_Node* pNext;
		KK_Node* pPrev;
		KK_Node* lchild;
		KK_Node* rchild;
		KEY key;
		VALUE value;
		short ibf;
	};
public:
	KK_AvlTree(int nBlockSize = 10);
	int GetCount() const;
	int GetTreeHeight() const;
	bool IsEmpty() const;
    
	bool RemoveKey(ARG_KEY rKey);
	bool RemoveKey(ARG_KEY rKey, VALUE& rValue);
	void RemoveAll();
    
	VALUE& operator[](ARG_KEY rKey);
	void SetAt(ARG_KEY rKey, ARG_VALUE newValue);
    
	bool Lookup(ARG_KEY rKey) const;
	bool Lookup(ARG_KEY rKey, VALUE& rValue) const;
    
	KK_POSITION GetHeadPosition() const;
	KK_POSITION GetTailPosition() const;
	
	VALUE& GetPrev(KK_POSITION& rPosition);
	VALUE GetPrev(KK_POSITION& rPosition) const; // return *Position++
	void GetPrev(KK_POSITION& rPosition, KEY& rKey) const; // return *Position++
	void GetPrev(KK_POSITION& rPosition, KEY& rKey, VALUE& rValue) const; // return *Position++
    
	VALUE& GetNext(KK_POSITION& rPosition); // return *Position++
	VALUE GetNext(KK_POSITION& rPosition) const; // return *Position++
	void GetNext(KK_POSITION& rPosition, KEY& rKey) const; // return *Position++
	void GetNext(KK_POSITION& rPosition, KEY& rKey, VALUE& rValue) const; // return *Position++
	
private:
	const KK_AvlTree& operator = (const KK_AvlTree &src)
	{	
		KK_Assert(false);
		return *this;
	}
	
	KK_AvlTree(const KK_AvlTree &src) 
	{
		KK_Assert(false);
		*this = src;
	}
    
protected:
	KK_Node* NewNode();
	void FreeNode(KK_Node* pNode);
	short GetNodeByKey(KK_Node* &lpRoot, KK_Node* &lpNode, ARG_KEY rKey);
	KK_Node* RemoveLeftmostElement(KK_Node* &lpCurrent, KK_Node* &lpChild, int &iFlag);//删除右子树中最左孩子
	KK_Node* Remove(KK_Node* &lpCurrent,ARG_KEY rKey, KK_Node* &lpWaste, int &iFlag);//删除以lpCurrent为根的子树中节点值为val的节点并返回新的根lpWaste是被删除的结点
	int GetTreeHeight(KK_Node* lpRoot) const;
	
	KK_Node* LL_singleRotation(KK_Node* lpCurrent);
	KK_Node* RR_singleRotation(KK_Node* lpCurrent);
	KK_Node* RL_doubleRotation(KK_Node* lpCurrent);
	KK_Node* LR_doubleRotation(KK_Node* lpCurrent);
protected:
	KK_Node* m_lpRoot;
	KK_Node* m_pFreeList;
	KK_Node* m_pNodeHead;
	KK_Node* m_pNodeTail;
	struct KK_Plex* m_pBlocks;
	int m_nCount;
	int m_nBlockSize;
public:
	~KK_AvlTree();
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_AvlTree(int nBlockSize)
{
	m_lpRoot = NULL;
	m_pFreeList = NULL;
	m_pNodeHead = NULL;
	m_pNodeTail = NULL;
	m_pBlocks = NULL;
	m_nCount = 0;
	m_nBlockSize = nBlockSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::~KK_AvlTree()
{
	RemoveAll();
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key)
{
	KK_Node* lpTemp;
	GetNodeByKey(m_lpRoot, lpTemp, key);
	return lpTemp->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
{
	(*this)[key] = newValue;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
int KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetTreeHeight() const
{
	return GetTreeHeight(m_lpRoot);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
int KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetTreeHeight(KK_Node* lpRoot) const
{
	if (!lpRoot)
	{
		return 0;
	}
	else if (!lpRoot->lchild && !lpRoot->rchild)
	{
		return 1;
	}
	else
		return Max(GetTreeHeight(lpRoot->lchild),GetTreeHeight(lpRoot->rchild)) + 1;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline KK_POSITION KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetHeadPosition() const
{ 
    return (KK_POSITION) m_pNodeHead;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline KK_POSITION KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetTailPosition() const
{ 
    return (KK_POSITION) m_pNodeTail;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetPrev(KK_POSITION& rPosition) const
{ 
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pPrev;
	return pNode->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetPrev(KK_POSITION& rPosition)
{ 
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pPrev;
	return pNode->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetPrev(KK_POSITION& rPosition, KEY& rkey) const
{
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pPrev;
	rkey = pNode->key;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetPrev(KK_POSITION& rPosition, KEY& rkey, VALUE& rValue) const
{ 
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pPrev;
	rkey = pNode->key;
	rValue = pNode->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNext(KK_POSITION& rPosition) const
{ 
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pNext;
	return pNode->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNext(KK_POSITION& rPosition)
{ 
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pNext;
	return pNode->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNext(KK_POSITION& rPosition, KEY& rkey) const
{
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pNext;
	rkey = pNode->key;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNext(KK_POSITION& rPosition, KEY& rkey, VALUE& rValue) const
{ 
	KK_Node* pNode = (KK_Node*) rPosition;
	rPosition = (KK_POSITION) pNode->pNext;
	rkey = pNode->key;
	rValue = pNode->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline bool KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::IsEmpty() const
{
	return (m_lpRoot?false:true);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline int KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetCount() const
{
	return m_nCount;
}

template<class KEY,class ARG_KEY,class VALUE,class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewNode()
{
	if (m_pFreeList == NULL)
	{
		// add another block
		KK_Plex* newBlock = KK_Plex::Create(m_pBlocks, m_nBlockSize, sizeof(KK_Node));
		// chain them into free list
		KK_Node* pNode = (KK_Node*) newBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pFreeList;
			m_pFreeList = pNode;
		}
	}
	assert(m_pFreeList != NULL);  // we must have something
    
	KK_Node* pNode = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	assert(m_nCount > 0);  // make sure we don't overflow
	KK_ConstructElements<KEY>(&pNode->key, 1);
	KK_ConstructElements<VALUE>(&pNode->value, 1);   // special construct values
	return pNode;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::FreeNode(typename KK_AvlTree::KK_Node* pNode)
{
	KK_DestructElements<VALUE>(&pNode->value, 1);
	KK_DestructElements<KEY>(&pNode->key, 1);
	pNode->pNext = m_pFreeList;
	m_pFreeList = pNode;
	m_nCount--;
	assert(m_nCount >= 0);  // make sure we don't underflow
    
	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll()
{
	KK_Node* pNode;
	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
	{
		KK_DestructElements<VALUE>(&pNode->value, 1);
		KK_DestructElements<KEY>(&pNode->key, 1);
	}
	m_nCount = 0;
	m_lpRoot = NULL;
	m_pNodeHead = m_pNodeTail = m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key)
{
	int iFlag = HeightUnChanged;
	KK_Node *lpWaste = NULL;
	m_lpRoot = Remove(m_lpRoot, key, lpWaste, iFlag);
	if(lpWaste)
	{
		if(lpWaste == m_pNodeHead)
		{
			m_pNodeHead = m_pNodeHead->pNext;
			if(m_pNodeHead != NULL) m_pNodeHead->pPrev = NULL;
		}
		else if(lpWaste == m_pNodeTail)
		{
			m_pNodeTail = m_pNodeTail->pPrev;
			m_pNodeTail->pNext = NULL;
		}
		else
		{
			lpWaste->pPrev->pNext = lpWaste->pNext;
			lpWaste->pNext->pPrev = lpWaste->pPrev;
		}
		FreeNode(lpWaste);
		return true;
	}
	else
		return false;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key, VALUE& value)
{
	int iFlag = HeightUnChanged;
	KK_Node *lpWaste = NULL;
	m_lpRoot = Remove(m_lpRoot, key, lpWaste, iFlag);
	if (lpWaste) 
	{
		if(lpWaste == m_pNodeHead)
		{
			m_pNodeHead = m_pNodeHead->pNext;
			if(m_pNodeHead != NULL) m_pNodeHead->pPrev = NULL;
		}
		else if(lpWaste == m_pNodeTail)
		{
			m_pNodeTail = m_pNodeTail->pPrev;
			m_pNodeTail->pNext = NULL;
		}
		else
		{
			lpWaste->pPrev->pNext = lpWaste->pNext;
			lpWaste->pNext->pPrev = lpWaste->pPrev;
		}
		value = lpWaste->value;
		FreeNode(lpWaste);
		return true;
	}
	else
		return false;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
short KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNodeByKey(typename KK_AvlTree::KK_Node* &lpRoot, typename KK_AvlTree::KK_Node* &lpNode, ARG_KEY rKey)
{
	if (lpRoot == NULL)
	{
		lpRoot = NewNode();
		lpRoot->ibf = 0;
		lpRoot->lchild = NULL;
		lpRoot->rchild = NULL;
		lpRoot->key = rKey;
		
		m_pNodeHead = m_pNodeTail = lpRoot;
		lpRoot->pPrev = NULL;
		lpRoot->pNext = NULL;
		
		lpNode = lpRoot;
		return 0;
	}
	KEY *pkey = &lpRoot->key;
	if (KK_CompareElements(pkey, &rKey) == 0)
	{
		lpNode = lpRoot;
		return 0;
	}
	else if (KK_CompareElements(pkey, &rKey) > 0)		//插入左边
	{
		if (lpRoot->lchild == NULL)
		{
			lpRoot->lchild = NewNode();
			lpRoot->lchild->ibf = 0;
			lpRoot->lchild->lchild = NULL;
			lpRoot->lchild->rchild = NULL;
			lpRoot->lchild->pPrev = NULL;
			lpRoot->lchild->pNext = NULL;
			lpRoot->lchild->key = rKey;
			
			if(m_pNodeHead == lpRoot)
			{
				lpRoot->lchild->pNext = m_pNodeHead;
				m_pNodeHead->pPrev = lpRoot->lchild;
				m_pNodeHead = lpRoot->lchild;
				
				lpNode = m_pNodeHead;
			}
			else
			{
				lpRoot->pPrev->pNext = lpRoot->lchild;
				lpRoot->lchild->pPrev = lpRoot->pPrev;
				lpRoot->lchild->pNext = lpRoot;
				lpRoot->pPrev = lpRoot->lchild;
				
				lpNode = lpRoot->pPrev;
			}
		}
		else if(GetNodeByKey(lpRoot->lchild, lpNode, rKey) == 0)
			return 0;
		
		if (lpRoot->ibf == -1)
		{
			if (lpRoot->lchild->ibf <0)
				lpRoot = LL_singleRotation(lpRoot);
			else
				lpRoot = LR_doubleRotation(lpRoot);
			
			return 0;
		}
		
		return --lpRoot->ibf;
	}
	else
	{
		if (lpRoot->rchild == NULL)
		{	
			lpRoot->rchild = NewNode();
			lpRoot->rchild->ibf = 0;
			lpRoot->rchild->lchild = NULL;
			lpRoot->rchild->rchild = NULL;
			lpRoot->rchild->pPrev = NULL;
			lpRoot->rchild->pNext = NULL;
            
			lpRoot->rchild->key = rKey;
            
			if(m_pNodeTail == lpRoot)
			{
				m_pNodeTail->pNext = lpRoot->rchild;
				lpRoot->rchild->pPrev = m_pNodeTail;
				m_pNodeTail = lpRoot->rchild;
                
				lpNode = m_pNodeTail;
			}
			else
			{
				lpRoot->rchild->pNext = lpRoot->pNext;
				lpRoot->pNext->pPrev = lpRoot->rchild;
				lpRoot->pNext = lpRoot->rchild;
				lpRoot->rchild->pPrev = lpRoot;
                
				lpNode = lpRoot->pNext;
			}
		}
		
		else if(GetNodeByKey(lpRoot->rchild, lpNode, rKey) == 0)
			return 0;
		
		if (lpRoot->ibf == 1)
		{
			if (lpRoot->rchild->ibf >0)
				lpRoot = RR_singleRotation(lpRoot);
			else
				lpRoot = RL_doubleRotation(lpRoot);
			
			return 0;
		}
		
		return ++lpRoot->ibf;
	}
	
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveLeftmostElement(KK_Node* &lpCurrent, KK_Node* &lpChild, int &iFlag)
{
	KK_Node* pLeftChild = lpCurrent->lchild;
	
	if (lpCurrent->lchild == NULL)
	{
		lpChild = lpCurrent;
		iFlag = HeightChanged;
		return lpCurrent->rchild;
	}
	
	int iOldBf = pLeftChild->ibf;
	lpCurrent->lchild = RemoveLeftmostElement(pLeftChild, lpChild,iFlag);
	KK_Node* pNewLeftChild = lpCurrent->lchild;
	
	if((pNewLeftChild == NULL) && (lpCurrent->rchild == NULL))
		iFlag = HeightChanged;
	
	if(pNewLeftChild == NULL)
		lpCurrent->ibf++;
	else if((pNewLeftChild->ibf != iOldBf) && (pNewLeftChild->ibf == 0))
		lpCurrent->ibf++;
	
	if(lpCurrent->ibf > 1)
	{
		int iNewOldBf = lpCurrent->rchild->ibf;
		if (iNewOldBf < 0)
			return RL_doubleRotation(lpCurrent);
		else
			return RR_singleRotation(lpCurrent);
		
	}
	
	return lpCurrent;
	
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::Remove(typename KK_AvlTree::KK_Node* &lpCurrent,ARG_KEY key, KK_Node* &lpWaste, int &iFlag)
{
	KEY* pkey = &lpCurrent->key;
	if (KK_CompareElements(pkey, &key) == 0)
	{
		lpWaste = lpCurrent;
        
		if (lpCurrent->rchild == NULL)
		{
			iFlag = HeightChanged;
			return lpCurrent->lchild;
		}
        
		//int iOldBf = lpCurrent->rchild->ibf;
		KK_Node *lpNewRoot;
		//lpCurrent->rchild = RemoveLeftmostElement(lpCurrent->lchild, lpNewRoot, iFlag);
        lpCurrent->rchild = RemoveLeftmostElement(lpCurrent->rchild, lpNewRoot, iFlag);
		lpNewRoot->lchild = lpCurrent->lchild;
		lpNewRoot->rchild = lpCurrent->rchild;
        
		if(iFlag == HeightChanged)
		{
			if(lpCurrent->ibf == 1)
				iFlag = HeightChanged;
			else
				iFlag = HeightUnChanged;
			lpNewRoot->ibf = lpCurrent->ibf - 1;
		}
		if (lpNewRoot->ibf < -1)
		{
			int iNewOldBf = lpNewRoot->lchild->ibf;
            
			if(iNewOldBf > 0)
				return LR_doubleRotation(lpNewRoot);
			else
				return LL_singleRotation(lpNewRoot);
            
		}
        
		return lpNewRoot;
	}
    
	else if (KK_CompareElements(pkey, &key) > 0)
	{
		if(lpCurrent->lchild == NULL)
			return lpCurrent;
        
		int iOldBf = lpCurrent->lchild->ibf;
		lpCurrent->lchild = Remove(lpCurrent->lchild, key, lpWaste, iFlag);
        
		KK_Node *lpLeftChild = lpCurrent->lchild;
        
		if (lpLeftChild == NULL)
			lpCurrent->ibf++;
		else if((lpLeftChild->ibf != iOldBf) && (lpLeftChild->ibf == 0))
			lpCurrent->ibf++;
        
		if (lpCurrent->ibf > 1)
		{
			int iNewOldBf = lpCurrent->rchild->ibf;
			if (iNewOldBf < 0)
				return RL_doubleRotation(lpCurrent);
			else
			{
				KK_Node *lpTemp = RR_singleRotation(lpCurrent);
				if(iFlag == HeightChanged)
					lpCurrent->ibf++;
				
				return lpTemp;
			}
		}
        
		return lpCurrent;
        
	}
    
	else
	{
		if(lpCurrent->rchild == NULL)
			return lpCurrent;
        
		int iOldBf = lpCurrent->rchild->ibf;
		lpCurrent->rchild = Remove(lpCurrent->rchild, key, lpWaste, iFlag);
        
		KK_Node* lpRightChild = lpCurrent->rchild;
        
		if (lpRightChild == NULL)
			lpCurrent->ibf--;
		else if((lpRightChild->ibf != iOldBf) && (lpRightChild->ibf == 0))
			lpCurrent->ibf--;
        
		if (lpCurrent->ibf < -1)
		{
			int iNewOldBf = lpCurrent->lchild->ibf;
			if (iNewOldBf > 0)
				return LR_doubleRotation(lpCurrent);
			else
			{
				KK_Node *lpTemp = LL_singleRotation(lpCurrent);
				if(iFlag == HeightChanged)
					lpCurrent->ibf--;
                
				return lpTemp;
			}
		}
        
		return lpCurrent;
	}
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key) const
{
	KK_Node* lpTemp = (KK_Node*)m_lpRoot;
	
	while (lpTemp != NULL)
	{
		KEY* pkey = &lpTemp->key;
		if (KK_CompareElements(pkey, &key) == 0)
			return true;
		else if (KK_CompareElements(pkey, &key) > 0)
		{
			lpTemp = lpTemp->lchild;
		}
		else
		{
			lpTemp = lpTemp->rchild;
		}
	}
	
	return false;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& value) const
{
	KK_Node* lpTemp = (KK_Node*)m_lpRoot;
	
	while (lpTemp != NULL)
	{
		KEY* pkey = &lpTemp->key;
		if (KK_CompareElements(pkey, &key) == 0)
		{
			value = lpTemp->value;
			return true;
		}
		else if (KK_CompareElements(pkey, &key) > 0)
		{
			lpTemp = lpTemp->lchild;
		}
		else
		{
			lpTemp = lpTemp->rchild;
		}
	}
	
	return false;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>:: LL_singleRotation(KK_Node* lpCurrent)
{
	KK_Node* lpBNode;
	lpBNode =lpCurrent->lchild;
	lpCurrent->lchild = lpBNode->rchild;
    //lpCurrent->ibf = 0;
	lpCurrent->ibf = GetTreeHeight(lpCurrent->rchild) - GetTreeHeight(lpCurrent->lchild);  
	lpBNode->rchild =lpCurrent;
	
	if(lpBNode->ibf == 0)
		lpBNode->ibf = 1;
	
	else
		lpBNode->ibf = 0;
	
	return lpBNode;
	
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>:: RR_singleRotation(KK_Node* lpCurrent)
{
	KK_Node* lpBNode;
	lpBNode =lpCurrent->rchild;
	lpCurrent->rchild = lpBNode->lchild;
	//lpCurrent->ibf = 0;
    lpCurrent->ibf = GetTreeHeight(lpCurrent->rchild) - GetTreeHeight(lpCurrent->lchild);  
	lpBNode->lchild =lpCurrent;
	
	if(lpBNode->ibf == 0)
		lpBNode->ibf;
	else
		lpBNode->ibf = 0;
	
	return lpBNode;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>:: RL_doubleRotation(KK_Node* lpCurrent)
{
	KK_Node* lpCNode, *lpBNode;
	lpBNode = lpCurrent->rchild;
	lpCNode = lpBNode->lchild;
	lpBNode->lchild = lpCNode->rchild;
	lpCNode->rchild = lpBNode;
	lpCurrent->ibf = lpBNode->ibf = 0;
	
	if(lpCNode->ibf == -1)
		lpBNode->ibf = 1;
	
	if(lpCNode->ibf == 1)
		lpCurrent->ibf = -1;
	
	lpCurrent->rchild = lpCNode->lchild;
	lpCNode->lchild =lpCurrent;
	lpCNode->ibf =0;
	
	return lpCNode;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>::KK_Node* 
KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE>:: LR_doubleRotation(KK_Node* lpCurrent)
{
	KK_Node* lpCNode, *lpBNode;
	lpBNode = lpCurrent->lchild;
	lpCNode = lpBNode->rchild;
	lpBNode->rchild = lpCNode->lchild;
	lpCNode->lchild = lpBNode;
	lpCurrent->ibf = lpBNode->ibf = 0;
	
	if(lpCNode->ibf == -1)
		lpCurrent->ibf = 1;
	
	if(lpCNode->ibf == 1)
		lpBNode->ibf = -1;
	
	lpCurrent->lchild = lpCNode->rchild;
	lpCNode->rchild =lpCurrent;
	lpCNode->ibf =0;
	
	return lpCNode;
}


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_ByteStream& operator >> (KK_ByteStream & is, KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE> & tree)
{
	int nCount = is.ReadLength();
	while (nCount--)
	{
		KEY newKey;
		VALUE newValue;
		is >> newKey;
		is >> newValue;
		tree.SetAt(newKey, newValue);
	}
	return is;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
KK_ByteStream& operator << (KK_ByteStream & os, const KK_AvlTree<KEY, ARG_KEY, VALUE, ARG_VALUE> & tree)
{
	int nCount = tree.GetCount();
	os.WriteLength(nCount);
	for (KK_POSITION pos=tree.GetHeadPosition(); pos!=NULL; )
	{
		KEY key;
		VALUE value;
		tree.GetNext(pos, key, value);
		os << key;
		os << value;
	}
	return os;
}




class KK_ByteArray : public KK_Array<u_char, u_char> , public KK_StreamObject
{
public:
	static void GetString(u_char * lp, int len, KK_StringA & str, int radix = 16) {
		char buf[32];
		str.Empty();
		for (int i = 0; i < len; i++) {
			if (i > 0) str += ' ';
            sprintf(buf, "%d", lp[i]);
            //itoa(lp[i], buf, radix);
			str += buf;
		}
	}
    
    
public:
	inline void SetData(const u_char * data, int len) {
		SetSize(len);
		if (len > 0) memcpy(m_pData, data, len);
	}
    
	inline void GetString(KK_StringA & str, int radix = 16) { GetString(m_pData, GetSize(), str, radix); }
    
    
	inline KK_StringA GetStringA(int radix = 16) { KK_StringA str; GetString(str, radix); return str; }
   
	virtual void SerializeFrom(KK_ByteStream& is) {
		u_long len = is.ReadLength();
		SetSize(len);
		if (len > 0) is.ReadBytes(m_pData, len);
	}
	virtual void SerializeTo(KK_ByteStream& os) const {
		os.WriteLength(GetSize());
		if (GetSize() > 0) os.WriteBytes(m_pData, GetSize());
	}
};



#endif //__KK_TEMPL_H__


