// DlgCommandSheet.cpp : implementation file
//

#include "stdafx.h"
#include "CMD_dazai.h"
#include "DlgCommandSheet.h"
#include "afxdialogex.h"
#include "DlgAddCommand.h"
#include "TeleDisplay.h"
#include <sys/types.h>    
#include <sys/stat.h>  
#include <ctime>
IMPLEMENT_DYNAMIC(CDlgCommandSheet, CDialogEx)

CDlgCommandSheet::CDlgCommandSheet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommandSheet::IDD, pParent)
	, m_editCANinjectdir(_T(""))
	, m_editLVDSinjectdir(_T(""))
	, m_checkCANinjection(FALSE)
	, m_checkCANimmedieate(FALSE)
	, m_checkLVDSinjection(FALSE)
	, m_checkLVDSimmedieate(FALSE)
	, m_EditclientCode(0)
	, m_EdittaskCode(0)
	, m_Editstarttime(0)
	, m_Edittimelength(5)
	, m_Editoffsettime0(1)
	, m_Editoffsettime1(1)
	, m_Editoffsettime2(1)
	, m_Editoffsettime3(1)
	, m_Editdataaddr(0)
	, m_check_istaskdata(FALSE)
	, m_EditTaskInjectionDatafiel(_T(""))
	, m_checkSinglecmdlvds(FALSE)
	, m_checkSinglecmdcan(TRUE)
	, m_checkCANtele(FALSE)
	, m_checkLVDStele(FALSE)
	, m_edit_TaskT0Time(0)
{
	m_iRealCmdCnt = 0;
	m_mappackType["添加业务任务"] = 0x40;
	m_mappackType["添加业务指令"] = 0x41;
	m_mappackType["添加重构任务"] = 0x43;
	m_mappackType["添加测试任务"] = 0x47;
	m_mappackType["删除业务任务"] = 0x80;
	m_mappackType["删除重构任务"] = 0x83;
	m_mappackType["删除测试任务"] = 0x87;
	m_iscandatasend = FALSE;
	m_islvdsdatasend = FALSE;
}

CDlgCommandSheet::~CDlgCommandSheet()
{
}

void CDlgCommandSheet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTROL, m_ListCtrlCommand);
	DDX_Control(pDX, IDC_EDIT_CMDSEND_OUTPUT, m_pEditCmdSend);
	DDX_Control(pDX, IDC_LIST_TELEOUTPUT, m_ListTeleOutput);
	DDX_Control(pDX, IDC_COMBO_PACKTYPE, m_ComboBoxPackType);
	DDX_Control(pDX, IDC_COMBO_CANCMD, m_ComboCancmd);
	DDX_Control(pDX, IDC_COMBO_LVDSCMD, m_ComboLVDScmd);
	DDX_Text(pDX, IDC_EDIT_CANFILEDIR, m_editCANinjectdir);
	DDX_Text(pDX, IDC_EDIT_LVDSFILEDIR, m_editLVDSinjectdir);
	DDX_Check(pDX, IDC_CHECK_CANINJECTION, m_checkCANinjection);
	DDX_Check(pDX, IDC_CHECK_CANIMMEDIEATE, m_checkCANimmedieate);
	DDX_Check(pDX, IDC_CHECK_LVDSINJECTION, m_checkLVDSinjection);
	DDX_Check(pDX, IDC_CHECK_LVDSIMMEDIEATE, m_checkLVDSimmedieate);
	DDX_Text(pDX, IDC_EDIT_CLIENTCODE, m_EditclientCode);
	DDX_Text(pDX, IDC_EDIT_TASKCODE, m_EdittaskCode);
	DDX_Text(pDX, IDC_EDIT_STARTTIME, m_Editstarttime);
	DDX_Text(pDX, IDC_EDIT_TIMELENGTH, m_Edittimelength);
	DDX_Text(pDX, IDC_EDIT_OFFSETTIME0, m_Editoffsettime0);
	DDX_Text(pDX, IDC_EDIT_OFFSETTIME1, m_Editoffsettime1);
	DDX_Text(pDX, IDC_EDIT_OFFSETTIME2, m_Editoffsettime2);
	DDX_Text(pDX, IDC_EDIT_OFFSETTIME3, m_Editoffsettime3);
	DDX_Text(pDX, IDC_EDIT_DATAADDR, m_Editdataaddr);
	DDX_Check(pDX, IDC_CHECK_ISTASKDATA, m_check_istaskdata);
	DDX_Text(pDX, IDC_EDIT_TASKINJECTDATAFILE, m_EditTaskInjectionDatafiel);
	DDX_Check(pDX, IDC_CHECK_LISTCMDLVDS, m_checkSinglecmdlvds);
	DDX_Check(pDX, IDC_CHECK_LISTCMDCAN, m_checkSinglecmdcan);
	DDX_Check(pDX, IDC_CHECK_CANTELE, m_checkCANtele);
	DDX_Check(pDX, IDC_CHECK_LVDSTELE, m_checkLVDStele);
	DDX_Text(pDX, IDC_EDIT_TASKTOTIME, m_edit_TaskT0Time);
	DDX_Control(pDX, IDC_EDIT_OUTPUTREPORT, m_Edit_reportOutput);
}


BEGIN_MESSAGE_MAP(CDlgCommandSheet, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTROL, &CDlgCommandSheet::OnNMDblclkListControl)
	ON_BN_CLICKED(IDC_BUTTON_CANPARA, &CDlgCommandSheet::OnBnClickedButtonCanpara)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CONTROL, &CDlgCommandSheet::OnNMRClickListControl)
	ON_COMMAND(ID_CMD_SEND, &CDlgCommandSheet::OnCmdSend)
	ON_COMMAND(ID_CMD_MOVE_UP, &CDlgCommandSheet::OnCmdMoveUp)
	ON_COMMAND(ID_CMD_MOVE_DOWN, &CDlgCommandSheet::OnCmdMoveDown)
	ON_COMMAND(ID_CMD_INSERT, &CDlgCommandSheet::OnCmdInsert)
	ON_COMMAND(ID_CMD_DELETE, &CDlgCommandSheet::OnCmdDelete)
	ON_COMMAND(ID_CMD_DELETEALL, &CDlgCommandSheet::OnCmdDeleteall)
	ON_BN_CLICKED(IDC_BUTTON_CMD_INPUT, &CDlgCommandSheet::OnBnClickedButtonCmdInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CDlgCommandSheet::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_CMDSend, &CDlgCommandSheet::OnBnClickedButtonCmdsend)
	ON_BN_CLICKED(IDC_BUTTON_CANSEND, &CDlgCommandSheet::OnBnClickedButtonCansend)
	ON_BN_CLICKED(IDC_BUTTON_CANINJECTFILE, &CDlgCommandSheet::OnBnClickedButtonCaninjectfile)
	ON_BN_CLICKED(IDC_BUTTONLVDSINJECTSEND, &CDlgCommandSheet::OnBnClickedButtonlvdsinjectsend)
	ON_BN_CLICKED(IDC_BUTTON_LVDSINFILE, &CDlgCommandSheet::OnBnClickedButtonLvdsinfile)
	ON_BN_CLICKED(IDC_CHECK_CANINJECTION, &CDlgCommandSheet::OnBnClickedCheckCaninjection)
	ON_BN_CLICKED(IDC_CHECK_CANIMMEDIEATE, &CDlgCommandSheet::OnBnClickedCheckCanimmedieate)
	ON_BN_CLICKED(IDC_CHECK_LVDSINJECTION, &CDlgCommandSheet::OnBnClickedCheckLvdsinjection)
	ON_BN_CLICKED(IDC_CHECK_LVDSIMMEDIEATE, &CDlgCommandSheet::OnBnClickedCheckLvdsimmedieate)
	ON_BN_CLICKED(IDC_CHECK_ISTASKDATA, &CDlgCommandSheet::OnBnClickedCheckIstaskdata)
	ON_BN_CLICKED(IDC_BUTTON_TASKDATADIR, &CDlgCommandSheet::OnBnClickedButtonTaskdatadir)
	ON_BN_CLICKED(IDC_BUTTON_SINGELCMDSEDN, &CDlgCommandSheet::OnBnClickedButtonSingelcmdsedn)
	ON_BN_CLICKED(IDC_CHECK_CANTELE, &CDlgCommandSheet::OnBnClickedCheckCantele)
	ON_BN_CLICKED(IDC_CHECK_LVDSTELE, &CDlgCommandSheet::OnBnClickedCheckLvdstele)
	ON_BN_CLICKED(IDC_BUTTON_CURRENTTIME, &CDlgCommandSheet::OnBnClickedButtonCurrenttime)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgCommandSheet message handlers

void CDlgCommandSheet::OnSize(UINT nType, int cx, int cy)
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
void CDlgCommandSheet::get_control_original_proportion()
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

