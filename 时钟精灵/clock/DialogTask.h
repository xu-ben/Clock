#pragma once
#include "afxwin.h"


// CDialogTask 对话框

class CDialogTask : public CDialog
{
	DECLARE_DYNAMIC(CDialogTask)

public:
	CDialogTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogTask();

// 对话框数据
	enum { IDD = IDD_DIALOGTASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_task;
	virtual BOOL OnInitDialog();
	int tasknum;
	CListBox m_list1;
	COleDateTime m_taskdate;
	COleDateTime m_tasktime;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
};
