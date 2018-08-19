/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             sms.h                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             E201短消息终端头文件                              */
/*                                                                           */
/*                                                                            */
/*  作    者                                                                  */
/*             向国威                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-10-31                                                     */
/*                                                                            */
/*  历史记录   
			   2001-10-28
			   2001-10-30	                               */
/******************************************************************************/
///////////////////////包含头文件
#include "hwfcl.h"
#include "window.h"
#include "menu.h"
#include "Combox.h"
#include "edit.h"
#include "pswd.h"
#include "listview.h"
#include "messagebox.h"
#include "grid.h"
#include "phoneapp.h"
#include "phonedrv.h"
#include "ring.h"
#include "datadrv.h"
#include "jishi.h"


#define SMS_START_ADDR				0x01080000        //绝对地址 
/* SMS_START_ADDRESS是固话短消息所有存储空间的开始地址,是绝
对地址;以后所有需要存储空间的部件是在它的基础之上加一个相对
地址即可; */

#define SMM_COM_START_ADDR			(SMS_START_ADDR+0x00000000)
#define SMM_COM_INFOS				60     
/* 信箱中能存放的短信息数,最少要求是30条 */
#define SMM_COM_INFO_LEN			174
/* 每条短信息最大长度,第1、2字节为标志字节(自定义),
第3~18字节为发送方号码,第19字节为分隔符0xFF,第20字节
为接收方的子信箱(0,1,2,3),第21~34字节为发送时间,
第35~174字节为信息内容 */

#define SMM_PRI1_START_ADDR			(SMS_START_ADDR+0x000028C8)
#define SMM_PRI1_INFOS				60     
/* 信箱中能存放的短信息数,最少要求是30条 */
#define SMM_PRI1_INFO_LEN			174
/* 每条短信息最大长度,第1、2字节为标志字节(自定义),
第3~18字节为发送方号码,第19字节为分隔符0xFF,第20字节
为接收方的子信箱(0,1,2,3),第21~34字节为发送时间,
第35~174字节为信息内容 */

#define SMM_PRI2_START_ADDR			(SMS_START_ADDR+0x00005190)
#define SMM_PRI2_INFOS				60     
/* 私人信箱2中能存放的短信息数,最少要求是30条 */
#define SMM_PRI2_INFO_LEN			174
/* 每条短信息最大长度,第1、2字节为标志字节(自定义),
第3~18字节为发送方号码,第19字节为分隔符0xFF,第20字节
为接收方的子信箱(0,1,2,3),第21~34字节为发送时间,
第35~174字节为信息内容 */

#define SMM_PRI3_START_ADDR			(SMS_START_ADDR+0x00007A58)
#define SMM_PRI3_INFOS				60     
/* 私人信箱3中能存放的短信息数,最少要求是30条 */
#define SMM_PRI3_INFO_LEN			174
/* 每条短信息最大长度,第1、2字节为标志字节(自定义),
第3~18字节为发送方号码,第19字节为分隔符0xFF,第20字节
为接收方的子信箱(0,1,2,3),第21~34字节为发送时间,
第35~174字节为信息内容 */

#define EMM_START_ADDR				(SMS_START_ADDR+0x0000A320)
/* E_Mail存储部件起始地址 */
#define EMM_INFOS					60
/* E_Mail信箱中能存放的信息数,最少要求是30 */
#define EMM_INFO_LEN				243
/* 每条E_Mail最大长度,第1、2字节为标志字节(自定义),
第3~22字节为发送方的E_Mail地址,第23字节为分隔符0xFF,
第24~243字节为E_Mail内容 */


#define SSM_START_ADDR				(SMS_START_ADDR+0x0000DC14)  
 	/* 订阅的短消息存储部件SSM起始地址 */
#define SSM_INFOS					256
	/* 订阅的短消息存储部件能存放的短信息数,最少要求是99 */
#define SSM_INFO_LEN				241     
/* 订阅的短消息最大长度,第1、2字节为标志字节(自定义),
第3~10字节为ICP代码,第11~20字节为业务代码,第21~40字
节为信息标题,第41字节为分隔符0xFF,第42~241字节为信息
内容 */

#define CSM_START_ADDR				(SMS_START_ADDR+0x0001CD14)
/* 常用语存储部件CSM起始地址 */
#define CSM_INFOS					40
	/* 常用语数,最少要求是20 */
#define CSM_INFO_LEN				43
/* 常用语最大长度,第1、2字节为标志字节(自定义),
第3字节为常用语长度字节,第4~43字节为常用语内容 */

#define SRM_START_ADDR				(SMS_START_ADDR+0x0001D3CC)
	/* 特色振铃声存储部件SRM起始地址 */
#define SRM_LINGS					40
	/* 特色铃声数,最少要求为20 */
#define SRM_LING_LEN				244
/* 特色铃声最大长度,第1、2字节为标志字节(自定义),
第3字节为铃声长度字节,第4~23为标题字节，第24字节为
分隔符，25~244字节为铃声数组，其中奇数字节为音符音阶，
偶数字节为音长 */

#define FBM_START_ADDR				(SMS_START_ADDR+0x0001F9EC)
	/* 话费帐单存储部件FBM起始地址,话费帐单要求6KB空间*/


#define FDBM_START_ADDR				(SMS_START_ADDR+0x000211EC)
	/* 话费详单存储部件FDBM起始地址, 要求8KB空间*/

//									(SMS_START_ADDR+0x000231EC)

//存储部件定义

#define		EMM_Part		0x01	//	EMAIL存储部件
#define		SMM_Part		0x02	//	普通的短消息存储部件
#define		SSM_Part		0x03	//	订阅的短消息存储部件 


#define		CSM_Part		0x06	//	常用语存储部件
#define		SRM_Part		0x07	//	特色铃声存储部件
#define		FBM_Part		0x08	//	话费帐单存储部件
#define		FDBM_Part		0x09	//	话费详单存储部件
 
//DTMF十六进制等值编码表

#define     DTMF_D			0x00	 
#define		DTMF_1			0x01 
#define		DTMF_2			0x02
#define		DTMF_3			0x03 
#define		DTMF_4			0x04 
#define		DTMF_5			0x05 
#define     DTMF_6			0x06	 
#define		DTMF_7			0x07 
#define		DTMF_8			0x08 
#define		DTMF_9			0x09 
#define		DTMF_0			0x0A 
//#define		DTMF_*			0xB 
//#define     DTMF_#			0xC	 
#define		DTMF_A			0x0D 
#define		DTMF_B			0x0E 
#define		DTMF_C			0x0F 


//特殊字定义
#define		INFO_TYPE_SCDATA		0x84		//	下行FSK信息包的信息类型码
//#define		INFO_TYPE_SC_CHECK		0xF0	//	服务器对CPE用户包的接收确认信息类型码		
//#define		INFO_TYPE_CS_CHECK		0xF1	//				
#define		INFO_CHARA_SCDATA		0x01		//	下行FSK信息包的信息特征码	
#define		ACK_CAS					DTMF_A
#define		ACK_SCDATA_FSK_RIGHT	0x01		//	FSK包的ACK:DTMF'D1'
#define		ACK_SCDATA_FSK_ERROR    0x000a//0x0A	D0	//	FSK包的ACK:DTMF'D0'


