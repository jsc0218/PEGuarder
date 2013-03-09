#include "StdAfx.h"
#include "Engine.h"
#include "MemFileObject.h"


// 构造函数
//
CEngine::CEngine(void)
{
	m_pVDB = NULL;  // 指向病毒库的指针 

	// 扫描记录初始化
	m_ScanRecord.dwVirusID = 0;
	m_ScanRecord.pScanObject= NULL;
}


// 析构函数
//
CEngine::~CEngine(void)
{
	m_pVDB = NULL;  // 指向病毒库的指针
	Release();  // 释放扫描记录
}


// 加载指定病毒库
//
BOOL CEngine::Load(IN CVirusDB* pVDB)
{
	if(pVDB == NULL)
	{
		return FALSE;
	}
	m_pVDB = pVDB;  

	return TRUE;
}


// 释放扫描记录
//
void CEngine::Release()
{
	m_ScanRecord.dwVirusID = 0;
	m_ScanRecord.pScanObject= NULL;
}


// 获得扫描记录
//
SCAN_RECORD CEngine::GetScanRecord()
{
	return m_ScanRecord;
}


// 扫描
//
void CEngine::Scan(LPCSTR pszPathName)
{
	Release();  // 清空前次扫描记录

	CMemFileObject memFileObj;  // 内存映射文件对象
	memFileObj.m_strObjName = pszPathName;  // 扫描文件名字
	if(memFileObj.Open() == FALSE)
	{
		return;
	}

	// 将扫描对象在本病毒库中检测，返回病毒ID或0
	DWORD dwVID = m_pVDB->Search(&memFileObj);
	// 发现病毒
	if(dwVID != 0)
	{
		CFileObject* pScanObj = new CFileObject(memFileObj);  // 重新包装扫描对象

		m_ScanRecord.dwVirusID = dwVID;  // 病毒ID
		m_ScanRecord.pScanObject = pScanObj;  // 扫描对象指针
	}

	memFileObj.Close();  // 关闭对象
}


// 处理病毒
//
BOOL CEngine::Dispose(LPCSTR pszPathName)
{
	// 目前只有删除方法
	if(m_disposer.Delete(pszPathName) == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

