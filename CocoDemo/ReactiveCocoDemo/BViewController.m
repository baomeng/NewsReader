//
//  BViewController.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/10.
//  Copyright © 2016年 Seegle. All rights reserved.
//

#import "BViewController.h"
#define DEVW [UIScreen mainScreen].bounds.size.width
#define DEVH [UIScreen mainScreen].bounds.size.height
#import "AppDelegate.h"
#import "CMCenterManager.h"

#import "CMModuleCreatorProtocolDef.h"
#import "CMMediaModuleProtocolDef.h"

#import "DeviceCollectionViewCell.h"
#import "CustomCollectionReusableView.h"
#import "CMMonitorController.h"
#import "CMPlayerData.h"
#import "CMSettingViewController.h"
#import "CMDefaultSettingViewController.h"
#import "Masonry.h"

static NSString *const kCollectionViewCellIdentifier = @"collectionViewCellIdentifier";
static NSString *const kCollectionElementKindSectionHeaderIdentifier = @"headerIdentifier";
static NSString *const kCollectionElementKindSectionFooterIdentifier = @"footerIdentifier";

@interface BViewController ()
@end

@implementation BViewController


- (void)viewWillAppear:(BOOL)animated
{
    [centerManager regAccountHelper:self];
    [centerManager regDeviceHelper:self];
    
    [self updateNavigationBarContent];
    self.navigationController.navigationBarHidden = NO;
    
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
   if ([centerManager isWaitingResponse]) {
        //[self showWaiting];
    }else{
        //[self hideWaiting];
        [self.collectionView reloadData];
    }
    [super viewDidAppear:animated];
    
}

//显示进度滚轮指示器
-(void)showWaiting {
    _waittingAlert1 = [self showWaitingAlert1];
    [self updateNavigationBarContent];
}
//消除滚动轮指示器
-(void)hideWaiting{
    if (_waittingAlert1 !=nil ){
        [self dismissWaitingAlert1:_waittingAlert1];
    }
    [self updateNavigationBarContent];
}

-(UIAlertView *)showWaitingAlert1{
    UIAlertView *waittingAlert = [[UIAlertView alloc] initWithTitle: @"正在获取设备列表"
                                                            message: @"请稍候..."
                                                           delegate: nil
                                                  cancelButtonTitle: nil
                                                  otherButtonTitles: nil];
    
    
    UIActivityIndicatorView *activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    activityView.frame = CGRectMake(139.0f-18.0f, 80.0f, 37.0f, 37.0f);
    [waittingAlert addSubview:activityView];
    [activityView startAnimating];
    
    [waittingAlert show];
    
    return waittingAlert;
    
}
-(void) dismissWaitingAlert1:(UIAlertView*) waittingAlert{
    
    if (waittingAlert != nil) {
        [waittingAlert dismissWithClickedButtonIndex:0 animated:YES];
        // [waittingAlert release];
        waittingAlert =nil;
    }
}

- (void)viewWillDisappear:(BOOL)animated
{
    [centerManager unregAccountHelper:self];
    [centerManager unregDeviceHelper:self];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
}

-(void)updateNavigationBarContent
{
    UIBarButtonItem *barLeftButtonItem = nil;
    barLeftButtonItem=[[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"重登", nil) style:UIBarButtonItemStyleBordered target:self action:@selector(reLogin)];
    [self.navigationItem setLeftBarButtonItem:barLeftButtonItem animated:NO];
    
    UIBarButtonItem *barRightButtonItem = nil;
    barRightButtonItem=[[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"设置", nil) style:UIBarButtonItemStyleBordered target:self action:@selector(showSettingView)];
    [self.navigationItem setRightBarButtonItem:barRightButtonItem animated:NO];
}
-(void)reLogin
{
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    [centerManager cancelLogin];
    
    [centerManager.currentCellList removeAllObjects];
    
     [appDelegate popToLoginViewController];
}

