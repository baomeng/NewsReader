//
//  CMCenterManager.m
//  SG_MonitorForIOS_SDK_Test
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "CMCenterManager.h"
#import "CMModuleCreatorProtocolDef.h"
#import "SystemErrorCodeDef.h"
#import "AppDelegate.h"

#define         MC_PTZ_CTRL_SPEED                       50
#define         DEFAULT_MONITOR_MODEL                   RMMD_LEGIBLE

@implementation CMCenterManager

@synthesize idAccountModule,idDeviceModule, idMediaModule;
@synthesize serverPort,userName,serverIp,password,currentCellList,accountHelpers,deviceHelpers,mediaHelpers, currentChannelList;
@synthesize currentGroupId,currentGroupParentId,requestGroupId,requestDeviceId;
@synthesize videoList;
@synthesize currentMonitorModel;
@synthesize logincallBackBlock;
- (id)init
{
    self = [super init];
    if (self) {

//      self.recorder = [NSMutableDictionary dictionary];
        
        self.mediaHelpers = [NSMutableArray arrayWithCapacity:0];
        self.accountHelpers = [NSMutableArray arrayWithCapacity:0];
        self.deviceHelpers = [NSMutableArray arrayWithCapacity:0];
        
        idAccountModule = [[CMModuleCreatorSingleton getModuleCreator] getAccountModule];
        [idAccountModule regAccountModuleDelegate:self];
        
        idDeviceModule = [[CMModuleCreatorSingleton getModuleCreator] getDeviceModule];
        [idDeviceModule regDeviceModuleDelegate:self];
        
        idMediaModule = [[CMModuleCreatorSingleton getModuleCreator] getMediaModule];
        [idMediaModule regMediaModuleDelegate:self];
        
        self.currentGroupId = INVALID_GROUP_ID;
        self.currentGroupParentId = INVALID_DEV_ID;
        self.requestGroupId = INVALID_GROUP_ID;
        self.requestDeviceId = INVALID_DEV_ID;
    }
    return self;
}
- (void)dealloc{
    
    [idAccountModule unAccountModuleDelegate:self];
    [idDeviceModule unDeviceModuleDelegate:self];
    [idMediaModule unRegMediaModuleDelegate:self];
    
    if (userName!=nil) {
        [userName release];
    }
    if (serverIp!=nil) {
        [serverIp release];
    }
    if (password!=nil) {
        [password release];
    }
    if (currentCellList!=nil) {
        [currentCellList release];
    }
    if (currentChannelList!=nil) {
        [currentChannelList release];
    }
    if (accountHelpers!=nil) {
        [accountHelpers release];
    }
    if (deviceHelpers!=nil) {
        [deviceHelpers release];
    }
    if (mediaHelpers!=nil) {
        [mediaHelpers release];
    }
    if(videoList!=nil){
        [videoList release];
    }
    if(logincallBackBlock!=nil){
        [logincallBackBlock release];
    }

    [super dealloc];
}

- (NSMutableArray *)videoLists
{
    if (videoList == nil) {
        videoList = [[NSMutableArray alloc] init];
    }
    return videoList;
}

