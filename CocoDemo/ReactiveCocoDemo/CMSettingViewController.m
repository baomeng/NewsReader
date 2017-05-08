//
//  CMSettingViewController.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/14.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "CMSettingViewController.h"
#import "Masonry.h"
#import "ViewController.h"
#import "CMUtil.h"
#import "CMDataCenter.h"
#import "LoginView.h"
#import "Sqlite.h"
#import "CoreDataManager.h"
#import "SetViewController.h"
#import "scrollviewdemo.h"
#import "MeViewControllerDemo.h"

#define   SWITCH_MARGIN_RIGHT           0
#define   SWITCH_WIDTH                  79
#define   SWITCH_HEIGHT                 27
#define   SWITCH_TAG                    2000

@interface CMSettingViewController ()
{
    UIView *_aview;
}
@property (nonatomic, strong) NSMutableArray *dataArr;
@property (nonatomic, strong) NSMutableArray *data1Arr;
@property (nonatomic, strong) CoreDataManager *manager;

@end

@implementation CMSettingViewController

/*- (instancetype)init
{
    self = [super init];
    if (self) {
        _dataArr = [[NSMutableArray alloc] init];
        _manager = [CoreDataManager sharedCoreDataManager];
    }
    return self;
}
*/
-(void)viewWillAppear:(BOOL)animated
{
    
    [super viewWillAppear:animated];
    self.navigationController.navigationBarHidden =NO;
    self.navigationController.navigationBar.translucent = NO;
    
    //    创建取回数据请求
    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    //    设置要检索哪种类型的实体对象
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"User" inManagedObjectContext:_manager.managedObjContext];
    //    设置请求实体
    [request setEntity:entity];
    
    //    指定对结果的排序方式
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"age" ascending:NO];
    NSArray *sortDescriptions = [[NSArray alloc] initWithObjects:sortDescriptor, nil];
    [request setSortDescriptors:sortDescriptions];
    
    NSError *error = nil;
    //    执行获取数据请求，返回数组
    NSArray *fetchResult = [_manager.managedObjContext executeFetchRequest:request error:&error];
    if (!fetchResult)
    {
        NSLog(@"error:%@,%@",error,[error userInfo]);
    }
    [self.dataArr removeAllObjects];
    [self.dataArr addObjectsFromArray:fetchResult];
    
    
    [self.table reloadData];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    //1.首先，创建视图控件，添加到self.view上
    
 /*   UIView *view_1 = [[UIView alloc] init];
    view_1.backgroundColor = [UIColor redColor];
    [self.view addSubview:view_1];
    
    UIView *view_2 = [[UIView alloc] init];
    view_2.backgroundColor = [UIColor greenColor];
    [self.view addSubview:view_2];
    
    UIView *view_3 = [[UIView alloc] init];
    view_3.backgroundColor = [UIColor blueColor];
    [self.view addSubview:view_3];
    
    //2.然后，记得要把AutoresizingMask布局关掉
    view_1.translatesAutoresizingMaskIntoConstraints = NO;
    view_2.translatesAutoresizingMaskIntoConstraints = NO;
    view_3.translatesAutoresizingMaskIntoConstraints = NO;
    
    //3.接着，添加约束
    
    __weak __typeof(self) weakSelf = self;//这里用一个弱引用来表示self，用于下面的Block中
    
    //先确定view_1的约束
    [view_1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(weakSelf.view.mas_top).with.offset(30+60); //view_1的上，距离self.view是30px
        make.left.equalTo(weakSelf.view.mas_left).with.offset(30); //view_1de左，距离self.view是30px
    }];
    
    //然后确定view_2的约束
    [view_2 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.equalTo(view_1.mas_centerY).with.offset(0); //view2 Y方向上的中心线和view_1相等
        make.left.equalTo(view_1.mas_right).with.offset(30); //view2 的左距离view_1的右距离为30px
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-30); //view_2的右距离self.view30px
        make.width.equalTo(view_1); //view_2的宽度和view_1相等
        make.height.equalTo(view_1); //view_2的高度和view_1相等
    }];
    
    //最后确定view_3的约束
    [view_3 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(view_1.mas_bottom).with.offset(30); //view_3的上距离view_1的底部 30px
        make.left.equalTo(weakSelf.view.mas_left).with.offset(30); //view_3的左距离self.view 30px
        make.bottom.equalTo(weakSelf.view.mas_bottom).with.offset(-30);//view_3的底部距离self.view 30px
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-30); //view_3的右距离self.view 30px
        make.height.equalTo(view_1);//view_3的高度和view_1相等
    }];
    
    //先根据初始化添加的约束生成最初的frame并显示view
    [view_2 layoutIfNeeded];
    //开始动画
    [UIView animateWithDuration:3.0 animations:^{
        [view_2 mas_updateConstraints:^(MASConstraintMaker *make) {
            make.width.equalTo(@300);
        }];
        //更新约束  在某个时刻约束会被还原成frame使视图显示
        [self.view layoutIfNeeded];
    } completion:^(BOOL finished) {
    }];
  */
    self.view.backgroundColor = [UIColor whiteColor];
    
    UISegmentedControl *segment = [[UISegmentedControl alloc]initWithItems:@[@"红",@"黄",@"蓝"]];
    segment.frame =CGRectMake(60,100,200, 30);
    self.navigationItem.titleView = segment;
    segment.selectedSegmentIndex =1;
    [segment addTarget:self action:@selector(handelSegementControlAction:)forControlEvents:(UIControlEventValueChanged)];

    
    UITextField* serverIP = [[UITextField alloc] init];
    serverIP.height = 40;
    serverIP.leftView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 10, 0)];
    serverIP.leftViewMode = UITextFieldViewModeAlways;
    [serverIP setBackground:[UIImage resizeImageWihtImageName:@"operationbox_text"]];
    serverIP.clearButtonMode = UITextFieldViewModeWhileEditing;
    self.serverIP = serverIP;
    [self.view addSubview:self.serverIP];
    
    UITextField* serverPort = [[UITextField alloc] init];
    serverPort.height = 40;
    serverPort.leftView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 10, 0)];
    serverPort.leftViewMode = UITextFieldViewModeAlways;
    [serverPort setBackground:[UIImage resizeImageWihtImageName:@"operationbox_text"]];
    serverPort.clearButtonMode = UITextFieldViewModeWhileEditing;
    self.serverPort = serverPort;
    [self.view addSubview:self.serverPort];

    
    UIButton *pclButtonOK = [[UIButton alloc]init];
    pclButtonOK.enabled = YES;
    [pclButtonOK setTitle:@"确定" forState:UIControlStateNormal];
    [pclButtonOK setBackgroundImage:[UIImage resizeImageWihtImageName:@"fts_green_btn"] forState:UIControlStateNormal];
    [pclButtonOK addTarget:self action:@selector(settingClick) forControlEvents:UIControlEventTouchUpInside];
    self.pclButtonOK = pclButtonOK;
    [self.view addSubview:self.pclButtonOK];
    
    UIButton *pclButtonDB = [[UIButton alloc]init];
    pclButtonDB.enabled = YES;
    [pclButtonDB setTitle:@"SQLite测试" forState:UIControlStateNormal];
    [pclButtonDB setBackgroundImage:[UIImage resizeImageWihtImageName:@"fts_green_btn"] forState:UIControlStateNormal];
    [pclButtonDB addTarget:self action:@selector(dbTestClick) forControlEvents:UIControlEventTouchUpInside];
    self.pclButtonDB = pclButtonDB;
    [self.view addSubview:self.pclButtonDB];
    
    UIButton *pclBtnScroll = [[UIButton alloc]init];
    pclBtnScroll.enabled = YES;
    [pclBtnScroll setTitle:@"scrollview测试" forState:UIControlStateNormal];
    [pclBtnScroll setBackgroundImage:[UIImage resizeImageWihtImageName:@"fts_green_btn"] forState:UIControlStateNormal];
    [pclBtnScroll addTarget:self action:@selector(btnClick) forControlEvents:UIControlEventTouchUpInside];
    self.pclBtnScroll = pclBtnScroll;
    [self.view addSubview:self.pclBtnScroll];
    
    UIButton *pclBtnMe = [[UIButton alloc]init];
    pclBtnMe.enabled = YES;
    [pclBtnMe setTitle:@"我的布局测试" forState:UIControlStateNormal];
    [pclBtnMe setBackgroundImage:[UIImage resizeImageWihtImageName:@"fts_green_btn"] forState:UIControlStateNormal];
    [pclBtnMe addTarget:self action:@selector(btnMeClick:) forControlEvents:UIControlEventTouchUpInside];
    self.pclBtnMe = pclBtnMe;
    [self.view addSubview:self.pclBtnMe];
    
