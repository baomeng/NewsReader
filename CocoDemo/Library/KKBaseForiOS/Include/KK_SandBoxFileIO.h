//
//  SGSandBoxFileIO.h
//  SGSandBoxFileIO类，功能说明如下：
//      1、获取SandBox中，4种文件操作路径：
//         a. Documents: iTunes备份&恢复此目录，主要保存数据文件；
//         b. Library: iTunes备份&恢复此目录，主要保存默认配置文件、状态信息文件；
//         c. Library/Caches: iTunes不备份&恢复此目录，但是，应用程序退出时，不清空；
//         d. tmp: iTunes不备份&恢复此目录，并且在应用程序重启或者退出时，会清空；
//      2、设置应用程序的当前文件路径；
//      3、在SandBox的文件操作路径下，创建、删除文件夹；
//      4、在SandBox的文件操作路径下，创建、删除文件；
//      5、读写文件。
//  注释：
//      SGSandBoxFileIO类，没有线程同步处理。
//
//  Created by Kai Liu on 12-8-23.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>

////////////////////////////////////////////////////////////////
//  SandBox文件路径种类
enum _SANDBOX_DIR_TYPE_
{
    SBDT_DOCUMENTS  = NSDocumentDirectory,              // Documents目录
    SBDT_LIBRARY    = NSLibraryDirectory,               // Library目录
    SBDT_CACHES     = NSCachesDirectory,                // Library/Caches目录
    SBDT_TMP        = NSAllLibrariesDirectory + 100     // tmp目录
};

////////////////////////////////////////////////////////////////
//  SGSandBoxFileIO类的声明
@interface SGSandBoxFileIO : NSObject
{
@protected
    NSFileManager*      m_pclDefFileMgr;
    NSString*           m_pclBaseDir;
    NSString*           m_pclSubDir;
    _SANDBOX_DIR_TYPE_  m_eDirType;
}

- (id) init;
- (id) initWithDirType: (_SANDBOX_DIR_TYPE_) eDirType
            withSubDir: (NSString*) pclSubDir;
- (void) dealloc;

- (NSString*) getCurDirectory;

- (BOOL) ValidateCurDirectory;

- (BOOL) changeToSubDir: (NSString*) pclSubDir;
- (BOOL) changeDirWithType: (_SANDBOX_DIR_TYPE_) eDirType
                withSubDir: (NSString*) pclSubDir;

- (BOOL) createFolder: (NSString*) pclFolderName;
- (BOOL) destroyFolder: (NSString*) pclFolderName;

- (BOOL) createFile: (NSString*) pclFileName;
- (BOOL) destroyFile: (NSString*) pclFileName;

- (BOOL) fileIsExist: (NSString*) pclFileName;

- (NSData*) readFileInCurDir: (NSString*) pclFileName;
- (NSString*) readFileInCurDirEx: (NSString*) pclFileName;
- (NSArray*) readXMLFileInCurDir: (NSString*) pclFileName;

- (BOOL) writeFileInCurDir: (NSString*) pclFileName
                  withData: (NSData*) pclData;
- (BOOL) writeFileInCurDirEx: (NSString*) pclFileName
                  withData: (NSString*) pclData;
- (BOOL) writeXMLFileInCurDir: (NSString*) pclFileName
                    withData: (NSArray*) pclData;

@end    // SGSandBoxFileIO


// 注释[2012.08.31] : 该类后续会做成真正的单态
@interface SGSandBoxFileIOSingleton : NSObject

+ (NSString*) getSandBoxDirByType: (_SANDBOX_DIR_TYPE_) eDirType;

+ (BOOL) changeToDir: (NSString*) pclWholeDir;

+ (BOOL) createFolder: (NSString*) pclWholeDir;
+ (BOOL) destroyFolder: (NSString*) pclWholeDir;

+ (BOOL) fileIsExist: (NSString*) pclWholeFileName;

+ (BOOL) createFile: (NSString*) pclWholeFileName;
+ (BOOL) destroyFile: (NSString*) pclWholeFileName;

+ (BOOL) copyFile:(NSString*) srcFile dest:(NSString*)dstFile;


@end    // SGSandBoxFileIOSingleton