// Account callback
- (void) onGetInfoByGroupID: (u_int) uiGroupId
                    withRlt: (int) iResult
              andDeviceInfo: (CGetInfoList&) lstDevInfo
            andSubGroupInfo: (CGetInfoList&) lstGroupInfo
{
    if (uiGroupId!=ROOT_GROUP_ID+1 && self.requestGroupId!=uiGroupId) {
        return;
    }
    self.requestGroupId = INVALID_GROUP_ID;
    BOOL isChildrenZero = YES;
    if (iResult==CMED_OPERATE_SUCCESS)
    {
        self.currentGroupId = uiGroupId;
        u_int grandpaId ;
        if ([idAccountModule getFatherGroupId:self.currentGroupId fatgerId:grandpaId]) {
            self.currentGroupParentId = grandpaId;
        }else{
            self.currentGroupParentId = INVALID_GROUP_ID;
        }
        
        int iDevCount = lstDevInfo.GetCount();
        int iGroupCount = lstGroupInfo.GetCount();
        
        //NSLog(@"devCount %d, groupCount %d", iDevCount, iGroupCount);
        
        NSMutableArray *newValue = [NSMutableArray arrayWithCapacity:(iDevCount+iGroupCount)];
        
        KK_POSITION pos = lstGroupInfo.GetHeadPosition();
        while(pos) {
            CGetInfo & groupInfo = lstGroupInfo.GetNext(pos);
            CMDeviceTableCellInfo *cellInfo =[[CMDeviceTableCellInfo alloc]init];
            cellInfo.type = GROUP;
            cellInfo.self_id = groupInfo.m_uiID;
            cellInfo.parent_id = uiGroupId;
            cellInfo.status = DEV_NORMAL;
            cellInfo.name = groupInfo.m_pclName;
            [newValue addObject:cellInfo];
            //NSLog(@"group name%@", cellInfo.name);
            [cellInfo release];
        }
        
        pos = lstDevInfo.GetHeadPosition();
        while(pos) {
            CGetInfo & devInfo = lstDevInfo.GetNext(pos);
            CMDeviceTableCellInfo *cellInfo =[[CMDeviceTableCellInfo alloc]init];
            cellInfo.type = DEVICE;
            cellInfo.self_id = devInfo.m_uiID;
            cellInfo.parent_id = uiGroupId;
            cellInfo.status = devInfo.m_eStatus;
            cellInfo.name = devInfo.m_pclName;
            //NSLog(@"dev name%@", cellInfo.name);
            [newValue addObject:cellInfo];
            [cellInfo release];
        }
        if (newValue.count != 0) {
            self.currentCellList = newValue;
        }
        isChildrenZero = newValue.count==0;
    }else{
        
    }
    
    NSString * strResult = [NSString stringWithFormat:@"%d",iResult];
    NSString * strGroupId = [NSString stringWithFormat:@"%d",uiGroupId];
    NSString * strIsChildrenZero = [NSString stringWithFormat:@"%i",isChildrenZero];
    NSArray *param = [NSArray arrayWithObjects:strResult,strGroupId,strIsChildrenZero, nil];
    [self performSelectorOnMainThread:@selector(handlerOnGetGroupInfo:) withObject:param waitUntilDone:NO];
    
}
-(void)getDevLst{
    [self getRootDevGroupInfo];
}

-(void)onlogin:(int)iResult user:(NSString*)strUser
{
    if (iResult==CMED_OPERATE_SUCCESS) {
        [self getRootDevGroupInfo];
    }
    NSString * strResult = [NSString stringWithFormat:@"%d",iResult];
    NSArray *param = [NSArray arrayWithObjects:strResult,strUser, nil];
    [self performSelectorOnMainThread:@selector(handlerOnLogin:) withObject:param waitUntilDone:NO];
}

-(void) onlogout:(int)iErrCode
{
    NSString * param = [NSString stringWithFormat:@"%d",iErrCode];
    [self performSelectorOnMainThread:@selector(handlerOnLogout:) withObject:param waitUntilDone:NO];
    
}
- (BOOL) onReconnect: (u_long) ulReconCnt
{
    NSString * param = [NSString stringWithFormat:@"%ld",ulReconCnt];
    [self performSelectorOnMainThread:@selector(handlerOnReconnect:) withObject:param waitUntilDone:NO];
    //   InterLog(@"reconnect count = %d",(int)ulReconCnt);
    return YES;
}
-(void)onDevNameChangeNotify:(u_int)uiDevid newName:(NSString*)strName
{
    NSString * strDevId = [NSString stringWithFormat:@"%d",uiDevid];
    NSArray *param = [NSArray arrayWithObjects:strDevId,strName, nil];
    [self performSelectorOnMainThread:@selector(handlerOnDevNameChangeNotify:) withObject:param
                        waitUntilDone:NO];
}

-(void)onAddDevNotify:(u_int)uiDevId devname:(NSString*)strName groupId:(u_int)uiGrpId
{
    NSString * strDevId = [NSString stringWithFormat:@"%d",uiDevId];
    NSString * strGrpId = [NSString stringWithFormat:@"%d",uiGrpId];
    NSArray *param = [NSArray arrayWithObjects:strDevId,strName,strGrpId, nil];
    [self performSelectorOnMainThread:@selector(handlerOnAddDevNotify:) withObject:param waitUntilDone:NO];
    [self addNewDevice:strName selfId:uiDevId parentId:uiGrpId];
    
}

