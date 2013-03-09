#pragma once
#include "ScanObject.h"

// 特征码类型
enum BAV_SIGN_TYPE {BS_PHY_FILE/*纯文件偏移*/, BS_STRUCT_OFFSET/*以某个结构的参考点偏移*/};
/* BAV_SIGN_TYPE.dwType == BS_PHY_FILE
    dwSubType:0
	BAV_SIGN_TYPE.dwType == BS_STRUCT_OFFSET
    dwSubType:                                               */
#define BS_SUB_NT_HEADERS 1  // 以NT头为参考点
#define BS_SUB_ENTRY_POINT 2  // 以入口点为参考点   


// 用于存放单一特征码，很多个特征组成了一条病毒记录，也就是一个VRECORD
typedef struct tagVSIGNATURE {
	BAV_SIGN_TYPE eType;  // 用来定义特征码的类型
	DWORD dwSubType;  // 根据前一项的内容而定的子类型  
	INT nOffset;  // 存储该特征码的偏移量
	INT nSize;  // 存储特征码的长度
	BAV_SIGN_LOGIC_OPERATION eLogicOp;  // 逻辑操作
	BYTE	Signature[MAX_SIGNATURE_LEN];  // 存放特征码串
} VSIGNATURE, *PVSIGNATURE;


// 用于存放病毒库中每个病毒记录的内容
typedef struct tagVRECORD {
	DWORD dwVirusID;  // 指定病毒的ID编号
	DWORD dwSignCount;  // 存放特征码的段数，也就是对于该病毒取了多少段特征码
	PVSIGNATURE pVSign[8];  // 存放每段特征码的内容
} VRECORD, *PVRECORD;


// 实现病毒库的加载和卸载操作，主要暴露了Search()方法，
// 用于将扫描对象在本病毒库中检测。调用扫描对象里的Compare()方法进行特征码匹配。
class CVirusDB
{
public:
	CVirusDB(void);
	~CVirusDB(void);

	BOOL Load();  // 加载病毒库 
	BOOL Unload();  // 卸载病毒库
	// 将扫描对象在本病毒库中检测，返回病毒ID或0
	DWORD Search(IN CScanObject* pScanObj);

public:
	list<PVRECORD> m_listVRecords;  // 列表容器，用来存放多个病毒记录
};
