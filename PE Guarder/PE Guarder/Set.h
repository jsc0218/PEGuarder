#pragma once

#include "SignatureAdder.h"
#include "DBUpdate.h"

// CSet 对话框

class CSet : public CDialog
{
	DECLARE_DYNAMIC(CSet)

public:
	CSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSet();

// 对话框数据
	enum { IDD = IDD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedAddsignature();  // 添加特征码按钮事件
public:
	afx_msg void OnBnClickedDbupdate();  // 病毒库更新按钮事件

public:
	CSignatureAdder* m_pSignatureAdder;  // 添加特征码对话框变量
	CDBUpdate* m_pDBUpdate;  // 病毒库更新对话框变量
private:
	CBrush m_brush;  // 自定义画刷
};