-(void)onDelDevNotify:(u_int)uiDevId groupId:(u_int)uiGrpId
{
    NSString * strDevId = [NSString stringWithFormat:@"%d",uiDevId];
    NSString * strGrpId = [NSString stringWithFormat:@"%d",uiGrpId];
    NSArray *param = [NSArray arrayWithObjects:strDevId,strGrpId, nil];
    [self performSelectorOnMainThread:@selector(handlerOnDelDevNotify:) withObject:param waitUntilDone:NO];
    [self delDevice:uiDevId parentId:uiGrpId];
}

-(void)onPermChangeNotify: (bool) bCanMonitor andPTZCtrlEnable: (bool) bPTZCtrlEnable
{
    NSString * para1 = [NSString stringWithFormat:@"%d",bCanMonitor];
    NSString * para2 = [NSString stringWithFormat:@"%d",bPTZCtrlEnable];
    NSArray *param = [NSArray arrayWithObjects:para1,para2, nil];
    [self performSelectorOnMainThread:@selector(handlerOnPermChangeNotify:) withObject:param waitUntilDone:NO];
}

-(void)onSyncAccBalanceInfo:(u_long)ulCHCnt
{
    NSString * param = [NSString stringWithFormat:@"%ld",ulCHCnt];
    [self performSelectorOnMainThread:@selector(handlerOnSyncAccBalanceInfo:) withObject:param waitUntilDone:NO];
}

-  (void) regAccountHelper: (id<CMAccountManagerHelper>) helper
{
    if (![accountHelpers containsObject:helper]) {
        [accountHelpers addObject:helper];
    }
}
-  (void) unregAccountHelper: (id<CMAccountManagerHelper>) helper
{
    if ([accountHelpers containsObject:helper]) {
        [accountHelpers removeObject:helper];
    }
}

- (void) handlerOnGetGroupInfo:(id)arg
{
    NSArray *param = arg;
    int iRestult = [[param objectAtIndex:0] intValue];
    int uiGroupId = [[param objectAtIndex:1] intValue];
    BOOL isChildrenZero = [[param objectAtIndex:2] boolValue];
    if (isChildrenZero) {
        self.currentCellList=nil;
    }
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onGetDeviceTableCellInfo:iRestult parentId:uiGroupId isChildrenZero:isChildrenZero];
    }
}

- (void) handlerOnLogin:(id)arg
{
    NSArray *param = arg;
    int iResult = [[param objectAtIndex:0] intValue];
    NSString *strUser = [param objectAtIndex:1];
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onlogin:iResult user:strUser];
    }
    
 //   logincallBackBlock(iResult, strUser);
}

-(void) handlerOnLogout:(id)arg
{
    NSString *param = arg;
    int errorCode = [param intValue];
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onLogout:errorCode];
    }
    
}

- (void) handlerOnReconnect:(id)arg
{
    NSString *param = arg;
    int ulReconCnt = [param intValue];
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onReconnect:ulReconCnt];
    }
}

- (void) handlerOnDevNameChangeNotify:(id)arg
{
    NSArray *param = arg;
    int devId = [[param objectAtIndex:0] intValue];
    NSString *name = [param objectAtIndex:1];
    
    int ids[1] ={devId};
    BOOL isNeedReload = [self setCellName:name cellId:ids type:DEVICE];
    
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onDevNameChangeNotify:devId newName:name needReload:isNeedReload];
    }
}
- (void) handlerOnAddDevNotify:(id)arg
{
    NSArray *param = arg;
    int devId = [[param objectAtIndex:0] intValue];
    NSString * name = [param objectAtIndex:1];
    int grpId = [[param objectAtIndex:2] intValue];
    
    BOOL isNeedReload = [self addNewDevice:name selfId:devId parentId:grpId];
    
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onAddDevNotify:devId devname:name groupId:grpId needReload:isNeedReload];
    }
}
- (void) handlerOnDelDevNotify:(id)arg
{
    NSArray *param = arg;
    int devId = [[param objectAtIndex:0] intValue];
    int grpId = [[param objectAtIndex:1] intValue];
    
    BOOL isNeedReload = [self delDevice:devId parentId:grpId];
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onDelDevNotify:devId groupId:grpId needReload:isNeedReload];
    }
}

