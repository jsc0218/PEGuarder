#pragma once
#include "VirusDB.h"
#include "Disposer.h"

// 扫描记录结构
typedef struct tagScanRecord
{
	DWORD dwVirusID;  // 病毒ID 
	CScanObject*	pScanObject;  // 扫描对象指针
}SCAN_RECORD, *PSCAN_RECORD;


// 引擎类，实现杀毒主流程的控制。创建被查杀对象CScanObject，
// 调用病毒库对象CVirusDB来检测，并将结果记录下来。
class CEngine
{
public:
	CEngine(void);  // 构造函数
	~CEngine(void);  // 析构函数

	BOOL Load(IN CVirusDB* pVDB);  // 加载指定病毒库
	void Release();  // 释放扫描记录
	SCAN_RECORD GetScanRecord();  // 获得扫描记录 
	void Scan(IN LPCSTR pszPathName);  // 扫描
	BOOL Dispose(IN LPCSTR pszPathName);  // 处理病毒

protected:
	CVirusDB *m_pVDB;  // 指向病毒库的指针  
	SCAN_RECORD m_ScanRecord;  // 一段扫描记录
	CDisposer m_disposer;  // 处理者
};
