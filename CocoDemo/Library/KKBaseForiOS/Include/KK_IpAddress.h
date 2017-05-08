//
//  KK_IpAddress.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_IPADDRESS_H__
#define __KK_IPADDRESS_H__

#include "KK_Includes.h"
#include "KK_String.h"


class KK_IpAddr : public sockaddr_in
{
public:
	inline KK_IpAddr()
	{
		sin_family = AF_INET;
		memset(sin_zero, 0, sizeof(sin_zero));
	}
    
	inline KK_IpAddr(const struct sockaddr_in* lpAddr)
	{
		Set(lpAddr);
	}
    
	inline KK_IpAddr(const char* szAddr) // "127.0.0.1:80"
	{ 
		sin_family = AF_INET;
		memset(sin_zero, 0, sizeof(sin_zero));
	    Set(szAddr);
	}
    
	inline KK_IpAddr(const wchar_t* szAddr) // L"127.0.0.1:80"
	{
		sin_family = AF_INET;
		memset(sin_zero, 0, sizeof(sin_zero));
	    Set(szAddr);
	}
    
	inline KK_IpAddr(const char* szIp, u_short nPort) // "127.0.0.1"
	{
        sin_family = AF_INET;
		memset(sin_zero, 0, sizeof(sin_zero));
		Set(szIp, nPort);
	}
    
	inline KK_IpAddr(const wchar_t* szIp, u_short nPort) // L"127.0.0.1"
	{
        sin_family = AF_INET;
		memset(sin_zero, 0, sizeof(sin_zero));
		Set(szIp, nPort);
	}
    
	inline KK_IpAddr(u_long ulIp, u_short nPort)
	{
        sin_family = AF_INET;
		memset(sin_zero, 0, sizeof(sin_zero));
		sin_addr.s_addr = ulIp;
		sin_port             = htons(nPort);
	}
    
	inline ~KK_IpAddr() {}
    
public:
	inline void Set(const struct sockaddr_in* lpAddr)
	{
		sin_family = lpAddr->sin_family;
		sin_port   = lpAddr->sin_port;
		memset(sin_zero, 0, sizeof(sin_zero));
		sin_addr.s_addr = lpAddr->sin_addr.s_addr; 
	}
    
	inline void Set(const char* szAddr)
	{
		KK_StringA strAddr = szAddr;
		int nPos = strAddr.Find(':');
		if (nPos == -1) nPos = strAddr.GetLength();
		
		KK_StringA strIp = strAddr.Mid(0, nPos);
		KK_StringA strPort = strAddr.Mid(nPos+1, strAddr.GetLength()-nPos-1);
        
		sin_addr.s_addr = inet_addr(strIp);
	    sin_port = htons(atoi(strPort));
	}
    
	inline void Set(const wchar_t* szAddr)
	{
        KK_StringA strAddr = szAddr;
		Set(strAddr);
	}
    
	inline void Set(const char* szIp, u_short nPort)
	{
		sin_addr.s_addr = inet_addr(szIp);
        sin_port = htons(nPort);
	}
	
	inline void Set(const wchar_t* szIp, u_short nPort)
	{
        KK_StringA strIp = szIp;
        Set(strIp, nPort);
	}
    
	inline void Set(u_long ulIp, u_short nPort)
	{
		sin_addr.s_addr = ulIp;
		sin_port = htons(nPort);
	}
    
	inline void SetIp(const char* szIp)
	{
		sin_addr.s_addr = inet_addr(szIp);
	}
    
	inline void SetIp(const wchar_t* szIp)
	{
		KK_StringA strIp = szIp;
		Set(strIp);
	}
    
	inline void SetIp(u_long ulIp)
	{
        sin_addr.s_addr = ulIp;
	}
    
	inline void SetPort(u_short nPort)
	{
		sin_port = htons(nPort);
	}
    
	inline u_long GetIp() const //返回网络字节形式下的无符号长整形
	{
		return sin_addr.s_addr;
	}
    
	inline u_short GetPort() const
	{
		return ntohs(sin_port);
	}
    
	inline KK_String GetAddrString() const
	{
		return (KK_String)GetAddrStringA();
	}
    
	inline KK_StringA GetAddrStringA() const
	{
		KK_StringA strRet;
		strRet.Format("%s:%d", inet_ntoa(sin_addr), ntohs(sin_port));
		return strRet;
	}
    
	inline KK_StringW GetAddrStringW() const
	{
		return (KK_StringW)GetAddrStringA();
	}
    
	inline KK_StringU GetAddrStringU() const
	{
		return (KK_StringU)GetAddrStringA();
	}
    
	inline KK_StringWA GetAddrStringWA() const
	{
		return (KK_StringWA)GetAddrStringA();
	}
    
	inline void GetAddrString(KK_StringA& str) const
	{
		str.Format("%s:%d", inet_ntoa(sin_addr), ntohs(sin_port));
	}
    
	inline void GetAddrString(KK_StringW& str) const
	{
		KK_StringA strTemp;
		GetAddrString(strTemp);
		str = strTemp;
	}
    
	inline void GetAddrString(KK_StringU& str) const
	{
		KK_StringA strTemp;
		GetAddrString(strTemp);
		str = strTemp;
	}
    
	inline KK_String GetIpString() const
	{
		return (KK_String)GetIpStringA();
	}
    
	inline KK_StringA GetIpStringA() const
	{
		KK_StringA strRet = inet_ntoa(sin_addr);
		return strRet;
	}
	
	inline KK_StringW GetIpStringW() const
	{
		return (KK_StringW)GetIpStringA();
	}
    
	inline KK_StringU GetIpStringU() const
	{
		return (KK_StringU)GetIpStringA();
	}
    
	inline KK_StringWA GetIpStringWA() const
	{
		return (KK_StringWA)GetIpStringA();
	}
	
	inline void GetIpString(KK_StringA& strIp) const
	{
		strIp = inet_ntoa(sin_addr);
	}
	
	inline void GetIpString(KK_StringW& strIp) const
	{
		strIp = inet_ntoa(sin_addr);
	}
	
	inline void GetIpString(KK_StringU& strIp) const
	{
        strIp = inet_ntoa(sin_addr);		 
	}
    
	operator sockaddr_in* ()
	{	
		return (sockaddr_in*)this;
	}
    
	operator struct sockaddr* ()
	{
		return (struct sockaddr*)(sockaddr_in*)this;
	}
    
};



#endif // 
