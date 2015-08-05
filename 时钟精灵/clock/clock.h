// clock.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "clockdlg.h"
#include "DialogRing.h"
#include "DialogTask.h"
#include "DialogOption.h"

// CclockApp:
// 有关此类的实现，请参阅 clock.cpp
//

class CclockApp : public CWinApp
{
public:
	CclockApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
//	HWND m_hwndDlg;

protected:
//	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	CclockDlg dlg;
//	afx_msg void OnClose();
};

extern CclockApp theApp;