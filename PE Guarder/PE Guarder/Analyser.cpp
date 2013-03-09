// Analyser.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "Analyser.h"


// CAnalyser 对话框

IMPLEMENT_DYNAMIC(CAnalyser, CDialog)

CAnalyser::CAnalyser(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalyser::IDD, pParent)
{

}

CAnalyser::~CAnalyser()
{
}

void CAnalyser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl1);
}


BEGIN_MESSAGE_MAP(CAnalyser, CDialog)
	ON_BN_CLICKED(IDC_LOADPE, &CAnalyser::OnBnClickedLoadpe)
	ON_BN_CLICKED(IDC_DOSHEADER, &CAnalyser::OnBnClickedDosheader)
	ON_BN_CLICKED(IDC_PEHEADER, &CAnalyser::OnBnClickedPeheader)
	ON_BN_CLICKED(IDC_SECTIONTABLE, &CAnalyser::OnBnClickedSectiontable)
	ON_BN_CLICKED(IDC_DATADIRECTORY, &CAnalyser::OnBnClickedDatadirectory)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAnalyser 消息处理程序


// 对话框初始化函数
//
BOOL CAnalyser::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

	len = 0;
	ptr = NULL;
	bValid = FALSE;

	// 列表框加入所需列
	m_ListCtrl1.InsertColumn(0,"属性",LVCFMT_LEFT,110);
	m_ListCtrl1.InsertColumn(1,"值",LVCFMT_LEFT,90);
	m_ListCtrl1.InsertColumn(2,"说明",LVCFMT_LEFT,150);

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 对话框颜色设为白色

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 改变对话框颜色
//
HBRUSH CAnalyser::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
	return m_brush;
}