BOOL CDlgCommandSheet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  Add extra initialization here
	m_pInterface = new CInterface;
	LONG lStyle;
	lStyle = GetWindowLong(m_ListCtrlCommand.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_ListCtrlCommand.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListCtrlCommand.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//	dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	m_ListCtrlCommand.SetExtendedStyle(dwStyle); //设置扩展风格

	typedef struct {
		char text[20];
		int width;
	}LIST_HEAD;

	LIST_HEAD head[] = {
		{ "序号", 60 },
		{ "绝对时间", 240 },
		{ "指令功能描述", 180 },
		{ "指令码", 60 },
		{ "参数长度", 80 },
		{ "参数", 660 }
	};
	LV_COLUMN lvc[sizeof(head) / sizeof(LIST_HEAD)];
	int i;
	for (i = 0; i < sizeof(head) / sizeof(LIST_HEAD); i++)
	{
		lvc[i].mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
		lvc[i].iSubItem = 0;
		lvc[i].pszText = head[i].text;
		lvc[i].cx = head[i].width;
		m_ListCtrlCommand.InsertColumn(i, &lvc[i]);
	}
 //	m_ListCtrlCommand.InsertItem(0, _T(""));
// 	m_ListCtrlCommand.InsertItem(1, _T("测试文字2"));
// 	m_ListCtrlCommand.InsertItem(2, _T("测试文字3"));
// 	m_ListCtrlCommand.InsertItem(0, _T("测试文字2"));
// 	COLORREF color;
// 	color = RGB(51, 153, 255);
// 	m_ListCtrlCommand.SetColColor(0, color);
// 	color = RGB(255, 0, 0);
// 	m_ListCtrlCommand.SetColColor(1, color);
//	UpdateData(FALSE);
	
	m_CTeleDisplay.Open(&m_ListTeleOutput,TELECOMMANDSHEET);
	get_control_original_proportion();
	m_xml.Open("monitor.xml");
	m_MonitorCmdNum=GetMonitorxmlInfo(m_pMonitorInfo);
	
//	memcpy(m_CTeleDisplay.m_pMonitorInfo, m_pMonitorInfo, 256 * sizeof(CmdInfo));
//	m_CTeleDisplay.m_pMonitorInfo = m_pMonitorInfo;
	m_xml.Open("commandsCan.xml");
	m_CANcmdNum = GetMonitorxmlInfo(m_pCANcmdInfo);	
	m_xml.Open("commands_LVDS.xml");
	m_LVDScmdNum = GetMonitorxmlInfo(m_pLVDScmdInfo);

	m_xml.Open("commands.xml");
	GetCmdInfo(m_pCmdInfo);
	GetLocalTime(&m_GPSTimeNowday);
	SetTimer(0, 500, NULL);
	SetTimer(TIMERBROADCAST, 1000, NULL);
	if (m_checkCANtele)
	{
		SetTimer(TIMERCANSENTELE, 1000, NULL);
	}
	if (m_checkLVDStele)
	{
		SetTimer(TIEMRLVDSSENDTELE, 1000, NULL);
	}
	m_menu.LoadMenu(IDR_MENUCMD);
	CMenu *pMenu;
	pMenu = m_menu.GetSubMenu(0);
	map<CString, int>::iterator itr = m_mappackType.begin();
	while (itr != m_mappackType.end())
	{
		m_ComboBoxPackType.AddString(itr->first);
		//////////////////////////////////////////////////////////////////////////
		itr++;
	}	
	for (i = 0; i < m_CANcmdNum; i++)
	{
		CString str(m_pCANcmdInfo[i]->cmd_name);
		m_ComboCancmd.AddString(str);
	}
	for (i = 0; i < m_LVDScmdNum; i++)
	{
		CString str(m_pLVDScmdInfo[i]->cmd_name);
		m_ComboLVDScmd.AddString(str);
	}

	
	m_CTeleDisplay.displayList(true, m_pMonitorInfo, m_MonitorCmdNum, &m_ListTeleOutput,1);

// 	COLORREF color;
// 	color = RGB(51, 153, 255);
// 	m_ListTeleOutput.SetColColor(0, color);
// 	color = RGB(255, 0, 0);
// 	m_ListTeleOutput.SetColColor(1, color);
	UpdateData(FALSE);

	///open can card
	if (m_dlgCanConfig.DoModal() == IDOK)
	{
		m_pInterface->m_connect_CAN = m_dlgCanConfig.m_connect_CAN;
		m_pInterface->m_connect_LVDS = m_dlgCanConfig.m_connect_LVDS;
		m_pInterface->m_connect_RS422 = m_dlgCanConfig.m_connect_RS422;
		m_pInterface->m_cannum = m_dlgCanConfig.m_cannum;
	}
	Sleep(500);
	m_ComboBoxPackType.SetCurSel(3);
	m_ComboCancmd.SetCurSel(0);
	m_ComboLVDScmd.SetCurSel(0);
	m_ComboBoxPackType.SetItemHeight(-1, 25);
	m_ComboCancmd.SetItemHeight(-1, 25);
	m_ComboLVDScmd.SetItemHeight(-1, 25);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCommandSheet::OnNMDblclkListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int iListIndex = m_ListCtrlCommand.GetSelectionMark();//焦点所在行数

	int iAimPosition = 0;
	CMD_WN cmd;
	CMD_WN *pCmd, *pCmd1;

	if (iListIndex == -1)//新建指令
	{
		CDlgAddCommand dlgAddCommand(this, &cmd, 1);
		if (dlgAddCommand.DoModal() == IDOK)
		{

			AddCmdToList(&cmd, m_iRealCmdCnt, 1);
			memcpy(&m_cmdAddInfo[m_iRealCmdCnt], &cmd, sizeof(CMD_WN));
			m_iRealCmdCnt++;
		}
	}
	else//修改指令
	{
 		CDlgAddCommand dlgAddCommand(this, &m_cmdAddInfo[iListIndex], 0);
		if (dlgAddCommand.DoModal() == IDOK)
		{
			m_ListCtrlCommand.DeleteItem(iListIndex);
			AddCmdToList(&m_cmdAddInfo[iListIndex], iListIndex, 1);
			//		m_iRealCmdCnt++;			
		}
		m_ListCtrlCommand.SetItemState(iListIndex, 0, -1);
	}


	m_ListCtrlCommand.SetSelectionMark(-1);
	*pResult = 0;
}
CString CDlgCommandSheet::GetAbsTime(__int64 sec)
{
	SYSTEMTIME t;
	CString strBuf1, strBuf2;
	ULARGE_INTEGER base;

	base.QuadPart = sec*SEC_PER_100ns;
	FileTimeToSystemTime((FILETIME *)&base, &t);

	strBuf1 = "";
	strBuf2.Format("%04d年", t.wYear);
	strBuf1 += strBuf2;

	strBuf2.Format("%02d月", t.wMonth);
	strBuf1 += strBuf2;
	strBuf2.Format("%02d日", t.wDay);
	strBuf1 += strBuf2;

	strBuf2.Format("%02d时%02d分%02d秒",
		t.wHour, t.wMinute, t.wSecond);
	strBuf1 += strBuf2;
	return strBuf1;
}
void CDlgCommandSheet::ExtractArgValue1(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length)
{
	int i;

	int byteStart = bitStart / 8;
	char *endptr;
	bitStart &= 7;
	CString strBuf;
	if (!bitStart){
		for (i = 0; i < length / 8; i++){
			//			pDst[i] = pSrc[i + byteStart];
			strBuf.Format(strBuf + "%02x", pSrc[i + byteStart]);
		}
		//		length &= 7;
		//		if (length){
		//			pDst[i] = pSrc[i + byteStart] & (0xFF << (8 - length));
		//		}
		unsigned int arg = strtoul(strBuf, &endptr, 16);
		memcpy(pDst, &arg, length / 8);
		return;
	}

	for (i = 0; i < length / 8; i++){
		pDst[i] = (pSrc[i + byteStart] << bitStart) | pSrc[i + byteStart + 1] >> (8 - bitStart);
	}
	length &= 7;
	if (length){
		pDst[i] = (pSrc[i + byteStart] << bitStart);
		if (length + bitStart > 8){
			pDst[i] |= pSrc[i + byteStart + 1] >> (8 - bitStart);
		}
		pDst[i] &= (0xFF << (8 - length));
	}
}
void CDlgCommandSheet::AddCmdToList(CMD_WN *pCmd, int index, int bNew)
{
	int i, j, k;
	int iRow;
	CString strBuf, strBuf1;

	CString strDev;

	CmdInfo *pCmdInfo;
	CMD_WN *pAddedCmd;

	unsigned char temp[MAX_ARG_LENGTH];
	pCmd->dev_id = 0x33;
//	strDev = GetDeviceName(pCmd->dev_id);
	strDev = "试验机";
	if (!strDev.Compare("无效!")){
		MessageBox("设备不存在,指令插入/替换失败!", "警告");
		return;
	}
	pCmdInfo = m_pCmdInfo[pCmd->cmd_id];
	if (!pCmdInfo){
		MessageBox("指令不存在,指令插入/替换失败!", "警告");
		return;
	}

	iRow = m_ListCtrlCommand.GetItemCount();
	// 	if (index >= iRow){
	// 		MessageBox("指令定位错误,指令插入/替换失败!", "警告");
	// 		return;
	// 	}
	if (index < 0){
		index = iRow;
	}
	pAddedCmd = (CMD_WN *)malloc(sizeof(CMD_WN));
	memcpy(pAddedCmd, pCmd, sizeof(CMD_WN));
	if (!bNew){
		free((CMD *)m_ListCtrlCommand.GetItemData(index));
	}
	else{
		m_ListCtrlCommand.InsertItem(index, "");
		for (i = index; i < m_ListCtrlCommand.GetItemCount(); i++){
			strBuf.Format("%0d", i);
			m_ListCtrlCommand.SetItemText(i, COL_SEQ, strBuf);
		}
	}
	m_ListCtrlCommand.SetItemData(index, (DWORD)pAddedCmd);

//	m_ListCtrlCommand.SetItemText(index, COL_DEV, strDev);

// 	if (pAddedCmd->bus_flag){
// 		m_ListCtrlCommand.SetItemText(index, COL_BUS, "B");
// 	}
// 	else{
// 		m_ListCtrlCommand.SetItemText(index, COL_BUS, "A");
// 	}
	if (pAddedCmd->immediate_flag){
		m_ListCtrlCommand.SetItemText(index, COL_ABS_TIME, "立即令");
	}
	else{
		strBuf.Format("TO + %d", pAddedCmd->time);
		m_ListCtrlCommand.SetItemText(index, COL_ABS_TIME, strBuf);
	}

	m_ListCtrlCommand.SetItemText(index, COL_DESC, (char *)pCmdInfo->cmd_name);

	strBuf.Format("%02X", pAddedCmd->cmd_id);
	m_ListCtrlCommand.SetItemText(index, COL_CODE, strBuf);

	strBuf.Format("%02d", pCmdInfo->arg_byte_num);
	m_ListCtrlCommand.SetItemText(index, COL_ARG_LEN, strBuf);

	strBuf = "";
	for (j = 0; j < pCmdInfo->arg_num; j++){
		ExtractArgValue1(temp, pAddedCmd->args, pCmdInfo->bit_start[j], pCmdInfo->arg_length[j]);
		for (k = 0; k < pCmdInfo->arg_length[j] / 8; k++){
			strBuf1.Format("%02X", temp[k]);
			strBuf += strBuf1;
		}
		if (pCmdInfo->arg_length[j] & 0x7){
			if (pCmdInfo->input_type[j] == 0){
				temp[k] >>= (8 - (pCmdInfo->arg_length[j] & 0x7));
			}
			strBuf1.Format("%02X", temp[k]);
			strBuf += strBuf1;
		}
		strBuf += " ";
	}
	m_ListCtrlCommand.SetItemText(index, COL_ARG, strBuf);

	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT;

	lvc.pszText = _T("绝对时间");
	m_ListCtrlCommand.SetColumn(COL_ABS_TIME, &lvc);
	//	m_ListCtrlCommand.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return;
}