//时钟定义
#define		TS1			18			//	CPE等待CAS的最长时间24秒
#define		TS5			6			//	CPE等待第一个FSK包的最长时间6秒
#define		TS6			15			//	CPE等待后续FSK包的最长时间15秒
#define		TS7			15			//	CPE等待用户输入超时900秒(15分钟)
#define		TS8			1			//	CPE等待SERVER对FSK消息包的确认最长时间1秒

//重发定义
#define		TIMES_CAS	3			//	SERVER重发CAS次数
#define		TIMES_FSK	3			//	SERVER重发FSK信息包次数

////////消息定义

//////物理层发送给链路层的消息
#define Msg_CID_Coming_Phone_VZ_Chain		11000
#define Msg_EM_VZ_Chain						11001
#define Msg_Receive_FSK_VZ_Chain			11002
#define Msg_Hook_Off_VZ_Chain				11003
#define Msg_Key_EM_VZ						11004
#define Msg_Unsuccess                       11005  
///////链路层发送给物理层的消息
#define Order_Disconnect_Server_Chain		11006
#define Order_Smsdown_Chain_VZ				11007
#define Order_CAS_ACK_Chain_VZ				11008
#define Order_FSK_ACK_Chain_VZ				11009
#define Order_Send_DTMF_Chain_VZ			11010
#define Order_Send_FSK_Chain_VZ				11011
#define Order_Connect_Server_Chain			11012
#define Order_TimeOver_Chain_VZ				11013
//////链路层发送给消息层的消息
#define WM_SMS_ML2_MESSAGE					11014
#define Msg_Data_Chain_Msg					11015
#define Msg_Chain_Msg						11016
#define Msg_TimeOver_Chain_Msg				11017
#define Msg_Unsuccess_Chain_Msg             11018
//////消息层发送给链路层的消息
#define Order_DTMF_Data_Msg_Chain			11019
#define Order_FSK_Data_Msg_Chain			11020
#define Order_Smsdown_Msg_Chain				11021
#define Order_Connect_Server_Msg			11022
#define Order_Disconnect_Server_Msg			11023

///// 其他消息的定义
#define WM_UPDATE_MENU						11024
#define WM_DISPLAY_MSG						11025
#define WM_PAINT_SECOND						11026
///////////链路层发送给消息层的通知消息携带的参数
#define SMS_CENTER_CALL				0x01
#define SMS_CAS						0x02
#define SMS_PHONE_END				0x03
#define SMS_PHONE_STATUS			0x04
#define SMS_PHONE_BUTTON			0x05
#define SMS_USER_SHUT				0x06

/////////////物理层发送给链路层的通知消息携带的参数
#define Msg_CAS_EM					0x07
#define Msg_Ring_End_EM				0x08
#define Msg_State_EM				0x09
#define Msg_TS6_EM					0x10
#define Msg_TS1_EM					0x11
#define Msg_TS5_EM					0x12
/////////////////消息层发送给链路层的通知消息携带的参数
///////////DTMF回送码定义 
#define DTMF_AA			  0x0D0D///	0xDD /////下载成功
#define DTMF_A0			  0x0D0A///	0xDA /////不存在该存储部件
#define DTMF_A1			  0x0D01///	0xD1 /////存储部件满
#define DTMF_A2			  0x0D02///	0xD2 /////私人账号不存在
#define DTMF_A3			  0x0D03///	0xD3 /////用户因以外，终止下载
#define DTMF_A4			  0x0D04///	0xD4 /////其他，用户拒绝


/*  消息层DTMF方式发送短消息函数  */

//BOOL SendSmsDtmf(unsigned * pMsg,int length)



/*  消息层接收短消息函数,  */
//BYTE * ReceiveSms()

/*  物理层DTMF方式发送字节函数	*/
//BOOL SendByteDtmf(BYTE data)

/*  物理层DTMF方式发送比特函数  */
//BOOL SendBitDtmf(BYTE data,int bits)

/*  物理层接收字节函数  */
//static BYTE ReceiveByte() 

#define   ArrayLength  512             // 与物理层的消息缓冲区的长度

////////////////////消息发送函数定义
extern "C" void HwPaintRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 iColor);
extern "C" void HwDrawText(INT16 x, INT16 y, UINT8 iBkColor, UINT8 iForColor, UINT8 iDrawMode, UINT8 iHalign, UINT8 iValign, char *pText);
void SendMessageToSMSL2 ( UINT32,UINT32,UINT32 );//向链路层发送消息
void SendMessageToSMSMsgLayer ( UINT32 ,UINT32 ,UINT32  );///向消息层发送消息
void SendMessageToSMSPhyLayer( UINT32 ,UINT32 ,UINT32  );///向物理层发送消息
/*******************************************************************/
/*                物理层定义                                       */ 
/*******************************************************************/
////// class CSmsPhy
class CSmsPhy : public CFrameWnd
{
public:    
	CSmsPhy ( ){}
	virtual ~CSmsPhy (){}
public:
	BOOL OnCreate ( );
	virtual void Destroy ( )
	{
		ShowWindow ( SW_HIDE );
	}
protected:
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnOrderSmsdownChainVZ(UINT32 , UINT32 , UINT32 );
	MESSAGE_HANDLE void OnOrderCASACKChainVZ(UINT32 , UINT32 , UINT32 );
	MESSAGE_HANDLE void OnOrderFSKACKChainVZ(UINT32 , UINT32 , UINT32 );
	MESSAGE_HANDLE void OnOrderSendDTMFChainVZ(UINT32 , UINT32 , UINT32 );
	MESSAGE_HANDLE void OnOrderSendFSKChainVZ(UINT32 , UINT32 , UINT32 );
	MESSAGE_HANDLE void OnOrderConnectServerChain(UINT32 , UINT32 , UINT32 );
	MESSAGE_HANDLE void OnOrderTimeOverChainVZ ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnOrderDisconnectServerChain ( UINT32,UINT32,UINT32 );


};

/******************************************************************************/
/*函数名：unsigned char Sms_alternate(unsigned data)                          */
/*作  者：王  钰                                                              */
/*时  间：2001.10.30                                                          */
/*功  能：恢复数据原型：交换数据的高、低位                                    */ 
/*备  注：                                                                    */
/******************************************************************************/  
unsigned char Sms_alternate(unsigned data);

/******************************************************************************/
/*函数名：void Sms_Unpacking()                     */
/*作  者：王  钰                                                              */
/*时  间：2001.10.31                                                          */
/*功  能：此函数用于解包： 1 从物理层缓冲区读出数据                           */
/*                         2 恢复数据原型                                     */
/*                         3 分析结构错误，并进行响应                         */
/*                         4 把消息层所需要的内容放入与消息层的缓冲区中。     */
/*备  注：                                                                    */
/******************************************************************************/ 
bool Sms_Unpacking(void);  

////////////////////////////////////////////////////////////////////////////////
////// 消息层定义
////////////////////////////////////////////////////////////
///错误代码定义
#ifndef _SMS_ERROR_CODE
#define _SMS_ERROR_CODE

#define SMS_ERROR					0x100	//////错误
#define SMS_OK						0x101	//////正确
#define SMS_INFORMATION_END			0x102	//////下载完毕
#define SMS_INFORMATION_UEND		0x103	//////。。未完

