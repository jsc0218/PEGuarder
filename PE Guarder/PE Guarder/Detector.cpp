// Detector.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "Detector.h"
#include "VirusInfo.h"
#include "Set.h"

// CDetector 对话框

IMPLEMENT_DYNAMIC(CDetector, CDialog)

CDetector::CDetector(CWnd* pParent /*=NULL*/)
	: CDialog(CDetector::IDD, pParent)
	, m_FileNum(0)
	, m_VirusNum(0)
	, m_Time(_T(""))
	, m_FileName(_T(""))
{
	m_Seconds = 0;  //  数据初始化
}

CDetector::~CDetector()
{
	
}

void CDetector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_FolderTreeCtrl);
	DDX_Text(pDX, IDC_FILENUM, m_FileNum);
	DDX_Text(pDX, IDC_VIRUSNUM, m_VirusNum);
	DDX_Text(pDX, IDC_TIME, m_Time);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
}


BEGIN_MESSAGE_MAP(CDetector, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BeginAndExit, &CDetector::OnBnClickedBeginandexit)
	ON_BN_CLICKED(IDC_SuspendAndResume, &CDetector::OnBnClickedSuspendandresume)
	ON_MESSAGE(UM_DisplayState, OnDisplayState)
	ON_BN_CLICKED(IDC_SET, &CDetector::OnBnClickedSet)
END_MESSAGE_MAP()


// CDetector 消息处理程序


// 对话框初始化函数
//
BOOL CDetector::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

	m_FolderTreeCtrl.DisplayTree();  // 显示文件夹树
	GetDlgItem(IDC_SuspendAndResume)->EnableWindow(FALSE);  // 禁用"暂停/恢复"按钮

	// 列表框加入所需列
	m_ListCtrl2.InsertColumn(0,"文件名",LVCFMT_LEFT,200);
	m_ListCtrl2.InsertColumn(1,"威胁名称",LVCFMT_LEFT,90);
	m_ListCtrl2.InsertColumn(2,"状态",LVCFMT_LEFT,75);

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 改变对话框和控件背景色
//
HBRUSH CDetector::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 计时器函数
//
void CDetector::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_Seconds++;  // 增加一秒

	CString hours;
	hours.Format("%d",(m_Seconds/3600));
	if(hours.GetLength() == 1)
	{
		hours.Insert(0, "0");  // 显示为两位
	}

	CString minutes;
	minutes.Format("%d", ((m_Seconds%3600)/60));
	if(minutes.GetLength() == 1)
	{
		minutes.Insert(0, "0");  // 显示为两位
	}

	CString seconds;
	seconds.Format("%d", (m_Seconds%60));
	if(seconds.GetLength() == 1)
	{
		seconds.Insert(0, "0");  // 显示为两位
	}

	// 显示时间格式设置
	m_Time = hours + "小时" + minutes + "分" + seconds + "秒";

	UpdateData(FALSE);  // 将成员数据显示到对应的窗口中

	CDialog::OnTimer(nIDEvent);
}


// "开始\停止"按钮
//
void CDetector::OnBnClickedBeginandexit()
{
	// TODO: 在此添加控件通知处理程序代码

	CString buttonText;
	GetDlgItemText(IDC_BeginAndExit, buttonText);  // 获得按钮的文本内容  
	if(buttonText == "开始扫描")
	{
		ThreadScanBegin();  // 开始扫描线程
	}
	else                    
	{
		ThreadScanExit();  // 停止扫描线程
	}
}


// "挂起\重启"按钮
//
void CDetector::OnBnClickedSuspendandresume()
{
	// TODO: 在此添加控件通知处理程序代码

	CString buttonText;
	GetDlgItemText(IDC_SuspendAndResume, buttonText);  // 获得按钮的文本内容
	if(buttonText == "暂停扫描")
	{
		ThreadScanSuspend();  // 暂停扫描线程
	}
	else  
	{
		ThreadScanResume();  // 恢复扫描线程
	}   
}


