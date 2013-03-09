// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
	// 指针初始化
	m_pAnalyser = NULL;
	m_pDetector = NULL;
	m_pTool = NULL;
}

CMyTabCtrl::~CMyTabCtrl()
{
	// 清空指针所指内存单元
	delete m_pAnalyser;
	delete m_pDetector;
	delete m_pTool;
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序



void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	// 设置自定义选项卡切换时客户区矩形设置
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(1,20,2,2);

	// 光标设置，即选项卡号
	int cursel=this->GetCurSel();
	switch(cursel)
	{
	case 0:
		{
		m_pDetector->ShowWindow(SW_HIDE);
		m_pTool->ShowWindow(SW_HIDE);
		m_pAnalyser->MoveWindow(rect);
		m_pAnalyser->ShowWindow(SW_SHOW);
		break;
		}
	case 1:
		{
		m_pAnalyser->ShowWindow(SW_HIDE);
		m_pTool->ShowWindow(SW_HIDE);
		m_pDetector->MoveWindow(rect);
		m_pDetector->ShowWindow(SW_SHOW);
		break;
		}
	case 2:
		{
		m_pAnalyser->ShowWindow(SW_HIDE);
		m_pDetector->ShowWindow(SW_HIDE);
		m_pTool->MoveWindow(rect);
		m_pTool->ShowWindow(SW_SHOW);
		break;
		}
	}
	*pResult = 0;
}
