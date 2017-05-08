//
//  SystemResourceDef.h
//  CMModuleCreator
//
//  Created by Kai Liu on 12-6-8.
//  Copyright (c) 2012年 Seegle. All rights reserved.
//
//  文件描述 : 
//     1. 定义通用自定义数据类型；
//     2. 定义通用常量、宏。

#ifndef CMModuleCreator_SystemResourceDef_h
#define CMModuleCreator_SystemResourceDef_h

#import "KK_Includes.h"
#import "KK_Templet.h"
#import "KK_MemoryStream.h"
#ifdef DEBUG
#define InterLog    NSLog
#else
#define InterLog
#endif

///////////////////////////////////////////////////////////////
//  兼容64位
#if defined(__LP64__) && __LP64__
typedef unsigned int  ULONG_32;
#else
typedef unsigned long ULONG_32;
#endif

///////////////////////////////////////////////////////////////
//  常量&宏定义
const u_int INVALID_DEV_ID      = 0;            // 无效设备ID
const u_int INVALID_CHANNEL_ID  = 0;            // 无效通道ID
const u_int INVALID_GROUP_ID    = 0xFFFFFFFF;   // 无效设备组ID
const u_int ROOT_GROUP_ID       = 0;            // 根组ID
const u_int INVALID_ACCOUNT_ID  = 0xFFFFFFFF;   // 无效帐号ID
const u_int VERSION_FILED_COUNT = 6;            // 版本号字段数

const u_short SUB_DEVICE_CHANNLE = 0;           // 监控内部，通道的子设备类型(SUBDEV_CHANNEL)

#define CM_CLIENT_VERSION "V1.1.0.2016.11.07"

#define SWITCH_ON                   (1)
#define SWITCH_OFF                  (0)
#define MEDIA_RMONITOR_SHARE_MEMORY SWITCH_OFF

///////////////////////////////////////////////////////////////
//  云台控制命令
enum _PTZ_CTL_TYPE_
{	
    PTZ_CTL_STOP= 1,        // 停止
	PTZ_CTL_UP,             // 上
    PTZ_CTL_DOWN,           // 下
	PTZ_CTL_LEFT,           // 左
	PTZ_CTL_RIGHT,          // 右
    PTZ_CTL_IRISADD,		// 光圈
    PTZ_CTL_IRISSUB,
    PTZ_CTL_ZOOMADD,		// 变倍
    PTZ_CTL_ZOOMSUB,		// 
	PTZ_CTL_FOCUSADD,		// 焦距
	PTZ_CTL_FOCUSSUB,
	PTZ_CTL_LIGHTON,		//灯光开
	PTZ_CTL_LIGHTOFF,		//灯光关
	PTZ_CTL_BRUSHON,		//雨刮开
	PTZ_CTL_BRUSHOFF,		//雨刮关
    
	PTZ_CTL_UP_STOP,		//上-停	
	PTZ_CTL_DOWN_STOP,		//下-停	
	PTZ_CTL_LEFT_STOP,		//左-停
	PTZ_CTL_RIGHT_STOP,		//右-停
	PTZ_CTL_FOCUSADD_STOP,	//聚焦+ -停
	PTZ_CTL_FOCUSSUB_STOP,	//聚焦- -停
	PTZ_CTL_IRISADD_STOP,	//光圈+ -停
	PTZ_CTL_IRISSUB_STOP,	//光圈- -停
	PTZ_CTL_ZOOMADD_STOP,	//变倍+ -停
	PTZ_CTL_ZOOMSUB_STOP,	//变倍- -停	
    
	PTZ_CTL_GOTO_POSTION,	//调预置位
	PTZ_CTL_ROTATION_START, //自动巡航开始
	PTZ_CTL_ROTATION_STOP,  //自动巡航停止
    
	PTZ_CTL_POS_CRUISE_START,	// 预置位巡航开始
	PTZ_CTL_POS_CRUISE_STOP		// 预置位巡航停止
};

///////////////////////////////////////////////////////////////
//  监控前端设备在线状态
enum _CM_DEVICE_STATUS_
{
    DEV_OFFLINE,                                // 不在线
	DEV_NORMAL,                                 // 正常
	DEV_ABNORMAL,                               // 异常
};

///////////////////////////////////////////////////////////////
//  监看模式
enum _REAL_MONITOR_MODEL_
{
    RMMD_FLUENT  = (0),                         // 流畅优先，分辨率为QCIF
    RMMD_LEGIBLE = (1)                          // 清晰优先，分辨率为CIF
};

