#pragma once

// 初始化文件类，初始化文件(set.ini)存储有皮肤信息等
class CIniFile
{
public:
	CIniFile(void);
	~CIniFile(void);

	CString GetIniFileName();  // 获得ini文件路径
	void SetIniFileName(CString iniFileName);  // 设置ini文件路径
	void ReadIni(CString strSection, CString strItem, CString& strValue);  // 读取ini文件相关值
	void WriteIni(CString strSection, CString strItem, CString strValue);  // 写入ini文件相关值
	
private:
	CString m_IniFileName;  // ini文件路径
};
