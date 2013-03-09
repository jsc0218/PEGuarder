#pragma once

class CShellOperator
{
public:
	CShellOperator(void);
public:
	~CShellOperator(void);
public:
	// 合并两个PIDL，通常用于将父文件夹的全PIDL与它的子文件或文
	// 件夹对象的相对PIDL合并，从而得到文件或者子文件夹的全PIDL 
	static LPITEMIDLIST ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);  
public:
	static UINT GetSize(LPCITEMIDLIST pidl);  // 获得某个PIDL使用的字节数
public:
	static LPITEMIDLIST CreatePidl(UINT cbSize);  // 创建一个新的PIDL
public:
	static LPITEMIDLIST CopyItemID(LPMALLOC lpMalloc, LPCITEMIDLIST lpi);  // 深度拷贝一个PIDL
};
