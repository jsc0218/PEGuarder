#include "StdAfx.h"
#include "VirusInfo.h"

CVirusInfo::CVirusInfo(void)
{
}

CVirusInfo::~CVirusInfo(void)
{
}


// 根据病毒ID获取病毒名
//
LPCSTR CVirusInfo::GetNameByID(DWORD dwVID)
{
	LPCSTR pVName = NULL;

	switch(dwVID)
	{
	case 1:
		pVName = "Dos.Eicar";
		break;
	case 2:
		pVName = "NotVirus.Win32.Notepad.xp";
		break;
	case 3:
		pVName = "Win32.CIH.12";
		break;
	default:
		pVName = "可疑文件或自定义文件";
	}

	return pVName;
}