#define SMS_QUERY					0x104	//////信息查询
#define SMS_DOWNLOAD				0x105	//////信息下载
#define SMS_UPLOAD					0x106	//////信息上传
#define SMS_CHANGE					0x107	//////改变CPE的状态
#define SMS_IDSETTING				0x108	//////询问ID
#define SMS_CPESETTING				0x109	//////询问CPE的设置
#define SMS_SCREENOUT				0x110   //////屏幕输出

#define SMS_TYPE_NORMAL				0x111	//////信息是短消息
#define SMS_TYPE_USERMSG			0x112   //////信息是用户自订阅
#define SMS_TYPE_ERROR				0x113	//////信息类型错误
#define SMS_EMPTY_PHOTO				0x114	//////电话号码为空
#define SMS_INFORMATION				0x115	//////信息是屏幕询问
#define SMS_RING					0x116	//////特色铃声
#define SMS_GRAPHICS				0x117	//////图形管理模块
#define SMS_MONEY					0x118	//////话费账单
#define SMS_MONEYEX					0x119	//////话费详单
#define SMS_MODEL					0x120	//////常用语
#endif


///////////////////////////////////////////////////////////
////短消息数据结构定义
#define SMS_APP_TITLE_HEIGHT		17
#define PASSWORDLENGTH				10
#define SMS_MAX_SAVE_NUMBER			64  ////每个子信箱的最大数量
#define SMS_SUBSCIBE_MAX_NUMBER		100 ////订阅短消息的最大数量
#define SMS_MAX_HAVEPASSWORD_NUMBER	32  ////已发送信箱中最大能存在Flash中的数量
#define SMS_LETTER_MAX_NUMBER		640 ////每个信箱64条短消息,4个信箱共256条
										/////已发信箱共64条(每个信箱16条)
#define SMS_MONEY_MAX_NUMBER		12  /////话费账单，最大3k/256byte
#define SMS_MONEYEX_MAX_NUMBER		32	/////话费祥单，最大8k/256byte

#define SMS_CONTENT_MAX_NUMBER		141 /////短消息内容最大值

/////////////发送方式定义
#ifndef SMS_DTMF
#define SMS_DTMF
#endif

////////////////////////////////////////////////////////////
/// EMail的数据结构的定义
typedef struct tagMailStruct{

	char		szAddress[20]; /////发送方的地址
	char		szMail[220];////////EMail内容

}MAILSTRUCT,*LPMAILSTRUCT;

////////////////////////////////////////////////////////////
/// 订阅短消息的结构定义
typedef struct tagSubscibeStruct{

	UINT32      iconID;//////图标状态
	UINT8		ICPCode[8]; /////ICP代码
	UINT8		ICPOperationCode[10];////不同ICP、不同的信息提供
	char		szTime[17];//////时间
	char		szTitle[22];/////标题
	
	char		szContent[201]; ////订阅短消息的内容

}SUBSCIBESTRUCT,*LPSUBSCIBESTRUCT;

///////常用语结构定义
typedef struct tagDownLoadModel{
	
	//UINT32      iconID;//////图标状态
	//char		szTime[8];//////时间
	//char		szTitle[22];/////标题
	char		szContent[41]; ////订阅短消息的内容

}MODELDOWNLOAD,*LPMODELDOWNLOAD;
//////短消息发送存储结构
typedef struct tagSendSMS{
	
	
	char	szPhone[81];//////最多5个电话号码
	char	szMsg[SMS_CONTENT_MAX_NUMBER];//////短消息内容最多140个char
						/////或者70个汉字
	
}SENDSMS,*LPSENDSMS;

///////话费结构
typedef struct tagMoney {

	UINT32		iconID;//////图表
	
	char		szTitle[21];////标题最多20个字符
	char		szContent[221];/////话单内容最多220个子符

	BOOL		bIsData;//////是否是数据


}MONEY,*LPMONEY;
///////////////////////////////////////////////////////////////
////翻译命令模块
int TranslateCommand ( UINT8 *buffer );
////////////////////////////////////////////////////////////////////////////
///// 信息下载模块:解释消息类型
int SMSInformationDownload ( UINT8 *buffer );
//////////////////////////////////////////////////////////
///发送函数 
int SMSInformationUpload ( LPSMS pst );
///////////////////////////////////////////////////////
////提取短消息
SMS GetSMSIntoBuffer ( UINT8 *buffer,int &iFlag );
//////////////////////////////////////////////////////////////////
///// 业务询问处理模块
void SMSQueryServerID ( UINT32	uRequest );
//////////////////////////////////////////////////////////////////
/////改变CPE的接收状态模块
void SMSChangeReceiveStatus ( );
///////////////////////////////////////////////////////////////////
////系统状态报告模块
void SMSQueryCPEConfig ( UINT8 statuscode );


//////// 类的定义
class CSaveAsWnd;///另存
class CDXXWnd;
class CLetterBoxWnd;
class CModifyLetterWnd;
class CLeaderWnd;////wy
class CGroup_ModWnd;//wy
class CSETWnd;      //// wangyu set
class CCENTERNoWnd; //// wangyu set-center
class CRECEIVEWnd;  //// wangyu set-receive
class CContentListWnd;
class CContentWnd;
class CContent_ModWnd;
class CGRPSDWnd;
class CMODELWnd;


//////////////////电话新业务处理类
#define NEWPHONE_LISTVIEW		20500
class CNewPhone:public CAppWnd
{
private:
	CListView			m_ListView;///菜单显示
	UINT8				m_iMenuType;///菜单的类型
	UINT16				m_iSelectedItem,////纪录已经选中的项
						m_iLimite;////呼出限制的类型
public:
	CNewPhone ( ){}
	~CNewPhone ( ) {}

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

	void UpdateMenu ( );
protected:
	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnSelect ( UINT32,UINT32,UINT32 );

};
///////////////////账单处理类
#define MONEY_GRID				20410
#define MONEY_DELETEALL			20411

class CMoneyWnd:public CAppWnd
{
private:
	CGrid				m_pGrid;////////话费账单网格控件
	UINT8				m_iMoneyStyle;//////话费的类型,0-普通话费 ；1-详细话费

	CButton				m_DeleteBtn;//////全部删除按钮
public:
	CMoneyWnd ( );
	~CMoneyWnd ( );

	BOOL OnCreate ( );
	void OnPaint ( CDC &dc );
	BOOL OnDestroy ( );

	void SetStyle ( UINT8 style ) { m_iMoneyStyle = style; }

protected:

	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDeleteAll ( UINT32,UINT32,UINT32 );

};
///////////////////显示图表、信箱状态窗口
class CDisplayInformation:public CFrameWnd
{
private:
	UINT8		m_DisplayStyle;////要显示的风格,0---表示文本；1---表示图形
	UINT8		m_LetterNumber[4];
	char		m_szPhone[81];////存放电话号码
public:

	CDisplayInformation ( );
	~CDisplayInformation ( );

	
	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
	void SetPhone ( char *str );
	void OnDactive ( )
	{
		CFrameWnd::OnDactive ( );
		Destroy ( );
	}
	

	void SetLetterNumber ( UINT8 array[] )
	{
		m_LetterNumber[0] = array[0];
		m_LetterNumber[1] = array[1];
		m_LetterNumber[2] = array[2];
		m_LetterNumber[3] = array[3];
	}

	void SetStyle ( UINT8 style )
	{
		m_DisplayStyle = style;
	}

protected:

	void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	DECLARE_MESSAGE_MAP() ///消息声明

};

