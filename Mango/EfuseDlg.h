#pragma once
#include "ListCtrlEdit.h"

// CEfuseDlg 对话框

class CEfuseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEfuseDlg)

public:
	CEfuseDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEfuseDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFUSEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CEditListCtrl m_listCtrl;
	afx_msg void OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedRead();
	void ListCtrlInsertItem(UINT a[]);
	void ListCtrlDeleteAllItem();
};
