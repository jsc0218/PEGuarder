#pragma once
#include "ScanObject.h"

// 派生于CScanObject对象，实现CScanObject中定义的虚函数，用于物理文件的相关操作。
class CFileObject : public CScanObject
{
public:
	CFileObject(void);  // 构造函数
	CFileObject(CFileObject& cFileObj);  // 复制构造函数
	~CFileObject(void);  // 析构函数

	BAV_OBJ_TYPE GetObjectType();  // 获得扫描文件类型
	LPCSTR GetObjectName();  // 获得扫描文件名字
	BOOL SetObjectName(CString* pstrName);  // 设置扫描文件名字
	DWORD GetObjectSize();  // 获得扫描文件大小
	BOOL Open();  // 打开扫描文件
	BOOL Close();  // 关闭扫描文件
	BOOL IsOpened();  // 判断扫描文件是否已打开
	// 比较扫描文件与特征码
	BOOL Compare(IN INT nOffset, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp = BL_EQUAL);
	// 比较扫描文件与特征码
	BOOL Compare(IN LPBYTE pData, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp = BL_EQUAL);

public:
	CString m_strObjName;  // 扫描文件名字

protected:
	HANDLE	m_hFile;  // 扫描文件句柄
	DWORD	m_dwObjSize;  // 扫描文件大小
};
