//
//  KK_TcpSocket.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __KK_TcpSocket_H__
#define __KK_TcpSocket_H__

#include "KK_Includes.h"

class KK_TcpSocket  
{
public:
	KK_TcpSocket(u_short port = 0, const char *addr = "", u_short range = 1);
	virtual ~KK_TcpSocket();
    
public:
    
	inline int GetLastError() {
		return errno;
	}
    
	u_short GetPort();
	const char *GetAddr();
	int SetOption(int level, int optname, char *optval = NULL, int optlen = 0);
	int GetOption(int level, int optname, char *optval = NULL, int optlen = 0);
    
	int Close();
	int Wait(u_long msec);
	int Create();
	int Bind();
	int Bind(u_short port, const char *address);
	int Bind(u_short port, u_long address = 0);
    
	bool Connect(u_long host, u_short port);
	bool Connect(const char *host, u_short port);
	int Send(const void *buf, int nbytes);
	int Receive(void *buf, int nbytes);
	int Receive(void *buf, int nbytes, u_long timeout);
    
	int Listen(int backlog);
	bool Accept(KK_TcpSocket & s, sockaddr* addr = NULL, int* addr_len = NULL); 
    
	inline bool IsValid() { return m_socket != INVALID_SOCKET; }
	inline SOCKET GetSocket() { return m_socket; }
    
protected:
    
	u_short      m_port;    // port to bind to
	u_short      m_range;   // number of ports to try to bind to
	u_long       m_addr;    // address
	SOCKET       m_socket;  // socket descriptor
};

#endif