- (void) handlerOnPermChangeNotify:(id)arg
{
    NSArray *param = arg;
    BOOL para1 = [[param objectAtIndex:0] boolValue];
    BOOL para2 = [[param objectAtIndex:1] boolValue];
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
        [helper onPermChangeNotify:para1 andPTZCtrlEnable:para2];
    }
    
}
- (void) handlerOnSyncAccBalanceInfo:(id)arg
{
    NSString *param = arg;
    int ulCHCnt = [param intValue];
//  CMAppDelegate *appDelegate = (CMAppDelegate *)[UIApplication sharedApplication].delegate;
//  [appDelegate.playerDataCenter updateAccBalanceInfo:ulCHCnt];
    for (id<CMAccountManagerHelper> helper in accountHelpers) {
            [helper onSyncAccBalanceInfo:ulCHCnt];
    }
}

// // Device callback
- (void) onGetInfoByID: (u_int) uiDevId
            withResult: (int) iResult
         andChannelLst: (CChannelGetInfoList&) lstChannelInfo
              andDevIP: (NSString*) pclDevIP
              andDevSN: (NSString*) pclDevSN
{
    if (self.requestDeviceId!=uiDevId) {
        return;
    }
    self.requestDeviceId = INVALID_DEV_ID;
    BOOL isChildrenZero = YES;
    if (iResult==CMED_OPERATE_SUCCESS)
    {
        self.currentGroupId = uiDevId;
        u_int grandpaId ;
        if ([idAccountModule getGroupIdByDevId:self.currentGroupId groupId:grandpaId]) {
            self.currentGroupParentId = grandpaId;
        }else{
            self.currentGroupParentId = INVALID_GROUP_ID;
        }
        NSMutableArray *newValue = [NSMutableArray arrayWithCapacity:lstChannelInfo.GetCount()];
        KK_POSITION pos = lstChannelInfo.GetHeadPosition();
        while(pos) {
            CChannelGetInfo & channelInfo = lstChannelInfo.GetNext(pos);
            CMDeviceTableCellInfo *cellInfo =[[CMDeviceTableCellInfo alloc]init];
            cellInfo.type = CHANNEL;
            cellInfo.self_id = channelInfo.m_uiChannelID;
            cellInfo.parent_id = uiDevId;
            //      cellInfo.status = channelInfo.eStatus;
            cellInfo.name = channelInfo.m_pclChannelName;
            [newValue addObject:cellInfo];
            
            [cellInfo release];
        }
        if (newValue.count != 0) {
            //self.currentCellList = newValue;
            self.currentChannelList = newValue;
        }
        isChildrenZero = newValue.count==0;
    }else{
        
    }
    NSString * strResult = [NSString stringWithFormat:@"%d",iResult];
    NSString * strGroupId = [NSString stringWithFormat:@"%d",uiDevId];
    NSString * strIsChildrenZero = [NSString stringWithFormat:@"%i",isChildrenZero];
    NSArray *param = [NSArray arrayWithObjects:strResult,strGroupId,strIsChildrenZero, nil];
    [self performSelectorOnMainThread:@selector(handlerOnGetDeviceInfo:) withObject:param waitUntilDone:NO];
    
}

-(void)onDevStatusNotify:(u_int)uiDevId status:(_CM_DEVICE_STATUS_)eStatus
{
    NSString * strDevId = [NSString stringWithFormat:@"%d",uiDevId];
    NSString * status = [NSString stringWithFormat:@"%d",eStatus];
    NSArray *param = [NSArray arrayWithObjects:strDevId,status, nil];
    [self performSelectorOnMainThread:@selector(handlerOnDevStatusNotify:) withObject:param waitUntilDone:NO];
}

-(void)onChannelNameChangeNotify:(u_int)uiDevId channelId: (u_int)uiChannelId newName:(NSString*)strName
{
    NSString * strDevId = [NSString stringWithFormat:@"%d",uiDevId];
    NSString * strChannelId = [NSString stringWithFormat:@"%d",uiChannelId];
    NSArray *param = [NSArray arrayWithObjects:strDevId,strChannelId,strName, nil];
    [self performSelectorOnMainThread:@selector(handlerOnChannelNameChangeNotify:) withObject:param waitUntilDone:NO];
}
- (void) onPTZCtrl: (const CChannelID&) clChannelID
        withResult: (int) iResult
      withCtrlType: (int) iCtrlType
     withIntensity: (int) iIntensity
{
    
}

