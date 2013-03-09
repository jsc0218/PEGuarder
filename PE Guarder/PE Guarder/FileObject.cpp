#include "StdAfx.h"
#include "FileObject.h"


// 构造函数
//
CFileObject::CFileObject(void)
{
	m_eObjType = BO_PHY_FILE;  // 扫描对象类型为物理文件
	m_hFile = INVALID_HANDLE_VALUE;  // 初始化文件句柄
	m_dwObjSize = 0;  // 初始化文件大小
}


// 复制构造函数
//
CFileObject::CFileObject(CFileObject& cFileObj)
{
	m_eObjType = BO_PHY_FILE;  // 扫描对象类型为物理文件
	m_hFile = INVALID_HANDLE_VALUE;  // 初始化文件句柄
	m_dwObjSize = cFileObj.m_dwObjSize;  // 复制文件大小
	m_strObjName = cFileObj.m_strObjName;  // 复制文件名称
}


// 析构函数
//
CFileObject::~CFileObject(void)
{
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		Close();  // 关闭扫描文件
	}
}


// 获得扫描文件类型
//
BAV_OBJ_TYPE CFileObject::GetObjectType()
{
	return m_eObjType;
}


// 获得扫描文件名字
//
LPCSTR CFileObject::GetObjectName()
{
	return (LPCSTR)m_strObjName;
}


// 设置扫描文件名字
//
BOOL CFileObject::SetObjectName(CString* pstrName)
{
	if(pstrName == NULL)
	{
		return FALSE;
	}

	m_strObjName = *pstrName;  // 赋值文件名
	
	return TRUE;
}


// 获得扫描文件大小
//
DWORD CFileObject::GetObjectSize()
{
	return m_dwObjSize;
}


// 打开扫描文件
//
BOOL CFileObject::Open()
{
	// 打开文件并返回文件句柄
	m_hFile = ::CreateFile(m_strObjName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// 获得文件大小，单位为bytes
	m_dwObjSize = GetFileSize(m_hFile, NULL);														
	
	return TRUE; 
}


// 关闭扫描文件
//
BOOL CFileObject::Close()
{
	// 关闭文件句柄
	if(::CloseHandle(m_hFile) == TRUE)
	{
		m_hFile = INVALID_HANDLE_VALUE;  
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 判断扫描文件是否已打开
//
BOOL CFileObject::IsOpened()
{
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		return TRUE;  // 表示已打开
	}
	else
	{
		return FALSE;  // 表示未打开
	}
}


// 比较扫描文件与特征码
//
BOOL CFileObject::Compare(IN INT nOffset, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp)
{
	// 判断扫描文件是否已打开
	if(IsOpened() == FALSE)
	{
		return FALSE;
	}

	// 特征码位置超出文件大小
	if((nOffset+nSize) > (INT)m_dwObjSize)
	{
		return FALSE;
	}

	// 设置文件指针位置
	if(::SetFilePointer(m_hFile, nOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return FALSE;
	}

	BYTE buf[MAX_SIGNATURE_LEN];
	DWORD dwReadBytes;

	// 从文件指针所指位置开始读取文件
	if(::ReadFile(m_hFile, buf, nSize, &dwReadBytes, NULL) && nSize == dwReadBytes)
	{
		// 比较内存区域buf和pSignature的前nSize个字节
		int nRes = memcmp(buf, pSignature, nSize);  
	
		switch(eLogicOp)
		{
		case BL_EQUAL:
			return nRes == 0;
		case BL_NOT_EQUAL:
			return nRes != 0;
		default:
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}


// 比较扫描文件与特征码
//
BOOL CFileObject::Compare(IN LPBYTE pData, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp)
{
	// CFileObject不支持此方法
	return FALSE;
}