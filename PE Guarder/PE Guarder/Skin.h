#pragma once
#include "afxwin.h"


// CSkin 对话框

class CSkin : public CDialog
{
	DECLARE_DYNAMIC(CSkin)

public:
	CSkin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSkin();

// 对话框数据
	enum { IDD = IDD_SKIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedSkinblack();  // 黑色皮肤按钮事件
public:
	afx_msg void OnBnClickedSkinblue();  // 蓝色皮肤按钮事件
public:
	afx_msg void OnBnClickedSkinorange();  // 橙色皮肤按钮事件

public:
	CButton m_SkinBlack;  // 黑色皮肤按钮
	CButton m_SkinBlue;  // 蓝色皮肤按钮
	CButton m_SkinOrange;  //橙色皮肤按钮
	CString m_skinPath;  // 皮肤文件路径

private:
	CBrush m_brush;  // 自定义画刷
};
