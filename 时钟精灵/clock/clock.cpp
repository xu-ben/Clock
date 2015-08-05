// clock.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "clock.h"
#include "clockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclockApp

BEGIN_MESSAGE_MAP(CclockApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
//	ON_COMMAND(ID_CLOSE, &CclockApp::OnClose)
END_MESSAGE_MAP()


// CclockApp 构造

CclockApp::CclockApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CclockApp 对象

CclockApp theApp;


// CclockApp 初始化

BOOL CclockApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("语音时钟"));
	LoadStdProfileSettings(1000);			//确保并且加载最近使用的文件和最后浏览的状态

	CclockDlg dlg;
	//this->SetDialogBkColor(RGB(0,0,0));
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
//	m_hwndDlg=NULL;
	return FALSE;
}

/*BOOL CclockApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	//if(m_hwndDlg!=NULL)
	{
		//判断消息，如果消息是从对话框发出的或者其子控件发出的，我们就进行处理。sunxin
		if((lpMsg->hwnd==m_hwndDlg)||::IsChild(m_hwndDlg,lpMsg->hwnd))
		{
			//如果消息是WM_KEYDOWN，我们就弹出一个消息框。sunxin
			if(lpMsg->message==WM_RBUTTONDOWN)
			{
				AfxMessageBox(L"捕获成功！");
			}
		}
	}
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}
*/
BOOL CclockApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
//	if(pMsg->message==WM_NCRBUTTONDOWN)
//		AfxMessageBox(L"右键消息"); 
	if(pMsg->message==WM_KEYDOWN)
		return FALSE;
//	theApp
//	if(pMsg->message==WM_KEYUP)
//		AfxMessageBox(L"左键消息"); 
//	if(pMsg->message==WM_NCMOUSELEAVE)
//		AfxMessageBox(L"鼠标消息"); 
//	if(pMsg->message==WM_MOUSELEAVE)
//		AfxMessageBox(L"鼠标离开消息"); 

	return CWinApp::PreTranslateMessage(pMsg);
}

//void CclockApp::OnClose()
//{
	// TODO: 在此添加命令处理程序代码
//}
