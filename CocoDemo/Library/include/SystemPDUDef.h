//
//  SystemPDUDef.h
//  Public
//
//  Created by Kai Liu on 12-6-15.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SystemErrorCodeDef.h"
#import "SystemResourceDef.h"

////////////////////////////////////////////////////////////////
//  iPhone&iPad客户端类型
const ULONG_32  APPLE_REMOTE   = 108;
const u_short   MAX_PDU_LEN    = 65535;
const u_short   SYSTEM_ZIP_PDU = 65535;

////////////////////////////////////////////////////////////////
//  中心服务器各个模块的ID
enum _INDEXSRV_MODULE_ID_
{
    IMID_ACCOUNTMGR         = (0x1000),         // 帐号管理(CM_PDU_USERMGR_CMD)
    IMID_PERMMGR            = (0x1001),         // 权限管理(CM_PDU_PERMMGR_CMD)
    IMID_CLT_HEARTBEAT      = (0x1002),         // 客户端心跳(CM_PDU_USER_HEARTBEAT_CMD)
    IMID_DEVICEMGR          = (0x1003),         // 设备管理(CM_PDU_DEVMGR_CMD)
    IMID_MEDIAMGR           = (0x1004),         // 媒体管理(CM_PDU_MEDIAMGR_CMD)
    IMID_BILLINGMGR         = (0x1013),         // 计费管理(CM_PDU_BILLING_CMD)
    IMID_CLIENT_DETECT      = (0x1014)          // 客户端版本检测(CM_PDU_CLIENT_DETECT_CMD)
};

////////////////////////////////////////////////////////////////
//  客户端版本检测的PDU定义
enum _CLIENT_DETECT_PDU_
{
    PDU_VERSION_DETECT_REQUEST	= (0),          // 客户端版本检测请求(Client to IndexSrv)
	PDU_VERSION_DETECT_RESPONSE	= (1)           // 客户端版本检测应答(IndexSrv to Client)
};

////////////////////////////////////////////////////////////////
//  客户端心跳的PDU定义
enum _CLIENT_HEARTBEAT_PDU_
{
    PDU_HEART_BEAT_REQUEST      = (0),          // 客户端心跳请求(Client to IndexSrv)
    PDU_HEART_BEAT_RESPONSE     = (1)           // 客户端心跳应答(IndexSrv to Client)
};

////////////////////////////////////////////////////////////////
//  帐号计费管理相关的PDU定义
enum _ACCOUNT_BALANCE_PDU_
{
    PDU_GET_ACCOUNT_BALANCE_INFO_REQ	= (50),	// 获取当前登录账号的余额信息的请求(Client to IndexSrv)
    PDU_GET_ACCOUNT_BALANCE_INFO_RESP	= (51),	// 获取当前登录账号的余额信息的应答(IndexSrv to Client)
    
    PDU_SYNC_LOGON_ACCOUNT_BALANCE_INFO	= (52)	// 同步当前登录账号的余额信息(IndexSrv to Client)
};

////////////////////////////////////////////////////////////////
//  媒体管理相关的PDU定义
enum _MEDIA_MGR_PDU_
{
    PDU_START_REALMONITOR_REQUEST       = (0),	// 开始监看请求(Client to IndexSrv)
	PDU_START_REALMONITOR_RESPONSE      = (1),	// 开始监看应答(IndexSrv to Client)
	PDU_STOP_REALMONITOR_REQUEST        = (2),	// 停止监看请求(Client to IndexSrv)
	PDU_STOP_REALMONITOR_RESPONSE       = (3),	// 停止监看应答(IndexSrv to Client)
    PDU_DVS_DEV_START_LISTEN_REQUEST    = (5),	//	Client->IndexSrv
	PDU_DVS_DEV_START_LISTEN_RESPONSE	= (PDU_DVS_DEV_START_LISTEN_REQUEST+1), 	//	IndexSrv->Client
	PDU_DVS_DEV_STOP_LISTEN_REQUEST     = (PDU_DVS_DEV_START_LISTEN_REQUEST+2),	//	Client->IndexSrv
	PDU_DVS_DEV_STOP_LISTEN_RESPONSE	= (PDU_DVS_DEV_START_LISTEN_REQUEST+3),	//	IndexSrv->Client
	PDU_DVS_DEV_LISTEN_START_NOTIFY     = (PDU_DVS_DEV_START_LISTEN_REQUEST+4),
    PDU_SET_MONITOR_MODEL               = (103) // 设置监看模式(Client to IndexSrv)
};

////////////////////////////////////////////////////////////////
//  用户管理相关的PDU定义
enum _CLIENT_USERMGR_PDU_
{
    PDU_DVS_LOGIN_REQUES                = (0),	// 登陆请求(Client to IndexSrv)
	PDU_DVS_LOGIN_RESPONSE              = (1),	// 登陆应答(IndexSrv to Client)
	PDU_DVS_AUTH_REQUEST                = (2),	// 验证请求(Client to IndexSrv)
	PDU_DVS_AUTH_RESPONSE               = (3),	// 验证应答(IndexSrv to Client)
    
    PDU_MINICLT_GETINFO_BY_GROUPID_REQ	 = (600),		// 通过[组ID]查询数据的请求(Clt -> Srv)
	PDU_MINICLT_GETINFO_BY_GROUPID_REPLY = (601),		// 通过[组ID]查询数据的应答(Srv -> Clt)
    
};

