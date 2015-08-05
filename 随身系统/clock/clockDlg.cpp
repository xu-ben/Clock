// clockDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "clock.h"
#include "clockDlg.h"
#include "InputBox.h"
#include <cmath>
#include <mmsystem.h> 
#pragma comment(lib,"winmm.lib")
#define pi 3.1415926535897932384626433832795028841971693993751058209

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclockDlg 对话框




CclockDlg::CclockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CclockDlg::IDD, pParent)
	, m_timer(0)
	, point1(0)
	, point2(0)
	, ss(0)
	, m(0)
	, h(0)
	, hoursound(false)
	, halfhoursound(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CclockDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_CLOSE, &CclockDlg::OnClose)
	ON_COMMAND(ID__SOUNDTIME, &CclockDlg::OnSound)
	ON_COMMAND(ID__RING, &CclockDlg::OnSetRingTime)
	ON_COMMAND(ID__OPERATION, &CclockDlg::OnOperation)
	ON_COMMAND(ID__TASK, &CclockDlg::OnSetTask)
END_MESSAGE_MAP()


// CclockDlg 消息处理程序

BOOL CclockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	//SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设置程序运行时不在任务栏的运行区显示图标
	SetWindowLong(this->m_hWnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); 
	
	//设置控制时针走动的触发器为每秒一次，即引发WM_TIMER消息的频率是每秒一次。
	m_timer = this->SetTimer(1, 1000, NULL);
	
/////////////////////////////////////////////////////////////
	//画圆形对话框
	CRgn  rgn;   
	CRect  rc;   
	GetClientRect(&rc);   
    rgn.CreateEllipticRgn(rc.left,rc.top,rc.right,rc.bottom);   
    SetWindowRgn(rgn,TRUE);   
    rgn.DeleteObject();  
//////////////////////////////////////////////////////////////
	//用时钟背景图片作圆形对话框背景
	CBitmap   bm;   
	bm.LoadBitmap(IDB_BITMAP1);   //   IDB_BITMAP1为BITMAP资源ID,可以指定bitmap图片的路径   
	m_brush.CreatePatternBrush(&bm);   

	//SoundTime();
	//sndPlaySoundA("AM.wav",SND_ASYNC);
	//将对话框的句柄传递到CWinSunApp类中。sunxin
//	((CclockApp*)AfxGetApp())->m_hwndDlg=m_hWnd;



	//创建报时线程
	DWORD dwParam;
	pThread=AfxBeginThread(ThreadProc,&dwParam,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED,0);
	if(pThread==NULL)
		AfxMessageBox("创建线程错误!");


	//创建闹钟线程
	DWORD dwParam2;
	pThread2=AfxBeginThread(ThreadProc2,&dwParam2,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED,0);
	if(pThread2==NULL)
		AfxMessageBox("创建线程错误!");


	//创建处理定时任务线程
	DWORD dwParam3;
	pThread3=AfxBeginThread(ThreadProc3,&dwParam3,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED,0);
	if(pThread3==NULL)
		AfxMessageBox("创建线程错误!");



	int temp=AfxGetApp()->GetProfileInt("Sound","hoursound",0);
	if(temp==1)				//判断是否整点报时
		hoursound=true;
	else
		hoursound=false;
	temp=AfxGetApp()->GetProfileInt("Sound","halfhoursound",0);
	if(temp==1)				//判断是否半点报时
		halfhoursound=true;
	else
		halfhoursound=false;



	ringnum=AfxGetApp()->GetProfileIntA("ring","ringnum",0);//闹钟数量
	CString time;
	CString temps;
	for(int i=0;i<ringnum;i++)//从注册表中取出闹钟信息，存在结构体数组中
	{
		temps.Format("ring%d",i+1);
		time=AfxGetApp()->GetProfileStringA("ring",temps);
		mytimearray[i].hour=(time[0]-48)*10+time[1]-48;
		mytimearray[i].minute=(time[3]-48)*10+time[4]-48;
		mytimearray[i].second=(time[6]-48)*10+time[7]-48;
	}

	tasknum=AfxGetApp()->GetProfileIntA("task","tasknum",0);//定时任务数量
	CString taskdate;
	CString tasktime;
	for(int i=0;i<tasknum;i++)		//从注册表中取出定时任务信息，保存在定时任务结构体中
	{
		temps.Format("taskdate%d",i+1);
		taskdate=AfxGetApp()->GetProfileStringA("taskdate",temps);
		temps.Format("tasktime%d",i+1);
		tasktime=AfxGetApp()->GetProfileStringA("tasktime",temps);
		mytaskarray[i].year=(taskdate[0]-48)*1000+(taskdate[1]-48)*100+(taskdate[2]-48)*10+taskdate[3]-48;
		mytaskarray[i].month=(taskdate[5]-48)*10+taskdate[6]-48;
		mytaskarray[i].day=(taskdate[8]-48)*10+taskdate[9]-48;
		mytaskarray[i].hour=(tasktime[0]-48)*10+tasktime[1]-48;
		mytaskarray[i].minute=(tasktime[3]-48)*10+tasktime[4]-48;
		mytaskarray[i].second=(tasktime[6]-48)*10+tasktime[7]-48;
		temps.Format("task%d",i+1);
		mytaskarray[i].typle=AfxGetApp()->GetProfileIntA("task",temps,0);
	}





	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CclockDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		OnTimer(m_timer);
		CDialog::OnPaint();
	}	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CclockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CclockDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION :CDialog::OnNcHitTest(point);