xmlXPathObjectPtr CDlgCommandSheet::LocateXPath(char xpath_expr[])
{
	xmlXPathObjectPtr xpathObj = NULL;
	xpathObj = m_xml.GetNodesByXPath(BAD_CAST(xpath_expr));//查询XPath表达式
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
xmlXPathObjectPtr CDlgCommandSheet::LocateCommand(unsigned char dev_id, unsigned char cmd_id)
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
void CDlgCommandSheet::GetCmdInfo(CmdInfo *m_pCmdInfo[100])
{
	memset(m_pCmdInfo, 0, sizeof(CmdInfo *)* 100);
	xmlXPathObjectPtr xpathObj;
	char xpath_expr[200];
	int i, j;
	xmlNode *pNode, *pNodeSub;
	xmlChar *xmlRtn;
	char *endptr;
	int idx;
	int length;
	CmdInfo *pCmdInfo;
	CString strTemp;
	unsigned char temp[MAX_ARG_LENGTH];
	unsigned char edit_index, combo_index;
	int sumGB2312;

	sprintf(xpath_expr, "/CommandSet/Category/Command");
	xpathObj = LocateXPath(xpath_expr);//查询节点
//	monitor_numcnt = xpathObj->nodesetval->nodeNr;
	if (xpathObj){
		for (i = 0; i < xpathObj->nodesetval->nodeNr; i++){
			pNode = xpathObj->nodesetval->nodeTab[i];
			xmlRtn = xmlGetProp(pNode, BAD_CAST("CommandCode"));
			idx = strtol((char *)xmlRtn, &endptr, 16);
			idx &= 0xFF;
			xmlFree(xmlRtn);
			pCmdInfo = (CmdInfo *)malloc(sizeof(CmdInfo));
			m_pCmdInfo[idx] = pCmdInfo;
			memset(pCmdInfo, 0, sizeof(CmdInfo));
			xmlRtn = CXML::xmlGetPropGBK(pNode, BAD_CAST("CommandName"));
			length = strlen((char *)xmlRtn);
			if (length > MAX_NAME_LENGTH){
				length = MAX_NAME_LENGTH - 1;
			}
			memcpy(pCmdInfo->cmd_name, xmlRtn, length);
			sumGB2312 = 0;
			for (j = 0; j < length; j++){
				if (pCmdInfo->cmd_name[j] & 0x80){
					sumGB2312++;
				}
			}
			pCmdInfo->cmd_name[length] = '\0';
			if (sumGB2312 & 0x01){
				pCmdInfo->cmd_name[length - 1] = '\0';
			}
			xmlFree(xmlRtn);
			pNode = pNode->xmlChildrenNode;
			idx = 0;
			edit_index = 1;
			combo_index = 0;
			int cnt = 0;
			while (pNode){
				if (idx > MAX_ARG_NUM){
					break;
				}
				cnt++;
				if (!xmlStrcmp(pNode->name, BAD_CAST("Arg"))){//确定节点是否为Arg
					pCmdInfo->input_type[idx] = 1;
					pNodeSub = pNode->xmlChildrenNode;
					while (pNodeSub){
						if (!xmlStrcmp(pNodeSub->name, BAD_CAST("ArgValue"))){
							pCmdInfo->input_type[idx] = 0;
							break;
						}
						pNodeSub = pNodeSub->next;
					}
					xmlRtn = xmlGetProp(pNode, BAD_CAST("bitStart"));
					pCmdInfo->bit_start[idx] = atoi((char *)xmlRtn);
					xmlFree(xmlRtn);
					xmlRtn = xmlGetProp(pNode, BAD_CAST("bitLength"));
					pCmdInfo->arg_length[idx] = atoi((char *)xmlRtn);
					xmlFree(xmlRtn);
					xmlRtn = CXML::xmlGetPropGBK(pNode, BAD_CAST("ArgumentName"));
					length = strlen((char *)xmlRtn);
					if (length > MAX_NAME_LENGTH){
						length = MAX_NAME_LENGTH - 1;
					}
					memcpy(pCmdInfo->arg_name[idx], xmlRtn, length);
					sumGB2312 = 0;
					for (j = 0; j < length; j++){
						if (pCmdInfo->arg_name[idx][j] & 0x80){
							sumGB2312++;
						}
					}
					pCmdInfo->arg_name[idx][length] = '\0';
					if (sumGB2312 & 0x01){
						pCmdInfo->arg_name[idx][length - 1] = '\0';
					}
					xmlFree(xmlRtn);
					if (pCmdInfo->bit_start[idx] + pCmdInfo->arg_length[idx] > MAX_ARG_LENGTH * 8){
						break;
					}
					pCmdInfo->arg_byte_num += pCmdInfo->arg_length[idx];
					xmlRtn = xmlGetProp(pNode, BAD_CAST("initValue"));
					strTemp = xmlRtn;
					xmlFree(xmlRtn);
// 					strTemp.GetBufferSetLength((pCmdInfo->arg_length[idx] + 7) / 8 * 2);
// 					for (j = 0; j < pCmdInfo->arg_length[idx] / 8; j++){
// 						temp[j] = strtol(strTemp.Mid(j * 2, 2), &endptr, 16);
// 					}
// 					if (pCmdInfo->arg_length[idx] & 0x7){
// 						temp[j] = strtol(strTemp.Mid(j * 2, 2), &endptr, 16);
// 						if (pCmdInfo->input_type[idx] == 0){
// 							temp[j] <<= (8 - (pCmdInfo->arg_length[idx] & 0x7));
// 						}
// 					}

					unsigned int arg = strtoul(strTemp, &endptr, 16);
					memcpy(temp, &arg, (pCmdInfo->arg_length[idx] + 7) / 8);
					m_Ctelemetry.InsertArgValue(pCmdInfo->init_value, temp, pCmdInfo->bit_start[idx], pCmdInfo->arg_length[idx]);
					if (pCmdInfo->input_type[idx] == 0){
						pCmdInfo->input_ctrl_index[idx] = combo_index;
						combo_index++;
						edit_index += 2;
					}
					else{
						pCmdInfo->input_ctrl_index[idx] = edit_index;
						edit_index += 3;
					}
					idx++;
				}
				pNode = pNode->next;
			}
			pCmdInfo->arg_num = idx;
			pCmdInfo->arg_byte_num /= 8;
		}
		xmlXPathFreeObject(xpathObj);
	}
// 	CXML coef_xml;
// 	coef_xml.Open("coef.xml");
// 	for (i = 0; i < 256; i++){
// 		if (m_pCmdInfo[i]){
// 			for (j = 0; j < m_pCmdInfo[i]->arg_num; j++){
// 		//		m_pCmdInfo[i]->coef[j] = GetCoefficient(&coef_xml, i, m_pCmdInfo[i]->bit_start[j]);
// 			}
// 		}
// 	}
}
int CDlgCommandSheet::GetMonitorxmlInfo(CmdInfo *m_pCmdInfo[50])
{
	memset(m_pCmdInfo, 0, sizeof(CmdInfo *)* 50);
	xmlXPathObjectPtr xpathObj;
	char xpath_expr[200];
	int i, j;
	xmlNode *pNode, *pNodeSub;
	xmlChar *xmlRtn;
	char *endptr;
	int idx;
	int xmlCMDnum = 0;
	int length;
	CmdInfo *pCmdInfo;
	CString strTemp;
	unsigned char temp[MAX_ARG_LENGTH];
	unsigned char edit_index, combo_index;
	int sumGB2312;
	int combxCntinArg = 0;
	sprintf(xpath_expr, "/CommandSet/Category/Command");
	xpathObj = LocateXPath(xpath_expr);//查询节点
	CString str;
	if (xpathObj){
		xmlCMDnum = xpathObj->nodesetval->nodeNr;
		for (i = 0; i < xpathObj->nodesetval->nodeNr; i++){
			pNode = xpathObj->nodesetval->nodeTab[i];
			xmlRtn = xmlGetProp(pNode, BAD_CAST("CommandCode"));
			idx = strtol((char *)xmlRtn, &endptr, 16);
			idx &= 0xFF;
			xmlFree(xmlRtn);
			pCmdInfo = (CmdInfo *)malloc(sizeof(CmdInfo));
			m_pCmdInfo[idx] = pCmdInfo;
			memset(pCmdInfo, 0, sizeof(CmdInfo));
			xmlRtn = xmlGetProp(pNode, BAD_CAST("ArgumentByteLength"));
			pCmdInfo->arg_byte_num = strtol((char *)xmlRtn, &endptr, 10);
			xmlFree(xmlRtn);
			xmlRtn = CXML::xmlGetPropGBK(pNode, BAD_CAST("CommandName"));
			length = strlen((char *)xmlRtn);
			if (length > MAX_NAME_LENGTH){
				length = MAX_NAME_LENGTH - 1;
			}
			memcpy(pCmdInfo->cmd_name, xmlRtn, length);
			sumGB2312 = 0;
			for (j = 0; j < length; j++){
				if (pCmdInfo->cmd_name[j] & 0x80){
					sumGB2312++;
				}
			}
			pCmdInfo->cmd_name[length] = '\0';
			if (sumGB2312 & 0x01){
				pCmdInfo->cmd_name[length - 1] = '\0';
			}
			xmlFree(xmlRtn);
			pNode = pNode->xmlChildrenNode;
			idx = 0;
			edit_index = 1;
			combo_index = 0;
			int cnt = 0;
			while (pNode){
				if (idx > MAX_ARG_NUM){
					break;
				}
				cnt++;
				if (!xmlStrcmp(pNode->name, BAD_CAST("Arg"))){//确定节点是否为Arg
					pCmdInfo->input_type[idx] = 1;
					pNodeSub = pNode->xmlChildrenNode;
					combxCntinArg = 0;
					while (pNodeSub){
						if (!xmlStrcmp(pNodeSub->name, BAD_CAST("ArgValue"))){
							pCmdInfo->input_type[idx] = 0;
							xmlRtn = CXML::xmlGetPropGBK(pNodeSub, BAD_CAST("ArgumentValueName"));
							length = strlen((char *)xmlRtn);
							if (length > MAX_NAME_LENGTH){
								length = MAX_NAME_LENGTH - 1;
							}
							memcpy(pCmdInfo->Arg_CombxName[idx][combxCntinArg], xmlRtn, length);
							sumGB2312 = 0;
							for (j = 0; j < length; j++){
								if (pCmdInfo->Arg_CombxName[idx][combxCntinArg][j] & 0x80){
									sumGB2312++;
								}
							}
							pCmdInfo->Arg_CombxName[idx][combxCntinArg][length] = '\0';
							if (sumGB2312 & 0x01){
								pCmdInfo->Arg_CombxName[idx][combxCntinArg][length - 1] = '\0';
							}
							xmlFree(xmlRtn);
							xmlRtn = xmlNodeGetContent(pNodeSub);
							str = xmlRtn;

							pCmdInfo->Arg_CombxCode[idx][combxCntinArg] = strtol(str.Left(2), &endptr, 16);
							combxCntinArg++;

						}
						pCmdInfo->combcntNum[idx] = combxCntinArg;
						pNodeSub = pNodeSub->next;
					}
					xmlRtn = xmlGetProp(pNode, BAD_CAST("datatype"));
					if (xmlRtn)
					{
						str = xmlRtn;
						pCmdInfo->datatype[idx] = strtol(str.Left(2), &endptr, 16);
						xmlFree(xmlRtn);
					}

					xmlRtn = CXML::xmlGetPropGBK(pNode, BAD_CAST("cal"));
					if (xmlRtn)
					{
						str = xmlRtn;
						pCmdInfo->cal[idx] = strtol(str.Left(2), &endptr, 10);
						xmlFree(xmlRtn);
					}
					xmlRtn = xmlGetProp(pNode, BAD_CAST("bitStart"));
					pCmdInfo->bit_start[idx] = atoi((char *)xmlRtn);
					xmlFree(xmlRtn);
					xmlRtn = xmlGetProp(pNode, BAD_CAST("bitLength"));
					pCmdInfo->arg_length[idx] = atoi((char *)xmlRtn);
					xmlFree(xmlRtn);
					xmlRtn = CXML::xmlGetPropGBK(pNode, BAD_CAST("ArgumentName"));
					length = strlen((char *)xmlRtn);
					if (length > MAX_NAME_LENGTH){
						length = MAX_NAME_LENGTH - 1;
					}
					memcpy(pCmdInfo->arg_name[idx], xmlRtn, length);
					sumGB2312 = 0;
					for (j = 0; j < length; j++){
						if (pCmdInfo->arg_name[idx][j] & 0x80){
							sumGB2312++;
						}
					}
					pCmdInfo->arg_name[idx][length] = '\0';
					if (sumGB2312 & 0x01){
						pCmdInfo->arg_name[idx][length - 1] = '\0';
					}
					xmlFree(xmlRtn);
					if (pCmdInfo->bit_start[idx] + pCmdInfo->arg_length[idx] > MAX_ARG_LENGTH * 8){
						break;
					}
					//					pCmdInfo->arg_byte_num += pCmdInfo->arg_length[idx];
					xmlRtn = xmlGetProp(pNode, BAD_CAST("initValue"));
					strTemp = xmlRtn;
					xmlFree(xmlRtn);
					strTemp.GetBufferSetLength((pCmdInfo->arg_length[idx] + 7) / 8 * 2);
					for (j = 0; j < pCmdInfo->arg_length[idx] / 8; j++){
						temp[j] = strtol(strTemp.Mid(j * 2, 2), &endptr, 16);
					}
					if (pCmdInfo->arg_length[idx] & 0x7){
						temp[j] = strtol(strTemp.Mid(j * 2, 2), &endptr, 16);
						if (pCmdInfo->input_type[idx] == 0){
							temp[j] <<= (8 - (pCmdInfo->arg_length[idx] & 0x7));
						}
					}
					m_Ctelemetry.InsertArgValue(pCmdInfo->init_value, temp, pCmdInfo->bit_start[idx], pCmdInfo->arg_length[idx]);
					if (pCmdInfo->input_type[idx] == 0){
						pCmdInfo->input_ctrl_index[idx] = combo_index;
						combo_index++;
						edit_index += 2;
					}
					else{
						pCmdInfo->input_ctrl_index[idx] = edit_index;
						edit_index += 3;
					}
					idx++;
				}
				pNode = pNode->next;
			}
			pCmdInfo->arg_num = idx;
		}
		xmlXPathFreeObject(xpathObj);
	}
	return xmlCMDnum;
}

void CDlgCommandSheet::OnBnClickedButtonCanpara()
{
	// TODO: Add your control notification handler code here
	char buffer[1024];	
	if (m_dlgCanConfig.DoModal() == IDOK)
	{
		m_pInterface->m_connect_CAN = m_dlgCanConfig.m_connect_CAN;
		m_pInterface->m_connect_LVDS = m_dlgCanConfig.m_connect_LVDS;
		m_pInterface->m_connect_RS422 = m_dlgCanConfig.m_connect_RS422;
		m_pInterface->m_cannum = m_dlgCanConfig.m_cannum;
	

	}
}

void CDlgCommandSheet::SetCurrentTimer()
{
	time_t nowtime;
	nowtime = time(NULL);
	nowtime = m_edit_TaskT0Time + nowtime - m_pInterface->m_taskT0time;
	CString strBuf;
	strBuf.Format("%d", nowtime);
	SYSTEMTIME t;
// 	t = m_CTeleDisplay.TimetToSystemTime(nowtime);
// //	GetLocalTime(&t);
// 	strBuf.Format("%03d日%02d时%02d分%02d秒%03d毫秒",
// 		t.wDay-m_GPSTimeNowday.wDay,
// 		t.wHour,
// 		t.wMinute,
// 		t.wSecond,
// 		t.wMilliseconds);
	GetDlgItem(IDC_BUTTON_CURRENTTIME)->SetWindowText(strBuf);
}
void CDlgCommandSheet::Setteleinitvalue(RecvScanBuf telebuf)
{
	CmdInfo *pCmdInfo;
	int k = 0;
	int idx = 0;
	while (k < telebuf.len)
	{		
		pCmdInfo = m_pMonitorInfo[idx];
		if ((telebuf.len - k) < pCmdInfo->arg_byte_num)
		{
			memcpy(pCmdInfo->init_value, telebuf.Buf + k, telebuf.len - k);
			k = telebuf.len;
		}
		else
		{
			memcpy(pCmdInfo->init_value, telebuf.Buf + k, pCmdInfo->arg_byte_num);
			k += pCmdInfo->arg_byte_num;
		}
		
		
		idx++;
	}
}
void CDlgCommandSheet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	switch (nIDEvent)
	{
	case TIEMRCOMMANDTIEM:
		SetCurrentTimer();
		m_pEditCmdSend.SetWindowTextA(gstr_output_send);
		m_pEditCmdSend.LineScroll(m_pEditCmdSend.GetLineCount(), 0);
		m_Edit_reportOutput.SetWindowTextA(gstr_outputReport);
		m_Edit_reportOutput.LineScroll(m_Edit_reportOutput.GetLineCount(), 0);
		if (m_pInterface->m_telebuffull)
		{
			Setteleinitvalue(m_pInterface->m_telebuf);
			m_CTeleDisplay.displayList(false, m_pMonitorInfo, m_MonitorCmdNum, &m_ListTeleOutput,1);
			m_CTeleDisplay.displayList(false, m_pMonitorInfo, m_MonitorCmdNum, &m_dlgRefreshsheet->m_ListRefresh,3);
			m_pInterface->m_telebuffull = false;
		}
		if (m_iscandatasend)
		{
			if (!(m_pInterface->m_canimmedieateSend || m_pInterface->m_caninjectTaskSend))
			{
				GetDlgItem(IDC_BUTTON_CANSEND)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SINGELCMDSEDN)->EnableWindow(TRUE);
				m_iscandatasend = FALSE;
			}
		}
		if (m_islvdsdatasend)
		{
			if (!(m_pInterface->m_lvdsimmedieateSend || m_pInterface->m_lvdsinjectTaskSend))
			{
				GetDlgItem(IDC_BUTTONLVDSINJECTSEND)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SINGELCMDSEDN)->EnableWindow(TRUE);
				m_islvdsdatasend = FALSE;
			}
		}
		
		break;
	case TIMERCANSENTELE:
		m_pInterface->m_canteleframesend = true;		
		break;
	case TIEMRLVDSSENDTELE:		
		m_pInterface->m_lvdsteleframesend = true;
		break;
	case TIMERBROADCAST://遥测显示
		m_pInterface->m_cantimebroadcast = true;
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommandSheet::OnNMRClickListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CMenu* pPopup = m_menu.GetSubMenu(0);
	POINT point;
	GetCursorPos(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, CWnd::FromHandle(m_hWnd));

	*pResult = 0;
}

