//
//  SystemErrorCodeDef.h
//  CMModuleCreator
//
//  Created by Kai Liu on 12-6-6.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#ifndef CMModuleCreator_SystemErrorCodeDef_h
#define CMModuleCreator_SystemErrorCodeDef_h

////////////////////////////////////////////////////////////////
//  协议方法的说明符
#define SYNCHRONISM                             // 同步方法说明符
#define ASYNCHRONISM                            // 异步方法说明符

////////////////////////////////////////////////////////////////
//  协议&接口函数返回码
enum _CM_FUN_CODE_
{
    CMFC_SUCCESS    = (0),                      // 成功
    CMFC_FAILED     = (1),                      // 失败
    CMFC_CONTINUE   = (2)                       // 继续
};

////////////////////////////////////////////////////////////////
//  系统内部错误码，后续需要完善、统一 
enum _CM_ERROR_CODE_
{
	CMED_OPERATE_FAILED     = (-1),				// 不明原因失败
	CMED_OPERATE_SUCCESS    = (0),				// 操作成功
    
	CMED_PROXY_INVALID,                         // 代理服务器无效
	CMED_CONNECT_SERVER_FAILED,                 // 连接服务器出错
	CMED_CONNECT_SERVER_SUCCESS,                // 连接服务器成功
	CMED_DISCONNECT,                            // 连接断开
	CMED_CONNECT_DATABASE_FAILED,               // 数据库错误
	CMED_ERR_NO_PERM,                           // 无权限
	CMED_ERR_SERVER_INLINE,                     // 该服务器在线不能配置
	CMED_TOO_FREQUENT,                          // 操作过于频繁，请稍候再试
    
	CMED_HAVE_NOT_USER,                         // 没有这个用户
	CMED_USER_HAVE_INLINE,                      // 用户已经在线
	CMED_USER_PASS_ERROR,                       // 用户密码错误
	CMED_USER_OFFLINE,                          // 用户掉线
	CMED_ERR_HAVE_PERM,                         // 权限已经存在
	CMED_ERR_NO_USER,                           // 没有这个用户
	CMED_ERR_VERSION,                           // 版本不匹配
	CMED_HAVE_EXIST_USER,                       // 用户名已存在
    
	CMED_HAVE_PERM,                             // 拥有相应权限
	CMED_NOT_PERM,                              // 没有相应权限
    
	CMED_HAVE_NOT_SERVRE,                       // 没有服务器
	CMED_SERVRE_HAVE_INLINE,				    // 存在在线服务器
	CMED_SERVRE_PASS_ERROR,                     // 服务器认证失败
	CMED_SERVRE_OFFLINE,                        // 服务器离线
	CMED_SERVRE_EXIST,                          // 服务器已存在
    
	CMED_STORE_SERVER_FULL,                     // 所有存储服务器硬盘已满或没有存储服务器在线
	CMED_STORE_SERVER_FAILED,                   // 相关存储服务器出错
	CMED_DATA_ERROR_FAILED,                     // 数据库中数据有问题
    
	CMED_NOT_HAVEID_FAILED,                     // 无此记录
	CMED_DEFAULT_DELETE,                        // 默认不能删除
	CMED_NOT_ASSIGN,                            // 设备未分配到逻辑组
    
	CMED_DOUBLE_NAME_FAILED,                    // 重复
	CMED_NTYPE_FAILED,                          // 类型错误
    
	CMED_DEVICE_OFFLINE,                        // 设备不在线
	CMED_USER_DUPLICATE,                        // 用户名已存在
	CMED_DATA_ERROR,                            // 数据错误(用户名或密码过长，不合法)
	CMED_WRITE_INIFILE_ERROR,                   // 写配置文件失败
	CMED_START_TIMER_ERROR,                     // 启动定时器失败
    
	CMED_MAP_NOEXIST_MAPID,
	CMED_MAP_NOEXIST_FATHERMAPID,
	CMED_MAP_NOEXIST_MAPFILE,
	CMED_MAP_NOEXIST_DEVID,
	CMED_MAP_FILE_UPLOADING,
	CMED_MAP_FILE_DOWNLOADING,
	CMED_MAP_FILE_UPDATEBYOTHER,
	CMED_MAP_FILE_NEEDUPDATE,
	CMED_MAP_FILE_CHANGED,
    