- (void)showSettingView
{
    CMDefaultSettingViewController *settingController;
    settingController = [[CMDefaultSettingViewController alloc] initWithNibName:@"CMDefaultSettingViewController" bundle:nil];
    settingController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
    [self.navigationController pushViewController:settingController animated:YES];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor whiteColor];
    self.title = @"设备列表";
    
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    centerManager = appDelegate.centerManager;
    
    //[centerManager getDevLst];
    //_waittingAlert1 = [self showWaitingAlert1];
    
    //self.view.backgroundColor = [UIColor yellowColor];
    //self.edgesForExtendedLayout = UIRectEdgeNone;
    // Do any additional setup after loading the view.
    //[self initTableView];
    
    [self.view addSubview:self.collectionView];
    
    self.collectionView.translatesAutoresizingMaskIntoConstraints = NO;
    __weak __typeof(self) weakSelf = self;//这里用一个弱引用来表示self，用于下面的Block中
    
    [self.collectionView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(weakSelf.view.mas_top).with.offset(1);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(1);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-1);
        make.bottom.equalTo(weakSelf.view.mas_bottom).with.offset(-1);
    }];
    
    _isOnline = YES;

}

// collection
#pragma mark *** UICollectionViewDataSource ***
// 设置组数
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}
// 设置行数
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return [centerManager.currentCellList count];
}
// 设置单元格
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    // item重用机制
    
    DeviceCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:kCollectionViewCellIdentifier forIndexPath:indexPath];
    [self configCell:cell indexpath:indexPath collectionView:collectionView];
    return cell;
}

- (void)configCell:(DeviceCollectionViewCell *)cell indexpath:(NSIndexPath *)indexpath collectionView:(UICollectionView *)collectionView
{
    UIImage *statusImage;
    if (centerManager.currentCellList==nil || centerManager.currentCellList.count<=[indexpath row]) {
        return;
    }
    
    CMDeviceTableCellInfo * cellInfo = [centerManager.currentCellList objectAtIndex:[indexpath row]];
    switch (cellInfo.type) {
            case GROUP:{
                if (_isOnline) {
                    NSString *path = [[NSBundle mainBundle] pathForResource:@"group_online" ofType:@"png"];
                    statusImage = [UIImage imageWithContentsOfFile:path];
                }else{
                    NSString *path = [[NSBundle mainBundle] pathForResource:@"group_outline" ofType:@"png"];
                    statusImage = [UIImage imageWithContentsOfFile:path];
                }
                
                break;
            }
            case DEVICE:{
                if (_isOnline) {
                    switch (cellInfo.status) {
                            case DEV_NORMAL:{
                                NSString *path = [[NSBundle mainBundle] pathForResource:@"device_online" ofType:@"png"];
                                statusImage = [UIImage imageWithContentsOfFile:path];
                                break;
                            }
                            case DEV_ABNORMAL:{
                                NSString *path = [[NSBundle mainBundle] pathForResource:@"device_abnormal" ofType:@"png"];
                                statusImage = [UIImage imageWithContentsOfFile:path];
                                break;
                            }
                            case DEV_OFFLINE:{
                                NSString *path = [[NSBundle mainBundle] pathForResource:@"device_offline" ofType:@"png"];
                                statusImage = [UIImage imageWithContentsOfFile:path];
                                break;
                            }
                        default:
                            break;
                    }
                    
                }else{
                    NSString *path = [[NSBundle mainBundle] pathForResource:@"device_outline" ofType:@"png"];
                    statusImage = [UIImage imageWithContentsOfFile:path];
                }
                
                break;
            }
        default:
            break;
    }
    
    cell.titleLabel.text = cellInfo.name;
    cell.titleLabel.font = [UIFont systemFontOfSize:16.0f];
    cell.mainImageView.image = statusImage;
    
    return;
    
}

// 设置自定义头部、尾部视图
/*- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath{
 CustomCollectionReusableView *view = [collectionView dequeueReusableSupplementaryViewOfKind:kind withReuseIdentifier:kCollectionElementKindSectionHeaderIdentifier forIndexPath:indexPath];
 //标题头
 if (kind == UICollectionElementKindSectionHeader) {
 view.label.text = [NSString stringWithFormat:NSLocalizedString(@"设备列表",nil),indexPath.section];
 }else if (kind == UICollectionElementKindSectionFooter){
 //标题尾
 view.label.text = [NSString stringWithFormat:NSLocalizedString(@"第%d标题尾",nil),indexPath.section];
 }
 
 view.label.backgroundColor = [UIColor lightGrayColor];
 
 return view;
 }
 */