////////////////////////////////////////////////////////////////
//  设备管理相关的PDU定义
enum _CLIENT_UDEVMGR_PDU_
{
    PDU_ADDDEV_NOTIFY                   = (35), // 添加设备通知
    PDU_DELDEV_NOTIFY                   = (36), // 删除设备通知
    
    PDU_DEV_CTRL_PTZ_REQ                 = (77),        //云台控制
    PDU_DEV_CTRL_PTZ_REPLY               = (78),
    
    PDU_MINICLT_GETINFO_BY_DEVID_REQ	 = (602),		// 通过[设备ID]查询数据的请求(AndroidClt -> Srv)
	PDU_MINICLT_GETINFO_BY_DEVID_REPLY	 = (603),		// 通过[设备ID]查询数据的应答(Srv -> AndroidClt)
    
	PDU_MINICLT_DEV_STATUS_NOTIFY		 = (606),		// 设备状态通知(Srv -> AndroidClt)
	PDU_MINICLT_DEV_NAME_CHANGE_NOTIFY	 = (607),		// 设备名改变通知(Srv -> AndroidClt)
	PDU_MINICLT_CHNL_NAME_CHANGE_NOTIFY	 = (608),		// 通道名改变通知(Srv -> AndroidClt)
};

////////////////////////////////////////////////////////////////
//  权限管理相关的PDU定义
enum _CLIENT_PERMMGR_PDU_
{
    PDU_GET_OWNER_PERMDATA_REQUES            = (100),	// 获取用户权限请求(Client to IndexSrv)
	PDU_GET_OWNER_PERMDATA_REPLY             = (101),	// 获取用户权限(IndexSrv to Client)
    PDU_NOTIFY_PERM_CHANGE                   = (102),   // 当前用户权限改变通知
};

////////////////////////////////////////////////////////////////
//  监控系统内部的通用协议头
typedef struct _PROTOCOL_HEAD_
{
    u_short   usMDPDU;                            // 模块PDU值
    ULONG_32  ulClientModuleID;                   // 发起请求的客户端模块ID(苹果客户端，暂不使用)
    ULONG_32  ulReservered1;                      // 保留字段
    ULONG_32  ulReservered2;                      // 保留字段
} SProtocolHead, *LPSProtocolHead;

////////////////////////////////////////////////////////////////
//  媒体服务相关的协议定义
enum _MEDIA_SERVICE_PDU_
{
    MDSRV_PDU_CLIENT_HEART_BEAT     = (4),       // 心跳(Client to MediaSrv)
    MDSRV_PDU_MONITOR_NEW_TASK_REQ  = (200),     // 新建监看任务请求(Client to MediaSrv)
    MDSRV_PDU_MONITOR_NEW_TASK_RESP = (201),     // 新建监看任务应答(MediaSrv to Client)
    MDSRV_PDU_MONITOR_DATA          = (202),     // 监看视频数据(MediaSrv to Client)
    MDSRV_PDU_MONITOR_BIG_DATA      = (204),     // 监看视频数据分包(MediaSrv to Client)
    
    PDU_EMBED_DVS_NEW_LISTEN_TASK_REQ = (MDSRV_PDU_MONITOR_NEW_TASK_REQ + 9),		// 侦听论证;	方向:（中心－＞设备）	 ;参数：KK_String strAuth,
	PDU_EMBED_DVS_NEW_LISTEN_TASK_RESP = (MDSRV_PDU_MONITOR_NEW_TASK_REQ + 10),		//方向:（设备－＞中心）	 ;参数：int iResult, KK_String strAuth
	PDU_EMBED_DVS_LISTEN_TASK_DATA = (MDSRV_PDU_MONITOR_NEW_TASK_REQ + 11)			// 传输数据;				方向:（设备－＞中心）	 ;参数：
};

////////////////////////////////////////////////////////////////
//  通用函数声明
static KK_MemoryStream& operator<< (KK_MemoryStream& clOS, const SProtocolHead& stHead);
static KK_ByteStream& operator>> (KK_ByteStream& clIS, SProtocolHead& stHead);
static ULONG_32 makeWohlePDUKeyValue(u_short usIndexSrvMDID, u_short usMDPDU);

static KK_ByteStream& operator >> (KK_ByteStream& os, CGetInfo& clGetInfo);
static KK_ByteStream& operator >> (KK_ByteStream& is, CChannelGetInfo& clChannelGetInfo);

////////////////////////////////////////////////////////////////
//  客户端版本检测相关的数据序列化&反序列化
class CClientDetectReq
{
public:
    CClientDetectReq(void) : m_clClientVer()
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_VERSION_DETECT_REQUEST;
    };
    
    // 不可继承
    ~CClientDetectReq(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void) { return m_stProtocolHead; };
    
    void        setClientVersion(const KK_StringW& clClientVer) { m_clClientVer = (LPCWSTR)clClientVer; };
    KK_StringW& getClientVersion(void)                          { return m_clClientVer; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        clOS << m_clClientVer;
        clOS << APPLE_REMOTE;
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    KK_StringW      m_clClientVer;              // 当前客户端版本号
};

class CClientDetectResp
{
public:
    CClientDetectResp(void) : m_iResult(CMED_OPERATE_FAILED),
                              m_clClientLastVer()
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
    };
    
    // 不可继承
    ~CClientDetectResp(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void) { return m_stProtocolHead; };
    
    void setResult(int iResult) { m_iResult = iResult; };
    int  getResult(void)        { return m_iResult; };
    
    const KK_StringW& getClientLastVer(void) const { return m_clClientLastVer; };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        clIS >> m_stProtocolHead;
        clIS >> m_iResult;
        if ( clIS.GetUnusedBufSize() > 0 )
        {
            clIS >> m_clClientLastVer;
        }
        return clIS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;                  // 版本检测返回值
    KK_StringW      m_clClientLastVer;          // 最新版本号
};