-  (void) regDeviceHelper: (id<CMDeviceManagerHelper>) helper
{
    if (![deviceHelpers containsObject:helper]) {
        [deviceHelpers addObject:helper];
    }
}
-  (void) unregDeviceHelper: (id<CMDeviceManagerHelper>) helper
{
    if ([deviceHelpers containsObject:helper]) {
        [deviceHelpers removeObject:helper];
    }
}

- (void) handlerOnGetDeviceInfo:(id)arg
{
    NSArray *param = arg;
    int iRestult = [[param objectAtIndex:0] intValue];
    int uiDevId = [[param objectAtIndex:1] intValue];
    BOOL isChildrenZero = [[param objectAtIndex:2] boolValue];
    if (isChildrenZero) {
        self.currentCellList=nil;
    }
    for (id<CMDeviceManagerHelper> helper in deviceHelpers) {
        [helper onGetDeviceTableCellInfo:iRestult parentId:uiDevId isChildrenZero:isChildrenZero];
    }
}

- (void) handlerOnDevStatusNotify:(id)arg
{
    NSArray *param = arg;
    int devId = [[param objectAtIndex:0] intValue];
    _CM_DEVICE_STATUS_ devStatus =(_CM_DEVICE_STATUS_)[[param objectAtIndex:1] intValue];
    int ids[1] ={devId};
    BOOL isNeedReload = [self setCellStatus:devStatus cellId:ids type:DEVICE];
    
    for (id<CMDeviceManagerHelper> helper in deviceHelpers) {
        [helper onDevStatusNotify:devId status:devStatus needReload:isNeedReload];
    }
}
- (void) handlerOnChannelNameChangeNotify:(id)arg
{
    NSArray *param = arg;
    int devId = [[param objectAtIndex:0] intValue];
    int channelId = [[param objectAtIndex:1] intValue];
    NSString *name = [param objectAtIndex:2];
    
    int ids[2] ={channelId,devId};
    BOOL isNeedReload = [self setCellName:name cellId:ids type:CHANNEL];
    
    for (id<CMDeviceManagerHelper> helper in deviceHelpers) {
        [helper onChannelNameChangeNotify:devId channelId: channelId newName:name needReload:isNeedReload];
    }
}

// Media callback
- (BOOL) startRealMonitorRlt: (const CChannelID*) pclChannelID
                     withRlt: (int) iResult
{
    NSNumber *theKey = [NSNumber numberWithLongLong:pclChannelID->channelKey()];
    NSString * strResult = [NSString stringWithFormat:@"%d",iResult];
    NSArray *param = [NSArray arrayWithObjects:theKey,strResult, nil];
    
    [self performSelectorOnMainThread:@selector(handlerStartRealMonitorRlt:) withObject:param waitUntilDone:NO];
    
    return YES;
}

- (BOOL) drawChannelVideo: (const CChannelID*) pclChannelID
                withImage: (UIImage*) pclImage
               imageWidth: (int) iWidth
              imageHeight: (int) iHeight
{
    [self performSelectorOnMainThread:@selector(handlerDrawChannelVideo:) withObject:pclImage waitUntilDone:NO];
    return YES;
}

- (BOOL) changeRealMonitorModel: (const CChannelID*) pclChannelID
                      withModel: (_REAL_MONITOR_MODEL_) eRMModel
                  withFramerate: (int) iFramerate
{
    NSNumber *theKey = [NSNumber numberWithLongLong:pclChannelID->channelKey()];
    NSString * strModel = [NSString stringWithFormat:@"%d",eRMModel];
    NSString * strFrame = [NSString stringWithFormat:@"%d",iFramerate];
    NSArray *param = [NSArray arrayWithObjects:theKey,strModel,strFrame, nil];
    
    self.currentMonitorModel = eRMModel;
    self.currentFrameRate = iFramerate;
    
    /*    [self performSelectorOnMainThread:@selector(handlerChangeRealMonitorModel:) withObject:param waitUntilDone:NO];
     */
    for (id<CMMediaManagerHelper> helper in mediaHelpers) {
        [helper handlerChangeRealMonitorModel: param];
    }
    
    return YES;
}

- (BOOL) notifyPlayStatus: (const CChannelID*) pclChannelID
             onPlayStatus: (_REAL_MONITOR_STATUS_) eRMStatus
{
    NSNumber *param = [NSNumber numberWithInt:eRMStatus];
   
    //    [self performSelectorOnMainThread:@selector(handlerNotifyPlayStatus:) withObject:param waitUntilDone:NO];
    
    for (id<CMMediaManagerHelper> helper in mediaHelpers) {
        [helper handlerNotifyPlayStatus: param];
    }
    
    return YES;
}