//	return CDialog::OnNcHitTest(point);
}

void CclockDlg::OnTimer(UINT_PTR nIDEvent)//控制时钟走动
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//判断传递过来的时钟触发器是否是自己定义的时钟触发器
	if(nIDEvent == m_timer) 
	{
		//获得当前系统时间。
		CTime time = CTime::GetCurrentTime();
		int C;							//用于计算颜色
		CPen *PenOld,PenNew;
		CBrush *BrushOld,BrushNew;
		CClientDC dc(this);
		int S=time.GetSecond();
		float M=float(time.GetMinute()+S/60.0);
		float H=float(time.GetHour()+M/60.0);
		if(H>12)
			H=H-12;
		H=H*5;
		point1.x=103;
		point1.y=102;

//		OnPaint();
		//以下为画时针分针和秒针
		//方法为画每根针前用背景色擦去上一次画的针（由于背景色渐变，所以加入了计算）
//////////////////////////////////////////////	
		if(h<5)
			C=232;
		else if(h<10)
			C=235;
		else if(h<15)
			C=242;
		else if(h<20)
			C=247;
		else if(h<30)
			C=248;
		else if(h<35)
			C=244;
		else if(h<40)
			C=242;
		else if(h<47)
			C=235;
		else
			C=230;

		PenNew.CreatePen(PS_SOLID,4,RGB(C,C,C));
		BrushNew.CreateSolidBrush(RGB(C,C,C));
		BrushOld=dc.SelectObject(&BrushNew);
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+35*sin(h*pi/30);
		point2.y=102-35*cos(h*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);


		PenNew.DeleteObject();
		PenNew.CreatePen(PS_SOLID,4,RGB(0,0,0));
		PenOld=dc.SelectObject(&PenNew);
		BrushNew.DeleteObject();
		BrushNew.CreateSolidBrush(RGB(0,0,0));
		BrushOld=dc.SelectObject(&BrushNew);
		point2.x=103+35*sin(H*pi/30);
		point2.y=102-35*cos(H*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);
///////////////////////////////////////////////
		if(m<5)
			C=232;
		else if(m<10)
			C=235;
		else if(m<15)
			C=242;
		else if(m<20)
			C=247;
		else if(m<30)
			C=248;
		else if(m<35)
			C=244;
		else if(m<40)
			C=242;
		else if(m<47)
			C=235;
		else
			C=230;

		BrushNew.DeleteObject();
		BrushNew.CreateSolidBrush(RGB(C,C,C));
		BrushOld=dc.SelectObject(&BrushNew);
		PenNew.DeleteObject();
		PenNew.CreatePen(PS_SOLID,4,RGB(C,C,C));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+55*sin(m*pi/30);
		point2.y=102-55*cos(m*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);



		BrushNew.DeleteObject();
		BrushNew.CreateSolidBrush(RGB(0,0,0));
		BrushOld=dc.SelectObject(&BrushNew);
		PenNew.DeleteObject();
		PenNew.CreatePen(PS_SOLID,4,RGB(0,0,0));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+55*sin(M*pi/30);
		point2.y=102-55*cos(M*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);
		
////////////////////////////////////////////	画秒针的短轴	
		ss=(ss+30)%60;
		S=(S+30)%60;
		if(ss<5)
			C=232;
		else if(ss<10)
			C=235;
		else if(ss<15)
			C=242;
		else if(ss<20)
			C=247;
		else if(ss<30)
			C=248;
		else if(ss<35)
			C=244;
		else if(ss<40)
			C=242;
		else if(ss<47)
			C=235;
		else
			C=230;
		PenNew.DeleteObject();
		PenNew.CreatePen(PS_DASHDOTDOT,2,RGB(C,C,C));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+10*sin(ss*pi/30);
		point2.y=102-10*cos(ss*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);


		PenNew.DeleteObject();
		PenNew.CreatePen(PS_DASHDOTDOT,2,RGB(255,0,0));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+10*sin(S*pi/30);
		point2.y=102-10*cos(S*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);


///////////////////////画秒针的长轴
		ss=(ss+30)%60;
		S=(S+30)%60;
		if(ss<5)
			C=232;
		else if(ss<10)
			C=235;
		else if(ss<15)
			C=242;
		else if(ss<20)
			C=247;
		else if(ss<30)
			C=248;
		else if(ss<35)
			C=244;
		else if(ss<40)
			C=242;
		else if(ss<47)
			C=235;
		else
			C=230;
//		BrushNew.DeleteObject();
//		BrushNew.CreateSolidBrush(RGB(C,C,C));
//		BrushOld=dc.SelectObject(&BrushNew);
		PenNew.DeleteObject();
		PenNew.CreatePen(PS_DASHDOTDOT,2,RGB(C,C,C));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+63*sin(ss*pi/30);
		point2.y=102-63*cos(ss*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);


//		BrushNew.DeleteObject();
//		BrushNew.CreateSolidBrush(RGB(255,0,0));
//		BrushOld=dc.SelectObject(&BrushNew);
		PenNew.DeleteObject();
		PenNew.CreatePen(PS_DASHDOTDOT,2,RGB(255,0,0));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+63*sin(S*pi/30);
		point2.y=102-63*cos(S*pi/30);
		dc.MoveTo(point1);
		dc.LineTo(point2);



/*//////////////////////////////////////////////////////////////////
		CBrush m_brush_trad_s;
		CBitmap   bm_trad_s;   
		bm_trad_s.LoadBitmap(IDB_Trad_s);   //   IDB_Trad_s为BITMAP资源ID,可以指定bitmap图片的路径   
		m_brush_trad_s.CreatePatternBrush(&bm_trad_s); 
		BrushOld=dc.SelectObject(&m_brush_trad_s);
		PenNew.DeleteObject();
		PenNew.CreatePen(PS_NULL,1,RGB(255,255,255));//(PS_DASHDOTDOT,2,RGB(255,0,0));
		PenOld=dc.SelectObject(&PenNew);
		point2.x=103+63*sin(S*pi/30);
		point2.y=102-63*cos(S*pi/30);
		dc.Rectangle(94,40,107,169);
*///////////////////////////////////////////////////////////////////
		h=H;m=M;ss=S;
//////////////////////////////////////////////////////////////////
		dc.SetPixel(point1,RGB(0,0,0));
		dc.SetPixel(point1.x+1,point1.y,RGB(0,0,0));
		dc.SetPixel(point1.x,point1.y+1,RGB(0,0,0));
		dc.SetPixel(point1.x+1,point1.y+1,RGB(0,0,0));
		dc.SetPixel(point1.x-1,point1.y,RGB(0,0,0));
		dc.SetPixel(point1.x,point1.y-1,RGB(0,0,0));
		dc.SetPixel(point1.x-1,point1.y-1,RGB(0,0,0));





		for(int i=0;i<ringnum;i++)//判断是否有闹钟应被执行
		{
			if(time.GetHour()==mytimearray[i].hour&&time.GetMinute()==mytimearray[i].minute&&time.GetSecond()==mytimearray[i].second)
			{
				pThread2->ResumeThread();
				break;
			}
		}



		for(int i=0;i<tasknum;i++)//判断是否有定时任务应被执行
		{
			if(time.GetYear()==mytaskarray[i].year&&time.GetMonth()==mytaskarray[i].month&&time.GetDay()==mytaskarray[i].day)
			{
				if(time.GetHour()==mytaskarray[i].hour&&time.GetMinute()==mytaskarray[i].minute&&time.GetSecond()==mytaskarray[i].second)
				{
					tasktyple=mytaskarray[i].typle;
					pThread3->ResumeThread();
					break;
				}
			}
		}


		if(hoursound&&S==0&&M==0)//判断是否整点报时
			SoundTime();
		//判断是否半点报时
		if(halfhoursound&&S==0&&(M==0||M==30))
			SoundTime();
		this->UpdateData(false);
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CclockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brush;
}