////////////////////////////////////////////////////////////////
//  客户端心跳相关的数据序列化&反序列化
class CClientHeartBeatReq
{
public:
    CClientHeartBeatReq(void) : m_usMDPDU(PDU_HEART_BEAT_REQUEST)
    {
        // Do Nothing
    };
    
    // 不可继承
    ~CClientHeartBeatReq(void)
    {
        // Do Nothing
    };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_usMDPDU;
        clOS << (ULONG_32)time(NULL);
        return clOS;
    };
    
private:
    u_short m_usMDPDU;                          // 模块PDU值
};

////////////////////////////////////////////////////////////////
//  帐号计费管理相关的数据序列化&反序列化
class CGetAccBalanceInfoReq
{
public:
    CGetAccBalanceInfoReq(void)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_GET_ACCOUNT_BALANCE_INFO_REQ;
    };
    
    // 不可继承
    ~CGetAccBalanceInfoReq(void)
    {
        // Do Nothing
    };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        return clOS;
    };
    
private:
    SProtocolHead m_stProtocolHead;
};

class CGetAccBalanceInfoResp
{
public:
    CGetAccBalanceInfoResp(void) : m_iResult(CMED_OPERATE_FAILED),
                                   m_uiAccountID(INVALID_ACCOUNT_ID),
                                   m_ulCHCnt(0)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
    };
    
    // 不可继承
    ~CGetAccBalanceInfoResp(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void)        { return m_stProtocolHead; };
    int            getResult(void)              { return m_iResult; };
    u_int          getAccountID(void)           { return m_uiAccountID; };
    ULONG_32       getWanMonitorChlCnt(void)    { return m_ulCHCnt; };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        clIS >> m_stProtocolHead;
        clIS >> m_iResult;
        clIS >> m_uiAccountID;
        clIS >> m_ulCHCnt;
        return clIS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;					// 返回值
	u_int           m_uiAccountID;				// 账号ID
	ULONG_32        m_ulCHCnt;					// 当前公网监看路数
};

class CSyncLogonAccBalanceInfo
{
public:
    CSyncLogonAccBalanceInfo(void) : m_uiAccountID(INVALID_ACCOUNT_ID),
                                     m_ulCHCnt(0)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
    };
    
    // 不可继承
    ~CSyncLogonAccBalanceInfo(void)
    {
        // Do Nothing
    };
        
    SProtocolHead& getProtocolHead(void)        { return m_stProtocolHead; };
    u_int          getAccountID(void)           { return m_uiAccountID; };
    ULONG_32       getWanMonitorChlCnt(void)    { return m_ulCHCnt; };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        clIS >> m_stProtocolHead;
        clIS >> m_uiAccountID;
        clIS >> m_ulCHCnt;
        return clIS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
	u_int           m_uiAccountID;				// 账号ID
	ULONG_32        m_ulCHCnt;					// 当前公网监看路数
};

////////////////////////////////////////////////////////////////
//  媒体相关的数据序列化&反序列化
class CStartListenReq
{
public:
    CStartListenReq(void) : m_clChannelID(),
    m_bRemoteFlag(false)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_DVS_DEV_START_LISTEN_REQUEST;
    };
    
    // 不可继承
    ~CStartListenReq(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void) { return m_stProtocolHead; };
    
    void setChannelID(const CChannelID& clChannelID) { m_clChannelID = clChannelID; };
    void setRemoteFlag(bool bRemoteFlag)             { m_bRemoteFlag = bRemoteFlag; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        m_clChannelID.serializeToStream(clOS);
        clOS << m_bRemoteFlag;
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    CChannelID      m_clChannelID;              // 通道ID
    bool            m_bRemoteFlag;              // 远程设备标志
};


class CStartListenResp
{
public:
    CStartListenResp(void) : m_iResult(CMED_OPERATE_FAILED),
    m_clChannelID(),
    m_clAuthCode(),
    m_clVideoSrvIP(),
//    m_clUserName(),
//    m_clPassWord(),
    m_usVideoSrvPort(0)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
    };
    
    // 不可继承
    ~CStartListenResp(void)
    {
        // Do Nothing
    };
    
    SProtocolHead&    getProtocolHead(void)     { return m_stProtocolHead; };
    int               getResult(void)           { return m_iResult; };
    const CChannelID& getChannelID(void)        { return m_clChannelID; };
    const KK_StringA& getAuthCode(void)         { return m_clAuthCode; };
    const KK_StringA& getVideoSrvIP(void)       { return m_clVideoSrvIP; };
