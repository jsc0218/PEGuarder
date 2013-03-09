// Tool.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "Tool.h"
#include "SystemController.h"

// CTool 对话框

IMPLEMENT_DYNAMIC(CTool, CDialog)

CTool::CTool(CWnd* pParent /*=NULL*/)
	: CDialog(CTool::IDD, pParent)
	, m_FilePath(_T(""))
{

}

CTool::~CTool()
{
}

void CTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_FilePath);
	DDX_Control(pDX, IDC_SHATTER, m_Shatter);
	DDX_Control(pDX, IDC_SHATTERPROGRESS, m_ShatterProgress);
}


BEGIN_MESSAGE_MAP(CTool, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BROWSE, &CTool::OnBnClickedBrowse)
	ON_EN_CHANGE(IDC_FILEPATH, &CTool::OnEnChangeFilepath)
	ON_BN_CLICKED(IDC_DELETE, &CTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SHATTER, &CTool::OnBnClickedShatter)
END_MESSAGE_MAP()


// CTool 消息处理程序


// 对话框初始化函数
//
BOOL CTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 改变对话框和控件背景色
//
HBRUSH CTool::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 浏览按钮消息
//
void CTool::OnBnClickedBrowse()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ShatterProgress.SetPos(0);  // 重置进度条
	SetDlgItemText(IDC_STATICDIGITAL,"");  // 进度说明文本框初始化

	// 创建打开文件对话框对象
	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"All File(*.*)|*.*||",this);
	// 创建模式化对话框，如果点击“取消”按钮，
	// 则不继续执行后续代码
	if (fileDialog.DoModal() != IDOK)
	{
		return;
	}

	m_FilePath = fileDialog.GetPathName();  // 获得文件路径
	
	// 文件路径控件显示当前文件路径
	UpdateData(FALSE);
	// 获得文件大小
	GetFileSize();
}


// 路径文本框变动消息
//
void CTool::OnEnChangeFilepath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	// 将文本编辑框的内容赋值到路径变量
	UpdateData(TRUE);
	GetFileSize();  // 获得文件大小

	m_ShatterProgress.SetPos(0);  // 重置进度条
	// 进度说明文本框重置
	SetDlgItemText(IDC_STATICDIGITAL,"");  
}


// 删除按钮消息
//
void CTool::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码

	// 如果文件路径不是有效的，则不做响应
	CFileFind fileFind;
	if(!fileFind.FindFile(m_FilePath))
	{
		MessageBox("不是有效的文件路径！", "提示", MB_OK);
		fileFind.Close();  // 关闭文件查找，释放资源
		return;
	}
	fileFind.Close();  // 关闭文件查找，释放资源

	// 设置文件属性(取消只读属性)
	if(!CSystemController::FileAttributesOperation(m_FilePath))
	{
		MessageBox("文件属性设置失败！", "提示", MB_OK);
		return;
	}

	// 重启系统，删除文件
	if(::MoveFileEx(m_FilePath,NULL,MOVEFILE_DELAY_UNTIL_REBOOT))
	{
		// 重启提示框
		if (MessageBox("要完成删除操作请先重启电脑，是否现在重启？", "提示", MB_YESNO) == IDNO)
		{
			MessageBox("请稍后重启电脑!", "提示", MB_OK);
		}
		else  // 重启系统
		{
			CSystemController::RebootSystem();
		}
	}
	else
	{
		MessageBox("删除文件失败", "提示", MB_OK);
	}
}


// 粉碎按钮消息
//
void CTool::OnBnClickedShatter()
{
	// TODO: 在此添加控件通知处理程序代码

	// 如果文件路径不是有效的，则不做响应
	CFileFind fileFind;
	if(!fileFind.FindFile(m_FilePath))
	{
		MessageBox("不是有效的文件路径！", "提示", MB_OK);
		fileFind.Close();  // 关闭文件查找，释放资源
		return;
	}
	fileFind.Close();  // 关闭文件查找，释放资源

	// 设置文件属性(取消只读属性)
	if(!CSystemController::FileAttributesOperation(m_FilePath))
	{
		MessageBox("文件属性设置失败！", "提示", MB_OK);
		return;
	}

	::EnableWindow(m_Shatter, FALSE);  // 粉碎按钮变灰色且不可用

	// 创建线程，并返回句柄
	HANDLE hThread = CreateThread(NULL, 0, MyShatterThread, this, 0, NULL);
	// 如果线程创建失败则返回
	if(hThread == NULL)
	{
		MessageBox("线程创建错误！","提示",MB_OK);
		return;
	}
}


// 获得指定文件大小
//
void CTool::GetFileSize()
{
	// 打开指定文件并返回相应句柄
	HANDLE hHandle = ::CreateFile(m_FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	// 通过返回的句柄得到文件大小
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		m_FileSize = 0;	 
	}
	else
	{
		m_FileSize = ::GetFileSize(hHandle, NULL); 
	}
	CloseHandle(hHandle);  // 关闭句柄 
}


// 自定义线程函数
//
DWORD WINAPI MyShatterThread(LPVOID lpParameter)
{
	CTool *pTool = (CTool *)lpParameter;  // 参数传递转换

	FILE *pWrite;  // 文件写指针
	// 以二进制可写方式打开文件,先将打开的文件内容清空，
	// 再往里写数据
	fopen_s(&pWrite, pTool->m_FilePath, "wb");
	// 设置文件指针位置为文件头
	fseek(pWrite, 0, SEEK_SET);

	// 如果文件指针创建失败，则返回0
	if(pWrite == NULL)
	{
		return 0;
	}

	DWORD cout = pTool->m_FileSize/100;  // 商，代表文件每百分之一的大小
	DWORD mod = pTool->m_FileSize%100;  // 余数
	// 设置进度条控件的范围
	pTool->m_ShatterProgress.SetRange(0,100);
	DWORD ByteWritten=0;  // 记录被修改的字节数
	CString strProgress;  // 用于显示进度的字符串

	// 显示进度
	for (DWORD j=1;j<=100;j++)
	{
		for (DWORD k=1;k<=cout;k++)
		{
			// 写入0
			fputc(0,pWrite);
			// 记录被修改的字节数
			ByteWritten++;
		}
		// 进度条显示进度
		pTool->m_ShatterProgress.SetPos(j);
		strProgress.Format("%d%s",j,"%");
		// 进度文本框显示进度
		pTool->SetDlgItemText(IDC_STATICDIGITAL,strProgress);
	}

	// 处理余下的字节
	for (DWORD y=1;y<=mod;y++)
	{
		// 写入0
		fputc(0,pWrite);
		// 记录被修改的字节数
		ByteWritten++;
	}

	fclose(pWrite);  // 关闭文件写指针

	if (ByteWritten == pTool->m_FileSize) 
	{
		// 使能粉碎按钮，恢复正常
		::EnableWindow(pTool->m_Shatter,TRUE);
	}

	return 1;
}