/*    UITableView *table = [[UITableView alloc]init];
    table.delegate = self;
    table.dataSource = self;
    table.backgroundColor = [UIColor whiteColor];
    //table.separatorColor = [UIColor lightGrayColor];
    table.scrollEnabled = YES;
    self.table = table;
    [self.view addSubview:self.table];
 */
    
    UITableView *table = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStyleGrouped];
    table.delegate = self;
    table.dataSource = self;
    table.scrollEnabled = YES;
    self.table = table;
    [self.view addSubview:self.table];
    
    _aview = [[UIView alloc]initWithFrame:CGRectMake(60,200,200, 200)];
   // _aview = [[UIView alloc] init];
    _aview.backgroundColor = [UIColor yellowColor];
    [self.view addSubview:_aview];
    
    self.serverIP.translatesAutoresizingMaskIntoConstraints = NO;
    self.serverPort.translatesAutoresizingMaskIntoConstraints = NO;
    self.pclButtonOK.translatesAutoresizingMaskIntoConstraints = NO;
    self.pclButtonDB.translatesAutoresizingMaskIntoConstraints = NO;
    self.table.translatesAutoresizingMaskIntoConstraints = NO;
    _aview.translatesAutoresizingMaskIntoConstraints = NO;
    __weak __typeof(self) weakSelf = self;//这里用一个弱引用来表示self，用于下面的Block中
    
    [self.serverIP mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(weakSelf.view.mas_top).with.offset(20);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
    }];
    
    [self.serverPort mas_makeConstraints:^(MASConstraintMaker *make) {
        //make.centerX.equalTo(ip.mas_centerY).with.offset(0);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.top.equalTo(self.serverIP.mas_bottom).with.offset(10);
        make.width.equalTo(self.serverIP);
        make.height.equalTo(self.serverIP);
    }];

    [self.pclButtonOK mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.serverPort.mas_bottom).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.height.equalTo(self.serverIP);
    }];
    
    [self.pclButtonDB mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.pclButtonOK.mas_bottom).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.height.equalTo(self.serverIP);
    }];
    
    [self.pclBtnScroll mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.pclButtonDB.mas_bottom).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.height.equalTo(self.serverIP);
    }];
    
    [self.pclBtnMe mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.pclBtnScroll.mas_bottom).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.height.equalTo(self.serverIP);
    }];
    
    [self.table mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.pclBtnMe.mas_bottom).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.size.mas_equalTo(CGSizeMake(80,120));
        //make.bottom.equalTo(weakSelf.view.mas_bottom).with.offset(-60);
    }];
    
    [_aview mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.table.mas_bottom).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(60);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-60);
        make.bottom.equalTo(weakSelf.view.mas_bottom).with.offset(-20);
    }];
    
    self.serverIP.text = [CMUtil getServerIpForIndex:0];
    NSNumber *portNumber = [CMUtil getServerPortForIndex:0];
    self.serverPort.text =[portNumber stringValue];
    
    self.arrStudent = [[NSMutableArray alloc] init];
    
    _dataArr = [[NSMutableArray alloc] init];
    _manager = [CoreDataManager sharedCoreDataManager];
    [self loadNaviBar];
    
     _data1Arr = [[NSMutableArray alloc] init];
    [self.data1Arr addObject:@"朋友1"];
    [self.data1Arr addObject:@"朋友2"];
    [self.data1Arr addObject:@"朋友3"];
    [self.data1Arr addObject:@"朋友4"];
    [self.data1Arr addObject:@"朋友5"];
    [self.data1Arr addObject:@"朋友6"];
    [self.data1Arr addObject:@"朋友7"];
    [self.data1Arr addObject:@"朋友8"];
    [self.data1Arr addObject:@"朋友9"];
    [self.data1Arr addObject:@"朋友10"];
    
    
    [self.table setEditing:true];
    [self.table setSectionIndexColor:[UIColor redColor]];
}

