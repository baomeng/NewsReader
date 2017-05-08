//
//  KK_Utf8_Unicode.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_UTF8_UNICODE_H__
#define __KK_UTF8_UNICODE_H__

#include "KK_Includes.h"
#include "KK_BaseFunctions.h"


void Unicode2Utf8(const wchar_t *unicode, const int unicode_len, unsigned char *utf8, int &utf8_len);
void Utf82Unicode(wchar_t *unicode, int &unicode_len, const unsigned char *utf8, const int utf8_len);
void Mbcs2Utf8(const char *mbcs, const int mbcs_len, unsigned char *utf8, int &utf8_len);
void Utf82Mbcs(char *mbcs, int &mbcs_len, const unsigned char *utf8, const int utf8_len);

#ifdef _UNICODE
#define TString2Utf8 Unicode2Utf8
#define Utf82TString Utf82Unicode
#else
#define TString2Utf8 Mbcs2Utf8
#define Utf82TString Utf82Mbcs
#endif

#endif
