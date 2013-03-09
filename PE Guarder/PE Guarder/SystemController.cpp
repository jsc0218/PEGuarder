#include "StdAfx.h"
#include "SystemController.h"

CSystemController::CSystemController(void)
{
}

CSystemController::~CSystemController(void)
{
}


// 重启系统方法，封装了两种类型的系统
//
void CSystemController::RebootSystem()
{
	// 判断当前使用的操作系统
	DWORD dwVersion = GetVersion();

	if (dwVersion < 0x80000000)  // Windows NT/2000/XP
	{ 
		if(!RebootNTx86())
		{
			::MessageBox(NULL, "无法申请重启权限！" ,"提示" ,MB_OK);
			return;
		}
	}
	else  // Windows 9x/ME
	{ 
		ExitWindowsEx(EWX_REBOOT, 0);  
	}
}


// 专门针对NT系统的重启方法
//
BOOL CSystemController::RebootNTx86()
{
	HANDLE hToken = 0;  // 令牌句柄
	TOKEN_PRIVILEGES tp;  // 令牌权限

	// 得到进程的令牌句柄
	if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		::MessageBox(NULL, "无法申请重启权限！" ,"提示" ,MB_OK);
		return FALSE;
	}
	else
	{
		// 查询进程的权限
		if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid))
		{
			::MessageBox(NULL, "无法申请重启权限！" ,"提示" ,MB_OK);
			return FALSE;
		}
		else
		{
			// 赋给本进程特权
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			// 通知Windows NT修改本进程的权利      
			if(!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
			{
				::MessageBox(NULL, "无法申请重启权限！" ,"提示" ,MB_OK);
				return FALSE;
			}
			else
			{
				// 重启系统
				ExitWindowsEx(EWX_REBOOT, 0);
			}
		}
		CloseHandle(hToken);
		return TRUE;
	}
}


// 注册表设置函数
//
BOOL CSystemController::SetReg(LPCSTR lpSubKey)
{
	char path[MAX_PATH]={0};  // 保存文件路径
	GetModuleFileName(NULL,path,MAX_PATH);  // 将文件路径保存到path数组中
	strcat_s(path," %1");  // 注册表特有参数，表示输入的参数变量

	HKEY hKey;  // 注册表变量
	// 在注册表指定的项下创建一个新项
	RegCreateKey(HKEY_CLASSES_ROOT, lpSubKey, &hKey);
	// 设置注册表指定项的值
	LONG res = RegSetValueEx(hKey, NULL, 0, REG_SZ, (CONST BYTE *)path, (DWORD)strlen(path));

	// 注册表操作错误
	if (res != ERROR_SUCCESS)
	{
		return 0;
	}

	return 1;
}


// 设置文件属性
//
BOOL CSystemController::FileAttributesOperation(CString filePath)
{
	// 1.获得文件属性
	DWORD attr = ::GetFileAttributes(filePath);
	
	// 2.设置文件属性
	if(attr != INVALID_FILE_ATTRIBUTES)  // 文件属性获得成功
	{
		// 文件属性是只读
		if(attr & FILE_ATTRIBUTE_READONLY)  
		{
			// 设置文件属性为普通
			if(!::SetFileAttributes(filePath,FILE_ATTRIBUTE_NORMAL))  
			{
				return FALSE;
			}
		}
	}
	else  // 获得文件属性失败
	{
		return FALSE;
	}

	return TRUE;
}