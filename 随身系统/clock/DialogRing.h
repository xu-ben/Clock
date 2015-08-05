#pragma once
#include "afxwin.h"


// CDialogRing 对话框

class CDialogRing : public CDialog
{
	DECLARE_DYNAMIC(CDialogRing)

public:
	CDialogRing(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogRing();

// 对话框数据
	enum { IDD = IDD_DIALOGRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list1;
	virtual BOOL OnInitDialog();
	int ringnum;
	afx_msg void OnBnClickedOk();
	COleDateTime m_ringtime;
	afx_msg void OnBnClickedDelete();
};