/////////////////// 显示e消息的询问方式
#define SHOWQUESTION_OK				20400
#define SHOWQUESTION_CANCEL			20401
#define SHOWQUESTION_EDIT			20402
#define SHOWQUESTION_GRID			20403




class CShowQuestionWnd:public CAppWnd
{
private:
	
	char				m_szMessage[256];/////提示信息
	UINT8				m_iStyle;////要显示的方式
	
	CButton				m_OKBtn,////确定
						m_CancelBtn;///不确定

	CEdit				m_Edit;////提醒字串
	CGrid				m_pGrid;///多向选择控件
	UINT32				m_IconID;//Icon

public:
	CShowQuestionWnd ( );
	~CShowQuestionWnd ( );

	
	void FillGrid ( );
	void SetMessage ( char *str );
	void SetShowStyle ( UINT8 style );
	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

protected:
	DECLARE_MESSAGE_MAP() ///消息声明

	MESSAGE_HANDLE void OnOKBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnCloseBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );

};
/////////////////// 显示e消息
#define SHOWMESSAGEEX_EDIT			20300
#define SHOWMESSAGEEX_OK			20301

class CShowMessageEx :public CFrameWnd
{
private:
	BOOL			m_bIsNeedTimer;//定时器标志设置
	char			szContent[256];
	CStatic			m_Edit;///////显示e消息
	CButton			m_ReturnBtn;//////确定按钮
	UINT8			m_iSecondNumber,///显示的秒数
					m_bIsDestroy;///是否需要摧毁窗口 2 - 表示简单显示文本，不发送上传消息
					
public:
	
	CShowMessageEx ( );
	~CShowMessageEx () ;

	void ChangeText (  );
	UINT8 GetDestroyFlag ( ) { return m_bIsDestroy; }
	void SetDestroyFlag ( UINT8 b ){ m_bIsDestroy = b; }
	UINT8 GetSecond ( ) {  return m_iSecondNumber; }
	void SetString ( char *str )
	{
		strcpy ( szContent,str );
	}
	
	void SetTimerFlag ( BOOL b )
	{
		m_bIsNeedTimer = b;
	}
	
	
	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	virtual void OnPaint ( CDC &dc );
	
protected:
	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnCloseBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnPaintSecond ( UINT32,UINT32,UINT32 );
};

////////////////////////////////////////////////////////////////
///// class CReadSubscribeSMS
#define READINGSUBSCRIBE_SHOWMENU		23100
#define READINGSUBSCRIBE_DEL			23102
#define READINGSUBSCRIBE_SAVEAS			23103
#define READINGSUBSCRIBE_SENDTO			23104
#define READINGSUBSCRIBE_NAME			23105
#define READINGSUBSCRIBE_CONTENT		23106

class CReadSubscribeSMS:public CAppWnd
{
private:
	CDisplayInformation		*m_pShowTitle;////显示完整的标题
	int						m_CurrentPos;////当前选中项
	SUBSCIBESTRUCT			m_pst; ////////////短消息临时存储结构
	CMenuButton				m_ShowMenuBtn;////激活菜单按钮
	CButton					m_AnswerBtn,/////回复按钮
							m_DelBtn,////////删除按钮
							m_Name;///////姓名
	CMenu					m_Menu;//////////菜单
	CEdit					m_Content;////短消息内容

public:

	CReadSubscribeSMS ( );
	~CReadSubscribeSMS ( );
	
	void SetInformation ( int pos,SUBSCIBESTRUCT st )
	{
		m_CurrentPos = pos;
		m_pst = st; /////设置短消息信息
	}

	BOOL		OnCreate ( );
	BOOL		OnDestroy ( );
	void		OnPaint ( CDC &dc );

protected:
	DECLARE_MESSAGE_MAP()
	///// 消息函数的声明
	MESSAGE_HANDLE void OnShowMenu ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDel ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSaveAs ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSendTo ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnShowTitle ( UINT32,UINT32,UINT32 );
};

/////////////////// e消息
#define SUBSCRIBE_GRID			20200
#define SUBSCRIBE_DELETEALL		20201

class CSubscribeSMS:public CAppWnd  /////浏览e消息
{
private:
	CGrid					m_pGrid;//网格控件
	CReadSubscribeSMS		*m_pRaedSubscribe;///阅读e消息窗口
	CButton					m_DeleteAll;///全部删除按钮 
public:
	/////初始化变量
	CSubscribeSMS ( ):m_pRaedSubscribe(NULL)
	{
		///// TODO: your codes
	}

	~CSubscribeSMS ( ){ }
	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

protected:
	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDeleteAll ( UINT32,UINT32,UINT32 );
};
//////////////////// 家家e
#define SERVICE_GRID		20100
#define SERVICE_EDIT		20101
#define SERVICE_INPUTEDIT	20102
#define SERVICE_OK			20103


class CSMSServiceWnd:public CAppWnd
{
private:
	CListView				m_pGrid;//////菜单项
	
	CStatic					m_Edit;////////显示提示信息
	//CPswdEdit				m_InputEdit;////非线性输入
	CLineButton				m_InputEdit;////非线性输入

	UINT8					m_InputLength,////最大输入长度
							m_InputNumberFlag,////实际输入数量
							m_BackModel,///////回送方式
							m_EnquireModel ;///询问模式

	CButton					m_OKBtn;/////确定

	CShowQuestionWnd		*m_pQuestionWnd;//询问方式窗口
	
public:
	CSMSServiceWnd();
	~CSMSServiceWnd();

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
//	void OnOk ( UINT32,UINT32,UINT32 );

protected:
	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnUpdateMenu ( UINT32,UINT32,UINT32 );	
	MESSAGE_HANDLE void OnDisplayMessage ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnUserInput ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnOKBtn ( UINT32,UINT32,UINT32 );
};
/////////////////// 显示信息类
class CShowMessage:public CFrameWnd
{
private:
	BOOL	m_bIsVisible;////窗口状态
	char szMessage[30];////字符串内容
public:
	CShowMessage ():m_bIsVisible ( FALSE )
	{ 
		strcpy ( szMessage,"" ); 
	}

	~CShowMessage () {}


	BOOL GetVisibleFlag ( ) { return m_bIsVisible; }
	void SetMessage ( char *str )
	{
		if ( !m_bIsVisible ) return;
		strcpy ( szMessage,str );	
		OnPaint ( CDC() );
	}

	BOOL OnCreate ( )
	{
		if ( !CFrameWnd::OnCreate ( ) )
			return FALSE;

		return TRUE;
	}

	BOOL OnDestroy ( )
	{
		m_bIsVisible = FALSE;
		return CFrameWnd::OnDestroy ( );
	}

	void OnPaint ( CDC &dc );
	void ShowWindow(UINT32 nCmdShow = SW_SHOW );

protected:
	DECLARE_MESSAGE_MAP() ///消息声明
};

///////////////////常用语组选择窗口
#define CHOICEMODEL_LISTVIEW	20104
#define CHOICEMODEL_OK			20105
#define CHOICEMODEL_CLOSE		20106

class CChoiceModel:public CFrameWnd
{

private:
	int				m_iSelected;////纪录选择常用语
	CListView		m_pList;////弹出列表
	CIconButton		m_OKBtn,
					m_CloseBtn;
public:
	CChoiceModel ( );
	~CChoiceModel ( );


	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
	
protected:
	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnCurSel ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnOK ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnClose ( UINT32,UINT32,UINT32 );
};
/////////////////// 另存为窗口
#define SAVEAS_MODLE			20110

