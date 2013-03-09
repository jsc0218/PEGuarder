#include "StdAfx.h"
#include "Disposer.h"

CDisposer::CDisposer(void)
{
}

CDisposer::~CDisposer(void)
{
}


// É¾³ý
//
BOOL CDisposer::Delete(LPCSTR pszPathName)
{
	if(::DeleteFile(pszPathName) == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// ¸ôÀë
//
BOOL CDisposer::Isolate(LPCSTR pszPathName)
{
	return FALSE;
}


// ºöÂÔ
//
BOOL CDisposer::Ignore(LPCSTR pszPathName)
{
	return FALSE;
}