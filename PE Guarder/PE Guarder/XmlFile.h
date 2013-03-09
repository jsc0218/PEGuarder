#pragma once

#include "VirusDB.h"
#import <msxml3.dll>
using namespace MSXML2;

// 存放病毒特征码的xml文件类
class CXmlFile
{
public:
	CXmlFile(void);
	~CXmlFile(void);

	CString GetXmlFileName();  // 获得xml文件路径
	void SetXmlFileName(CString xmlFileName);  // 设置xml文件路径
	CString GetValue(CString nodeName);  // 得到指定结点名的值
	BOOL SetValue(CString nodeName, CString nodeValue);  // 修改指定节点值
	BOOL AppendForPE(CString sign1, CString sign2, CString sign3, CString sign4);  // 添加结点和结点值
	BOOL LoadForPE(list<PVRECORD>* pList);  // 加载病毒库
	PVSIGNATURE StrToSign(CString str);  // 字符串转特征码

private:
	CString m_XmlFileName;  // xml文件路径
};
