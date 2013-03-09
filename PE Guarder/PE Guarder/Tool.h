#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTool 对话框

class CTool : public CDialog
{
	DECLARE_DYNAMIC(CTool)

public:
	CTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTool();

// 对话框数据
	enum { IDD = IDD_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CString m_FilePath;  // 文件路径
	DWORD m_FileSize;  // 文件大小
	CButton m_Shatter;  // 粉碎按钮变量
	CProgressCtrl m_ShatterProgress;  // 进度条控件变量

private:
	CBrush m_brush;  // 自定义画刷

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedBrowse();  // 浏览按钮消息
public:
	afx_msg void OnEnChangeFilepath();  // 路径文本框变动消息
public:
	afx_msg void OnBnClickedDelete();  // 删除按钮消息
public:
	afx_msg void OnBnClickedShatter();  // 粉碎按钮消息
public:
	void GetFileSize();  // 获得指定文件大小
};

DWORD WINAPI MyShatterThread(LPVOID lpParameter);  // 自定义线程函数