// 设置是否允许移动
- (BOOL)collectionView:(UICollectionView *)collectionView canMoveItemAtIndexPath:(NSIndexPath *)indexPath {
    return YES;
}
// 移动元素
- (void)collectionView:(UICollectionView *)collectionView moveItemAtIndexPath:(NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath *)destinationIndexPath {
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

#pragma mark *** UICollectionViewDelegateFlowLayout ***
// 通过UICollectionViewDelegateFlowLayout提供的方法可高度定制流式布局；
- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section {
    return UIEdgeInsetsMake(1.0f, 20.0f, 1.0f, 20.0f);
}

#pragma mark *** UICollectionViewDelegate ***
// 选中单元格
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    
    DeviceCollectionViewCell *cell = (DeviceCollectionViewCell *)[collectionView cellForItemAtIndexPath:indexPath];
    
    CMDeviceTableCellInfo * cellInfo = [centerManager.currentCellList objectAtIndex:[indexPath row]];
    switch ([cellInfo type]) {
            case DEVICE:
        {
            if (cellInfo.status==DEV_NORMAL) {
                [centerManager getDeviceTableCellChildrenInfo:cellInfo];
                //[self showWaiting];
                self.bPlay = true;
            }
            break;
        }
        default:
            break;
    }
}

// 取消选中
- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath {
    DeviceCollectionViewCell *cell = (DeviceCollectionViewCell *)[collectionView cellForItemAtIndexPath:indexPath];
}


- (CGFloat) collectionView:(UICollectionView *)collectionView
                    layout:(UICollectionViewLayout *)collectionViewLayout
minimumInteritemSpacingForSectionAtIndex:(NSInteger)section
{
    return 10.0f;
}

#pragma mark *** Getters ***
- (UICollectionViewFlowLayout *)collectionViewFlowLayout {
    if (!_collectionViewFlowLayout) {
        _collectionViewFlowLayout = [[UICollectionViewFlowLayout alloc] init];
        // 设置item估计值
        _collectionViewFlowLayout.estimatedItemSize = CGSizeMake(80, 90);
        // 全局配置item尺寸,单独定义调用协议方法[sizeForItemAtIndexPath]
        _collectionViewFlowLayout.itemSize = CGSizeMake(80, 90);
        // 全局配置每行之间的间距,单独定义可调用协议方法[minimumLineSpacingForSectionAtIndex]
        _collectionViewFlowLayout.minimumLineSpacing = 20;
        // 全局配置每行内部item的间距,单独定义可调用协议方法[minimumInteritemSpacingForSectionAtIndex]
        _collectionViewFlowLayout.minimumInteritemSpacing = 20;
        // 设置头部size
        _collectionViewFlowLayout.headerReferenceSize = CGSizeMake(CGRectGetWidth(self.view.bounds), 20);
        // 设置尾部size
        _collectionViewFlowLayout.footerReferenceSize = CGSizeZero;
        // 设置滚动方向
        // UICollectionViewScrollDirectionVertical
        // UICollectionViewScrollDirectionHorizontal
        _collectionViewFlowLayout.scrollDirection = UICollectionViewScrollDirectionVertical;
        //_collectionViewFlowLayout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
        // 设置是否当元素超出屏幕之后固定头部视图位置，默认NO；
        _collectionViewFlowLayout.sectionHeadersPinToVisibleBounds = YES;
        // 设置是否当元素超出屏幕之后固定尾部视图位置，默认NO；
        _collectionViewFlowLayout.sectionFootersPinToVisibleBounds = YES;
    }
    return _collectionViewFlowLayout;
}

