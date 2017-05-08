//
//  KK_UdpSocket.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __KK_UDPSOCKET_H__
#define __KK_UDPSOCKET_H__

#include "KK_Includes.h"

#define SOCKET_RECV_TIME_OUT 0x7fffffff

class KK_UdpSocket  
{
public:
	KK_UdpSocket(u_short port = 0, const char *addr = "", u_short range = 1);
	virtual ~KK_UdpSocket();
    
public:
	inline SOCKET GetSocket() { return m_socket; }
    
	inline int GetLastError() 
	{
		return errno;
	}
	u_short GetPort();
	const char *GetAddr();
	int SetOption(int level, int optname, char *optval = NULL, int optlen = 0);
	int GetOption(int level, int optname, char *optval = NULL, int optlen = 0);
    
	int Close();
	int Wait(u_long msec); // millisecond
	int Create();
	int Bind();
	int Bind(u_short port, const char *address);
	int Bind(u_short port, u_long address = 0);
    
	int Send(const void *buf, int nbytes, u_long addr, u_short port);
	int Send(const void *buf, int nbytes, const char *addr, u_short port);
	int Send(const void *buf, int nbytes, struct sockaddr_in & saddr);
	
	int Receive(void *buf, int nbytes, u_long &addr, u_short &port);
	int Receive(void *buf, int nbytes, u_long &addr, u_short &port, u_long timeout); // millisecond
	int Receive(void *buf, int nbytes, struct sockaddr_in & saddr);
	int Receive(void *buf, int nbytes, struct sockaddr_in & saddr, u_long timeout); // millisecond
    
	bool IsValid() { return m_socket != INVALID_SOCKET; }
	
protected:
	u_short      m_port;    // port to bind to
	u_short      m_range;   // number of ports to try to bind to
	u_long       m_addr;    // address
	SOCKET       m_socket;  // socket descriptor
    
};


#endif // __KK_UDPSOCKET_H__
