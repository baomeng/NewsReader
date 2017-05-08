//
//  CMDefaultSettingViewController.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/4/18.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "CMDefaultSettingViewController.h"
#import "CMUtil.h"
#import "Masonry.h"
#import "CMCenterManager.h"
#import "AppDelegate.h"
#import "ModalAlert.h"

#define   SWITCH_MARGIN_RIGHT           0
#define   SWITCH_WIDTH                  79
#define   SWITCH_HEIGHT                 27
#define   SWITCH_TAG                    2000

@interface CMDefaultSettingViewController ()



@end

@implementation CMDefaultSettingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    self.title = NSLocalizedString(@"默认设置", nil);
    
    self.table = [[UITableView alloc]init];
    //self.table.backgroundColor = [UIColor greenColor];
    [self.view addSubview:self.table];
    
    [self.table setDelegate:self];
    [self.table setDataSource:self];
    
    self.table.translatesAutoresizingMaskIntoConstraints = NO;
    
    __weak __typeof(self) weakSelf = self;//这里用一个弱引用来表示self，用于下面的Block中
    
    //先确定view_1的约束
    [self.table mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(weakSelf.view.mas_top).with.offset(10);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(10);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-10);
        make.bottom.equalTo(weakSelf.view.mas_bottom).with.offset(-10);
    }];
    
    
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    self.table = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 3;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    if (section==0) {
        return  2;
    }
    else if(section==1)
        return 1;
    else if(section==2)
        return 1;
    else
        return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"DefaultSetting";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
        if ([indexPath section]==0 && [indexPath row]==0) {
            CGFloat cellW = tableView.frame.size.width;
            CGFloat cellH = cell.frame.size.height;
            UISwitch *switchButton = [[UISwitch alloc] initWithFrame:CGRectMake(cellW - SWITCH_MARGIN_RIGHT - SWITCH_WIDTH,cellH/2-SWITCH_HEIGHT/2 , SWITCH_WIDTH, SWITCH_HEIGHT)];
            switchButton.tag = SWITCH_TAG;
            [switchButton setOn:[CMUtil getDefaultAutoLoginTag]];
            [switchButton addTarget:self action:@selector(switchAction:) forControlEvents:UIControlEventValueChanged];
            [cell addSubview:switchButton];
        }
    }
    UISwitch *autoSwitch = (UISwitch *)[cell viewWithTag:SWITCH_TAG];
    if (autoSwitch!=nil) {
        [autoSwitch setOn:[CMUtil getDefaultAutoLoginTag]];
    }
    // Configure the cell...
    if ([indexPath section]==0 && [indexPath row]==0) {
        cell.textLabel.text = NSLocalizedString(@"自动登录", nil);
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    else if ([indexPath section]==0 && [indexPath row]==1) {
        cell.accessoryType= UITableViewCellAccessoryDisclosureIndicator;
        cell.textLabel.text = NSLocalizedString(@"恢复默认", nil);
    }
    else if ([indexPath section]==1 && [indexPath row]==0) {
        NSString* title = [NSString stringWithFormat:@"%@  %s",NSLocalizedString(@"当前版本：", nil),CM_CLIENT_VERSION];
        cell.textLabel.text = title;
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    else if ([indexPath section]==2 && [indexPath row]==0) {
        cell.textLabel.text = NSLocalizedString(@"关于", nil);
    }
    
    return cell;
}

/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */

/*
 // Override to support editing the table view.
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
 {
 if (editingStyle == UITableViewCellEditingStyleDelete) {
 // Delete the row from the data source
 [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
 }
 else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
 }
 }
 */

/*
 // Override to support rearranging the table view.
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
 {
 }
 */

/*
 // Override to support conditional rearranging of the table view.
 - (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Return NO if you do not want the item to be re-orderable.
 return YES;
 }
 */

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
    /*
     DetailViewController *detailViewController = [[DetailViewController alloc] initWithNibName:@"Nib name" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
     [self.navigationController pushViewController:detailViewController animated:YES];
     [detailViewController release];
     */
 /*   if ([indexPath section]==0 && [indexPath row]==1) {
        CMRegainHistoryController *regainController;
        switch ([CMUtil getDeviceModel]) {
            case DeviceModel_iPhone:
            {
                regainController = [[CMRegainHistoryController alloc] initWithNibName:@"CMDefaultSettingView" bundle:nil];
                break;
            }
            case DeviceModel_iPad:
            {
                regainController = [[CMRegainHistoryController alloc] initWithNibName:@"CMDefaultSettingView-iPad" bundle:nil];
                break;
            }
            default:
                regainController = [[CMRegainHistoryController alloc] initWithNibName:@"CMDefaultSettingView" bundle:nil];
                break;
        }
        //  settingController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
        [self.navigationController pushViewController:regainController animated:YES];
        [regainController release];
    }else
 */
    if([indexPath section]==1 && [indexPath row]==0){
        [self updateAction];
    }
 
}
-(void)switchAction:(id)sender
{
    UISwitch *switchButton = (UISwitch*)sender;
    [CMUtil updateAutoLoginTag:[switchButton isOn]];
}
-(void)updateAction
{
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    CMCenterManager *centerManager = appDelegate.centerManager;
   
    NSString* updateInfo = [appDelegate updateIdentifier];
    if (updateInfo!=nil && ![updateInfo isEqualToString:@""]) {
        if ([centerManager.idAccountModule isUpdateEnable]) {
            NSString* msg = NSLocalizedString(@"update_info", nil);
            if ([ModalAlert ask:msg]== YES){
                NSString* strUrl = [NSString stringWithFormat:@"%@%@",updateInfoPrefix,updateInfo ];
                NSLog(@"strUrl info = %@",strUrl);
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];
            }
        }else{
            NSString* msg = NSLocalizedString(@"已是最新版本。", nil);
            [ModalAlert MessageBox:msg];
        }
    }
 
}

@end