	CMED_DEV_HASRECORD,                         // 设备已经在录相
	CMED_DEV_READYRECORD,						// 设备正在准备录相
	CMED_DEV_NOT_EXIST,                         // 设备不存在
	CMED_DEV_DEVTYPE_UNSUPPORT,                 // 不支持此类型设备
	CMED_DEV_UNCONNECTED,						// 设备没有链接
	CMED_DEV_ALEARDYONLINE,                     // 设备已经在线
	CMED_DEV_CONNECERRO,						// 管理通道连接错误
	CMED_DEV_UNSUPPORT,                         // 设备不支持该操作
	CMED_DEV_USER_NORIGHT,                      // 设备用户无权限
	CMED_DEV_BUSYING,							// 设备忙
	CMED_DEV_TASKEXIST,                         // 任务已存在
	CMED_DEV_REQUEST_REFUSE,					// 请求被拒绝
	CMED_DEV_USER_FULL,                         // 设备用户已满
	CMED_DEV_RESOURCE_NOTEXIST,                 // 请求资源不存在 
	CMED_DEV_INSIDE_ERROR,                      // 设备内部错误
	CMED_DEV_USER_PASS_UNPASS,                  // 用户名或密码错误
	CMED_DEV_TIMEOUT,							// 设备超时
	CMED_DEV_INVALID_PARAMETER,                 // 参数不合法
	CMED_DVE_PARAMERTER_TOOSHORT,               // 参数过长
	CMED_DEV_UPDATE_FILE_ERRO,                  // 升级文件错误
	CMED_DEV_UPDATE_UNMATCH,					// 升级文件不匹配
	CMED_DEV_UPDATING,                          // 设备已经在升级
	CMED_DEV_RESOURCE_USING,                    // 设备资源正在使用中
    
	CMED_DEV_PLANVIDEO_COMPLETE,                // 计划录像完成
	CMED_LOCALKINESCOPE_NODISK,                 // 未设置本地存储路径
	CMED_DEV_SPEAKING,                          // 设备对讲中
	CMED_LOCALDISK_NOSPACE,                     // 磁盘空间不足
	CMED_RELAYSRV_OFFLINE,                      // 转发服务器不在线
	CMED_STORESRV_OFFLINE,                      // 转发服务器不在线
	CMED_OPERATE_CONFLICT,                      // 操作冲突
    
	CMED_OPERATE_TIMEOUT,						// 操作超时
	CMED_OPERATE_CANCEL,                        // 操作取消
	CMED_ENCODEDEV_UNBIND,                      // 未绑定解码设备。
	CMED_REGISTER_AUTH_SUCCESS,                 // 注册用户验证码成功。
	CMED_REGISTER_AUTH_FAILED,                  // 注册用户验证码验证失败。
	CMED_GET_AUTH_CODE_SUCCESS,                 // 获取验证码成功。
	CMED_GET_AUTH_CODE_FAILED,                  // 获取验证码失败。
	CMED_AUTH_CODE_INVALID,                     // 验证码失效。
	CMED_USER_FORBIDLOGON,                      // 用户被禁止登陆。
	CMED_USER_NONUSE,							// 账户不可用。
	CMED_ERR_NO_DEV,							// 设备不存在。
	CMED_ERR_DEV_AUTH,                          // 设备验证失败。
	CMED_OPERATE_CANCEL_SERVER,                 // 取消连接服务器
    
	CMED_ERR_USERGROUP_DUPLICATE,				// 用户组已经存在
	CMED_ERR_NO_USERGROUP,                      // 没有用户组

	CMED_USER_FORCELINEOFF_TIME,				// 用户强制下线期间

	CMED_ERR_ROLE_DUPLICATE,					// 角色名已经存在
	CMED_ERR_NO_EXIT_PERM,                      // 该用户没有退出系统的权限