class CSaveAsWnd:public CAppWnd
{
private:
	int				m_iItem;////选中项
	
	CChoiceModel	*m_pChoiceModel;
	CCheckButton	m_mailbox1,////私人1
					m_mailbox2,////私人2
					m_mailbox3,////私人3
					m_mailbox4,////公共信箱4
					m_mailbox5;////自定义信箱


public:
	CSaveAsWnd ():m_pChoiceModel(NULL)
	{}
	~CSaveAsWnd(){}

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
	void OnOk ( UINT32,UINT32,UINT32 );/////确定
	void SetID ( int item ) { m_iItem = item; }
	int GetID ( ){ return m_iItem; }
	
	void SetCheckButton ( int item,int  bStatus )
	{
		m_iItem = bStatus;

		BOOL b;
		if ( m_iItem != -1 )
			b = TRUE;
		else b = FALSE;

		if ( item == 5 )
			m_mailbox5.SetCheck ( b );


	}
protected:
	DECLARE_MESSAGE_MAP() ///消息声明
	MESSAGE_HANDLE void OnModle ( UINT32,UINT32,UINT32 );
};

//////////
class CButtonEx : public CCtrl
{
protected:
	BOOL    m_bPressed;            //记录按钮当前是否被按下状态
	UINT8	m_bStyle;

public:
	
	void SetStyle ( UINT8 style ) { m_bStyle = style; }
	void SetButtonCaption ( char *strCap )
	{
		strcpy ( m_strName,strCap );
		OnPaint ( CDC() );
	}

	
	BOOL      Create(char *strCaption, CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId=0,BOOL bStyle = FALSE );
	
    ///////////////////////////////////////////////////////////////////////////
	//描述:
	//         按钮消息处理函数
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         按钮重绘函数
	virtual void OnPaint(CDC &dc);
};
/////////////////////////////////////////////////////////////////
//// class CSetNamePassword;
#define SETNAMEPASSWORD_NAME			20000
#define SETNAMEPASSWORD_PASSWORD		20001
#define SETNAMEPASSWORD_REVTYPE			20002
#define SETNAMEPASSWORD_SOUND			20003
#define SETNAMEPASSWORD_SOUNDPATH		20004
#define SETNAMEPASSWORD_SHOWPOPUPLV		20005
#define SETNAMEPASSWORD_POPUPLV			20006
#define SETNAMEPASSWORD_SOUNDSTOP		20008

class CSetNamePassword:public CAppWnd
{
private:
	BOOL				bSoundCtrlFlag;
	CButtonEx			m_Name,
						m_Password,//密码
						m_SoundCtrl;///控制声音的播放

	UINT8				m_iStyle;////当前的页数
	
	BOOL				m_bIsPublic;/////是否公共信箱
	int iItem;
	CPswdEdit			m_SoundName;//声音名字

	CCheckButton		m_RevType,////接收方式控制
						m_Sound,//////是否使用声音提示
						m_EnableSound,////发送与接受是否要声音
						m_EnableReturn;////是否需要服务器回执
						//m_EnablePassword;///是否加密发送

	CIconButton			m_ShowPopupLV;////谈出ListBox
	CPopUpList			m_PopupLV;////ListBox
public:

	CSetNamePassword ();
	~CSetNamePassword ();

	void SetSettingStyle ( UINT8 style ) {
		
		m_iStyle = style;
	}
	
	void SetMailBoxFlag ( BOOL b )
	{
		m_bIsPublic = b;
	}

	void SetItem ( int item ) 
	{
		iItem = item;
	}

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnOk ( UINT32,UINT32,UINT32 );
	void OnPaint ( CDC &dc );
protected:
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明
	MESSAGE_HANDLE void OnNameInput ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnPasswordInput ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnShowLV ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnCurSel (  UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSoundCtrl ( UINT32,UINT32,UINT32 );
};
//////////////////////////////////////////////////////////////////////
///// class CSetLetterBox
#define SETLETTERBOX_LISTVIEW			21000
//#define SETLETTERBOX_SETTING_BTN		21001

class CSetLetterBox:public CAppWnd
{
	
private:
	CListView		m_pListView; ////列表控件

	UINT32			iSelectedItem;////选定信箱
	BOOL			bIsSetting; ////是否有设置信箱的动作
	CSetNamePassword  *m_pSetNamePassword;/////设置密码和名字
public:
	CSetLetterBox ( );
	~CSetLetterBox ( );
	void OnPaint ( CDC &dc );
	virtual BOOL OnCreate();
	BOOL OnDestroy ( );

protected:
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明
	MESSAGE_HANDLE void OnListViewCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSet ( UINT32,UINT32,UINT32 );
};
///////////////////// class CDXXWnd;
#define DXX_LISTVIEW		22000 ///ListView控件


class CDXXWnd  : public CAppWnd
{
public:
	CDXXWnd();
	virtual ~CDXXWnd();

	void OnPaint ( CDC &dc );
	virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private:
	BOOL				bIsDisplay;///是否显示信箱信息
	CLetterBoxWnd		*m_pLetterBoxWnd;////信箱
	CListView			m_pDXXListView;////ListView控件
	CDisplayInformation	*m_pMailBoxInfo;///信箱信息
	
protected:
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明
	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView消息函数

};
////////////////////////// class Select
#define CHOICEMONEY_LISTVIEW			230001


class CChoiceMoneyWnd :public CAppWnd
{
private:
	CMoneyWnd			*m_pMoney;/////话费显示
	CListView			m_ListView;/////列表控件

public:
	
	CChoiceMoneyWnd ( );
	~CChoiceMoneyWnd ( );

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

	void ShowMoney ( UINT8 iStyle );
protected:
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明
	MESSAGE_HANDLE	void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );  /////////ListView控件的函数声明

};
////////////////////////// class CHelloWorld;
////////////////定义短消息的ID号
#define SM_MAININTERFACE_LISTVIEW	23000   ///主界面的ListView控件

class CHelloWorld : public CAppWnd
{
private:
	
	
	CListView			m_MainInterfaceListView;////主菜单的ListView控件
	CDXXWnd				*m_pDXXWnd;////短消息子窗口
	CSETWnd				*m_pSETWnd;////设置子窗口  wangyu
	CSubscribeSMS		*m_pSubscribeWnd;///////e消息窗口
	CChoiceMoneyWnd		*m_pChoiceMoney;////////话单选择窗口
	CNewPhone			*m_pNewPhone;//////电话新业务


public:
	CHelloWorld ( );
	~CHelloWorld ( );


	virtual BOOL OnCreate();
	BOOL InitSMMainInterface ( );
	void OnPaint(CDC &dc);
	
	BOOL OnDestroy ( );
	void OnMoney ( UINT8 iStyle );
	
protected:

	
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明
	MESSAGE_HANDLE	void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );  /////////ListView控件的函数声明
	
};

////////////////////////////////////////////////////////////////
///// class CReadingSMS
#define READING_SHOWMENU		26000
#define READING_ANSWER			26001
#define READING_DEL				26002
#define READING_SAVEAS			26003
#define READING_SENDTO			26004
#define READING_SHOWPHONE		26005
#define READING_CONTENT			26006

class CReadingSMS:public CAppWnd
{
private:

