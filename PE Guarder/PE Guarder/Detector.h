#pragma once
#include "foldertreectrl.h"
#include "afxcmn.h"
#include "Engine.h"

#define UM_DisplayState WM_USER+2  // 显示扫描状态的消息


// 扫描线程参数结构
typedef struct _ScanThreadParam
{
	HWND hWnd;  // 窗口句柄
	CStringList *pListDir;  // 字符串列表指针
}ScanThreadParam, *LPScanThreadParam;


// 扫描消息
typedef struct _ScanMessage
{
	CString fileName;  // 文件名(完整路径)
	BOOL bDisposed;  // 处理成功与否
}ScanMessage, *LPScanMessage;


// CDetector 对话框

class CDetector : public CDialog
{
	DECLARE_DYNAMIC(CDetector)

public:
	CDetector(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetector();

// 对话框数据
	enum { IDD = IDD_DETECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CFolderTreeCtrl m_FolderTreeCtrl;  // 文件夹树变量
	CListCtrl m_ListCtrl2;  // 列表框变量
	int m_FileNum;  // 扫描文件数
	int m_VirusNum;  // 发现病毒数
	CString m_Time;  // 显示时间
	CString m_FileName;  // 显示正在扫描文件名称
	DWORD m_Seconds;  // 扫描时间秒数
	CWinThread* m_pThreadScan;  // 扫描线程 

private:
	CBrush m_brush;  // 自定义画刷

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	void ThreadScanBegin();  // 开始扫描线程
public:	
	void ThreadScanExit();  // 终止扫描线程
public:	
	void ThreadScanSuspend();  // 挂起扫描线程
public:	
	void ThreadScanResume();  // 重新启动扫描线程
public:
	static UINT ScanThread(LPVOID pParam);  // 扫描文件线程函数
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);  // 计时器函数
public:
	afx_msg void OnBnClickedBeginandexit();  // "开始\停止"按钮
public:
	afx_msg void OnBnClickedSuspendandresume();  // "挂起\重启"按钮
public:
	// 用于显示当前扫描到的文件信息
	afx_msg LRESULT OnDisplayState(WPARAM wParam, LPARAM lParam);  
public:
	afx_msg void OnBnClickedSet();  // "设置"按钮
};
