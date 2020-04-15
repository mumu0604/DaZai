#pragma once

#include <list>
#include "afxcmn.h"
#include "Xml.h"	// Added by ClassView
#include "Common.h"
#include "DlgCANCOFIG.h"
#include "ListCtrlCl.h"
#include "afxwin.h"
#include "TeleDisplay.h"
#include "Interface.h"
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
	CXML m_xmlMonitor;
	int m_iRealCmdCnt;
	CMD_WN m_cmdAddInfo[MAXCOMMAND];
	CmdInfo *m_pCmdInfo[256];
	CmdInfo *m_pMonitorInfo[256];
	map<CString, int> m_mappackType;
//	void ExtractArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length);
	xmlXPathObjectPtr LocateXPath(char xpath_expr[]);
	xmlXPathObjectPtr LocateCommand(unsigned char dev_id, unsigned char cmd_id);
	void GetCmdInfo(CmdInfo *m_pCmdInfo[256]);
	void GetMonitorxmlInfo(CmdInfo *m_pCmdInfo[256]);
//	void InsertArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length);
	afx_msg void OnNMDblclkListControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCanpara();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetCurrentTimer();
	SYSTEMTIME m_GPSTimeNowday;
	CMenu m_menu;
	unsigned char m_bus_flag;
	ULARGE_INTEGER m_base_relative;
	afx_msg void OnNMRClickListControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCmdSend();
	afx_msg void OnCmdMoveUp();
	afx_msg void OnCmdMoveDown();
	afx_msg void OnCmdInsert();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdDeleteall();
	afx_msg void OnBnClickedButtonCmdInput();
	afx_msg void OnBnClickedButtonOutput();
	void AddCmdToList(CMD_WN *pCmd, int index, int bNew);
	CString GetAbsTime(__int64 sec);
	void ShowInfo(CString str);
	CString m_StrCMDsend;
	CEdit m_pEditCmdSend;
	afx_msg void OnBnClickedButtonCmdsend();
	int m_editCmdSendCnt;
	int m_editCmdReactCnt;
	CListCtrlCl m_ListTeleOutput;
	static UINT ReceiveThread(void *param);
	int m_MonitorCmdNum;
	CTeleDisplay m_CTeleDisplay;
	CInterface *m_pInterface;
	CComboBox m_ComboBoxPackType;
};
