/*
	$Id: Common.h 624 2012-06-29 04:22:44Z lwy $
	$URL: http://172.17.8.1/svn/QUESS/tags/QKDBench-2.2.1/Common.h $
*/
#ifndef __Common_h__
#define __Common_h__

#define MAXCOMMAND 100

#define SEC_PER_100ns	10000000I64
struct CMD
{
	unsigned char ID;
	unsigned char Args[52];
	__int64      ShipTime;

	unsigned char ArgByteLength;
	bool IsLocal;
	CString Note;
	CString SubNote;
};
enum{
		cmdType_indirect = 0,
		cmdType_Plat = 1,
		cmdType_GPS = 2,
		cmdType_shipTime = 3
};
enum
{
		PORT_NUM1 = 5,
		PORT_NUM2 = 6,
		PORT_NUM3 = 7,
		PORT_NUM4 = 8
};
enum
{
	COL_SEQ = 0,
	COL_ABS_TIME,
	COL_DESC,
	COL_CODE,
	COL_ARG_LEN,
	COL_ARG
};
#define injectTaskNUM		  20
#define MAX_ARG_NUM				150
#define MAX_Combx_NUM			100
#define MAX_ARG_LENGTH			256
#define MAX_NAME_LENGTH			256
#define MAX_INJECTTION			17*1024*1024
#define DEV_ID_KZ				0x03
#define DEV_ID_MT				0x33
#define DEV_ID_JF				0x93
#define DEV_ID_XG				0xF3

#define CAN_BUS_A				0x00
#define CAN_BUS_B				0x30
#define TELECOMMANDSHEET		0
#define TELEREFRESHSHEET		1
#define TELEREPALYCONFIG		2
#define TELEFRAMELEN				512		//遥测帧长度

#define TELETYPED3			    0xD3			//工程参数
#define TELETYPED2			    0xD2			//工程参数
#define TELETYPED1			    0xD1			//工程参数
#define D1LENGTH				154
#define D2LENGTH				154
#define D3LENGTH				154
#define TELEFRAMELEN				308		//载荷遥测帧长度

#define MONITORCMDNUM			6

#define ATPCMDSEGTEMP			0X20
#define ATPCMDPARANUM				32

#define LVDSRS422SEGLENGTH		1500 //段长

#define	RECV_SCAN_head0		0x00
#define	RECV_SCAN_head1		0x01
#define	RECV_SCAN_head2		0x02
#define	RECV_SCAN_head3		0x03
#define	RECVFRAMELEN0		0x04
#define	RECVFRAMELEN1		0x05
#define RECVDEVICETYPE		0X06
#define RECVDEVICECODE		0X07
#define RECVFRAMECODE       0X08
#define RECVRESERVE0		0X09
#define RECVRESERVE1		0X0A
#define RECVRESERVE2		0X0B
#define RECV_SCAN_CONTENT	0x0C


#define CRYPT_RESULT_LEVAL_NORMAL		 0x00
#define CRYPT_RESULT_LEVAL_WARNING		 0x01
#define CRYPT_RESULT_LEVAL_FATAL		 0x02
#define CRYPT_RESULT_NORMAL				 0x00
#define CRYPT_RESULT_VERSIONERR			 0x01
#define CRYPT_RESULT_TAGERR				0x02
#define CRYPT_RESULT_SENDIDERR			 0x03
#define CRYPT_RESULT_RECVIDERR			0x04
#define CRYPT_RESULT_LENERR				 0x05
#define CRYPT_RESULT_SEQERR				 0x06
#define CRYPT_RESULT_ALGORITHMERR		  0x07
#define CRYPT_RESULT_MACERR				 0x08
#define CRYPT_RESULT_DECRYPTERR			  0x09
#define CRYPT_RESULT_MAC_CMPERR				 0x10

#define CRYPT_SM4_ECB     0x0401
#define CRYPT_SM4_CBC     0x0402
#define CRYPT_SM4_CFB     0x0404
#define CRYPT_SM4_OFB     0x0408
#define CRYPT_SM4_MAC     0x0410
#define CRYPT_SM3_HMAC     0x0008


//RS422
#define RS422TYPEVARIABLECMD	0XA5	
#define RS422INJECTIONCODE		0X03

extern CListBox *g_pListBoxSendoutput;
extern CListBox *g_pListBoxRecvoutput;

typedef struct
{
	char *Buf;
	int len;
}RecvScanBuf;
typedef struct
{
	char buffer[LVDSRS422SEGLENGTH];
	char framehead[12];
	unsigned short len;
	char scanning;
	int buffercnt;
}lvdsRS422frame;
typedef struct
{
	char buffer[LVDSRS422SEGLENGTH - 1];
	int segmark;
	bool issafebear;
	int segNum;
	int bufcnt;
}dataseginfo;
typedef struct
{
	unsigned short CmdBcnt;
	char Buf[256*64];
}CMDbuf;
typedef struct
{
	unsigned char dev_id;
	unsigned char cmd_id;
	unsigned char immediate_flag;
	unsigned char bus_flag;
	unsigned char args[MAX_ARG_LENGTH];
	__int64 time;
	unsigned char addCntIdx;
}CMD_WN;

typedef struct
{
	unsigned char cmd_name[MAX_NAME_LENGTH];
	unsigned char init_value[MAX_ARG_LENGTH];
	unsigned short bit_start[MAX_ARG_NUM];
	unsigned short arg_length[MAX_ARG_NUM];
	unsigned char arg_name[MAX_ARG_NUM][MAX_NAME_LENGTH];
	unsigned char Arg_CombxName[MAX_ARG_NUM][MAX_Combx_NUM][MAX_NAME_LENGTH];
	unsigned char Arg_CombxCode[MAX_ARG_NUM][MAX_Combx_NUM];
	unsigned char datatype[MAX_ARG_NUM];
	unsigned char cal[MAX_ARG_NUM];;
	int combcntNum[MAX_ARG_NUM];
	unsigned char input_type[MAX_ARG_NUM];
	unsigned char input_ctrl_index[MAX_ARG_NUM];
	double coef[MAX_ARG_NUM];
	unsigned char arg_num;
	unsigned short arg_byte_num;
}CmdInfo;
typedef struct 
{
	char type;
	int clientCode;
	int taskCode;
	int starttime;
	unsigned short timelength;
	unsigned short offsettime0;
	unsigned short offsettime1;
	unsigned short offsettime2;
	unsigned short offsettime3;
	unsigned short dataaddr;
}InjectionInfo;
typedef struct
{
	int hour;
	int minute;
	int second;
}HMSTime;
enum{
	ADCHANNEL_QKD28V = 0,
	ADCHANNEL_QKDMAINSLAVE = 1,
	ADCHANNEL_SYN = 2
};
void ListBoxInfo(CString str, CListBox *ListBoxSendoutput);
unsigned int getUint32BE(unsigned char *pByte);
unsigned int getUint24BE(unsigned char *pByte);
unsigned short getUint16BE(unsigned char *pByte);
void setUint32BE(unsigned int data, unsigned char *pByte);
void setUint24BE(unsigned int data, unsigned char *pByte);
void setUint16BE(unsigned short data, unsigned char *pByte);
void invertBuffer(int len, unsigned char temp[MAX_ARG_LENGTH]);
#endif