//    const KK_StringA& getUserName(void)         { return m_clUserName; };
//    const KK_StringA& getPassWord(void)         { return m_clPassWord; };
    u_short           getVideoSrvPort(void)     { return m_usVideoSrvPort; };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        bool bIsUdp = false;
        
        clIS >> m_stProtocolHead;
        clIS >> m_iResult;
        if ( CMED_OPERATE_SUCCESS == m_iResult )
        {
            clIS >> m_clAuthCode;
            m_clChannelID.deserializeFromStream(clIS);
            clIS >> m_clVideoSrvIP;
            clIS >> m_usVideoSrvPort;
            clIS >> bIsUdp;
//            clIS >> m_clUserName;
//            clIS >> m_clPassWord;
        }
        
        return clIS;
    };
    
    KK_MemoryStream& serializeToMediaStream(KK_MemoryStream& clOS)
    {
        clOS << m_clAuthCode;
        clOS << (bool)true;
        
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;                  // 返回值
    CChannelID      m_clChannelID;              // 通道ID
	KK_StringA      m_clAuthCode;               // 监看认证码
	KK_StringA      m_clVideoSrvIP;             // 视频服务的IP
//	KK_StringA      m_clUserName;               // 视频服务的用户名
//	KK_StringA      m_clPassWord;               // 视频服务的密码
	u_short         m_usVideoSrvPort;           // 视频服务的端口
};

class CStopListenReq
{
public:
    CStopListenReq(void) : m_clChannelID(),
    m_clAuthCode()
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_DVS_DEV_STOP_LISTEN_REQUEST;
    };
    
    // 不可继承
    ~CStopListenReq(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void) { return m_stProtocolHead; };
    
    void setChannelID(const CChannelID& clChannelID) { m_clChannelID = clChannelID; };
    void setAuthCode(const KK_StringA& clAuthCode)   { m_clAuthCode = (LPCSTR)clAuthCode; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        m_clChannelID.serializeToStream(clOS);
        clOS << m_clAuthCode;
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    CChannelID      m_clChannelID;              // 通道ID
    KK_StringA      m_clAuthCode;               // 监看认证码
};

class CStartRealMonitorReq
{
public:
    CStartRealMonitorReq(void) : m_clChannelID(),
                                 m_bRemoteFlag(false)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_START_REALMONITOR_REQUEST;
    };
    
    // 不可继承
    ~CStartRealMonitorReq(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void) { return m_stProtocolHead; };
    
    void setChannelID(const CChannelID& clChannelID) { m_clChannelID = clChannelID; };
    void setRemoteFlag(bool bRemoteFlag)             { m_bRemoteFlag = bRemoteFlag; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        m_clChannelID.serializeToStream(clOS);
        clOS << (bool)true;
        clOS << m_bRemoteFlag;
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    CChannelID      m_clChannelID;              // 通道ID
    bool            m_bRemoteFlag;              // 远程设备标志
};

class CStartRealMonitorResp
{
public:
    CStartRealMonitorResp(void) : m_iResult(CMED_OPERATE_FAILED),
                                  m_clChannelID(),
                                  m_clAuthCode(),   
                                  m_clVideoSrvIP(),
                                  m_clUserName(),
                                  m_clPassWord(),
                                  m_usVideoSrvPort(0)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
    };
    
    // 不可继承
    ~CStartRealMonitorResp(void)
    {
        // Do Nothing
    };
    
    SProtocolHead&    getProtocolHead(void)     { return m_stProtocolHead; };
    int               getResult(void)           { return m_iResult; };
    const CChannelID& getChannelID(void)        { return m_clChannelID; };
    const KK_StringA& getAuthCode(void)         { return m_clAuthCode; };
    const KK_StringA& getVideoSrvIP(void)       { return m_clVideoSrvIP; };
    const KK_StringA& getUserName(void)         { return m_clUserName; };
    const KK_StringA& getPassWord(void)         { return m_clPassWord; };
    u_short           getVideoSrvPort(void)     { return m_usVideoSrvPort; };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        bool bThrough = false;
        
        clIS >> m_stProtocolHead;
        clIS >> m_iResult;
        m_clChannelID.deserializeFromStream(clIS);
        if ( CMED_OPERATE_SUCCESS == m_iResult )
        {
            clIS >> m_clAuthCode;
            clIS >> m_clVideoSrvIP;
            clIS >> m_usVideoSrvPort;
            clIS >> bThrough;
            clIS >> m_clUserName;
            clIS >> m_clPassWord; 
        }
        
        return clIS;
    };
    
    KK_MemoryStream& serializeToMediaStream(KK_MemoryStream& clOS)
    {
        clOS << m_clAuthCode;
        clOS << (bool)true;
        
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;                  // 返回值
    CChannelID      m_clChannelID;              // 通道ID
	KK_StringA      m_clAuthCode;               // 监看认证码
	KK_StringA      m_clVideoSrvIP;             // 视频服务的IP
	KK_StringA      m_clUserName;               // 视频服务的用户名
	KK_StringA      m_clPassWord;               // 视频服务的密码
	u_short         m_usVideoSrvPort;           // 视频服务的端口
};

class CStopRealMonitorReq
{
public:
    CStopRealMonitorReq(void) : m_clChannelID(),
                                m_clAuthCode()
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_STOP_REALMONITOR_REQUEST;
    };
    
    // 不可继承
    ~CStopRealMonitorReq(void)
    {
        // Do Nothing
    };
    
    SProtocolHead& getProtocolHead(void) { return m_stProtocolHead; };
    
    void setChannelID(const CChannelID& clChannelID) { m_clChannelID = clChannelID; };
    void setAuthCode(const KK_StringA& clAuthCode)   { m_clAuthCode = (LPCSTR)clAuthCode; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        m_clChannelID.serializeToStream(clOS);
        clOS << m_clAuthCode;
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    CChannelID      m_clChannelID;              // 通道ID
    KK_StringA      m_clAuthCode;               // 监看认证码
};