	CMED_ERR_GROUP_NOT_EMPTY,					// 用户组非空无法删除
	CMED_HAVE_EXIST_DEALER,                     // 该组已经存在经销商
	CMED_NO_RECORD_FIND,                        // 没有查询到符合条件的记录
	CMED_NO_GROUP_ADMIN,						// 当前用户无关联组（没有能管理的组）
    
////////////////////////////////////////////////////////////////
//  以下错误码，为客户端新增，同步中心新增的错误码，要在该处以上增加
    CMED_RECONNECT_FAILED,                      // 重连失败
    CMED_USER_FORBIDMONITOR                     // 无监看权限
};
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//  系统错误码，对应的字符串提示
static NSString* ERROR_CODE_STRING[] = 
{
    @"不明原因失败",
    @"操作成功",
    @"代理服务器无效",
    @"连接服务器出错",
    @"连接服务器成功",
    @"连接断开",
    @"数据库错误",
    @"无权限",
    @"该服务器在线不能配置",
    @"操作过于频繁，请稍候再试",
    @"没有这个用户",
    @"用户已经在线",
    @"用户密码错误",
    @"用户掉线",
    @"权限已经存在",
    @"没有这个用户",
    @"版本不匹配",
    @"用户名已存在",
    @"拥有相应权限",
    @"没有相应权限",
    @"没有服务器",
    @"存在在线服务器",
    @"服务器认证失败",
    @"服务器离线",
    @"服务器已存在",
    @"所有存储服务器硬盘已满或没有存储服务器在线",
    @"相关存储服务器出错",
    @"数据库中数据有问题",
    @"无此记录",
    @"默认不能删除",
    @"设备未分配到逻辑组",
    @"重复",
    @"类型错误",
    @"设备不在线",
    @"用户名已存在",
    @"数据错误(用户名或密码过长，不合法)",
    @"写配置文件失败",
    @"启动定时器失败",
    @"MapID不存在",
    @"FatherMapID不存在",
    @"Map文件不存在",
    @"设备ID不存在",
    @"正在上传Map文件",
    @"正在下载Map文件",
    @"其他人正在更新Map文件",
    @"Map文件需要更新",
    @"Map文件发生改变",
    @"设备已经在录相",
    @"设备正在准备录相",
    @"设备不存在",
    @"不支持此类型设备",
    @"设备没有链接",
    @"设备已经在线",
    @"管理通道连接错误",
    @"设备不支持该操作",
    @"设备用户无权限",
    @"设备忙",
    @"任务已存在",
    @"请求被拒绝",
    @"设备用户已满",
    @"请求资源不存在",
    @"设备内部错误",
    @"用户名或密码错误",
    @"设备超时",
    @"参数不合法",
    @"参数过长",
    @"升级文件错误",
    @"升级文件不匹配",
    @"设备已经在升级",
    @"设备资源正在使用中",
    @"计划录像完成",
    @"未设置本地存储路径",
    @"设备对讲中",
    @"磁盘空间不足",
    @"转发服务器不在线",
    @"转发服务器不在线",
    @"操作冲突",
    @"操作超时",
    @"操作取消",
    @"未绑定解码设备。",
    @"注册用户验证码成功。",
    @"注册用户验证码验证失败。",
    @"获取验证码成功。",
    @"获取验证码失败。",
    @"验证码失效。",
    @"用户被禁止登陆。",
    @"账户不可用。",
    @"设备不存在。",
    @"设备验证失败。",
    @"取消连接服务器",
    @"用户组已经存在",
    @"没有用户组",
    @"用户强制下线期间",
    @"角色名已经存在",
    @"该用户没有退出系统的权限",
    @"用户组非空无法删除",
    @"该组已经存在经销商",
    @"没有查询到符合条件的记录",
    @"当前用户无关联组（没有能管理的组）",
////////////////////////////////////////////////////////////////
//  以下错误码，为客户端新增，同步中心新增的错误码，要在该处以上增加
    @"重连失败",
    @"用户无监看权限"
};

static NSString* convertErrorCodeToString(enum _CM_ERROR_CODE_ eErrorCode)
{
    return NSLocalizedString(ERROR_CODE_STRING[eErrorCode + 1], nil);
};
////////////////////////////////////////////////////////////////

#endif