///////////////////////////////////////////////////////////////
//  通道码流类型
enum _CODE_STREAM_TYPE_
{
    CST_MAIN = (0),                             // 主码流
    CST_VICE = (1)                              // 辅码流
};

///////////////////////////////////////////////////////////////
//  监控前端设备通道ID
class CChannelID
{
public:
    CChannelID(void) : m_uiDevID(INVALID_DEV_ID),
                       m_uiChannelID(INVALID_CHANNEL_ID),
                       m_eCodeStreamType(CST_VICE)
    {
        // Do Nothing
    };
    
    CChannelID(const CChannelID& clSrc)
    {
        *this = clSrc;
    };
    
    ~CChannelID(void)
    {
        // Do Nothing
    };
    
    CChannelID& operator= (const CChannelID& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiDevID         = clSrc.m_uiDevID;
            m_uiChannelID     = clSrc.m_uiChannelID;
            m_eCodeStreamType = clSrc.m_eCodeStreamType;
        }
        
        return *this;
    };
    
///////////////////////////////////////////////////////////////
//  For m_uiDevID&m_uiChannelID
    void SetChannelID(u_int uiDevID, u_int uiChannelID, _CODE_STREAM_TYPE_ eCodeStreamType = CST_VICE)
    {
        m_uiDevID     = uiDevID;
        m_uiChannelID = uiChannelID;
    };
    
    void SetCodeStreamType(_CODE_STREAM_TYPE_ eCodeStreamType)
    {
        m_eCodeStreamType = eCodeStreamType;
    };
    
    u_int              GetDevID(void) const          { return m_uiDevID; };
    u_int              GetChannelID(void) const      { return m_uiChannelID; };
    _CODE_STREAM_TYPE_ GetCodeStreamType(void) const { return m_eCodeStreamType; };
    
    KK_MemoryStream& serializeToStream(KK_MemoryStream& clOS)
    {
        clOS << m_uiDevID;
        clOS << m_uiChannelID;
        clOS << SUB_DEVICE_CHANNLE;
        clOS << (u_short)m_eCodeStreamType;

        return clOS;
    };
    
    KK_ByteStream& deserializeFromStream(KK_ByteStream& clIS)
    {
        u_short usSubDeviceType  = 0;
        u_short usCodeStreamType = 0;
        
        clIS >> m_uiDevID;
        clIS >> m_uiChannelID;
        clIS >> usSubDeviceType;
        clIS >> usCodeStreamType;
        
        m_eCodeStreamType = (_CODE_STREAM_TYPE_)usCodeStreamType;
        
        return clIS;
    };
    
    void setChannelKey(u_longlong ullChannelKey)
    {
        m_uiDevID     = ullChannelKey >> 32;
        m_uiChannelID = ullChannelKey & 0x00000000FFFFFFFF;
    }
    
    u_longlong channelKey(void) const
    {
        u_longlong ullChannelKey = m_uiDevID;
        
        ullChannelKey <<= 32;
        ullChannelKey  |= m_uiChannelID;
        
        return ullChannelKey;
    };
    
private:
    u_int               m_uiDevID;
    u_int               m_uiChannelID;
    _CODE_STREAM_TYPE_  m_eCodeStreamType;
};

///////////////////////////////////////////////////////////////
//  监控前端设备通道信息
class CChannelInfo
{
public:
    CChannelInfo(void) : m_clChannelID(),
                         m_pclChannelName(nil)
    {
        // Do Nothing
    };
    
    CChannelInfo(const CChannelInfo& clSrc)
    {
        *this = clSrc;
    };
    
    // 不可派生
    ~CChannelInfo(void)
    {
    };
    
    CChannelInfo& operator= (const CChannelInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_clChannelID = clSrc.m_clChannelID;
            SetChannelNameEx(clSrc.m_pclChannelName);
        }
        
        return *this;
    };
    
///////////////////////////////////////////////////////////////
//  For m_clChannelID
    void  SetChannelID(u_int uiDevID, u_int uiChannelID, _CODE_STREAM_TYPE_ eCodeStreamType = CST_VICE)
    {
        m_clChannelID.SetChannelID(uiDevID, uiChannelID, eCodeStreamType);
    };
    
    const CChannelID& GetChannelID(void) const      { return m_clChannelID; };
    CChannelID& GetChannelID(void)                  { return m_clChannelID; };
    
