#include "StdAfx.h"
#include "IniFile.h"

CIniFile::CIniFile(void)
{
}

CIniFile::~CIniFile(void)
{
}


// 获得ini文件路径
//
CString CIniFile::GetIniFileName()
{
	return m_IniFileName;
}


// 设置ini文件路径
//
void CIniFile::SetIniFileName(CString iniFileName)
{
	m_IniFileName = iniFileName;
}


// 读取ini文件相关值
//
void CIniFile::ReadIni(CString strSection, CString strItem, CString& strValue)
{
	GetPrivateProfileString(strSection, strItem, NULL, strValue.GetBuffer(MAX_PATH), MAX_PATH, m_IniFileName); 
}


// 写入ini文件相关值
//
void CIniFile::WriteIni(CString strSection, CString strItem, CString strValue)
{
	WritePrivateProfileString(strSection, strItem, strValue, m_IniFileName); 
}