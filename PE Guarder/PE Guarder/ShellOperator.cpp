#include "StdAfx.h"
#include "ShellOperator.h"

CShellOperator::CShellOperator(void)
{
}

CShellOperator::~CShellOperator(void)
{
}


// 合并两个PIDL，通常用于将父文件夹的全PIDL与它的子文件或文件夹对象的
// 相对PIDL合并，从而得到文件或者子文件夹的全PIDL
//
LPITEMIDLIST CShellOperator::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	UINT cb1 = 0;
	if (pidl1 != NULL)
	{
		cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);  // pidl1所占字节数，不包括必须部分
	}

	UINT cb2 = GetSize(pidl2);  // pidl2所占字节数

	LPITEMIDLIST pidlNew = CreatePidl(cb1 + cb2);  //创建一个新的PIDL
	if (pidlNew != NULL)
	{
		if (pidl1 != NULL)
		{
			memcpy(pidlNew, pidl1, cb1);  // 由pidl1所指内存区域复制cb1个字节到pidlNew所指内存区域
		}

		// 由pidl2所指内存区域复制cb2个字节到pidlNew+cb1所指内存区域
		memcpy(((LPSTR)pidlNew) + cb1, pidl2, cb2);
	}

	return pidlNew;
}


// 获得某个PIDL使用的字节数
//
UINT CShellOperator::GetSize(LPCITEMIDLIST pidl)
{
	UINT cbTotal = 0;  // 用来计字节数

	if (pidl != NULL)
	{
		cbTotal += sizeof(pidl->mkid.cb);  // 一个unsigned short类型变量的长度，这是必须要加的

		while (pidl->mkid.cb > 0)
		{
			cbTotal += pidl->mkid.cb;  // 每一项item所占字节数  

			// 得到一个PIDL中指向下一个item ID的指针
			LPSTR pb = (LPSTR)pidl;
			pb += pidl->mkid.cb;  // 内存分配是连续的
			pidl = (LPITEMIDLIST)pb;
		}
	}

	return cbTotal;
}


// 创建一个新的PIDL
//
LPITEMIDLIST CShellOperator::CreatePidl(UINT cbSize)
{
	LPITEMIDLIST pidl = NULL;

	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);  // 取得IMalloc接口 
	
	if(pMalloc != NULL)
	{
		pidl = (LPITEMIDLIST)pMalloc->Alloc(cbSize);  // 分配内存

		if (pidl != NULL)
		{
			memset(pidl, 0, cbSize);  // 内存清零
		}

		pMalloc->Release();  // 释放IMalloc接口
	}

	return pidl;  
}


// 深度拷贝一个PIDL
//
LPITEMIDLIST CShellOperator::CopyItemID(LPMALLOC pMalloc, LPCITEMIDLIST pidl)
{
	if(pMalloc == NULL || pidl == NULL)
	{
		return NULL;
	}

	LPITEMIDLIST pidlTemp = NULL;
	pidlTemp=(LPITEMIDLIST)pMalloc->Alloc(pidl->mkid.cb+sizeof(pidl->mkid.cb));  // 分配内存
	// 将pidl内存的数据复制到pidlTemp位置
	CopyMemory((PVOID)pidlTemp, (CONST VOID *)pidl, pidl->mkid.cb+sizeof(pidl->mkid.cb));

	return pidlTemp;
}