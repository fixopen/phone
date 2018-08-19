/******************************************************************************/
/*                                                                            */
/*  ��Ȩ����(c)  2001�����Ƽ�                                                 */
/*                                                                            */
/******************************************************************************/
/*  �� �� ��                                                                  */
/*             sms.h                                                       */
/*                                                                            */
/*  �� �� ��                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             E201����Ϣ�ն�ͷ�ļ�                              */
/*                                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �����                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             2001-10-31                                                     */
/*                                                                            */
/*  ��ʷ��¼   
			   2001-10-28
			   2001-10-30	                               */
/******************************************************************************/
///////////////////////����ͷ�ļ�
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


#define SMS_START_ADDR				0x01080000        //���Ե�ַ 
/* SMS_START_ADDRESS�ǹ̻�����Ϣ���д洢�ռ�Ŀ�ʼ��ַ,�Ǿ�
�Ե�ַ;�Ժ�������Ҫ�洢�ռ�Ĳ����������Ļ���֮�ϼ�һ�����
��ַ����; */

#define SMM_COM_START_ADDR			(SMS_START_ADDR+0x00000000)
#define SMM_COM_INFOS				60     
/* �������ܴ�ŵĶ���Ϣ��,����Ҫ����30�� */
#define SMM_COM_INFO_LEN			174
/* ÿ������Ϣ��󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3~18�ֽ�Ϊ���ͷ�����,��19�ֽ�Ϊ�ָ���0xFF,��20�ֽ�
Ϊ���շ���������(0,1,2,3),��21~34�ֽ�Ϊ����ʱ��,
��35~174�ֽ�Ϊ��Ϣ���� */

#define SMM_PRI1_START_ADDR			(SMS_START_ADDR+0x000028C8)
#define SMM_PRI1_INFOS				60     
/* �������ܴ�ŵĶ���Ϣ��,����Ҫ����30�� */
#define SMM_PRI1_INFO_LEN			174
/* ÿ������Ϣ��󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3~18�ֽ�Ϊ���ͷ�����,��19�ֽ�Ϊ�ָ���0xFF,��20�ֽ�
Ϊ���շ���������(0,1,2,3),��21~34�ֽ�Ϊ����ʱ��,
��35~174�ֽ�Ϊ��Ϣ���� */

#define SMM_PRI2_START_ADDR			(SMS_START_ADDR+0x00005190)
#define SMM_PRI2_INFOS				60     
/* ˽������2���ܴ�ŵĶ���Ϣ��,����Ҫ����30�� */
#define SMM_PRI2_INFO_LEN			174
/* ÿ������Ϣ��󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3~18�ֽ�Ϊ���ͷ�����,��19�ֽ�Ϊ�ָ���0xFF,��20�ֽ�
Ϊ���շ���������(0,1,2,3),��21~34�ֽ�Ϊ����ʱ��,
��35~174�ֽ�Ϊ��Ϣ���� */

#define SMM_PRI3_START_ADDR			(SMS_START_ADDR+0x00007A58)
#define SMM_PRI3_INFOS				60     
/* ˽������3���ܴ�ŵĶ���Ϣ��,����Ҫ����30�� */
#define SMM_PRI3_INFO_LEN			174
/* ÿ������Ϣ��󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3~18�ֽ�Ϊ���ͷ�����,��19�ֽ�Ϊ�ָ���0xFF,��20�ֽ�
Ϊ���շ���������(0,1,2,3),��21~34�ֽ�Ϊ����ʱ��,
��35~174�ֽ�Ϊ��Ϣ���� */

#define EMM_START_ADDR				(SMS_START_ADDR+0x0000A320)
/* E_Mail�洢������ʼ��ַ */
#define EMM_INFOS					60
/* E_Mail�������ܴ�ŵ���Ϣ��,����Ҫ����30 */
#define EMM_INFO_LEN				243
/* ÿ��E_Mail��󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3~22�ֽ�Ϊ���ͷ���E_Mail��ַ,��23�ֽ�Ϊ�ָ���0xFF,
��24~243�ֽ�ΪE_Mail���� */


#define SSM_START_ADDR				(SMS_START_ADDR+0x0000DC14)  
 	/* ���ĵĶ���Ϣ�洢����SSM��ʼ��ַ */
#define SSM_INFOS					256
	/* ���ĵĶ���Ϣ�洢�����ܴ�ŵĶ���Ϣ��,����Ҫ����99 */
#define SSM_INFO_LEN				241     
/* ���ĵĶ���Ϣ��󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3~10�ֽ�ΪICP����,��11~20�ֽ�Ϊҵ�����,��21~40��
��Ϊ��Ϣ����,��41�ֽ�Ϊ�ָ���0xFF,��42~241�ֽ�Ϊ��Ϣ
���� */

#define CSM_START_ADDR				(SMS_START_ADDR+0x0001CD14)
/* ������洢����CSM��ʼ��ַ */
#define CSM_INFOS					40
	/* ��������,����Ҫ����20 */
#define CSM_INFO_LEN				43
/* ��������󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3�ֽ�Ϊ�����ﳤ���ֽ�,��4~43�ֽ�Ϊ���������� */

#define SRM_START_ADDR				(SMS_START_ADDR+0x0001D3CC)
	/* ��ɫ�������洢����SRM��ʼ��ַ */
#define SRM_LINGS					40
	/* ��ɫ������,����Ҫ��Ϊ20 */
#define SRM_LING_LEN				244
/* ��ɫ������󳤶�,��1��2�ֽ�Ϊ��־�ֽ�(�Զ���),
��3�ֽ�Ϊ���������ֽ�,��4~23Ϊ�����ֽڣ���24�ֽ�Ϊ
�ָ�����25~244�ֽ�Ϊ�������飬���������ֽ�Ϊ�������ף�
ż���ֽ�Ϊ���� */