void CDlgCommandSheet::ShowInfo(CString str)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_StrCMDsend.Format(m_StrCMDsend + "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	m_StrCMDsend += "：";
	m_StrCMDsend += str;
	m_StrCMDsend += "\r\n";
	m_pEditCmdSend.SetWindowTextA(m_StrCMDsend);
	m_pEditCmdSend.LineScroll(m_pEditCmdSend.GetLineCount(), 0);
}
//发送单条指令
void CDlgCommandSheet::OnCmdSend()
{
	// TODO: Add your command handler code here
	
	UpdateData(TRUE);
	int i = 0, iListIndex = -1;
	int cmdIdx;
	bool reb = false;
	
	cmdIdx = m_ListCtrlCommand.GetSelectionMark();
	if (cmdIdx < 0)
	{
		MessageBox("未选定指令！");
		return;
	}
	if (!(m_checkSinglecmdlvds || m_checkSinglecmdcan))
	{
		MessageBox("请选择发送通道!", "警告");
		return;
	}
	GetDlgItem(IDC_BUTTON_SINGELCMDSEDN)->EnableWindow(FALSE);
	char *InjectionBuffer, *dataFramBuffer;
	InjectionBuffer = (char *)malloc(MAX_INJECTTION);//
	dataFramBuffer = (char *)malloc(MAX_INJECTTION);//最大数据长度
	InjectionInfo Injectionpara;
	Getinjectionpara(&Injectionpara);
	CMDbuf cmdbuf;
	bool bret = GetCMDsingle(&cmdbuf);
	FILE *fp;
	fp = fopen("", "wb");
	if (!bret)
	{
		free(InjectionBuffer);
		free(dataFramBuffer);
		return;
	}
	int injectcnt = getcmdinjectiondata(cmdbuf, InjectionBuffer, dataFramBuffer);
	
	if (m_checkSinglecmdlvds)
	{
		GetDlgItem(IDC_BUTTONLVDSINJECTSEND)->EnableWindow(FALSE);
		m_islvdsdatasend = TRUE;
		m_pInterface->m_lvdsimmedieateSend = TRUE;
		m_pInterface->getLVDSRS422injectbuf(LVDS_CARD_N1, injectcnt, dataFramBuffer, &m_pInterface->m_sendLVDSdatabuf);
	}
	if (m_checkSinglecmdcan)
	{
		m_iscandatasend = TRUE;
		GetDlgItem(IDC_BUTTON_CANSEND)->EnableWindow(FALSE);
		m_pInterface->m_canimmedieateSend = TRUE;	
		m_pInterface->getCANinjectbuf(injectcnt, dataFramBuffer, &m_pInterface->m_sendcandatabuf);
		
	}		
	free(InjectionBuffer);
	free(dataFramBuffer);

}


