#pragma once

// ¥¶¿Ì’ﬂ
class CDisposer
{
public:
	CDisposer(void);
	~CDisposer(void);

	BOOL Delete(IN LPCSTR pszPathName);  // …æ≥˝
	BOOL Isolate(IN LPCSTR pszPathName);  // ∏Ù¿Î  
	BOOL Ignore(IN LPCSTR pszPathName);  // ∫ˆ¬‘
};
