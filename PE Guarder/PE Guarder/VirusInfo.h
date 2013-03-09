#pragma once

// 根据病毒ID获取病毒相关信息
class CVirusInfo
{
public:
	CVirusInfo(void);
	~CVirusInfo(void);

	static LPCSTR GetNameByID(DWORD dwVID);  // 根据病毒ID获取病毒名
};