#define FBM_START_ADDR				(SMS_START_ADDR+0x0001F9EC)
	/* �����ʵ��洢����FBM��ʼ��ַ,�����ʵ�Ҫ��6KB�ռ�*/


#define FDBM_START_ADDR				(SMS_START_ADDR+0x000211EC)
	/* �����굥�洢����FDBM��ʼ��ַ, Ҫ��8KB�ռ�*/

//									(SMS_START_ADDR+0x000231EC)

//�洢��������

#define		EMM_Part		0x01	//	EMAIL�洢����
#define		SMM_Part		0x02	//	��ͨ�Ķ���Ϣ�洢����
#define		SSM_Part		0x03	//	���ĵĶ���Ϣ�洢���� 


#define		CSM_Part		0x06	//	������洢����
#define		SRM_Part		0x07	//	��ɫ�����洢����
#define		FBM_Part		0x08	//	�����ʵ��洢����
#define		FDBM_Part		0x09	//	�����굥�洢����
 
//DTMFʮ�����Ƶ�ֵ�����

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


//�����ֶ���
#define		INFO_TYPE_SCDATA		0x84		//	����FSK��Ϣ������Ϣ������
//#define		INFO_TYPE_SC_CHECK		0xF0	//	��������CPE�û����Ľ���ȷ����Ϣ������		
//#define		INFO_TYPE_CS_CHECK		0xF1	//				
#define		INFO_CHARA_SCDATA		0x01		//	����FSK��Ϣ������Ϣ������	
#define		ACK_CAS					DTMF_A
#define		ACK_SCDATA_FSK_RIGHT	0x01		//	FSK����ACK:DTMF'D1'
#define		ACK_SCDATA_FSK_ERROR    0x000a//0x0A	D0	//	FSK����ACK:DTMF'D0'


//ʱ�Ӷ���
#define		TS1			18			//	CPE�ȴ�CAS���ʱ��24��
#define		TS5			6			//	CPE�ȴ���һ��FSK�����ʱ��6��
#define		TS6			15			//	CPE�ȴ�����FSK�����ʱ��15��
#define		TS7			15			//	CPE�ȴ��û����볬ʱ900��(15����)
#define		TS8			1			//	CPE�ȴ�SERVER��FSK��Ϣ����ȷ���ʱ��1��

//�ط�����
#define		TIMES_CAS	3			//	SERVER�ط�CAS����
#define		TIMES_FSK	3			//	SERVER�ط�FSK��Ϣ������

////////��Ϣ����

//////����㷢�͸���·�����Ϣ
#define Msg_CID_Coming_Phone_VZ_Chain		11000
#define Msg_EM_VZ_Chain						11001
#define Msg_Receive_FSK_VZ_Chain			11002
#define Msg_Hook_Off_VZ_Chain				11003
#define Msg_Key_EM_VZ						11004
#define Msg_Unsuccess                       11005  
///////��·�㷢�͸���������Ϣ
#define Order_Disconnect_Server_Chain		11006
#define Order_Smsdown_Chain_VZ				11007
#define Order_CAS_ACK_Chain_VZ				11008
#define Order_FSK_ACK_Chain_VZ				11009
#define Order_Send_DTMF_Chain_VZ			11010
#define Order_Send_FSK_Chain_VZ				11011
#define Order_Connect_Server_Chain			11012
#define Order_TimeOver_Chain_VZ				11013
//////��·�㷢�͸���Ϣ�����Ϣ
#define WM_SMS_ML2_MESSAGE					11014
#define Msg_Data_Chain_Msg					11015
#define Msg_Chain_Msg						11016
#define Msg_TimeOver_Chain_Msg				11017
#define Msg_Unsuccess_Chain_Msg             11018
//////��Ϣ�㷢�͸���·�����Ϣ
#define Order_DTMF_Data_Msg_Chain			11019
#define Order_FSK_Data_Msg_Chain			11020
#define Order_Smsdown_Msg_Chain				11021
#define Order_Connect_Server_Msg			11022
#define Order_Disconnect_Server_Msg			11023

///// ������Ϣ�Ķ���
#define WM_UPDATE_MENU						11024
#define WM_DISPLAY_MSG						11025
#define WM_PAINT_SECOND						11026
///////////��·�㷢�͸���Ϣ���֪ͨ��ϢЯ���Ĳ���
#define SMS_CENTER_CALL				0x01
#define SMS_CAS						0x02
#define SMS_PHONE_END				0x03
#define SMS_PHONE_STATUS			0x04
#define SMS_PHONE_BUTTON			0x05
#define SMS_USER_SHUT				0x06

/////////////����㷢�͸���·���֪ͨ��ϢЯ���Ĳ���
#define Msg_CAS_EM					0x07
#define Msg_Ring_End_EM				0x08
#define Msg_State_EM				0x09
#define Msg_TS6_EM					0x10
#define Msg_TS1_EM					0x11
#define Msg_TS5_EM					0x12
/////////////////��Ϣ�㷢�͸���·���֪ͨ��ϢЯ���Ĳ���
///////////DTMF�����붨�� 
#define DTMF_AA			  0x0D0D///	0xDD /////���سɹ�
#define DTMF_A0			  0x0D0A///	0xDA /////�����ڸô洢����
#define DTMF_A1			  0x0D01///	0xD1 /////�洢������
#define DTMF_A2			  0x0D02///	0xD2 /////˽���˺Ų�����
#define DTMF_A3			  0x0D03///	0xD3 /////�û������⣬��ֹ����
#define DTMF_A4			  0x0D04///	0xD4 /////�������û��ܾ�


