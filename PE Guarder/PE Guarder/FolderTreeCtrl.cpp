// FolderTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "PE Guarder.h"
#include "FolderTreeCtrl.h"
#include "ShellOperator.h"  // 引入对pidl的操作头文件

// CFolderTreeCtrl

IMPLEMENT_DYNAMIC(CFolderTreeCtrl, CTreeCtrl)

CFolderTreeCtrl::CFolderTreeCtrl()
{

}

CFolderTreeCtrl::~CFolderTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CFolderTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CFolderTreeCtrl::OnTvnItemexpanded)
END_MESSAGE_MAP()



// CFolderTreeCtrl 消息处理程序


// 当鼠标左键点击在CheckBox或"+"上时,需单独处理
//
void CFolderTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);  // 获取点击位置并返回相应结点句柄

	if(uFlags & TVHT_ONITEMSTATEICON )  // 若点击在选择框上
	{            
		UINT uState = GetItemState(hItem, TVIS_STATEIMAGEMASK) >> 12;  // 获取结点状态
		
		if(uState == PartChecked)  // 若本来为部分选择状态,则置为全部选择
		{
			uState = AllChecked;
		}
		else if(uState == UnChecked)  // 若本来为未选择状态,则置为全部选择
		{
			uState = AllChecked;
		}
		else if(uState == AllChecked)  // 若本来为全部选择状态,则置为未选择
		{
			uState = UnChecked;
		}

		SetItemState(hItem, INDEXTOSTATEIMAGEMASK(uState), TVIS_STATEIMAGEMASK);  // 设置结点状态
		SetChildItemState(hItem, uState);  // 设置所有子结点状态
		SetParentItemState(hItem, uState);  //  设置所有父结点状态
	}
	else if(uFlags & TVHT_ONITEMBUTTON)  // 若点击在与树项关联的按钮"+"上  
	{                                       
		// 必须先插入一项,展开后再删除
		HTREEITEM hTempItem = InsertItem("*.*", hItem);
		CTreeCtrl::OnLButtonDown(nFlags, point);
		DeleteItem(hTempItem);
	}
	else
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);  // 系统自动添加
	}
}


// 结点展开处理
//
void CFolderTreeCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 曾经展开过,则不再处理
	if(pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE)   
	{
		return;
	}

	LPTVItemData pItemData = (LPTVItemData)pNMTreeView->itemNew.lParam;
	if(pItemData != NULL)
	{
		LPSHELLFOLDER psfNew;
		// 获取当前IShellFolder接口的lpi所指的IShellFolder接口，psfNew接收检索到的接口指针
		pItemData->lpsfParent->BindToObject(pItemData->lpi, 0, IID_IShellFolder, (LPVOID*)&psfNew);
		// 填充结点
		FillTreeCtrl(psfNew, pItemData->lpifq, pNMTreeView->itemNew.hItem);

		// 排列树项目的结构
		TVSORTCB tvSortCB;
		tvSortCB.hParent = pNMTreeView->itemNew.hItem;
		tvSortCB.lpfnCompare = TreeViewCompareProc;
		tvSortCB.lParam  = NULL;
		
		// 使用由应用程序定义的排序函数TreeViewCompareProc来排列子项
		SortChildrenCB(&tvSortCB); 
	}

	*pResult = 0;
}