- (void) loadNaviBar
{
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"新增" style:UIBarButtonItemStylePlain target:self action:@selector(clickAdd:)];
}

-(void) btnClick
{
    scrollviewdemo *set = [[scrollviewdemo alloc] init];
    [self.navigationController pushViewController:set animated:YES];
}

-(void) btnMeClick:(id)sender
{
    MeViewControllerDemo *me = [[MeViewControllerDemo alloc] init];
    [self.navigationController pushViewController:me animated:YES];
}

- (void) clickAdd:(id)sender
{
    SetViewController *set = [[SetViewController alloc] init];
    set.title = @"添加";
    set.type = ADD;
    [self.navigationController pushViewController:set animated:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)settingClick
{
    if (![self.navigationController.topViewController isMemberOfClass:[ViewController class]]) {
        ViewController *theLoginController = nil;
        for (UIViewController* controller in self.navigationController.viewControllers) {
            if ([controller isMemberOfClass:[ViewController class]]) {
                theLoginController = (ViewController *)controller;
                break;
            }
        }
        if (theLoginController!=nil) {
            
            [CMUtil insertServerIPToHistoryDataCenter:self.serverIP.text serverPort:[self.serverPort.text intValue]];
            
            //[theLoginController setServerIp:self.serverIP.text];
            //[theLoginController setServerPort:[self.serverPort.text intValue]];
            
            if(self.mysettingBlock){
                self.mysettingBlock(self.serverIP.text, [self.serverPort.text intValue]);
            }
            
            [self.navigationController popToViewController:theLoginController animated:YES];
        }else{
            ViewController *loginController ;
            UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
            loginController = [storyboard instantiateViewControllerWithIdentifier:@"ViewController"];
            
            
            //loginController.isWaitingView = NO;
            //[loginController showLoginView];
            
            [CMUtil insertServerIPToHistoryDataCenter:self.serverIP.text serverPort:[self.serverPort.text intValue]];
            //[theLoginController setServerIp:self.serverIP.text];
            //[theLoginController setServerPort:[self.serverPort.text intValue]];

            if(self.mysettingBlock){
                self.mysettingBlock(self.serverIP.text, [self.serverPort.text intValue]);
            }
            
            
            [self.navigationController pushViewController:loginController animated:YES];
        }
    }
}

-(void)dbTestClick{
    
    [Sqlite openSqlite];
    
    [Sqlite createTable];
    
    for(int i=0;i<2;i++)
    {
        student *stu = [[student alloc] init];
        stu.age=20;
        stu.name=@"张三";
        stu.sex=@"男";
        stu.number = i;
        
        [Sqlite addStudent:stu];
    }
    
    self.arrStudent = [Sqlite selectWithStu];

    //[_table reloadData];
    
}

//[dubai]点击分段控制执行相应的方法
- (void)handelSegementControlAction:(UISegmentedControl *)segment
{
    
    NSLog(@"%s",__FUNCTION__);
    switch (segment.selectedSegmentIndex) {
        case 0:
        {
            _aview.backgroundColor = [UIColor redColor];
            break;
        }
        case 1:
        {
            _aview.backgroundColor = [UIColor yellowColor];
            break;
        }
        case 2:
        {
            
            _aview.backgroundColor = [UIColor blueColor];
            break;
        }
            
        case 3:
        {
            _aview.backgroundColor = [UIColor greenColor];
            break;
        }
        default:
        {
            _aview.backgroundColor = [UIColor blackColor];
            break;
        }
        break;
    }
}

//
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
/*if (section==0) {
        return self.arrStudent.count;
    }
   else if(section==1){
        return 1;
    } else if(section==2) {
        return 1;
    }else if (section==3) {
        return 1;
    }else if (section==4) {
        return 1;
    }else if (section==5) {
        return 1;
    }*/
    
    if (section == 0) {
        return _dataArr.count;
    }
    else if(section == 1){
        return _data1Arr.count;
    }
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
    static NSString *CellIdentifier = @"ServerIPHistoryIdentifier";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
        
 /*       if ([indexPath section]==0 && [indexPath row]==0) {
            CGFloat cellW = tableView.frame.size.width;
            CGFloat cellH = cell.frame.size.height;
            UISwitch *switchButton = [[UISwitch alloc] initWithFrame:CGRectMake(cellW - SWITCH_MARGIN_RIGHT - SWITCH_WIDTH,cellH/2-SWITCH_HEIGHT/2 , SWITCH_WIDTH, SWITCH_HEIGHT)];
            switchButton.tag = SWITCH_TAG;
            [switchButton setOn:[CMUtil getDefaultAutoLoginTag]];
            [switchButton addTarget:self action:@selector(switchAction:) forControlEvents:UIControlEventValueChanged];
            [cell addSubview:switchButton];
        }
  */
    }
 //   else//当页面拉动的时候 当cell存在并且最后一个存在 把它进行删除就出来一个独特的cell我们在进行数据配置即可避免
 //   {
 //       while ([cell.contentView.subviews lastObject] != nil) {