-  (void) regMediaManagerHelper: (id<CMMediaManagerHelper>) helper
{
    if (![mediaHelpers containsObject:helper]) {
        [mediaHelpers addObject:helper];
    }
}

-  (void) unregMediaManagerHelper: (id<CMMediaManagerHelper>) helper
{
    if ([mediaHelpers containsObject:helper]) {
        [mediaHelpers removeObject:helper];
    }
}

- (void) handlerStartRealMonitorRlt:(id)arg
{
    for (id<CMMediaManagerHelper> helper in mediaHelpers) {
        [helper handlerStartRealMonitorRlt: arg];
    }
}

- (void) handlerDrawChannelVideo:(id)arg
{
    for (id<CMMediaManagerHelper> helper in mediaHelpers) {
        [helper handlerDrawChannelVideo: arg];
    }
}

//
-(int)login:(NSString*) strSrvIp port:(u_short) usPort user:(NSString*)strUser password:(NSString*)strPassword withBlock:(loginBlock)block
{
    self.serverIp = strSrvIp;
    self.serverPort = usPort;
    self.userName = strUser;
    self.password = strPassword;
    
    //logincallBackBlock = block;
    
    return [idAccountModule login:strSrvIp port:usPort user:strUser password:strPassword];
}

- (void) cancelLogin
{
    if (idAccountModule!=nil) {
        [idAccountModule cancelLogin];
    }
}

-(void)getRootDevGroupInfo
{
    self.requestGroupId = ROOT_GROUP_ID+1;
    [idAccountModule getRootDevGroupInfo];
}
-(int) getDeviceTableCellChildrenInfo: (CMDeviceTableCellInfo *)theCell
{
    if (theCell==nil) {
        return CMFC_FAILED;
    }
    switch (theCell.type) {
        case GROUP:
        {
            return [self getDeviceTableCellInfoById:theCell.self_id];
            
        }
        case DEVICE:
        {
            self.requestDeviceId = theCell.self_id;
            return [idDeviceModule getInfoById:theCell.self_id];
        }
        default:
            return CMFC_FAILED;
    }
}

-(int) getDeviceTableCellInfoById:(u_int) theId//提供的id必须是某节点的爷爷节点
{
    //当前的爷爷节点必然是组节点
    self.requestGroupId = theId;
    return [idAccountModule getDevInfoByGroupId:theId];
}

// media
- (void) stopAllMonitor
{
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    CMDeviceTableCellInfo* info = [appDelegate.playerData GetCurCellInfo];
    
    CChannelID channelID;
    channelID.SetChannelID(info.parent_id, info.self_id);
    [self stopMonitorWithChannelInfo:&channelID];
}

-(void) stopMonitorWithChannelInfo:(CChannelID*)clChannelID
{
    if ([idMediaModule stopRealMonitor:clChannelID]==CMFC_SUCCESS) {
        
        self.currentFrameRate = DEFAULT_FRAMERATE;
        self.currentMonitorModel = DEFAULT_MONITOR_MODEL;
        
    }else{
        // InterLog(@"warning module stopRealMonitor failed");
    }
}

- (int) playMonitorWithChannelInfo:(CMDeviceTableCellInfo *)info isRemoteDevice:(BOOL)bRemoteFlag
{
    if (info == nil) {
         return CMFC_FAILED;
    }
    CChannelID clChannelID;
    clChannelID.SetChannelID(info.parent_id, info.self_id);
    if (![idMediaModule isRealMonitor:&clChannelID])
    {
        return [idMediaModule startRealMonitor:&clChannelID withRemoteFlag: bRemoteFlag];
    }
    return CMFC_FAILED;
}

