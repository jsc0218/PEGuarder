// Skin.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "Skin.h"
#include "PE GuarderDlg.h"

// CSkin 对话框

IMPLEMENT_DYNAMIC(CSkin, CDialog)

CSkin::CSkin(CWnd* pParent /*=NULL*/)
	: CDialog(CSkin::IDD, pParent)
{

}

CSkin::~CSkin()
{
}

void CSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKINBLACK, m_SkinBlack);
	DDX_Control(pDX, IDC_SKINBLUE, m_SkinBlue);
	DDX_Control(pDX, IDC_SKINORANGE, m_SkinOrange);
}


BEGIN_MESSAGE_MAP(CSkin, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SKINBLACK, &CSkin::OnBnClickedSkinblack)
	ON_BN_CLICKED(IDC_SKINBLUE, &CSkin::OnBnClickedSkinblue)
	ON_BN_CLICKED(IDC_SKINORANGE, &CSkin::OnBnClickedSkinorange)
END_MESSAGE_MAP()


// CSkin 消息处理程序


// 对话框初始化函数
//
BOOL CSkin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 按钮添加位图
	if(m_SkinBlack.GetBitmap() == NULL)
	{
		m_SkinBlack.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SKINBLACK)));
	}

	// 按钮添加位图
	if(m_SkinBlue.GetBitmap() == NULL)
	{
		m_SkinBlue.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SKINBLUE)));
	}

	// 按钮添加位图
	if(m_SkinOrange.GetBitmap() == NULL)
	{
		m_SkinOrange.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SKINORANGE)));
	}

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	GetModuleFileName(NULL, m_skinPath.GetBuffer(MAX_PATH), MAX_PATH);  // 获得当前应用程序路径
	m_skinPath.ReleaseBuffer();  // 一定要释放，否则无法拼接
	m_skinPath = m_skinPath.Left(m_skinPath.ReverseFind('\\'));  // 去掉当前应用程序名
	m_skinPath = m_skinPath + "\\bin\\";  // 皮肤目录设置  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 改变对话框和控件背景色
//
HBRUSH CSkin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 黑色皮肤按钮事件
//
void CSkin::OnBnClickedSkinblack()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_skinPath += "bk1.bmp";  // 皮肤目录
	((CPEGuarderDlg*)GetParent())->SetSkin(m_skinPath);  // 设置皮肤
	
	OnCancel();  // 取消对话框
}


// 蓝色皮肤按钮事件
//
void CSkin::OnBnClickedSkinblue()
{
	// TODO: 在此添加控件通知处理程序代码

	m_skinPath += "bk2.bmp";  // 皮肤目录
	((CPEGuarderDlg*)GetParent())->SetSkin(m_skinPath);  // 设置皮肤
	
	OnCancel();  // 取消对话框
}


// 橙色皮肤按钮事件
//
void CSkin::OnBnClickedSkinorange()
{
	// TODO: 在此添加控件通知处理程序代码

	m_skinPath += "bk3.bmp";  // 皮肤目录
	((CPEGuarderDlg*)GetParent())->SetSkin(m_skinPath);  // 设置皮肤
	
	OnCancel();  // 取消对话框
}
