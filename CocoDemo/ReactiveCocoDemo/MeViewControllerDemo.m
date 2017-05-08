//
//  MeViewControllerDemo.m
//  ReactiveCocoDemo
//
//  Created by SeegleET on 17/5/6.
//  Copyright © 2017年 SeegleET. All rights reserved.
//

#import "MeViewControllerDemo.h"
#import "Masonry.h"
#import "DetailViewController.h"

#define KScreen_Width [UIScreen mainScreen].bounds.size.width
#define KScreen_Height [UIScreen mainScreen].bounds.size.height

const CGFloat BackGroupHeight = 100;

@interface MeViewControllerDemo ()

//@property(nonatomic, strong) NSMutableArray* arrData;
@end

@implementation MeViewControllerDemo

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    UITableView *tableview=[[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStyleGrouped];
    
    tableview.dataSource = self;
    tableview.delegate = self;
    self.tableview = tableview;
    [self.view addSubview:self.tableview];
    
    self.tableview.tableHeaderView = [[UIView alloc]initWithFrame:CGRectMake(0.0f,0.0f,self.tableview.bounds.size.width,5.0f)];
    
    self.tableview.sectionFooterHeight = 5.0f;
    self.tableview.sectionHeaderHeight = 0;
    
    __weak __typeof(self) weakSelf = self;//这里用一个弱引用来表示self，用于下面的Block中
    [self.tableview mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(weakSelf.view.mas_top).with.offset(5);
        make.left.equalTo(weakSelf.view.mas_left).with.offset(5);
        make.right.equalTo(weakSelf.view.mas_right).with.offset(-5);
        make.bottom.equalTo(weakSelf.view.mas_bottom).with.offset(-5);
    }];
    
    //self.arrData = [[NSMutableArray alloc]init];
    //[self.arrData addObject:@"1"];
    self.tableview.contentInset=UIEdgeInsetsMake(BackGroupHeight, 0, 0, 0);
    
    imageBG=[[UIImageView alloc]init];
    imageBG.frame=CGRectMake(0, -BackGroupHeight, KScreen_Width, BackGroupHeight);
    imageBG.image=[UIImage imageNamed:@"BGimage.jpg"];
    [self.tableview addSubview:imageBG];
    
    BGView=[[UIView alloc]init];
    BGView.backgroundColor=[UIColor clearColor];
    BGView.frame=CGRectMake(0, -BackGroupHeight, KScreen_Width, BackGroupHeight);
    [self.tableview addSubview:BGView];
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat yOffset  = scrollView.contentOffset.y;
    CGFloat xOffset = (yOffset + BackGroupHeight)/2;
    
    if (yOffset < -BackGroupHeight) {
        
        CGRect rect = imageBG.frame;
        rect.origin.y = yOffset;
        rect.size.height =  -yOffset ;
        rect.origin.x = xOffset;
        rect.size.width = KScreen_Width + fabs(xOffset)*2;
        
        imageBG.frame = rect;
    }
    
    
    CGFloat alpha = (yOffset+BackGroupHeight)/BackGroupHeight;
    [self.navigationController.navigationBar setBackgroundImage:[self imageWithColor:[[UIColor orangeColor]colorWithAlphaComponent:alpha]] forBarMetrics:UIBarMetricsDefault];
    
    alpha=fabs(alpha);
    alpha=fabs(1-alpha);
    
    alpha=alpha<0.2? 0:alpha-0.2;
    
    
    BGView.alpha=alpha;
    
    
}


