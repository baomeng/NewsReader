//
//  KK_TempletEx.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//


#ifndef __KK_TEMPL_EX_H__
#define __KK_TEMPL_EX_H__


#include "KK_Templet.h"
#include "KK_Locker.h"

template<> inline unsigned int KK_HashKey(const sockaddr_in& key)
{ return ((key.sin_port << 16) + key.sin_port) ^ key.sin_addr.s_addr; }
template<> inline int KK_CompareElements(const sockaddr_in* pElement1, const sockaddr_in* pElement2)
{ if (pElement1->sin_port == pElement2->sin_port && pElement1->sin_addr.s_addr == pElement2->sin_addr.s_addr) return 0;
    else return -1; }



template<class TYPE, class ARG_TYPE>
inline bool KK_ArrayAddUnique(KK_Array<TYPE, ARG_TYPE> & array, ARG_TYPE value)
{
	int i;
	for (i = array.GetSize() - 1; i >= 0; i--) {
		if (array[i] == value) return false;
	}
	array.Add(value);
	return true;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ArrayRemoveUnique(KK_Array<TYPE, ARG_TYPE> & array, ARG_TYPE value)
{
	int i;
	for (i = array.GetSize() - 1; i >= 0; i--) {
		if (array[i] == value) {
			array.RemoveAt(i);
			return true;
		}
	}
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ArrayExistUnique(KK_Array<TYPE, ARG_TYPE> & array, ARG_TYPE value)
{
	int i;
	for (i = array.GetSize() - 1; i >= 0; i--) {
		if (array[i] == value) return true;
	}
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ListAddUnique(KK_List<TYPE, ARG_TYPE> & list, ARG_TYPE value)
{
	KK_POSITION pos;
	for (pos = list.GetHeadPosition(); pos != NULL;) {
		if (list.GetNext(pos) == value) return false;
	}
	list.AddTail(value);
	return true;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ListRemoveUnique(KK_List<TYPE, ARG_TYPE> & list, ARG_TYPE value)
{
	KK_POSITION pos, prev;
	for (pos = list.GetHeadPosition(); pos != NULL;) {
		prev = pos;
		if (list.GetNext(pos) == value) {
			list.RemoveAt(prev);
			return true;
		}
	}
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ListExistUnique(KK_List<TYPE, ARG_TYPE> & list, ARG_TYPE value)
{
	KK_POSITION pos;
	for (pos = list.GetHeadPosition(); pos != NULL;) {
		if (list.GetNext(pos) == value) return true;
	}
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ArrayAddUnique(KK_LckArray<TYPE, ARG_TYPE> & array, ARG_TYPE value, bool bLock = true)
{
	if (bLock) array.Lock();
	int i;
	for (i = array.GetSize() - 1; i >= 0; i--) {
		if (array[i] == value) {
			if (bLock) array.Unlock();
			return false;
		}
	}
	array.Add(value);
	if (bLock) array.Unlock();
	return true;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ArrayRemoveUnique(KK_LckArray<TYPE, ARG_TYPE> & array, ARG_TYPE value, bool bLock = true)
{
	if (bLock) array.Lock();
	int i;
	for (i = array.GetSize() - 1; i >= 0; i--) {
		if (array[i] == value) {
			array.RemoveAt(i);
			if (bLock) array.Unlock();
			return true;
		}
	}
	if (bLock) array.Unlock();
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ArrayExistUnique(KK_LckArray<TYPE, ARG_TYPE> & array, ARG_TYPE value, bool bLock = true)
{
	if (bLock) array.Lock();
	int i;
	for (i = array.GetSize() - 1; i >= 0; i--) {
		if (array[i] == value) {
			if (bLock) array.Unlock();
			return true;
		}
	}
	if (bLock) array.Unlock();
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ListAddUnique(KK_LckList<TYPE, ARG_TYPE> & list, ARG_TYPE value, bool bLock = true)
{
	if (bLock) list.Lock();
	KK_POSITION pos;
	for (pos = list.GetHeadPosition(); pos != NULL;) {
		if (list.GetNext(pos) == value) {
			if (bLock) list.Unlock();
			return false;
		}
	}
	list.AddTail(value);
	if (bLock) list.Unlock();
	return true;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ListRemoveUnique(KK_LckList<TYPE, ARG_TYPE> & list, ARG_TYPE value, bool bLock = true)
{
	KK_POSITION pos, prev;
	if (bLock) list.Lock();
	for (pos = list.GetHeadPosition(); pos != NULL;) {
		prev = pos;
		if (list.GetNext(pos) == value) {
			list.RemoveAt(prev);
			if (bLock) list.Unlock();
			return true;
		}
	}
	if (bLock) list.Unlock();
	return false;
}

template<class TYPE, class ARG_TYPE>
inline bool KK_ListExistUnique(KK_LckList<TYPE, ARG_TYPE> & list, ARG_TYPE value, bool bLock = true)
{
	if (bLock) list.Lock();
	KK_POSITION pos;
	for (pos = list.GetHeadPosition(); pos != NULL;) {
		if (list.GetNext(pos) == value) {
			if (bLock) list.Unlock();
			return true;
		}
	}
	if (bLock) list.Unlock();
	return false;
}



class KK_PtrList : public KK_List<void*, void*> {};
class KK_PtrArray : public KK_Array<void*, void*> {};
class KK_ULongList : public KK_List<u_long, u_long> {};
class KK_ULongArray : public KK_Array<u_long, u_long> {};

class KK_LckPtrList : public KK_LckList<void*, void*> {};
class KK_LckPtrArray : public KK_LckArray<void*, void*> {};
class KK_LckULongList : public KK_LckList<u_long, u_long> {};
class KK_LckULongArray : public KK_LckArray<u_long, u_long> {};


class KK_MapName2Value : public KK_Map<KK_String, const KK_String&, KK_String, const KK_String&> {};
class KK_MapName2ValueW : public KK_Map<KK_StringW, const KK_StringW&, KK_StringW, const KK_StringW&> {};
class KK_MapName2ValueA : public KK_Map<KK_StringA, const KK_StringA&, KK_StringA, const KK_StringA&> {};

class KK_MapAddr2Socket : public KK_Map<sockaddr_in, const sockaddr_in&, SOCKET, SOCKET> {};

typedef KK_Map<u_long, u_long, void*, void*> KK_MapULong2Ptr;


#endif // __KK_TEMPL_EX_H__
