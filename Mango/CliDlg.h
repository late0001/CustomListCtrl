#pragma once


// CCliDlg 对话框

class CCliDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCliDlg)

public:
	CCliDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCliDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
