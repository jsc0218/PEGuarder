#include "StdAfx.h"
#include "XmlFile.h"



CXmlFile::CXmlFile(void)
{
}

CXmlFile::~CXmlFile(void)
{
}


// 获得xml文件路径
//
CString CXmlFile::GetXmlFileName()
{
	return m_XmlFileName;
}


// 设置xml文件路径
//
void CXmlFile::SetXmlFileName(CString xmlFileName)
{
	m_XmlFileName = xmlFileName;
}


// 得到指定结点名的值
//
CString CXmlFile::GetValue(CString nodeName)
{
	CString nodeValue;  // 用来保存结点值
	CoInitialize(NULL);  // 初始化com

	// 创建文档对象
	MSXML2::IXMLDOMDocumentPtr pDoc;  
	HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument30));
	if(hr < 0)
	{     
		return NULL;
	}   
	pDoc->load((const _variant_t)m_XmlFileName);  // 加载指定xml
	
	MSXML2::IXMLDOMElementPtr pRoot=pDoc->GetdocumentElement();  // 得到根结点指针
	MSXML2::IXMLDOMElementPtr pTarget=pRoot->selectSingleNode((_bstr_t)nodeName);  // 得到目标结点指针        
	nodeValue = _com_util::ConvertBSTRToString(pTarget->text);  // 得到目标结点值  
	
	pTarget.Release();  // 目标结点释放
	pRoot.Release();  // 根结点释放
	pDoc.Release();  // 文档对象释放

	CoUninitialize();  // 结束com

	return nodeValue;
}


// 修改指定节点值
//
BOOL CXmlFile::SetValue(CString nodeName, CString nodeValue)
{
	CoInitialize(NULL);  // 初始化com

	// 创建文档对象
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument30));
	if(hr < 0)
	{     
		return FALSE;
	}   
	pDoc->load((const _variant_t)m_XmlFileName);  // 加载指定xml

	MSXML2::IXMLDOMElementPtr pRoot = pDoc->GetdocumentElement();  // 得到根结点指针            
	MSXML2::IXMLDOMElementPtr pTarget = pRoot->selectSingleNode((_bstr_t)nodeName);  // 得到目标结点指针
	pTarget->Puttext((_bstr_t)nodeValue);  // 设置目标结点值
	pDoc->save((const _variant_t)m_XmlFileName);  // 保存

	pTarget.Release();  // 目标结点释放
	pRoot.Release();  // 根结点释放
	pDoc.Release();  // 文档对象释放

	CoUninitialize();  // 结束com

	return TRUE;
}


// 添加结点和结点值
//
BOOL CXmlFile::AppendForPE(CString sign1, CString sign2, CString sign3, CString sign4)
{
	LONG num = atol(GetValue("num"));  // 病毒库中已有病毒数
	num++;  // 病毒数增加

	// 长整型转化成字符串
	CString strNum;
	strNum.Format("%ld",num);

	SetValue("num", strNum);  // 修改xml文件中病毒数

	CoInitialize(NULL);  // 初始化com

	// 创建文档对象
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument30));
	if(hr < 0)
	{     
		return FALSE;
	}   
	pDoc->load((const _variant_t)m_XmlFileName);  // 加载指定xml

	MSXML2::IXMLDOMElementPtr pRoot = pDoc->GetdocumentElement();  // 得到根结点指针

	// 子结点
	MSXML2::IXMLDOMElementPtr pChild = pDoc->createElement("virus");
	pRoot->appendChild(pChild);

	/* -------------- 孙子结点 ------------ */
	// ID
	MSXML2::IXMLDOMElementPtr pGrandChild = pDoc->createElement("ID");
	pGrandChild->Puttext((_bstr_t)strNum);
	pChild->appendChild(pGrandChild);

	/*---------- Signature ------------*/
	if(sign1.IsEmpty() == false)
	{
		pGrandChild = pDoc->createElement("sign");
		pGrandChild->Puttext((_bstr_t)sign1);
		pChild->appendChild(pGrandChild);
	}

	if(sign2.IsEmpty() == false)
	{
		pGrandChild = pDoc->createElement("sign");
		pGrandChild->Puttext((_bstr_t)sign2);
		pChild->appendChild(pGrandChild);
	}

	if(sign3.IsEmpty() == false)
	{
		pGrandChild = pDoc->createElement("sign");
		pGrandChild->Puttext((_bstr_t)sign3);
		pChild->appendChild(pGrandChild);
	}

	if(sign4.IsEmpty() == false)
	{
		pGrandChild = pDoc->createElement("sign");
		pGrandChild->Puttext((_bstr_t)sign4);
		pChild->appendChild(pGrandChild);
	}

	pDoc->save((const _variant_t)m_XmlFileName);  // 保存

	pGrandChild.Release();  // 孙子结点释放
	pChild.Release();  // 儿子结点释放
	pRoot.Release();  // 根结点释放
	pDoc.Release();  // 文档对象释放

	CoUninitialize();  // 结束com

	return TRUE;
}


