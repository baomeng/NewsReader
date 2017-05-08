//
//  KK_Base64Codec.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_BASE64CODEC_H__
#define __KK_BASE64CODEC_H__

#include "KK_Includes.h"

int Base64EncodeString(const char* szEncoding, char* szEnOut, int nEncodLen, bool bAddCrLf);
int Base64DecodeString(const char*  szDecoding, char* szOutput,int nDecordLen, bool bDelCrLf);

#endif //__KK_BASE64CODEC_H__