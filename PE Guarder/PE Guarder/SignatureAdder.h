#pragma once

#include "XmlFile.h"  // 引入xml操作类


// CSignatureAdder 对话框

class CSignatureAdder : public CDialog
{
	DECLARE_DYNAMIC(CSignatureAdder)

public:
	CSignatureAdder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSignatureAdder();

// 对话框数据
	enum { IDD = IDD_ADDSIGNATURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedSave();  // 保存按钮事件
public:
	afx_msg void OnEnChangeSignature1();  // 特征码1变动事件
public:
	afx_msg void OnEnChangeSignature2();  // 特征码2变动事件
public:
	afx_msg void OnEnChangeSignature3();  // 特征码3变动事件
public:
	afx_msg void OnEnChangeSignature4();  // 特征码4变动事件
public:
	afx_msg void OnBnClickedFormat();  // 参考格式按钮事件

public:
	CString m_Sign1;  //特征码1变量
	CString m_Sign2;  //特征码2变量
	CString m_Sign3;  //特征码3变量
	CString m_Sign4;  //特征码4变量
	CXmlFile m_XmlFile;  // xml文件变量
private:
	CBrush m_brush;  // 自定义画刷
};
