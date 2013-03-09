// SignatureAdder.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "SignatureAdder.h"


// CSignatureAdder 对话框

IMPLEMENT_DYNAMIC(CSignatureAdder, CDialog)

CSignatureAdder::CSignatureAdder(CWnd* pParent /*=NULL*/)
	: CDialog(CSignatureAdder::IDD, pParent)
	, m_Sign1(_T(""))
	, m_Sign2(_T(""))
	, m_Sign3(_T(""))
	, m_Sign4(_T(""))
{

}

CSignatureAdder::~CSignatureAdder()
{
}

void CSignatureAdder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIGNATURE1, m_Sign1);
	DDX_Text(pDX, IDC_SIGNATURE2, m_Sign2);
	DDX_Text(pDX, IDC_SIGNATURE3, m_Sign3);
	DDX_Text(pDX, IDC_SIGNATURE4, m_Sign4);
}


BEGIN_MESSAGE_MAP(CSignatureAdder, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SAVE, &CSignatureAdder::OnBnClickedSave)
	ON_EN_CHANGE(IDC_SIGNATURE1, &CSignatureAdder::OnEnChangeSignature1)
	ON_EN_CHANGE(IDC_SIGNATURE2, &CSignatureAdder::OnEnChangeSignature2)
	ON_EN_CHANGE(IDC_SIGNATURE3, &CSignatureAdder::OnEnChangeSignature3)
	ON_EN_CHANGE(IDC_SIGNATURE4, &CSignatureAdder::OnEnChangeSignature4)
	ON_BN_CLICKED(IDC_FORMAT, &CSignatureAdder::OnBnClickedFormat)
END_MESSAGE_MAP()


// CSignatureAdder 消息处理程序


// 对话框初始化函数
//
BOOL CSignatureAdder::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString xmlFileName;  // xml文件路径变量
	GetModuleFileName(NULL, xmlFileName.GetBuffer(MAX_PATH), MAX_PATH);  // 获得当前应用程序路径
	xmlFileName.ReleaseBuffer();  // 一定要释放，否则无法拼接
	xmlFileName = xmlFileName.Left(xmlFileName.ReverseFind('\\'));  // 去掉当前应用程序名
	xmlFileName = xmlFileName + "\\bin\\DB.xml";  // 拼接成xml文件路径

	m_XmlFile.SetXmlFileName(xmlFileName);  // 设置xml文件路径

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 改变对话框和控件背景色
//
HBRUSH CSignatureAdder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 保存按钮事件
//
void CSignatureAdder::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查输入格式
	if(m_Sign1.Trim() == "" && m_Sign2.Trim() == "" && m_Sign3.Trim()== "" && m_Sign4.Trim()== "")
	{
		MessageBox("输入框不能全为空或空字符！");

		// 输入框还原
		m_Sign1.Empty();
		m_Sign2.Empty();
		m_Sign3.Empty();
		m_Sign4.Empty();
		UpdateData(FALSE);
		
		return;
	}

	// 向xml中加入病毒数据
	if(m_XmlFile.AppendForPE(m_Sign1.Trim(), m_Sign2.Trim(), m_Sign3.Trim(), m_Sign4.Trim()) == TRUE)
	{
		MessageBox("保存成功！");
	}
	else
	{
		MessageBox("保存失败！");
	}

	// 输入框还原
	m_Sign1.Empty();
	m_Sign2.Empty();
	m_Sign3.Empty();
	m_Sign4.Empty();
	UpdateData(FALSE);
}


// 特征码1变动事件
//
void CSignatureAdder::OnEnChangeSignature1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
}


// 特征码2变动事件
//
void CSignatureAdder::OnEnChangeSignature2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
}


// 特征码3变动事件
//
void CSignatureAdder::OnEnChangeSignature3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
}


// 特征码4变动事件
//
void CSignatureAdder::OnEnChangeSignature4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
}


// 参考格式按钮事件
//
void CSignatureAdder::OnBnClickedFormat()
{
	// TODO: 在此添加控件通知处理程序代码

	MessageBox("例如：BS_STRUCT_OFFSET, 1, -1, 1, BL_NOT_EQUAL, 0x00,");
}