-(UICollectionView *)collectionView {
    if (!_collectionView) {
        _collectionView = [[UICollectionView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.view.bounds), CGRectGetHeight(self.view.bounds) - 0) collectionViewLayout:self.collectionViewFlowLayout];
        
        _collectionView.backgroundColor = [UIColor whiteColor];
        // 设置是否允许滚动
        _collectionView.scrollEnabled = YES;
        // 设置是否允许选中，默认YES
        _collectionView.allowsSelection = YES;
        // 设置是否允许多选，默认NO
        _collectionView.allowsMultipleSelection = NO;
        // 设置代理
        _collectionView.delegate = self;
        // 设置数据源
        _collectionView.dataSource = self;
        // 注册Item
        [_collectionView registerClass:[DeviceCollectionViewCell class] forCellWithReuseIdentifier:kCollectionViewCellIdentifier];
        // 注册头部视图
        [_collectionView registerClass:[CustomCollectionReusableView class] forSupplementaryViewOfKind:UICollectionElementKindSectionHeader withReuseIdentifier:kCollectionElementKindSectionHeaderIdentifier];
        // 注册尾部视图
        [_collectionView registerClass:[CustomCollectionReusableView class] forSupplementaryViewOfKind:UICollectionElementKindSectionFooter withReuseIdentifier:kCollectionElementKindSectionFooterIdentifier];
    }
    return _collectionView;
}

- (void)showMonitorView:(CMDeviceTableCellInfo *)cellInfo
{
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    [appDelegate.playerData SetCurCellInfo:cellInfo];
    
    if (![self.navigationController.topViewController isMemberOfClass:[CMMonitorController class]]) {
        CMMonitorController *monitorController = nil;
        for (UIViewController* controller in self.navigationController.viewControllers) {
            if ([controller isMemberOfClass:[CMMonitorController class]]) {
                monitorController = (CMMonitorController *)controller;
                break;
            }
        }
        
        
        if (monitorController!=nil) {
            [self.navigationController popToViewController:monitorController animated:YES];
        }else{
            
            [self.navigationController pushViewController:appDelegate.monitorController animated:YES];
        }
    }
  
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)onlogin:(int)iResult user:(NSString*)strUser{
    
}
-(void)onLogout:(int)errorCode{
    
}
-(void)onReconnect:(int)count{
    
}
-(void)onDevNameChangeNotify:(u_int)uiDevid newName:(NSString*)strName needReload:(BOOL)isReload{
    
}
-(void)onAddDevNotify:(u_int)uiDevId devname:(NSString*)strName groupId:(u_int)uiGrpId needReload:(BOOL)isReload{
    
}
-(void)onDelDevNotify:(u_int)uiDevId groupId:(u_int)uiGrpId needReload:(BOOL)isReload{
    
}
-(void)onPermChangeNotify:(bool)bCanMonitor andPTZCtrlEnable: (bool) bPTZCtrlEnable{
    
}
-(void)onSyncAccBalanceInfo:(u_long)ulCHCnt{
    
}
-(void)onGetDeviceTableCellInfo:(int)iResult parentId:(u_int)pId isChildrenZero:(BOOL)zero
{
    
    if (iResult == CMFC_SUCCESS ) {
        
        [self updateNavigationBarContent];
        
        if (centerManager.currentCellList.count>0 && pId == 1) {
            [_collectionView reloadData];
        }
        else if(centerManager.currentCellList.count<=0){
         
            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"错误信息", nil)
                                                                message:@"获取列表失败"
                                                               delegate:nil
                                                      cancelButtonTitle:NSLocalizedString(@"确定", nil)
                                                      otherButtonTitles:nil, nil];
            [alertView show];
        }
        
        if (centerManager.currentChannelList.count>0 && _bPlay) {
            
            CMDeviceTableCellInfo *cellInfo = [centerManager.currentChannelList objectAtIndex:0];
            
            [self showMonitorView:cellInfo];
            _bPlay = false;
            
        }
    }
    //if (_waittingAlert1 !=nil ){
   //    [self dismissWaitingAlert1:_waittingAlert1];
    //}
}

-(void)onDevStatusNotify:(int)uiDevId status:(_CM_DEVICE_STATUS_)eStatus needReload:(BOOL)isReload{
    
}
-(void)onChannelNameChangeNotify:(int)uiDevId channelId: (int)uiChannelId newName:(NSString*)strName needReload:(BOOL)isReload{
    
}
//-(int)ptzCtrl:(CSubDevId)subDevId ctrltype:(int)iCtrlType intensity:(int)iIntensity;
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
