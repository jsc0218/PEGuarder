#pragma once

enum BAV_OBJ_TYPE {BO_PHY_FILE, BO_MEM_FILE};  // 扫描对象类型
enum BAV_SIGN_LOGIC_OPERATION {BL_EQUAL, BL_NOT_EQUAL};  // 逻辑操作参数

// 扫描对象基类，定义扫描病毒时需要用到的对各种病毒对象操作方法，
// 但仅仅虚函数，真正的功能需要在派生于它的对象中进行实现。
class CScanObject
{
public:
	CScanObject(void);
	~CScanObject(void);

	BAV_OBJ_TYPE GetObjType();  // 获得扫描对象类型

	/*----- 纯虚函数，无函数体 --------*/
	virtual BAV_OBJ_TYPE GetObjectType() = 0;  // 获得扫描对象类型
	virtual LPCSTR GetObjectName() = 0;  // 获得扫描对象名字
	virtual BOOL SetObjectName(CString* pstrName) = 0;  // 设置扫描对象名字
	virtual DWORD GetObjectSize() = 0;  // 获得扫描对象大小
	virtual BOOL Open() = 0;  // 打开扫描对象
	virtual BOOL Close() = 0;  // 关闭扫描对象
	virtual BOOL IsOpened() = 0;  // 判断扫描对象是否已打开
	// 比较扫描对象与特征码
	virtual BOOL Compare(IN INT nOffset, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp = BL_EQUAL) = 0;
	// 比较扫描对象与特征码
	virtual BOOL Compare(IN LPBYTE pData, IN INT nSize, IN LPBYTE pSignature, IN BAV_SIGN_LOGIC_OPERATION eLogicOp = BL_EQUAL) = 0;
	
protected:
	BAV_OBJ_TYPE m_eObjType;  // 扫描对象 
};
