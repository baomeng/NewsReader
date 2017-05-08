//
//  KK_String.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_STRING_H__
#define __KK_STRING_H__

//#define _UNICODE

#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include "KK_StringA.h"
#include "KK_StringW.h"
#include "KK_StringU.h"

typedef KK_StringU KK_String;

#define _StrUtf8

//#define _SEVER
//
//#if defined(_SEVER) || defined(_SERVER)
//    #if defined(_UNICODE) || defined(UNICODE)
//        typedef KK_StringU KK_String;
//        #define _StrUtf8
//    #else		
//        typedef KK_StringA KK_String;
//        #define _StrAnsi
//    #endif
//#else
//    #if defined(_UNICODE) || defined(UNICODE)
//        typedef KK_StringW KK_String;
//        #define _StrUnic
//    #else
//        typedef KK_StringA KK_String;
//        #define _StrAnsi
//    #endif
//#endif

#if defined(_UNICODE) || defined(UNICODE)
typedef KK_StringW KK_StringWA;
#else
typedef KK_StringA KK_StringWA;
#endif


inline KK_StringA::KK_StringA(const KK_StringW& stringSrc)
{
	Init();
	*this = (LPCWSTR)stringSrc;
}

inline KK_StringA::KK_StringA(const KK_StringU& stringSrc)
{
	Init();
	KK_StringW strTemp;
	strTemp.SetUtf8String(stringSrc);
	*this = strTemp;
}

inline const KK_StringA& KK_StringA::operator=(const KK_StringW& stringSrc)
{
	*this = (LPCWSTR)stringSrc;
    return *this;
}

inline const KK_StringA& KK_StringA::operator=(const KK_StringU& stringSrc)
{
	KK_StringW strTemp;
	strTemp.SetUtf8String(stringSrc);
	*this = strTemp;
    
	return *this;
}

inline KK_StringW::KK_StringW(const KK_StringA& stringSrc)
{	
	Init();
	*this = (LPCSTR)stringSrc;
}

inline KK_StringW::KK_StringW(const KK_StringU& stringSrc)
{
	Init();
	this->SetUtf8String(stringSrc);
}

inline const KK_StringW& KK_StringW::operator=(const KK_StringA& stringSrc)
{
	*this = (LPCSTR)stringSrc;
    return *this;
}

inline const KK_StringW& KK_StringW::operator=(const KK_StringU& stringSrc)
{
	this->SetUtf8String(stringSrc);
    return *this;
}

inline KK_StringU::KK_StringU(const KK_StringA& stringSrc)
{
	Init();
	KK_StringW strTemp = stringSrc;
	*this = strTemp;
}

inline KK_StringU::KK_StringU(const KK_StringW& stringSrc)
{
	Init();
	*this = (LPCWSTR)stringSrc;
}

inline const KK_StringU& KK_StringU::operator=(const KK_StringA& stringSrc)
{
	KK_StringW strTemp = stringSrc;
	*this = strTemp;
    return *this;
}

inline const KK_StringU& KK_StringU::operator=(const KK_StringW& stringSrc)
{
	*this = (LPCWSTR)stringSrc;
    return *this;
}

#endif //__KK_STRING_H__