// 显示树形结构目录
//
BOOL CFolderTreeCtrl::DisplayTree()
{
	// 获取系统小图标句柄
	SHFILEINFO shFileInfo;
	HIMAGELIST hSysSmallImgList = (HIMAGELIST)SHGetFileInfo("c:\\", 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	// 将系统小图标句柄绑定到TreeCtrl 上
	if(hSysSmallImgList != NULL)
	{
		::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) TVSIL_NORMAL, (LPARAM) hSysSmallImgList);
	}
	else
	{
		return FALSE;
	} 

	// 选择框图标设置
	m_ImgState.Create(IDB_BMP_STATE, 13, 0, RGB(255,255,255));
	SetImageList(&m_ImgState, TVSIL_STATE);

	// 插入"桌面"作为树根结点
	m_hDesktop = InsertSpecialFolder(CSIDL_DESKTOP, TVI_ROOT);
	// 设置桌面结点无选择框
	SetItemState(m_hDesktop, INDEXTOSTATEIMAGEMASK(NoCheckBox), TVIS_STATEIMAGEMASK);

	// 返回一个桌面文件夹的IShellFolder接口
	LPSHELLFOLDER psfDesktop;
	SHGetDesktopFolder(&psfDesktop);

	// 填充结点
	FillTreeCtrl(psfDesktop, NULL, m_hDesktop);

	// 释放接口
	psfDesktop->Release();

	// 展开桌面结点
	Expand(m_hDesktop, TVE_EXPAND);

	// 展开我的电脑，为了能展开,必须先插入一项,展开后再删除
	HTREEITEM hTemp = InsertItem("*", m_hMyComputer);
	Expand(m_hMyComputer, TVE_EXPAND);
	DeleteItem(hTemp);

	return TRUE;
}


// 在hParent后插入由nFolder指定的特殊文件夹
//
HTREEITEM CFolderTreeCtrl::InsertSpecialFolder(int nFolder, HTREEITEM hParent)
{
	// 得到"桌面"的磁盘"全路径"
	LPITEMIDLIST pidl;             
	SHGetSpecialFolderLocation(m_hWnd, nFolder, &pidl);

	// 返回一个"桌面"文件夹的IShellFolder接口
	LPSHELLFOLDER psfDesktop;
	SHGetDesktopFolder(&psfDesktop);  

	// 获取pidl对应文件夹的显示名
	CString fileName;
	GetFileName(psfDesktop, pidl, fileName);

	// 释放接口
	psfDesktop->Release();

	// 树中插入指定项
	HTREEITEM hItem = InsertItem(fileName, GetSmallIcon(pidl), GetSelectedIcon(pidl), hParent); 

	return hItem;
}


// 获取pidl对应文件夹的显示名
//
void CFolderTreeCtrl::GetFileName(LPSHELLFOLDER psf, LPITEMIDLIST pidl, CString& fileName)
{
	// 获得显示名
	STRRET strRet;  // 用来存储IShellFolder接口的方法返回的显示名
	psf->GetDisplayNameOf(pidl, SHGDN_NORMAL, &strRet);  // 把显示名存入STRRET结构

	// 把STRRET结构内的显示名导出
	LPSTR str;
	StrRetToStr(&strRet, pidl, &str);  // 显示名由STRRET结构传递到str
	fileName = str;  // 显示名传给fileName

	CoTaskMemFree(str);  // 释放资源  
}