class CStopRealMonitorResp
{
public:
    CStopRealMonitorResp(void) : m_iResult(CMED_OPERATE_FAILED),
                                 m_clChannelID(),
                                 m_clAuthCode()
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
    };
    
    // 不可继承
    ~CStopRealMonitorResp(void)
    {
        // Do Nothing
    };
    
    SProtocolHead&    getProtocolHead(void)     { return m_stProtocolHead; };
    int               getResult(void)           { return m_iResult; };
    const CChannelID& getChannelID(void)        { return m_clChannelID; };
    const KK_StringA& getAuthCode(void)         { return m_clAuthCode; };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        clIS >> m_stProtocolHead;
        clIS >> m_iResult;
        m_clChannelID.deserializeFromStream(clIS);
        clIS >> m_clAuthCode;
        
        return clIS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;                  // 返回值
    CChannelID      m_clChannelID;              // 通道ID
	KK_StringA      m_clAuthCode;               // 监看认证码
};

class CSetMonitorModel
{
public:
    CSetMonitorModel(void) :  m_clChannelID(),
                              m_iMonitorModel(0),
                              m_iFrameCnt(0)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_SET_MONITOR_MODEL;
    };
    
    // 不可继承
    ~CSetMonitorModel(void)
    {
        // Do Nothing
    };
    
    void setChannelID(const CChannelID& clChannelID) { m_clChannelID = clChannelID; };
    void setMonitorModel(int iMonitorModel)          { m_iMonitorModel = iMonitorModel; };
    void setFrameCnt(int iFrameCnt)                  { m_iFrameCnt = iFrameCnt; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_stProtocolHead;
        clOS << m_clChannelID.GetDevID();
        clOS << m_clChannelID.GetChannelID();
        clOS << m_iMonitorModel;
        clOS << m_iFrameCnt;
        
        return clOS;
    };
    
private:
    SProtocolHead   m_stProtocolHead;           // 协议头
    CChannelID      m_clChannelID;              // 通道ID
    int             m_iMonitorModel;            // 监看模式
    int             m_iFrameCnt;                // 帧率
};

////////////////////////////////////////////////////////////////
//  用户相关的数据序列化&反序列化
class CLoginReq
{
public:
    CLoginReq(void): m_ulLocalAddr(0),
                     m_usPort(0)
    {
        m_stProtocolHead.usMDPDU = PDU_DVS_LOGIN_REQUES;
    };
 
    KK_MemoryStream& serializeToStream(KK_MemoryStream& os)
    {
        os << m_stProtocolHead;
        os << m_strUser;
        os << m_ulLocalAddr;
        os << m_usPort;
        return os;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    KK_StringW      m_strUser;                  // 用户名
    ULONG_32        m_ulLocalAddr;              // 本地ip
    u_short         m_usPort;                   // 本地端口
    
};

class CLoginResp
{
public:
    CLoginResp(void): m_iResult(CMED_OPERATE_FAILED),
                      m_strUser(),
                      m_iYear(0),             
                      m_iMonth(0),            
                      m_iDay(0),
                      m_iDayOfWeek(0),
                      m_iHour(0),
                      m_iminute(0),
                      m_iSecond(0),
                      m_iMilliSeconds(0)
    {
        m_stProtocolHead.usMDPDU = PDU_DVS_LOGIN_REQUES;
        memset(m_aucEncryptKey, 0, sizeof(u_char) * 32);
    };
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_iResult;
        if (CMED_OPERATE_SUCCESS == m_iResult)
        {
            is >> m_strUser;
            is >> m_iYear;
            is >> m_iMonth;
            is >> m_iDay;
            is >> m_iDayOfWeek;
            is >> m_iHour;
            is >> m_iminute;
            is >> m_iSecond;
            is >> m_iMilliSeconds;
            is.ReadBytes(m_aucEncryptKey, 32);
        }
        
        return is;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;
    KK_StringW      m_strUser;                  // 用户名
    WORD             m_iYear;                    // 年
    WORD             m_iMonth;                   // 月
    WORD             m_iDay;
    WORD             m_iDayOfWeek;
    WORD             m_iHour;
    WORD             m_iminute;
    WORD             m_iSecond;
    WORD             m_iMilliSeconds;
    u_char          m_aucEncryptKey[32];
};

class CAuthReq
{
  
public:
    CAuthReq(void)
    {
        m_stProtocolHead.usMDPDU = PDU_DVS_AUTH_REQUEST;
        memset(m_aucKey, 0, sizeof(u_char) * 32);

    };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& os)
    {
        os << m_stProtocolHead;
        os << m_strUser;
        os.WriteBytes(m_aucKey, 32);
        return os;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    KK_StringW      m_strUser;
    u_char          m_aucKey[32];
};

class CAuthResp
{
public:
    CAuthResp(void):m_iResult(0)
    {
        m_stProtocolHead.usMDPDU = PDU_DVS_AUTH_RESPONSE;
    };
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_iResult;
        return is;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头  
    int m_iResult;
};

class CGetDevInfoReq
{
public:
    CGetDevInfoReq(void):m_iGroupId(1)
    {
        m_stProtocolHead.usMDPDU = PDU_MINICLT_GETINFO_BY_GROUPID_REQ;
  
    }
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& os)
    {
        os << m_stProtocolHead;
        os << m_iGroupId;
        return os;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    int m_iGroupId;
};

class CGetDevInfoResp
{
public:
    CGetDevInfoResp(void)
    {
        m_stProtocolHead.usMDPDU = PDU_MINICLT_GETINFO_BY_GROUPID_REPLY;
    };
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_iResult;
        is >> m_iGroupId;
        is >> m_lstDevInfo;
        is >> m_lstGroupInfo;
        
