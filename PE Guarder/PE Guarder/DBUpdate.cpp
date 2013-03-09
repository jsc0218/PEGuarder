// DBUpdate.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "DBUpdate.h"

BOOL gThreadController;  // 线程控制者，全局变量


// CDBUpdate 对话框

IMPLEMENT_DYNAMIC(CDBUpdate, CDialog)

CDBUpdate::CDBUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDBUpdate::IDD, pParent)
{
}

CDBUpdate::~CDBUpdate()
{
	gThreadController = TRUE;  // 发挥作用，安全结束线程
}

void CDBUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBUpdate, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STARTUPDATE, &CDBUpdate::OnBnClickedStartupdate)
	ON_MESSAGE(UM_UpdateState, OnUpdateState)
	ON_BN_CLICKED(IDC_STOPUPDATE, &CDBUpdate::OnBnClickedStopupdate)
END_MESSAGE_MAP()


// CDBUpdate 消息处理程序


// 对话框初始化函数
//
BOOL CDBUpdate::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 改变对话框和控件背景色
//
HBRUSH CDBUpdate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 开始更新按钮事件
//
void CDBUpdate::OnBnClickedStartupdate()
{
	// TODO: 在此添加控件通知处理程序代码

	CString savePath;  // 文件保存路径变量
	GetModuleFileName(NULL, savePath.GetBuffer(MAX_PATH), MAX_PATH);  // 获得当前应用程序路径
	savePath.ReleaseBuffer();  // 释放缓存
	savePath = savePath.Left(savePath.ReverseFind('\\'));  // 去掉当前应用程序名
	savePath = savePath + "\\bin\\";  // 拼接成保存目录 

	CString ip;
	GetDlgItemText(IDC_IP, ip);  // 得到服务器IP地址
	UINT port = GetDlgItemInt(IDC_PORT);  // 得到服务器端口号

	PUpdateDBThreadParam pParam = new UpdateDBThreadParam;  // 更新线程参数结构
	pParam->strIP = ip;  // IP
	pParam->uiPort = port;  // 端口
	pParam->hWnd = m_hWnd;  // 窗口句柄
	pParam->strSavePath = savePath;  // 保存路径

	GetDlgItem(IDC_STARTUPDATE)->EnableWindow(FALSE);  // 禁用开始更新按钮

	gThreadController = FALSE;  // 不发挥作用
	m_pUpdateThread = AfxBeginThread(UpdateDBThread, (LPVOID)pParam);  // 开始更新线程
}


// 停止更新按钮事件
//
void CDBUpdate::OnBnClickedStopupdate()
{
	// TODO: 在此添加控件通知处理程序代码

	gThreadController = TRUE;  // 发挥作用

	GetDlgItem(IDC_STARTUPDATE)->EnableWindow(TRUE);  // 启用开始更新按钮
}


// 更新病毒库线程
//
UINT CDBUpdate::UpdateDBThread(LPVOID pParam)
{
	// 参数传递
	CString ip = ((PUpdateDBThreadParam)pParam)->strIP;
	UINT port = ((PUpdateDBThreadParam)pParam)->uiPort;
	HWND hWnd = ((PUpdateDBThreadParam)pParam)->hWnd;
	CString savePath = ((PUpdateDBThreadParam)pParam)->strSavePath;
	delete pParam;

	CSocket sockClient;  // 客户端套接字
	sockClient.Create();  // 端口由程序指定
	
	// 连接到指定套接字
	if(sockClient.Connect(ip, port) == FALSE)
	{
		::SendMessage(hWnd, UM_UpdateState, 0, NULL);  //  更新失败
		sockClient.Close();  // 客户端套接字关闭
		return 0;
	}

	SOCKET_STREAM_FILE_INFO streamFileInfo;  // 流文件信息结构
	sockClient.Receive(&streamFileInfo, sizeof(SOCKET_STREAM_FILE_INFO));  // 接收服务器发来的数据

	savePath = savePath + streamFileInfo.cFileName;  // 完整保存路径

	// 创建文件
	CFile file(savePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	DWORD dwRead = 0;  // 已经读取的数据量
	DWORD dwReal = 0;  // 每次真正读取的数据量

	// 接收所有文件数据并分段(1KB)写入文件
	while(dwRead < streamFileInfo.dwFileSize)
	{
		// 检测有否收到结束通知
		if(gThreadController == TRUE)
		{
			file.Close();  // 文件对象关闭
			sockClient.Close();  // 客户端套接字关闭
			return 0;
		}
		LPBYTE pSegment = new BYTE[1024];  // 开辟1KB的空间
		dwReal = sockClient.Receive(pSegment, 1024);  // 把文件1KB或小于1KB的数据放入上述空间，返回实际值
		file.Write(pSegment, dwReal);  // 数据写入文件 
		dwRead += dwReal;  // 已读数据增加
		delete pSegment;  // 清空空间
	}

	file.Close();  // 文件对象关闭
	sockClient.Close();  // 客户端套接字关闭

	::SendMessage(hWnd, UM_UpdateState, 1, NULL);  // 更新成功

	return 0;
}


// 更新状态消息
//
LRESULT CDBUpdate::OnUpdateState(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_STARTUPDATE)->EnableWindow(TRUE);  // 启用开始更新按钮

	if(wParam == 1)
	{
		MessageBox("更新成功！");
	}
	else
	{
		MessageBox("连接服务器失败，请稍后再试！");
	}

	return 0;
}