// 获取pidl对应文件夹小图标
//
int CFolderTreeCtrl::GetSmallIcon(LPITEMIDLIST pidl)
{
	SHFILEINFO shFileInfo;
	SHGetFileInfo((LPCTSTR)pidl, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	
	return shFileInfo.iIcon;  // 返回图标的系统索引号  
}


// 获取pidl对应文件夹被选定时的图标
//
int CFolderTreeCtrl::GetSelectedIcon(LPITEMIDLIST pidl)
{
	SHFILEINFO shFileInfo;
	SHGetFileInfo((LPCTSTR)pidl, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
	
	return shFileInfo.iIcon;  // 返回图标的系统索引号        
}


// 填充结点
//
void CFolderTreeCtrl::FillTreeCtrl(LPSHELLFOLDER psf, LPITEMIDLIST pidl, HTREEITEM hParent)
{
	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);  // 取得IMalloc接口   
	
	LPENUMIDLIST peidl;  // IEnumIDList接口指针
	// 以指定文件夹为根，枚举文件(夹)(包括隐藏文件(夹))，并静默磁盘通知消息
	HRESULT hr = psf->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN | SHCONTF_STORAGE, &peidl);  
	// 处理空磁盘情况
	if(FAILED(hr))
	{
		return;
	}

	LPITEMIDLIST pItem;  // 相对于psf的相对PIDL
	ULONG ulFetched; 
	ULONG ulAttrs;
	
	// 调用IEnumIDList接口对象的Next方法遍历，并获得每个子对象的相对PIDL
	while(peidl->Next(1, &pItem, &ulFetched) == NOERROR)
	{
		ulAttrs = SFGAO_FOLDER | SFGAO_HASSUBFOLDER;  // 文件夹且有子文件夹
		psf->GetAttributesOf(1, (const ITEMIDLIST**)&pItem, &ulAttrs);  // 获得对象属性

		if(ulAttrs & SFGAO_FOLDER)  // 是文件夹 
		{
			TVITEM tvItem;  // 用来发送或接收关于一个树型视图的项目的有关信息
			tvItem.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;  // 指定TVITEM有效成员

			if(ulAttrs & SFGAO_HASSUBFOLDER)  // 有子文件夹
			{
				// 显示"+"
				tvItem.mask = tvItem.mask | TVIF_CHILDREN;  
			}

			// 设置子结点与父结点状态一致
			UINT uState = GetItemState(hParent, TVIS_STATEIMAGEMASK) >> 12;  // 得到父结点状态
			tvItem.state = INDEXTOSTATEIMAGEMASK(uState);  // 设置子结点状态
			tvItem.stateMask = TVIS_STATEIMAGEMASK;  // 设置子结点状态  

			// 获取pItem对应文件夹的显示名
			CString fileName;
			GetFileName(psf, pItem, fileName);

			// 不显示"控制面板"、"网上邻居"、"回收站"
			if(fileName == "控制面板" || fileName == "网上邻居" || fileName == "回收站")           
			{
				continue;
			}

			// 合并两个PIDL，获得"全路径"
			LPITEMIDLIST pidlNew = CShellOperator::ConcatPidls(pidl, pItem);

			tvItem.pszText = fileName.GetBuffer(MAX_PATH);  // 获得指向显示名缓冲区的指针
			tvItem.iImage = GetSmallIcon(pidlNew);  // 获取pidlNew对应文件夹小图标
			tvItem.iSelectedImage = GetSelectedIcon(pidlNew);  // 获取pidlNew对应文件夹被选定时的图标

			LPTVItemData pItemData = (LPTVItemData)pMalloc->Alloc(sizeof(TVItemData));  // 分配内存
			pItemData->lpi = CShellOperator::CopyItemID(pMalloc, pItem);  // 相对PIDL
			pItemData->lpifq = pidlNew;  // 全PIDL
			pItemData->lpsfParent = psf;  // 父文件夹的IShellFolder指针

			tvItem.lParam = (LPARAM)pItemData;

			TVINSERTSTRUCT tvInsertStruct;  // 包含添加新项到树形视控件所使用的信息
			tvInsertStruct.item = tvItem;  // 所要添加的条目
			tvInsertStruct.hParent = hParent;  // 父结点
			tvInsertStruct.hInsertAfter = TVI_LAST;  // 插入到最后位置

			HTREEITEM hNewItem = InsertItem(&tvInsertStruct);  // 插入新项

			GetPathFromIDList(hNewItem, pidlNew, fileName);  // 把ITEMIDLIST转换为文档系统路径
		}
	}

	pMalloc->Free(pItem);  // 释放标识符列表
	pMalloc->Release();  // 释放IMalloc接口
}


// 把ITEMIDLIST转换为文档系统路径
//
void CFolderTreeCtrl::GetPathFromIDList(HTREEITEM hNewItem, LPCITEMIDLIST pidl, CString fileName)
{
	CHAR folderPath[MAX_PATH];
	if(SHGetPathFromIDList(pidl, folderPath))
	{
		CString path;
		path.Format("%s", folderPath);
		m_mapFolderPath[hNewItem] = path;
	}

	// "我的电脑"要特殊处理，因为SHGetPathFromIDList得出的路径为空
	if(fileName == "我的电脑")     
	{                                 
		m_hMyComputer = hNewItem; 
	}
}