//            [(UIView *)[cell.contentView.subviews lastObject] removeFromSuperview];
//        }
//    }

//    if([indexPath section] ==0)
    {
//        student* stu = [self.arrStudent objectAtIndex:[indexPath row]];
//        cell.textLabel.text = stu.name;
 //       cell.detailTextLabel.text = [NSString stringWithFormat:@"%@,%ld",stu.sex, stu.age];
        //cell.textLabel.text =@"王五1";
    }
    /*else if([indexPath section] ==1){
        cell.textLabel.text =@"王五2";
    }else if([indexPath section] ==2){
        cell.textLabel.text =@"王五3";
    }else if([indexPath section] ==3){
        cell.textLabel.text =@"王五4";
    }else if([indexPath section] ==4){
        cell.textLabel.text =@"王五5";
    }else if([indexPath section] ==5){
        cell.textLabel.text =@"王五6";
    }
    */
    //cell.textLabel.font = [UIFont systemFontOfSize:16.0f];
    //cell.accessoryType = UITableViewCellAccessoryNone;
    //cell.selectionStyle =UITableViewCellSelectionStyleBlue;
    
    if([indexPath section] ==0){
        User *user = [_dataArr objectAtIndex:indexPath.row];
        cell.textLabel.text = user.name;
        cell.detailTextLabel.text = [NSString stringWithFormat:@"%@,%d",user.sex ,user.age.intValue];
    }
    else if([indexPath section] ==1){
        cell.textLabel.text = [_data1Arr objectAtIndex:indexPath.row];
        cell.detailTextLabel.text = @"";
    }
    return cell;
}
-(void)switchAction:(id)sender
{
    UISwitch *switchButton = (UISwitch*)sender;
    [CMUtil updateAutoLoginTag:[switchButton isOn]];
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60.0f;
}

