#include "clockDlg.h"
#pragma once


// CDialogOption 对话框

class CDialogOption : public CDialog
{
	DECLARE_DYNAMIC(CDialogOption)

public:
	CDialogOption(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogOption();

// 对话框数据
	enum { IDD = IDD_DIALOGOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	BOOL CheckHalfHour;
protected:
	BOOL CheckRun;

public:
	BOOL CheckHour;
public:
	afx_msg void OnBnClickedOk();
};
//CString PATH;