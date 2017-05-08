//
//  KK_Includes.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//


#ifndef __KK_INCLUDES_H__
#define __KK_INCLUDES_H__


#include <Foundation/Foundation.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <wchar.h>

#define KK_Assert(exp)      assert(exp)
#define _alloca             alloca

typedef long long               longlong;

typedef unsigned char           u_char;
typedef unsigned long           u_long;
typedef unsigned int            u_int;
typedef unsigned short          u_short;
typedef unsigned long long      u_longlong;

typedef unsigned char           uchar;
typedef unsigned long           ulong;
typedef unsigned short          ushort;
typedef unsigned long long      ulonglong;


#define KTraceA					printf
#define KTraceW					wprintf
#define KTrace					_tprintf
#define KK_Trace				theLog.Print

#define Min(a, b)               (((a) < (b)) ? (a): (b))
#define Max(a, b)               (((a) > (b)) ? (a): (b))
#define Abs(v)                  (((v) > 0) ? (v): -(v))
#define ZeroData(d)             memset((&d), 0, sizeof(d))
#define CountOf(t)              (sizeof(t) / sizeof((t)[0]))


#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#define SAFE_FREE(x) { if (x) free(x); x = NULL; }
#define SAFE_DELETE(x) { if (x) delete x; x = NULL; }
#define SAFE_ARRAY_DELETE(x) { if (x) delete [] x; x = NULL; }

#define INFINITE                    0xFFFFFFFF

//file
#define INVALID_HANDLE_VALUE        ((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_SIZE           ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER    ((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES     ((DWORD)-1)

#define MAX_PATH			260

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

#define SEEK_SET            0               /* seek to an absolute position */
#define SEEK_CUR            1               /* seek relative to current position */
#define SEEK_END            2               /* seek relative to end of file */


#define VOID void
#define CONST const
#define __int64 long long
#define TRUE 1
#define FALSE 0

#define __cdecl 
#define PASCAL
#define __stdcall


#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))


#if defined(_UNICODE) || defined(UNICODE)
#define _T(x) L##x
#define T(x) L##x
#else
#define _T(x) x
#define T(x) x
#endif

#define ZeroMemory(Destination,Length) memset((Destination), 0, (Length))

typedef int SOCKET;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define INT_SIZE_LENGTH 20
#define LONG_SIZE_LENGTH 40
#define I64_SIZE_LENGTH 80

#ifdef UNICODE
#define lstrcpy  wcscpy
#define lstrcat  wcscat
#define lstrcmp  wcscmp
#define lstrlen  wcslen
#else
#define lstrcpy  strcpy
#define lstrcat  strcat
#define lstrcmp  strcmp
#define lstrlen  strlen
#endif


typedef unsigned __int64	POINTER_64_INT;

typedef int					LONG32, *PLONG32;
typedef int					INT32, *PINT32;

typedef unsigned int		ULONG32, *PULONG32;
typedef unsigned int		DWORD32, *PDWORD32;
typedef unsigned int		UINT32, *PUINT32;


typedef int					INT_PTR, *PINT_PTR;
typedef unsigned int		UINT_PTR, *PUINT_PTR;

typedef long				LONG_PTR, *PLONG_PTR;
typedef unsigned long		ULONG_PTR, *PULONG_PTR;
typedef unsigned long		ULONG;

typedef __int64				LONG64, *PLONG64;
typedef __int64				INT64, *PINT64;

typedef unsigned __int64	ULONG64, *PULONG64;
typedef unsigned __int64	DWORD64, *PDWORD64;
typedef unsigned __int64	UINT64, *PUINT64;

typedef unsigned char		u_char;
typedef unsigned long		u_long;
typedef unsigned short		u_short;
typedef unsigned char		uchar;
typedef unsigned long		ulong;
typedef unsigned short		ushort;

typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL	            *PBOOL;
typedef BOOL	            *LPBOOL;
typedef BYTE                *PBYTE;
typedef BYTE                *LPBYTE;
typedef int                 *PINT;
typedef int                 *LPINT;
typedef WORD                *PWORD;
typedef WORD                *LPWORD;
typedef long                *LPLONG;
typedef DWORD               *PDWORD;
typedef DWORD               *LPDWORD;
typedef void                *LPVOID;
typedef CONST void          *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

/* Types use for passing & returning polymorphic values */
typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;

typedef DWORD				*PDWORD;
typedef unsigned long		ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR			DWORD_PTR, *PDWORD_PTR;
typedef void				*LPVOID;
typedef const void			*LPCVOID;
typedef void				*PVOID;
typedef void				*PVOID64;

typedef char				CHAR;
typedef short				SHORT;
typedef long				LONG;
typedef double              DOUBLE;
typedef wchar_t				WCHAR;

typedef WCHAR				*PWCHAR;
typedef WCHAR				*LPWCH, *PWCH;
typedef CONST WCHAR			*LPCWCH, *PCWCH;
typedef WCHAR				*NWPSTR;
typedef WCHAR				*LPWSTR, *PWSTR;
typedef WCHAR				*LPUWSTR, *PUWSTR;

typedef CONST WCHAR			*LPCWSTR, *PCWSTR;
typedef CONST WCHAR			*LPCUWSTR, *PCUWSTR;

typedef CHAR				*PCHAR;
typedef CHAR				*LPCH, *PCH;

typedef CONST CHAR			*LPCCH, *PCCH;
typedef CHAR				*NPSTR;
typedef CHAR				*LPSTR, *PSTR;
typedef CONST CHAR			*LPCSTR, *PCSTR;



#ifdef  UNICODE /* UNICODE */
#ifndef _TCHAR_DEFINED
typedef WCHAR				TCHAR, *PTCHAR;
typedef WCHAR				TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif 

typedef LPWSTR				LPTCH, PTCH;
typedef LPWSTR				PTSTR, LPTSTR;
typedef LPCWSTR				PCTSTR, LPCTSTR;
typedef LPUWSTR				PUTSTR, LPUTSTR;
typedef LPCUWSTR			PCUTSTR, LPCUTSTR;
typedef LPWSTR				LP;

#else /* UNICODE */

#ifndef _TCHAR_DEFINED
typedef char				TCHAR, *PTCHAR;
typedef unsigned char		TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif

typedef LPSTR				LPTCH, PTCH;
typedef LPSTR				PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR				PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
#endif /* UNICODE */               

typedef SHORT				*PSHORT;  
typedef LONG				*PLONG;    

typedef void				*HANDLE;
typedef PVOID				HANDLE;
typedef HANDLE				*PHANDLE;

typedef void				*HINSTANCE;

typedef BYTE				FCHAR;
typedef WORD				FSHORT;
typedef DWORD				FLONG;

typedef char				CCHAR;          
typedef DWORD				LCID;         
typedef PDWORD				PLCID;       
typedef WORD				LANGID;      

typedef struct _FLOAT128 {
    
    __int64 LowPart;
    __int64 HighPart;
    
} FLOAT128;

typedef FLOAT128			*PFLOAT128;

typedef __int64				LONGLONG;
typedef unsigned __int64	ULONGLONG;

void* KK_Malloc(size_t size);

void KK_Free(void *p);

u_long KK_GetCurrentTime();


#ifdef DEBUG

#define SLog(...) NSLog(__VA_ARGS__)
#else 
#define SLog(...)

#endif


#endif //__KK_INCLUDES_H__