///////////////////////////////////////////////////////////////
//  For m_pclChannelName
    void SetChannelName(NSString* pclChnlName)
    {
        m_pclChannelName = pclChnlName;
    };
    
    void SetChannelNameEx(NSString* pclChnlName)
    {
        m_pclChannelName = [[NSString alloc] initWithString:pclChnlName];
    };
    
    NSString* GetChannelName(void)
    {
        return m_pclChannelName;
    };
    
    void SetChannelName(const KK_StringW& clChannelName)
    {
        m_pclChannelName = [NSString stringWithUTF8String: (const char*)KK_StringU(clChannelName)];
    };
    
    void GetChannelName(KK_StringW& clChannelName)
    {
        clChannelName = KK_StringU([m_pclChannelName UTF8String]);
    };
    
private:
    CChannelID  m_clChannelID;              // 通道ID
    NSString*   m_pclChannelName;           // 通道名
};

typedef KK_List<CChannelInfo, CChannelInfo&> CChannelList;

///////////////////////////////////////////////////////////////
//  监控前端设备组信息
class CDeviceGroupInfo
{
public:
    CDeviceGroupInfo(void) : m_uiFatherGroupID(INVALID_GROUP_ID),
                             m_uiGroupID(INVALID_GROUP_ID),
                             m_pclGroupName(nil)
    {
        // Do Nothing
    };
    
    CDeviceGroupInfo(const CDeviceGroupInfo& clSrc)
    {
        *this = clSrc;
    };
    
    ~CDeviceGroupInfo(void)
    {
    };
    
    CDeviceGroupInfo& operator= (const CDeviceGroupInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiFatherGroupID = clSrc.m_uiFatherGroupID;
            m_uiGroupID       = clSrc.m_uiGroupID;
            SetGroupNameEx(clSrc.m_pclGroupName);
        }
        
        return *this;
    };
    
///////////////////////////////////////////////////////////////
//  For m_uiFatherGroupID
    void  SetFatherGroupID(u_int uiFatherGroupID)    { m_uiFatherGroupID = uiFatherGroupID; };
    u_int GetFatherGroupID(void) const               { return m_uiFatherGroupID; };
    
///////////////////////////////////////////////////////////////
//  For m_uiGroupID
    void  SetGroupID(u_int uiGroupID)    { m_uiGroupID = uiGroupID; };
    u_int GetGroupID(void) const         { return m_uiGroupID; };
    
///////////////////////////////////////////////////////////////
//  For m_pclGroupName
    void SetGroupName(NSString* pclGroupName)
    {
        m_pclGroupName = pclGroupName;
    };
    
    void SetGroupNameEx(NSString* pclGroupName)
    {
        m_pclGroupName = [[NSString alloc] initWithString:pclGroupName];
    };
    
    NSString* GetGroupName(void) const
    {
        return m_pclGroupName;
    };
    
    void SetGroupName(const KK_StringW& clGroupName)
    {
        m_pclGroupName = [NSString stringWithUTF8String: (const char*)KK_StringU(clGroupName)];
    };
    
    void GetGroupName(KK_StringW& clGroupName) const
    {
        clGroupName = KK_StringU([m_pclGroupName UTF8String]);
    };
    
private:
    u_int       m_uiFatherGroupID;          // 父组ID
    u_int       m_uiGroupID;                // 组ID
    NSString*   m_pclGroupName;             // 组名
};

///////////////////////////////////////////////////////////////
//  监控前端设备信息
class CDeviceInfo
{
public:
    CDeviceInfo(void) : m_uiDevID(INVALID_DEV_ID),
                        m_uiFatherGroupID(INVALID_GROUP_ID),
                        m_eDevStatus(DEV_OFFLINE),
                        m_pclDevName(nil),
                        m_pclDevSN(nil),
                        m_pclDevIP(nil),
                        m_clChnlList(),
                        m_bRemote(false)
    {
        // Do Nothing
    };
    
    CDeviceInfo(const CDeviceInfo& clSrc)
    {
        *this = clSrc;
    };
    
    // 不可派生
    ~CDeviceInfo(void)
    {
    };
    
    CDeviceInfo& operator= (const CDeviceInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiDevID           = clSrc.m_uiDevID;
            m_uiFatherGroupID   = clSrc.m_uiFatherGroupID;
            m_eDevStatus        = clSrc.m_eDevStatus;
            m_bRemote           = clSrc.m_bRemote;
            SetDevNameEx(clSrc.m_pclDevName);
            SetDevSNEx(clSrc.m_pclDevSN);
            SetDevIPEx(clSrc.m_pclDevIP);
            SetChnlList((CChannelList*)&clSrc.m_clChnlList);
        }
        