- (UIImage *)imageWithColor:(UIColor *)color
{
    // 描述矩形
    CGRect rect = CGRectMake(0.0f, 0.0f, 1.0f, 1.0f);
    
    // 开启位图上下文
    UIGraphicsBeginImageContext(rect.size);
    // 获取位图上下文
    CGContextRef context = UIGraphicsGetCurrentContext();
    // 使用color演示填充上下文
    CGContextSetFillColorWithColor(context, [color CGColor]);
    // 渲染上下文
    CGContextFillRect(context, rect);
    // 从上下文中获取图片
    UIImage *theImage = UIGraphicsGetImageFromCurrentImageContext();
    // 结束上下文
    UIGraphicsEndImageContext();
    
    return theImage;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
//
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 4;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0) {
        return 1;
    }else if (section == 1) {
        return 2;
    }else if (section == 2) {
        return 2;
    }else if (section == 3) {
        return 1;
    }
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
    static NSString *CellIdentifier = @"MeTable";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    UIImage *statusImage;
    
    if([indexPath section] ==0){
        if ([indexPath row] == 0) {
            cell.textLabel.text = @"我的活动";
            NSString *path = [[NSBundle mainBundle] pathForResource:@"7" ofType:@"png"];
            statusImage = [UIImage imageWithContentsOfFile:path];
        }
    }else if([indexPath section] ==1){
        if ([indexPath row] == 0) {
            cell.textLabel.text = @"当月消费";
            NSString *path = [[NSBundle mainBundle] pathForResource:@"7" ofType:@"png"];
            statusImage = [UIImage imageWithContentsOfFile:path];
            
 /*           UIView *lbl = [[UIView alloc] init]; //定义一个label用于显示cell之间的分割线（未使用系统自带的分割线），也可以用view来画分割线
            lbl.frame = CGRectMake(cell.frame.origin.x + 5, cell.frame.size.height + 15, cell.frame.size.width - 10, 1);
            lbl.backgroundColor =  [UIColor lightGrayColor];
            [cell.contentView addSubview:lbl];
  */
            
        }else if ([indexPath row] == 1) {
            cell.textLabel.text = @"账户余额";
            NSString *path = [[NSBundle mainBundle] pathForResource:@"7" ofType:@"png"];
            statusImage = [UIImage imageWithContentsOfFile:path];
            
            UILabel *label = [[UILabel alloc] init]; //定义一个在cell最右边显示的label
            label.text = @"¥0.00";
            label.font = [UIFont boldSystemFontOfSize:14];
            [label sizeToFit];
            label.backgroundColor = [UIColor clearColor];
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0) {
                label.frame =CGRectMake([UIScreen mainScreen].bounds.size.width - label.frame.size.width - 40,\
                                        21, label.frame.size.width, label.frame.size.height);
            } else {
                label.frame =CGRectMake([UIScreen mainScreen].bounds.size.width - label.frame.size.width - 20,\
                                        12, label.frame.size.width, label.frame.size.height);
            }
            [cell.contentView addSubview:label];
            label.backgroundColor = [UIColor clearColor];
            label.textColor = [UIColor grayColor];
            
        }
    }else if([indexPath section] ==2){
        if ([indexPath row] == 0) {
            cell.textLabel.text = @"邀请码";
            NSString *path = [[NSBundle mainBundle] pathForResource:@"7" ofType:@"png"];
            statusImage = [UIImage imageWithContentsOfFile:path];
            
/*            UIView *lbl = [[UIView alloc] init]; //定义一个label用于显示cell之间的分割线（未使用系统自带的分割线），也可以用view来画分割线
            lbl.frame = CGRectMake(cell.frame.origin.x + 5, cell.frame.size.height + 15, cell.frame.size.width - 10, 1);
            lbl.backgroundColor =  [UIColor lightGrayColor];
            [cell.contentView addSubview:lbl];
 */
        }else if ([indexPath row] == 1) {
            cell.textLabel.text = @"代金券";
            NSString *path = [[NSBundle mainBundle] pathForResource:@"7" ofType:@"png"];
            statusImage = [UIImage imageWithContentsOfFile:path];
            
            UILabel *label = [[UILabel alloc] init]; //定义一个在cell最右边显示的label
            label.text = @"1张";
            label.font = [UIFont boldSystemFontOfSize:14];
            [label sizeToFit];
            label.backgroundColor = [UIColor clearColor];
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0) {
                label.frame =CGRectMake([UIScreen mainScreen].bounds.size.width - label.frame.size.width - 40,\
                                        21, label.frame.size.width, label.frame.size.height);
            } else {
                label.frame =CGRectMake([UIScreen mainScreen].bounds.size.width - label.frame.size.width - 20,\
                                        12, label.frame.size.width, label.frame.size.height);
            }
            [cell.contentView addSubview:label];
            label.backgroundColor = [UIColor clearColor];
            label.textColor = [UIColor grayColor];
        }
    }else if([indexPath section] ==3){
        if ([indexPath row] == 0) {
            cell.textLabel.text = @"设置";
            NSString *path = [[NSBundle mainBundle] pathForResource:@"7" ofType:@"png"];
            statusImage = [UIImage imageWithContentsOfFile:path];
        }
    }

    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    cell.imageView.image = statusImage;
    
    return cell;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60.0f;
}
//设置表头高度
/*- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 1.0f;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    UIView* view=[[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 1)];
    view.backgroundColor = [UIColor clearColor];
    return view ;
}
*/
/*
-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    static NSString *headerSectionID = @"headerSectionID";
    UITableViewHeaderFooterView *headerView = [tableView dequeueReusableHeaderFooterViewWithIdentifier:headerSectionID];
    UILabel *headerLabel;
    
    if (headerView == nil) {
        headerView = [[UITableViewHeaderFooterView alloc] initWithReuseIdentifier:headerSectionID];
        headerLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 200)];
        headerLabel.backgroundColor = [UIColor redColor];
        [headerView addSubview:headerLabel];
    }

    return headerView;
}
*/

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    DetailViewController *detailViewController = [[DetailViewController alloc] init];
    if([indexPath section] == 0){
        if ([indexPath row] == 0) {
            detailViewController.type = myaction;
            detailViewController.title = @"我的活动";
        }
    }else if([indexPath section] == 1){
        if ([indexPath row] == 0) {
            detailViewController.type = no;
            detailViewController.title = @"当月消费";
        }else if ([indexPath row] == 1) {
            detailViewController.type = no;
            detailViewController.title = @"账户余额";
        }
    }else if([indexPath section] == 2){
        if ([indexPath row] == 0) {
            detailViewController.type = no;
            detailViewController.title = @"邀请码";
        }else if ([indexPath row] == 1) {
            detailViewController.type = no;
            detailViewController.title = @"代金券";
        }
    }else if([indexPath section] == 3){
        if ([indexPath row] == 0) {
            detailViewController.type = no;
            detailViewController.title = @"设置";
        }
    }

    
    [self.navigationController pushViewController:detailViewController animated:YES];
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
