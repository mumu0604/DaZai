
// CMD_dazaiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CMD_dazai.h"
#include "CMD_dazaiDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCMD_dazaiDlg dialog



CCMD_dazaiDlg::CCMD_dazaiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCMD_dazaiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sizeChangeCnt = 0;
}

void CCMD_dazaiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_CMD_tab);
}

BEGIN_MESSAGE_MAP(CCMD_dazaiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CCMD_dazaiDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCMD_dazaiDlg message handlers

BOOL CCMD_dazaiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_CMD_tab.InsertItem(0, _T("运行控制"));
	m_CMD_tab.InsertItem(1, _T("实时工参"));

	//创建两个对话框
	m_CommandSheet.Create(IDD_DLGCOMMANDSHEET, &m_CMD_tab);
	m_RefreshSheet.Create(IDD_DLGREFRESHSHEET, &m_CMD_tab);
	//设定在Tab内显示的范围
	CRect rc;
	m_CMD_tab.GetClientRect(rc);
	rc.top += 25;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_CommandSheet.MoveWindow(&rc);
	m_RefreshSheet.MoveWindow(&rc);

	//把对话框对象指针保存起来
	pDialog[0] = &m_CommandSheet;
	pDialog[1] = &m_RefreshSheet;
	//显示初始页面
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	//保存当前选择
	m_CurSelTab = 0;
	
	get_control_original_proportion();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCMD_dazaiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCMD_dazaiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCMD_dazaiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCMD_dazaiDlg::get_control_original_proportion()
{
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild)
	{
		CRect rect;//获取当前窗口的大小
		control* tempcon = new control;
		CWnd* pWnd = GetDlgItem(::GetDlgCtrlID(hwndChild));//获取ID为woc的空间的句柄
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		tempcon->Id = ::GetDlgCtrlID(hwndChild);//获得控件的ID;
		tempcon->scale[0] = (double)rect.left / m_rect.Width();//注意类型转换，不然保存成long型就直接为0了
		tempcon->scale[1] = (double)rect.right / m_rect.Width();
		tempcon->scale[2] = (double)rect.top / m_rect.Height();
		tempcon->scale[3] = (double)rect.bottom / m_rect.Height();
		m_con_list.push_back(tempcon);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

void CCMD_dazaiDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = m_CMD_tab.GetCurSel();
	//把新的页面显示出来
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CCMD_dazaiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (nType == 1)
	{
		return;
	}
	else
	{
		CRect rect;//获取当前窗口的大小
		for (std::list<control*>::iterator it = m_con_list.begin(); it != m_con_list.end(); it++)
		{
			CWnd* pWnd = GetDlgItem((*it)->Id);//获取ID为woc的空间的句柄
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
			rect.left = (*it)->scale[0] * cx;
			rect.right = (*it)->scale[1] * cx;
			rect.top = (*it)->scale[2] * cy;
			rect.bottom = (*it)->scale[3] * cy;
			pWnd->MoveWindow(rect);//设置控件大小	
		}
	}
	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小
	if (m_sizeChangeCnt++ == 0){
		return;
	}
	CRect TabRect;
	m_CMD_tab.GetClientRect(&TabRect);
	TabRect.left += 1;
	TabRect.right -= 1;
	TabRect.top += 25;
	TabRect.bottom -= 1;	m_CommandSheet.OnSize(nType, cx, cy);
	m_RefreshSheet.OnSize(nType, cx, cy);
	switch (m_CMD_tab.GetCurSel())
	{
	case 0:
		m_CommandSheet.SetWindowPos(NULL, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_SHOWWINDOW);
		m_RefreshSheet.SetWindowPos(NULL, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 1:
		m_CommandSheet.SetWindowPos(NULL, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_HIDEWINDOW);
		m_RefreshSheet.SetWindowPos(NULL, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}	return;
}