        return is;
    };
    
    SProtocolHead    m_stProtocolHead;           // 协议头
    int              m_iResult;
    int              m_iGroupId;
    CGetInfoList     m_lstDevInfo;
    CGetInfoList     m_lstGroupInfo;
};

class CGetChannelInfoReq
{
public:
    CGetChannelInfoReq(void):m_uiDevId(0)
    {
        m_stProtocolHead.usMDPDU = PDU_MINICLT_GETINFO_BY_DEVID_REQ;
        
    }
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& os)
    {
        os << m_stProtocolHead;
        os << m_uiDevId;
        return os;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    u_int m_uiDevId;
};

class CGetChannelInfoResp
{
public:
    CGetChannelInfoResp(void)
    {
        m_bUnderWCHCtrl = true;
        m_stProtocolHead.usMDPDU = PDU_MINICLT_GETINFO_BY_DEVID_REPLY;
    };
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_iResult;
        is >> m_uiDevId;
        int iTemp;
        is >> iTemp;
        m_eStatus = (_CM_DEVICE_STATUS_)iTemp;
        is >> m_clChannelGetInfoLst;
        is >> m_strDevIp;
        is >> m_strDevSn;
        is >> m_bUnderWCHCtrl;
        
        return is;
    };
    
    SProtocolHead       m_stProtocolHead;           // 协议头
    int                 m_iResult;
    u_int               m_uiDevId;
    _CM_DEVICE_STATUS_  m_eStatus;
    CChannelGetInfoList m_clChannelGetInfoLst;
    KK_StringA          m_strDevIp;
    KK_StringA          m_strDevSn;
    bool                m_bUnderWCHCtrl;            // 是否列入公网并发控制
};

class CGetOwnerPermDataReq
{
public:
    CGetOwnerPermDataReq(void)
    {
        m_stProtocolHead.usMDPDU = PDU_GET_OWNER_PERMDATA_REQUES;
        
    }
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& os)
    {
        os << m_stProtocolHead;
        return os;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    
};

class CGetOwnerPermDataResp
{
public:
    CGetOwnerPermDataResp(void) : m_iResult(0),
                                  m_bMonitorPerm(false),
                                  m_bPTZCtrlPerm(false)
    {
        m_stProtocolHead.usMDPDU = PDU_GET_OWNER_PERMDATA_REPLY;
        
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_iResult;
        is >> m_bMonitorPerm;
        if ( is.GetUnusedBufSize() )
        {
            is >> m_bPTZCtrlPerm;
        }
        
        return is;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    int             m_iResult;
    bool            m_bMonitorPerm;
    bool            m_bPTZCtrlPerm;
};

class CPermChangeNotify
{
public:
    CPermChangeNotify(void) : m_bMonitorPerm(false),
                              m_bPTZCtrlPerm(false)
    {
        m_stProtocolHead.usMDPDU = PDU_NOTIFY_PERM_CHANGE;
        
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_bMonitorPerm;
        if ( is.GetUnusedBufSize() )
        {
            is >> m_bPTZCtrlPerm;
        }
        
        return is;
    };
    
    SProtocolHead   m_stProtocolHead;           // 协议头
    bool            m_bMonitorPerm;
    bool            m_bPTZCtrlPerm;
};

class CDevStatusNotify
{
public:
    
    SProtocolHead       m_stProtocolHead;           // 协议头
    u_int               m_uiDevId;
    _CM_DEVICE_STATUS_  m_eStatus;
    KK_StringA          m_strDevIp;
    
    CDevStatusNotify(void)
    {
        m_stProtocolHead.usMDPDU = PDU_MINICLT_DEV_STATUS_NOTIFY;
        m_uiDevId = 0;
        m_eStatus = DEV_OFFLINE;
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_uiDevId;
        int iTemp;
        is >> iTemp;
        m_eStatus = (_CM_DEVICE_STATUS_)iTemp;
        is >> m_strDevIp;
        return is;
    }
};

class CDevNameChangeNotify
{
public:
    
    SProtocolHead       m_stProtocolHead;           // 协议头
    u_int               m_uiDevId;
    NSString*           m_strDevName;
    
    CDevNameChangeNotify(void) : m_uiDevId(INVALID_DEV_ID),
                                 m_strDevName(nil)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_MINICLT_DEV_NAME_CHANGE_NOTIFY;
    }
    
    virtual ~CDevNameChangeNotify(void)
    {
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_uiDevId;
        KK_StringU clDevNameU;
        is >> clDevNameU;
        
        m_strDevName = [NSString stringWithUTF8String:(const char *)clDevNameU];
        
        return is;
    }    
};

class CChannelNameChangeNotify
{
public:
    
    SProtocolHead       m_stProtocolHead;           // 协议头
    u_int               m_uiDevId;
    u_int               m_uiChannelId;
    NSString*           m_strChannelName;
    
    CChannelNameChangeNotify(void) : m_uiDevId(INVALID_DEV_ID),
                                     m_uiChannelId(0),
                                     m_strChannelName(nil)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_MINICLT_CHNL_NAME_CHANGE_NOTIFY;
    }
    
    virtual ~CChannelNameChangeNotify(void)
    {
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_uiDevId;
        is >> m_uiChannelId;
        KK_StringU clChannelNameU;
        is >> clChannelNameU;
        
        m_strChannelName = [NSString stringWithUTF8String:(const char*)clChannelNameU];
        
        return is;
    }    
};