//设置表头高度
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 40.0f;
}
//设置表尾高度
/*- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section{
    return 40.0f;
}
*/
-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    static NSString *headerSectionID = @"headerSectionID";
    UITableViewHeaderFooterView *headerView = [tableView dequeueReusableHeaderFooterViewWithIdentifier:headerSectionID];
    UILabel *headerLabel;
    
    if (headerView == nil) {
        headerView = [[UITableViewHeaderFooterView alloc] initWithReuseIdentifier:headerSectionID];
        headerLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 200, 40)];
        headerLabel.backgroundColor = [UIColor redColor];
        [headerView addSubview:headerLabel];
    }
    
    if (section == 0) {
        headerLabel.text=@"热门商区";
    }else if (section == 1){
        headerLabel.text = @"分类";
    }
    
    return headerView;
}

/*-(UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section{
    static NSString *footerSectionID = @"footerSectionID";
    UITableViewHeaderFooterView *footerView = [tableView dequeueReusableHeaderFooterViewWithIdentifier:footerSectionID];
    UILabel *label;
    
    if (footerView == nil) {
        footerView = [[UITableViewHeaderFooterView alloc] initWithReuseIdentifier:footerSectionID];
        label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 200, 40)];
        label.backgroundColor = [UIColor blueColor];
        [footerView addSubview:label];
    }
    return footerView;
}
 */