void CDlgCommandSheet::OnCmdMoveUp()
{
	// TODO: Add your command handler code here
	int i = 0, iListIndex = -1;
	CString strTxt;
	CString Str[2][8];
	CMD_WN m_cmdAddInfotemp;
	int cmdIdx;
	iListIndex = m_ListCtrlCommand.GetSelectionMark();
	if (!m_ListCtrlCommand.GetItemState(iListIndex, LVIS_FOCUSED)){
		MessageBox("请先选择需上移的指令！", "错误");
		return;
	}
	if (iListIndex < 1)
	{
		MessageBox("指令已经是最上面的啦！", "错误");
		return;
	}
	memcpy(&m_cmdAddInfotemp, m_cmdAddInfo + iListIndex - 1, sizeof(CMD_WN));
	memcpy(m_cmdAddInfo + iListIndex - 1, m_cmdAddInfo + iListIndex, sizeof(CMD_WN));
	memcpy(m_cmdAddInfo + iListIndex, &m_cmdAddInfotemp, sizeof(CMD_WN));
	CMD_WN *PCMD;
	PCMD = (CMD_WN *)malloc(sizeof(CMD_WN));
	PCMD = (CMD_WN *)m_ListCtrlCommand.GetItemData(iListIndex - 1);
	m_ListCtrlCommand.SetItemData(iListIndex - 1, (DWORD)((CMD_WN *)m_ListCtrlCommand.GetItemData(iListIndex)));
	m_ListCtrlCommand.SetItemData(iListIndex, (DWORD)PCMD);

	for (i = 1; i < 8; i++)
	{
		Str[0][i] = m_ListCtrlCommand.GetItemText(iListIndex - 1, i);
		Str[1][i] = m_ListCtrlCommand.GetItemText(iListIndex, i);
		m_ListCtrlCommand.SetItemText(iListIndex - 1, i, Str[1][i]);
		m_ListCtrlCommand.SetItemText(iListIndex, i, Str[0][i]);
	}
	m_ListCtrlCommand.SetSelectionMark(-1);
}


void CDlgCommandSheet::OnCmdMoveDown()
{
	// TODO: Add your command handler code here
	int i = 0, iListIndex = -1;
	CString strTxt;
	CString Str[2][8];
	CMD_WN m_cmdAddInfotemp;
	int cmdIdx;
	iListIndex = m_ListCtrlCommand.GetSelectionMark();
	int totalCMD = m_ListCtrlCommand.GetItemCount();
	if (!m_ListCtrlCommand.GetItemState(iListIndex, LVIS_FOCUSED)){
		MessageBox("请先选择需上移的指令！", "错误");
		return;
	}
	if (iListIndex >= totalCMD - 1)
	{
		MessageBox("指令已经是最下面的啦！", "错误");
		return;
	}
	memcpy(&m_cmdAddInfotemp, m_cmdAddInfo + iListIndex + 1, sizeof(CMD_WN));
	memcpy(m_cmdAddInfo + iListIndex + 1, m_cmdAddInfo + iListIndex, sizeof(CMD_WN));
	memcpy(m_cmdAddInfo + iListIndex, &m_cmdAddInfotemp, sizeof(CMD_WN));
	CMD_WN *PCMD;
	PCMD = (CMD_WN *)malloc(sizeof(CMD_WN));
	PCMD = (CMD_WN *)m_ListCtrlCommand.GetItemData(iListIndex + 1);
	m_ListCtrlCommand.SetItemData(iListIndex + 1, (DWORD)((CMD_WN *)m_ListCtrlCommand.GetItemData(iListIndex)));
	m_ListCtrlCommand.SetItemData(iListIndex, (DWORD)PCMD);
	for (i = 1; i < 8; i++)
	{
		Str[0][i] = m_ListCtrlCommand.GetItemText(iListIndex, i);
		Str[1][i] = m_ListCtrlCommand.GetItemText(iListIndex + 1, i);
		m_ListCtrlCommand.SetItemText(iListIndex, i, Str[1][i]);
		m_ListCtrlCommand.SetItemText(iListIndex + 1, i, Str[0][i]);
	}
	m_ListCtrlCommand.SetSelectionMark(-1);
}


void CDlgCommandSheet::OnCmdInsert()
{
	CMD_WN cmd;
	CDlgAddCommand dlgAddCmd(this, &cmd, 1);
	int index;

	index = m_ListCtrlCommand.GetSelectionMark();

	if (!m_ListCtrlCommand.GetItemState(index, LVIS_FOCUSED)){
		MessageBox("请先选择插入位置！", "错误");
		return;
	}

	unsigned int time;
	CMD_WN *pCmd, *pCmd1;
	pCmd = (CMD_WN *)m_ListCtrlCommand.GetItemData(index);


	for (int i = index - 1; i >= 0; i--){
		pCmd1 = (CMD_WN *)m_ListCtrlCommand.GetItemData(i);		
		if (pCmd1->immediate_flag){
			continue;
		}
		time = pCmd1->time;
		break;
	}

	cmd.bus_flag = m_bus_flag;
	int ret = dlgAddCmd.DoModal();
	if (ret == IDOK){
		AddCmdToList(&cmd, index, 1);
		memcpy(&m_cmdAddInfo[m_iRealCmdCnt], &cmd, sizeof(CMD_WN));
		m_iRealCmdCnt++;
		//		m_list.DeleteItem(index);

	}
	m_ListCtrlCommand.SetSelectionMark(-1); 
	// TODO: Add your command handler code here

}


void CDlgCommandSheet::OnCmdDelete()
{
	// TODO: Add your command handler code here
	int i = 0, iListIndex = -1;
	CString strTxt;
	int cmdIdx;
	iListIndex = m_ListCtrlCommand.GetSelectionMark();
	if (!m_ListCtrlCommand.GetItemState(iListIndex, LVIS_FOCUSED)){
		MessageBox("请先选择需删除的指令！", "错误");
		return;
	}
	for (int i = iListIndex; i < MAXCOMMAND - 1; i++)
	{
		memcpy(m_cmdAddInfo + i, m_cmdAddInfo + i + 1, sizeof(CMD_WN));
	}
	m_iRealCmdCnt -= 1;
	CMD_WN *pCmd;
	pCmd = (CMD_WN *)m_ListCtrlCommand.GetItemData(iListIndex);
	m_ListCtrlCommand.DeleteItem(iListIndex);

	for (int i = iListIndex; i < m_ListCtrlCommand.GetItemCount(); i++){
		strTxt.Format("%d", i);
		m_ListCtrlCommand.SetItemText(i, COL_SEQ, strTxt);
	}
	m_ListCtrlCommand.SetSelectionMark(-1);
}


void CDlgCommandSheet::OnCmdDeleteall()
{
	// TODO: Add your command handler code here
	m_iRealCmdCnt = 0;
	m_ListCtrlCommand.DeleteAllItems();

	m_ListCtrlCommand.SetSelectionMark(-1);
}


