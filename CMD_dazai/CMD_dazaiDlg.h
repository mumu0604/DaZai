
// CMD_dazaiDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "DlgCommandSheet.h"
#include "DlgRefreshSheet.h"
#include <list>
using namespace std;

// CCMD_dazaiDlg dialog
class CCMD_dazaiDlg : public CDialogEx
{
// Construction
public:
	CCMD_dazaiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CMD_DAZAI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CTabCtrl m_CMD_tab;
	int m_CurSelTab;
	CDlgCommandSheet m_CommandSheet;
	CDlgRefreshSheet m_RefreshSheet;
	CDialog* pDialog[2];
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	void get_control_original_proportion();
	typedef struct Rect
	{
	public:
		int Id;
		double scale[4];
	}control;
	CRect m_rect;
	list<control*> m_con_list;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_sizeChangeCnt;
};