/*  ��Ϣ��DTMF��ʽ���Ͷ���Ϣ����  */

//BOOL SendSmsDtmf(unsigned * pMsg,int length)



/*  ��Ϣ����ն���Ϣ����,  */
//BYTE * ReceiveSms()

/*  �����DTMF��ʽ�����ֽں���	*/
//BOOL SendByteDtmf(BYTE data)

/*  �����DTMF��ʽ���ͱ��غ���  */
//BOOL SendBitDtmf(BYTE data,int bits)

/*  ���������ֽں���  */
//static BYTE ReceiveByte() 

#define   ArrayLength  512             // ����������Ϣ�������ĳ���

////////////////////��Ϣ���ͺ�������
extern "C" void HwPaintRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 iColor);
extern "C" void HwDrawText(INT16 x, INT16 y, UINT8 iBkColor, UINT8 iForColor, UINT8 iDrawMode, UINT8 iHalign, UINT8 iValign, char *pText);
void SendMessageToSMSL2 ( UINT32,UINT32,UINT32 );//����·�㷢����Ϣ
void SendMessageToSMSMsgLayer ( UINT32 ,UINT32 ,UINT32  );///����Ϣ�㷢����Ϣ
void SendMessageToSMSPhyLayer( UINT32 ,UINT32 ,UINT32  );///������㷢����Ϣ
/*******************************************************************/
/*                ����㶨��                                       */ 
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
/*��������unsigned char Sms_alternate(unsigned data)                          */
/*��  �ߣ���  ��                                                              */
/*ʱ  �䣺2001.10.30                                                          */
/*��  �ܣ��ָ�����ԭ�ͣ��������ݵĸߡ���λ                                    */ 
/*��  ע��                                                                    */
/******************************************************************************/  
unsigned char Sms_alternate(unsigned data);

/******************************************************************************/
/*��������void Sms_Unpacking()                     */
/*��  �ߣ���  ��                                                              */
/*ʱ  �䣺2001.10.31                                                          */
/*��  �ܣ��˺������ڽ���� 1 ������㻺������������                           */
/*                         2 �ָ�����ԭ��                                     */
/*                         3 �����ṹ���󣬲�������Ӧ                         */
/*                         4 ����Ϣ������Ҫ�����ݷ�������Ϣ��Ļ������С�     */
/*��  ע��                                                                    */
/******************************************************************************/ 
bool Sms_Unpacking(void);  

////////////////////////////////////////////////////////////////////////////////
////// ��Ϣ�㶨��
////////////////////////////////////////////////////////////
///������붨��
#ifndef _SMS_ERROR_CODE
#define _SMS_ERROR_CODE

#define SMS_ERROR					0x100	//////����
#define SMS_OK						0x101	//////��ȷ
#define SMS_INFORMATION_END			0x102	//////�������
#define SMS_INFORMATION_UEND		0x103	//////����δ��

#define SMS_QUERY					0x104	//////��Ϣ��ѯ
#define SMS_DOWNLOAD				0x105	//////��Ϣ����
#define SMS_UPLOAD					0x106	//////��Ϣ�ϴ�
#define SMS_CHANGE					0x107	//////�ı�CPE��״̬
#define SMS_IDSETTING				0x108	//////ѯ��ID
#define SMS_CPESETTING				0x109	//////ѯ��CPE������
#define SMS_SCREENOUT				0x110   //////��Ļ���

#define SMS_TYPE_NORMAL				0x111	//////��Ϣ�Ƕ���Ϣ
#define SMS_TYPE_USERMSG			0x112   //////��Ϣ���û��Զ���
#define SMS_TYPE_ERROR				0x113	//////��Ϣ���ʹ���
#define SMS_EMPTY_PHOTO				0x114	//////�绰����Ϊ��
#define SMS_INFORMATION				0x115	//////��Ϣ����Ļѯ��
#define SMS_RING					0x116	//////��ɫ����
#define SMS_GRAPHICS				0x117	//////ͼ�ι���ģ��
#define SMS_MONEY					0x118	//////�����˵�
#define SMS_MONEYEX					0x119	//////�����굥
#define SMS_MODEL					0x120	//////������
#endif


///////////////////////////////////////////////////////////
////����Ϣ���ݽṹ����
#define SMS_APP_TITLE_HEIGHT		17
#define PASSWORDLENGTH				10
#define SMS_MAX_SAVE_NUMBER			64  ////ÿ����������������
#define SMS_SUBSCIBE_MAX_NUMBER		100 ////���Ķ���Ϣ���������
#define SMS_MAX_HAVEPASSWORD_NUMBER	32  ////�ѷ�������������ܴ���Flash�е�����
#define SMS_LETTER_MAX_NUMBER		640 ////ÿ������64������Ϣ,4�����乲256��
										/////�ѷ����乲64��(ÿ������16��)
#define SMS_MONEY_MAX_NUMBER		12  /////�����˵������3k/256byte
#define SMS_MONEYEX_MAX_NUMBER		32	/////�����鵥�����8k/256byte

#define SMS_CONTENT_MAX_NUMBER		141 /////����Ϣ�������ֵ

/////////////���ͷ�ʽ����
#ifndef SMS_DTMF
#define SMS_DTMF
#endif

////////////////////////////////////////////////////////////
/// EMail�����ݽṹ�Ķ���
typedef struct tagMailStruct{

	char		szAddress[20]; /////���ͷ��ĵ�ַ
	char		szMail[220];////////EMail����

}MAILSTRUCT,*LPMAILSTRUCT;

