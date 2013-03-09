// PE GuarderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "PE GuarderDlg.h"
#include "SystemController.h"
#include "Skin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPEGuarderDlg 对话框




CPEGuarderDlg::CPEGuarderDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(CPEGuarderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEGuarderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_MyTabCtrl);
}

BEGIN_MESSAGE_MAP(CPEGuarderDlg, CDialogSK)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MIN, &CPEGuarderDlg::OnBnClickedMin)
	ON_BN_CLICKED(IDC_TRAY, &CPEGuarderDlg::OnBnClickedTray)
	ON_COMMAND(ID_FILE_PEGUARDER, &CPEGuarderDlg::OnFilePeguarder)
	ON_COMMAND(ID_FILE_EXIT, &CPEGuarderDlg::OnFileExit)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_BN_CLICKED(IDC_SKIN, &CPEGuarderDlg::OnBnClickedSkin)
END_MESSAGE_MAP()


// CPEGuarderDlg 消息处理程序

BOOL CPEGuarderDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	// 创建圆角对话框
	CRect dialogRect; 
	GetClientRect(&dialogRect); 
	CRgn rgn; 
	rgn.CreateRoundRectRgn(0,0,dialogRect.Width(),dialogRect.Height(),20,20); 
	SetWindowRgn(rgn,TRUE); 

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CString iniFileName;  // ini文件路径变量
	GetModuleFileName(NULL, iniFileName.GetBuffer(MAX_PATH), MAX_PATH);  // 获得当前应用程序路径
	iniFileName.ReleaseBuffer();  // 一定要释放，否则无法拼接
	iniFileName = iniFileName.Left(iniFileName.ReverseFind('\\'));  // 去掉当前应用程序名
	iniFileName = iniFileName + "\\bin\\set.ini";  // 拼接成ini文件路径  
	m_iniFile.SetIniFileName(iniFileName);  // 设置ini文件路径

	CString skinPath;  // 皮肤路径
	m_iniFile.ReadIni("SET", "SKIN", skinPath);  // 读取ini文件中的皮肤路径

	// 如果文件路径不是有效的，则设置默认皮肤
	CFileFind fileFind;
	if(!fileFind.FindFile(skinPath))
	{
		skinPath = iniFileName;
		skinPath = skinPath.Left(skinPath.ReverseFind('\\'));  // 去掉配置文件名
		skinPath = skinPath + "\\bk1.bmp";
		SetBitmap(skinPath);  // 设置默认背景
	}
	else
	{
		SetBitmap(skinPath);  // 设置背景
	}
	fileFind.Close();  // 关闭文件查找，释放资源

	EnableEasyMove();  // 单击任何位置都能移动对话框
	SetStyle(LO_RESIZE);  // 设置对话框适应位图
	SetTransparentColor(RGB(0, 255, 0));  // 设置绿色作为透明色

	// 自定义选项卡加入所需标签名
	m_MyTabCtrl.InsertItem(0, "PE 文件分析器");
	m_MyTabCtrl.InsertItem(1, "PE 病毒检测器");
	m_MyTabCtrl.InsertItem(2, "工   具");

	// 创建分析器非模式化对话框
	CAnalyser* m_A = new CAnalyser();
	m_MyTabCtrl.m_pAnalyser = m_A;
	m_A->Create(IDD_ANALYSER, &m_MyTabCtrl);

	// 创建检测器非模式化对话框
	CDetector* m_D = new CDetector();
	m_MyTabCtrl.m_pDetector = m_D;
	m_D->Create(IDD_DETECTOR, &m_MyTabCtrl);

	// 创建工具非模式化对话框
	CTool* m_T = new CTool();
	m_MyTabCtrl.m_pTool = m_T;
	m_T->Create(IDD_TOOL, &m_MyTabCtrl);

	// 设置初始客户区标签矩形
	CRect tabRect;
	m_MyTabCtrl.GetClientRect(tabRect);
	tabRect.DeflateRect(1,20,2,2);
	m_D->ShowWindow(SW_HIDE);
	m_T->ShowWindow(SW_HIDE);
	m_A->MoveWindow(tabRect);
	m_A->ShowWindow(SW_SHOW);

	if(!CSystemController::SetReg("*\\shell\\PE Guarder\\Command"))
	{
		MessageBox("注册表设置错误！", "提示", MB_OK);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPEGuarderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogSK::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEGuarderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialogSK::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CPEGuarderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 最小化消息
//
void CPEGuarderDlg::OnBnClickedMin()
{
	// TODO: 在此添加控件通知处理程序代码

	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(0, 0));  // 发送最小化消息
}


// "缩小到托盘"按钮
//
void CPEGuarderDlg::OnBnClickedTray()
{
	// TODO: 在此添加控件通知处理程序代码

	m_TrayIcon.Create(this,WM_ICON_NOTIFY,"PE Guarder正在运行中",m_hIcon,IDR_MENU);  // 构造
	ShowWindow(SW_HIDE);  // 隐藏窗口
}


// 菜单"PE Guarder"
//
void CPEGuarderDlg::OnFilePeguarder()
{
	// TODO: 在此添加命令处理程序代码

	m_TrayIcon.RemoveIcon();
	ShowWindow(SW_SHOW);
}


// 菜单" 退出"
//
void CPEGuarderDlg::OnFileExit()
{
	// TODO: 在此添加命令处理程序代码

	m_TrayIcon.RemoveIcon();
	OnCancel();
}


// 托盘消息
//
LRESULT CPEGuarderDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam,lParam);
}


// "换肤"按钮
//
void CPEGuarderDlg::OnBnClickedSkin()
{
	// TODO: 在此添加控件通知处理程序代码

	// 打开皮肤对话框
	CSkin skin;
	skin.DoModal();
}


// 设置皮肤
//
void CPEGuarderDlg::SetSkin(CString skinPath)
{
	SetBitmap(skinPath);  // 设置皮肤(位图)

	// 写入ini文件皮肤路径
	m_iniFile.WriteIni("SET", "SKIN", skinPath);
}