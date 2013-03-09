// PE Guarder ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder Server.h"
#include "PE Guarder ServerDlg.h"

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


// CPEGuarderServerDlg 对话框




CPEGuarderServerDlg::CPEGuarderServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPEGuarderServerDlg::IDD, pParent)
	, m_FilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEGuarderServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_FilePath);
}

BEGIN_MESSAGE_MAP(CPEGuarderServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SELECTFILE, &CPEGuarderServerDlg::OnBnClickedSelectfile)
	ON_EN_CHANGE(IDC_FILEPATH, &CPEGuarderServerDlg::OnEnChangeFilepath)
	ON_BN_CLICKED(IDC_STARTUPDATE, &CPEGuarderServerDlg::OnBnClickedStartupdate)
	ON_MESSAGE(UM_UpdateState, OnUpdateState)
END_MESSAGE_MAP()


// CPEGuarderServerDlg 消息处理程序

BOOL CPEGuarderServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	CHAR hostName[128];  // 主机名变量   
	gethostname(hostName, 128);  // 获得主机名  
	hostent* pHost = gethostbyname(hostName);  // 获得主机结构  
	CString ip = inet_ntoa(*((in_addr*)pHost->h_addr));  // 获得本机IP
	SetDlgItemText(IDC_IP, ip);  // 显示本机IP

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPEGuarderServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEGuarderServerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CPEGuarderServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 改变对话框和控件背景色
//
HBRUSH CPEGuarderServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255,255,255));  // 设置控件背景色（白色）
		return m_brush;  // 返回自定义的画刷句柄
	}
	return hbr;  // 否则的话返回系统默认的画刷句柄
}


// 选择更新文件按钮事件
//
void CPEGuarderServerDlg::OnBnClickedSelectfile()
{
	// TODO: 在此添加控件通知处理程序代码

	// 创建打开文件对话框对象
	CFileDialog fileDialog(TRUE);
	// 如果选择取消按钮时
	if(fileDialog.DoModal() != IDOK)
	{
		return;
	}

	m_FilePath = fileDialog.GetPathName();  // 得到文件路径

	UpdateData(FALSE);  // 同步对话框
}


// 文件路径框变动事件
//
void CPEGuarderServerDlg::OnEnChangeFilepath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);  // 同步对话框
}


// 开始更新按钮事件
//
void CPEGuarderServerDlg::OnBnClickedStartupdate()
{
	// TODO: 在此添加控件通知处理程序代码

	UINT port = GetDlgItemInt(IDC_PORT);  // 得到端口号

	// 输入检查
	if(m_FilePath.Trim() == "")
	{
		MessageBox("路径不能为空和空字符！");
		return;
	}
	if(port < 1 || port > 65535)
	{
		MessageBox("端口不在有效的范围内！");
		return;
	}

	// 如果文件路径不是有效的
	CFileFind fileFind;
	if(!fileFind.FindFile(m_FilePath))
	{
		MessageBox("不是有效的文件路径！");
		fileFind.Close();  // 关闭文件查找，释放资源
		return;
	}
	fileFind.Close();  // 关闭文件查找，释放资源

	WIN32_FIND_DATA findData;  // 文件信息结构
	::FindClose(::FindFirstFile(m_FilePath, &findData));  // 找到文件并将信息存入上述结构

	PUpdateDBThreadParam pParam = new UpdateDBThreadParam();  // 更新线程参数结构

	strcpy_s(pParam->streamFileInfo.cFileName, findData.cFileName);  // 文件名
	pParam->streamFileInfo.dwFileSize = findData.nFileSizeLow;  // 文件大小

	pParam->strFilePath = m_FilePath;  // 文件路径
	pParam->uiPort = port;  // 端口
	pParam->hWnd = m_hWnd;  // 窗口句柄
	
	GetDlgItem(IDC_STARTUPDATE)->EnableWindow(FALSE);  // 禁用开始更新按钮

	m_pUpdateThread = AfxBeginThread(UpdateDBThread, (LPVOID)pParam);  // 开始更新线程
}


// 更新病毒库线程
//
UINT CPEGuarderServerDlg::UpdateDBThread(LPVOID pParam)
{
	// 参数传递
	SOCKET_STREAM_FILE_INFO streamFileInfo = ((PUpdateDBThreadParam)pParam)->streamFileInfo;
	CString filePath = ((PUpdateDBThreadParam)pParam)->strFilePath;
	UINT port = ((PUpdateDBThreadParam)pParam)->uiPort;
	HWND hWnd = ((PUpdateDBThreadParam)pParam)->hWnd;
	delete pParam;

	CSocket sockServ;  // 服务器套接字
	sockServ.Create(port);  // 端口设置
	sockServ.Listen();  // 开始监听

	CSocket sockRecv;  // 接收套接字	
	sockServ.Accept(sockRecv);  // 绑定接收套接字和服务器套接字

	// 先发一个通知信息给客户端，告知应该开辟多大的空间来接收文件
	sockRecv.Send(&streamFileInfo, sizeof(SOCKET_STREAM_FILE_INFO));  

	// 打开文件，只读和二进制
	CFile file;
	BOOL bOpen = file.Open(filePath, CFile::modeRead | CFile::typeBinary);  

	if(bOpen == TRUE)
	{
		DWORD dwRead = 0;  // 已经读取的数据量
		DWORD dwReal = 0;  // 每次真正读取的数据量

		// 读取所有文件数据并分段(1KB)发送
		while(dwRead < streamFileInfo.dwFileSize)
		{
			LPBYTE pSegment = new BYTE[1024];  // 开辟1KB的空间
			dwReal = file.Read(pSegment, 1024);  // 把文件1KB或小于1KB的数据放入上述空间，返回实际值
			sockRecv.Send(pSegment, dwReal);  // 发送此空间中的数据
			dwRead += dwReal;  // 已读数据增加
			delete pSegment;  // 清空空间
		}

		file.Close();  // 文件对象关闭
		sockRecv.Close();  // 接收套接字关闭
		sockServ.Close();  // 服务器套接字关闭
		::SendMessage(hWnd, UM_UpdateState, 1, NULL);  // 更新成功
	}
	else
	{
		sockRecv.Close();  // 接收套接字关闭
		sockServ.Close();  // 服务器套接字关闭
		::SendMessage(hWnd, UM_UpdateState, 0, NULL);  // 更新失败
	}

	return 0;
}


// 更新状态消息
//
LRESULT CPEGuarderServerDlg::OnUpdateState(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_STARTUPDATE)->EnableWindow(TRUE);  // 启用开始更新按钮
	
	if(wParam == 1)
	{
		MessageBox("更新成功！");
	}
	else
	{
		MessageBox("更新失败！");
	}

	return 0;
}