class CDevCtrlPtzReq
{
public:
    SProtocolHead       m_stProtocolHead;        // 协议头
    CChannelID          m_clChannelID;
    int                 m_iCtrlType;
    int                 m_iIntensity;
    
    CDevCtrlPtzReq(void) : m_clChannelID(),
                           m_iCtrlType(0),
                           m_iIntensity(0)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_DEV_CTRL_PTZ_REQ;
    }
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& os)
    {
        os << m_stProtocolHead;
        m_clChannelID.serializeToStream(os);
        os << m_iCtrlType;
        os << m_iIntensity;
        return os;
    }
};

class CDevCtrlPtzResp
{
public:
    SProtocolHead       m_stProtocolHead;        // 协议头
    int                 m_iResult;
    int                 m_iCtrlType;
    int                 m_iIntensity;
    CChannelID          m_clChannelID;
    
    CDevCtrlPtzResp(void) : m_iResult(CMED_OPERATE_FAILED),
                            m_iCtrlType(0),
                            m_iIntensity(0),
                            m_clChannelID()
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_DEV_CTRL_PTZ_REPLY;
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        m_iResult    = is.ReadInt();
        m_clChannelID.deserializeFromStream(is);
        m_iCtrlType  = is.ReadInt();
        m_iIntensity = is.ReadInt();
        
        return is;
    }
};

class CAddDevNotify
{
public:
    SProtocolHead       m_stProtocolHead;        // 协议头
    u_int               m_uiResult;
    NSString*           m_strDevName;
    u_int               m_uiDevId;
    
    CAddDevNotify(void) : m_uiResult(CMED_OPERATE_FAILED),
                          m_strDevName(nil),
                          m_uiDevId(INVALID_DEV_ID)
    {
        memset(&m_stProtocolHead, 0, sizeof(m_stProtocolHead));
        m_stProtocolHead.usMDPDU = PDU_ADDDEV_NOTIFY;
    }
    
    virtual ~CAddDevNotify(void)
    {
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_uiResult;
        KK_StringU str;
        is >> str;
        is >> m_uiDevId;
        
        m_strDevName = [NSString stringWithUTF8String:(const char *)str];
        
        return is;
    }
    
    
};

class CDelDevNotify
{
public:
    SProtocolHead       m_stProtocolHead;        // 协议头
    u_int               m_uiResult;
    u_int               m_uiDevId;
    u_int               m_uiGroupId;
    
    CDelDevNotify(void)
    {
        m_stProtocolHead.usMDPDU = PDU_DELDEV_NOTIFY;
        m_uiDevId = 0;
        m_uiGroupId = 0;
    }
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& is)
    {
        is >> m_stProtocolHead;
        is >> m_uiResult;
        is >> m_uiDevId;
        is >> m_uiGroupId;
        return is;
    }
    
    
};


////////////////////////////////////////////////////////////////
//  媒体服务PDU相关的定义
class CListenNewTask
{
public:
    CListenNewTask(void) : m_iResult(CMED_OPERATE_SUCCESS),
    m_clAuthCode()
    {
        // Do Nothing
    };
    
    ~CListenNewTask(void)
    {
        // Do Nothing
    };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_clAuthCode;
        
        return clOS;
    };
    
    KK_MemoryStream& serializeToStreamCallBack(KK_MemoryStream& clOS)
    {
        clOS << m_iResult;
        clOS << m_clAuthCode;
        
        return clOS;
    };
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& clIS)
    {
        clIS >> m_iResult;
        clIS >> m_clAuthCode;
        
        return clIS;
    };
    
    void setAuthCode(const KK_StringA& clAuthCode) { m_clAuthCode = (LPCSTR)clAuthCode; };
    
    int               getResult(void)   { return m_iResult; };
    const KK_StringA& getAuthCode(void) { return m_clAuthCode; };
    
private:
    int         m_iResult;
    KK_StringA  m_clAuthCode;
};

class CRealMonitorNewTask
{
public:
    CRealMonitorNewTask(void) : m_iResult(CMED_OPERATE_SUCCESS),
                                m_clAuthCode(),
                                m_bSend(false)
    {
        // Do Nothing
    };
    
    ~CRealMonitorNewTask(void)
    {
        // Do Nothing
    };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_clAuthCode;
        clOS << (bool)true;
        
        return clOS;
    };
    
    KK_MemoryStream& serializeToStreamCallBack(KK_MemoryStream& clOS)
    {
        clOS << m_iResult;
        clOS << m_clAuthCode;
        clOS << (bool)true;
        
        return clOS;
    };
    
    KK_ByteStream& deserializeToStream(KK_ByteStream& clIS)
    {
        clIS >> m_iResult;
        clIS >> m_clAuthCode;
        clIS >> m_bSend;
        
        return clIS;
    };
    
    void setAuthCode(const KK_StringA& clAuthCode) { m_clAuthCode = (LPCSTR)clAuthCode; };
    
    int               getResult(void)   { return m_iResult; };
    const KK_StringA& getAuthCode(void) { return m_clAuthCode; };
    bool              isSend(void)      { return m_bSend; };
    
private:
    int         m_iResult;
    KK_StringA  m_clAuthCode;
    bool        m_bSend;
};

