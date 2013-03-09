#pragma once

// 引进分析器、检测器、工具头文件
#include "Analyser.h"
#include "Detector.h"
#include "Tool.h"

// CMyTabCtrl
// 定义自己的选项卡控件
class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// 创建分析器、检测器工具指针
	CAnalyser* m_pAnalyser;
	CDetector* m_pDetector;
	CTool* m_pTool;

public:
	// 选项卡选择切换消息
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