void CDlgCommandSheet::OnBnClickedButtonCmdInput()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "QCmdList", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOCHANGEDIR,
		"QCmdXml Files (*.bin)|*.bin||", NULL);
	//		"QCmdList Files (*.cls)|*.cls|QCmdLog Files (*.clg)|*.clg|QCmdXml Files (*.xml)|*.xml||", NULL);
	if (dlg.DoModal() != IDOK)
		return;

	m_ListCtrlCommand.DeleteAllItems();
	CString strFileName = dlg.GetPathName();
	CString strExt = dlg.GetFileExt();
	m_iRealCmdCnt = 0;
	LoadFromPLD(strFileName);
}
unsigned short CRC16Table[256];

void CRCTableGenerate(unsigned short h, unsigned char *crcTable)
{
	unsigned int i, j;
	unsigned char m = 16;
	unsigned int data;

	for (i = 0; i < 256; i++){
		data = i;
		for (j = m; j > 0; j--){
			if ((data & (1u << (m - 1))) == 0){
				data <<= 1;
			}
			else{
				data = (data << 1) ^ h;
			}
		}
		for (j = 0; j < m >> 3; j++){
			crcTable[(i << (m >> 4)) + j] = (unsigned char)((data & (0xFFu << (j << 3))) >> (j << 3) & 0xFFu);
		}
	}
}
unsigned short CRC16(unsigned char *pDataToCRC, unsigned short crc_init, int length_crc)
{
	unsigned short crc = crc_init;
	unsigned short crc_tmp;
	unsigned char da;
	volatile int i;

	for (i = 0; i < length_crc; i++){
		da = (unsigned char)(crc >> 8);
		crc_tmp = (unsigned short)(crc << 8);
		crc = (unsigned short)(CRC16Table[da^ pDataToCRC[i]] ^ crc_tmp);
	}
	return(crc);
}
void CDlgCommandSheet::LoadFromPLD(CString fileName)
{
	CFile pldFile;
	pldFile.Open(fileName, CFile::modeRead | CFile::typeBinary);
	CMD_WN cmd;
	CmdInfo *pCmdInfo;

	unsigned char buf[64];
	int cnt, idx = 0, iPos;

	cnt = pldFile.GetLength();
	// 	if ((cnt & 0x1F) != 10){
	// 		return;
	// 	}

	cnt >>= 6;
	//	pldFile.Read(buf, 8);//度xw_header[8]
	unsigned short crc = 0;

	// 	for (idx = 0; idx < cnt; idx++){
	// 		pldFile.Read(buf, 32);
	// 		crc = CRC16((unsigned char *)buf, crc, 32);
	// 	}
	// 	pldFile.Read(buf, 2);
	// 	if ((((crc & 0xFF00) >> 8) != buf[0]) || ((crc & 0xFF) != buf[1])){
	// 		MessageBox("文件格式错误！");
	// 		return;
	// 	}

	pldFile.SeekToBegin();
	//	pldFile.Read(buf, 8);



	for (int i = 0; i < cnt; i++){
		pldFile.Read(buf, 64);
		iPos = 4;
		for (int j = 0; j < (buf[3] & 0xF); j++){
			cmd.dev_id = buf[0];
			cmd.bus_flag = buf[2] & 0xF0;
			cmd.immediate_flag = (buf[3] & 0xF0) ? 0 : 1;

			cmd.time = (buf[iPos] << 24) | (buf[iPos + 1] << 16) |
				(buf[iPos + 2] << 8) | buf[iPos + 3];
			iPos += 4;

			cmd.cmd_id = buf[iPos];
			pCmdInfo = m_pCmdInfo[cmd.cmd_id];
			if (!pCmdInfo){
				MessageBox("指令码错误,导入注入数据失败！", "错误");
				return;
			}
			iPos++;
			memcpy(cmd.args, buf + iPos, pCmdInfo->arg_byte_num);
			iPos += pCmdInfo->arg_byte_num;
			//			AddCmdToList(&cmd, -1, 1);
			AddCmdToList(&cmd, m_iRealCmdCnt, 1);
			memcpy(&m_cmdAddInfo[m_iRealCmdCnt], &cmd, sizeof(CMD_WN));
			m_iRealCmdCnt++;
		}
	}
	pldFile.Close();
}
void CDlgCommandSheet::SaveToPLD(CFile *pldFile)
{
	int i, j;
	CMD_WN *pCmd;
	CmdInfo *pCmdInfo;
	int cmdTimeFlag;

	char *pBuf, *pBuf1;
	int cnt;

	int idx32B = 0, eventNum32B = 0, dev32B, bus32B, left32B = 0, time32B, byte32B;//数据注入段采用32字节
	//指令类型：1字节，指令序列计数：1字节；数据注入段顺序、AB总线标志：1字节；事件标识：1字节；数据：28字节
	int cnt32BDev[4] = { 0, 0, 0, 0 }, start32BDev[4] = { 0, 0, 0, 0 }, end32BDev[4] = { 0, 0, 0, 0 };
	int devIndex;

	unsigned int time;

	unsigned char sum;
	char xw_header[8];

	pBuf1 = (char *)malloc(32 * 1024 + 8);
	pBuf = pBuf1;

	cnt = m_ListCtrlCommand.GetItemCount();
	if (cnt > 1024){
		cnt = 1024;
	}
	eventNum32B = 0;
	idx32B = 0;
	char *buffer;
	char buffer1[64];
	for (i = 0; i < cnt; i++){
		buffer = (char*)m_ListCtrlCommand.GetItemData(i);
		memcpy(buffer1, buffer, 64);
		pCmd = (CMD_WN *)m_ListCtrlCommand.GetItemData(i);
		pCmdInfo = m_pCmdInfo[pCmd->cmd_id & 0xFF];
		if (!pCmdInfo){
			MessageBox("无法读取指令定义信息，导出到注入文件失败！", "错误");
			free(pBuf1);
			return;
		}
			
		cmdTimeFlag = 1;
		if (pCmd->immediate_flag){
			cmdTimeFlag = 0;
		}

		if ((left32B < pCmdInfo->arg_byte_num + 5) || //32B full
			(pCmd->dev_id != dev32B) || //device changed
			(pCmd->bus_flag != bus32B) || //bus changed
			(cmdTimeFlag != time32B)){	//time flag changed
			//1.output old 32B
			if (eventNum32B > 0){
				pBuf[0] = dev32B;
				devIndex = ((dev32B & 0x80) >> 6) | ((dev32B & 0x20) >> 5);//a simple hack for retrieving device index.
				if ((dev32B == DEV_ID_KZ) && (time32B == 0)){
					pBuf[1] = 0;
					pBuf[2] = bus32B;
				}
				else{
					pBuf[1] = cnt32BDev[devIndex];
					if (!cnt32BDev[devIndex]){
						start32BDev[devIndex] = idx32B;
					}
					end32BDev[devIndex] = idx32B;
					cnt32BDev[devIndex] ++;
					pBuf[2] = bus32B | 0xC;
				}
				pBuf[3] = eventNum32B;
				if (time32B){
					pBuf[3] |= 0xF0;
				}
				memset(pBuf + byte32B, 0x5A, left32B);
				idx32B++;
				pBuf += 64;
			}

			//2. new 32B
			eventNum32B = 0;//指令计数
			dev32B = pCmd->dev_id;
			bus32B = pCmd->bus_flag;
			left32B = 59;////////////////////////////////////////////////
			byte32B = 4;
			time32B = cmdTimeFlag;
		}
		time = 0;
		if (time32B){
			time = pCmd->time;
		}
		pBuf[byte32B] = (time & 0xFF000000) >> 24;
		pBuf[byte32B + 1] = (time & 0xFF0000) >> 16;
		pBuf[byte32B + 2] = (time & 0xFF00) >> 8;
		pBuf[byte32B + 3] = time & 0xFF;//时间，立即令是0
		pBuf[byte32B + 4] = pCmd->cmd_id;//指令ID
		memcpy(pBuf + byte32B + 5, pCmd->args, pCmdInfo->arg_byte_num);//参数写入
		byte32B += pCmdInfo->arg_byte_num + 5;
		left32B -= pCmdInfo->arg_byte_num + 5;
		eventNum32B++;
	}

	pBuf[0] = dev32B;
	devIndex = ((dev32B & 0x80) >> 6) | ((dev32B & 0x20) >> 5);//a simple hack for retrieving device index.
	if ((dev32B == DEV_ID_KZ) && (time32B == 0)){
		pBuf[1] = 0;
		pBuf[2] = bus32B;//ABcan总线
	}
	else{
		pBuf[1] = cnt32BDev[devIndex];//指令序列计数
		if (!cnt32BDev[devIndex]){
			start32BDev[devIndex] = idx32B;
		}
		end32BDev[devIndex] = idx32B;
		cnt32BDev[devIndex] ++;
		pBuf[2] = bus32B | 0xC;
	}
	pBuf[3] = eventNum32B;
	if (time32B){
		pBuf[3] |= 0xF0;
	}

	memset(pBuf + byte32B, 0x5A, left32B);
	idx32B++;

	for (i = 0; i < 4; i++){
		if (!cnt32BDev[i]){
			continue;
		}
		if (cnt32BDev[i] == 1){
			pBuf1[2 + start32BDev[i] * 64] = pBuf1[2 + start32BDev[i] * 64] & 0xF0;
		}
		else{
			pBuf1[2 + start32BDev[i] * 64] = (pBuf1[2 + start32BDev[i] * 64] & 0xF0) | 0x03;
			pBuf1[2 + end32BDev[i] * 64] = (pBuf1[2 + end32BDev[i] * 64] & 0xF0) | 0x0F;
		}
	}

	pBuf = pBuf1;
	for (i = 0; i < idx32B; i++){
		sum = 0;
		for (j = 0; j < 63; j++){
			sum += pBuf[j];
		}
		pBuf[63] = sum;
		pBuf += 64;
	}


	unsigned short crc;
	pBuf = pBuf1;
	crc = CRC16((unsigned char *)pBuf, 0, idx32B * 64);
	// 	pBuf[idx32B * 32] = (crc & 0xFF00) >> 8;
	// 	pBuf[idx32B * 32 + 1] = crc & 0xFF;


	i = idx32B * 64 + 2;
	xw_header[0] = (i & 0xFF000000) >> 24;
	xw_header[1] = (i & 0xFF0000) >> 16;
	xw_header[2] = (i & 0xFF00) >> 8;
	xw_header[3] = i & 0xFF;
	xw_header[4] = 0x01;
	xw_header[5] = 0x24;
	crc = 0;
	for (i = 0; i < idx32B * 64 + 2; i++){
		crc += (unsigned char)(pBuf[i]);
	}
	xw_header[6] = (crc & 0xFF00) >> 8;
	xw_header[7] = crc & 0xFF;

	//	pldFile->Write(xw_header, 8);
	//	pldFile->Write(pBuf, idx32B * 32 + 2);
	pldFile->Write(pBuf, idx32B * 64);	
	free(pBuf1);
	//	m_Str_send += "\r\n";
}
void CDlgCommandSheet::OnBnClickedButtonOutput()
{
	// TODO: Add your control notification handler code here
	if (!m_ListCtrlCommand.GetItemCount()){
		MessageBox("指令都没有,输出个鬼啊!", "错误");
		return;
	}
	FILE *fp;
	int cmdIndex = 0;
	int cmdCntIn64B = 0;
	int cnt_64B = 0;
	int leftByteOf64B = 64;

	char filter[] = "QCmdList Files(*.bin) |*.bin|QCmdTxt Files(*.txt) |*.txt||";
	CString FileName;
	CFileDialog dlgOpen(FALSE, NULL, TEXT("cmdlist"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST, (LPCTSTR)filter, NULL);
	if (dlgOpen.DoModal() == IDOK)
		FileName = dlgOpen.GetPathName();
	else
		return;
	CString strExt;
	strExt = dlgOpen.GetFileExt();

	if (!strcmp((LPCTSTR)strExt, "")){
		if (dlgOpen.m_ofn.nFilterIndex == 1){
			strExt = "bin";
			FileName += ".bin";
		}
		else{
			strExt = "txt";
			FileName += ".txt";
		}
	}

	CFile pld_file;

	pld_file.Open(FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, NULL);
	SaveToPLD(&pld_file);
	pld_file.Close();
	AfxMessageBox(_T("导出成功！"));
}
//获取指令序列cmd
bool CDlgCommandSheet::GetCMDlist(CMDbuf *cmdbuf)
{
	int idx = 0;
	CMD_WN *pCmd;
	CmdInfo *pCmdInfo;
	int i;
	int cnt = 0;
	char *buffer;
	char buffer1[128];
	cnt = m_ListCtrlCommand.GetItemCount();
	if (cnt > 1024){
		cnt = 1024;
	}
	else if (cnt<=0)
	{
		MessageBox("请选择指令！", "错误");
		return false;
	}
	unsigned short cmdtime = 0;
	for (i = 0; i < cnt; i++)
	{
		buffer = (char*)m_ListCtrlCommand.GetItemData(i);
		memcpy(buffer1, buffer, 64);
		pCmd = (CMD_WN *)m_ListCtrlCommand.GetItemData(i);
		pCmdInfo = m_pCmdInfo[pCmd->cmd_id & 0xFF];
		if (!pCmdInfo){
			MessageBox("指令列表里没有指令！", "错误");
			return false;
		}
		if (!pCmd->immediate_flag)
		{
			cmdtime = pCmd->time;///////////////////时间值需要修改
		}
		*(unsigned short*)(cmdbuf->Buf + idx) = cmdtime;
		idx += 2;
		cmdbuf->Buf[idx] = pCmd->cmd_id;
		idx++;
		memcpy((cmdbuf->Buf + idx), pCmd->args, pCmdInfo->arg_byte_num);
		idx += pCmdInfo->arg_byte_num;
	}
	cmdbuf->CmdBcnt = idx;
	return TRUE;
	
}
//获取单条指令cmd
bool CDlgCommandSheet::GetCMDsingle(CMDbuf *cmdbuf)
{
	int idx = 0;
	CMD_WN *pCmd;
	CmdInfo *pCmdInfo;
	int cnt = 0;
	char *buffer;
	char buffer1[128];
	int cmdIdx = m_ListCtrlCommand.GetSelectionMark();
	if (cmdIdx < 0)
	{
		MessageBox("未选定指令！");
		return false;
	}
	unsigned short cmdtime = 0;
	buffer = (char*)m_ListCtrlCommand.GetItemData(cmdIdx);
	memcpy(buffer1, buffer, 64);
	pCmd = (CMD_WN *)m_ListCtrlCommand.GetItemData(cmdIdx);
	pCmdInfo = m_pCmdInfo[pCmd->cmd_id & 0xFF];
	if (!pCmdInfo){
		MessageBox("指令列表里没有指令！", "错误");
		return false;
	}
	if (!pCmd->immediate_flag)
	{
		cmdtime = pCmd->time;///////////////////时间值需要修改
	}
	*(unsigned short*)(cmdbuf->Buf + idx) = cmdtime;
	idx += 2;
	cmdbuf->Buf[idx] = pCmd->cmd_id;
	idx++;
	memcpy((cmdbuf->Buf + idx), pCmd->args, pCmdInfo->arg_byte_num);
	idx += pCmdInfo->arg_byte_num;
	cmdbuf->CmdBcnt = idx;
}
//获取任务帧参数 开始时间要修改
void CDlgCommandSheet::Getinjectionpara(InjectionInfo *Injectionpara)
{
	CString strtmp;
	if (m_Editstarttime == 0)
	{
		time_t nowtime;
		nowtime = time(NULL);
		m_Editstarttime = m_edit_TaskT0Time + nowtime - m_pInterface->m_taskT0time;;
	}
	m_ComboBoxPackType.GetWindowText(strtmp);
	Injectionpara->type = m_mappackType[strtmp];
	Injectionpara->clientCode = m_EditclientCode;
	Injectionpara->taskCode = m_EdittaskCode;
	Injectionpara->starttime = m_Editstarttime;
	Injectionpara->offsettime0 = m_Editoffsettime0;
	Injectionpara->offsettime1 = m_Editoffsettime1;
	Injectionpara->offsettime2 = m_Editoffsettime2;
	Injectionpara->offsettime3 = m_Editoffsettime3;
	Injectionpara->dataaddr = m_Editdataaddr;
	Injectionpara->timelength = m_Edittimelength;
	
}
//cmd和data打包成应用层格式 InjectionBuffer注入数据包 dataFramBuffer应用层帧
int CDlgCommandSheet::getcmdinjectiondata(CMDbuf cmdbuf, char *InjectionBuffer, char *dataFramBuffer)
{
	InjectionInfo Injectionpara;
	Getinjectionpara(&Injectionpara);
	char filename[256];
	sprintf(filename, (const char*)(LPCTSTR)m_EditTaskInjectionDatafiel);
	int len = m_pInterface->UpdataAndCMDpack(&cmdbuf, filename, InjectionBuffer, Injectionpara, m_check_istaskdata);
	int injectcnt = m_pInterface->injectionFramformat(len, InjectionBuffer, dataFramBuffer);
	return injectcnt;
}
//注入数据生成
void CDlgCommandSheet::OnBnClickedButtonCmdsend()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char filter[] = "QCmdList Files(*.bin) |*.bin|QCmdTxt Files(*.txt) |*.txt||";
	CString cmdFileName;
	CFileDialog dlgOpen(FALSE, NULL, TEXT("list"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST, (LPCTSTR)filter, NULL);
	if (dlgOpen.DoModal() == IDOK)
		cmdFileName = dlgOpen.GetPathName();
	else
		return;
	CString strExt;
	strExt = dlgOpen.GetFileExt();

	if (!strcmp((LPCTSTR)strExt, "")){
		if (dlgOpen.m_ofn.nFilterIndex == 1){
			strExt = "bin";
			cmdFileName += ".bin";
		}
		else{
			strExt = "txt";
			cmdFileName += ".txt";
		}
	}
	char filename[256];
	sprintf_s(filename, 256, cmdFileName);
	char *InjectionBuffer, *dataFramBuffer;
	InjectionBuffer = (char *)malloc(MAX_INJECTTION);//注入数据包
	dataFramBuffer = (char *)malloc(MAX_INJECTTION);//应用层帧格式
	
	CMDbuf cmdbuf;
	bool bret = GetCMDlist(&cmdbuf);
	if (!bret)
	{
		free(InjectionBuffer);
		free(dataFramBuffer);
		return;
	}
	int injectcnt = getcmdinjectiondata(cmdbuf, InjectionBuffer, dataFramBuffer);
	
	FILE *fp;
	if (injectcnt > 0)
	{
		fp = fopen(filename, "wb");
		fwrite(dataFramBuffer, 1, injectcnt, fp);
		fclose(fp);
	}	
	free(InjectionBuffer);
	free(dataFramBuffer);
}

//CAN 注入
void CDlgCommandSheet::OnBnClickedButtonCansend()
{
	// TODO: Add your control notification handler code here

	if (!(m_checkCANinjection || m_checkCANimmedieate))
	{
		MessageBox("请选择发送的指令类型!", "警告");
		return;
	}
	GetDlgItem(IDC_BUTTON_CANSEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SINGELCMDSEDN)->EnableWindow(FALSE);
	m_iscandatasend = TRUE;
	if (m_checkCANinjection)
	{	
		m_pInterface->m_caninjectTaskSend = TRUE;
	}
	if (m_checkCANimmedieate)
	{
		int index = m_ComboCancmd.GetCurSel();
		char buffer[CANSEGLENGTH];
		memcpy(buffer, m_pCANcmdInfo[index]->init_value, m_pCANcmdInfo[index]->arg_byte_num);
		m_pInterface->getCANimmediatebuf(m_pCANcmdInfo[index]->datatype[0], buffer, &m_pInterface->m_sendcandatabuf);
		m_pInterface->m_canimmedieateSend = TRUE;
	}
//	m_pInterface->Releasethread(m_pInterface->m_hMutexCan);
}

//lvds 注入
void CDlgCommandSheet::OnBnClickedButtonlvdsinjectsend()
{
	// TODO: Add your control notification handler code here	
	if (!(m_checkLVDSinjection || m_checkLVDSimmedieate))
	{
		MessageBox("请选择发送的指令类型!", "警告");
		return;
	}
	GetDlgItem(IDC_BUTTONLVDSINJECTSEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SINGELCMDSEDN)->EnableWindow(FALSE);
	m_islvdsdatasend = TRUE;
	if (m_checkLVDSinjection)
	{
		m_pInterface->m_lvdsinjectTaskSend = TRUE;		
		m_pInterface->m_iswaitresponse = TRUE;
	}
	if (m_checkLVDSimmedieate)
	{
		int index = m_ComboLVDScmd.GetCurSel();
		m_pInterface->m_sendLVDSdatabuf.sendLen = m_pLVDScmdInfo[index]->arg_byte_num;
		m_pInterface->m_sendLVDSdatabuf.framecode = m_pLVDScmdInfo[index]->datatype[0];
		memcpy(m_pInterface->m_sendLVDSdatabuf.buffer, m_pLVDScmdInfo[index]->init_value, m_pLVDScmdInfo[index]->arg_byte_num);
		m_pInterface->m_lvdsimmedieateSend = TRUE;
	}
}
//can注入目录
void CDlgCommandSheet::OnBnClickedButtonCaninjectfile()
{
	m_pInterface->m_injectCANtaskcnt = 0;
	size_t index;
	CString cstrsucstring;

	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T("QCmd Files (*.bin;*.dat)|*.bin;*.dat|Task Files(*.txt;*.xml)|*.txt;*.xml||"));

	TCHAR *pBuffer = new TCHAR[MAX_PATH*injectTaskNUM];//最多允许同时打开20个文件
	filedlg.m_ofn.lpstrFile = pBuffer;
	filedlg.m_ofn.nMaxFile = MAX_PATH * injectTaskNUM;
	filedlg.m_ofn.lpstrFile[0] = '\0';

	if (filedlg.DoModal() == IDOK)
	{
		CString cstrfilepath = _T("");
		POSITION pos = filedlg.GetStartPosition();
		while (pos != NULL)
		{
			cstrfilepath = filedlg.GetNextPathName(pos);//取得文件路径
			m_editCANinjectdir += cstrfilepath;
			m_editCANinjectdir += ";";
			m_pInterface->m_injectCanfile[m_pInterface->m_injectCANtaskcnt] = cstrfilepath;
			m_pInterface->m_injectCANtaskcnt++;
		}

	}

	UpdateData(FALSE);

}

