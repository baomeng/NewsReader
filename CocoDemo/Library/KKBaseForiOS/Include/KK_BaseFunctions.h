//
//  KK_BaseFunctions.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_BASEFUNCTIONS_H__
#define __KK_BASEFUNCTIONS_H__

#include "KK_Includes.h"
#include <locale.h>

#define FC6_LINUX
class KK_BaseFirstRun
{
public:
	KK_BaseFirstRun();
};

//static KK_BaseFirstRun kk_basefirstrun;

char* strrev(char* szrev);
wchar_t* wcsrev(wchar_t* wszrev);

void CharUpperA(char* lpsz);
void CharLowerA(char* lpsz);
char* CharNextA(const char* lpsz);
int lstrlenA(const char* lpString);

void CharUpperW(wchar_t* lpwsz);
void CharLowerW(wchar_t* lpwsz);
wchar_t* CharNextW(const wchar_t* lpwsz);
int lstrlenW(const wchar_t* lpString);


#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations
#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation

#define MB_PRECOMPOSED            0x00000001  // use precomposed chars
#define MB_COMPOSITE              0x00000002  // use composite chars
#define MB_USEGLYPHCHARS          0x00000004  // use glyph chars, not ctrl chars
#define MB_ERR_INVALID_CHARS      0x00000008  // error for invalid chars

#define WC_COMPOSITECHECK         0x00000200  // convert composite to precomposed
#define WC_DISCARDNS              0x00000010  // discard non-spacing chars
#define WC_SEPCHARS               0x00000020  // generate separate chars
#define WC_DEFAULTCHAR            0x00000040  // replace w/ default char
#define WC_NO_BEST_FIT_CHARS      0x00000400  // do not use best fit chars


BOOL IsBadStringPtrW(LPCWSTR, UINT);
BOOL IsBadStringPtrA(LPCSTR, UINT);

LONG InterlockedIncrement(
                          LPLONG lpAddend   // pointer to the variable to increment
                          );
LONG InterlockedDecrement(
                          LPLONG lpAddend   // address of the variable to decrement
                          );

DWORD GetPrivateProfileString(
                              LPCTSTR lpAppName,        // points to section name
                              LPCTSTR lpKeyName,        // points to key name
                              LPCTSTR lpDefault,        // points to default string
                              LPTSTR lpReturnedString,  // points to destination buffer
                              DWORD nSize,              // size of destination buffer
                              LPCTSTR lpFileName        // points to initialization filename
                              );

BOOL WritePrivateProfileString(
                               LPCTSTR lpAppName,  // pointer to section name
                               LPCTSTR lpKeyName,  // pointer to key name
                               LPCTSTR lpString,   // pointer to string to add
                               LPCTSTR lpFileName  // pointer to initialization filename
                               );


static void __cdecl xtoa (unsigned long val, char *buf, unsigned radix, int is_neg);
char* __cdecl _itoa (int val, char *buf, int radix);
char* __cdecl _ltoa (long val, char *buf, int radix);
char* __cdecl _ultoa (unsigned long val, char *buf, int radix);
static void __stdcall x64toa (unsigned __int64 val, char *buf, unsigned radix, int is_neg);
char* __cdecl _i64toa (__int64 val, char *buf, int radix);
char* __cdecl _ui64toa (unsigned __int64 val, char *buf, int radix);

int  atexit(void (*)(void));

#ifndef FC6_LINUX
int atoi(const char *);
long atol(const char *);
#endif
__int64 _atoi64(const char *nptr);

int wtoi(const WCHAR *);
long wtol(const WCHAR *);
__int64 _wtoi64(const WCHAR *nptr);

wchar_t* __cdecl _itow (int val, wchar_t *buf, int radix);
wchar_t* __cdecl _ltow (long val, wchar_t *buf, int radix);
wchar_t* __cdecl _ultow (unsigned long val, wchar_t *buf, int radix);
wchar_t* __cdecl _i64tow (__int64 val,wchar_t *buf, int radix);
wchar_t* __cdecl _ui64tow (unsigned __int64 val, wchar_t *buf, int radix);
char* __cdecl _strdup (const char* string);
wchar_t* __cdecl _wcsdup (const wchar_t* string);

#ifdef UNICODE
#define _tcsdup     _wcsdup
#else	
#define _tcsdup     _strdup
#endif


#endif