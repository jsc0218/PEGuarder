#pragma once
#include "FileObject.h"

// 派生于CScanObject对象，实现CScanObject中定义的虚函数，用于内存文件对象的相关操作。
class CMemFileObject : public CFileObject
{
public:
	CMemFileObject(void);  // 构造函数
	CMemFileObject(CMemFileObject& cMemFileObj);  // 复制构造函数
	~CMemFileObject(void);  // 析构函数

	LPBYTE GetBuffer();  // 返回内存指针
	BOOL Open();  // 打开内存文件
	BOOL Close();  // 关闭内存文件
	BOOL IsOpened();  // 判断内存文件是否已打开
	// 比较内存文件与特征码
	BOOL Compare(IN INT nOffset, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp = BL_EQUAL);
	// 比较内存文件与特征码
	BOOL Compare(IN LPBYTE pData, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp = BL_EQUAL);
	
protected:
	HANDLE	m_hMapFile;  // 内存文件句柄
	LPBYTE	m_pFile;  // 内存指针(指向映射)
};
