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
#include "Ctelemetry.h"
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
	CmdInfo *m_pCANcmdInfo[256];
	CmdInfo *m_pLVDScmdInfo[256];
	map<CString, int> m_mappackType;

	xmlXPathObjectPtr LocateXPath(char xpath_expr[]);
	xmlXPathObjectPtr LocateCommand(unsigned char dev_id, unsigned char cmd_id);
	void GetCmdInfo(CmdInfo *m_pCmdInfo[256]);
	int GetMonitorxmlInfo(CmdInfo *m_pCmdInfo[256]);
//	void InsertArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length);
	afx_msg void OnNMDblclkListControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCanpara();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetCurrentTimer();
	bool GetCMDlist(CMDbuf *cmdbuf);
	bool GetCMDsingle(CMDbuf *cmdbuf);
	void Getinjectionpara(InjectionInfo *Injectionpara);
	int getcmdinjectiondata(CMDbuf cmdbuf, char *InjectionBuffer, char *dataFramBuffer);
	void SaveToPLD(CFile *pldFile);
	void LoadFromPLD(CString fileName);
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
	int m_CANcmdNum;
	int m_LVDScmdNum;
	CTeleDisplay m_CTeleDisplay;
	Ctelemetry m_Ctelemetry;
	CInterface *m_pInterface;
	CComboBox m_ComboBoxPackType;
	CComboBox m_ComboCancmd;
	CComboBox m_ComboLVDScmd;
// 	map<CString, int>m_mapComboCancmd;
// 	map<CString, int>m_mapComboLVDScmd;
	afx_msg void OnBnClickedButtonCansend();
	afx_msg void OnBnClickedButtonCaninjectfile();
	afx_msg void OnBnClickedButtonCaninfile2();
	CString m_editCANinjectdir;
	CString m_editLVDSinjectdir;
	BOOL m_checkCANinjection;
	BOOL m_checkCANimmedieate;
	BOOL m_checkLVDSinjection;
	BOOL m_checkLVDSimmedieate;
	afx_msg void OnBnClickedButtonlvdsinjectsend();
	afx_msg void OnBnClickedButtonLvdsinfile();
	afx_msg void OnBnClickedCheckCaninjection();
	afx_msg void OnBnClickedCheckCanimmedieate();
	afx_msg void OnBnClickedCheckLvdsinjection();
	afx_msg void OnBnClickedCheckLvdsimmedieate();
	int m_EditclientCode;
	int m_EdittaskCode;
	int m_Editstarttime;
	short m_Edittimelength;
	short m_Editoffsettime0;
	short m_Editoffsettime1;
	short m_Editoffsettime2;
	short m_Editoffsettime3;
	short m_Editdataaddr;
	BOOL m_check_istaskdata;
	afx_msg void OnBnClickedCheckIstaskdata();
	afx_msg void OnBnClickedButtonTaskdatadir();
	CString m_EditTaskInjectionDatafiel;
	BOOL m_checkSinglecmdlvds;
	BOOL m_checkSinglecmdcan;
	afx_msg void OnBnClickedButtonSingelcmdsedn();
	BOOL m_checkCANtele;
	BOOL m_checkLVDStele;
	afx_msg void OnBnClickedCheckCantele();
	afx_msg void OnBnClickedCheckLvdstele();
};