//lvds注入目录
void CDlgCommandSheet::OnBnClickedButtonLvdsinfile()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pInterface->m_injectLVDStaskcnt = 0;
	size_t index;
	CString cstrsucstring;

	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T("QCmd Files (*.bin;*.dat)|*.bin;*.dat|Task Files(*.txt;*.xml)|*.txt;*.xml||"));

	TCHAR *pBuffer = new TCHAR[MAX_PATH * injectTaskNUM];//最多允许同时打开20个文件
	filedlg.m_ofn.lpstrFile = pBuffer;
	filedlg.m_ofn.nMaxFile = MAX_PATH * injectTaskNUM;
	filedlg.m_ofn.lpstrFile[0] = '\0';

	if (filedlg.DoModal() == IDOK)
	{
		CString cstrfilepath = _T("");
		POSITION pos = filedlg.GetStartPosition();
		while (pos != NULL)
		{
			cstrfilepath = filedlg.GetNextPathName(pos);//取得文件路径
			m_editLVDSinjectdir += cstrfilepath;
			m_editLVDSinjectdir += ";";
			m_pInterface->m_injectLVDSfile[m_pInterface->m_injectCANtaskcnt] = cstrfilepath;
			m_pInterface->m_injectLVDStaskcnt++;
		}

	}