////////////////////////////////////////////////////////////
/// ���Ķ���Ϣ�Ľṹ����
typedef struct tagSubscibeStruct{

	UINT32      iconID;//////ͼ��״̬
	UINT8		ICPCode[8]; /////ICP����
	UINT8		ICPOperationCode[10];////��ͬICP����ͬ����Ϣ�ṩ
	char		szTime[17];//////ʱ��
	char		szTitle[22];/////����
	
	char		szContent[201]; ////���Ķ���Ϣ������

}SUBSCIBESTRUCT,*LPSUBSCIBESTRUCT;

///////������ṹ����
typedef struct tagDownLoadModel{
	
	//UINT32      iconID;//////ͼ��״̬
	//char		szTime[8];//////ʱ��
	//char		szTitle[22];/////����
	char		szContent[41]; ////���Ķ���Ϣ������

}MODELDOWNLOAD,*LPMODELDOWNLOAD;
//////����Ϣ���ʹ洢�ṹ
typedef struct tagSendSMS{
	
	
	char	szPhone[81];//////���5���绰����
	char	szMsg[SMS_CONTENT_MAX_NUMBER];//////����Ϣ�������140��char
						/////����70������
	
}SENDSMS,*LPSENDSMS;

///////���ѽṹ
typedef struct tagMoney {

	UINT32		iconID;//////ͼ��
	
	char		szTitle[21];////�������20���ַ�
	char		szContent[221];/////�����������220���ӷ�

	BOOL		bIsData;//////�Ƿ�������


}MONEY,*LPMONEY;
///////////////////////////////////////////////////////////////
////��������ģ��
int TranslateCommand ( UINT8 *buffer );
////////////////////////////////////////////////////////////////////////////
///// ��Ϣ����ģ��:������Ϣ����
int SMSInformationDownload ( UINT8 *buffer );
//////////////////////////////////////////////////////////
///���ͺ��� 
int SMSInformationUpload ( LPSMS pst );
///////////////////////////////////////////////////////
////��ȡ����Ϣ
SMS GetSMSIntoBuffer ( UINT8 *buffer,int &iFlag );
//////////////////////////////////////////////////////////////////
///// ҵ��ѯ�ʴ���ģ��
void SMSQueryServerID ( UINT32	uRequest );
//////////////////////////////////////////////////////////////////
/////�ı�CPE�Ľ���״̬ģ��
void SMSChangeReceiveStatus ( );
///////////////////////////////////////////////////////////////////
////ϵͳ״̬����ģ��
void SMSQueryCPEConfig ( UINT8 statuscode );


//////// ��Ķ���
class CSaveAsWnd;///���
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


//////////////////�绰��ҵ������
#define NEWPHONE_LISTVIEW		20500
class CNewPhone:public CAppWnd
{
private:
	CListView			m_ListView;///�˵���ʾ
	UINT8				m_iMenuType;///�˵�������
	UINT16				m_iSelectedItem,////��¼�Ѿ�ѡ�е���
						m_iLimite;////�������Ƶ�����
public:
	CNewPhone ( ){}
	~CNewPhone ( ) {}

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

	void UpdateMenu ( );
protected:
	DECLARE_MESSAGE_MAP() ///��Ϣ����
	MESSAGE_HANDLE void OnSelect ( UINT32,UINT32,UINT32 );

};
///////////////////�˵�������
#define MONEY_GRID				20410
#define MONEY_DELETEALL			20411

class CMoneyWnd:public CAppWnd
{
private:
	CGrid				m_pGrid;////////�����˵�����ؼ�
	UINT8				m_iMoneyStyle;//////���ѵ�����,0-��ͨ���� ��1-��ϸ����

	CButton				m_DeleteBtn;//////ȫ��ɾ����ť
public:
	CMoneyWnd ( );
	~CMoneyWnd ( );

	BOOL OnCreate ( );
	void OnPaint ( CDC &dc );
	BOOL OnDestroy ( );

	void SetStyle ( UINT8 style ) { m_iMoneyStyle = style; }

protected:

	DECLARE_MESSAGE_MAP() ///��Ϣ����
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDeleteAll ( UINT32,UINT32,UINT32 );

};
///////////////////��ʾͼ������״̬����
class CDisplayInformation:public CFrameWnd
{
private:
	UINT8		m_DisplayStyle;////Ҫ��ʾ�ķ��,0---��ʾ�ı���1---��ʾͼ��
	UINT8		m_LetterNumber[4];
	char		m_szPhone[81];////��ŵ绰����
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

	DECLARE_MESSAGE_MAP() ///��Ϣ����

};

/////////////////// ��ʾe��Ϣ��ѯ�ʷ�ʽ
#define SHOWQUESTION_OK				20400
#define SHOWQUESTION_CANCEL			20401
#define SHOWQUESTION_EDIT			20402
#define SHOWQUESTION_GRID			20403




class CShowQuestionWnd:public CAppWnd
{
private:
	
	char				m_szMessage[256];/////��ʾ��Ϣ
	UINT8				m_iStyle;////Ҫ��ʾ�ķ�ʽ
	
	CButton				m_OKBtn,////ȷ��
						m_CancelBtn;///��ȷ��

	CEdit				m_Edit;////�����ִ�
	CGrid				m_pGrid;///����ѡ��ؼ�
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
	DECLARE_MESSAGE_MAP() ///��Ϣ����

	MESSAGE_HANDLE void OnOKBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnCloseBtn ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );

};
/////////////////// ��ʾe��Ϣ
#define SHOWMESSAGEEX_EDIT			20300
#define SHOWMESSAGEEX_OK			20301

