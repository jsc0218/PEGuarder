#pragma once

#define UM_UpdateState WM_USER+3  // 更新状态消息


// 流式文件信息结构
typedef struct _SOCKET_STREAM_FILE_INFO {
	CHAR cFileName[MAX_PATH];  // 文件名           
	DWORD dwFileSize;  // 文件大小            
} SOCKET_STREAM_FILE_INFO, *PSOCKET_STREAM_FILE_INFO;


// 更新线程参数结构
typedef struct _UpdateDBThreadParam {
	CString strIP;  // IP
	UINT uiPort;  // 端口
	HWND hWnd;  // 窗口句柄
	CString strSavePath;  // 保存路径
} UpdateDBThreadParam, *PUpdateDBThreadParam;


// CDBUpdate 对话框

class CDBUpdate : public CDialog
{
	DECLARE_DYNAMIC(CDBUpdate)

public:
	CDBUpdate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBUpdate();

// 对话框数据
	enum { IDD = IDD_DBUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();  // 对话框初始化函数
public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedStartupdate();  // 开始更新按钮事件
public:
	afx_msg void OnBnClickedStopupdate();  // 停止更新按钮事件
public:
	static UINT UpdateDBThread(LPVOID pParam);  // 更新病毒库线程
public:
	afx_msg LRESULT OnUpdateState(WPARAM wParam, LPARAM lParam);  // 更新状态消息  

private:
	CWinThread* m_pUpdateThread;  // 更新线程
	CBrush m_brush;  // 自定义画刷	
};


