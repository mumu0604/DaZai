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
		CRect rect;//��ȡ��ǰ���ڵĴ�С
		for (std::list<control*>::iterator it = m_con_list.begin(); it != m_con_list.end(); it++)
		{
			CWnd* pWnd = GetDlgItem((*it)->Id);//��ȡIDΪwoc�Ŀռ�ľ��
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
			rect.left = (*it)->scale[0] * cx;
			rect.right = (*it)->scale[1] * cx;
			rect.top = (*it)->scale[2] * cy;
			rect.bottom = (*it)->scale[3] * cy;
			pWnd->MoveWindow(rect);//���ÿؼ���С	
		}
	}
	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С
}
void CDlgRefreshSheet::get_control_original_proportion()
{
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild)
	{
		CRect rect;//��ȡ��ǰ���ڵĴ�С
		control* tempcon = new control;
		CWnd* pWnd = GetDlgItem(::GetDlgCtrlID(hwndChild));//��ȡIDΪwoc�Ŀռ�ľ��
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		tempcon->Id = ::GetDlgCtrlID(hwndChild);//��ÿؼ���ID;
		tempcon->scale[0] = (double)rect.left / m_rect.Width();//ע������ת������Ȼ�����long�;�ֱ��Ϊ0��
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
	m_CTeleDisplay.Open(&m_ListRefresh,TELEREFRESHSHEET);

	get_control_original_proportion();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