// 当选择(取消)某一项时,自动选择(取消)其所有子(孙)项(递归实现)
// 
void CFolderTreeCtrl::SetChildItemState(HTREEITEM hItem, UINT uState)
{
	hItem = GetChildItem(hItem);  // 获取由hItem指定的项的子项

	// 同层遍历
	while(hItem != NULL)
	{
		SetItemState(hItem, INDEXTOSTATEIMAGEMASK(uState), TVIS_STATEIMAGEMASK);  // 设置hItem指定的项的状态
		SetChildItemState(hItem, uState);  // 递归
		hItem = GetNextSiblingItem(hItem);  // 获取下一个兄弟项
	}
}


// 当选择(取消)某一项时, 修改其所有父项状态(递归实现)
// (1).  子结点为"部分选择",则父结点也为"部分选择"
// (2.1).子结点为"没有选择",若其兄弟结点有"全部选择"或"部分选择",则父结点为"部分选择"
// (2.2).子结点为"没有选择",若其兄弟结点都"没有选择",则父结点为"没有选择"
// (3.1).子结点为"全部选择",若其兄弟结点有"没有选择"或"部分选择",则父结点为"部分选择"
// (3.2).子结点为"全部选择",若其兄弟结点都"全部选择",则父结点为"全部选择"
//
void CFolderTreeCtrl::SetParentItemState(HTREEITEM hItem, UINT uState)
{
	// "桌面"结点不显示选择框
	if(hItem == m_hDesktop)                   
	{
		SetItemState(m_hDesktop, INDEXTOSTATEIMAGEMASK(NoCheckBox), TVIS_STATEIMAGEMASK);
		return;
	}

	UINT parentState;
	HTREEITEM hParent = GetParentItem(hItem);  // 获得父结点句柄
	if(hParent == NULL)  // 无父结点
	{
		return;
	}

	if(uState == PartChecked)  // (1)子结点为"部分选择",则父结点也为"部分选择"
	{
		parentState = PartChecked;  
	}

	// (2)子结点为"没有选择"时
	if(uState == UnChecked)
	{
		HTREEITEM hSiblingItem = GetChildItem(hParent);  // 获得第一个子结点句柄

		UINT uSiblingState;
		// 同层遍历
		while(hSiblingItem != NULL)
		{
			uSiblingState = GetItemState(hSiblingItem, TVIS_STATEIMAGEMASK) >> 12;  // 获得结点状态
			// (2.1)子结点为"没有选择",若其兄弟结点有"全部选择"或"部分选择",则父结点为"部分选择"	
			if(uSiblingState != UnChecked)           
			{
				parentState = PartChecked;
				break;
			}
			hSiblingItem = GetNextSiblingItem(hSiblingItem);  // 获得兄弟结点
		}

		// (2.2)子结点为"没有选择",若其兄弟结点都"没有选择",则父结点为"没有选择"
		if(uSiblingState == UnChecked)                   
		{
			parentState = UnChecked;
		}
	}

	// (3)子结点为"全部选择"时
	if(uState == AllChecked)
	{
		HTREEITEM hSiblingItem = GetChildItem(hParent);  // 获得第一个子结点句柄

		UINT uSiblingState;
		// 同层遍历
		while(hSiblingItem != NULL)
		{
			uSiblingState = GetItemState(hSiblingItem, TVIS_STATEIMAGEMASK) >> 12;  // 获得结点状态
			// (3.1)子结点为"全部选择",若其兄弟结点有"没有选择"或"部分选择",则父结点为"部分选择"
			if(uSiblingState != AllChecked)
			{
				parentState = PartChecked;      
				break;
			}
			hSiblingItem = GetNextSiblingItem(hSiblingItem);  // 获得兄弟结点
		}

		// (3.2)子结点为"全部选择",若其兄弟结点都"全部选择",则父结点为"全部选择"
		if(uSiblingState == AllChecked)
		{
			parentState = AllChecked;               
		}
	}

	// 设置父结点选择框状态
	SetItemState(hParent, INDEXTOSTATEIMAGEMASK(parentState), TVIS_STATEIMAGEMASK);

	// 递归
	SetParentItemState(hParent, parentState);
}