        return *this;
    };

///////////////////////////////////////////////////////////////
//  For m_uiDevID
    void  SetDevID(u_int uiDevID)   { m_uiDevID = uiDevID; };
    u_int GetDevID(void) const      { return m_uiDevID; };
    
///////////////////////////////////////////////////////////////
//  For m_uiFatherGroupID
    void  SetFatherGroupID(u_int uiFatherGroupID)    { m_uiFatherGroupID = uiFatherGroupID; };
    u_int GetFatherGroupID(void) const               { return m_uiFatherGroupID; };

///////////////////////////////////////////////////////////////
//  For m_eDevStatus
    void                SetDevStatus(_CM_DEVICE_STATUS_ eStatus)   { m_eDevStatus = eStatus; };
    _CM_DEVICE_STATUS_  GetDevStatus(void) const                   { return m_eDevStatus; };
    
///////////////////////////////////////////////////////////////
//  For m_pclDevName
    void SetDevName(NSString* pclDevName)
    {
        m_pclDevName = pclDevName;
    };
    
    void SetDevNameEx(NSString* pclDevName)
    {
        m_pclDevName = [[NSString alloc] initWithString:pclDevName];
    };
    
    NSString* GetDevName(void) const
    {
        return m_pclDevName;
    };
    
    void SetDevName(const KK_StringW& clDevName)
    {
        m_pclDevName = [NSString stringWithUTF8String: (const char*)KK_StringU(clDevName)];
    };
    
    void GetDevName(KK_StringW& clDevName) const
    {
        clDevName = KK_StringU([m_pclDevName UTF8String]);
    };

///////////////////////////////////////////////////////////////
//  For m_pclDevSN
    void SetDevSN(NSString* pclDevSN)
    {
        m_pclDevSN = pclDevSN;
    };
    
    void SetDevSNEx(NSString* pclDevSN)
    {
        m_pclDevSN = [[NSString alloc] initWithString:pclDevSN];
    };
    
    NSString* GetDevSN(void) const
    {
        return m_pclDevSN;
    };
    
    void SetDevSN(const KK_StringW& clDevSN)
    {
        m_pclDevSN = [NSString stringWithUTF8String: (const char*)KK_StringU(clDevSN)];
    };
    
    void GetDevSN(KK_StringW& clDevSN) const
    {
        clDevSN = KK_StringU([m_pclDevSN UTF8String]);
    };
    
///////////////////////////////////////////////////////////////
//  For m_pclDevIP
    void SetDevIP(NSString* pclDevIP)
    {
        m_pclDevIP = pclDevIP;
    };
    
    void SetDevIPEx(NSString* pclDevIP)
    {
        m_pclDevIP = [[NSString alloc] initWithString:pclDevIP];
    };
    
    NSString* GetDevIP(void) const
    {
        return m_pclDevIP;
    };
    
    void SetDevIP(const KK_StringW& clDevIP)
    {
        m_pclDevIP = [NSString stringWithUTF8String: (const char*)KK_StringU(clDevIP)];
    };
    
    void GetDevIP(KK_StringW& clDevIP) const
    {
        clDevIP = KK_StringU([m_pclDevIP UTF8String]);
    };
    
///////////////////////////////////////////////////////////////
//  For m_clChnlList
    void SetChnlList(CChannelList* pclChnlList)
    {
        m_clChnlList.RemoveAll();
        if ( NULL != pclChnlList )
        {
            m_clChnlList.AddTail(pclChnlList);
        }
    };
    
    const CChannelList& GetChnlList(void) const
    {
        return m_clChnlList;
    };
    
private:
    u_int               m_uiDevID;          // 设备ID
    u_int               m_uiFatherGroupID;  // 设备组ID
    _CM_DEVICE_STATUS_  m_eDevStatus;       // 设备在线状态
    NSString*           m_pclDevName;       // 设备名
    NSString*           m_pclDevSN;         // 设备SN
    NSString*           m_pclDevIP;         // 设备IP
    CChannelList        m_clChnlList;       // 通道列表
    bool                m_bRemote;          // 远程设备标志
};