/*
- (void)playMonitorWithChannelInfo:(CChannelID *)clChannelID isRemoteDevice:(BOOL)bRemoteFlag
{
    if (![idMediaModule isRealMonitor:clChannelID])
    {
        int result =  [idMediaModule startRealMonitor:clChannelID withRemoteFlag: bRemoteFlag];
        if (result !=CMFC_SUCCESS)
        {
            [self showInfoText:NSLocalizedString(@"因为网络原因监看失败,请重新尝试。", nil) interval:0];
            CMAppDelegate *appDelegate = (CMAppDelegate *)[UIApplication sharedApplication].delegate;
            [appDelegate.playerDataCenter delCellInfoFromDictionary:info];
             return;
 
        }
        NSNumber *theKey = [NSNumber numberWithLongLong:clChannelID.channelKey()];
        [center regDelegateWithChannelKey:self key:theKey];
    }
}
*/
- (bool)isRealMonitor:(CChannelID*)channelID
{
   return [idMediaModule isRealMonitor:channelID];
}

-(bool)isCanMonitor
{
    return [idAccountModule isCanMonitor];
}
- (bool)isPtzEnable
{
    return [idAccountModule isPTZCtrlEnable];
}

- (BOOL) isWaitingResponse
{
    return self.requestGroupId!=INVALID_GROUP_ID || self.requestDeviceId!=INVALID_DEV_ID;
}

- (void) cancelRequestDevInfo
{
    self.requestGroupId = INVALID_GROUP_ID;
    self.requestDeviceId = INVALID_DEV_ID;
}

- (BOOL) addNewDevice:(NSString *) devName selfId:(int)devId parentId:(int)groupId
{
    int ids[1] = {devId};
    CMDeviceTableCellInfo *theCell = [self findCellInCurrentListById:ids type:DEVICE];
    if (theCell==nil && self.currentGroupId ==groupId) {
        theCell = [[CMDeviceTableCellInfo alloc] init];
        theCell.self_id = devId;
        theCell.parent_id = groupId;
        theCell.name = devName;
        theCell.type = DEVICE;
        theCell.status = DEV_NORMAL;
        [self.currentCellList addObject:theCell];
        [theCell release];
        return YES;
    }
    return NO;
}
- (BOOL) delDevice:(int)devId parentId:(int)groupId
{
    int ids[1] = {devId};
    CMDeviceTableCellInfo *theCell = [self findCellInCurrentListById:ids type:DEVICE];
    if (theCell!=nil) {
        if (groupId == theCell.parent_id) {
            [self.currentCellList removeObject:theCell];
            return YES;
        }
    }else{
        if (self.currentGroupId == devId) {
            [self getDeviceTableCellInfoById:(self.currentGroupParentId==INVALID_GROUP_ID?(ROOT_GROUP_ID+1):self.currentGroupParentId)];
        }
    }
    return NO;
}

//一些帮助函数
-(BOOL) setCellName:(NSString *)newName cellId:(int*)cId type:(DEVICE_TYPE)cellType
{
    CMDeviceTableCellInfo *theCell = [self findCellInCurrentListById:cId type:cellType];
    if (theCell!=nil) {
        theCell.name = newName;
        return YES;
    }
    return NO;
}

-(BOOL) setCellStatus:(_CM_DEVICE_STATUS_)eStatus cellId:(int*)cId type:(DEVICE_TYPE)cellType
{
    CMDeviceTableCellInfo *theCell = [self findCellInCurrentListById:cId type:cellType];
    if (theCell!=nil) {
        theCell.status = eStatus;
        return YES;
    }else{
        if (self.currentGroupId == cId[0] && eStatus !=DEV_NORMAL) {
            [self getDeviceTableCellInfoById:(self.currentGroupParentId==INVALID_GROUP_ID?(ROOT_GROUP_ID+1):self.currentGroupParentId)];
        }
    }
    return NO;
}

-(CMDeviceTableCellInfo *) findCellInCurrentListById:(int*)cId type:(DEVICE_TYPE)cellType
{
    switch (cellType) {
        case CHANNEL:
        {
            for (CMDeviceTableCellInfo *theCell in self.currentCellList) {
                if (theCell.self_id==cId[0] && theCell.parent_id ==cId[1]) {
                    return theCell;
                }
            }
            break;
        }
        default:
            for (CMDeviceTableCellInfo *theCell in self.currentCellList) {
                if (theCell.self_id==cId[0] && cellType == theCell.type) {
                    return theCell;
                }
            }
            break;
    }
    return nil;
}

- (BOOL) notifyListenStatus: (const CChannelID*) pclChannelID
             onListenStatus: (_AUDIO_STATUS_) audioStatus
{
    return false;
}

- (u_longlong) listenningChannelKey
{
    return [idMediaModule listenningChannelKey];
}

