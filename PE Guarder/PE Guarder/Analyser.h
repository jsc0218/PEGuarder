#pragma once
#include "afxcmn.h"


// CAnalyser 对话框

class CAnalyser : public CDialog
{
	DECLARE_DYNAMIC(CAnalyser)

public:
	CAnalyser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalyser();

// 对话框数据
	enum { IDD = IDD_ANALYSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_ListCtrl1;  // 列表框变量
	IMAGE_DOS_HEADER dosHeader;  // DOS头结构
	IMAGE_NT_HEADERS ntHeader;    // PE头结构
	DWORD len;  // 记录文件长度
	BYTE *ptr;   // 用来保存文件
	BOOL bValid;   // 判断PE文件的标识

private:
	CBrush m_brush;  // 自定义画刷

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  
public:
	afx_msg void OnBnClickedLoadpe();  // 检查输入文件是否为PE文件
public:
	afx_msg void OnBnClickedDosheader();  // 读DOS头
public:
	afx_msg void OnBnClickedPeheader();  // 读PE头(ntHeader)
public:
	afx_msg void OnBnClickedSectiontable();  // 读块表
public:
	afx_msg void OnBnClickedDatadirectory();  // 读数据目录

};