class CShowMessageEx :public CFrameWnd
{
private:
	BOOL			m_bIsNeedTimer;//��ʱ����־����
	char			szContent[256];
	CStatic			m_Edit;///////��ʾe��Ϣ
	CButton			m_ReturnBtn;//////ȷ����ť
	UINT8			m_iSecondNumber,///��ʾ������
					m_bIsDestroy;///�Ƿ���Ҫ�ݻٴ��� 2 - ��ʾ����ʾ�ı����������ϴ���Ϣ
					
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
	DECLARE_MESSAGE_MAP() ///��Ϣ����
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
	CDisplayInformation		*m_pShowTitle;////��ʾ�����ı���
	int						m_CurrentPos;////��ǰѡ����
	SUBSCIBESTRUCT			m_pst; ////////////����Ϣ��ʱ�洢�ṹ
	CMenuButton				m_ShowMenuBtn;////����˵���ť
	CButton					m_AnswerBtn,/////�ظ���ť
							m_DelBtn,////////ɾ����ť
							m_Name;///////����
	CMenu					m_Menu;//////////�˵�
	CEdit					m_Content;////����Ϣ����

public:

	CReadSubscribeSMS ( );
	~CReadSubscribeSMS ( );
	
	void SetInformation ( int pos,SUBSCIBESTRUCT st )
	{
		m_CurrentPos = pos;
		m_pst = st; /////���ö���Ϣ��Ϣ
	}

	BOOL		OnCreate ( );
	BOOL		OnDestroy ( );
	void		OnPaint ( CDC &dc );

protected:
	DECLARE_MESSAGE_MAP()
	///// ��Ϣ����������
	MESSAGE_HANDLE void OnShowMenu ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDel ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSaveAs ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSendTo ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnShowTitle ( UINT32,UINT32,UINT32 );
};

/////////////////// e��Ϣ
#define SUBSCRIBE_GRID			20200
#define SUBSCRIBE_DELETEALL		20201

class CSubscribeSMS:public CAppWnd  /////���e��Ϣ
{
private:
	CGrid					m_pGrid;//����ؼ�
	CReadSubscribeSMS		*m_pRaedSubscribe;///�Ķ�e��Ϣ����
	CButton					m_DeleteAll;///ȫ��ɾ����ť 
public:
	/////��ʼ������
	CSubscribeSMS ( ):m_pRaedSubscribe(NULL)
	{
		///// TODO: your codes
	}

	~CSubscribeSMS ( ){ }
	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

protected:
	DECLARE_MESSAGE_MAP() ///��Ϣ����
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnDeleteAll ( UINT32,UINT32,UINT32 );
};
//////////////////// �Ҽ�e
#define SERVICE_GRID		20100
#define SERVICE_EDIT		20101
#define SERVICE_INPUTEDIT	20102
#define SERVICE_OK			20103


class CSMSServiceWnd:public CAppWnd
{
private:
	CListView				m_pGrid;//////�˵���
	
	CStatic					m_Edit;////////��ʾ��ʾ��Ϣ
	//CPswdEdit				m_InputEdit;////����������
	CLineButton				m_InputEdit;////����������

	UINT8					m_InputLength,////������볤��
							m_InputNumberFlag,////ʵ����������
							m_BackModel,///////���ͷ�ʽ
							m_EnquireModel ;///ѯ��ģʽ

	CButton					m_OKBtn;/////ȷ��

	CShowQuestionWnd		*m_pQuestionWnd;//ѯ�ʷ�ʽ����
	
public:
	CSMSServiceWnd();
	~CSMSServiceWnd();

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
//	void OnOk ( UINT32,UINT32,UINT32 );

protected:
	DECLARE_MESSAGE_MAP() ///��Ϣ����
	MESSAGE_HANDLE void OnGridCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnUpdateMenu ( UINT32,UINT32,UINT32 );	
	MESSAGE_HANDLE void OnDisplayMessage ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnUserInput ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnOKBtn ( UINT32,UINT32,UINT32 );
};
/////////////////// ��ʾ��Ϣ��
class CShowMessage:public CFrameWnd
{
private:
	BOOL	m_bIsVisible;////����״̬
	char szMessage[30];////�ַ�������
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
	DECLARE_MESSAGE_MAP() ///��Ϣ����
};

///////////////////��������ѡ�񴰿�
#define CHOICEMODEL_LISTVIEW	20104
#define CHOICEMODEL_OK			20105
#define CHOICEMODEL_CLOSE		20106

class CChoiceModel:public CFrameWnd
{

private:
	int				m_iSelected;////��¼ѡ������
	CListView		m_pList;////�����б�
	CIconButton		m_OKBtn,
					m_CloseBtn;
public:
	CChoiceModel ( );
	~CChoiceModel ( );


	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
	
protected:
	DECLARE_MESSAGE_MAP() ///��Ϣ����
	MESSAGE_HANDLE void OnCurSel ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnOK ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnClose ( UINT32,UINT32,UINT32 );
};
/////////////////// ���Ϊ����
#define SAVEAS_MODLE			20110

class CSaveAsWnd:public CAppWnd
{
private:
	int				m_iItem;////ѡ����
	
	CChoiceModel	*m_pChoiceModel;
	CCheckButton	m_mailbox1,////˽��1
					m_mailbox2,////˽��2
					m_mailbox3,////˽��3
					m_mailbox4,////��������4
					m_mailbox5;////�Զ�������


public:
	CSaveAsWnd ():m_pChoiceModel(NULL)
	{}
	~CSaveAsWnd(){}

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );
	void OnOk ( UINT32,UINT32,UINT32 );/////ȷ��
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
	DECLARE_MESSAGE_MAP() ///��Ϣ����
	MESSAGE_HANDLE void OnModle ( UINT32,UINT32,UINT32 );
};

//////////
class CButtonEx : public CCtrl
{
protected:
	BOOL    m_bPressed;            //��¼��ť��ǰ�Ƿ񱻰���״̬
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
	//����:
	//         ��ť��Ϣ������
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	///////////////////////////////////////////////////////////////////////////
	//����:
	//         ��ť�ػ溯��
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
						m_Password,//����
						m_SoundCtrl;///���������Ĳ���

