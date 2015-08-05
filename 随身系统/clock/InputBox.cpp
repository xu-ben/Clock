// InputBox.cpp : 实现文件
//

#include "stdafx.h"
#include "clock.h"
#include "InputBox.h"


// CInputBox 对话框

IMPLEMENT_DYNAMIC(CInputBox, CDialog)

CInputBox::CInputBox(CWnd* pParent /*=NULL*/)
	: CDialog(CInputBox::IDD, pParent)
	, InputStr(_T(""))
{

}

CInputBox::~CInputBox()
{
}

void CInputBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITINPUT, InputStr);
}


BEGIN_MESSAGE_MAP(CInputBox, CDialog)
END_MESSAGE_MAP()


// CInputBox 消息处理程序