// 检查输入文件是否为PE文件
//
void CAnalyser::OnBnClickedLoadpe()
{
	// TODO: 在此添加控件通知处理程序代码

	//m_ListCtrl1.DeleteAllItems();  // 清空列表框的数据

	// 创建打开文件对话框对象
	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All File(*.*)|*.*||",this);
	if(fileDialog.DoModal() != IDOK)
	{
   		return;
	}

	// 创建文件，并返回文件句柄
	HANDLE hFile = ::CreateFile(fileDialog.GetFileName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	// 检查返回的文件句柄是否有效
	if(hFile == INVALID_HANDLE_VALUE)         
	{
		MessageBox("无效文件",NULL,MB_OK);
		return;
	}

	// 依据返回的文件句柄得到文件的大小
	len = GetFileSize(hFile,NULL);
	// 依据文件大小开辟内存空间
	ptr = new BYTE[len];
	if(!ptr){
		MessageBox("内存不够",NULL,MB_OK);
		return;
	}
	// 读取全部文件放入ptr所指内存空间内
	::ReadFile(hFile,ptr,len,&len,NULL);


	// 将ptr所指的dosHeader大小的内容放入dosHeader地址块中
	memmove(&dosHeader,ptr,sizeof(dosHeader));

	// 通过头两个字节是否为"MZ"，检查DOS头是否有效
	if(dosHeader.e_magic == IMAGE_DOS_SIGNATURE)
	{
		// 移动文件指针到PE文件头的PE文件标志处
		::SetFilePointer(hFile,dosHeader.e_lfanew,NULL,FILE_BEGIN);
		// 依据文件句柄读取ntHeader部分到ntHeader地址块中
		::ReadFile(hFile,&ntHeader,sizeof(ntHeader),&len,NULL);

		// 检查PE文件头(NT映像头)的PE文件标志是否为"PE00"
		if(ntHeader.Signature == IMAGE_NT_SIGNATURE)
		{
			bValid = TRUE;   // TRUE代表是PE文件
		}
		else
		{
			bValid = FALSE;  // FALSE代表不是PE文件
		}

	}
	else
	{
		bValid = FALSE;  // FALSE代表不是PE文件
	}

	// 通过检查是否为PE文件后，做出判定结论
	if(bValid)
	{
		MessageBox("是PE文件",NULL,MB_OK);
	}
	else
	{
		MessageBox("不是PE文件",NULL,MB_OK);
	}

	::CloseHandle(hFile);  // 关闭文件句柄
	m_ListCtrl1.DeleteAllItems();  // 清空列表框的数据

	return;
}


// 读DOS头
//
void CAnalyser::OnBnClickedDosheader()
{
	// TODO: 在此添加控件通知处理程序代码

	// 如果进入程序从未打开过任何文件，则ptr为NULL，从而无法读取文件数据
	// 或者所读文件不是PE文件
	if(ptr == NULL || bValid == FALSE)
	{
	   return;
	}

	m_ListCtrl1.DeleteAllItems();  // 清空列表框的数据
	// 将ptr所指的dosHeader大小的内容放入dosHeader地址块中
	memmove(&dosHeader,ptr,sizeof(dosHeader));

	CString dosHeaderInf;  // 存储dosHeader信息

	/*--------以十六进制显示DOS头中有关结构信息--------*/

	m_ListCtrl1.InsertItem(0,"");
	dosHeaderInf.Format("%xh",dosHeader.e_ss);
	m_ListCtrl1.SetItemText(0,0,"e_ss");
	m_ListCtrl1.SetItemText(0,1,dosHeaderInf);

	m_ListCtrl1.InsertItem(1,"");
	dosHeaderInf.Format("%xh",dosHeader.e_sp);
	m_ListCtrl1.SetItemText(1,0,"e_sp");
	m_ListCtrl1.SetItemText(1,1,dosHeaderInf);

	m_ListCtrl1.InsertItem(2,"");
	dosHeaderInf.Format("%xh",dosHeader.e_ip);
	m_ListCtrl1.SetItemText(2,0,"e_ip");
	m_ListCtrl1.SetItemText(2,1,dosHeaderInf);

	m_ListCtrl1.InsertItem(3,"");
	dosHeaderInf.Format("%xh",dosHeader.e_cs);
	m_ListCtrl1.SetItemText(3,0,"e_cs");
	m_ListCtrl1.SetItemText(3,1,dosHeaderInf);

	m_ListCtrl1.InsertItem(4,"");
	dosHeaderInf.Format("%xh",dosHeader.e_lfanew);
	m_ListCtrl1.SetItemText(4,0,"e_lfanew");
	m_ListCtrl1.SetItemText(4,1,dosHeaderInf);
	m_ListCtrl1.SetItemText(4,2,"指向PE文件头起始位置");
}


// 读PE头(ntHeader)
//
void CAnalyser::OnBnClickedPeheader()
{
	// TODO: 在此添加控件通知处理程序代码

	// 如果进入程序从未打开过任何文件，则ptr为NULL，从而无法读取文件数据
	// 或者所读文件不是PE文件
	if(ptr == NULL || bValid == FALSE)
	{
	   return;
	}

	m_ListCtrl1.DeleteAllItems();  // 清空列表框的数据
	// 将ntHeader部分放入ntHeader变量地址块中
	memmove(&ntHeader,ptr+dosHeader.e_lfanew,sizeof(ntHeader));

	CString ntHeaderInf;  // 存储ntHeader信息

	/*------------------File头部分---------------------*/

	// 块数
	m_ListCtrl1.InsertItem(0,"");
	ntHeaderInf.Format("%xh",ntHeader.FileHeader.NumberOfSections);
	m_ListCtrl1.SetItemText(0,0,"NumberOfSections");
	m_ListCtrl1.SetItemText(0,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(0,2,"块数");

	// 文件创建的时间日期(从1970年1月1日00：00：00起的秒数)
	m_ListCtrl1.InsertItem(1,"");
	ntHeaderInf.Format("%xh",ntHeader.FileHeader.TimeDateStamp);
	m_ListCtrl1.SetItemText(1,0,"TimeDateStamp");
	m_ListCtrl1.SetItemText(1,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(1,2,"文件创建的时间日期(从1970年1月1日00：00：00起的秒数)");

	// 可选头大小(常值e0h)
	m_ListCtrl1.InsertItem(2,"");
	ntHeaderInf.Format("%xh",ntHeader.FileHeader.SizeOfOptionalHeader);
	m_ListCtrl1.SetItemText(2,0,"SizeOfOptionalHeader");
	m_ListCtrl1.SetItemText(2,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(2,2,"可选头大小(常值e0h)");


	// 文件信息标志，如dll、exe
	m_ListCtrl1.InsertItem(3,"");
	ntHeaderInf.Format("%xh",ntHeader.FileHeader.Characteristics);
	m_ListCtrl1.SetItemText(3,0,"Characteristics");
	m_ListCtrl1.SetItemText(3,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(3,2,"文件信息标志，如dll、exe");


	/*------------------NT可选头部分----------------*/

	// 标志字(常值10bh)
	m_ListCtrl1.InsertItem(4,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.Magic);
	m_ListCtrl1.SetItemText(4,0,"Magic");
	m_ListCtrl1.SetItemText(4,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(4,2,"标志字(常值10bh)");

	// 程序入口RVA地址
	m_ListCtrl1.InsertItem(5,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.AddressOfEntryPoint);
	m_ListCtrl1.SetItemText(5,0,"AddressOfEntryPoint");
	m_ListCtrl1.SetItemText(5,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(5,2,"程序入口RVA地址");


	// 代码块起始RVA
	m_ListCtrl1.InsertItem(6,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.BaseOfCode);
	m_ListCtrl1.SetItemText(6,0,"BaseOfCode");
	m_ListCtrl1.SetItemText(6,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(6,2,"代码块起始RVA");

	// 数据块起始RVA
	m_ListCtrl1.InsertItem(7,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.BaseOfData);
	m_ListCtrl1.SetItemText(7,0,"BaseOfData");
	m_ListCtrl1.SetItemText(7,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(7,2,"数据块起始RVA");

	// 基址
	m_ListCtrl1.InsertItem(8,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.ImageBase);
	m_ListCtrl1.SetItemText(8,0,"ImageBase");
	m_ListCtrl1.SetItemText(8,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(8,2,"基址");

	// 映像大小
	m_ListCtrl1.InsertItem(9,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.SizeOfImage);
	m_ListCtrl1.SetItemText(9,0,"SizeOfImage");
	m_ListCtrl1.SetItemText(9,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(9,2,"映像大小");

	// 块前头部大小
	m_ListCtrl1.InsertItem(10,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.SizeOfHeaders);
	m_ListCtrl1.SetItemText(10,0,"SizeOfHeaders");
	m_ListCtrl1.SetItemText(10,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(10,2,"块前头部大小");

	// 校验和
	m_ListCtrl1.InsertItem(11,"");
	ntHeaderInf.Format("%xh",ntHeader.OptionalHeader.CheckSum);
	m_ListCtrl1.SetItemText(11,0,"CheckSum");
	m_ListCtrl1.SetItemText(11,1,ntHeaderInf);
	m_ListCtrl1.SetItemText(11,2,"校验和");
}


// 读块表
//
void CAnalyser::OnBnClickedSectiontable()
{
	// TODO: 在此添加控件通知处理程序代码

	// 如果进入程序从未打开过任何文件，则ptr为NULL，从而无法读取文件数据
	// 或者所读文件不是PE文件
	if(ptr == NULL || bValid == FALSE)
	{
	   return;
	}

	m_ListCtrl1.DeleteAllItems();  // 清空列表框的数据
	// 将dosHeader部分放入dosHeader变量地址块中
	memmove(&dosHeader,ptr,sizeof(dosHeader));
	// 将ntHeader部分放入ntHeader变量地址块中
	memmove(&ntHeader,ptr+dosHeader.e_lfanew,sizeof(ntHeader));
	
	CString sectionHeaderInf;  // 存储sectionHeader信息
	IMAGE_SECTION_HEADER sectionHeader;  // 块头结构
	DWORD num = ntHeader.FileHeader.NumberOfSections;  // 块数

	// 输出所有块的重要内容
	for(DWORD i = 0;i < num;i ++)
	{
		// 把每一块的内容依次移到sectionHeader变量地址块中
		memmove(&sectionHeader,ptr+dosHeader.e_lfanew+sizeof(ntHeader)+i*sizeof(sectionHeader),sizeof(sectionHeader));
   
		// 块名
		m_ListCtrl1.InsertItem(0+7*i,"");
		sectionHeaderInf.Format("%s",sectionHeader.Name);
		m_ListCtrl1.SetItemText(0+7*i,0,"Name");
		m_ListCtrl1.SetItemText(0+7*i,1,sectionHeaderInf);
		m_ListCtrl1.SetItemText(0+7*i,2,"块名");

		// 内存中块大小
		m_ListCtrl1.InsertItem(1+7*i,"");
		sectionHeaderInf.Format("%xh",sectionHeader.Misc.VirtualSize);
		m_ListCtrl1.SetItemText(1+7*i,0,"VirtualSize");
		m_ListCtrl1.SetItemText(1+7*i,1,sectionHeaderInf);
		m_ListCtrl1.SetItemText(1+7*i,2,"内存中块大小");

		// 内存中块RVA值
		m_ListCtrl1.InsertItem(2+7*i,"");
		sectionHeaderInf.Format("%xh",sectionHeader.VirtualAddress);
		m_ListCtrl1.SetItemText(2+7*i,0,"VirtualAddress");
		m_ListCtrl1.SetItemText(2+7*i,1,sectionHeaderInf);
		m_ListCtrl1.SetItemText(2+7*i,2,"内存中块RVA值");

		// 文件中块大小
		m_ListCtrl1.InsertItem(3+7*i,"");
		sectionHeaderInf.Format("%xh",sectionHeader.SizeOfRawData);
		m_ListCtrl1.SetItemText(3+7*i,0,"SizeOfRawData");
		m_ListCtrl1.SetItemText(3+7*i,1,sectionHeaderInf);
		m_ListCtrl1.SetItemText(3+7*i,2,"文件中块大小");

		// 文件中块偏移
		m_ListCtrl1.InsertItem(4+7*i,"");
		sectionHeaderInf.Format("%xh",sectionHeader.PointerToRawData);
		m_ListCtrl1.SetItemText(4+7*i,0,"PointerToRawData");
		m_ListCtrl1.SetItemText(4+7*i,1,sectionHeaderInf);
		m_ListCtrl1.SetItemText(4+7*i,2,"文件中块偏移");

		// 块属性
		m_ListCtrl1.InsertItem(5+7*i,"");
		sectionHeaderInf.Format("%xh",sectionHeader.Characteristics);
		m_ListCtrl1.SetItemText(5+7*i,0,"Characteristics");
		m_ListCtrl1.SetItemText(5+7*i,1,sectionHeaderInf);
		m_ListCtrl1.SetItemText(5+7*i,2,"块属性");

		// 分割线
		m_ListCtrl1.InsertItem(6+7*i,"");
	}
}


// 读数据目录
//
void CAnalyser::OnBnClickedDatadirectory()
{
	// TODO: 在此添加控件通知处理程序代码

	// 如果进入程序从未打开过任何文件，则ptr为NULL，从而无法读取文件数据
	// 或者所读文件不是PE文件
	if(ptr == NULL || bValid == FALSE)
	{
	   return;
	}

	m_ListCtrl1.DeleteAllItems();  // 清空列表框的数据
	// 将dosHeader部分放入dosHeader变量地址块中
	memmove(&dosHeader,ptr,sizeof(dosHeader));
	// 将ntHeader部分放入ntHeader变量地址块中
	memmove(&ntHeader,ptr+dosHeader.e_lfanew,sizeof(ntHeader));

	CString dataDirectoryInf;  // 存储数据目录信息

	/*------------以十六进制显示dataDirectory中有关结构信息--------------*/

	// 输出表的RVA值及大小
	m_ListCtrl1.InsertItem(0,"");
	dataDirectoryInf.Format("%xh",ntHeader.OptionalHeader.DataDirectory[0].VirtualAddress);
	m_ListCtrl1.SetItemText(0,0,"VirtualAddress");
	m_ListCtrl1.SetItemText(0,1,dataDirectoryInf);
	m_ListCtrl1.SetItemText(0,2,"输出表的RVA值");

	// 输出表大小
	m_ListCtrl1.InsertItem(1,"");
	dataDirectoryInf.Format("%xh",ntHeader.OptionalHeader.DataDirectory[0].Size);
	m_ListCtrl1.SetItemText(1,0,"Size");
	m_ListCtrl1.SetItemText(1,1,dataDirectoryInf);
	m_ListCtrl1.SetItemText(1,2,"输出表大小");

	// 分割线
	m_ListCtrl1.InsertItem(2,"");

	// 输入表的RVA值及大小
	m_ListCtrl1.InsertItem(3,"");
	dataDirectoryInf.Format("%xh",ntHeader.OptionalHeader.DataDirectory[1].VirtualAddress);
	m_ListCtrl1.SetItemText(3,0,"VirtualAddress");
	m_ListCtrl1.SetItemText(3,1,dataDirectoryInf);
	m_ListCtrl1.SetItemText(3,2,"输入表的RVA值");

	// 输入表大小
	m_ListCtrl1.InsertItem(4,"");
	dataDirectoryInf.Format("%xh",ntHeader.OptionalHeader.DataDirectory[1].Size);
	m_ListCtrl1.SetItemText(4,0,"Size");
	m_ListCtrl1.SetItemText(4,1,dataDirectoryInf);
	m_ListCtrl1.SetItemText(4,2,"输入表大小");
}