// 取得被选择的路径
// 外部接口函数，其主要功能由GetSelectedPath(HTREEITEM hItem, CStringArray& listSelectedPath)实现
//  
void CFolderTreeCtrl::GetSelectedPath(CStringList& listSelectedPath)
{
	listSelectedPath.RemoveAll();  // 清空字符串列表
	HTREEITEM hStartItem = GetChildItem(m_hDesktop);  // 获得"桌面"的第一个子结点句柄
	GetSelectedPath(hStartItem, listSelectedPath);  // 真正实现功能的函数
}


// 获取hItem下面"全部选择"的文件夹的路径,由listSelectedPath返回(递归实现)
// (1.1).若为"全部选择"且不为"我的电脑",则由m_mapFolderPath[hItem]返回路径
// (1.2).若为"全部选择"且为"我的电脑",由于SHGetPathFromIDList返回的路径为空,所以必须手动添加"我的电脑"下所有文件夹
// (2).  若为"部分选择",则到其子目录下进行查找
// (3).  若有兄弟,查找其兄弟项
//
void CFolderTreeCtrl::GetSelectedPath(HTREEITEM hItem, CStringList& listSelectedPath)
{
	UINT uState = GetItemState(hItem, TVIS_STATEIMAGEMASK) >> 12;  // 获得结点状态      
       
	// (1)."全部选择"时
	if(uState == AllChecked)
	{
		// (1.1).若为"全部选择"且不为"我的电脑",则由m_mapFolderPath[hItem]返回路径
		if(hItem != m_hMyComputer)      
		{
			listSelectedPath.AddTail(m_mapFolderPath[hItem]);  // 在列表尾部添加元素(路径)
		}
		// (1.2).若为"全部选择"且为"我的电脑",由于SHGetPathFromIDList返回的路径为空,
		// 所以必须手动添加"我的电脑"下所有文件夹
		else                            
		{
			HTREEITEM hRootDir = GetChildItem(m_hMyComputer);  // 获得"我的电脑"第一个子结点句柄
			// 同层遍历
			while(hRootDir != NULL)
			{
				listSelectedPath.AddTail(m_mapFolderPath[hRootDir]);  // 在列表尾部添加元素(路径)
				hRootDir = GetNextSiblingItem(hRootDir);  // 获得兄弟结点句柄
			}
		}
	}
        
	// (2).若为"部分选择",则到其子目录下进行查找
	if(uState == PartChecked)               
	{
		HTREEITEM hChild = GetChildItem(hItem);  // 获得第一个子结点句柄
		GetSelectedPath(hChild, listSelectedPath);  // 递归
	} 

	// (3).若有兄弟,查找其兄弟项
	HTREEITEM hSibling = GetNextSiblingItem(hItem);  // 获得兄弟结点句柄
	if(hSibling != NULL)                            
	{
		GetSelectedPath(hSibling, listSelectedPath);  // 递归
	}
}


// 用于整理树项排列的回调函数
//
int CALLBACK CFolderTreeCtrl::TreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{
	LPTVItemData lptvid1=(LPTVItemData)lparam1;
	LPTVItemData lptvid2=(LPTVItemData)lparam2;
	if(lptvid1 == NULL || lptvid2 == NULL)
	{
		return 0;  // 两者地位等价
	}
        
	HRESULT hr = lptvid1->lpsfParent->CompareIDs(0,lptvid1->lpi,lptvid2->lpi);  // 比较两者相对顺序顺序

	// 如果返回0，代表两者顺序等价
	// 如果大于0，代表前者在后者后面
	// 如果小于0，代表前者在后者前面 
	return (short)HRESULT_CODE((hr));  
}