- (BOOL) isListenning
{
    return [idMediaModule isListenning];
}

-(BOOL)stopListenWithChannelKey:(u_longlong)channelKey
{
    return [idMediaModule stopListen:channelKey];
}

-(BOOL)stopListen:(CChannelID *)channelID
{
    BOOL result = NO;
    result = [idMediaModule stopListen:channelID->channelKey()];
    return result;
}

- (int) startListen: (const CChannelID*) pclChannelID withRemoteFlag: (bool) bRemoteFlag
{
//    id<PTMediaModule> idMediaModule1 = [[CMModuleCreatorSingleton getModuleCreator] getMediaModule];
    int result = [idMediaModule startListen:pclChannelID withRemoteFlag:bRemoteFlag];
    return result == CMFC_SUCCESS;
}

//
- (BOOL) isRecordering:(CChannelID *)channelID
{
    return [idMediaModule isRecordering:channelID];
}

- (BOOL) startRecorderingWithOutPath:(CChannelID*)channelID withOutPath:(NSString*)path withFPS:(int)fps
{
    return [idMediaModule startRecordering:channelID withOutPath:path withFPS:fps];
}

- (BOOL) stopRecordering:(CChannelID*)channelID
{
    return [idMediaModule stopRecordering:channelID];
}

//
- (BOOL) setRealMonitorModel:(_REAL_MONITOR_MODEL_) eRMModel channelId:(CChannelID&)channelID
{
    if ( 0 != channelID.channelKey() ) {
        NSNumber* theKey = [NSNumber numberWithLongLong:channelID.channelKey()];
        //CMPlayerData* data = [self findPlayerDataByChannelKey:theKey];
        //if (data!=nil)
        {
            //id<PTMediaModule> idMediaModule = [[CMModuleCreatorSingleton getModuleCreator] getMediaModule];
            if (self.currentFrameRate>MAX_FRAMERATE || self.currentFrameRate<MIN_FRAMERATE) {
                self.currentFrameRate = DEFAULT_FRAMERATE;
            }
            if ([idMediaModule setRealMonitorModel:&channelID
                                         withModel:eRMModel
                                     withFramerate:self.currentFrameRate])
            {
                self.currentMonitorModel = eRMModel;
                return YES;
            }
        }
    }
    return NO;
}

- (void) controlPTZ :(_PTZ_CTL_TYPE_)ptzType intensity:(int)iIntensity channelId:(CChannelID&)channelID Status:(_REAL_MONITOR_STATUS_)currentStatus;
{
    if ( channelID.channelKey() != 0)
    {
        //NSNumber* theKey = [NSNumber numberWithLongLong:channelID.channelKey()];
        //CMPlayerData* data = [self findPlayerDataByChannelKey:theKey];
        //if (data!=nil && data.player!=nil)
        {
            switch (currentStatus) {
                case MSTATUS_SLOW_PLAY:
                case MSTATUS_NORMAL_PLAY:
                case MSTATUS_FAST_PLAY:
                case MSTATUS_BUFF_FRAME:
                {
                    if (iIntensity==0) {
                        iIntensity = MC_PTZ_CTRL_SPEED;
                    }
                    //id<PTDeviceModule>  idDevModule = [[CMModuleCreatorSingleton getModuleCreator] getDeviceModule];
                    [idDeviceModule ptzCtrl:channelID ctrltype:ptzType intensity:iIntensity];
                    break;
                }
                default:
                    break;
            }
        }
    }
}

- (BOOL) setRealMonitorFrame:(int) iFramerate channelId:(CChannelID&)channelID
{
    if (0 != channelID.channelKey()) {
        if (iFramerate>MAX_FRAMERATE || iFramerate<MIN_FRAMERATE) {
            return NO;
        }
        //NSNumber* theKey = [NSNumber numberWithLongLong:channelID.channelKey()];
        //CMPlayerData* data = [self findPlayerDataByChannelKey:theKey];
        //if (data!=nil)
        {
            //id<PTMediaModule> idMediaModule = [[CMModuleCreatorSingleton getModuleCreator] getMediaModule];
            if ([idMediaModule setRealMonitorModel:&channelID withModel:currentMonitorModel withFramerate:iFramerate])
            {
                self.currentFrameRate = iFramerate;
                return YES;
            }
        }
    }
    return NO;
}

@end