// 报时函数
bool CclockDlg::SoundTime(void)
{
	pThread->ResumeThread();//激活报时线程
	return true;
}

//void CclockDlg::OnRButtonDown(UINT nFlags, CPoint point)
//{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	MessageBox(L"调用我之前");
//	OnContextMenu(this);
//	MessageBox(L"调用我之后");

//	CDialog::OnRButtonDown(nFlags, point);
//}

// 右键弹出菜单处理函数
void CclockDlg::OnContextMenu(CWnd* pWnd)
{
	CMenu menu;
	if(menu.LoadMenu(IDR_MENU))
	{
		CMenu * popup=menu.GetSubMenu(0);
		ASSERT(popup!=NULL);
		CPoint mouse;
		GetCursorPos(& mouse);
		//::SetForegroundWindow(data.hWnd);
		popup->TrackPopupMenu(TPM_LEFTALIGN,mouse.x,mouse.y,pWnd,NULL);
	}
}

void CclockDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)//右键点击窗体消息处理函数
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnContextMenu(this);
	CDialog::OnNcRButtonDown(nHitTest, point);
}

void CclockDlg::OnClose()//关闭时钟程序
{
	// TODO: 在此添加命令处理程序代码
	pThread->SuspendThread();//关闭程序前挂起其它三个线程
	pThread2->SuspendThread();
	pThread3->SuspendThread();
	exit(0);
}