////////////////////////////////////////////////////////////////
//  从平台服务器，获取到的设备或设备组基本信息
class CGetInfo
{
public:
    u_int               m_uiID;     // 设备or设备组ID
    NSString*           m_pclName;  // 设备or设备组名
    _CM_DEVICE_STATUS_  m_eStatus;  // 设备在线状态，设备组不使用
    
    CGetInfo(void) : m_uiID(0),
                     m_pclName(nil),
                     m_eStatus(DEV_OFFLINE)
    {
        // Do Nothing
    }
    
    CGetInfo(const CGetInfo& clSrc)
    {
        *this = clSrc;
    }
    
    // 不可派生
    ~CGetInfo(void) 
    {
    }
    
    CGetInfo& operator= (const CGetInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiID    = clSrc.m_uiID;
            m_eStatus = clSrc.m_eStatus;
            SetNameEx(clSrc.m_pclName);
        }

        return *this;
    }
    
    void SetName(NSString* pclSrc)
    {
        m_pclName = pclSrc;
    }
    
    void SetNameEx(NSString* pclSrc)
    {
        m_pclName = [[NSString alloc] initWithString:pclSrc];
    }
};

typedef KK_List<CGetInfo, CGetInfo&> CGetInfoList;

////////////////////////////////////////////////////////////////
//  从平台服务器，获取到的通道基本信息
class CChannelGetInfo
{
public:
    u_int       m_uiChannelID;
    NSString*   m_pclChannelName;

    CChannelGetInfo(void) : m_uiChannelID(0),
                            m_pclChannelName(nil)
    {
        // Do Nothing
    }
    
    CChannelGetInfo(const CChannelGetInfo& clSrc)
    {
        *this = clSrc;
    }
    
    ~CChannelGetInfo(void)
    {
    }
    
    CChannelGetInfo& operator = (const CChannelGetInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiChannelID = clSrc.m_uiChannelID;
            SetChannelNameEx(clSrc.m_pclChannelName);
        }

        return *this;
    }
    
    void SetChannelName(NSString* pclSrc)
    {
        m_pclChannelName = pclSrc;
    }
    
    void SetChannelNameEx(NSString* pclSrc)
    {
        m_pclChannelName = [[NSString alloc] initWithString:pclSrc];
    }
};

typedef KK_List<CChannelGetInfo, CChannelGetInfo&> CChannelGetInfoList;

////////////////////////////////////////////////////////////////
//  从平台服务器，获取到的设备组信息，包括下属设备信息和下属设备组信息
class CDeviceGroupGetInfo
{
public:
    u_int        m_uiGroupID;
    CGetInfoList m_clSubGroupList;
    CGetInfoList m_clDeivceList;
    
    CDeviceGroupGetInfo(void) : m_uiGroupID(INVALID_GROUP_ID),
                                m_clSubGroupList(),
                                m_clDeivceList()
    {
        // Do Nothing
    }
    
    CDeviceGroupGetInfo(const CDeviceGroupGetInfo& clSrc)
    {
        *this = clSrc;
    }
    
    // 不可派生
    ~CDeviceGroupGetInfo(void)
    {
        // Do Nothing
    }
    
    CDeviceGroupGetInfo& operator= (const CDeviceGroupGetInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiGroupID = clSrc.m_uiGroupID;
            
            m_clSubGroupList.RemoveAll();
            m_clSubGroupList.AddTail((CGetInfoList*)&clSrc.m_clSubGroupList);
                        
            m_clDeivceList.RemoveAll();
            m_clDeivceList.AddTail((CGetInfoList*)&clSrc.m_clDeivceList);
        }
        
        return *this;
    }
};

////////////////////////////////////////////////////////////////
//  从平台服务器，获取到的设备信息
class CDevChannelInfo
{
 public:
    u_int               m_uiDevID;
    _CM_DEVICE_STATUS_  m_eStatus;
    CChannelGetInfoList m_clChannelGetInfoList;
    NSString*           m_pclDevIP;
    NSString*           m_pclDevSN;
    bool                m_bRemote;                  // 远程设备标志
    bool                m_bUnderWCHCtrl;            // 是否列入公网并发控制
    
    CDevChannelInfo(void) : m_uiDevID(INVALID_DEV_ID),
                            m_eStatus(DEV_OFFLINE),
                            m_clChannelGetInfoList(),
                            m_pclDevIP(nil),
                            m_pclDevSN(nil),
                            m_bRemote(false),
                            m_bUnderWCHCtrl(false)
    {
        // Do Nothing
    }
    
