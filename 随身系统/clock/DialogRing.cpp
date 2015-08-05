// DialogRing.cpp : 实现文件
//

#include "stdafx.h"
#include "clock.h"
#include "DialogRing.h"


// CDialogRing 对话框

IMPLEMENT_DYNAMIC(CDialogRing, CDialog)

CDialogRing::CDialogRing(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogRing::IDD, pParent)
	, ringnum(0)
	, m_ringtime(COleDateTime::GetCurrentTime())
{

}

CDialogRing::~CDialogRing()
{
}

void CDialogRing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_ringtime);
}


BEGIN_MESSAGE_MAP(CDialogRing, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogRing::OnBnClickedOk)
	ON_BN_CLICKED(IDDELETE, &CDialogRing::OnBnClickedDelete)
END_MESSAGE_MAP()


// CDialogRing 消息处理程序

BOOL CDialogRing::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//从注册表中读取数据初始化设置对话框
	ringnum=AfxGetApp()->GetProfileIntA("ring","ringnum",0);
	CString temp;
	CString time;
	for(int i=1;i<=ringnum;i++)
	{
		temp.Format("ring%d",i);
		time=AfxGetApp()->GetProfileStringA("ring",temp);
		m_list1.InsertString(i-1,time);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogRing::OnBnClickedOk()//确认设置
{
	// TODO: 在此添加控件通知处理程序代码
	//将设置保存在注册表中
	this->UpdateData(TRUE);
	if(ringnum>=20)
	{
		AfxMessageBox("对不起，最多只能添加20个闹钟！");
		return;
	}
	ringnum++;
	AfxGetApp()->WriteProfileInt("ring","ringnum",ringnum);
	CString temp;
	CString time;
	time=m_ringtime.Format("%H:%M:%S");
	temp.Format("ring%d",ringnum);
	AfxGetApp()->WriteProfileStringA("ring",temp,time);
	m_list1.InsertString(ringnum-1,time);
	//OnOK();
}

void CDialogRing::OnBnClickedDelete()//删除闹钟
{
	CString temp;
	CString time;
	int i;
	for(i=1;i<=ringnum;i++)
	{
		temp.Format("ring%d",i);
		AfxGetApp()->WriteProfileStringA("ring",temp,NULL);
	}
	ringnum--;
	AfxGetApp()->WriteProfileInt("ring","ringnum",ringnum);
	int t=m_list1.GetCurSel();
	m_list1.DeleteString(t);
	for(i=0;i<ringnum;i++)
	{
		m_list1.GetText(i,time);
		temp.Format("ring%d",i+1);
		AfxGetApp()->WriteProfileStringA("ring",temp,time);
	}
	// TODO: 在此添加控件通知处理程序代码
}