	SMS						m_pst; ////////////短消息临时存储结构
	CMenuButton				m_ShowMenuBtn;////激活菜单按钮
	CButton					m_AnswerBtn,/////回复按钮
							m_DelBtn,////////删除按钮
							m_ShowPhone;///显示完整的电话号码
	short					m_iDataID;//////当前的数据ID
	CMenu					m_Menu;//////////菜单
	CStatic					m_Content;////短消息内容

	char					m_szName[16];/////收件人的姓名
	CDisplayInformation		*m_pShowPhone;////显示完整电话号码窗口
public:

	CReadingSMS ( );
	~CReadingSMS ( );
	
	void  SetDataID ( short data ) { m_iDataID = data ; }
	void  SetSMSInformation ( SMS st )
	{
		m_pst = st; /////设置短消息信息

		SetName ( m_pst.szPhone );
	}

	void SetName ( char *str )
	{
		int i;
		if ( strlen ( str ) > 10 )
		{
			for ( i = 0;i < 10;i ++ )
			{
				m_szName[i] = str[i];
				if ( str[i] == '\0' ||
					 str[i] == '*' ||
					 str[i] == '#' ) 
					  if ( i != 0 ) break;
			}

			m_szName[i] = '\0';

		}else strcpy ( m_szName,str );
		
		
	}

	BOOL		OnCreate ( );
	BOOL		OnDestroy ( );
	void		OnPaint ( CDC &dc );

protected:
	DECLARE_MESSAGE_MAP()
	///// 消息函数的声明
	MESSAGE_HANDLE void OnShowMenu ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDel ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnAnswer ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSendTo ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSaveAs ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnShowPhone ( UINT32,UINT32,UINT32 );
};

/////////// class CLetterBoxWnd;
#define LETTERBOX_NEW_BTN			24000//// New ID
#define LETTERBOX_REV_BTN			24001/// Rev
#define LETTERBOX_SEND_BTN			24002/// Send
#define LETTERBOX_GRIDVIEW			24003///// ListView 
#define LETTERBOX_SENDED_BTN		24004//// Sended
#define LETTERBOX_DELETE_BTN		24005

class CLetterBoxWnd  :public CAppWnd
{
private:
	////////变量定义
	BOOL			m_bIsNeedReDraw;/////是否在信箱中重画
	int				iUnreadNumber;
	CButton			m_NewBtn,/////新增
					m_DeleteAll;///删除所有短信
	CPages			m_RevBtn,////收信箱
					m_SendBtn,////发信箱
					m_SendedBtn;///已经发送的信箱


	CGrid			m_pGridView;////ListView
	

	/////定义信件的结构
	struct tagLetterStatus{
		int iType;////// 0 ---- 未读
				  /////  1 ---- 已读
		          /////  2 ---- 来电转移
		          ////   3 ---- 加锁
		char szTitle[64];
		char szPassword[10];/// 密码

	}LetterStatus;

public:
	CLetterBoxWnd();
	virtual ~CLetterBoxWnd();

	void SetFlag ( BOOL b )
	{
		m_bIsNeedReDraw = b;
	}
	void OnPaint ( CDC &dc );
	void PaintGrid ( CDC &dc,BOOL bIsDraw );
	BOOL OnCreate ( );
	void FillListView ( int iType  );
	BOOL OnDestroy ( );
protected:
	
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE	void OnShowMenu ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE	void OnNew ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnRevBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE  void OnSendBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE  void OnSendedBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE  void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE  void OnDelete ( UINT32,UINT32,UINT32 );
	
};

//////////////////////////////// class ModifyLetterWnd;
#define MODIFYLETTER_MENU_BTN		25000
#define MODIFYLETTER_SEND_BTN		25001
#define MODIFYLETTER_SAVE_BTN		25002
#define MODIFYLETTER_WORD			25003
#define MODIFYLETTER_SEARCH_BTN		25005
#define MODIFYLETTER_COMBOX			25006
#define MODIFYLETTER_EDIT			25007
#define MODIFYLETTER_GROUP_BTN      25008
#define MODIFYLETTER_SAVEAS			25010
#define MODIFYLETTER_SHOWLIST		25011
#define MODIFYLETTER_PHONE			25012			
#define MODIFYLETTER_MENUSAVE		25013


class CModifyLetterWnd  :public CAppWnd
{
private:
	////////
	CMenuButton			m_MenuBtn;/////激活菜单
	CIconButton			m_PopupListBox;////激活ListBox
	CButton				m_SendBtn,/////发送按钮
						m_SearchBtn,///查找按钮
						m_SaveBtn,/////保存按钮,当标示为真时变为删除按钮
						m_GroupBtn;////群发按钮
	CMenu				m_Menu;////////菜单
	//CCombox				*m_pCombox;//////组合控件
	CPswdEdit			m_Edit,////编辑控件
						m_Phone;///电话编辑控件
	CPopUpList			m_List;///列表控件

	bool				bIsSendStatus,////标示是否为发送状态
						bIsSaved;////是否已经保存
	
public:

	CModifyLetterWnd();
	virtual ~CModifyLetterWnd();

	void SetFlag ( bool b ) 
	{
		bIsSendStatus = b;
	}
	void OnPaint ( CDC &dc );
	BOOL OnCreate ( );
	BOOL OnDestroy ( );
protected:
	 DECLARE_MESSAGE_MAP()
	 MESSAGE_HANDLE	void OnSend ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	 MESSAGE_HANDLE void OnComboxInput ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	 MESSAGE_HANDLE void OnEditInput ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	 MESSAGE_HANDLE void OnShowMenu ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	 MESSAGE_HANDLE void OnSelectWord ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnSaveAs ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnSave ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnShowList ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnCurSel ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnGroupSend ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnSearchCard ( UINT32,UINT32,UINT32 );
	 MESSAGE_HANDLE void OnMenuSave ( UINT32,UINT32,UINT32 );
};

////////////////////////////  设置 --------  王钰编写
//////////////////////////////////////////////////////////////////////////
////////////
////////////  class CSETWnd 09_设置  类定义  start  wangyu       ////////
////////////
/////////////////////////////////////////////////////////////////////////
#define SET_LISTVIEW		20010 ///ListView控件 
class CSETWnd  : public CAppWnd
{
public:
	CSETWnd();
	virtual ~CSETWnd();
	void OnPaint ( CDC &dc );
    virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private: 
	CSetNamePassword	*m_pSetOther;    ////设置声音、绘制
	CSetLetterBox		*m_pSetLetterBox;////设定私人信箱
	CListView			m_pSETListView;  //  ListView控件
    CCENTERNoWnd        *m_pCenterNoWnd;  //  信息中心号码子窗口
   // CRECEIVEWnd         *m_pReceiveWnd;   //  接收子窗口
protected:    
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明 
	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView消息函数
};
/////////// class CSETWnd       09_设置  类定义       end    wangyu /////// 