// 加载病毒库
//
BOOL CXmlFile::LoadForPE(list<PVRECORD>* pList)
{
	CoInitialize(NULL);  // 初始化com

	// 创建文档对象
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument30));
	if(hr < 0)
	{     
		return FALSE;
	}   
	pDoc->load((const _variant_t)m_XmlFileName);  // 加载指定xml

	MSXML2::IXMLDOMElementPtr pRoot = pDoc->GetdocumentElement();  // 得到根结点指针
	MSXML2::IXMLDOMNodeListPtr pVirusList = pRoot->selectNodes("virus");  // 得到virus结点指针列表
	PVRECORD pVRecord;  // 病毒记录

	// 遍历所有病毒记录
	for(long i=0; i<pVirusList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pID = pVirusList->Getitem(i)->selectSingleNode("ID");  // ID结点
		pVRecord = new VRECORD;  // 新建病毒记录
		pVRecord->dwVirusID = atol(_com_util::ConvertBSTRToString(pID->text));  // 病毒ID
		MSXML2::IXMLDOMNodeListPtr pSignList = pVirusList->Getitem(i)->selectNodes("sign");  // 得到sign结点指针列表
		pVRecord->dwSignCount = pSignList->Getlength();  // 一条记录的特征码段数

		for(DWORD i=0; i<pVRecord->dwSignCount; i++)
		{
			// 字符串转特征码
			pVRecord->pVSign[i] = StrToSign(_com_util::ConvertBSTRToString(pSignList->Getitem(i)->text));
		}

		pList->push_back(pVRecord);  // 加入病毒记录 

		pSignList.Release();  // 结点指针列表释放
		pID.Release();  // ID结点释放
	}

	pVirusList.Release();  // 结点指针列表释放
	pRoot.Release();  // 根结点释放
	pDoc.Release();  // 文档对象释放

	CoUninitialize();  // 结束com

	return TRUE;
}


// 字符串转特征码
//
PVSIGNATURE CXmlFile::StrToSign(CString str)
{
	PVSIGNATURE pVSignature = new VSIGNATURE;

	/*------------------- eType -------------------*/
	CString type = str.Left(str.Find(",")).Trim();
	if(type == "BS_PHY_FILE")
	{
		pVSignature->eType = BS_PHY_FILE;
	}
	else if(type == "BS_STRUCT_OFFSET")
	{
		pVSignature->eType = BS_STRUCT_OFFSET;
	}

	str.Delete(0,str.Find(",")+1);  // 字符串中去掉此项，包括空格和逗号 

	/*----------------- dwSubType -----------------*/
	pVSignature->dwSubType = atol(str.Left(str.Find(",")).Trim());
	
	str.Delete(0,str.Find(",")+1);  // 字符串中去掉此项，包括空格和逗号

	/*------------------- nOffset -------------------*/
	pVSignature->nOffset = atoi(str.Left(str.Find(",")).Trim());
	
	str.Delete(0,str.Find(",")+1);  // 字符串中去掉此项，包括空格和逗号

	/*-------------------- nSize --------------------*/
	pVSignature->nSize = atoi(str.Left(str.Find(",")).Trim());
	
	str.Delete(0,str.Find(",")+1);  // 字符串中去掉此项，包括空格和逗号

	/*------------------ eLogicOp -------------------*/
	CString op = str.Left(str.Find(",")).Trim();
	if(op == "BL_EQUAL")
	{
		pVSignature->eLogicOp = BL_EQUAL;
	}
	else if(op == "BL_NOT_EQUAL")
	{
		pVSignature->eLogicOp = BL_NOT_EQUAL;
	}

	str.Delete(0,str.Find(",")+1);  // 字符串中去掉此项，包括空格和逗号

	/*------------------ Signature -------------------*/
	str.Trim();  // 特征码段(真正的特征码)

	CString strByte;  // 一个字节(字符串)  
	int iTemp;  // 过渡变量
	for(int i=0; i<pVSignature->nSize; i++)
	{
		strByte = str.Left(str.Find(",")).Trim();
		sscanf_s(strByte, "%x", &iTemp);  // 将strByte中的值按16进制的格式解释，放入iTemp中
		pVSignature->Signature[i] = (BYTE)iTemp;
		str.Delete(0,str.Find(",")+1);  // 字符串中去掉此项，包括空格和逗号
	}


	return pVSignature;
}