void CclockDlg::OnSound()
{
	// TODO: 在此添加命令处理程序代码
	//pThread->ResumeThread();
	SoundTime();
}
UINT ThreadProc(LPVOID pParam)//报时线程
{
	while(1)
	{
		pThread3->SuspendThread();//挂起其它线程
		pThread2->SuspendThread();
		CTime m_NowTime;
		m_NowTime = CTime::GetCurrentTime();
		int n_Hour = m_NowTime.GetHour();
		//以下为报时代码,原理为通过对当前时间的判断,调用不同的声音文件(资源文件)进行组合播放,形成报时效果
		PlaySound(MAKEINTRESOURCE(IDR_WAVETIMENOW),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		Sleep(1550);
		if(n_Hour<8)
			PlaySound(MAKEINTRESOURCE(IDR_WAVEMM),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		else if(n_Hour<13)
			PlaySound(MAKEINTRESOURCE(IDR_WAVEAM),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		else if(n_Hour<19)
			PlaySound(MAKEINTRESOURCE(IDR_WAVEPM),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		else
			PlaySound(MAKEINTRESOURCE(IDR_WAVEEM),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		if (n_Hour>12)
		{
		   n_Hour = n_Hour-12;
		}
		int n_Minute = m_NowTime.GetMinute();
		int n_Second = m_NowTime.GetSecond();

		//PlaySound(("c:\\chs\\4.wav"),NULL, SND_ASYNC|SND_NODEFAULT );

		//sndPlaySound ("c:\\chs\\4.wav",SND_ALIAS);

		Sleep(580);
		switch (n_Hour)
		{
		case 0:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET00),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		case 1:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET01),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 2:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET2),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 3:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET03),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 4:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET04),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 5:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET05),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 6:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET06),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 7:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET07),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 8:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET08),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 9:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET09),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 10:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET10),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 11:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET11),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		case 12:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET12),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		   break;
		}
		if(n_Hour<10)
			Sleep(450);
		else
			Sleep(550);
		PlaySound(MAKEINTRESOURCE(IDR_WAVEPOINT),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		Sleep(500);

		int n_Front_Min;
		int n_Behind_Min;
		int n_FTemp = n_Minute;

		n_Front_Min = n_Minute/10;
		n_Behind_Min = n_Minute%10;

		if (n_Front_Min<=0)
		{
			if(n_FTemp!=0)
				PlaySound(MAKEINTRESOURCE(IDR_WAVET00),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			Sleep(400);
			switch (n_FTemp)
			{
			case 0:
			break;
			case 1:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET01),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 2:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET02),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 3:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET03),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 4:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET04),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 5:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET05),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 6:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET06),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 7:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET07),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 8:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET08),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			case 9:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET09),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
			default:
			break;
			}
			Sleep(500);
			if(n_FTemp!=0)
				PlaySound(MAKEINTRESOURCE(IDR_WAVEMIN),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);

		}
		else
		{
		   switch (n_Front_Min)
		   {
		   case 1:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET10),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 2:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET20),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 3:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET30),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 4:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET40),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 5:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET50),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   }
		   if(n_Front_Min==1)
			   Sleep(470);
		   else if(n_Front_Min==4)
			   Sleep(565);
		   else if(n_Behind_Min>0)
			   Sleep(520);
		   switch (n_Behind_Min)
		   {
		   case 1:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET01),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 2:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET02),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 3:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET03),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 4:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET04),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 5:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET05),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 6:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET06),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 7:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET07),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 8:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET08),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   case 9:
			PlaySound(MAKEINTRESOURCE(IDR_WAVET09),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
			break;
		   }

		   Sleep(560);
			PlaySound(MAKEINTRESOURCE(IDR_WAVEMIN),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		}
		pThread3->ResumeThread();
		pThread->SuspendThread();
	}
	return 0;
}



