#pragma once

#include "shlwapi.h"


// IDB_BMP_STATE状态: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
enum CheckState{        
	NoCheckBox,  // 无选择框
	UnChecked,  // 没有选择
	PartChecked,  // 部分选择
	AllChecked  // 全部选择
};

// 该结构用于保存一个树项的信息
typedef struct _TV_ItemData     
{
	LPITEMIDLIST lpi;  // 相对PIDL
	LPITEMIDLIST lpifq;  // 全PIDL
	LPSHELLFOLDER lpsfParent;  // 父文件夹的IShellFolder指针
}TVItemData, *LPTVItemData;


// CFolderTreeCtrl
// 定义自己的文件夹树
class CFolderTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFolderTreeCtrl)

public:
	CFolderTreeCtrl();
	virtual ~CFolderTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL DisplayTree();  // 显示树形结构目录
	void GetSelectedPath(CStringList& listSelectedPath);  // 取得被选择的路径

private:
	// 在hParent后插入由nFolder指定的特殊文件夹
	HTREEITEM InsertSpecialFolder(int nFolder, HTREEITEM hParent);  

	void GetFileName(LPSHELLFOLDER psf, LPITEMIDLIST pidl, CString& fileName);  // 获取pidl对应文件夹的显示名
	int GetSmallIcon(LPITEMIDLIST pidl);  // 获取pidl对应文件夹小图标
	int GetSelectedIcon(LPITEMIDLIST pidl);  // 获取pidl对应文件夹被选定时的图标

	void FillTreeCtrl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent);  // 填充结点

	// 把ITEMIDLIST转换为文档系统路径
	void GetPathFromIDList(HTREEITEM hNewItem, LPCITEMIDLIST pidlNew, CString fileName);  

	// 当选择(取消)某一项时,自动选择(取消)其所有子(孙)项(递归实现)
	void SetChildItemState(HTREEITEM hItem, UINT uState);  
	// 当选择(取消)某一项时, 修改其所有父项状态(递归实现)
	void SetParentItemState(HTREEITEM hItem, UINT uState);  

	// 获取hItem下面"全部选择"的文件夹的路径,由listSelectedPath返回(递归实现)
	void GetSelectedPath(HTREEITEM hItem, CStringList& listSelectedPath);  

	// 用于整理树项排列的回调函数
	static int CALLBACK TreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort);

private:
	CImageList m_ImgState;  // 选择框图标变量
	HTREEITEM m_hDesktop;  // 桌面根结点
	CMap<HTREEITEM, HTREEITEM, CString, CString> m_mapFolderPath;  // 存放<key=HTREEITEM, value=文件路径>
	HTREEITEM m_hMyComputer;  // "我的电脑"节点要特殊处理,因为SHGetPathFromIDList获取路径为空

public:	
	// 当鼠标左键点击在CheckBox或"+"上时,需单独处理
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	// 结点展开处理
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
};


