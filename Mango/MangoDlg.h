
// MangoDlg.h: 头文件
//

#pragma once
#include "EfuseDlg.h"
#include "CliDlg.h"
// CMangoDlg 对话框
class CMangoDlg : public CDialogEx
{
// 构造
public:
	CMangoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANGO_DIALOG };
#endif

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
	CTabCtrl m_tabctrl;
	CEfuseDlg m_efuseDlg;
	CCliDlg m_cliDlg;
	int m_curSelTab;
	CDialogEx* pDlg[2];
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	afx_msg LRESULT OnDevicechange(WPARAM wParam, LPARAM lParam);
public:
	CStatic m_statusLabel1;
	void PrintStatus(CString str);

};

bool RegisterDevice(HANDLE hwnd);
BOOL PrintStatus(CString str);