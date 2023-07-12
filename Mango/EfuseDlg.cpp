// EfuseDlg.cpp: 实现文件
//

#include "pch.h"
#include "Mango.h"
#include "EfuseDlg.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CEfuseDlg 消息处理程序


BOOL CEfuseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// TODO: 在此添加额外的初始化代码
	// Style
	DWORD dwStyle = ::GetWindowLong(m_listCtrl.GetSafeHwnd(), GWL_STYLE);
	dwStyle |= LVS_SINGLESEL;       //只可单行选中
	dwStyle |= LVS_SHOWSELALWAYS;   //Always show selection
	dwStyle |= LVS_REPORT; //报告风格
	::SetWindowLong(m_listCtrl.GetSafeHwnd(), GWL_STYLE, dwStyle);

	// Extended Style
	DWORD dwStyleEx = m_listCtrl.GetExtendedStyle();
	dwStyleEx |= LVS_EX_GRIDLINES;        //网格线
	dwStyleEx |= LVS_EX_FULLROWSELECT;    //整行高亮
	dwStyleEx |= LVS_EX_CHECKBOXES;       //Item前生成check box
	m_listCtrl.SetExtendedStyle(dwStyleEx);

	// 插入列
	m_listCtrl.InsertColumn(0, TEXT("姓名"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, TEXT("年龄"), LVCFMT_LEFT, 60);
	m_listCtrl.InsertColumn(2, TEXT("性别"), LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(3, TEXT("上衣颜色"), LVCFMT_LEFT, 100);

	//
	int nItem = 0;
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("张扬"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("26"));   //子项从1开始,0代表主项
	m_listCtrl.SetItemText(nItem, 2, TEXT("男"));
	m_listCtrl.SetItemText(nItem, 3, TEXT("0xFF0000"));//255 数值 代表背景的颜色值
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("王艳"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("24"));
	m_listCtrl.SetItemText(nItem, 2, TEXT("女"));
	m_listCtrl.SetItemText(nItem, 3, TEXT("0x163172"));//255*256 数值 代表背景的颜色值
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("李娜"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("24"));
	m_listCtrl.SetItemText(nItem, 2, TEXT("女"));
	m_listCtrl.SetItemText(nItem, 3, TEXT("0xFF00FF"));//255+255*256 数值 代表背景的颜色值

	//这些语句最好放到初始化的最后，至少要放到插入完列以后，否则需要传递列数
	m_listCtrl.column_type();//获得列数,并初始化 各列的默认类型,不同的类型见下面的赋值语句
	m_listCtrl.m_column_type[1] = 1;//第2列编辑框
	m_listCtrl.m_column_type[2] = 2;//第3列组合框
	m_listCtrl.m_column_type[3] = 3;//第4列背景变色框,文本的数值为背景的颜色数值

	m_listCtrl.column_string();//初始化各列的项目内容, 组合框列的项目要单独赋值,见下面语句
	m_listCtrl.m_column_string[2] = "男\n女\n";//第3列为性别,组合框的内容以换行符分割
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