// 	CFileDialog dlg(TRUE, NULL, NULL,
// 		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOCHANGEDIR,
// 		"QCmdXml Files (*.bin)|*.bin||", NULL);
// 	//		"QCmdList Files (*.cls)|*.cls|QCmdLog Files (*.clg)|*.clg|QCmdXml Files (*.xml)|*.xml||", NULL);
// 	if (dlg.DoModal() != IDOK)
// 		return;
// 
// 	m_editLVDSinjectdir = dlg.GetPathName();
	UpdateData(FALSE);
}


void CDlgCommandSheet::OnBnClickedCheckCaninjection()
{
	// TODO: Add your control notification handler code here

	m_checkCANinjection = !m_checkCANinjection;
	if (m_checkCANinjection)
	{
		m_checkCANimmedieate = FALSE;
	}
	UpdateData(FALSE);
}


void CDlgCommandSheet::OnBnClickedCheckCanimmedieate()
{
	// TODO: Add your control notification handler code here

	m_checkCANimmedieate = !m_checkCANimmedieate;
	if (m_checkCANimmedieate)
	{
		m_checkCANinjection = FALSE;
	}
	UpdateData(FALSE);
}


void CDlgCommandSheet::OnBnClickedCheckLvdsinjection()
{
	// TODO: Add your control notification handler code here
	m_checkLVDSinjection = !m_checkLVDSinjection;
	if (m_checkLVDSinjection)
	{
		m_checkLVDSimmedieate = FALSE;
	}
	UpdateData(FALSE);
}


void CDlgCommandSheet::OnBnClickedCheckLvdsimmedieate()
{
	// TODO: Add your control notification handler code here
	m_checkLVDSimmedieate = !m_checkLVDSimmedieate;
	if (m_checkLVDSimmedieate)
	{
		m_checkLVDSinjection = FALSE;
	}
	UpdateData(FALSE);
}


void CDlgCommandSheet::OnBnClickedCheckIstaskdata()
{
	// TODO: Add your control notification handler code here
	if (m_check_istaskdata)
	{

	}
}

//任务帧数据目录
void CDlgCommandSheet::OnBnClickedButtonTaskdatadir()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOCHANGEDIR,
		"QCmdXml Files (*.dat)|*.dat||", NULL);
	//		"QCmdList Files (*.cls)|*.cls|QCmdLog Files (*.clg)|*.clg|QCmdXml Files (*.xml)|*.xml||", NULL);
	if (dlg.DoModal() != IDOK)
		return;
	m_EditTaskInjectionDatafiel = dlg.GetPathName();
	UpdateData(FALSE);
}


void CDlgCommandSheet::OnBnClickedButtonSingelcmdsedn()
{
	// TODO: Add your control notification handler code here
	
	OnCmdSend();
}


void CDlgCommandSheet::OnBnClickedCheckCantele()
{
	// TODO: Add your control notification handler code here
	m_checkCANtele = !m_checkCANtele;
	if (m_checkCANtele)
	{
		m_checkLVDStele = FALSE;
		KillTimer(TIEMRLVDSSENDTELE);
		SetTimer(TIMERCANSENTELE, 1000, NULL);
	}
	else
	{
		KillTimer(TIMERCANSENTELE);
		m_pInterface->m_canteleframesend = false;
	}
	UpdateData(FALSE);
	
}


void CDlgCommandSheet::OnBnClickedCheckLvdstele()
{
	// TODO: Add your control notification handler code here
	m_checkLVDStele = !m_checkLVDStele;
		
	if (m_checkLVDStele)
	{
		m_checkCANtele = FALSE;
		KillTimer(TIMERCANSENTELE);
		SetTimer(TIEMRLVDSSENDTELE, 1000, NULL);
	}
	else
	{
		m_pInterface->m_lvdsteleframesend = false;
		KillTimer(TIEMRLVDSSENDTELE);

	}
	UpdateData(FALSE);
	
}
void CDlgCommandSheet::setRefreshSheet(CDlgRefreshSheet *Refreshsheet, CReplayConfig *ReplayConfig)
{
	m_dlgRefreshsheet = Refreshsheet;
	m_CReplayConfig = ReplayConfig;
	strcpy((char*)m_CReplayConfig->m_ptelecmdInfo, (char*)m_pMonitorInfo);
	m_CReplayConfig->m_MonitorCmdNum = m_MonitorCmdNum;
	m_dlgRefreshsheet->m_CTeleDisplayfre.displayList(true, m_pMonitorInfo, m_MonitorCmdNum, &m_dlgRefreshsheet->m_ListRefresh,3);
}

void CDlgCommandSheet::OnBnClickedButtonCurrenttime()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	time_t nowtimestart = time(NULL);
	m_pInterface->m_taskT0startcnt = m_edit_TaskT0Time;
	m_pInterface->m_taskT0time = nowtimestart;


// 	CRect rect(m_rect);
// 	int width = 100;
// 	if (width < m_rect.Width())
// 	{
// 		rect.right = rect.left + width;
// 		m_rect.left += width;
// 	}
// 	else//换行
// 	{
// 		m_rect.left = CANVAS_LEFTTOPx;
// 		m_rect.top += LINEHEIGHT;
// 		m_rect.bottom += LINEHEIGHT;
// 
// 		rect.left = CANVAS_LEFTTOPx;
// 		rect.right = rect.left + width;
// 		rect.top = rect.top + LINEHEIGHT;
// 		rect.bottom = rect.top + LINEHEIGHT;
// 	}
// 	rect.bottom += COMBOBOXHEIGHT;
// 	CDynComboBox *pComboBox = new CDynComboBox(987);
// 	pComboBox->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, rect, this, 102);
// 	pComboBox->AddString("1");
// 	pComboBox->AddString("1");
// 	pComboBox->AddString("1");
// 	pComboBox->AddString("1");
}


void CDlgCommandSheet::OnDestroy()
{
	CDialogEx::OnDestroy();
	m_pInterface->closelogFile();
	// TODO: Add your message handler code here

}
