
// MangoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Mango.h"
#include "MangoDlg.h"
#include "afxdialogex.h"
#include "dbt.h"
#include "rlrelative.h"

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
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);

	DDX_Control(pDX, IDC_STATIC1, m_statusLabel1);
}

BEGIN_MESSAGE_MAP(CMangoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMangoDlg::OnSelchangeTab1)
	ON_MESSAGE(WM_DEVICECHANGE, &CMangoDlg::OnDevicechange)
END_MESSAGE_MAP()

CMangoDlg* pMangoDlg = NULL;
BOOL PrintStatus(CString str)
{
	if (pMangoDlg) {
		pMangoDlg->PrintStatus(str);
		return true;
	}
	return false;
}

void CMangoDlg::PrintStatus(CString str)
{
	m_statusLabel1.SetWindowText(str);
}
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
	//_CrtSetBreakAlloc(153);

	//_CrtDumpMemoryLeaks();
	pMangoDlg = this;
	AdapterRefresh();
	RegisterDevice(GetSafeHwnd());
	m_tabctrl.InsertItem(0, _T("Efuse"));      //添加参数一选项卡 
	m_tabctrl.InsertItem(1, _T("Cli"));      //添加参数二选项卡
	m_efuseDlg.Create(IDD_EFUSEDLG, &m_tabctrl);
	m_cliDlg.Create(IDD_CLIDLG, &m_tabctrl);
	//获得IDC_TABTEST客户区大小
	CRect rs;
	m_tabctrl.GetClientRect(&rs);

	//调整子对话框在父窗口中的位置
	rs.top += 30;
	rs.bottom -= 20;
	rs.left += 4;
	rs.right -= 4;
	//把对话框对象指针保存起来
	pDlg[0] = &m_efuseDlg;
	pDlg[1] = &m_cliDlg;
	//设置子对话框尺寸并移动到指定位置
	m_efuseDlg.MoveWindow(&rs);
	m_cliDlg.MoveWindow(&rs);
	//显示初始页面
	pDlg[0]->ShowWindow(SW_SHOW);
	pDlg[1]->ShowWindow(SW_HIDE);
	//设置默认的选项卡
	m_tabctrl.SetCurSel(0);
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





void CMangoDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	pDlg[m_curSelTab]->ShowWindow(SW_HIDE);
	m_curSelTab = m_tabctrl.GetCurSel();
	//
	pDlg[m_curSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

bool RegisterDevice(HANDLE hwnd)
{
	const GUID GUID_DEVINTERFACE_LIST[] = {
		{ 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
		//{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
		//{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }, /* HID */
		//GUID_NDIS_LAN_CLASS
		//{0XAD498944, 0X762F, 0X11D0, { 0X8D,0XCB,0X00,0X00,0X4F,0XC3,0X35,0X8C }}
	};

	int i;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for (int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST) / sizeof(GUID); i++)
	{

		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		HDEVNOTIFY hDevNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if (!hDevNotify)
		{
			int Err = GetLastError();
			LOGD(_T("Register Device Notification Failed\n"));
		}

	}

	return true;
}

afx_msg LRESULT CMangoDlg::OnDevicechange(WPARAM wParam, LPARAM lParam)
{
	if ((DWORD)wParam == DBT_DEVNODES_CHANGED) {
		//printf("dev nodes change LPARAM = %x\n", lp);
	}
	if ((DWORD)wParam == DBT_DEVICEARRIVAL) {
		DEV_BROADCAST_DEVICEINTERFACE* p = (DEV_BROADCAST_DEVICEINTERFACE*)lParam;
		if (p->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
			LOGD(_T("有新网卡插入\n"));
			Sleep(1000);
			AdapterRefresh();
		}

	}
	else if ((DWORD)wParam == DBT_DEVICEREMOVECOMPLETE) {
		DEV_BROADCAST_DEVICEINTERFACE* p = (DEV_BROADCAST_DEVICEINTERFACE*)lParam;
		if (p->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
			LOGD(_T("啊……网卡被拔掉了\n"));
			PrintStatus(_T("未检测到设备"));
		}
	}
	return 0;
}
