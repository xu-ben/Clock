// clockDlg.h : 头文件
//

#pragma once
#include "atltypes.h"
#include "afxwin.h"
#include "DialogRing.h"
#include "DialogOption.h"
#include "DialogTask.h"


// CclockDlg 对话框
class CclockDlg : public CDialog
{
// 构造
public:
	CclockDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	// 时钟触发器
	UINT m_timer;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPoint point1;
	CPoint point2;
	int ss;
	float m;
	float h;
	CBrush m_brush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 报时函数
	bool SoundTime(void);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// 右键弹出菜单处理函数
	void OnContextMenu(CWnd* pWnd);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnSound();
	afx_msg void OnSetRingTime();
	afx_msg void OnOperation();
	afx_msg void OnSetTask();
	bool hoursound;
	bool halfhoursound;
	struct mytime
	{
		int hour;
		int minute;
		int second;
	};
	mytime mytimearray[20];

protected:
	int ringnum;
	int tasknum;
public:
	struct mytasktime
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		int typle;
	};
	mytasktime mytaskarray[20];
};
	static CWinThread* pThread;
	static CWinThread* pThread2;
	static CWinThread* pThread3;
	static int tasktyple=0;
	UINT ThreadProc(LPVOID pParam);
	UINT ThreadProc2(LPVOID pParam);
	UINT ThreadProc3(LPVOID pParam);
	LRESULT CALLBACK keyproc(int code,WPARAM wParam,LPARAM lParam);
	BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM IParam);
