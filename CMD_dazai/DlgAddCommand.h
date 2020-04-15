#pragma once
#include <list>
#include "DlgCommandSheet.h"
#include "Common.h"
#include "afxcmn.h"
#include "Xml.h"
#include "libxml/parser.h"
#include "libxml/xmlmemory.h"
#include "DynEdit.h"
#include "DynComboBox.h"
#include <vector>
#include "afxdtctl.h"
using namespace std;
//class CDlgCommandSheet;
// CDlgAddCommand dialog
enum{
	CANVAS_LEFTTOPx = 250,
	CANVAS_LEFTTOPy = 40,
	CANVAS_WIDTH = 700,
	CANVAS_HEIGHT = 500,
	LINEHEIGHT = 25,
	TABHEIGHT = 3,
	COMBOBOXWIDTH = 300,
	COMBOBOXHEIGHT = 500,
	TEXTWIDTH = 300,
	EDITWIDTH = 300,
};
class CDlgAddCommand : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddCommand)

public:
	CDlgAddCommand(CDlgCommandSheet *pDlg, CMD_WN *pCmd, int bNew, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddCommand();

// Dialog Data
	enum { IDD = IDD_DLGADDCOMMAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	std::vector<CDynEdit*> m_vecEdit;
	std::vector<CDynComboBox*> m_vecCombobox;
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
	CListCtrl m_ListAddCMD;
	CXML m_xml;
	CMD_WN* m_pCmd_WN;
	CDlgCommandSheet *m_pDlg;

//	CDlgCommandSheet *m_pDlg;
	afx_msg void OnNMDblclkListAddcmd(NMHDR *pNMHDR, LRESULT *pResult);
	void SetValueToUI(unsigned char cmd_id, unsigned char *pArgValue);
	void DeleteEditAndCombo();
	CDynComboBox* CreateComboBox(int id, int width, xmlNodePtr pNode);
	CDynEdit* CreatMyEdit(int id, int width, CString text, bool canEdit, int bitLength, double coef);
	void InitUI(unsigned char *pCmd);
	CDateTimeCtrl m_timePicker;
	afx_msg void OnBnClickedOk();
	BOOL m_CheckTimeFlag;
};