/////////// class CRECEIVEWnd  09_设置_接收 类定义 start  wangyu /////// 
  /*
#define    Rev_DXXFULL        20013
#define    Rev_DXXARRIVED     20014
#define    Rev_DXXAUTOREPLAY  20015
#define    Rev_DXXSOUND       20016
// 此结构中存放：接收设置中复选框、组合框的标志  
struct SetRevFlag{
  BOOL    DxxFullFlag;
  BOOL    DxxArrivedFlag;
  BOOL    DxxAutoReplyFlag;
  UINT32  DxxSoundFlag;
};

class CRECEIVEWnd  : public CAppWnd
{
private:
	CCheckButton     m_DxxFull,     /////短信息满时，不接收新短信息
		             m_DxxArrived,  /////短信息到达时，声音提示
					 m_DxxAutoReply;/////收到短信自动回复：我收到你的短信 	
	CCombox          m_DxxSound;    ////提示声音选项菜单 	              
public:
	CRECEIVEWnd();
	virtual ~CRECEIVEWnd();
	void OnPaint (CDC &dc);
	BOOL OnCreate ();
	void FillListView (int iType);
	void Destroy ();
	void OnOk(UINT32, UINT32, UINT32);
protected:
	DECLARE_MESSAGE_MAP() 	
		MESSAGE_HANDLE void OnDxxFull ( UINT32  ,UINT32  ,UINT32  );
	    MESSAGE_HANDLE void OnDxxArrived ( UINT32  ,UINT32  ,UINT32  );
	    MESSAGE_HANDLE void OnDxxAutoReplay(UINT32  ,UINT32  ,UINT32);
	   // MESSAGE_HANDLE void OnDxxSound ( UINT32  ,UINT32  ,UINT32  );
};
   */
///////////  class CRECEIVEWnd  09_设置_接收  类定义 end    wangyu //// 

///////////  class CCENTERNownd  09_设置_信息码   类定义 start  wangyu //// 
#define  CENTERNoWND_REV			 20011
#define  CENTERNoWND_SEND			 20012

class CCENTERNoWnd : public CAppWnd
{
private:

       CLineButton		m_RevEdit;   ///// 接收号码
	   CLineButton		m_SendEdit; //// 发送号码
public:
	CCENTERNoWnd();
	virtual ~CCENTERNoWnd();
	void OnPaint ( CDC &dc );
	BOOL OnCreate ( );
	void FillListView ( int iType  );
	void Destroy ( );
	void OnOk(UINT32, UINT32, UINT32);
protected:
	
	DECLARE_MESSAGE_MAP() 
	MESSAGE_HANDLE void OnRevEditInput ( UINT32  ,UINT32  ,UINT32  );
	MESSAGE_HANDLE void OnSendEditInput ( UINT32  ,UINT32  ,UINT32  );
};
//////////  class CCENTERNownd  09_设置_信息码     类定义  end   wangyu ////

/////////////////////////////////////////////////////////////////////////
//////////
//////////  class CMODELWnd  01_常用语    类定义  start   wangyu       ////
//////////
///////////////////////////////////////////////////////////////////////// 
  
#define MODEL_LISTVIEW         20020   //model
#define MODELCon_LISTVIEW      20050   //model-content
#define MODELMod_LISTVIEW      20051   //model-modify
#define MODEL_ComBox           20021
#define MODEL_NewButton        20022///新建按钮
#define MODEL_Rename           20023///重命名
#define MODEL_Delete           20024///删除
#define MODEL_Taxis            20025///排序
#define MODEL_RenameButton     20026///改名按钮
#define MODEL_DeleteButton     20027///删除按钮
#define MODEL_TaxisUpButton    20028///排序按钮--上移
#define MODEL_TaxisDwButton    20029///排序按钮--下移
//////////////////
     
#define   WYCALL                      1   /// 修改内容(WYCALL)
#define   SYJCALL                     2   /// 调用内容(SYJCALL)
#define   ENDCALL                     0   /// 销毁窗口(ENDCALL) 
 
class CMODELWnd : public CAppWnd
{
public:
	CMODELWnd();
	virtual ~CMODELWnd();
	void OnPaint ( CDC &dc );
    virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private: 
	CListView			m_pModelListView;  //  ListView控件 	

	CContentWnd         *m_ContentWnd; // 内容控件 
	CContent_ModWnd     *m_pDeleteWnd, // 删除
	                    *m_pRenameWnd, // 改名
	                    *m_pTaxisWnd ; // 排序
	
	CMenu                m_Content;      
	CMenuButton          m_ConMB;        
	CButton              m_pConNewButton;
 
protected:    
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明 
	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView消息函数
    MESSAGE_HANDLE  BOOL OnGRPSDNewBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnShowMenuBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnRename ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnTaxis  ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};
//////////  class CMODELWnd   01_常用语         类定义  end   wangyu //// 

////////////class CContentWnd  01_常用语_内容    类定义  start   wangyu /////
#define   MODIFYCONTENT        20043 
#define    Content_NewButton    20047
 
class CContentWnd: public CAppWnd
{
private: 	              
    CGrid              m_pGridView;
	CContentListWnd    *m_ContentList;

	CButton             m_pNewButton;
public:
	CContentWnd();
	virtual ~CContentWnd();
	void OnPaint (CDC &dc);
	BOOL OnCreate (); 
	void Destroy ();
    BOOL OnSMSaveAs(short, char *);
//	void OnOk(UINT32, UINT32, UINT32);

protected:
	DECLARE_MESSAGE_MAP() 	
		 
  	MESSAGE_HANDLE  void OnModifyContent ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  UINT32 OnNewContent ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};
////////////class CContentWnd      01_常用语_内容    类定义  end   wangyu /////

////////////class CContentListWnd      01_常用语_内容_查看    类定义  start   wangyu /////
#define   ConList_DeleteButton       20044
#define   ConList_ModifyButton       20045
#define   ConList_SendButton         20046
#define   LISTCONTENT                20048

class CContentListWnd: public CAppWnd
{
public:
	CContentListWnd();
	virtual ~CContentListWnd();
	void OnPaint (CDC &dc); 
	BOOL OnCreate (); 
	void Destroy ();
private:
	CPswdEdit			m_Edit;////编辑控件
	CButton             m_pListDelete,
		                m_pListModify,
						m_pListSend;
protected:
	DECLARE_MESSAGE_MAP() 
		    MESSAGE_HANDLE  UINT32 OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
            MESSAGE_HANDLE  UINT32 OnModify ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	        MESSAGE_HANDLE  UINT32 OnSend   ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );

};

////////////class CContenListtWnd      01_常用语_内容_查看     类定义  end   wangyu /////

////////////class CContent_ModWnd  01_常用语_修改    类定义  end   wangyu /////
class CContent_ModWnd : public CAppWnd
{
private: 	              
	CListView			m_pConModifyListView;  //  ListView控件 
 
	CButton              m_pModifyButton,
		                 m_pUpButton,
					 	 m_pDwButton;

    CPswdEdit            m_Edit;  ////编辑控件，用于:改名
public:
	CContent_ModWnd();
	virtual ~CContent_ModWnd();
	void OnPaint (CDC &dc);
 
	BOOL OnCreate (); 
	void Destroy ();
	void OnOk(UINT32, UINT32, UINT32);
    UINT32 OnDeleteModel ( UINT32, UINT32, UINT32 );
    UINT32 OnRenameModel ( UINT32, UINT32, UINT32 );
protected:
	DECLARE_MESSAGE_MAP() 
 	  MESSAGE_HANDLE  UINT32 OnModifyModel ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );	  
	  MESSAGE_HANDLE  UINT32 OnTaxisModelUp ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	  MESSAGE_HANDLE  UINT32 OnTaxisModelDw ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};

 
////////////class CContent_ModWnd  01_常用语_修改    类定义  end   wangyu /////
 