class CRealMonitorBigData
{
public:
    CRealMonitorBigData(void) : m_paucDataBuf(NULL),
                                m_ulSeq(0),
                                m_ulLen(0),
                                m_ulRecLen(0),
                                m_usPKGCount(0),
                                m_usIndex(0),
                                m_usCmd(0),
                                m_usPKGLen(0)
    {
        // Do Nothing
    };
    
    ~CRealMonitorBigData(void)
    {
        clearData();
    };
    
    BOOL creatDataBuf(ULONG_32 ulDataLen)
    {
        BOOL bResult = NO;
        
        clearData();
        
        if ( ulDataLen > 0 )
        {
            m_paucDataBuf = (u_char*)malloc(sizeof(u_char) * ulDataLen);
            if ( NULL != m_paucDataBuf )
            {
                memset(m_paucDataBuf, 0, sizeof(u_char) * ulDataLen);
                m_ulLen = ulDataLen;
                bResult = YES;
            }
        }
        
        return bResult;
    };
    
    void clearData(void)
    {
        if ( NULL != m_paucDataBuf )
        {
            free(m_paucDataBuf);
        }
        
        m_paucDataBuf = NULL;
        m_ulSeq       = 0;
        m_ulLen       = 0;
        m_ulRecLen    = 0;
        m_usPKGCount  = 0;
        m_usIndex     = 0;
        m_usCmd       = 0;
        m_usPKGLen    = 0;
    };
    
    BOOL parseDataStream(KK_ByteStream& clIS)
    {
        BOOL      bResult     = NO;
        ULONG_32  ulSeq       = 0;
        ULONG_32  ulLen       = 0;
        u_short   usPKGCount  = 0;
        u_short   usIndex     = 0;
        u_short   usCmd       = 0;
        u_short   usPKGLen    = 0;
        
        clIS >> ulSeq;
        clIS >> usCmd;
        clIS >> ulLen;
        clIS >> usPKGCount;
        clIS >> usIndex;
        
        if ( (0 == usIndex) || (NULL == m_paucDataBuf) )
        {
            bResult = creatDataBuf(ulLen);
            if ( YES == bResult )
            {
                m_ulSeq      = ulSeq;
                m_usPKGCount = usPKGCount;
                m_usIndex    = usIndex;
                m_usCmd      = usCmd;
                
                clIS >> usPKGLen;
                clIS.ReadBytes(m_paucDataBuf, usPKGLen);
                
                m_ulRecLen += usPKGLen;
            }
        }
        else if ( (ulSeq == m_ulSeq) && (ulLen == m_ulLen) &&
                  (usCmd == m_usCmd) && (usPKGCount == m_usPKGCount) &&
                  (usIndex == (m_usIndex + 1)) )
        {
            clIS >> usPKGLen;
            clIS.ReadBytes(m_paucDataBuf + m_ulRecLen, usPKGLen);
            
            m_ulRecLen += usPKGLen;
        }
        else
        {
            clearData();
        }
        
        return bResult;
    };
    
    BOOL isReciveSuccessed(void)
    {
        return (NULL != m_paucDataBuf) && (m_ulLen == m_ulRecLen);
    };
    
    void detachDataBuf(u_char*& paucTemp, ULONG_32& ulLen)
    {
        paucTemp = m_paucDataBuf;
        ulLen    = m_ulLen;
        clearData();
    };
    
    const u_char* getData(void)     { return m_paucDataBuf; };
    ULONG_32      getDataLen(void)  { return m_ulLen; };
    
private:
    u_char*     m_paucDataBuf;
    ULONG_32    m_ulSeq;
	ULONG_32    m_ulLen;
    ULONG_32    m_ulRecLen;
	u_short     m_usPKGCount;
	u_short     m_usIndex;
	u_short     m_usCmd;
	u_short     m_usPKGLen;
};

////////////////////////////////////////////////////////////////
//  通用函数实现
static KK_MemoryStream& operator<< (KK_MemoryStream& clOS, const SProtocolHead& stHead)
{
    clOS << stHead.usMDPDU;
    clOS << stHead.ulClientModuleID;
    clOS << stHead.ulReservered1;
    clOS << stHead.ulReservered2;
    
    return clOS;
};

static KK_ByteStream& operator>> (KK_ByteStream& clIS, SProtocolHead& stHead)
{
    clIS >> stHead.usMDPDU;
    clIS >> stHead.ulClientModuleID;
    clIS >> stHead.ulReservered1;
    clIS >> stHead.ulReservered2;
    
    return clIS;
};

static ULONG_32 makeWohlePDUKeyValue(u_short usIndexSrvMDID, u_short usMDPDU)
{
    ULONG_32 ulPDUValue = usIndexSrvMDID;
    
    ulPDUValue <<= 16;
    ulPDUValue  |= usMDPDU;
    
    return ulPDUValue;
};

static KK_ByteStream& operator >> (KK_ByteStream& is, CGetInfo& clGetInfo)
{
    KK_StringU clNameU;
    is >> clGetInfo.m_uiID;
    is >> clNameU;
    clGetInfo.m_eStatus = (_CM_DEVICE_STATUS_)is.ReadInt();
    clGetInfo.SetName([NSString stringWithUTF8String:(const char*)clNameU]);
    
    return is;
};

static KK_ByteStream& operator >> (KK_ByteStream& is, CChannelGetInfo& clChannelGetInfo)
{
    KK_StringU clChannelNameU;
    is >> clChannelGetInfo.m_uiChannelID;
    is >> clChannelNameU;
    clChannelGetInfo.SetChannelName([NSString stringWithUTF8String:(const char*)clChannelNameU]);
    
    return is;
};








