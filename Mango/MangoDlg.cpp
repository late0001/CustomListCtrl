
// MangoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Mango.h"
#include "MangoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMangoDlg 对话框



CMangoDlg::CMangoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MANGO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMangoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CMangoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CMangoDlg::OnCustomdrawList1)
END_MESSAGE_MAP()


// CMangoDlg 消息处理程序

BOOL CMangoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

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
	m_listCtrl.InsertColumn(3, TEXT("上衣颜色"), LVCFMT_LEFT, 80);

	//
	int nItem = 0;
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("张扬"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("26"));   //子项从1开始,0代表主项
	m_listCtrl.SetItemText(nItem, 2, TEXT("男"));
	m_listCtrl.SetItemText(nItem, 3, TEXT("255"));//255 数值 代表背景的颜色值
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("王艳"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("24"));
	m_listCtrl.SetItemText(nItem, 2, TEXT("女"));
	m_listCtrl.SetItemText(nItem, 3, TEXT("65280"));//255*256 数值 代表背景的颜色值
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), TEXT("李娜"), 0);
	m_listCtrl.SetItemText(nItem, 1, TEXT("24"));
	m_listCtrl.SetItemText(nItem, 2, TEXT("女"));
	m_listCtrl.SetItemText(nItem, 3, TEXT("65535"));//255+255*256 数值 代表背景的颜色值

	//这些语句最好放到初始化的最后，至少要放到插入完列以后，否则需要传递列数
	m_listCtrl.column_type();//获得列数,并初始化 各列的默认类型,不同的类型见下面的赋值语句
	m_listCtrl.m_column_type[1] = 1;//第2列编辑框
	m_listCtrl.m_column_type[2] = 2;//第3列组合框
	m_listCtrl.m_column_type[3] = 3;//第4列背景变色框,文本的数值为背景的颜色数值

	m_listCtrl.column_string();//初始化各列的项目内容, 组合框列的项目要单独赋值,见下面语句
	m_listCtrl.m_column_string[2] = "男\n女\n";//第3列为性别,组合框的内容以换行符分割

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMangoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMangoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMangoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMangoDlg::OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
	m_listCtrl.OnCustomdrawMyList(pNMHDR, pResult);
	
}
