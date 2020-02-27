// DlgAddCommand.cpp : implementation file
//

#include "stdafx.h"
#include "CMD_dazai.h"
#include "DlgAddCommand.h"
#include "afxdialogex.h"


// CDlgAddCommand dialog

IMPLEMENT_DYNAMIC(CDlgAddCommand, CDialogEx)

CDlgAddCommand::CDlgAddCommand(CDlgCommandSheet *pDlg, CMD_WN *pCmd, int bNew, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAddCommand::IDD, pParent)
{
	m_pDlg = pDlg;
	m_pCmd_WN = pCmd;
}

CDlgAddCommand::~CDlgAddCommand()
{
}

void CDlgAddCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDCMD, m_ListAddCMD);
}


BEGIN_MESSAGE_MAP(CDlgAddCommand, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADDCMD, &CDlgAddCommand::OnNMDblclkListAddcmd)
END_MESSAGE_MAP()


// CDlgAddCommand message handlers


void CDlgAddCommand::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
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
	// TODO: Add your message handler code here
}
void CDlgAddCommand::get_control_original_proportion()
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

BOOL CDlgAddCommand::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_xml.Open("commands.xml");
	UpdateData(FALSE);
	xmlXPathObjectPtr xpathObj;

	//	xpathObj = m_xml.GetNodesByXPath(BAD_CAST("//Command[@name]"));
//	xpathObj = m_pDlg->LocateCommand(0x33, 0);


	LONG lStyle = m_ListAddCMD.SendMessage
		(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
		LVS_EX_HEADERDRAGDROP;
	m_ListAddCMD.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		(LPARAM)lStyle);
	CFont font;
	font.CreatePointFont(150, _T("宋体"));
	m_ListAddCMD.SetFont(&font);

	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH /*| LVCF_FMT*/;
	lvc.iSubItem = 0;
	lvc.pszText = (char *)"指令";
	lvc.cx = 200;
	m_ListAddCMD.InsertColumn(0, &lvc);
	xpathObj = LocateCommand(0x33, 0);
	int iCmdIdx = 0;
	xmlChar *xmlRtn;
	char *endptr;
	if (xpathObj){
		for (iCmdIdx = 0; iCmdIdx < xpathObj->nodesetval->nodeNr; iCmdIdx++){
			//			m_ListCtrlInCommand.InsertItem(iCmdIdx, (char *)CXML::xmlGetPropGBK(xpathObj->nodesetval->nodeTab[iCmdIdx], BAD_CAST("name")));
			m_ListAddCMD.InsertItem(iCmdIdx, (char *)CXML::xmlGetPropGBK(xpathObj->nodesetval->nodeTab[iCmdIdx], BAD_CAST("CommandName")));
			xmlRtn = xmlGetProp(xpathObj->nodesetval->nodeTab[iCmdIdx], (xmlChar *)"CommandCode");
			int rec = m_ListAddCMD.SetItemData(iCmdIdx, (unsigned char)(strtol((char *)xmlRtn, &endptr, 16) & 0xFF));
		}
		xmlXPathFreeObject(xpathObj);
	}
	get_control_original_proportion();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAddCommand::OnNMDblclkListAddcmd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int iCmdIdx = m_ListAddCMD.GetSelectionMark();//获取选中的位置

	xmlXPathObjectPtr xpathObj;
	xmlNodePtr pNode;
	xmlChar *xmlRtn;
	char *endptr;

	char xpath_expr[200];


	CString strCmdName;

	if (iCmdIdx > -1)//
	{
		DeleteEditAndCombo();
		m_rect.SetRect(CANVAS_LEFTTOPx, CANVAS_LEFTTOPy, CANVAS_LEFTTOPx + CANVAS_WIDTH, CANVAS_LEFTTOPy + LINEHEIGHT);
		m_pCmd_WN->cmd_id = m_ListAddCMD.GetItemData(iCmdIdx);
		m_pCmd_WN->addCntIdx = m_pCmd_WN->cmd_id;
		InitUI((unsigned char *)(LPCSTR)(strCmdName));//根据strCmdName从command.xml 解析出指令内容
		SetValueToUI(m_pCmd_WN->cmd_id, m_pDlg->m_pCmdInfo[m_pCmd_WN->cmd_id & 0xFF]->init_value);
	}
	*pResult = 0;
}
xmlXPathObjectPtr CDlgAddCommand::LocateXPath(char xpath_expr[])
{
	xmlXPathObjectPtr xpathObj = NULL;
	xpathObj = m_xml.GetNodesByXPath(BAD_CAST(xpath_expr));
	if (xpathObj){
		if (xpathObj->nodesetval){
			if (xpathObj->nodesetval->nodeNr > 0){
				return xpathObj;
			}
		}
		xmlXPathFreeObject(xpathObj);
	}
	return NULL;

}
xmlXPathObjectPtr CDlgAddCommand::LocateCommand(unsigned char dev_id, unsigned char cmd_id)
{
	char xpath_expr[200];
	xmlXPathObjectPtr xpathObj = NULL;

	if (cmd_id){
		sprintf(xpath_expr, "/CommandSet/Category/Device[translate(@DeviceID,'abcdef','ABCDEF')='%02X']/parent::*/Command[translate(@CommandCode,'abcdef','ABCDEF')='%02X']\0", dev_id, cmd_id);
	}
	else{
		sprintf(xpath_expr, "/CommandSet/Category/Device[translate(@DeviceID,'abcdef','ABCDEF')='%02X']/parent::*/Command[@CommandCode]\0", dev_id);
	}
	return LocateXPath(xpath_expr);
}
void CDlgAddCommand::SetValueToUI(unsigned char cmd_id, unsigned char *pArgValue)
{

	int i = 0, j;

	CmdInfo *pCmdInfo;
	unsigned char temp[MAX_ARG_LENGTH];
	CString strBuf, strArg;

	pCmdInfo = m_pDlg->m_pCmdInfo[cmd_id & 0xFF];

	if (!pCmdInfo){
		return;
	}

	for (i = 0; i < pCmdInfo->arg_num; i++){
		m_pDlg->ExtractArgValue(temp, pArgValue, pCmdInfo->bit_start[i], pCmdInfo->arg_length[i]);
		strArg = "";
		for (j = 0; j < pCmdInfo->arg_length[i] / 8; j++){
			strBuf.Format("%02X", temp[j]);
			strArg += strBuf;
		}
		if (pCmdInfo->arg_length[i] & 0x7){
			if (pCmdInfo->input_type[i] == 0) {
				temp[j] >>= (8 - (pCmdInfo->arg_length[i] & 0x7));
			}
			strBuf.Format("%02X", temp[j]);
			strArg += strBuf;
		}
		if (pCmdInfo->input_type[i] == 1){
			m_vecEdit[pCmdInfo->input_ctrl_index[i]]->GetWindowText(strBuf);
			if (strBuf == ""){
				m_vecEdit[pCmdInfo->input_ctrl_index[i]]->SetWindowText(strArg);
			}
		}
		else{
			for (j = 0; j < m_vecCombobox[pCmdInfo->input_ctrl_index[i]]->GetCount(); j++)
			{
				m_vecCombobox[pCmdInfo->input_ctrl_index[i]]->GetLBText(j, strBuf);
				if (strBuf.Find(strArg) == 2)
				{
					m_vecCombobox[pCmdInfo->input_ctrl_index[i]]->SetCurSel(j);
					break;
				}
			}
		}
	}
}

