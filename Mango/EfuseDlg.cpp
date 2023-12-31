﻿// EfuseDlg.cpp: 实现文件
//

#include "pch.h"
#include "Mango.h"
#include "EfuseDlg.h"
#include "afxdialogex.h"
#include "xcmd.h"

// CEfuseDlg 对话框

IMPLEMENT_DYNAMIC(CEfuseDlg, CDialogEx)

CEfuseDlg::CEfuseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EFUSEDLG, pParent)
{

}

CEfuseDlg::~CEfuseDlg()
{
}

void CEfuseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CEfuseDlg, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CEfuseDlg::OnCustomdrawList1)
	ON_BN_CLICKED(ID_READ, &CEfuseDlg::OnBnClickedRead)
END_MESSAGE_MAP()


// CEfuseDlg 消息处理程序


BOOL CEfuseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// TODO: 在此添加额外的初始化代码
	// Style
	DWORD dwStyle = ::GetWindowLong(m_listCtrl.GetSafeHwnd(), GWL_STYLE);
	//dwStyle |= LVS_SINGLESEL;       //只可单行选中
	dwStyle |= LVS_SHOWSELALWAYS;   //Always show selection
	dwStyle |= LVS_REPORT; //报告风格
	::SetWindowLong(m_listCtrl.GetSafeHwnd(), GWL_STYLE, dwStyle);

	// Extended Style
	DWORD dwStyleEx = m_listCtrl.GetExtendedStyle();
	dwStyleEx |= LVS_EX_GRIDLINES;        //网格线
	dwStyleEx |= LVS_EX_FULLROWSELECT;    //整行高亮
	//dwStyleEx |= LVS_EX_CHECKBOXES;       //Item前生成check box
	m_listCtrl.SetExtendedStyle(dwStyleEx);

	// 插入列
	m_listCtrl.InsertColumn( 0, TEXT(" "), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 1, TEXT("0"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 2, TEXT("1"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 3, TEXT("2"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 4, TEXT("3"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 5, TEXT("4"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 6, TEXT("5"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 7, TEXT("6"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 8, TEXT("7"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn( 9, TEXT("8"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(10, TEXT("9"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(11, TEXT("A"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(12, TEXT("B"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(13, TEXT("C"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(14, TEXT("D"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(15, TEXT("E"), LVCFMT_RIGHT, 32);
	m_listCtrl.InsertColumn(16, TEXT("F"), LVCFMT_RIGHT, 32);
	//m_listCtrl.InsertColumn(17, TEXT("颜色"), LVCFMT_LEFT, 100);
	//
	int nItem = 0;
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("00"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("26"));   //子项从1开始,0代表主项
	m_listCtrl.SetItemText(nItem, 2, TEXT("FF"));
	//m_listCtrl.SetItemText(nItem, 17, TEXT("0xFF0000"));//255 数值 代表背景的颜色值
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("01"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("24"));
	m_listCtrl.SetItemText(nItem, 2, TEXT("FF"));
	//m_listCtrl.SetItemText(nItem, 17, TEXT("0x163172"));//255*256 数值 代表背景的颜色值
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("02"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("24"));
	m_listCtrl.SetItemText(nItem, 2, TEXT("FF"));
	//m_listCtrl.SetItemText(nItem, 17, TEXT("0xFF00FF"));//255+255*256 数值 代表背景的颜色值
	//这些语句最好放到初始化的最后，至少要放到插入完列以后，否则需要传递列数
	m_listCtrl.column_type();//获得列数,并初始化 各列的默认类型,不同的类型见下面的赋值语句
	m_listCtrl.m_column_type[1] = 1;//第2列编辑框
	m_listCtrl.m_column_type[2] = 1;//第3列组合框
	m_listCtrl.m_column_type[3] = 1;//第3列组合框
	m_listCtrl.m_column_type[4] = 1;//第3列组合框
	m_listCtrl.m_column_type[5] = 1;//第3列组合框
	m_listCtrl.m_column_type[6] = 1;//第3列组合框
	m_listCtrl.m_column_type[7] = 1;//第3列组合框
	m_listCtrl.m_column_type[8] = 1;//第3列组合框
	m_listCtrl.m_column_type[9] = 1;//第3列组合框
	m_listCtrl.m_column_type[10] = 1;//第3列组合框
	m_listCtrl.m_column_type[11] = 1;//第3列组合框
	m_listCtrl.m_column_type[12] = 1;//第3列组合框
	m_listCtrl.m_column_type[13] = 1;//第3列组合框
	m_listCtrl.m_column_type[14] = 1;//第3列组合框
	m_listCtrl.m_column_type[15] = 1;//第3列组合框
	m_listCtrl.m_column_type[16] = 1;//第3列组合框
	//m_listCtrl.m_column_type[17] = 3;//3;//第4列背景变色框,文本的数值为背景的颜色数值
	//m_listCtrl.column_string();//初始化各列的项目内容, 组合框列的项目要单独赋值,见下面语句
	//m_listCtrl.m_column_string[2] = "FF\n00\n";//第3列为性别,组合框的内容以换行符分割
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEfuseDlg::OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
	m_listCtrl.OnCustomdrawMyList(pNMHDR, pResult);
}

void  CEfuseDlg::ListCtrlDeleteAllItem()
{
	m_listCtrl.DeleteAllItems();
}

void CEfuseDlg::ListCtrlInsertItem(UINT a[]) 
{
	int nItem = 0;
	CString str;
	str.Format(_T("%02X"), m_listCtrl.GetItemCount());
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), str, 0);
	str.Format(_T("%02X"), a[0]);
	m_listCtrl.SetItemText(nItem, 1, str);   //子项从1开始,0代表主项
	str.Format(_T("%02X"), a[1]);
	m_listCtrl.SetItemText(nItem, 2, str);
	str.Format(_T("%02X"), a[2]);
	m_listCtrl.SetItemText(nItem, 3, str);
	str.Format(_T("%02X"), a[3]);
	m_listCtrl.SetItemText(nItem, 4, str);
	str.Format(_T("%02X"), a[4]);
	m_listCtrl.SetItemText(nItem, 5, str);
	str.Format(_T("%02X"), a[5]);
	m_listCtrl.SetItemText(nItem, 6, str);
	str.Format(_T("%02X"), a[6]);
	m_listCtrl.SetItemText(nItem, 7, str);
	str.Format(_T("%02X"), a[7]);
	m_listCtrl.SetItemText(nItem, 8, str);
	str.Format(_T("%02X"), a[8]);
	m_listCtrl.SetItemText(nItem, 9, str);
	str.Format(_T("%02X"), a[9]);
	m_listCtrl.SetItemText(nItem, 10, str);
	str.Format(_T("%02X"), a[10]);
	m_listCtrl.SetItemText(nItem, 11, str);
	str.Format(_T("%02X"), a[11]);
	m_listCtrl.SetItemText(nItem, 12, str);
	str.Format(_T("%02X"), a[12]);
	m_listCtrl.SetItemText(nItem, 13, str);
	str.Format(_T("%02X"), a[13]);
	m_listCtrl.SetItemText(nItem, 14, str);
	str.Format(_T("%02X"), a[14]);
	m_listCtrl.SetItemText(nItem, 15, str);
	str.Format(_T("%02X"), a[15]);
	m_listCtrl.SetItemText(nItem, 16, str);
	//m_listCtrl.SetItemText(nItem, 17, TEXT("0xFF00FF"));//255 数值 代表背景的颜色值
}
void CEfuseDlg::OnBnClickedRead()
{
	// TODO: 在此添加控件通知处理程序代码
	cli_read_efuse(this);
}