    CDevChannelInfo(const CDevChannelInfo& clSrc)
    {
        *this = clSrc;
    }
    
    // 不可派生
    ~CDevChannelInfo(void)
    {
        // Do Nothing
    }
    
    CDevChannelInfo& operator = (const CDevChannelInfo& clSrc)
    {
        if ( this != &clSrc )
        {
            m_uiDevID       = clSrc.m_uiDevID;
            m_eStatus       = clSrc.m_eStatus;
            m_bRemote       = clSrc.m_bRemote;
            m_bUnderWCHCtrl = clSrc.m_bUnderWCHCtrl;
            SetDeivceIPEx(clSrc.m_pclDevIP);
            SetDeviceSNEx(clSrc.m_pclDevSN);
            
            m_clChannelGetInfoList.RemoveAll();
            m_clChannelGetInfoList.AddTail((CChannelGetInfoList*)&clSrc.m_clChannelGetInfoList);
        }
        
        return *this;
    }
    
    void SetDeivceIP(NSString* pclDevIP)
    {
        m_pclDevIP = pclDevIP;
    }
    
    void SetDeivceIPEx(NSString* pclDevIP)
    {
        m_pclDevIP = [[NSString alloc] initWithString:pclDevIP];
    }
    
    void SetDeviceSN(NSString* pclDevSN)
    {
        m_pclDevSN = pclDevSN;
    }
    
    void SetDeviceSNEx(NSString* pclDevSN)
    {
        m_pclDevSN = [[NSString alloc] initWithString:pclDevSN];
    }
};

//////////////////////////////////////////////////////////////////////////
// CClientVersionInfo
class CClientVersionInfo
{
public:
	CClientVersionInfo(void)
	{
		memset(m_aiFiledAry, 0, sizeof(int) * VERSION_FILED_COUNT);
	}
    
	bool FormatFromString(const KK_StringW& clVerInfo)
	{
        NSString* pclVerInfo = [NSString stringWithUTF8String: (const char*)KK_StringU(clVerInfo)];
        NSArray*  pclArray   = [[pclVerInfo substringFromIndex:1] componentsSeparatedByString:@"."];
        
        if ( VERSION_FILED_COUNT == [pclArray count] )
        {
            for (int iLoop = 0; iLoop < VERSION_FILED_COUNT; ++iLoop)
            {
                m_aiFiledAry[iLoop] = [[pclArray objectAtIndex:iLoop] intValue];
            }
            
            return true;
        }
        
        return false;
    }
    
	bool operator>= (const CClientVersionInfo& clRight) const
	{
        for (int iLoop = 0; iLoop < VERSION_FILED_COUNT - 1; ++iLoop)
        {
            if ( m_aiFiledAry[iLoop] < clRight.m_aiFiledAry[iLoop] )
            {
                return false;
            }
            else if ( m_aiFiledAry[iLoop] > clRight.m_aiFiledAry[iLoop] )
            {
                return true;
            }
        }
        
        if ( m_aiFiledAry[VERSION_FILED_COUNT - 1] >= clRight.m_aiFiledAry[VERSION_FILED_COUNT - 1] )
        {
            return true;
        }
        
        return false;
	}
    
	bool operator> (const CClientVersionInfo& clRight) const
	{
		for (int iLoop = 0; iLoop < VERSION_FILED_COUNT - 1; ++iLoop)
        {
            if ( m_aiFiledAry[iLoop] < clRight.m_aiFiledAry[iLoop] )
            {
                return false;
            }
            else if ( m_aiFiledAry[iLoop] > clRight.m_aiFiledAry[iLoop] )
            {
                return true;
            }
        }
        
        if ( m_aiFiledAry[VERSION_FILED_COUNT - 1] > clRight.m_aiFiledAry[VERSION_FILED_COUNT - 1] )
        {
            return true;
        }
        
        return false;
	}
    
    bool IsUpdateEnable(const KK_StringW& clVerInfo)
    {
        if ( FormatFromString(clVerInfo) )
        {
            CClientVersionInfo clLocalVer;
            if ( clLocalVer.FormatFromString(KK_StringW(CM_CLIENT_VERSION)) )
            {
                return (*this > clLocalVer);
            }
        }
        
        return false;
    }
    
private:
    int	m_aiFiledAry[VERSION_FILED_COUNT];
};
//////////////////////////////////////////////////////////////////////////

#endif
