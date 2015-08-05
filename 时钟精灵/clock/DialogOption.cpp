// DialogOption.cpp : 实现文件
//

#include "stdafx.h"
#include "clock.h"
#include "DialogOption.h"


// CDialogOption 对话框

IMPLEMENT_DYNAMIC(CDialogOption, CDialog)

CDialogOption::CDialogOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOption::IDD, pParent)
	, CheckHalfHour(FALSE)
	, CheckRun(FALSE)
	, CheckHour(FALSE)
{

}

CDialogOption::~CDialogOption()
{
}

void CDialogOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECKHALFHOUR, CheckHalfHour);
	DDX_Check(pDX, IDC_CHECKRUN, CheckRun);
	DDX_Check(pDX, IDC_CHECKHOUR, CheckHour);
}


BEGIN_MESSAGE_MAP(CDialogOption, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogOption::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogOption 消息处理程序

BOOL CDialogOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int temp=AfxGetApp()->GetProfileInt("Sound","hoursound",0);
	if(temp==1)
		CheckHour=true;
	else
		CheckHour=false;
	temp=AfxGetApp()->GetProfileInt("Sound","halfhoursound",0);
	if(temp==1)
		CheckHalfHour=true;
	else
		CheckHalfHour=false;
	temp=AfxGetApp()->GetProfileInt("Run","run",0);
	if(temp==1)
		CheckRun=true;
	else
		CheckRun=false;
	this->UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogOption::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
	int temp;
	if(CheckHour)
		temp=1;
	else
		temp=0;
	AfxGetApp()->WriteProfileInt("Sound","hoursound",temp);
	if(CheckHalfHour)
		temp=1;
	else
		temp=0;
	AfxGetApp()->WriteProfileInt("Sound","halfhoursound",temp);
	if(CheckRun)
		temp=1;
	else
		temp=0;
	AfxGetApp()->WriteProfileInt("Run","run",temp);


	if(CheckRun)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(__argv[0],drive,dir,fname,ext);//拆分文件名，将完整路径拆分为四个部分，分别处理
		if(drive[0]>96&&drive[0]<123)
			drive[0]-=32;
		CString path(drive);
		path+=dir;
		path+="语音时钟.exe";
		TCHAR m_path[MAX_PATH];
		wsprintf(m_path,path);

		//设置本程序为开机启动
		HKEY hkey;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\Currentversion\\Run",0,KEY_WRITE,& hkey);//打开注册表项
		RegSetValueEx(hkey,"语音时钟",0,REG_SZ,(LPBYTE)m_path,lstrlen(m_path)+1);//设置注册表键值
		RegCloseKey(hkey);							//释放指定注册键的句柄
	}
	else
	{
		HKEY hkey;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_WRITE,&hkey)   ;   
		RegDeleteValue(hkey,"语音时钟");   
		RegCloseKey(hkey);   
	}

	OnOK();
}
