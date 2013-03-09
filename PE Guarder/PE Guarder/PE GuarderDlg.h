// PE GuarderDlg.h : 头文件
//

#pragma once

#include "CDialogSK.h"  // 引入皮肤对话框头文件
#include "afxcmn.h"
#include "MyTabCtrl.h"  // 引入自定义选项卡头文件
#include "TrayIcon.h"  // 引入托盘头文件
#include "IniFile.h"  // 引入初始化配置头文件

#define WM_ICON_NOTIFY WM_USER+1  // 自定义托盘消息


// CPEGuarderDlg 对话框
class CPEGuarderDlg : public CDialogSK
{
// 构造
public:
	CPEGuarderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PEGUARDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CMyTabCtrl m_MyTabCtrl;  // 自定义选项卡控件的变量
	CTrayIcon m_TrayIcon;  // 托盘变量
	CIniFile m_iniFile;  // 初始化配置文件变量

// 实现
protected:
	HICON m_hIcon;
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedMin();  // 最小化消息
public:
	afx_msg void OnBnClickedTray();  // "缩小到托盘"按钮
public:
	afx_msg void OnFilePeguarder();  // 菜单"PE Guarder"
public:
	afx_msg void OnFileExit();  // 菜单" 退出"
public:
	LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);  // 托盘消息
public:
	afx_msg void OnBnClickedSkin();  // "换肤"按钮
public:
	void SetSkin(CString skinPath);  // 设置皮肤
};