// 用于显示当前扫描到的文件信息
// 
LRESULT CDetector::OnDisplayState(WPARAM wParam, LPARAM lParam)
{
	PCHAR szScanCode = (PCHAR)wParam;
	int nCode = atoi(szScanCode);  // 字符串转为整数
	if(nCode == -1)  // 扫描完毕            
	{
		SetDlgItemText(IDC_BeginAndExit, "开始扫描");  // 按钮显示为开始扫描       
		GetDlgItem(IDC_TREE)->EnableWindow(TRUE);  // 启用TreeCtrl
		GetDlgItem(IDC_SuspendAndResume)->EnableWindow(FALSE);  // 禁用"暂停/恢复"按钮
		KillTimer(1);  // 关闭计时器
		MessageBox("扫描完毕!!!", "提示", MB_OK);
	}
	else 
	{
		LPScanMessage pScanMessage = (LPScanMessage)lParam;
		m_FileName = pScanMessage->fileName;  // 文件名(完整路径)

		m_FileNum++;  // 扫描文件数递加
		
		if(nCode > 0)  // 发现病毒
		{
			m_VirusNum++;  // 病毒数递增
			
			// 列表框
			m_ListCtrl2.InsertItem(0, "");  // 插入一行
			m_ListCtrl2.SetItemText(0, 0, m_FileName);  // 文件名
			m_ListCtrl2.SetItemText(0, 1, CVirusInfo::GetNameByID(nCode));  // 威胁名称
			// 处理成功与否
			if(pScanMessage->bDisposed == TRUE)
			{
				m_ListCtrl2.SetItemText(0, 2, "处理成功");  // 状态
			}
			else
			{
				m_ListCtrl2.SetItemText(0, 2, "处理失败");  // 状态
			}
		}
		else if(nCode == -2)  // 表示未知威胁
		{
			// 列表框
			m_ListCtrl2.InsertItem(0, "");  // 插入一行
			m_ListCtrl2.SetItemText(0, 0, m_FileName);  // 文件名
			m_ListCtrl2.SetItemText(0, 1, CVirusInfo::GetNameByID(nCode));  // 威胁名称
			m_ListCtrl2.SetItemText(0, 2, "未处理");  // 状态
		}

		UpdateData(FALSE);  // 显示更新数据
		delete pScanMessage;  // 清除指针所指对象
	}

	delete szScanCode;  // 清除指针所指对象
	return 0;
}


