//
//  KK_Plex.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __KK_PLEX_H__
#define __KK_PLEX_H__

#include "KK_Includes.h"

struct _KK_POSITION { };
typedef _KK_POSITION* KK_POSITION;

#define KK_BEFORE_START_POSITION ((KK_POSITION)-1L)


struct KK_Plex     // warning variable length structure
{
	KK_Plex* pNext;
    
    
	void* data() { return this + 1; }
	
	static KK_Plex* Create(KK_Plex*& head, unsigned int nMax, unsigned int cbElement);
	
	void FreeDataChain();       // free this one and links
};



#endif //__KK_PLEX_H__