///////////////////////////////////////////////////////////////////////
//////////
//////////  class CGRPSDWnd  02_群发    类定义  start   wangyu     ////
//////////
/////////////////////////////////////////////////////////////////////// 
#define GRPSD_LISTVIEW         20030 
#define GRPSDLeader_LISTVIEW   20052
#define GRPSDModify_LISTVIEW   20053 
#define GRPSD_ComBox           20031
#define GRPSD_NewButton        20032///新建按钮
#define GRPSD_Rename           20033///重命名
#define GRPSD_Delete           20034///删除
#define GRPSD_Taxis            20035///排序
#define GRPSD_RenameButton     20036///改名按钮
#define GRPSD_DeleteButton     20037///删除按钮
#define GRPSD_TaxisUpButton    20038///排序按钮--上移
#define GRPSD_TaxisDwButton    20039///排序按钮--下移
//////////////////////////////////////////////////////////


class CGRPSDWnd : public CAppWnd
{
public:
	CGRPSDWnd();
	virtual ~CGRPSDWnd();
	void OnPaint ( CDC &dc );
    virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private: 
	CListView			m_pGrpSdListView;  //  ListView控件 
	CLeaderWnd          *m_pLeaderWnd; //一级领导控件 
	CGroup_ModWnd       *m_pDeleteWnd, //删除
	                    *m_pRenameWnd, //改名
	                    *m_pTaxisWnd ; //排序
                          
	CMenu                m_GrpSd;     // 
	CMenuButton          m_GrpSdMB;        
	CButton              m_pGSNewButton;
	CScrollBar         m_pGrpSdScrllBar;  //
protected:    
	DECLARE_MESSAGE_MAP()
	////////消息函数的声明 
      	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView消息函数
        MESSAGE_HANDLE  BOOL OnGRPSDNewBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnShowMenuBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnRename ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnTaxis  ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};
//////////  class CGRPSDWnd   02_群发         类定义  end   wangyu //// 

////////////class CLeaderWnd  02_群发_领导    类定义  start   wangyu /////
#define   MODIFYLEADER_EDIT           20060 
#define   MODIFYLEADERNAME_EDIT       20061
#define   MODIFYLEADER_NameCard       20062
#define	  MODIFYLEADER_SEND			  20063

class CLeaderWnd: public CAppWnd
{
private: 
	 CGrid              m_pGridView;       
	 CButton			m_SendBtn;////发送按钮
public:
	CLeaderWnd(); 
	virtual ~CLeaderWnd();
	void OnPaint (CDC &dc);
	BOOL OnCreate (); 
	BOOL OnCallFormCard(short );
	BOOL OnModifyName(short);
	BOOL OnModifyPhone(short );
    void Repaint();
	void Destroy ();
	void OnOk(UINT32, UINT32, UINT32);

protected:
	DECLARE_MESSAGE_MAP() 	
    MESSAGE_HANDLE  UINT32 OnModifyLeader ( UINT32, UINT32, UINT32 );
	MESSAGE_HANDLE  void OnSend ( UINT32,UINT32,UINT32 );
};
////////////class CLeaderWnd  02_群发_领导    类定义  end   wangyu /////

////////////class CGroup_ModWnd  02_群发_修改    类定义  end   wangyu /////

class CGroup_ModWnd : public CAppWnd
{
private: 	              
	CListView			 m_pGrpModifyListView;  //  ListView控件 
 	CButton              m_pGSModifyButton,
		                 m_pGSUpButton,
					 	 m_pGSDwButton;
    CPswdEdit            m_Edit;  ////编辑控件，用于:改名
public:
	CGroup_ModWnd();
	virtual ~CGroup_ModWnd();
	void OnPaint (CDC &dc);
	void OnPaintTemp(CDC &dc); 
	BOOL OnCreate (); 
	void Destroy ();
	void OnOk(UINT32, UINT32, UINT32);
    UINT32 OnDeleteGroup ( UINT32, UINT32, UINT32 );
    UINT32 OnRenameGroup ( UINT32, UINT32, UINT32 );
 
protected:
	DECLARE_MESSAGE_MAP() 		 
		MESSAGE_HANDLE  UINT32 OnModifyGroup ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );      
    	MESSAGE_HANDLE  UINT32 OnTaxisGroupUp ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  UINT32 OnTaxisGroupDw ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};
////////////class CGroup_ModWnd  02_群发_修改    类定义  end   wangyu /////

/////////////////////// 链路层
////////////////////// class CSMSL2Wnd;
class CSMSL2Wnd  :public CFrameWnd
{
public:
	CSMSL2Wnd();
	virtual ~CSMSL2Wnd();
	BOOL OnCreate ( );
	virtual void Destroy ( )
	{
		ShowWindow ( SW_HIDE );
	}
protected:
	DECLARE_MESSAGE_MAP()

	MESSAGE_HANDLE void OnUpload ( UINT32,UINT32,UINT32);
	MESSAGE_HANDLE void OnDTMF_Data ( UINT32,UINT32,UINT32);
	MESSAGE_HANDLE void OnFSK_Data ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnSmsdown_Msg ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnSMS_Come ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnConnect_Server_Chain ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnDisconnect_Server_Chain ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnDeal_L1Msg ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnReceive_FSK ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnHook_Off ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnKey_EM ( UINT32 ,UINT32 ,UINT32  );
    MESSAGE_HANDLE void OnUnsuccess ( UINT32 ,UINT32 ,UINT32  );


};

//////////////////////////////消息层 SYJ
class CSMSMessage  :public CFrameWnd
{

public:
	CSMSMessage();
	virtual ~CSMSMessage();
	BOOL OnCreate ( );
	virtual void Destroy ( )
	{
		ShowWindow ( SW_HIDE );
	}

protected:
	DECLARE_MESSAGE_MAP() ///////////////消息映射
	MESSAGE_HANDLE void OnDataChainMsg ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnChainMsg ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnTimerOver ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnUnsuccessChainMsg ( UINT32,UINT32,UINT32 );
};


////wangyu1228
/////////////class CDrawPicWnd  图形管理strat  wangyu   ////////////
class CDrawPicWnd : public CAppWnd
{
private:
    INT16  iX0, iY0;  // 坐标原点，相对于窗口的左上角
	INT16  m_iCurX, m_iCurY;    //当前位置
	UINT8  m_iPenColor;         //笔颜色
    UINT8  m_iBackColor;        //背景色
    UINT8  m_iDrawMode;         //做图属性
    UINT8  m_iPenWidth;         //笔宽度     
    UINT8  m_iLineStyle;        //画线类型 
public:
	CDrawPicWnd();
    virtual ~CDrawPicWnd();
    BOOL OnCrate();
	void OnPaint (CDC &dc);
    void Destroy();
//
	 void DrawReset();
	 void DrawClear();
	 void DrawStyle(UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle);
     void SetOrigin(INT16 x0, INT16 y0);	 
	 void DrawPoint(INT16 iX, INT16 iY);	  
	 void Drawline(INT16 x0, INT16 y0, INT16 x1, INT16 y1, 
                   UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle);
	 void DrawRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
	 void DrawKPoint(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
	 void DrawEllipse(INT16 x0, INT16 y0, INT16 x1, INT16 y1);//椭圆
	  

protected:
  DECLARE_MESSAGE_MAP()

};
/////////////class CDrawPicWnd 图形管理  end     wangyu   ////////

//////////////////////////////////////// SMS END