// 开始扫描线程
//
void CDetector::ThreadScanBegin()
{
	// 取得被选择的路径
	CStringList *pListDir = new CStringList();
	m_FolderTreeCtrl.GetSelectedPath(*pListDir); 

	// 如果字符串列表里没有文件夹，返回
	if(pListDir->GetCount() == 0)
	{
		MessageBox("请选择扫描文件夹!!!", "扫描文件不存在", MB_OK);
		return;
	}

	CString showPath = "";
	// 遍历字符串列表
	POSITION pos = pListDir->GetHeadPosition();  // 获取字符串列表的头元素  
	while(pos != NULL)
	{
		showPath += pListDir->GetNext(pos);
		showPath += "\n";
	}

	// 弹出确认框，"确定"继续，"取消"返回
	if (MessageBox(showPath, "扫描路径", MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	// 数据初始化
	m_FileNum = 0;
	m_VirusNum = 0;
	m_Seconds = 0;
	m_Time = "";
	m_ListCtrl2.DeleteAllItems();

	UpdateData(FALSE);  // 将成员数据显示到对应的窗口中
	SetTimer(1, 1000, NULL);  // 计时器按秒计

	SetDlgItemText(IDC_BeginAndExit, "停止扫描");  //开始扫描按钮变停止扫描
	GetDlgItem(IDC_TREE)->EnableWindow(FALSE);  // 禁用TreeCtrl
	GetDlgItem(IDC_SuspendAndResume)->EnableWindow(TRUE);  // 启用"暂停/恢复"按钮

	LPScanThreadParam pParam = new ScanThreadParam();  // 扫描线程参数结构
	pParam->hWnd = m_hWnd;  // 指定本窗口句柄
	pParam->pListDir = pListDir;  // 指定字符串列表指针
	m_pThreadScan = AfxBeginThread(ScanThread, (LPVOID)pParam);  // 创建工作线程  
}


// 终止扫描线程
//
void CDetector::ThreadScanExit()
{
	if(m_pThreadScan != NULL)
	{
		ThreadScanSuspend();  // 先挂起扫描线程

		if(MessageBox("扫描尚未完成,确定停止吗?", "停止扫描", MB_OKCANCEL) == IDOK)
		{
			DWORD dwExitCode;                       
			GetExitCodeThread(m_pThreadScan->m_hThread, &dwExitCode);  // 得到线程结束码
			TerminateThread(m_pThreadScan->m_hThread, dwExitCode);  // 终止线程
			CloseHandle(m_pThreadScan->m_hThread);  // 删除线程 

			SetDlgItemText(IDC_BeginAndExit, "开始扫描");  // 停止扫描按钮变开始扫描
			SetDlgItemText(IDC_SuspendAndResume, "暂停扫描");  //  "暂停\恢复"按钮显示暂停扫描
			GetDlgItem(IDC_SuspendAndResume)->EnableWindow(FALSE);  // 使"暂停\恢复"按钮不可按
			GetDlgItem(IDC_TREE)->EnableWindow(TRUE);  // 使能树控件
			return;
		}
		else
		{
			ThreadScanResume();  // 重新启动扫描线程
		}
	}
}


// 挂起扫描线程
//
void CDetector::ThreadScanSuspend()
{
	m_pThreadScan->SuspendThread();  // 挂起扫描线程
	SetDlgItemText(IDC_SuspendAndResume, "恢复扫描");  //  "暂停\恢复"按钮显示恢复扫描
	KillTimer(1);  // 停止计时器
}


// 重新启动扫描线程
//
void CDetector::ThreadScanResume()
{ 
	m_pThreadScan->ResumeThread();  // 重新启动扫描线程

	SetDlgItemText(IDC_SuspendAndResume, "暂停扫描");  //  "暂停\恢复"按钮显示暂停扫描
	SetTimer(1, 1000, NULL);  // 启动计时器
}


// 扫描文件线程函数
//
UINT CDetector::ScanThread(LPVOID pParam)
{
	// 病毒库加载
	CVirusDB VDB;
	VDB.Load();

	// 病毒引擎装载病毒库
	CEngine engine;
	engine.Load(&VDB);

	BOOL bDisposed;  // 判断病毒是否被处理成功

	HWND hWnd = ((LPScanThreadParam)pParam)->hWnd;  // 窗口句柄
	CStringList *pListDir = ((LPScanThreadParam)pParam)->pListDir;  // 字符串列表指针
	delete pParam;  // 删除此指针所指对象

	while(pListDir->GetCount() > 0)  // 字符串列表不为空             
	{
		POSITION pos = pListDir->GetHeadPosition();  // 从第一个结点开始搜索
		CString& fileName = pListDir->GetAt(pos);  // 获得字符串列表中指定位置的值

		// 设置成x:\xx\*.*格式
		if(fileName[fileName.GetLength()-1] != '\\')
		{
			fileName += "\\";
		}
		fileName += "*.*";

		WIN32_FIND_DATA findFileData;  // 此结构用于表示找到的文件或目录，包含其基本信息
		HANDLE hFindFile = ::FindFirstFile(fileName, &findFileData);  // 查找第一个目录或文件，获取查找句柄
		if(hFindFile != INVALID_HANDLE_VALUE)  // 判断查找句柄有效性
		{
			do  // 同层遍历 
			{
				// 不处理 '.' \ '..' (表示当前目录\上层目录)，cFileName不是完整路径
				if(findFileData.cFileName[0] == '.')
				{
					continue;
				}

				// 若为目录,则插入列表前面
				if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					CString dirName;  
					dirName.Format("%s", findFileData.cFileName);  // 格式化目录名
					dirName.Insert(0, fileName.Left(fileName.GetLength()-3));  // 拼凑完整路径

					pListDir->InsertAfter(pos, dirName);  // 目录插入列表
				}
				// 若为文件，在这里进行病毒分析
				else  
				{
					CString filePath;  
					filePath.Format("%s", findFileData.cFileName);  // 格式化文件名
					filePath.Insert(0, fileName.Left(fileName.GetLength()-3));  // 拼凑完整路径 
					PCHAR szScanCode = new CHAR[32];

					engine.Scan(filePath);  // 扫描一个文件
					int VID = engine.GetScanRecord().dwVirusID;  // 返回关于这个文件的记录
					
					// 发现病毒，就处理
					if(VID > 0)
					{
						if(engine.Dispose(filePath) == TRUE)
						{
							bDisposed = TRUE;  // 处理成功
						}
						else
						{
							bDisposed = FALSE;  // 处理失败
						}
					}
					else
					{
						bDisposed = TRUE;  // 不是病毒，默认处理成功
					}

					// 扫描消息
					LPScanMessage pScanMessage = new ScanMessage;  
					pScanMessage->fileName = filePath;  // 文件名(完整路径)
					pScanMessage->bDisposed = bDisposed;  // 处理成功与否

					// wParam为0，表示没有病毒，wParam>0，表示有病毒且为病毒ID，wParam=-2，表示存在未知威胁，
					_itoa_s(VID, szScanCode, 32, 10);  // 整数转化为字符串 
					::SendMessage(hWnd, UM_DisplayState, (WPARAM)szScanCode, (LPARAM)pScanMessage);  // 发送显示消息
				}
			}while(::FindNextFile(hFindFile, &findFileData));
		}

		::FindClose(hFindFile);  // 关闭FindFirstFile创建的搜索句柄
		pListDir->RemoveHead();  // 此目录搜索完毕,删除
	}

	pListDir->RemoveAll();  // 清空字符串列表
	delete pListDir;  // 删除字符串列表对象

	engine.Release();  // 释放扫描记录
	VDB.Unload();  // 卸载病毒库

	PCHAR szScanCode = new CHAR[32];   // 通知主线程结束扫描
	_itoa_s(-1, szScanCode, 32, 10);  // wParam为-1, 表示扫描结束，整数转化为字符串
	::SendMessage(hWnd, UM_DisplayState, (WPARAM)szScanCode, NULL);  // 发送显示消息，完成扫描

	return 0;
}


// "设置"按钮
//
void CDetector::OnBnClickedSet()
{
	// TODO: 在此添加控件通知处理程序代码

	// 创建模态对话框
	CSet set;
	set.DoModal();  
}
