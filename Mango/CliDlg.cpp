// CliDlg.cpp: 实现文件
//

#include "pch.h"
#include "Mango.h"
#include "CliDlg.h"
#include "afxdialogex.h"


// CCliDlg 对话框

IMPLEMENT_DYNAMIC(CCliDlg, CDialogEx)

CCliDlg::CCliDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIDLG, pParent)
{

}

CCliDlg::~CCliDlg()
{
}

void CCliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCliDlg, CDialogEx)
END_MESSAGE_MAP()


// CCliDlg 消息处理程序
