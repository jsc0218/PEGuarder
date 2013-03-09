// Set.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "Set.h"


// CSet 对话框

IMPLEMENT_DYNAMIC(CSet, CDialog)

CSet::CSet(CWnd* pParent /*=NULL*/)
	: CDialog(CSet::IDD, pParent)
{

}

CSet::~CSet()
{
	// 删除指针所指内容
	delete m_pSignatureAdder;
	delete m_pDBUpdate;
}

void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSet, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ADDSIGNATURE, &CSet::OnBnClickedAddsignature)
	ON_BN_CLICKED(IDC_DBUPDATE, &CSet::OnBnClickedDbupdate)
END_MESSAGE_MAP()


// CSet 消息处理程序


// 对话框初始化函数
//
BOOL CSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 创建非模态添加特征码对话框
	m_pSignatureAdder = new CSignatureAdder();
	m_pSignatureAdder->Create(IDD_ADDSIGNATURE, this);

	// 创建非模态病毒库更新对话框
	m_pDBUpdate = new CDBUpdate();
	m_pDBUpdate->Create(IDD_DBUPDATE, this);

	m_pSignatureAdder->ShowWindow(SW_SHOW);  // 显示添加特征码对话框
	m_pDBUpdate->ShowWindow(SW_HIDE);  // 隐藏病毒库更新对话框

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 改变对话框和控件背景色
//
HBRUSH CSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 添加特征码按钮事件
//
void CSet::OnBnClickedAddsignature()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pSignatureAdder->ShowWindow(SW_SHOW);  // 显示添加特征码对话框
	m_pDBUpdate->ShowWindow(SW_HIDE);  // 隐藏病毒库更新对话框
}


// 病毒库更新按钮事件
//
void CSet::OnBnClickedDbupdate()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pDBUpdate->ShowWindow(SW_SHOW);  // 显示病毒库更新对话框
	m_pSignatureAdder->ShowWindow(SW_HIDE);  // 隐藏添加特征码对话框
}
