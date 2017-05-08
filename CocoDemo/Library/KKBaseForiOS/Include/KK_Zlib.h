//
//  KK_Zlib.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-4.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_ZLIB_H__
#define __KK_ZLIB_H__

unsigned long zipCompress(unsigned char** dest, const unsigned char* source, unsigned long sourcelen);
unsigned long zipUnCompress(unsigned char* dest, unsigned long* destlen, const unsigned char* source, unsigned long sourcelen);

#endif //__KK_ZLIB_H__