	UINT8				m_iStyle;////��ǰ��ҳ��
	
	BOOL				m_bIsPublic;/////�Ƿ񹫹�����
	int iItem;
	CPswdEdit			m_SoundName;//��������

	CCheckButton		m_RevType,////���շ�ʽ����
						m_Sound,//////�Ƿ�ʹ��������ʾ
						m_EnableSound,////����������Ƿ�Ҫ����
						m_EnableReturn;////�Ƿ���Ҫ��������ִ
						//m_EnablePassword;///�Ƿ���ܷ���

	CIconButton			m_ShowPopupLV;////̸��ListBox
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
	////////��Ϣ����������
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
	CListView		m_pListView; ////�б�ؼ�

	UINT32			iSelectedItem;////ѡ������
	BOOL			bIsSetting; ////�Ƿ�����������Ķ���
	CSetNamePassword  *m_pSetNamePassword;/////�������������
public:
	CSetLetterBox ( );
	~CSetLetterBox ( );
	void OnPaint ( CDC &dc );
	virtual BOOL OnCreate();
	BOOL OnDestroy ( );

protected:
	DECLARE_MESSAGE_MAP()
	////////��Ϣ����������
	MESSAGE_HANDLE void OnListViewCmd ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnSet ( UINT32,UINT32,UINT32 );
};
///////////////////// class CDXXWnd;
#define DXX_LISTVIEW		22000 ///ListView�ؼ�


class CDXXWnd  : public CAppWnd
{
public:
	CDXXWnd();
	virtual ~CDXXWnd();

	void OnPaint ( CDC &dc );
	virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private:
	BOOL				bIsDisplay;///�Ƿ���ʾ������Ϣ
	CLetterBoxWnd		*m_pLetterBoxWnd;////����
	CListView			m_pDXXListView;////ListView�ؼ�
	CDisplayInformation	*m_pMailBoxInfo;///������Ϣ
	
protected:
	DECLARE_MESSAGE_MAP()
	////////��Ϣ����������
	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView��Ϣ����

};
////////////////////////// class Select
#define CHOICEMONEY_LISTVIEW			230001


class CChoiceMoneyWnd :public CAppWnd
{
private:
	CMoneyWnd			*m_pMoney;/////������ʾ
	CListView			m_ListView;/////�б�ؼ�

public:
	
	CChoiceMoneyWnd ( );
	~CChoiceMoneyWnd ( );

	BOOL OnCreate ( );
	BOOL OnDestroy ( );
	void OnPaint ( CDC &dc );

	void ShowMoney ( UINT8 iStyle );
protected:
	DECLARE_MESSAGE_MAP()
	////////��Ϣ����������
	MESSAGE_HANDLE	void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );  /////////ListView�ؼ��ĺ�������

};
////////////////////////// class CHelloWorld;
////////////////�������Ϣ��ID��
#define SM_MAININTERFACE_LISTVIEW	23000   ///�������ListView�ؼ�

class CHelloWorld : public CAppWnd
{
private:
	
	
	CListView			m_MainInterfaceListView;////���˵���ListView�ؼ�
	CDXXWnd				*m_pDXXWnd;////����Ϣ�Ӵ���
	CSETWnd				*m_pSETWnd;////�����Ӵ���  wangyu
	CSubscribeSMS		*m_pSubscribeWnd;///////e��Ϣ����
	CChoiceMoneyWnd		*m_pChoiceMoney;////////����ѡ�񴰿�
	CNewPhone			*m_pNewPhone;//////�绰��ҵ��


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
	////////��Ϣ����������
	MESSAGE_HANDLE	void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );  /////////ListView�ؼ��ĺ�������
	
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

	SMS						m_pst; ////////////����Ϣ��ʱ�洢�ṹ
	CMenuButton				m_ShowMenuBtn;////����˵���ť
	CButton					m_AnswerBtn,/////�ظ���ť
							m_DelBtn,////////ɾ����ť
							m_ShowPhone;///��ʾ�����ĵ绰����
	short					m_iDataID;//////��ǰ������ID
	CMenu					m_Menu;//////////�˵�
	CStatic					m_Content;////����Ϣ����

	char					m_szName[16];/////�ռ��˵�����
	CDisplayInformation		*m_pShowPhone;////��ʾ�����绰���봰��
