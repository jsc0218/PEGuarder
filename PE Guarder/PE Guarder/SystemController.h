#pragma once

class CSystemController
{
public:
	CSystemController(void);
public:
	~CSystemController(void);
public:
	static void RebootSystem();  // 重启系统方法，封装了两种类型的系统
public:
	static BOOL RebootNTx86();  // 专门针对NT系统的重启方法
public:
	static BOOL SetReg(LPCSTR lpSubKey);  // 注册表设置函数
public:
	static BOOL FileAttributesOperation(CString filePath);  // 设置文件属性
};
