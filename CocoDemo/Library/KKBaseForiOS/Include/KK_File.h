//
//  KK_File.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 12-8-21.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_FILE__
#define __KK_FILE__

#include "KK_Includes.h"
#include "KK_Templet.h"
#include "KK_DateTimeEx.h"
#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>

#define INVALID_FILE_SIZE_64 ((__int64)0xFFFFFFFFFFFFFFFF)
#define APPEND_BUF_SIZE 1048576 //(1024*1024)


class IFile
{
public:
    virtual size_t Read(void *lpReadBuf, size_t nCount) = 0;
    virtual size_t Write(const void *lpWriteBuf, size_t nCount) = 0;
};

//#define KK_StringWA KK_StringA//目前先把KK_StringWA定义为KK_StringA

//st_atime 文件最近一次被存取或被执行的时间，一般只有在用mknod、utime、read、write与tructate时改变。
//st_mtime 文件最后一次被修改的时间，一般只有在用mknod、utime和write时才会改变
//st_ctime i-node最近一次被更改的时间，此参数会在文件所有者、组、权限被更改时更新先前所描述的st_mode 则定义了下
struct KK_FileState
{
    mode_t ttFileAttributes;
    KK_DatetimeEx ttINodeTime;
    KK_DatetimeEx ttLastAccessTime;
    KK_DatetimeEx ttLastWriteTime;
    DWORD dwFileSize;
};

//此文件操作暂时不支持大于4G的文件操作。
class KK_File : public IFile
{
public:
	enum OpenFlags {
		modeRead        = 0x00000001, //读写标志
		modeWrite       = 0x00000002,
		modeReadWrite    = modeRead | modeWrite,
        
		DenyNone		= 0x00000000, //共享标志
		DenyRead		= 0x00000010,
		DenyWrite		= 0x00000020,
		DenyAll		    = DenyRead | DenyWrite,
        
		OpenExisting		= 0x00000100, //打开标志
		OpenAlways			= 0x00000200,
		CreateAlways		= 0x00000300,
		CreateNoExisting	= 0x00000400,
		
		ReadOnly            = 0x00001000, //属性标志
		Hidden              = 0x00002000,
		archive             = 0x00004000,
		//Normal              = 0x00008000,//默认，可以不必加
    };
    
	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
    
    //构造函数和析构函数
public:
	KK_File();
	KK_File(FILE* hFile);
	KK_File(const KK_StringA& strFileName, unsigned int uOpenFlag);
	virtual ~KK_File();
	
    //文件操作
public:
    BOOL Open(const KK_StringA& strFileName, unsigned int uOpenFlag);
	BOOL Close();
	
    size_t Read(void *bufRead, size_t nCount);
	size_t Write(const void *bufWrite, size_t nCount);
	__int64 Seek(__int64 nOffSet, int nOriginPos);
    
    static BOOL Append(const KK_StringA& strObjName, const KK_StringA& strSrcName);
    BOOL Append(const KK_StringA& strFileName, BOOL bFlag = TRUE); //bFlag用来判断是把当前文件加到给定文件尾还是把给定文件加到当前文件尾。
    
    BOOL SeekToEnd();
    BOOL SeekToBegin(); 
    
    inline FILE* GetFileHandle() { return m_hFile; }
    
    //取得和设置文件的一些属性
public:
	KK_StringA GetFullFileName() const;
	KK_StringA GetFileName() const;  
	KK_StringA GetFileTitle() const;          
    KK_StringA GetFilePath() const; 
    KK_StringA GetFileExtension() const;
	
	__int64 GetFileLength(); 
    BOOL SetFileLength(__int64 nFileLenth); 
    BOOL Truncate();//在文件指针的当前位置截断。
	__int64 GetPosition();
	BOOL Flush();
    
    static BOOL GetStatus(const KK_StringA& strPath, KK_FileState& tStatus);
	BOOL GetStatus(KK_FileState& tStatus) const;    
    
public:
    BOOL IsEOF();
    BOOL IsOpen();
    BOOL Abort();
    
	/*
     大于4G的文件暂时不考虑
     int cmd = F_SETLK:
     F_SETLK 设置文件锁定的状态。此时flcok 结构的l_type 值必须是F_RDLCK、F_WRLCK或F_UNLCK。如果无法建立锁定，则返回-1，错误代码为EACCES 或EAGAIN。
     F_SETLKW F_SETLK 作用相同，但是无法建立锁定时，此调用会一直等到锁定动作成功为止。若在等待锁定的过程中被信号中断时，会立即返回-1，错误代码为EINTR。
     
     l_type 有三种状态:
     F_RDLCK 建立一个供读取用的锁定
     F_WRLCK 建立一个供写入用的锁定
     F_UNLCK 删除之前建立的锁定
     l_whence 也有三种方式:
     SEEK_SET 以文件开头为锁定的起始位置。
     SEEK_CUR 以目前文件读写位置为锁定的起始位置
     SEEK_END 以文件结尾为锁定的起始位置。
     */
	BOOL LockFile(__int64 nStart, __int64 nBytes, short int l_type = F_RDLCK, short int l_whence = SEEK_SET, int cmd = F_SETLK);
    BOOL UnLockFile(__int64 nStart, __int64 nBytes, int cmd = F_SETLK);
    
    const KK_File& operator = (KK_File hFile);   
    
	static BOOL Remove(const KK_StringA& strFileName);
    
protected:
	//文件名为全路径，否则就是在当前模块所在的地方
	KK_StringA m_strFileName;
	FILE* m_hFile;
};


#endif //__KK_FILE__