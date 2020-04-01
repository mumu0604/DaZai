// DlgRefreshSheet.cpp : implementation file
//

#include "stdafx.h"
#include "CMD_dazai.h"
#include "DlgRefreshSheet.h"
#include "afxdialogex.h"


// CDlgRefreshSheet dialog

IMPLEMENT_DYNAMIC(CDlgRefreshSheet, CDialogEx)

CDlgRefreshSheet::CDlgRefreshSheet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRefreshSheet::IDD, pParent)
{

}

CDlgRefreshSheet::~CDlgRefreshSheet()
{
}

void CDlgRefreshSheet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REFRESH, m_ListRefresh);
}


BEGIN_MESSAGE_MAP(CDlgRefreshSheet, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgRefreshSheet message handlers


void CDlgRefreshSheet::OnSize(UINT nType, int cx, int cy)
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
}
void CDlgRefreshSheet::get_control_original_proportion()
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

BOOL CDlgRefreshSheet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LONG lStyle;
	lStyle = GetWindowLong(m_ListRefresh.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_ListRefresh.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = dwStyle = m_ListRefresh.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//	dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	m_ListRefresh.SetExtendedStyle(dwStyle); //设置扩展风格

	LV_COLUMN lvc[10];
	int i;
	for (i = 0; i < 10; i++)
	{
		lvc[i].mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	}
	lvc[0].iSubItem = 0;
	lvc[0].pszText = "序号";
	lvc[0].cx = 80;

	lvc[1].iSubItem = 0;
	lvc[1].pszText = "参数名称";
	lvc[1].cx = 200;

	lvc[2].iSubItem = 0;
	lvc[2].pszText = "功能描述";
	lvc[2].cx = 200;

	lvc[3].iSubItem = 0;
	lvc[3].pszText = "参数值";
	lvc[3].cx = 200;

	lvc[4].iSubItem = 0;
	lvc[4].pszText = "";
	lvc[4].cx = 80;

	lvc[5].iSubItem = 0;
	lvc[5].pszText = "序号";
	lvc[5].cx = 80;

	lvc[6].iSubItem = 0;
	lvc[6].pszText = "参数名称";
	lvc[6].cx = 200;

	lvc[7].iSubItem = 0;
	lvc[7].pszText = "功能描述";
	lvc[7].cx = 200;

	lvc[8].iSubItem = 0;
	lvc[8].pszText = "参数值";
	lvc[8].cx = 200;
	// 
	// 	lvc[9].iSubItem = 0;
	// 	lvc[9].pszText = "";
	// 	lvc[9].cx = 300;

	m_ListRefresh.DeleteAllItems();

	for (i = 0; i < 9; i++)
	{
		m_ListRefresh.InsertColumn(i, &lvc[i]);
	}


	get_control_original_proportion();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
