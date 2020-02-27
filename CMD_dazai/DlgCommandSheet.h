#pragma once

#include <list>
#include "afxcmn.h"
#include "Xml.h"	// Added by ClassView
#include "Common.h"
#include "DlgCANCOFIG.h"

using namespace std;
// CDlgCommandSheet dialog

class CDlgCommandSheet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommandSheet)

public:
	CDlgCommandSheet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCommandSheet();

// Dialog Data
	enum { IDD = IDD_DLGCOMMANDSHEET };

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
	CDlgCANCOFIG dlgCanConfig;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlCommand;
	CXML m_xml;
	int m_iRealCmdCnt;
	CMD_WN m_cmdAddInfo[MAXCOMMAND];
	CmdInfo *m_pCmdInfo[256];
	void ExtractArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length);
	xmlXPathObjectPtr LocateXPath(char xpath_expr[]);
	void GetCmdInfo(CmdInfo *m_pCmdInfo[256]);
	void InsertArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length);
	afx_msg void OnNMDblclkListControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCanpara();
};
