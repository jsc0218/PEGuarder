#include "StdAfx.h"
#include "ScanObject.h"

CScanObject::CScanObject(void)
{
}

CScanObject::~CScanObject(void)
{
}


// 获得扫描对象类型
//
BAV_OBJ_TYPE CScanObject::GetObjType()
{
	return m_eObjType;
}