/*
//添加标头中的内容
-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    static NSString *headerSectionID = @"headerSectionID";
    UITableViewHeaderFooterView *headerView = [tableView dequeueReusableHeaderFooterViewWithIdentifier:headerSectionID];
    UILabel *label;

    if (headerView == nil) {
        headerView = [[UITableViewHeaderFooterView alloc] initWithReuseIdentifier:headerSectionID];
        label = [[UILabel alloc] initWithFrame:CGRectMake(5, 5, 200, 40)];
        //label.font = FONT(13);
        [headerView addSubview:label];
    }

    if (section == 0) {
        label.text = @"我的同学1";
    }else if (section == 1) {
        label.text = @"我的同学2";
    }else if (section == 2) {
        label.text = @"我的同学3";
    }else if (section == 3) {
        label.text = @"我的同学4";
    }else if (section == 4) {
        label.text = @"我的同学5";
    }else if (section == 5) {
        label.text = @"我的同学6";
    }else{
        label.text=@"";
    }
    
    return headerView;
}
*/

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if([indexPath section] ==0){
        User *user = [_dataArr objectAtIndex:indexPath.row];
        
        SetViewController *set = [[SetViewController alloc] init];
        set.title = @"修改或者删除";
        set.type = CHANGE;
        set.user = user;
        [self.navigationController pushViewController:set animated:YES];
    }
}

/*- (void)tableView:(UITableView *)tableView commitEditingStyle:
(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    NSUInteger row = [indexPath row];
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        [CMUtil removeServerIPFromHistoryDataCenter:row];
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath]
                         withRowAnimation:UITableViewRowAnimationFade];
    }
}
*/

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}
- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath{
    if ([indexPath section] == 0){
        return UITableViewCellEditingStyleDelete;
    }
    else if ([indexPath section] == 1){
        return UITableViewCellEditingStyleInsert;
    }
    return UITableViewCellEditingStyleNone;
}
- (void)tableView:(UITableView *)tableView commitEditingStyle:
(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if ([indexPath section] == 0)
    {
        NSUInteger row = [indexPath row];
        if (editingStyle == UITableViewCellEditingStyleDelete) {
            [self.dataArr removeObjectAtIndex:row];
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath]
                             withRowAnimation:UITableViewRowAnimationAutomatic];
        }
     }
    else if ([indexPath section] == 1)
    {

        if (editingStyle == UITableViewCellEditingStyleInsert) {
            [self.data1Arr addObject:@"new"];
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
        }
    }
    [self.table reloadData];
}
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    if ([indexPath section] == 1)
        return YES;
    return NO;
}
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)
sourceIndexPath toIndexPath:(NSIndexPath *)destinationIndexPath {
    
    NSUInteger fromRow = [sourceIndexPath row];
    NSUInteger toRow = [destinationIndexPath row];
    
    id object = [self.data1Arr objectAtIndex:fromRow];
    [self.data1Arr removeObjectAtIndex:fromRow];
    [self.data1Arr insertObject:object atIndex:toRow];
    
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
