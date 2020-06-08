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
#include "DlgRefreshSheet.h"
#include "ReplayConfig.h"
#include "DlgPlotSheet.h"
using namespace std;
// CDlgCommandSheet dialog


#define TIMERBROADCAST				3			//时间码发送
#define TIMERCANSENTELE				1			//CAN TELE
#define TIEMRLVDSSENDTELE			2			//LVDS TELE
#define TIEMRCOMMANDTIEM			0			//LVDS TELE
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
	CDlgCANCOFIG m_dlgCanConfig;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlCommand;
	CXML m_xml;
	CXML m_xmlMonitor;
	int m_iRealCmdCnt;
	CMD_WN m_cmdAddInfo[MAXCOMMAND];
	CmdInfo *m_pCmdInfo[100];
	CmdInfo *m_pMonitorInfo[MONITORCMDNUM];
	CmdInfo *m_pCANcmdInfo[MONITORCMDNUM];
	CmdInfo *m_pLVDScmdInfo[MONITORCMDNUM];
	map<CString, int> m_mappackType;

	xmlXPathObjectPtr LocateXPath(char xpath_expr[]);
	xmlXPathObjectPtr LocateCommand(unsigned char dev_id, unsigned char cmd_id);
	void GetCmdInfo(CmdInfo *m_pCmdInfo[100]);
	int GetMonitorxmlInfo(CmdInfo *m_pCmdInfo[MONITORCMDNUM]);
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
	void Setteleinitvalue(RecvScanBuf telebuf);
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
	void ExtractArgValue1(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length);
	void ShowInfo(CString str);
	CString m_StrCMDsend;
	CEdit m_pEditCmdSend;
	CEdit m_Edit_reportOutput;
	afx_msg void OnBnClickedButtonCmdsend();
	CListCtrl m_ListTeleOutput;
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
	BOOL m_iscandatasend;//can数据正在发送
	BOOL m_islvdsdatasend;//lvds数据正在发送
	CDlgRefreshSheet *m_dlgRefreshsheet;
	CReplayConfig *m_CReplayConfig;
	CDlgPlotSheet *m_dlgPlotSheet;
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
	void setRefreshSheet(CDlgRefreshSheet *Refreshsheet, CReplayConfig *ReplayConfig, CDlgPlotSheet *PlotSheet);
	afx_msg void OnBnClickedButtonCurrenttime();
	int m_edit_TaskT0Time;
	
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClearstrSend();
	afx_msg void OnBnClickedButtonClearstrRecv();
	CListBox m_ListBoxSendoutput;
	CListBox m_ListBoxRecvoutput;
};
