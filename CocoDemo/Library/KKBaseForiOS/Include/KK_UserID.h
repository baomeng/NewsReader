//
//  KK_UserID.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __KK_USERID_H__
#define __KK_USERID_H__



#include "KK_Includes.h"
#include "KK_ByteStream.h"
#include "KK_String.h"
#include "KK_Templet.h"
#include "KK_BaseFunctions.h"


#define INVALID_USER_ID				____T("Invalid ID")

#define INVALID_USER_ID_MBCS		"Invalid ID"
#define INVALID_USER_ID_UNICODE		L"Invalid ID"


enum KK_USER_ID_TYPE 
{
	UID_INVALID = 0,
	UID_PUBLIC,
	UID_ENTERPRISE,
	UID_GUEST
};




class KK_UserID : public KK_StreamObject
{
public:
	KK_UserID();
	KK_UserID(const KK_UserID& iduser);
	KK_UserID(u_long h, u_long l);
	virtual ~KK_UserID();
    
	static KK_USER_ID_TYPE GetUserIDByString(const KK_StringA& strHighID, KK_StringA strID, KK_UserID& iduser);
	static KK_USER_ID_TYPE GetUserIDByString(const KK_StringW& strHighID, const KK_StringW& strID, KK_UserID& iduser);
	static KK_USER_ID_TYPE GetUserIDByString(u_long ulHighID, const KK_StringA& strID, KK_UserID& iduser);
	static KK_USER_ID_TYPE GetUserIDByString(u_long ulHighID, const KK_StringW& strID, KK_UserID& iduser);
	static bool ParseUserIdsString(u_long ulHighID, KK_StringA& strUserID, KK_Array<KK_UserID, const KK_UserID&>& arUserID, int nMaxCount = 100);
    
public:
	virtual void SerializeFrom(KK_ByteStream& is);
	virtual void SerializeTo(KK_ByteStream& os) const;
    
public:
	static bool IsValidEnterpriseLow(u_long ulHigh, u_long ulLow);
	static u_long GetNextEnterpriseLow(u_long ulLow);
	static bool IsValidEnterpriseID(u_long ulHigh);
    
    bool IsValid() const;
    
	inline u_long HashKey() const{ return m_ulLow^m_ulHigh; }
    
	KK_USER_ID_TYPE GetIDType() const;

	KK_StringA GetKKStringA() const;
	KK_StringW GetKKStringW() const;
	KK_StringU GetKKStringU() const;
	KK_StringWA GetKKStringWA() const;
	void GetString(KK_StringA& str) const;
	void GetString(KK_StringW& str) const;
	void GetString(KK_StringU& str) const;
    
	KK_USER_ID_TYPE Set(const char* str);
	KK_USER_ID_TYPE Set(const wchar_t* str);
    
    
public:
	inline u_long GetHigh() const { return m_ulHigh; }
	inline u_long GetLow() const { return m_ulLow; }
    
	inline void Set(const KK_UserID& iduser) { m_ulHigh = iduser.m_ulHigh; m_ulLow = iduser.m_ulLow; }
	inline void Set(u_long h, u_long l) { m_ulHigh = h; m_ulLow = l; }
	inline void Set(const char* h, const char* l) { m_ulHigh = atol(h); m_ulLow = atol(l); }
	inline void Set(const wchar_t* h, const wchar_t* l) { m_ulHigh = wtol(h); m_ulLow = wtol(l); }
    
	inline void SetHigh(u_long h) { m_ulHigh = h; }
	inline void SetLow(u_long l) { m_ulLow = l; }
    
	inline const KK_UserID& operator = (const KK_UserID& iduser) { Set(iduser); return *this; }
	inline bool operator == (const KK_UserID& iduser) const { return iduser.m_ulLow == m_ulLow && iduser.m_ulHigh == m_ulHigh; }
	inline bool operator != (const KK_UserID& iduser) const { return iduser.m_ulLow != m_ulLow || iduser.m_ulHigh != m_ulHigh; }
	inline bool operator > (const KK_UserID& iduser) const { return m_ulHigh > iduser.m_ulHigh || (m_ulHigh == iduser.m_ulHigh && m_ulLow > iduser.m_ulLow); }
	inline bool operator < (const KK_UserID& iduser) const { return m_ulHigh < iduser.m_ulHigh || (m_ulHigh == iduser.m_ulHigh && m_ulLow < iduser.m_ulLow); }
    
	inline bool operator <= (const KK_UserID& iduser) const { return (*this < iduser || *this == iduser); }
    
	inline bool operator >= (const KK_UserID& iduser) const { return (*this > iduser || *this == iduser); }
    
    operator u_longlong() const {return ((u_longlong)m_ulHigh << 32) | (u_longlong)m_ulLow;}   

	void Reset() { m_ulLow = 0; m_ulHigh = 0; }
    
//    inline friend int KK_CompareElements(const KK_UserID* p1, const KK_UserID* p2) 
//    {
//        if(*p1 == *p2) return 0;
//        else if(*p1 < *p2) return -1;
//        else return 1;
//    }
 	
protected:
    u_long m_ulLow, m_ulHigh;
};


//template<> inline u_int KK_HashKey(const KK_UserID & key)
//{
//    u_int nHash = key.GetHigh() ^ key.GetLow();
//    return nHash;
//}

template<> inline int KK_CompareElements(const KK_UserID* p1, const KK_UserID* p2) 
{
    if(*p1 == *p2) return 0;
    else if(*p1 < *p2) return -1;
    else return 1;
}


typedef KK_List<KK_UserID, const KK_UserID&> KK_UserIDList;
typedef KK_Array<KK_UserID, const KK_UserID&> KK_UserIDArray;

#endif // __KK_USERID_H__

