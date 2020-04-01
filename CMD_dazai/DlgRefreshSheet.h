#pragma once
#include "ListCtrlCl.h"
#include <list>
#include "afxcmn.h"
using namespace std;
// CDlgRefreshSheet dialog

class CDlgRefreshSheet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRefreshSheet)

public:
	CDlgRefreshSheet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRefreshSheet();

// Dialog Data
	enum { IDD = IDD_DLGREFRESHSHEET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void get_control_original_proportion();
	typedef struct Rect
	{
	public:
		int Id;
		double scale[4];
	}control;
	CRect m_rect;
	list<control*> m_con_list;
	virtual BOOL OnInitDialog();
	CListCtrlCl m_ListRefresh;
};