public:

	CReadingSMS ( );
	~CReadingSMS ( );
	
	void  SetDataID ( short data ) { m_iDataID = data ; }
	void  SetSMSInformation ( SMS st )
	{
		m_pst = st; /////���ö���Ϣ��Ϣ

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
	///// ��Ϣ����������
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
	////////��������
	BOOL			m_bIsNeedReDraw;/////�Ƿ����������ػ�
	int				iUnreadNumber;
	CButton			m_NewBtn,/////����
					m_DeleteAll;///ɾ�����ж���
	CPages			m_RevBtn,////������
					m_SendBtn,////������
					m_SendedBtn;///�Ѿ����͵�����


	CGrid			m_pGridView;////ListView
	

	/////�����ż��Ľṹ
	struct tagLetterStatus{
		int iType;////// 0 ---- δ��
				  /////  1 ---- �Ѷ�
		          /////  2 ---- ����ת��
		          ////   3 ---- ����
		char szTitle[64];
		char szPassword[10];/// ����

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
	CMenuButton			m_MenuBtn;/////����˵�
	CIconButton			m_PopupListBox;////����ListBox
	CButton				m_SendBtn,/////���Ͱ�ť
						m_SearchBtn,///���Ұ�ť
						m_SaveBtn,/////���水ť,����ʾΪ��ʱ��Ϊɾ����ť
						m_GroupBtn;////Ⱥ����ť
	CMenu				m_Menu;////////�˵�
	//CCombox				*m_pCombox;//////��Ͽؼ�
	CPswdEdit			m_Edit,////�༭�ؼ�
						m_Phone;///�绰�༭�ؼ�
	CPopUpList			m_List;///�б�ؼ�

	bool				bIsSendStatus,////��ʾ�Ƿ�Ϊ����״̬
						bIsSaved;////�Ƿ��Ѿ�����
	
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

////////////////////////////  ���� --------  ���ڱ�д
//////////////////////////////////////////////////////////////////////////
////////////
////////////  class CSETWnd 09_����  �ඨ��  start  wangyu       ////////
////////////
/////////////////////////////////////////////////////////////////////////
#define SET_LISTVIEW		20010 ///ListView�ؼ� 
class CSETWnd  : public CAppWnd
{
public:
	CSETWnd();
	virtual ~CSETWnd();
	void OnPaint ( CDC &dc );
    virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private: 
	CSetNamePassword	*m_pSetOther;    ////��������������
	CSetLetterBox		*m_pSetLetterBox;////�趨˽������
	CListView			m_pSETListView;  //  ListView�ؼ�
    CCENTERNoWnd        *m_pCenterNoWnd;  //  ��Ϣ���ĺ����Ӵ���
   // CRECEIVEWnd         *m_pReceiveWnd;   //  �����Ӵ���
protected:    
	DECLARE_MESSAGE_MAP()
	////////��Ϣ���������� 
	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView��Ϣ����
};
/////////// class CSETWnd       09_����  �ඨ��       end    wangyu /////// 

/////////// class CRECEIVEWnd  09_����_���� �ඨ�� start  wangyu /////// 
  /*
#define    Rev_DXXFULL        20013
#define    Rev_DXXARRIVED     20014
#define    Rev_DXXAUTOREPLAY  20015
#define    Rev_DXXSOUND       20016
// �˽ṹ�д�ţ����������и�ѡ����Ͽ�ı�־  
struct SetRevFlag{
  BOOL    DxxFullFlag;
  BOOL    DxxArrivedFlag;
  BOOL    DxxAutoReplyFlag;
  UINT32  DxxSoundFlag;
};

class CRECEIVEWnd  : public CAppWnd
{
private:
	CCheckButton     m_DxxFull,     /////����Ϣ��ʱ���������¶���Ϣ
		             m_DxxArrived,  /////����Ϣ����ʱ��������ʾ
					 m_DxxAutoReply;/////�յ������Զ��ظ������յ���Ķ��� 	
	CCombox          m_DxxSound;    ////��ʾ����ѡ��˵� 	              
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
///////////  class CRECEIVEWnd  09_����_����  �ඨ�� end    wangyu //// 

///////////  class CCENTERNownd  09_����_��Ϣ��   �ඨ�� start  wangyu //// 
#define  CENTERNoWND_REV			 20011
#define  CENTERNoWND_SEND			 20012

class CCENTERNoWnd : public CAppWnd
{
private:

       CLineButton		m_RevEdit;   ///// ���պ���
	   CLineButton		m_SendEdit; //// ���ͺ���
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
//////////  class CCENTERNownd  09_����_��Ϣ��     �ඨ��  end   wangyu ////

/////////////////////////////////////////////////////////////////////////
//////////
//////////  class CMODELWnd  01_������    �ඨ��  start   wangyu       ////
//////////
///////////////////////////////////////////////////////////////////////// 
  
#define MODEL_LISTVIEW         20020   //model
#define MODELCon_LISTVIEW      20050   //model-content
#define MODELMod_LISTVIEW      20051   //model-modify
#define MODEL_ComBox           20021
#define MODEL_NewButton        20022///�½���ť
#define MODEL_Rename           20023///������
#define MODEL_Delete           20024///ɾ��
#define MODEL_Taxis            20025///����
#define MODEL_RenameButton     20026///������ť
#define MODEL_DeleteButton     20027///ɾ����ť
#define MODEL_TaxisUpButton    20028///����ť--����
#define MODEL_TaxisDwButton    20029///����ť--����
//////////////////
     
#define   WYCALL                      1   /// �޸�����(WYCALL)
#define   SYJCALL                     2   /// ��������(SYJCALL)
#define   ENDCALL                     0   /// ���ٴ���(ENDCALL) 
 
class CMODELWnd : public CAppWnd
{
public:
	CMODELWnd();
	virtual ~CMODELWnd();
	void OnPaint ( CDC &dc );
    virtual BOOL OnCreate();
	BOOL OnDestroy ( );

private: 
	CListView			m_pModelListView;  //  ListView�ؼ� 	

	CContentWnd         *m_ContentWnd; // ���ݿؼ� 
	CContent_ModWnd     *m_pDeleteWnd, // ɾ��
	                    *m_pRenameWnd, // ����
	                    *m_pTaxisWnd ; // ����
	
	CMenu                m_Content;      
	CMenuButton          m_ConMB;        
	CButton              m_pConNewButton;
 
protected:    
	DECLARE_MESSAGE_MAP()
	////////��Ϣ���������� 
	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView��Ϣ����
    MESSAGE_HANDLE  BOOL OnGRPSDNewBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnShowMenuBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnRename ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	MESSAGE_HANDLE  void OnTaxis  ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};
//////////  class CMODELWnd   01_������         �ඨ��  end   wangyu //// 

////////////class CContentWnd  01_������_����    �ඨ��  start   wangyu /////
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
////////////class CContentWnd      01_������_����    �ඨ��  end   wangyu /////

////////////class CContentListWnd      01_������_����_�鿴    �ඨ��  start   wangyu /////
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
	CPswdEdit			m_Edit;////�༭�ؼ�
	CButton             m_pListDelete,
		                m_pListModify,
						m_pListSend;
protected:
	DECLARE_MESSAGE_MAP() 
		    MESSAGE_HANDLE  UINT32 OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
            MESSAGE_HANDLE  UINT32 OnModify ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
	        MESSAGE_HANDLE  UINT32 OnSend   ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );

};

////////////class CContenListtWnd      01_������_����_�鿴     �ඨ��  end   wangyu /////

////////////class CContent_ModWnd  01_������_�޸�    �ඨ��  end   wangyu /////
class CContent_ModWnd : public CAppWnd
{
private: 	              
	CListView			m_pConModifyListView;  //  ListView�ؼ� 
 
	CButton              m_pModifyButton,
		                 m_pUpButton,
					 	 m_pDwButton;

    CPswdEdit            m_Edit;  ////�༭�ؼ�������:����
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

 
////////////class CContent_ModWnd  01_������_�޸�    �ඨ��  end   wangyu /////
 
///////////////////////////////////////////////////////////////////////
//////////
//////////  class CGRPSDWnd  02_Ⱥ��    �ඨ��  start   wangyu     ////
//////////
/////////////////////////////////////////////////////////////////////// 
#define GRPSD_LISTVIEW         20030 
#define GRPSDLeader_LISTVIEW   20052
#define GRPSDModify_LISTVIEW   20053 
#define GRPSD_ComBox           20031
#define GRPSD_NewButton        20032///�½���ť
#define GRPSD_Rename           20033///������
#define GRPSD_Delete           20034///ɾ��
#define GRPSD_Taxis            20035///����
#define GRPSD_RenameButton     20036///������ť
#define GRPSD_DeleteButton     20037///ɾ����ť
#define GRPSD_TaxisUpButton    20038///����ť--����
#define GRPSD_TaxisDwButton    20039///����ť--����
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
	CListView			m_pGrpSdListView;  //  ListView�ؼ� 
	CLeaderWnd          *m_pLeaderWnd; //һ���쵼�ؼ� 
	CGroup_ModWnd       *m_pDeleteWnd, //ɾ��
	                    *m_pRenameWnd, //����
	                    *m_pTaxisWnd ; //����
                          
	CMenu                m_GrpSd;     // 
	CMenuButton          m_GrpSdMB;        
	CButton              m_pGSNewButton;
	CScrollBar         m_pGrpSdScrllBar;  //
protected:    
	DECLARE_MESSAGE_MAP()
	////////��Ϣ���������� 
      	MESSAGE_HANDLE  void OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ); ///ListView��Ϣ����
        MESSAGE_HANDLE  BOOL OnGRPSDNewBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnShowMenuBtn ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnRename ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
    	MESSAGE_HANDLE  void OnTaxis  ( UINT32 nMessage, UINT32 wParam , UINT32 lParam );
};
//////////  class CGRPSDWnd   02_Ⱥ��         �ඨ��  end   wangyu //// 

////////////class CLeaderWnd  02_Ⱥ��_�쵼    �ඨ��  start   wangyu /////
#define   MODIFYLEADER_EDIT           20060 
#define   MODIFYLEADERNAME_EDIT       20061
#define   MODIFYLEADER_NameCard       20062
#define	  MODIFYLEADER_SEND			  20063

class CLeaderWnd: public CAppWnd
{
private: 
	 CGrid              m_pGridView;       
	 CButton			m_SendBtn;////���Ͱ�ť
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
////////////class CLeaderWnd  02_Ⱥ��_�쵼    �ඨ��  end   wangyu /////

////////////class CGroup_ModWnd  02_Ⱥ��_�޸�    �ඨ��  end   wangyu /////

class CGroup_ModWnd : public CAppWnd
{
private: 	              
	CListView			 m_pGrpModifyListView;  //  ListView�ؼ� 
 	CButton              m_pGSModifyButton,
		                 m_pGSUpButton,
					 	 m_pGSDwButton;
    CPswdEdit            m_Edit;  ////�༭�ؼ�������:����
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
////////////class CGroup_ModWnd  02_Ⱥ��_�޸�    �ඨ��  end   wangyu /////

/////////////////////// ��·��
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

//////////////////////////////��Ϣ�� SYJ
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
	DECLARE_MESSAGE_MAP() ///////////////��Ϣӳ��
	MESSAGE_HANDLE void OnDataChainMsg ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnChainMsg ( UINT32 ,UINT32 ,UINT32  );
	MESSAGE_HANDLE void OnTimerOver ( UINT32,UINT32,UINT32 );
	MESSAGE_HANDLE void OnUnsuccessChainMsg ( UINT32,UINT32,UINT32 );
};


////wangyu1228
/////////////class CDrawPicWnd  ͼ�ι���strat  wangyu   ////////////
class CDrawPicWnd : public CAppWnd
{
private:
    INT16  iX0, iY0;  // ����ԭ�㣬����ڴ��ڵ����Ͻ�
	INT16  m_iCurX, m_iCurY;    //��ǰλ��
	UINT8  m_iPenColor;         //����ɫ
    UINT8  m_iBackColor;        //����ɫ
    UINT8  m_iDrawMode;         //��ͼ����
    UINT8  m_iPenWidth;         //�ʿ��     
    UINT8  m_iLineStyle;        //�������� 
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
	 void DrawEllipse(INT16 x0, INT16 y0, INT16 x1, INT16 y1);//��Բ
	  

protected:
  DECLARE_MESSAGE_MAP()

};
/////////////class CDrawPicWnd ͼ�ι���  end     wangyu   ////////

//////////////////////////////////////// SMS END
