// PE Guarder ServerDlg.h : 头文件
//

#pragma once

#define UM_UpdateState WM_USER+1  // 更新状态消息


// 流式文件信息结构
typedef struct _SOCKET_STREAM_FILE_INFO {
	CHAR cFileName[MAX_PATH];  // 文件名           
	DWORD dwFileSize;  // 文件大小            
} SOCKET_STREAM_FILE_INFO, *PSOCKET_STREAM_FILE_INFO;


// 更新线程参数结构
typedef struct _UpdateDBThreadParam {
	SOCKET_STREAM_FILE_INFO streamFileInfo;  // 流式文件信息
	CString strFilePath;  // 文件路径
	UINT uiPort;  // 端口
	HWND hWnd;  // 窗口句柄
} UpdateDBThreadParam, *PUpdateDBThreadParam;


// CPEGuarderServerDlg 对话框
class CPEGuarderServerDlg : public CDialog
{
// 构造
public:
	CPEGuarderServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PEGUARDERSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 改变对话框和控件背景色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedSelectfile();  // 选择更新文件按钮事件
public:
	afx_msg void OnEnChangeFilepath();  // 文件路径框变动事件
public:
	afx_msg void OnBnClickedStartupdate();  // 开始更新按钮事件
public:
	static UINT UpdateDBThread(LPVOID pParam);  // 更新病毒库线程
public:
	afx_msg LRESULT OnUpdateState(WPARAM wParam, LPARAM lParam);  // 更新状态消息  

private:
	CString m_FilePath;  // 更新文件路径
	CWinThread* m_pUpdateThread;  // 更新线程
	CBrush m_brush;  // 自定义画刷
};
