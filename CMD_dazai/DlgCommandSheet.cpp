// DlgCommandSheet.cpp : implementation file
//

#include "stdafx.h"
#include "CMD_dazai.h"
#include "DlgCommandSheet.h"
#include "afxdialogex.h"
#include "DlgAddCommand.h"

// CDlgCommandSheet dialog

IMPLEMENT_DYNAMIC(CDlgCommandSheet, CDialogEx)

CDlgCommandSheet::CDlgCommandSheet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommandSheet::IDD, pParent)
{
	m_iRealCmdCnt = 0;
}

CDlgCommandSheet::~CDlgCommandSheet()
{
}

void CDlgCommandSheet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTROL, m_ListCtrlCommand);
}


BEGIN_MESSAGE_MAP(CDlgCommandSheet, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTROL, &CDlgCommandSheet::OnNMDblclkListControl)
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
		{ "序号", 36 },
		{ "分系统", 60 },
		{ "总线", 40 },
		{ "绝对时间", 180 },
		{ "指令功能描述", 200 },
		{ "指令码", 50 },
		{ "参数长度", 60 },
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
	m_xml.Open("commands.xml");

	// TODO:  Add extra initialization here
	get_control_original_proportion();
	GetCmdInfo(m_pCmdInfo);
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
	//	CDlgAddCommand dlgAddCommand;
// 		dlgAddCommand.m_pCMD = &m_CMDArray[m_iRealCmdCnt];
// 		dlgAddCommand.m_bIsAddNewCmd = true;//控制addCommandsdlg不显示某个指令
// 		dlgAddCommand.m_bHaveChosenCmd = false;//

		if (dlgAddCommand.DoModal() == IDOK)
		{

// 			AddCmdToList(&cmd, m_iRealCmdCnt, 1);
// 			memcpy(&m_cmdAddInfo[m_iRealCmdCnt], &cmd, sizeof(CMD_WN));
			m_iRealCmdCnt++;
		}
	}
	else//修改指令
	{
 		CDlgAddCommand dlgAddCommand(this, &m_cmdAddInfo[iListIndex], 0);
//		CDlgAddCommand dlgAddCommand;
// 		dlgAddCommand.m_pCMD = &m_CMDArray[iListIndex];
// 		dlgAddCommand.m_bIsAddNewCmd = false;//控制addCommandsdlg显示m_CMDArray[iListIndex]指令
// 		dlgAddCommand.m_bHaveChosenCmd = true;
// 		CMD cmdTmp = m_CMDArray[iListIndex];//保存原来CMD指令
		if (dlgAddCommand.DoModal() == IDOK)
		{
// 			m_ListCtrlCommand.DeleteItem(iListIndex);
// 			AddCmdToList(&m_cmdAddInfo[iListIndex], iListIndex, 1);
			//		m_iRealCmdCnt++;			
		}
		m_ListCtrlCommand.SetItemState(iListIndex, 0, -1);
	}


	m_ListCtrlCommand.SetSelectionMark(-1);
	*pResult = 0;
}
void CDlgCommandSheet::ExtractArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length)
{
	int i;

	int byteStart = bitStart / 8;

	bitStart &= 7;

	if (!bitStart){
		for (i = 0; i < length / 8; i++){
			pDst[i] = pSrc[i + byteStart];
		}
		length &= 7;
		if (length){
			pDst[i] = pSrc[i + byteStart] & (0xFF << (8 - length));
		}
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
void CDlgCommandSheet::GetCmdInfo(CmdInfo *m_pCmdInfo[256])
{
	memset(m_pCmdInfo, 0, sizeof(CmdInfo *)* 256);
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
					InsertArgValue(pCmdInfo->init_value, temp, pCmdInfo->bit_start[idx], pCmdInfo->arg_length[idx]);
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
void CDlgCommandSheet::InsertArgValue(unsigned char *pDst, unsigned char *pSrc, int bitStart, int length)
{
	int byteStart, bitEnd;

	unsigned char mask;
	int i;

	if (length <= 0){
		return;
	}

	byteStart = bitStart >> 3;


	bitStart = bitStart & 0x7;
	bitEnd = bitStart + length;

	if (!bitStart){
		for (i = 0; i < length / 8; i++){
			pDst[i + byteStart] = pSrc[i];
		}
		length &= 7;
		if (length){
			mask = 0xFF << (8 - length);
			pDst[i + byteStart] &= ~mask;
			pDst[i + byteStart] |= pSrc[i] & mask;
		}
		return;
	}

	mask = (0xFF >> bitStart);

	if (bitEnd < 8){
		mask &= 0xFF << (8 - bitEnd);
		pDst[byteStart] &= ~mask;
		pDst[byteStart] |= (pSrc[0] >> bitStart) & mask;
		return;
	}

	pDst[byteStart] &= ~mask;
	pDst[byteStart] |= (pSrc[0] >> bitStart) & mask;

	for (i = 1; i < bitEnd / 8; i++){
		pDst[i + byteStart] = (pSrc[i] >> bitStart) | (pSrc[i - 1] << (8 - bitStart));
	}

	bitEnd &= 7;
	if (bitEnd){
		mask = 0xFF << (8 - bitEnd);
		pDst[i + byteStart] &= ~mask;
		if (bitEnd > bitStart){
			pDst[i + byteStart] |= (pSrc[i] >> bitStart) & mask;
		}
		pDst[i + byteStart] |= (pSrc[i - 1] << (8 - bitStart)) & mask;
	}
}