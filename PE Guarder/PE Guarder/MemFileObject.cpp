#include "StdAfx.h"
#include "MemFileObject.h"


// 构造函数
//
CMemFileObject::CMemFileObject(void)
{
	m_eObjType = BO_MEM_FILE;  // 扫描对象类型为内存文件
	m_hMapFile	= NULL;  // 初始化内存文件句柄
	m_pFile = NULL;  // 内存指针初始化
}


// 复制构造函数
//
CMemFileObject::CMemFileObject(CMemFileObject& cMemFileObj)
{
	m_eObjType = BO_MEM_FILE;  // 扫描对象类型为内存文件
	m_hFile = INVALID_HANDLE_VALUE;  // 初始化文件句柄
	m_dwObjSize = cMemFileObj.m_dwObjSize;  // 复制文件大小
	m_strObjName = cMemFileObj.m_strObjName;  // 复制文件名称
	m_hMapFile	= NULL;  // 初始化内存文件句柄
	m_pFile = NULL;  // 内存指针初始化
}


// 析构函数
//
CMemFileObject::~CMemFileObject(void)
{
	if(m_pFile != NULL)
	{
		Close();  // 关闭内存文件
	}
}


// 返回内存指针
//
LPBYTE CMemFileObject::GetBuffer()
{
	return m_pFile;
}


// 打开内存文件
//
BOOL CMemFileObject::Open()
{
	// 调用CFileObject的Open方法
	if(CFileObject::Open() == FALSE)
	{
		return FALSE;
	}

	// 创建一个新的文件映射对象(把文件映射到一个进程的函数)
	m_hMapFile = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(m_hMapFile == NULL)
	{
		Close();
		return FALSE;
	}

	// 创建视图，将文件映射到当前进程内存虚拟地址空间，返回值属于虚拟地址
	m_pFile = (LPBYTE)(::MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 0));
	if(m_pFile == NULL)
	{
		Close();
		return FALSE;
	}

	return TRUE;
}


// 关闭内存文件
//
BOOL CMemFileObject::Close()
{
	// 取消文件映射
	if(m_pFile != NULL)
	{
		::UnmapViewOfFile(m_pFile);
		m_pFile = NULL;
	}

	// 关闭映射对象
	if(m_hMapFile != NULL)
	{
		::CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
	}

	// 调用CFileObject的Close方法
	return CFileObject::Close();
}


// 判断内存文件是否已打开
//
BOOL CMemFileObject::IsOpened()
{
	if(m_pFile != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 比较内存文件与特征码
//
BOOL CMemFileObject::Compare(IN INT nOffset, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp)
{
	return Compare(m_pFile+nOffset, nSize, pSignature, eLogicOp);
}


// 比较内存文件与特征码
//
BOOL CMemFileObject::Compare(IN LPBYTE pData, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp)
{
	// 判断内存文件是否已打开
	if(IsOpened() == FALSE)
	{
		return FALSE;
	}

	// 特征码位置超出文件范围
	if(pData < m_pFile || pData > (m_pFile + m_dwObjSize))
	{
		return FALSE;
	}

	// 比较内存区域pData和pSignature的前nSize个字节
	int nRes = memcmp(pData, pSignature, nSize);

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