void CclockDlg::OnSetRingTime()//设置闹钟时间
{
	CDialogRing dlg(this);
	dlg.DoModal();//调出设置闹钟对话框,将设置保存在注册表中
	ringnum=AfxGetApp()->GetProfileIntA("ring","ringnum",0);
	CString time;
	CString temps;
	for(int i=0;i<ringnum;i++)//从注册表中读取修改后的数据存入结构体数组
	{
		temps.Format("ring%d",i+1);
		time=AfxGetApp()->GetProfileStringA("ring",temps);
		mytimearray[i].hour=(time[0]-48)*10+time[1]-48;
		mytimearray[i].minute=(time[3]-48)*10+time[4]-48;
		mytimearray[i].second=(time[6]-48)*10+time[7]-48;
	}
	// TODO: 在此添加命令处理程序代码
}

void CclockDlg::OnOperation()//选项菜单
{
	CDialogOption dlg(this);
	dlg.DoModal();//选项对话框
	if(dlg.CheckHour)
		hoursound=true;
	else
		hoursound=false;
	if(dlg.CheckHalfHour)
		halfhoursound=true;
	else
		halfhoursound=false;

	// TODO: 在此添加命令处理程序代码
}

void CclockDlg::OnSetTask()//设置定时任务
{
	// TODO: 在此添加命令处理程序代码
	CDialogTask dlg(this);
	dlg.DoModal();//调出设置定时任务对话框,将设置保存在注册表中
	tasknum=AfxGetApp()->GetProfileIntA("task","tasknum",0);
	CString taskdate;
	CString tasktime;
	CString temps;
	for(int i=0;i<tasknum;i++)//从注册表中读取修改后的数据存入结构体数组
	{
		temps.Format("taskdate%d",i+1);
		taskdate=AfxGetApp()->GetProfileStringA("taskdate",temps);
		temps.Format("tasktime%d",i+1);
		tasktime=AfxGetApp()->GetProfileStringA("tasktime",temps);
		mytaskarray[i].year=(taskdate[0]-48)*1000+(taskdate[1]-48)*100+(taskdate[2]-48)*10+taskdate[3]-48;
		mytaskarray[i].month=(taskdate[5]-48)*10+taskdate[6]-48;
		mytaskarray[i].day=(taskdate[8]-48)*10+taskdate[9]-48;
		mytaskarray[i].hour=(tasktime[0]-48)*10+tasktime[1]-48;
		mytaskarray[i].minute=(tasktime[3]-48)*10+tasktime[4]-48;
		mytaskarray[i].second=(tasktime[6]-48)*10+tasktime[7]-48;
		temps.Format("task%d",i+1);
		mytaskarray[i].typle=AfxGetApp()->GetProfileIntA("task",temps,0);
	}

}
UINT ThreadProc2(LPVOID pParam)//闹钟线程
{
	while(1)
	{
		pThread->SuspendThread();//挂起其它两个线程
		pThread3->SuspendThread();
		PlaySound(MAKEINTRESOURCE(IDR_WAVESOUND),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		Sleep(7000);
		PlaySound(MAKEINTRESOURCE(IDR_WAVESOUND),AfxGetApp()->m_hInstance,SND_RESOURCE|SND_ASYNC|SND_NODEFAULT);
		Sleep(8000);
		pThread->ResumeThread();//恢复其它线程
		pThread3->ResumeThread();
		pThread2->SuspendThread();
	}
	return 0;
}
UINT ThreadProc3(LPVOID pParam)//处理定时任务线程
{
	while(1)
	{
		switch(tasktyple)
		{
		case 1:
			{
				AfxMessageBox("请注意，系统即将关闭，请保存工作后点确定键！");
				system("shutdown -s -t 0");
			}break;
		case 2:
			{
				AfxMessageBox("请注意，系统即将重新启动，请保存工作后点确定键！");
				system("shutdown -r -t 0");
			}break;
		case 3:
			{
				CInputBox dlg;
				if(dlg.DoModal()==IDOK)
				{
					CString str;
					str="\"";
					str+=dlg.InputStr;
					str+="\"";
					char szMailAddress[80];
					strcpy(szMailAddress,str);
					ShellExecute(NULL,"open",szMailAddress,NULL,NULL,SW_SHOWNORMAL);
				}
			}break;
		case 4:
			{
				CInputBox dlg;
				if(dlg.DoModal()==IDOK)
				{
					CString str;
					str="\"";
					str+=dlg.InputStr;
					str+="\"";
					char szMailAddress[80];
					strcpy(szMailAddress,str);
					ShellExecute(NULL,"open",szMailAddress,NULL,NULL,SW_SHOWNORMAL);
				}

			}break;
		case 5:
			{
				AfxMessageBox("对不起，本模块仍在开发中，敬请期待！");
				//AfxMessageBox("打开程序！");
				//WinExec("notepad.exe",SW_SHOW); //运行计事本 
			}break;
		case 6:
			{
				//OSVERSIONINFO OsVersionInfo;
				//OsVersionInfo.dwPlatformId 
				system("dfrg.msc");			//XP下用
				system("defrag -c");		//vista下用
				system("dfrgui");			//
			}break;
		case 7:
			{
				mciSendString("Set cdAudio door open wait",NULL,0,NULL);
				AfxMessageBox("请注意，光驱已经打开，请插入光盘！装入光盘后点击确定将关闭光驱！");
				mciSendString("Set cdAudio door closed wait",NULL,0,NULL);
			}break;
		case 8:
			{
				AfxMessageBox("对不起，本模块仍在开发中，敬请期待！");
			}break;
		case 9:
			{
				system("shutdown -s -t 30");
			}break;
		case 10:
			{
				system("shutdown -r -t 30");
			}break;
		case 11:
			{
				AfxMessageBox("请注意，系统即将注销，请保存工作后点确定键！");
				system("logoff");
			}break;
		case 12:
			{
				SetWindowsHookEx(WH_KEYBOARD,keyproc,GetModuleHandle(NULL),0);//安装键盘钩子

				AfxMessageBox("键盘锁开启（只对XP系统有效），如需解锁，请关闭本程序或重新启动计算机！");

			}break;
		case 13:
			{
				RECT rect;
				rect.bottom=1;
				rect.right=1;

				ClipCursor(&rect);

				//AfxMessageBox("鼠标已锁，如需解锁，请反复点击鼠标！");

				//::Sleep(36000000);

				//ClipCursor(NULL);//释放

			}break;
		case 14:
			{
				HWND disk,mask;

				disk=FindWindow("ProgMan",NULL);

				mask=FindWindow("Shell_TrayWnd",NULL);

				ShowWindow(mask,SW_HIDE);//隐藏任务栏

				ShowWindow(disk,SW_HIDE);//隐藏桌面

				//AfxMessageBox("10分钟后会自动出现桌面.....");

				Sleep(360000000);

				ShowWindow(mask,SW_SHOW);//显示

				ShowWindow(disk,SW_SHOW);//显示


			}break;
		case 15:
			{
				//AfxMessageBox("关闭显示器");
				 SendMessage(FindWindow(0,0),WM_SYSCOMMAND,SC_MONITORPOWER,2);//关闭

				 //Sleep(600000);

				 //SendMessage(FindWindow(0,0),WM_SYSCOMMAND,SC_MONITORPOWER,-1);//打开
			}break;
		case 16:
			{
				Sleep(0);
				EnumWindows(EnumWindowsProc,0);//将窗口句柄传给回调函数处理

			}break;
		default:break;
		}
		pThread3->SuspendThread();
	}
	return 0;
}
LRESULT CALLBACK keyproc(int code,WPARAM wParam,LPARAM lParam)
{
	return 1;//返回1可使键盘停止响应
} 
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM IParam)//回调函数

{
	Sleep(0);

    ::PostMessage(hwnd,WM_CLOSE,0,0);//结束窗口

 return (true);//返回FALSE时EnumWindows结束

}

