// DialogTask.cpp : 实现文件
//

#include "stdafx.h"
#include "clock.h"
#include "DialogTask.h"


// CDialogTask 对话框

IMPLEMENT_DYNAMIC(CDialogTask, CDialog)

CDialogTask::CDialogTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTask::IDD, pParent)
	, tasknum(0)
	, m_taskdate(COleDateTime::GetCurrentTime())
	, m_tasktime(COleDateTime::GetCurrentTime())
{

}

CDialogTask::~CDialogTask()
{
}

void CDialogTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_task);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_taskdate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_tasktime);
}


BEGIN_MESSAGE_MAP(CDialogTask, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogTask::OnBnClickedOk)
	ON_BN_CLICKED(IDDELETE, &CDialogTask::OnBnClickedDelete)
END_MESSAGE_MAP()


// CDialogTask 消息处理程序

BOOL CDialogTask::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//从注册表中读取定时任务初始化对话框
	m_task.SetCurSel(0);
	tasknum=AfxGetApp()->GetProfileIntA("task","tasknum",0);
	CString temp;
	CString task;
	CString taskdate;
	CString tasktime;
	int taskt;
	for(int i=1;i<=tasknum;i++)
	{
		temp.Format("task%d",i);
		taskt=AfxGetApp()->GetProfileIntA("task",temp,0);
		switch(taskt)
		{
		case 0:task="0:未选择操作";break;
		case 1:task="1:关闭计算机";break;
		case 2:task="2:重新启动计算机";break;
		case 3:task="3:打开浏览器";break;
		case 4:task="4:打开文件夹";break;
		case 5:task="5:打开程序";break;
		case 6:task="6:整理磁盘碎片";break;
		case 7:task="7:打开光驱";break;
		case 8:task="8:扫描病毒";break;
		case 9:task="9: 关闭计算机(倒计时30秒提醒)";break;
		case 10:task="10: 重新启动计算机(倒计时30秒提醒)";break;
		case 11:task="11: 注销";break;
		case 12:task="12: 锁定键盘";break;
		case 13:task="13: 锁定鼠标";break;
		case 14:task="14: 隐藏桌面";break;
		case 15:task="15: 关闭显示器";break;
		case 16:task="16: 关闭所有窗口";break;
		default:task="?:????";break;
		}
		temp.Format("taskdate%d",i);
		taskdate=AfxGetApp()->GetProfileStringA("taskdate",temp);
		temp.Format("tasktime%d",i);
		tasktime=AfxGetApp()->GetProfileStringA("tasktime",temp);
		taskdate+="  ";
		taskdate+=tasktime;
		taskdate+="  执行操作";
		taskdate+=task;
		m_list1.InsertString(i-1,taskdate);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogTask::OnBnClickedOk()//确认设置
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
	if(tasknum>=20)
	{
		AfxMessageBox("对不起，最多只能添加20项任务！");
		return;
	}
	tasknum++;
	AfxGetApp()->WriteProfileInt("task","tasknum",tasknum);
	CString temp;
	CString temp2;
	CString task;
	temp=m_taskdate.Format("%Y-%m-%d");
	task+=temp;
	task+="  ";
	temp2.Format("taskdate%d",tasknum);
	AfxGetApp()->WriteProfileStringA("taskdate",temp2,temp);
	temp=m_tasktime.Format("%H:%M:%S");
	task+=temp;
	task+="  执行操作";
	temp2.Format("tasktime%d",tasknum);
	AfxGetApp()->WriteProfileStringA("tasktime",temp2,temp);
	int index=m_task.GetCurSel();
	temp2.Format("task%d",tasknum);
	AfxGetApp()->WriteProfileInt("task",temp2,index);
	switch(index)
	{
		case 0:task+="0: 未选择操作";break;
		case 1:task+="1: 关闭计算机";break;
		case 2:task+="2: 重新启动计算机";break;
		case 3:task+="3: 打开浏览器";break;
		case 4:task+="4: 打开文件夹";break;
		case 5:task+="5: 打开程序";break;
		case 6:task+="6: 整理磁盘碎片";break;
		case 7:task+="7: 打开光驱";break;
		case 8:task+="8: 扫描病毒";break;
		case 9:task+="9: 关闭计算机(倒计时30秒提醒)";break;
		case 10:task+="10: 重新启动计算机(倒计时30秒提醒)";break;
		case 11:task+="11: 注销";break;
		case 12:task+="12: 锁定键盘";break;
		case 13:task+="13: 锁定鼠标";break;
		case 14:task+="14: 隐藏桌面";break;
		case 15:task+="15: 关闭显示器";break;
		case 16:task+="16: 关闭所有窗口";break;
		default:task+="?: ????";break;
	}
	m_list1.InsertString(tasknum-1,task);
	this->UpdateData(FALSE);
	//OnOK();
}

void CDialogTask::OnBnClickedDelete()//删除任务
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
	CString temp;
	CString task;
	char date[12];
	char time[10];
	int curtask;
	int i,j;
	for(i=1;i<=tasknum;i++)
	{
		temp.Format("task%d",i);
		AfxGetApp()->WriteProfileInt("task",temp,NULL);
		temp.Format("taskdate%d",i);
		AfxGetApp()->WriteProfileStringA("taskdate",temp,NULL);
		temp.Format("tasktime%d",i);
		AfxGetApp()->WriteProfileStringA("tasktime",temp,NULL);
	}
	tasknum--;
	AfxGetApp()->WriteProfileInt("task","tasknum",tasknum);
	int t=m_list1.GetCurSel();
	m_list1.DeleteString(t);
	for(i=0;i<tasknum;i++)
	{
		m_list1.GetText(i,task);
		for(j=0;j<10;j++)
		{
			date[j]=task[j];
		}
		date[10]='\0';
		for(j=12;j<=19;j++)
		{
			time[j-12]=task[j];
		}
		time[8]='\0';
		if(task[31]==':')
			curtask=task[30]-48;
		else
			curtask=(task[30]-48)*10+task[31]-48;
		temp.Format("task%d",i+1);
		AfxGetApp()->WriteProfileInt("task",temp,curtask);
		temp.Format("taskdate%d",i+1);
		AfxGetApp()->WriteProfileStringA("taskdate",temp,date);
		temp.Format("tasktime%d",i+1);
		AfxGetApp()->WriteProfileStringA("tasktime",temp,time);
	}
	this->UpdateData(FALSE);

}