void CDlgAddCommand::InitUI(unsigned char *pCmd)
{
	CString strArgName;

	int i = 0;

	CmdInfo *pCmdInfo;

	xmlXPathObjectPtr xpathObj;
	char xpath_expr[200];

	pCmdInfo = m_pDlg->m_pCmdInfo[m_pCmd_WN->cmd_id & 0xFF];
	if (!pCmdInfo){
		return;
	}

	GetDlgItem(IDC_EDITCMDNAME)->SetWindowText((char *)pCmdInfo->cmd_name);


	if (pCmdInfo->arg_num < 24){
		m_rect.top = CANVAS_LEFTTOPy + (24 - pCmdInfo->arg_num) * 5;
		m_rect.bottom = m_rect.top + LINEHEIGHT;
	}


	for (i = 0; i < pCmdInfo->arg_num; i++){
		CString strBuf;
		strBuf.Format("%4d", pCmdInfo->arg_length[i]);
		strBuf += "bits";
		strArgName = pCmdInfo->arg_name[i];
		CreatMyEdit(0, TEXTWIDTH, strArgName, false, 10, 0);
		if (pCmdInfo->input_type[i] == 0)
		{
			sprintf(xpath_expr, "/CommandSet/Category/Command[translate(@CommandCode,'abcdef','ABCDEF')='%02X']/Arg", m_pCmd_WN->cmd_id);
			xpathObj = m_pDlg->LocateXPath(xpath_expr);
			if (xpathObj){
				if (i < xpathObj->nodesetval->nodeNr){
					CreateComboBox(i, COMBOBOXWIDTH, xpathObj->nodesetval->nodeTab[i]);
				}
				xmlXPathFreeObject(xpathObj);
			}
		}
		else//输入的createEdit
		{
			CreatMyEdit(i, EDITWIDTH, "", true, pCmdInfo->arg_length[i], pCmdInfo->coef[i]);
		}
		CreatMyEdit(0, 60, strBuf, false, 10, 0);
		m_rect.left = CANVAS_LEFTTOPx;
		m_rect.top += LINEHEIGHT + TABHEIGHT;
		m_rect.bottom = m_rect.top + LINEHEIGHT;
	}
	return;
}
void CDlgAddCommand::DeleteEditAndCombo()//清空edit和combo
{
	CComboBox *combo;
	int n = m_vecCombobox.size();
	for (int i = 0; i < n; i++)
	{
		combo = m_vecCombobox[i];
		combo->DestroyWindow();
		delete combo;
	}
	m_vecCombobox.clear();

	CEdit* edt;
	n = m_vecEdit.size();
	for (int i = 0; i < n; i++)
	{
		edt = m_vecEdit[i];
		edt->DestroyWindow();
		delete edt;
	}
	m_vecEdit.clear();
}
CDynEdit* CDlgAddCommand::CreatMyEdit(int id, int width, CString text, bool canEdit, int bitLength, double coef)
{

	CRect rect(m_rect);
	if (width < m_rect.Width())
	{
		rect.right = rect.left + width;
		m_rect.left += width;
	}
	else//换行
	{
		m_rect.left = CANVAS_LEFTTOPx;
		m_rect.top += LINEHEIGHT;
		m_rect.bottom += LINEHEIGHT;

		rect.left = CANVAS_LEFTTOPx;
		rect.right = rect.left + width;
		rect.top = rect.top + LINEHEIGHT;
		rect.bottom = rect.top + LINEHEIGHT;
	}

	CDynEdit *pEdit = new CDynEdit(id, bitLength, coef);
	if (canEdit){
		pEdit->Create(WS_BORDER | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_CENTER | ES_UPPERCASE, rect, this, 101);
	}
	else{
		rect.top += 5;
		rect.bottom += 5;
		rect.right -= 5;
		pEdit->Create(ES_READONLY | WS_CHILD | WS_VISIBLE | ES_RIGHT, rect, this, 101);
	}
	pEdit->SetLimitText((bitLength + 7) / 8 * 2);
	pEdit->ShowWindow(SW_SHOW);
	pEdit->SetWindowText(text);
	m_vecEdit.push_back(pEdit);
	return pEdit;
}
CDynComboBox* CDlgAddCommand::CreateComboBox(int id, int width, xmlNodePtr pNode)
{
	CRect rect(m_rect);
	if (width < m_rect.Width())
	{
		rect.right = rect.left + width;
		m_rect.left += width;
	}
	else//换行
	{
		m_rect.left = CANVAS_LEFTTOPx;
		m_rect.top += LINEHEIGHT;
		m_rect.bottom += LINEHEIGHT;

		rect.left = CANVAS_LEFTTOPx;
		rect.right = rect.left + width;
		rect.top = rect.top + LINEHEIGHT;
		rect.bottom = rect.top + LINEHEIGHT;
	}
	rect.bottom += COMBOBOXHEIGHT;
	CDynComboBox *pComboBox = new CDynComboBox(id);
	pComboBox->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, rect, this, 102);

	xmlNodePtr child;
	xmlChar *xmlRtn;
	CString strCode;

	child = pNode->xmlChildrenNode;
	while (child != NULL)
	{
		if (!xmlStrcmp(child->name, (xmlChar *)"ArgValue")){
			xmlRtn = xmlNodeGetContent(child);
			strCode = xmlRtn;
			strCode.MakeUpper();
			strCode.Insert(0, "0x");
			strCode += "    ";
			xmlFree(xmlRtn);
			xmlRtn = CXML::xmlGetPropGBK(child, (xmlChar *)"ArgumentValueName");
			strCode += (CString)xmlRtn;
			xmlFree(xmlRtn);
			pComboBox->AddString(strCode);
		}
		child = child->next;
	}
	pComboBox->SetCurSel(0);
	m_vecCombobox.push_back(pComboBox);
	return pComboBox;
}
