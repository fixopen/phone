////////////////////////////////////////////////////////////////
/////          ����Ϣͷ�ļ�
////////////////////////////////////////////////////////////////
#include "sms.h"
#include "keydrv.h"

/////////////////wangyu  start
//struct SetRevFlag     m_CheckFlag;      // wangyu �������ø�ѡ��
static int            GroupCall = WYCALL; // �������ֵ�����:�޸�����(WYCALL)
static int            ModelCall = WYCALL; //                ��������(SYJCALL)
                                          //                ���ٴ���(ENDCALL)                     
////////////Ⱥ�� wangyu
GROUP               g_pGroup[MAX_GROUP_NUMBER];     /// ���Ⱥ�����������                   
GROUP               g_pGroupSend;                   /// �����Ӣ����Ҫ������                    
static int          GroupNumber  ;                  /// �Ѵ��ڵ� Ⱥ��ĸ���

BOOL                ModifyFlag;                     /// �ػ���־
UINT32              GrpSDModifyFlag ;               /// ��ʶ ѡ�е� �����˵� �е� ѡ��
UINT32              LEADERFLAG =-1;                 /// ��ʶ ѡ�е���
////////////������  wangyu
// new  2001.12.12
short               g_Model_Locked[MAX_MODEL_NUMBER];     ///��� �������� ��Ӧ��ID�� 
short               g_Content_locked[MAX_CONTENT_NUMBER]; ///��� �������� ��Ӧ��ID�� 
UINT32              g_nLocal = 0;                         ///��� �������� ��Ӧ��λ�ü�����
UINT32              g_nNumber = 0; 
BOOL                m_FirstPaint ;    /// ��ʶ��һ��(true)����OnPaint()����(false)�����������ڵ��ػ�
UINT32              ContentNO=NULL;   /// ��ʶ ���������еĵ� ContentNO ������    

BOOL                m_First;
char                g_ModelSend[SMS_CONTENT_MAX_NUMBER];         /// �����Ӣ����Ҫ������ 
short               ModelFlagID;              /// ��ʶ ѡ�еĳ�������
UINT32              ModelModifyFlag = NULL;   /// ��ʶ ѡ�е� �����˵� �е� ѡ�� 

///////////ʱ��
NU_TIMER			TS,  //��ʱ��
					SYJTS;////
/////////////////////   wangyu  end ////////////////////////////////////


//////////////////SongYingJian start
MONEY					g_Money[SMS_MONEY_MAX_NUMBER],////////�����˵�
						g_MoneyEx[SMS_MONEYEX_MAX_NUMBER];////////�����굥

SMS						g_ST,			// ����Ϣ�洢�ṹ
						g_SendedSMS[4][SMS_MAX_SAVE_NUMBER];///�ѷ�����ṹ����

SUBSCIBESTRUCT			g_SubscibeSMS[SMS_SUBSCIBE_MAX_NUMBER];//���Ķ���Ϣ�ṹ
int						g_iSubscribePos = 0;/////��ʾ���Ķ��ŵĵ�ǰָ��

SENDSMS					g_SendST;	/////������Ϣ�洢 
int 					iCurrentLetterBox = 3, ////��ǰѡ�е�����
										// 0 - 3 ˽��1-3
						iCurrentChildLetterBox = 0;///������������
										// 0 - 2 �����䡢�����䡢�ѷ�����

BOOL					bIsSuccessSend = FALSE;////��ʾTS6�Ƿ���Ҫ��ʾ	
short					g_pSMSDataIDBuf[SMS_LETTER_MAX_NUMBER];
short					g_pSMSDataTempIDBuf[SMS_MAX_SAVE_NUMBER];////�洢ÿ������Ϣ����
short					g_CurrentID = 0;/////��¼��ǰ���ݵ�ID��
UINT16					g_iCurrentPos = 0;////��¼��ǰ�����±�
UINT8					g_iServerType = -1;//////�������� 0-������Ϣ������1-������Ϣ������2-Email��Ϣ����;
BOOL                    g_icommuicationstate = FALSE;  //����Ϣ����״̬��־ FALSE-��δ�ɹ� TRUE-�ɹ� 
UINT8					g_pMenuBuffer[256];//////��Ļѯ�ʲ˵�������
CSaveAsWnd				*m_pSaveAsWnd = NULL;/////���Ϊ����
CShowMessageEx			*m_pShowMsg = NULL;////��ʾ��Ϣ
BOOL					g_bIsModel = FALSE;////�������Ƿ��Ѿ���
BOOL					g_bIsGroup = FALSE;////Ⱥ���Ƿ��Ѿ���
char					g_szHaveNewMessage[80];

///// SongYingJian End

///////�������� START
extern SYSTEM_OPTION	g_SystemOption;       ////ϵͳ����
UINT32					g_pL12MsgBuffer[3],     // ���������·�㽻�������Ļ�����
						g_pL23MsgBuffer[3];     // ��Ϣ������·�㽻�������Ļ�����
UINT8					L3Buffer[ArrayLength];         // ���ݻ�����

CHelloWorld				*g_pHelloWorld = NULL;  // Ӧ�ò�ȫ��ָ��
CSMSMessage				*m_pSMSMessage = NULL;  // ��Ϣ��ȫ��ָ��
CSMSL2Wnd				*m_pSMSL2 = NULL;			// ��·��ȫ��ָ��
CSmsPhy					*g_pSmsPhy=NULL;            // �����ȫ��ָ��
CReadingSMS				*m_pReadingLetter = NULL;///�Ķ�����
CShowMessage			*g_pShowMsg = NULL;///��Ϣ����
CSMSServiceWnd			*m_pService = NULL; ////�Ҽ�e���ô���
CMODELWnd				*m_pMODELWnd = NULL;////������
CGRPSDWnd				*m_pGRPSDWnd = NULL;////Ⱥ��
CModifyLetterWnd		*m_pModifyLetter = NULL;/////�������޸Ĵ���
CDrawPicWnd				*g_pGraphicsWnd = NULL;//////ͼ�ι���ģ��
extern BOOL             g_pPhoneSmsState;   //true is sms state
BOOL					g_bIsRing = FALSE; ////������Ƿ񲥷�����
BOOL					g_bIsCanReset = FALSE;//////�Ƿ������л�
///////�������� END

////////����Ƭ�Ľӿ�
extern short SelectTelNum(char * , short , CARDFIELD & );

///////////////////////////////// ////////////////////////////////////////
////    �����     �� ��:�� �� ��        xgw
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSmsPhy, CFrameWnd)
  ON_MESSAGE(NULL, Order_Smsdown_Chain_VZ,OnOrderSmsdownChainVZ)   
  ON_MESSAGE(NULL, Order_CAS_ACK_Chain_VZ,OnOrderCASACKChainVZ )
  ON_MESSAGE(NULL, Order_FSK_ACK_Chain_VZ, OnOrderFSKACKChainVZ)
  ON_MESSAGE(NULL, Order_Send_DTMF_Chain_VZ, OnOrderSendDTMFChainVZ)
  ON_MESSAGE(NULL, Order_Send_FSK_Chain_VZ, OnOrderSendFSKChainVZ)
  ON_MESSAGE(NULL, Order_Connect_Server_Chain, OnOrderConnectServerChain)
  ON_MESSAGE(NULL, Order_Disconnect_Server_Chain,OnOrderDisconnectServerChain)
  ON_MESSAGE(NULL, Order_TimeOver_Chain_VZ, OnOrderTimeOverChainVZ)
END_MESSAGE_MAP

/////������Ϣ
void SendMessageToSMSPhyLayer( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	SYSTEM_MESSAGE message;
	message.handle = (UINT32)g_pSmsPhy;
	message.message = uMessage;
	message.wparam = wParam;
    message.lparam = lParam;

	PostMessageToQueue ( &message );

}

///////////////////////////////////////////////////////////////////////////////
void InitSmsPhy()
{
	if ( g_pSmsPhy == NULL )
	{
		g_pSmsPhy = new CSmsPhy;
		g_pSmsPhy->Create("SMSPhy", 
			WS_BORDER, 
			CRect(0 ,  0, 1, 1) , 0);
	}
}

BOOL CSmsPhy::OnCreate()
{
	if ( CFrameWnd::OnCreate ( ) == FALSE ) 
		return FALSE;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	��һ����Ϣ������
//	��ϢΪ:Order_Smsdown_Chain_VZ
//	��ز����� (g_pL12MsgBuffer[0])(g_pL12MsgBuffer[1])
//	DTMF_AA		0x0D 0x0D /////���سɹ�
//	DTMF_A0		0x0D 0x0A /////�����ڸô洢����
//	DTMF_A1		0x0D 0x01 /////�洢������
//	DTMF_A2		0x0D 0x02 /////˽���˺Ų�����
//	DTMF_A3		0x0D 0x03 /////�û������⣬��ֹ����
//	DTMF_A4		0x0D 0x04 /////�������û��ܾ�
void CSmsPhy::OnOrderSmsdownChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	UINT8  temp[2];
	temp[0]=g_pL12MsgBuffer[0];
	temp[1]=g_pL12MsgBuffer[1];
	OrderSendDTMFData(temp,2,-1);
}
//	�ڶ�����Ϣ������
//	��ϢΪ:Order_CAS_ACK_Chain_VZ
//	��ز����� (g_pL12MsgBuffer[0])
void CSmsPhy::OnOrderCASACKChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	/*UINT8  temp[2];/////syj 2002.2.26
	temp[0]=g_pL12MsgBuffer[0];   
	//temp[1]=g_pL12MsgBuffer[1];
	OrderSendDTMFChainVZ(temp,1); */
}
//	��������Ϣ������
//	��ϢΪ:Order_FSK_ACK_Chain_VZ
//	��ز����� (g_pL12MsgBuffer[0])(g_pL12MsgBuffer[1])
//	DTMF_D0		0x00 0x0A /////FSK���ݴ�����ȷ
//	DTMF_D1		0x00 0x01 /////FSK���ݴ��ʹ���

void CSmsPhy::OnOrderFSKACKChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	UINT8  temp[2];
	temp[0] =  g_pL12MsgBuffer[0];  
	temp[1] =  g_pL12MsgBuffer[1];
	temp[0] &= 0x0f;
	temp[1] &= 0x0f;
	SendDTMFFskAck(temp);
}
//	���ĸ���Ϣ������
//	��ϢΪ:Order_Send_DTMF_Chain_VZ
//	��ز����� L3Buffer[512],���ݶ����һλ��0x7F

void CSmsPhy::OnOrderSendDTMFChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	int i;
	
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow();
	g_bIsCanReset = TRUE;
	g_pShowMsg->SetMessage ( "���ڷ������� ���Ժ�..." );
	UINT8  temp[ArrayLength];
	for(i=0;( L3Buffer[i]!=0x7F )&&( i<ArrayLength );i++){
		temp[i]=L3Buffer[i];
	}

	OrderSendDTMFData(temp,i,g_iServerType);
}
//	�������Ϣ������
void CSmsPhy::OnOrderSendFSKChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	//��ƽ̨��FSK��������;
}
//	��������Ϣ������
//  ��ƽ̨��������
//	g_SystemOption.g_pCenterReceivePhone[32]Ϊ���Ľ������
//	g_SystemOption.g_pCenterSendPhone[32]Ϊ���ķ��ͺ���

void CSmsPhy::OnOrderConnectServerChain(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow();
	g_bIsCanReset = TRUE;
	g_pShowMsg->SetMessage ( "�����ӷ����� ���Ժ�..." );
	SmsDialOut(g_SystemOption.g_pCenterReceivePhone);
}
//	���߸���Ϣ������:��ƽ̨�Ͽ�����
void CSmsPhy::OnOrderDisconnectServerChain(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{

	DisconnectServer(); ///�һ�
	if ( g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow( SW_HIDE );
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	
	
	//////֪ͨ��Ϣ�㲥������
	if ( g_bIsRing )
	{

		g_bIsRing = FALSE;
		
		///////������ɫ����
		if ( g_SystemOption.SMSStatus[g_ST.bMailBox].iRingID != 0xff )
			::sndPlayMusic(g_SystemOption.SMSStatus[g_ST.bMailBox].iRingID );


		if ( !g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ();
		g_pShowMsg->SetMessage ( g_szHaveNewMessage );
		NU_Sleep ( 800 );
		if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );

	}

}

//	�ڰ˸���Ϣ������:��ʱ���,��Ҫ�һ�
void CSmsPhy::OnOrderTimeOverChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{

	g_bIsRing = FALSE;
	if ( g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow( SW_HIDE );
	//g_pShowMsg->ShowWindow ( SW_HIDE );
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	if (g_pPhoneSmsState)
	{
		if ( !g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ();
		g_pShowMsg->SetMessage ( "���緱æ,���Ժ�����!" );
		DisconnectServer();
		NU_Sleep ( 100 );
		if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );

		///////////����Ҽ�e���ڴ��ڽ�������
		///////////����Ҽ�e���ڴ��ڽ�������
		if ( m_pShowMsg ) ///// syj modify 2002.1.5
		{
						
			m_pShowMsg->Destroy ( );
			delete m_pShowMsg;
			m_pShowMsg = NULL;
		
		}


		if ( m_pService ) 
		{

			m_pService->Destroy ( );
			delete m_pService;
			m_pService = NULL;
		
		}


		
	}
}
////////////////////////////////����� END/////////////////////////////////// 


///////////////////////////////// ��·��
/******************************************************************************/
/*��������unsigned char Sms_alternate(unsigned data)                          */
/*��  �ߣ���  ��                                                              */
/*ʱ  �䣺2001.10.30                                                          */
/*��  �ܣ��ָ�����ԭ�ͣ��������ݵĸߡ���λ                                    */ 
/*��  ע��                                                                    */
/******************************************************************************/  
unsigned char Sms_alternate(unsigned data)
{ 
   int i;	 
   unsigned char temp;
  
   for(i=0;i<8;i++)
   { 
	 temp<<=1;
     temp|=(data  & 0x01);	  
     data >>=1;     
   }
   return temp;
} 
/******************************************************************************/
/*��������void Sms_Unpacking( )                                      */
/*��  �ߣ���  ��                                                              */
/*ʱ  �䣺2001.10.31                                                          */
/*��  �ܣ��˺������ڽ���� 1 ������㻺������������                           */ 
/*                         2 �����ṹ���󣬲�������Ӧ                         */
/*                         3 ����Ϣ������Ҫ�����ݷ�������Ϣ��Ļ������С�     */
/*��  ע��                                                                    */
/******************************************************************************/ 
bool Sms_Unpacking(void)  
{
	int i; 
	UINT8  length=0,verify=0;      
    UINT8  temp[ArrayLength];	 
	//memset(temp, 0, ArrayLength);

    // 1 �ӻ������������� 
    for(i=0; i<ArrayLength; i++) 
	{      
        temp[i]= L3Buffer[i];	             
	}
    //memset(L3Buffer, 0, ArrayLength);  // ���������
     
     //   2  �����ṹ����
   length=(temp[1]-0x01);   // ��Ϣ������   
   for(i=0; i<=length+3; i++) // ����У��λֵ
	  {
		verify+=temp[i];
	  }
   
   
   if((temp[0]==0x84 )/*&& (temp[2]==0x01 ) */&& (verify == 0x00)){
      
	   for(i=0; i<length; i++)
	     L3Buffer[i]=temp[i+3];  // 3 ����Ϣ���������ݷ��뻺����         
	  
	   g_pL12MsgBuffer[0] = 0x00; // ���ݰ���ȷ 'DTMF d1' 
	   g_pL12MsgBuffer[1] = 0x01;
	   SendMessageToSMSPhyLayer(Order_FSK_ACK_Chain_VZ, 0, 0); 
	  
   }else {

      g_pL12MsgBuffer[0] = 0x00; // ���ݰ����� 'DTMF d0'
      g_pL12MsgBuffer[1] = 0x0a;
      SendMessageToSMSPhyLayer(Order_FSK_ACK_Chain_VZ, 0, 0);     
	  
	  return false;
   }


   return true;

}
///////////////////////////////////////////////////
///��ʱ������
void TimerOver( unsigned long lParam)
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	SendMessageToSMSPhyLayer(Order_TimeOver_Chain_VZ, 0, 0); // 1 ֪ͨ��Ϣ�㣺ʱ�䳬ʱ
	SendMessageToSMSMsgLayer(Msg_TimeOver_Chain_Msg, 0, 0);  // 2 ֪ͨ��Ϣ�㣺ʱ�䳬ʱ
}

void SYJTimerOver( unsigned long lParam)
{
	
	SYSTEM_MESSAGE message;
	message.handle = (UINT32)m_pShowMsg;
	message.message = WM_PAINT_SECOND;
	message.wparam = 0;
    message.lparam = 0;
    
	PostMessageToQueue ( &message );

	
	if ( m_pShowMsg->GetSecond ( ) == 0 ) ////��ʱ����ʱ
	{
		NU_Control_Timer(&SYJTS, NU_DISABLE_TIMER);

#ifdef SMS_DTMF	
		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x00;
		L3Buffer[2] = 0x7f;
#endif

#ifdef SMS_FSK

		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x00;
		L3Buffer[2] = 0x7f;

#endif


		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );


	}
}

////////////////////// class CSMSL2Wnd
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CSMSL2Wnd, CFrameWnd) 
    // 1 ������Ϣ�����Ϣ
	ON_MESSAGE(NULL, WM_SMS_ML2_MESSAGE , OnUpload)              // ��Ϣ��--����·�㣺��FSK���������ڲ��� 
    ON_MESSAGE(NULL, Order_DTMF_Data_Msg_Chain, OnDTMF_Data)     // ��Ϣ��--����·�㣺�ϴ�DTMF��
    ON_MESSAGE(NULL, Order_FSK_Data_Msg_Chain, OnFSK_Data)       // ��Ϣ��--����·�㣺�ϴ�FSK��
    ON_MESSAGE(NULL, Order_Smsdown_Msg_Chain, OnSmsdown_Msg)     // ��Ϣ��--����·�㣺������FSK������Ӧ
	ON_MESSAGE(NULL, Order_Connect_Server_Msg, OnConnect_Server_Chain) // ��Ϣ��--����·�㣺���������������
    ON_MESSAGE(NULL, Order_Disconnect_Server_Msg, OnDisconnect_Server_Chain) // ��Ϣ��--����·�㣺���������������
    // 2 ������������Ϣ
	ON_MESSAGE(NULL, Msg_CID_Coming_Phone_VZ_Chain, OnSMS_Come)  // �����--����·�㣺
	ON_MESSAGE(NULL, Msg_EM_VZ_Chain, OnDeal_L1Msg)              // �����--����·�㣺
	ON_MESSAGE(NULL, Msg_Receive_FSK_VZ_Chain, OnReceive_FSK)    // �����--����·�㣺
	ON_MESSAGE(NULL, Msg_Hook_Off_VZ_Chain, OnHook_Off)          // �����--����·�㣺
	ON_MESSAGE(NULL, Msg_Key_EM_VZ, OnKey_EM)                    // �����--����·�㣺
    ON_MESSAGE(NULL, Msg_Unsuccess, OnUnsuccess)                    // �����--����·�㣺
	
	// 3 
	
END_MESSAGE_MAP

CSMSL2Wnd::CSMSL2Wnd()
{
  
}

CSMSL2Wnd::~CSMSL2Wnd()
{

}

//////////////////   ��������Ϣ�����Ϣ  start  wy  ///////////////////////////////
// 1  ��������ƽ̨��Ϣ
void CSMSL2Wnd::OnConnect_Server_Chain( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
  	SendMessageToSMSPhyLayer(Order_Connect_Server_Chain, 0, 0);// ֪ͨ�������������ƽ̨
}

// 2 ����Ͽ�����
void CSMSL2Wnd::OnDisconnect_Server_Chain( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
    SendMessageToSMSPhyLayer(Order_Disconnect_Server_Chain,0,0);
}

// 3  �����ϴ�DTMF����Ϣ
void CSMSL2Wnd::OnDTMF_Data ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

#ifdef SMS_DTMF
     unsigned char temp[ArrayLength];
	 int i;
	 //memset(temp, 0, ArrayLength);
	 temp[0]=0x0E; //DTMF_B
	 for(i=0; i<ArrayLength; i++){
	   temp[i+1]= L3Buffer[i];
	   if(L3Buffer[i]==0x7f){
		   break;
	   }
	 }
 
	 //memset(L3Buffer, 0, ArrayLength);  // ���������
	 for(i=0; i<ArrayLength; i++){
		L3Buffer[i]=temp[i];	 
	 }

#endif
	 
	 SendMessageToSMSPhyLayer(Order_Send_DTMF_Chain_VZ,0,0); // 1 �ϴ���DTMF���Է��ڻ�������
}

// 4 �����ϴ�FSK����Ϣ(��ʱ������ʵ��)
void CSMSL2Wnd::OnFSK_Data ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{	 
	//::MessageBox ( "�ϴ�FSK��","Msg",MB_OK );
}

// 5  ������FSK������Ӧ
void CSMSL2Wnd::OnSmsdown_Msg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{	 
	UINT8  temp[2];
	temp[0]=g_pL23MsgBuffer[0];
	temp[1]=g_pL23MsgBuffer[1];
	OrderSendDTMFData(temp,2,-1);
	//SendMessageToSMSPhyLayer(Order_Smsdown_Chain_VZ, wParam, 0);//֪ͨ�����AA,A0,ect;
}

//  6 ����Ϣ���������ԣ�����ؽ���FSK�� 
void CSMSL2Wnd::OnUpload ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
		
	SendMessageToSMSMsgLayer (Msg_Data_Chain_Msg,0,0 ); 
     
}
///////////////////   ��������Ϣ�����Ϣ    end    /////////////////////////////////

 
//////////////////    ��������������Ϣ    start wy  /////////////////////////////////
// 1 ����Ϣ��������
void CSMSL2Wnd::OnSMS_Come ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{	
	SmsPickUpPhone();
	
	g_iServerType = 1;
	g_pL23MsgBuffer[0] = SMS_CENTER_CALL;
	g_pL23MsgBuffer[1] = 0;
    SendMessageToSMSMsgLayer(Msg_Chain_Msg,0,0); // ֪ͨ��Ϣ��
				// ���ö�ʱ�������� TS1 ��׼������CAS
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	NU_Reset_Timer(&TS, TimerOver, TS1*250, TS1*250, NU_ENABLE_TIMER);
}

// 2 
void CSMSL2Wnd::OnDeal_L1Msg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{ 
     switch( g_pL12MsgBuffer[0] )
	 {
	 case Msg_CAS_EM: // �յ�CAS�ź�
		        NU_Control_Timer(&TS, NU_DISABLE_TIMER); // ����������ֹͣTS1        
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
				g_pShowMsg->SetMessage ( "���ӳɹ�!" );
				g_pL12MsgBuffer[0] = DTMF_A;
				g_pL12MsgBuffer[1] = 0;
   				SendMessageToSMSPhyLayer(Order_CAS_ACK_Chain_VZ,0 , 0);				
				g_pL23MsgBuffer[0] = SMS_CAS;
				g_pL23MsgBuffer[1] = 0;
				SendMessageToSMSMsgLayer(Msg_Chain_Msg, 0, 0);    
					  // �������ö�ʱ�������� TS5 ��׼������FSK��            
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				NU_Reset_Timer(&TS, TimerOver, TS5*300, TS5*300, NU_ENABLE_TIMER);
		  break;
	case Msg_TS6_EM:
	   				 // �������ö�ʱ�������� TS6 ��׼�����ܺ���FSK��            
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				if( g_iServerType == 0)
					NU_Reset_Timer(&TS, TimerOver, TS6*250*3, TS6*250*3, NU_ENABLE_TIMER);
				if(( g_iServerType == 1)||( g_iServerType == 2))
					NU_Reset_Timer(&TS, TimerOver, TS6*300, TS6*300, NU_ENABLE_TIMER);
		  break;
	case Msg_TS1_EM:
				     // ���ö�ʱ�������� TS1 ��׼������CAS
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				NU_Reset_Timer(&TS, TimerOver, TS1*450, TS1*450, NU_ENABLE_TIMER);
		  break;
	default: 	      
		  break; 
	 }
}

//  3  ���ա�����FSK���ݰ�
void CSMSL2Wnd::OnReceive_FSK ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);  // FSK����������ֹͣTS5/6
	
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ();
	g_pShowMsg->SetMessage ("���ڽ������� ���Ժ�..." );
	if ( Sms_Unpacking() )   // ��·�����FSK��
		SendMessageToSMSMsgLayer(Msg_Data_Chain_Msg, 0, 0);

	g_bIsCanReset = FALSE;
}

//  4  �ж϶���Ϣ
void CSMSL2Wnd::OnHook_Off ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{ 
	g_pL23MsgBuffer[0] = SMS_USER_SHUT;
	g_pL23MsgBuffer[1] = 0;
	SendMessageToSMSMsgLayer(Msg_Chain_Msg,0 , 0);
}

  // 5 �绰����
void CSMSL2Wnd::OnKey_EM ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{ 
	g_pL23MsgBuffer[0] = SMS_PHONE_BUTTON;
	g_pL23MsgBuffer[1] = g_pL12MsgBuffer[0];
	SendMessageToSMSMsgLayer(Msg_Chain_Msg, 0, 0);
}
 // 6 EM-VZ֮�����ݴ������   
void CSMSL2Wnd::OnUnsuccess( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER); 
    SendMessageToSMSMsgLayer(Msg_Unsuccess_Chain_Msg,0,0);
} 
//////////////////    ��������������Ϣ   end  /////////////////////////////////



BOOL CSMSL2Wnd::OnCreate ( )
{
	if ( CFrameWnd::OnCreate ( ) == FALSE )
		return FALSE;
	/////////////��ʼ����·���һЩ����

	return TRUE;
}

void RunSMSL2 ( )
{
	/////��ʼ������
	if(!m_pSMSL2)
	{
		m_pSMSL2 = new CSMSL2Wnd;
		m_pSMSL2->Create("SMSL2", 
			WS_BORDER, 
			CRect(0 ,  0, 1, 1) , 0);
	}

	/////////////��ʼ��Ⱥ��
    LoadGroupSend(&g_pGroup[0]);
	if( g_pGroup[0].szName[0] == 0xff || g_pGroup[0].szGroupNumber==-1){ 
	    
	    for(short i=0; i<MAX_GROUP_NUMBER; i++) {
			memset(&g_pGroup[i] ,0 , sizeof(g_pGroup[i]));
			GroupNumber= g_pGroup[i].szGroupNumber = 0;		     
            strcpy(g_pGroup[i].szName,"");
	        strcpy(g_pGroup[i].szReserved,"");
	        for(int j=0; j<5; j++){
	            strcpy(g_pGroup[i].Numbers[j].szName, "");
	            strcpy(g_pGroup[i].Numbers[j].szPhone,"");
			}
		} 
	} 		
	GroupNumber=g_pGroup[0].szGroupNumber;

	NU_Create_Timer(&TS, "waitTS", TimerOver, 9, 1000, 1000, NU_DISABLE_TIMER);//init tIMER
	
	//NU_Control_Timer(&TS, NU_DISABLE_TIMER);

}


void SendMessageToSMSL2 ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	SYSTEM_MESSAGE message;
	message.handle = (UINT32)m_pSMSL2;
	message.message = uMessage;
	message.wparam = wParam;
    message.lparam = lParam;
    
	PostMessageToQueue ( &message );
	
	
}
///////////////////////WANG YU END//////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//////////////////////// ��Ϣ��
void FillTime ( char *datebuffer )
{
	DateTime	date;
	GetSysTime ( &date );
	
	char szTemp1[17],szTemp2[5];
	strcpy ( szTemp1,"" );
	//// Get year
	sprintf ( szTemp2,"%d",date.year );
	strcat ( szTemp1,szTemp2 );
	strcat ( szTemp1,"-" );
	/// Get month
	sprintf ( szTemp2,"%d",date.month );
	if ( strlen ( szTemp2 ) < 2 )
	{
		szTemp2[1] = szTemp2[0];
		szTemp2[0] = '0';
		szTemp2[2] = '\0';
	}

	strcat ( szTemp1,szTemp2 );
	strcat ( szTemp1,"-" );

	/// Get day
	sprintf ( szTemp2,"%d",date.day );
	if ( strlen ( szTemp2 ) < 2 )
	{
		szTemp2[1] = szTemp2[0];
		szTemp2[0] = '0';
		szTemp2[2] = '\0';
	}

	strcat ( szTemp1,szTemp2 );
	strcat ( szTemp1," " );

	///Get hour
	sprintf ( szTemp2,"%d",date.hour );
	if ( strlen ( szTemp2 ) < 2 )
	{
		szTemp2[1] = szTemp2[0];
		szTemp2[0] = '0';
		szTemp2[2] = '\0';
	}

	strcat ( szTemp1,szTemp2 );
	strcat ( szTemp1,":" );

	//// Get minute
	sprintf ( szTemp2,"%d",date.minute );
	if ( strlen ( szTemp2 ) < 2 )
	{
		szTemp2[1] = szTemp2[0];
		szTemp2[0] = '0';
		szTemp2[2] = '\0';
	}

	strcat ( szTemp1,szTemp2 );

	///// Get date
	memcpy ( datebuffer,szTemp1,17 );
	
}

////��������ģ��
int TranslateCommand ( UINT8 *buffer )
{

	UINT8 bCommand = buffer[0];//////�õ�������
	int iCommandCode;

	switch ( bCommand )
	{
	case 0xb0: //////////////////////ҵ���ѯ
		iCommandCode = SMS_QUERY;
		break;
	case 0xb1: ////////////////////��Ϣ����
		iCommandCode = SMS_DOWNLOAD;
		break;
	case 0xb2: /////////////////////��Ϣ�ϴ�
		iCommandCode = SMS_UPLOAD;
		g_icommuicationstate = TRUE;  
		break;
	case 0xb3: ///////////////////��Ļѯ��
		iCommandCode = SMS_INFORMATION;
		break;
	case 0xb4: //////////////////��Ļ���
		iCommandCode =SMS_SCREENOUT;
		break;
	case 0xb5: //////////////////��ȫ����
		break;
	case 0xb6: /////////////////ͼ����������
		iCommandCode = SMS_GRAPHICS;
		break;
	case 0xb7: ////////////////�ı�CPE�Ľ���״̬
		iCommandCode = SMS_CHANGE;
		break;
	case 0xb8: ////////////////CPE���ò���ѯ��
		if ( buffer[2] == 0x01 )
			iCommandCode = SMS_IDSETTING;
		else if ( buffer[2] == 0x02 )
			iCommandCode = SMS_CPESETTING;
		break;
	}


	return iCommandCode;
}
//////////////////////////////////////////////////////////
///����Ϣ�ϴ�ģ�� ,���ݴ��
int SMSInformationUpload ( LPSENDSMS pst )
{
	
	
	/////////�绰����
	char szPhoto[81];
	////////////////�绰1
	int iLen = strlen ( pst->szPhone );
	if ( iLen < 1 ) 
		return SMS_EMPTY_PHOTO;
	

	if ( pst->szPhone[iLen-1] == '#' )
		pst->szPhone[iLen-1] = '\0';
	
	iLen = strlen ( pst->szPhone );
	if ( pst->szPhone[iLen-1] == '*' )
		strcat ( pst->szPhone ,"0" );

	iLen = strlen ( pst->szPhone );
	if ( pst->szPhone[iLen-2] != '*' )
		strcat ( pst->szPhone,"*0" );



	 int  mm,nn;  /// xgw modify
	 iLen = strlen ( pst->szPhone );
	 for(mm=0;mm<iLen;mm++)
	 {
		if((pst->szPhone[mm] == '#') && (pst->szPhone[mm-2] != '*'))
		{
			for(nn=iLen-1;nn>=mm;nn--)
				pst->szPhone[nn+2] = pst->szPhone[nn];
			
			   pst->szPhone[mm] ='*';
			   pst->szPhone[mm+1] ='0';
			   iLen = strlen ( pst->szPhone );
			   pst->szPhone[iLen] = '\0';
		 }
	 }
	
	
	 //////ȥ������4����������
	 iLen = strlen ( pst->szPhone );
	 for ( int i = 0;i < iLen;i ++ )
	 {
		if ( pst->szPhone[i] == '*' )
			if ( pst->szPhone[i+1] > '3' ||
				 pst->szPhone[i+1] < '0' )
					pst->szPhone[i+1] = '0';
	 }

	 strcpy ( szPhoto,pst->szPhone );
		
	//////////////����Ϣ
	UINT8 bLen = strlen ( szPhoto ) + strlen ( pst->szMsg );
	UINT8 *buffer = new UINT8[bLen+3]; ////DTMF��

	UINT8 bBufferLen = 0;
	buffer[bBufferLen] = bLen + 2;
	bBufferLen ++;//////�����ֽڳ���

	if ( g_SystemOption.bIsNeedReturn )
		buffer[bBufferLen] = 0x01; /////ͨ��APP����,bit0
	else buffer[bBufferLen] = 0x00;
	
	bBufferLen ++;/////////��ִ��־,0x00-No;0x01-Yes

	UINT8 bPos = 0;
	while ( szPhoto[bPos] != '\0' ) //////�绰����
	{
		buffer[bBufferLen] = szPhoto[bPos];
		bPos ++;
		bBufferLen ++;
	}
	
	buffer[bBufferLen] = 0xff;
	bBufferLen ++;//////�ָ��� 0xff

	bPos = 0;
	while ( pst->szMsg[bPos] != '\0' )////����Ϣ����
	{
		buffer[bBufferLen] = pst->szMsg[bPos];
		bPos ++;
		bBufferLen ++;
	}

	
	////////////////��UINT8��������ɵ��ֽ�
	UINT8 *buf = new UINT8[2*(bLen+3)];
	for ( iLen = 0;iLen < bLen + 3;iLen ++ )
	{
		UINT8 uc = buffer[iLen];
		UINT8 luc = uc&0x0f;
		UINT8 huc = (uc>>4)&0x0f;
		buf[2*iLen] = huc;
		buf[2*iLen+1] = luc;
	}

	////��buffer������ָ��������
	for ( iLen = 0;iLen < 2*(bLen+3);iLen ++ )
		L3Buffer[iLen] = buf[iLen];
	L3Buffer[iLen] = 0x7F;

	delete []buffer;
	delete []buf;
	
	return SMS_OK;
}

/////FSK
int SMSInformationUploadEx ( LPSENDSMS pst )
{
	/////////�绰����
	char szPhoto[81];
	////////////////�绰1
	int iLen = strlen ( pst->szPhone );
	if ( iLen < 1 ) 
		return SMS_EMPTY_PHOTO;
	

	if ( pst->szPhone[iLen-1] == '#' )
		pst->szPhone[iLen-1] = '\0';
	
	iLen = strlen ( pst->szPhone );
	if ( pst->szPhone[iLen-1] == '*' )
		strcat ( pst->szPhone ,"0" );

	iLen = strlen ( pst->szPhone );
	if ( pst->szPhone[iLen-2] != '*' )
		strcat ( pst->szPhone,"*0" );



	 int  mm,nn;  /// xgw modify
	 iLen = strlen ( pst->szPhone );
	 for(mm=0;mm<iLen;mm++)
	 {
		if((pst->szPhone[mm] == '#') && (pst->szPhone[mm-2] != '*'))
		{
			for(nn=iLen-1;nn>=mm;nn--)
				pst->szPhone[nn+2] = pst->szPhone[nn];
			
			   pst->szPhone[mm] ='*';
			   pst->szPhone[mm+1] ='0';
			   iLen = strlen ( pst->szPhone );
			   pst->szPhone[iLen] = '\0';
		 }
	 }
	
	
	 //////ȥ������4����������
	 iLen = strlen ( pst->szPhone );
	 for ( int i = 0;i < iLen;i ++ )
	 {
		if ( pst->szPhone[i] == '*' )
			if ( pst->szPhone[i+1] > '3' ||
				 pst->szPhone[i+1] < '0' )
					pst->szPhone[i+1] = '0';
	 }

	 strcpy ( szPhoto,pst->szPhone );
		
	//////////////����Ϣ
	UINT8 bLen = strlen ( szPhoto ) + strlen ( pst->szMsg );
	UINT8 *buffer = new UINT8[bLen+3]; ////DTMF��

	UINT8 bBufferLen = 0;
	buffer[bBufferLen] = bLen + 2;
	bBufferLen ++;//////�����ֽڳ���

	if ( g_SystemOption.bIsNeedReturn )
		buffer[bBufferLen] = 0x01; /////ͨ��APP����,bit0
	else buffer[bBufferLen] = 0x00;
	
	bBufferLen ++;/////////��ִ��־,0x00-No;0x01-Yes

	UINT8 bPos = 0;
	while ( szPhoto[bPos] != '\0' ) //////�绰����
	{
		buffer[bBufferLen] = szPhoto[bPos];
		bPos ++;
		bBufferLen ++;
	}
	
	buffer[bBufferLen] = 0xff;
	bBufferLen ++;//////�ָ��� 0xff

	bPos = 0;
	while ( pst->szMsg[bPos] != '\0' )////����Ϣ����
	{
		buffer[bBufferLen] = pst->szMsg[bPos];
		bPos ++;
		bBufferLen ++;
	}

	
	////////////////��UINT8��������ɵ��ֽ�
	UINT8 *buf = new UINT8[2*(bLen+3)];
	for ( iLen = 0;iLen < bLen + 3;iLen ++ )
	{
		UINT8 uc = buffer[iLen];
		UINT8 luc = uc&0x0f;
		UINT8 huc = (uc>>4)&0x0f;
		buf[2*iLen] = huc;
		buf[2*iLen+1] = luc;
	}

	////��buffer������ָ��������
	for ( iLen = 0;iLen < 2*(bLen+3);iLen ++ )
		L3Buffer[iLen] = buf[iLen];
	L3Buffer[iLen] = 0x7F;

	delete []buffer;
	delete []buf;
		
	return SMS_OK;
}

///////////////////////////////////////////////////////
////��ȡ����Ϣ
SMS GetSMSIntoBuffer ( UINT8 *buffer,int &iFlag )
{
	SMS	ss;
	memset ( &ss,0,sizeof ( ss ) );

	UINT8 bLen = buffer[1]; ///////�õ�����
	
	UINT8 bFlag = buffer[2]; //////�ж������Ƿ����
	if ( bFlag & 0x40 )
		iFlag = SMS_INFORMATION_UEND;
	else iFlag = SMS_INFORMATION_END;

	UINT8 bBufferPos = 4;////////ȡ�÷��ͷ�����
	UINT8 bTempPos = 0;
	while ( buffer[bBufferPos] != 0xff )
	{
		ss.szPhone[bTempPos] = buffer[bBufferPos];
		bTempPos ++;
		bBufferPos ++;
	}

	ss.szPhone [bTempPos] = '\0';

	bBufferPos ++; //////////�õ����շ���������
	ss.bMailBox = buffer[bBufferPos++] - 0x30; 
	if ( ss.bMailBox > 3 || ss.bMailBox < 0 ) 
		ss.bMailBox = 0;
		
	//////////////ȡ��ʱ��
	for ( bTempPos = 0;bTempPos < 14;bTempPos ++,bBufferPos ++ )
		ss.szTime[bTempPos] = buffer[bBufferPos];
	
	for ( int j = 15;j > 1;j -- )
		ss.szTime[j] = ss.szTime[j-2];
	//ss.szTime [14] = '\0';
	ss.szTime [0] = '2';
	ss.szTime [1] = '0';
	ss.szTime [16] = '\0';

	///////////////ȡ�ö���Ϣ����
	bTempPos = 0;
	while ( bBufferPos <= bLen + 1 )
	{
		ss.szMsg[bTempPos] = buffer[bBufferPos];
		bTempPos ++;
		bBufferPos ++;
	}

	ss.szMsg [bTempPos] = '\0';


	return ss;

}


//////////////////////////////////////////////////////////////////
///// ҵ��ѯ�ʴ���ģ��
void SMSQueryServerID (  )
{
	////////������Ϣ����·�㣬˵������Ϣ����
	//////��һ���ֽ�ת���ɶ����ֽ�
	int i;
	for ( i = 0;i < 7;i ++ )
		L3Buffer[i] = 0;
		
	if ( g_iServerType == 0 ){ ////������Ϣ��
	
		//// 4B 0x00000000
		L3Buffer[7] = 0x00;

	}else if ( g_iServerType == 1 ){///������Ϣ����
	
		//// 4B 0x00000001
		L3Buffer[7] = 0x01;
		
	}else if ( g_iServerType == 2 ){///Email
	
		//// 4B 0x00000002
		L3Buffer[7] = 0x02;
	}

	L3Buffer[8] = 0x7F;
	SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
}
/////////////////////////////////////////////////////////////////
//// ��ȡEMail
MAILSTRUCT GetMailIntoBuffer ( UINT8 *buffer )
{	
		
	MAILSTRUCT	ms; ////����Mail����ʱ�ṹ
	memset ( &ms,0,sizeof ( MAILSTRUCT ) );


	UINT8	bLen = buffer[1]; /////�õ��������ĳ���
	int iPos = 4; 
	while ( true )  //////EMail���ͷ���ַ
	{
		ms.szAddress[iPos-4] = buffer[iPos];
		iPos++;
		if ( buffer[iPos] == 0xFF ) break;
	}

	ms.szAddress[iPos-4] = '\0';

	int iTempPos = 0;
	while ( iPos <= bLen )
	{
		ms.szMail[iTempPos] = buffer[iPos];
		iPos ++;
		iTempPos ++;
	}

	ms.szMail[iTempPos] = '\0';

	return ms;
}	

////////////////////////////////////////////////////////////////////
/////��ȡ���ĵĶ���Ϣ
SUBSCIBESTRUCT GetSubscibeIntoBuffer ( UINT8 *buffer )
{

	SUBSCIBESTRUCT ss;
	memset ( &ss,0,sizeof ( ss ) ); 

	UINT8 bLen = buffer[1]; //// �õ������ĳ���

	int i;
	UINT8 pos = 4;
	for ( UINT8 i = 0;i < 8;i ++,pos ++ ) //////ICP����
		ss.ICPCode[i] = buffer[pos];

	for ( i = 0;i < 10;i ++,pos ++ ) /////ҵ�����
		ss.ICPOperationCode[i] = buffer[pos];

			
	for ( i = 0;buffer[pos] != 0xFF;i ++,pos++ )//////////��Ϣ�ı���
		ss.szTitle[i] = buffer[pos];
	ss.szTitle[i] = '\0';

	pos ++;///////�����ָ���
	for ( i = 0;pos < bLen;i ++,pos ++ )  //////���Ķ��ŵ�����
		ss.szContent[i] = buffer[pos];
	ss.szContent[i] = '\0';


	return ss;
}

//////////////////////////////////////////////////////////////////////////
/////// ȡ�û����˵�
MONEY GetMoney ( UINT8 *buffer )
{
	MONEY money;

	UINT8	iLen = buffer[0];

	int i;
	for ( i = 5;;i ++ )  ///// Get title
	{
		money.szTitle[i-5] = buffer[i];
		if ( buffer[i] == 0xFF ) 
		{
			money.szTitle[i-5] = '\0';
			break;
		}
	}

	i ++;
	int pos = 0;
	for ( ;i < iLen;i++ )
	{
		money.szContent[pos] = buffer[i];
		pos ++;
	}

	money.szContent[pos] = '\0';


	return money;

}

////////////////////////////////////////////////////////////////////////////
////// ��ȡ������
MODELDOWNLOAD GetModel ( UINT8 *buffer )
{
	MODELDOWNLOAD model;
	/////fomat: ������2B|����|�ܰ���|���|������Ϣ
	UINT8	iLen = buffer[4]; 

	if ( iLen > 40 ) iLen = 40;
	UINT8 i,pos = 0;
	for ( i = 5;i < iLen + 5  ;i ++,pos ++ )
		model.szContent[pos] = buffer[i];

	model.szContent[pos] = '\0';

	return model;

}
////////////////////////////////////////////////////////////////////////////
///// ��Ϣ����ģ��:���Ͳ�������
int SMSInformationDownload ( UINT8 *buffer )
{

	int iCmd;	

	switch ( buffer[3] ) //////������
	{
	case EMM_Part: ///////////Email
		g_pL23MsgBuffer[0] = 0x0D;//DTMF_A0; //////�����ڸò���
		g_pL23MsgBuffer[1] = 0x0A;
		SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		iCmd = SMS_TYPE_ERROR;
		break;
	case SMM_Part: /////////��ͨ����Ϣ
		iCmd = SMS_TYPE_NORMAL;
		break;
	case SSM_Part: /////////���Ķ���Ϣ
		iCmd = SMS_TYPE_USERMSG;
		break;
	case 0x04: /////////Memory��д����
		
		g_pL23MsgBuffer[0] = 0x0D;//DTMF_A0; //////�����ڸò���
		g_pL23MsgBuffer[1] = 0x0A;
		SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		iCmd = SMS_TYPE_ERROR;

		break;
	case 0x05://///////IC��д����
		g_pL23MsgBuffer[0] = 0x0D;//DTMF_A0; //////�����ڸò���
		g_pL23MsgBuffer[1] = 0x0A;
		SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		iCmd = SMS_TYPE_ERROR;
		
		break;
	case CSM_Part://///////������洢��
		iCmd = SMS_MODEL;
		break;
	case SRM_Part://///////��ɫ�����洢��
	     iCmd = SMS_RING;	
		break;
	case FBM_Part://///////�����˵��洢��
		iCmd = SMS_MONEY;
		break;
	case FDBM_Part://///////�����鵥�洢��
		iCmd = SMS_MONEYEX;
		break;
	}

	return iCmd;
}

//////////////////////////////////////////////////////////////////
/////�ı�CPE�Ľ���״̬ģ��
void SMSChangeReceiveStatus ( UINT8 *buffer )
{
	
	if ( buffer[2] == 0x00 ) { ////////////////�һ���׼������
	
		
		g_pL23MsgBuffer[2] = 0x00;
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );///֪ͨ��·��һ�
			// g_iServerType = -1 ��ʾ��Ч;///�������� 0-������Ϣ������1-������Ϣ������2-Email��Ϣ����
		if(g_iServerType == 1 )
		{    
			if( g_icommuicationstate )
			{
				g_icommuicationstate = false;

				if ( m_pShowMsg ) ///// syj modify 2002.1.5
				{
					
					m_pShowMsg->Destroy ( );
					delete m_pShowMsg;
					m_pShowMsg = NULL;
	
				}
			}
		}
		
		if(g_iServerType == 0 )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			
			
			g_pShowMsg->SetMessage ("�һ�!");
			NU_Sleep ( 400 );  // waiting 400*5 =2000ms
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
		}
	
	}else if ( buffer[2] == 0xFF ) { //////��������ͨ�ţ����뻰����ʽ
	
		g_pL23MsgBuffer[2] = 0xFF;
		
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );
		
	
	}else {
	
		g_pL23MsgBuffer[2] = buffer[2];
		
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );
	}


	///////////����Ҽ�e���ڴ��ڽ�������
	if ( m_pShowMsg ) ///// syj modify 2002.1.5
	{
					
		m_pShowMsg->Destroy ( );
		delete m_pShowMsg;
		m_pShowMsg = NULL;
	
	}


	if ( m_pService ) 
	{

		m_pService->Destroy ( );
		delete m_pService;
		m_pService = NULL;
	
	}
			
}

///////////////////////////////////////////////////////////////////
////ϵͳ״̬����ģ��
void SMSQueryCPEConfig ( UINT8 statuscode )  /////DTMF
{
	if ( statuscode == 0x01 ) ///////�ն�ID����
	{
		UINT8	bufferID[8]; /////�ն�ID�������ݰ�9���ֽ�
		int i;

		bufferID[0] = 0x18; ///////���Ҵ���
		bufferID[1] = 0x02;////////�ն�����
						  ////////0x01 - ��׼��
		                  ////////0x02 - ������
		bufferID[2] = 0x01;////////�꣺��BDC���뷽ʽ ��1999�꣩
		bufferID[3] = 0x01;////////��: .............  (12��)
		bufferID[4] = 0x01; ///////�汾�ţ������Զ�
		
		bufferID[5] = 0x01; ///////���кţ������Զ���3���ֽ�
		bufferID[6] = 0x02;
		bufferID[7] = 0x03;
		
		//////////////��һ���ֽ�ת���ɶ����ֽ�
		UINT8    tempBuffer[17];
		for ( i = 0;i < 8;i ++ )
		{
		
			UINT8 h = (bufferID[i]>>4)&0x0f;
			UINT8 l = bufferID[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[16] = 0x7F;
		/////////������ָ��������
		for ( i = 0;i < 17;i ++ )
			L3Buffer[i] = tempBuffer[i];
		
	}else if ( statuscode == 0x02 ){ /////////////�ն�������Ϣ����
	
		UINT8	bufferCPE[3];//////�ն�������Ϣ����
		int i;
		
		bufferCPE[0] = 0x0A;//////�ն�������
		bufferCPE[1] = 0x09;//////�ն�������
		bufferCPE[2] = 0x0B;//////������־
		                   //////bit0:��ͨ�ն�
		                   //////bit1:ͼ������
						   //////bit2:��ȫ����
						   //////bit3:Email����
						   //////bit4:FSK�ϴ�����
						   //////bit5:����
						   //////bit6:����
						   //////bit7:����

		//////////////��һ���ֽ�ת���ɶ����ֽ�
		UINT8    tempBuffer[7];
		for ( i = 0;i < 3;i ++ )
		{
		
			UINT8 h = (bufferCPE[i]>>4)&0x0f;
			UINT8 l = bufferCPE[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[6] = 0x7F;
		/////////������ָ��������
		for ( i = 0;i < 7;i ++ )
			L3Buffer[i] = tempBuffer[i];

	}

}

//////////////// FSK
void SMSQueryCPEConfigEx ( UINT8 statuscode )  
{
	if ( statuscode == 0x01 ) ///////�ն�ID����
	{
		UINT8	bufferID[8]; /////�ն�ID�������ݰ�9���ֽ�
		int i;

		bufferID[0] = 0x18; ///////���Ҵ���
		bufferID[1] = 0x02;////////�ն�����
						  ////////0x01 - ��׼��
		                  ////////0x02 - ������
		bufferID[2] = 0x01;////////�꣺��BDC���뷽ʽ ��1999�꣩
		bufferID[3] = 0x01;////////��: .............  (12��)
		bufferID[4] = 0x01; ///////�汾�ţ������Զ�
		
		bufferID[5] = 0x01; ///////���кţ������Զ���3���ֽ�
		bufferID[6] = 0x02;
		bufferID[7] = 0x03;
		
		//////////////��һ���ֽ�ת���ɶ����ֽ�
		UINT8    tempBuffer[17];
		for ( i = 0;i < 8;i ++ )
		{
		
			UINT8 h = (bufferID[i]>>4)&0x0f;
			UINT8 l = bufferID[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[16] = 0x7F;
		/////////������ָ��������
		for ( i = 0;i < 17;i ++ )
			L3Buffer[i] = tempBuffer[i];
		
	}else if ( statuscode == 0x02 ){ /////////////�ն�������Ϣ����
	
		UINT8	bufferCPE[3];//////�ն�������Ϣ����
		int i;
		
		bufferCPE[0] = 0x0A;//////�ն�������
		bufferCPE[1] = 0x09;//////�ն�������
		bufferCPE[2] = 0x0B;//////������־
		                   //////bit0:��ͨ�ն�
		                   //////bit1:ͼ������
						   //////bit2:��ȫ����
						   //////bit3:Email����
						   //////bit4:FSK�ϴ�����
						   //////bit5:����
						   //////bit6:����
						   //////bit7:����

		//////////////��һ���ֽ�ת���ɶ����ֽ�
		UINT8    tempBuffer[7];
		for ( i = 0;i < 3;i ++ )
		{
		
			UINT8 h = (bufferCPE[i]>>4)&0x0f;
			UINT8 l = bufferCPE[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[6] = 0x7F;
		/////////������ָ��������
		for ( i = 0;i < 7;i ++ )
			L3Buffer[i] = tempBuffer[i];

	}

}


///////////////////////////��Ļѯ��ģ��
BOOL ScreenEnquire ( UINT8 *buffer )
{
	
	int i;
	for ( i = 0;i < 256;i ++ )
		g_pMenuBuffer[i] = buffer[i];
	
	m_pService->SendMessage ( WM_UPDATE_MENU,0,0 );///֪ͨ���¿ͷ����Ĳ˵�

	return TRUE;
}
//////////////////////////��Ļ���ģ��
BOOL ScreenPutOut ( UINT8 *buffer )
{
	extern void SMSOpenFSK ( );
	SMSOpenFSK ( );

	int i;
	for ( i = 0;i < 256;i ++ )
		g_pMenuBuffer[i] = buffer[i];
	
	if ( m_pService )
		m_pService->SendMessage ( WM_DISPLAY_MSG,0,0 );///֪ͨ���¿ͷ����Ĳ˵�
	else {
	
		UINT8	bLen = g_pMenuBuffer[1],////����
				bChildCmd = g_pMenuBuffer[2];//��������

		char    szTitle[40];
		char    szTemp[256];
		int i;


		for ( i = 4;i < bLen + 2;i ++ )
			szTemp[i-4] = g_pMenuBuffer[i];		

		szTemp[i-4] = '\0';
		/////��ʾe��Ϣ
		if ( !m_pShowMsg )
			m_pShowMsg = new CShowMessageEx;
		
		/////�Ƿ�������ʱ��
		if ( g_pMenuBuffer[3] == 0x01 )
			NU_Reset_Timer(&SYJTS, SYJTimerOver, 1000, 200, NU_ENABLE_TIMER);				
		
		m_pShowMsg->SetTimerFlag ( g_pMenuBuffer[3] );
		m_pShowMsg->SetString ( szTemp ); 
		m_pShowMsg->SetDestroyFlag ( 1 );
		m_pShowMsg->Create ( "��ʾ",
			WS_VISIBLE | WS_CAPTION | WS_CHILD,
			CRect ( 0,0,159,159 ) ,0 );

		
		m_pShowMsg->ShowWindow ( );
	
	}

	return TRUE;
	
}

////////////////////////////////// class CSMSMessage
/////////ɾ��Memory�еĶ����¼
BOOL DeleteRecordInMemory ( int iMailBox )
{
	
	int i;
	for ( i = 0;i < SMS_MAX_SAVE_NUMBER;i ++ )
	{
		if ( !g_SendedSMS[iMailBox][i].iLockStatus  )  ////û�м���������ɾ��
		{
			
			if ( g_SendedSMS[iMailBox][i].bIsFlash )
			{
				//////����С��ID��Ӧ�ļ�¼ɾ��
				DeleteSMS ( g_SendedSMS[iMailBox][i].dataID );	
				g_SystemOption.SMSStatus[iMailBox].bHavePasswordItem --;
			}

			////�ƶ�Memory�е�����
			memmove ( &g_SendedSMS[iMailBox][i],
				&g_SendedSMS[iMailBox][i+1],
				(SMS_MAX_SAVE_NUMBER-i-1)*sizeof ( SMS ) );
			
			g_SystemOption.SMSStatus[iMailBox].bSendedNumber --;

			return TRUE;
		}
	}
	//////û�з��������ļ�¼
	return FALSE;
}


/////////ɾ��Flash�еĶ����¼
BOOL DeleteRecord ( int iMailBox,int iChildBox )
{
		/////////������ķ�ʽ���Ƚ��ȳ�
		//////�����Ƚ���ļ�¼ɾ��
		int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////���Ҽ�¼			
		int pos = 0,i,j;
		
		for ( i = 0;i < r;i ++ ) ///����
		{
		
			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///�õ���¼ָ��
			if ( lpST->bMailBox == iMailBox && /////�ж������Ƿ����ڵ�ǰѡ������
				lpST->iChildMailBox == iChildBox ) {			////�еķ�����
			
				if ( !lpST->iLockStatus &&
					 lpST->iLetterStatus == ICON_LETTERREADED )  ////û�м���������ɾ��
				{
					DeleteSMS ( g_pSMSDataTempIDBuf[i] );
					pos ++;
					break;

				}
			}
		
		}

		if ( !pos ) return FALSE;////�Ҳ������������ļ�¼

		if ( iChildBox == 0 )
			g_SystemOption.SMSStatus[iMailBox].bRevNumber --;
		else if ( iChildBox == 1 )
			g_SystemOption.SMSStatus[iMailBox].bSendNumber --;
		//else if ( iChildBox == 2 )
		//	g_SystemOption.SMSStatus[iMailBox].bHavePasswordItem --;

		return TRUE;

}
///////��ӻ��Ѽ�¼
int AddMoneyRecord ( MONEY money ,BOOL bStyle )
{

	int i;
	if ( bStyle ){  /////��ͨ����
	
		for ( i = 0;i < SMS_MONEY_MAX_NUMBER;i ++ )
			if ( !g_Money[i].bIsData ) 
			{
				g_Money[i] = money;
				return 1;
			}
		
		for ( i = 0;i < SMS_MONEY_MAX_NUMBER-1;i ++ )
			g_Money[i] = g_Money[i+1];

		g_Money[i] = money;

		return 1;
	
	}else { //////��ϸ����
	
		for ( i = 0;i < SMS_MONEYEX_MAX_NUMBER;i ++ )
			if ( !g_MoneyEx[i].bIsData ) 
			{
				g_MoneyEx[i] = money;
				return 1;
			}
		
		for ( i = 0;i < SMS_MONEYEX_MAX_NUMBER-1;i ++ )
			g_MoneyEx[i] = g_MoneyEx[i+1];

		g_MoneyEx[i] = money;

		return 1;
	
	}

	return 1; /////ok
}
///////��Ӷ��ĵĶ���Ϣ
int AddSubscibeRecord ( SUBSCIBESTRUCT st )
{
	/////////���Ķ���Ϣ�����ﵽ���
	if ( g_iSubscribePos > SMS_SUBSCIBE_MAX_NUMBER - 1 )
	{
		for ( int i = 0;i < SMS_SUBSCIBE_MAX_NUMBER - 2;i ++ )
			g_SubscibeSMS[i] = g_SubscibeSMS[i+1];
		
		g_SubscibeSMS[SMS_SUBSCIBE_MAX_NUMBER-1] = st;
	
	}else { ////δ�ﵽ���ֵ
	
		g_SubscibeSMS[g_iSubscribePos] = st;
		g_iSubscribePos ++;	
	
	}


	return 1;
}

///////����ѷ������¼
BOOL AddSendedRecord ( SMS ss )
{

	if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber>SMS_MAX_SAVE_NUMBER-1 )
		if ( DeleteRecordInMemory  ( iCurrentLetterBox ) == FALSE ) /////ɾ��û�õļ�¼
			return FALSE;

	
	g_SendedSMS[ss.bMailBox][g_SystemOption.SMSStatus[ss.bMailBox].bSendedNumber] = ss;
	g_SystemOption.SMSStatus[ss.bMailBox].bSendedNumber ++;

	return TRUE;
}
/////////��ӷ���ʧ�ܵļ�¼
BOOL AddSendFailedRecord ( )
{
	if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber>SMS_MAX_SAVE_NUMBER-1 )
		if ( DeleteRecord ( iCurrentLetterBox, 1 ) == FALSE ) /////ɾ��û�õļ�¼
			return FALSE;


	///////// �򷢼�������һ������ʧ�ܵļ�¼
	if ( strlen ( g_SendST.szPhone ) <= 0 )
		return FALSE;

	SMS ss;
	memset ( &ss,0,sizeof ( ss ) );///// clear 0
	strcpy ( ss.szPhone,g_SendST.szPhone ); ////Phone
	strcpy ( ss.szMsg,g_SendST.szMsg );  /////Msg

	
	////// ���ʱ��
	FillTime ( ss.szTime );

	
	ss.Flag = 0;///ֻ��
	ss.bMailBox = iCurrentLetterBox;///��ǰѡ�е�����
	ss.iChildMailBox = 1;/////���浽������
	ss.iLetterStatus = ICON_SENDFAILED;////ʧ��

	int r = AppendSMS  ( &ss );
	if ( r == -1 ) 
		::MessageBox ( "�洢����������","����",MB_OK|MB_ICONSTOP );

	g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber ++;
	
	return TRUE;
}

/////////��������Ӽ�¼
BOOL AddSendRecord ( SMS ss ,BOOL bIsChange )
{
	ss.bIsFlash = 0;
	if ( strlen ( ss.szPhone ) <= 0 ) return FALSE;

	if ( bIsChange ) /////����ģʽ
	{
	
		ModifySMS ( g_CurrentID,&ss ); 
		
		return TRUE;
	}

	/////////// ��ʾ�û���Ҫ�ﵽ�洢����
	if ( g_SystemOption.SMSStatus[ss.bMailBox].bSendNumber>SMS_MAX_SAVE_NUMBER-1 )
	{
		if ( MessageBox ( "��������������������ԭ��һ�����ݣ�ȷ����?",
		             "����",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			if ( DeleteRecord ( ss.bMailBox, ss.iChildMailBox ) == FALSE ) /////ɾ��û�õļ�¼
			{
				MessageBox ( "�洢ʧ�ܣ�","����",MB_OK|MB_ICONSTOP ); 
				return FALSE;
			}
		
		}else {
		
			return FALSE;
		}

	}


	static short iCmpID = -2;

	if ( g_CurrentID != iCmpID ) {////������¼
	
		int r = AppendSMS  ( &ss );
		if ( r == -1 ) 
		{
			MessageBox ( "�洢����������","����",MB_OK|MB_ICONSTOP );
			return FALSE;
		}

		g_CurrentID = r;
		iCmpID = r;

	}else {//////////�޸ļ�¼
	
		ModifySMS ( g_CurrentID,&ss ); 
	}
	

	 
	g_SystemOption.SMSStatus[ss.bMailBox].bSendNumber ++;
	
	return TRUE;
}
////////���ռ�����Ӽ�¼
short AddRevRecord ( SMS ss ,BOOL bIsDownload )
{
	int r;
	char szName[81];
	if ( g_SystemOption.SMSStatus[ss.bMailBox].bRevNumber>SMS_MAX_SAVE_NUMBER-1 ) ////��������
	{
	
		if ( !g_SystemOption.SMSStatus[g_ST.bMailBox].bIsCanDelete ) ///�ܽ�ģʽ
		{
			if ( bIsDownload ) ///�Ƿ�����
			{
				///////֪ͨ��·��һ�
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);

				UINT8  temp[2];


				g_pL23MsgBuffer[0] = 0x0D;//D1 /////�洢������
				g_pL23MsgBuffer[1] = 0x01;

				
				temp[0]=g_pL23MsgBuffer[0];
				temp[1]=g_pL23MsgBuffer[1];
				OrderSendDTMFData(temp,2,-1);

				//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

				g_pL23MsgBuffer[2] = 0x00;
				SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );


			}

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();

			strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
			strcat ( szName,"�洢�ռ�����" );
			g_pShowMsg->SetMessage ( szName );
			NU_Sleep ( 200 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			return -1;
			
		}else { ///�Ƚ��ȳ�ģʽ
		
			if ( DeleteRecord ( ss.bMailBox, ss.iChildMailBox ) == FALSE ) /////ɾ��û�õļ�¼
			{
				if ( bIsDownload ) ///�Ƿ�����
				{
					///////֪ͨ��·��һ�
					NU_Control_Timer(&TS, NU_DISABLE_TIMER);

					g_pL23MsgBuffer[0] = 0x0D;//D1 /////�洢������
					g_pL23MsgBuffer[1] = 0x01;
					SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

					g_pL23MsgBuffer[2] = 0x00;
					SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

				}

				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
				
				strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
				strcat ( szName,"�洢�ռ�����" );
				g_pShowMsg->SetMessage ( szName );

				NU_Sleep ( 200 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );


				return -1;
			}

			ss.bIsFlash = 0;
			r = AppendSMS  ( &ss );//// Add a record
			if ( r == -1 ) 
			{
			
				if ( bIsDownload ) ///�Ƿ�����
				{
					///////֪ͨ��·��һ�
					NU_Control_Timer(&TS, NU_DISABLE_TIMER);

					g_pL23MsgBuffer[0] = 0x0D;//D1 /////�洢������
					g_pL23MsgBuffer[1] = 0x01;
					SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

					g_pL23MsgBuffer[2] = 0x00;
					SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

				}
				
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();

				strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
				strcat ( szName,"�洢�ռ�����" );
				g_pShowMsg->SetMessage ( szName );

				NU_Sleep ( 200 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );
				

			}else { /////���Ӽ�¼�ɹ�

				if ( bIsDownload ) ///�Ƿ�����
				{
					g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
					g_pL23MsgBuffer[1] = 0x0D;

					UINT8 temp[2];
					temp[0]=g_pL23MsgBuffer[0];
					temp[1]=g_pL23MsgBuffer[1];
					OrderSendDTMFData(temp,2,-1);

					//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
				}
			}
		
		}
	
	}else { //////����δ����ֱ�Ӵ洢
	
			ss.bIsFlash = 0;
			r = AppendSMS  ( &ss );//// Add a record
			if ( r == -1 ) 
			{
			
				if ( bIsDownload ) ///�Ƿ�����
				{
					///////֪ͨ��·��һ�
					NU_Control_Timer(&TS, NU_DISABLE_TIMER);

					g_pL23MsgBuffer[0] = 0x0D;//D1 /////�洢������
					g_pL23MsgBuffer[1] = 0x01;
					SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

					g_pL23MsgBuffer[2] = 0x00;
					SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

				}
				
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
			
				strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
				strcat ( szName,"�洢�ռ�����" );
				g_pShowMsg->SetMessage ( szName );

				NU_Sleep ( 200 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );
				

			}else { /////���Ӽ�¼�ɹ�

				if ( bIsDownload ) ///�Ƿ�����
				{
					g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
					g_pL23MsgBuffer[1] = 0x0D;
					
					UINT8 temp[2];
					temp[0]=g_pL23MsgBuffer[0];
					temp[1]=g_pL23MsgBuffer[1];
					OrderSendDTMFData(temp,2,-1);

					//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
				}
			}
	
	}
				
	
	
	g_SystemOption.SMSStatus[ss.bMailBox].bRevNumber ++;
	SetLED ( LED_SMS ); ////���ж���ʱ����LED����

	return r;
	
}

//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CSMSMessage, CFrameWnd) 
	ON_MESSAGE( NULL, Msg_Data_Chain_Msg , OnDataChainMsg) ///�´���FSK��Ϣ���
	ON_MESSAGE( NULL,Msg_Chain_Msg,OnChainMsg)////////////////������Ϣ
	ON_MESSAGE ( NULL,Msg_TimeOver_Chain_Msg,OnTimerOver )////��ʱ�洦
	ON_MESSAGE ( NULL,Msg_Unsuccess_Chain_Msg,OnUnsuccessChainMsg ) ////����ʧ��
END_MESSAGE_MAP

CSMSMessage::CSMSMessage()
{

}

CSMSMessage::~CSMSMessage()
{

}

///////////////////////////////////////////////////////////////
/////��Ϣģ��
//////////////////��ʱ��Ϣ
void CSMSMessage::OnTimerOver ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{		
	if ( g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow ( SW_HIDE ); // ????
	if ( g_iServerType != 0 ) ////�������߽���
	{
		AddSendFailedRecord ( ); ////�����������һ��ʧ�ܵ��ż�			
	}
}

/////// ����ʧ��
void CSMSMessage::OnUnsuccessChainMsg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ();
	g_pShowMsg->SetMessage ( "���ݴ������!" );
	
	g_pL23MsgBuffer[2] = 0x00;
	SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );///�һ�
	if ( g_iServerType != 0 ) ////�������߽���
	{
		AddSendFailedRecord ( ); ////�����������һ��ʧ�ܵ��ż�			
	}
	NU_Sleep ( 100 );
	if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );
}

void CSMSMessage::OnDataChainMsg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	///////////////////����·���������Ϣ
	///////////////////������Ϣ
	int i;
	UINT8 buffer[ArrayLength];
	for ( i = 0;i < ArrayLength;i ++)
		buffer[i]  = L3Buffer[i];
	//memset ( L3Buffer,0,ArrayLength );

	int iFlag;
	int r = TranslateCommand ( buffer );

	if ( SMS_QUERY == r )				///////////ҵ���ѯ
	{
		SMSQueryServerID ( );

	}else if ( SMS_DOWNLOAD == r ){		////////////��Ϣ����

		
		r = SMSInformationDownload ( buffer );
		if ( r == SMS_TYPE_NORMAL ){
			
			/////�����û����µĶ���Ϣ
			/////������Ϣ���ݴӻ���������ȡ
			g_ST = GetSMSIntoBuffer ( buffer,iFlag );

			///////////��������·����Ϣ
			if ( iFlag == SMS_INFORMATION_END ) /////���ؽ���
			{
				///// ��ʾ����Ϣ���ݣ������浽�ռ���
				/////�����ż�
				//char str[80];
				if ( g_ST.bMailBox == 0 ) ////��������
					g_ST.bMailBox = 3;
				else if ( g_ST.bMailBox == 1 ) ///˽��1
					g_ST.bMailBox = 0;
				else if ( g_ST.bMailBox == 2 )
					g_ST.bMailBox = 1;
				else if ( g_ST.bMailBox == 3 ) 
					g_ST.bMailBox = 2;

				g_ST.bReadStatus = 0;
				g_ST.iChildMailBox = 0;
				g_ST.iLetterStatus = ICON_LETTERUNREAD;////δ��;
				g_ST.Flag = 0;///ֻ��
				


				//////����һ����¼
				if ( ( r = AddRevRecord ( g_ST ,TRUE ) ) != -1 )
					g_CurrentID = r;
				else return;
				
				

				///////֪ͨ��·��һ�
				//NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				//g_pL23MsgBuffer[2] = 0x00;
				//SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );				
				///֪ͨ�û����µ��ż�
				strcpy ( g_szHaveNewMessage,"[" );
				strcat ( g_szHaveNewMessage,g_SystemOption.SMSStatus[g_ST.bMailBox].szName );
				strcat ( g_szHaveNewMessage,"]�������¶���!" );

				g_bIsRing = TRUE;

				}else if ( iFlag == SMS_INFORMATION_UEND ){////����ʧ��
				
				////////////���������ٷ���
				//////////��ʾ���û�
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
				g_pShowMsg->SetMessage ( "����ʧ��." );
				NU_Sleep ( 400 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );
				
			}


		}else if ( r == SMS_TYPE_USERMSG ){ ////���ĵĶ���Ϣ
		
			
			
			SUBSCIBESTRUCT st = GetSubscibeIntoBuffer ( buffer );
			st.iconID = ICON_LETTERUNREAD;
			////// ���ʱ��
			FillTime ( st.szTime );

			AddSubscibeRecord ( st );
			
			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
			g_pL23MsgBuffer[1] = 0x0D;

			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);

			///// hide windows

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "���¶�����Ϣ" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			///////֪ͨ��·��һ�
			

			//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

		}else if ( r == SMS_MONEY ) { /////�����˵�����
		
			MONEY money = GetMoney ( buffer );
			money.iconID = ICON_LETTERUNREAD;
			money.bIsData = 1;
			AddMoneyRecord ( money, TRUE );

			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
			g_pL23MsgBuffer[1] = 0x0D;

			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);
			

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "�򵥻����˵����سɹ�" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			

			//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		
		}else if ( r == SMS_MONEYEX ) { /////�����굥����
		
			MONEY money = GetMoney ( buffer );
			money.bIsData = 1;
			money.iconID = ICON_LETTERUNREAD;
			AddMoneyRecord ( money, FALSE );

			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
			g_pL23MsgBuffer[1] = 0x0D;

			
			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);
		

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "��ϸ�����˵����سɹ�" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			

			//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

		}else if ( r == SMS_RING ){		//////��ɫ����
	

			extern int SMS_SAVE_RING(BYTE * );

			UINT8 tempBuffer[ArrayLength];
			for ( i = 4;i < ArrayLength;i ++ )
				tempBuffer[i-4] = buffer[i];
			SMS_SAVE_RING ( tempBuffer );

			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
			g_pL23MsgBuffer[1] = 0x0D;

			
			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);
			
			/////////��ʾ�µ���������
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "���µ���ɫ����" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
			
	
			


		}else if ( r == SMS_MODEL ){ /////����������
		
			MODELDOWNLOAD model = GetModel ( buffer );
			SUBSCIBESTRUCT st;
			st.iconID = ICON_LETTERUNREAD;
			strcpy ( st.szTitle,"������" );
			strcpy ( st.szContent,model.szContent );
			////// ���ʱ��
			FillTime ( st.szTime );

			/*DateTime	date;
			GetSysTime ( &date );
			sprintf ( st.szTime,"%d:%d",
				  date.hour,
				  date.minute );*/

			AddSubscibeRecord ( st );
			
			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; ���سɹ�
			g_pL23MsgBuffer[1] = 0x0D;

			
			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);

			//NU_Control_Timer(&TS, NU_DISABLE_TIMER);
			//g_pL23MsgBuffer[2] = 0x00;
			//SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

		
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "���³�����" );
			NU_Sleep ( 500 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			
		
		}else if ( r == SMS_TYPE_ERROR ){ /////����Ϣ����
		
		
		}

		
	}else if ( SMS_UPLOAD ==  r ){				////////////��Ϣ�ϴ�


#ifdef SMS_DTMF
		if ( SMSInformationUpload ( &g_SendST ) == SMS_EMPTY_PHOTO ) /////�յ绰����
		{
			::MessageBox ( "�绰���벻��Ϊ�գ����������롣","����",MB_OK );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
			return ;
		}
#endif

#ifdef SMS_FSK
		if ( SMSInformationUploadEx ( &g_SendST ) == SMS_EMPTY_PHOTO ) /////�յ绰����
		{
			::MessageBox ( "�绰���벻��Ϊ�գ����������롣","����",MB_OK );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
			return ;
		}
#endif

		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );///֪ͨ��·��ȥ��������
		bIsSuccessSend = TRUE; ////��־����

		//////��Ϊ���ͳɹ�������������Ϣ�洢���ѷ�����
		SMS ss;
		memset ( &ss,0,sizeof ( SMS ) );///// clear 0
		strcpy ( ss.szPhone,g_SendST.szPhone ); ////Phone
		strcpy ( ss.szMsg,g_SendST.szMsg );  /////Msg

		
		////// ���ʱ��
		FillTime ( ss.szTime );
		
		ss.Flag = 0;///ֻ��
		ss.bMailBox = iCurrentLetterBox;///��ǰѡ�е�����
		ss.iChildMailBox = 2;/////���浽������
		ss.iLetterStatus = ICON_SENDOK;////�ɹ�
		ss.dataID = -1;		
			
		///���ѷ�����������һ����¼
		AddSendedRecord ( ss );				

	}else if ( SMS_IDSETTING == r ){		////ѯ��ID����

#ifdef SMS_DTMF	
		SMSQueryCPEConfig ( 0x01 );
#endif

#ifdef SMS_FSK
		SMSQueryCPEConfigEx ( 0x01 );
#endif
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
	
	}else if ( SMS_CPESETTING == r ) {		/////ѯ��CPE����

#ifdef SMS_DTMF
		SMSQueryCPEConfig ( 0x02 );
#endif

#ifdef SMS_FSK
		SMSQueryCPEConfigEx ( 0x02 );
#endif
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
	
	}else if ( SMS_CHANGE == r ){ ///////�ı仰��״̬
	
		SMSChangeReceiveStatus ( buffer );//////�һ�׼��
		///�ӵ��һ�����ر���Ϣ��
		
	}else if ( SMS_INFORMATION == r ){////////////��Ļѯ�ʣ��û���������
	
		////////��ʱδ��
		ScreenEnquire ( buffer );

	}else if ( r == SMS_SCREENOUT ){ /////��Ļ���,0xB4
	
		ScreenPutOut ( buffer );		
	
	}else if ( r == SMS_GRAPHICS ) {  /////ͼ�ι���ģ��
	
		for ( i = 0;i < 256;i ++ )
			g_pMenuBuffer[i] = buffer[i];
		
		/////����ͼ�δ���
		if ( !g_pGraphicsWnd )
			g_pGraphicsWnd = new CDrawPicWnd;

		g_pGraphicsWnd->Create ( "��ͼ",
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD,
			CRect ( 0,0,159,159 ) );

		g_pGraphicsWnd->ShowWindow ( );

	
	} 


}

///////������Ϣ����
void CSMSMessage::OnChainMsg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	switch ( g_pL23MsgBuffer[0] )
	{
	case SMS_CENTER_CALL: /////����Ϣ��������
		break;
	case SMS_CAS: ///////�ɹ��յ�CAS�ź�
		//////////֪ͨ�û����ӳɹ�
		break;
	case SMS_PHONE_STATUS:////�绰״̬,lParamЯ��״̬��
		break;
	case SMS_PHONE_BUTTON: ////�绰����,lParamЯ������ֵ
		switch ( g_pL23MsgBuffer[1] ) //////�ж��û�����
		{
		
		}
		
		break;
	case SMS_USER_SHUT://///�û���ֹ
		/////////////֪ͨ�û������ҽ���ϵͳ����Ȩ
		///////֪ͨ��·��һ�
		NU_Control_Timer(&TS, NU_DISABLE_TIMER);
		g_pL23MsgBuffer[2] = 0x00;
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

		break;
	}
}

///////////////Create��Ϣ
BOOL CSMSMessage::OnCreate ( )
{
	if ( CFrameWnd::OnCreate ( ) == FALSE )
		return FALSE;

	////////////////��ʼ��


	return TRUE;
}


////////������Ϣ��
void RunSMSMessage ( )
{
	

	////������ʱ��
	NU_Create_Timer(&SYJTS, "syjTS", SYJTimerOver, 8, 1000, 200, NU_DISABLE_TIMER);//Init 			
	
	//////������ָ���������
	///// TODO: Your codes
	/////����1
		g_SystemOption.SMSStatus[0].bRevNumber = 0;
		g_SystemOption.SMSStatus[0].bSendNumber = 0;
		g_SystemOption.SMSStatus[0].bSendedNumber = 0;
		g_SystemOption.SMSStatus[0].bHavePasswordItem = 0;

		////����2
		g_SystemOption.SMSStatus[1].bSendNumber = 0;
		g_SystemOption.SMSStatus[1].bRevNumber = 0;
		g_SystemOption.SMSStatus[1].bSendedNumber = 0;
		g_SystemOption.SMSStatus[1].bHavePasswordItem = 0;

		////����3
		g_SystemOption.SMSStatus[2].bSendNumber = 0;
		g_SystemOption.SMSStatus[2].bRevNumber = 0;
		g_SystemOption.SMSStatus[2].bSendedNumber = 0;
		g_SystemOption.SMSStatus[2].bHavePasswordItem = 0;

		////��������
		g_SystemOption.SMSStatus[3].bSendNumber = 0;
		g_SystemOption.SMSStatus[3].bRevNumber = 0;
		g_SystemOption.SMSStatus[3].bSendedNumber = 0;
		g_SystemOption.SMSStatus[3].bHavePasswordItem = 0;

	/////��Flash�е����ݵ���Memory
	int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////���Ҽ�¼
	BOOL bIsShutOffLED = TRUE;
	if ( r != 0 ){//////Finded data in flash
		
		
		for ( int i = 0; i < r;i ++ )
		{

			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///�õ���¼ָ��

			if ( lpST->iLetterStatus != ICON_LETTERREADED &&
				 lpST->iChildMailBox == 0 &&
				 lpST->bReadStatus != 1 )
				bIsShutOffLED = FALSE;
			if ( lpST->bIsFlash == 1 )//// Sened letter 
			{
				g_SendedSMS[lpST->bMailBox][g_SystemOption.SMSStatus[lpST->bMailBox].bSendedNumber] = *lpST;
				g_SystemOption.SMSStatus[lpST->bMailBox].bSendedNumber ++;
				if ( lpST->iLockStatus )
					g_SystemOption.SMSStatus[lpST->bMailBox].bHavePasswordItem ++;
			}else {
			
				if ( lpST->iChildMailBox == 0 )
					g_SystemOption.SMSStatus[lpST->bMailBox].bRevNumber ++;
				else if ( lpST->iChildMailBox == 1 )
					g_SystemOption.SMSStatus[lpST->bMailBox].bSendNumber ++;
			}
		}
	}

	if ( bIsShutOffLED ) ///�����δ�����Ž�LED����
		ClearLED ( LED_SMS );
	else SetLED ( LED_SMS );

	///////��ʼ����Ϣ��
	if ( g_pShowMsg == NULL )
		g_pShowMsg = new CShowMessage;
	g_pShowMsg->Create ( "��Ϣ",
		0,CRect ( 14,60,142,100 ),0 );

	///// ������Ϣ��
	if(!m_pSMSMessage)
	{
		m_pSMSMessage = new CSMSMessage;
		m_pSMSMessage->Create("SMSMsg", 
			0, 
			CRect(0 ,  0, 1, 2) , 0);
	}
}


void SendMessageToSMSMsgLayer ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	SYSTEM_MESSAGE message;
	message.handle = (UINT32)m_pSMSMessage;
	message.message = uMessage;
	message.wparam = wParam;
    message.lparam = lParam;

	PostMessageToQueue ( &message );
	//m_pSMSMessage->SendMessage ( uMessage,wParam,lParam );

}


////////////////////////////////////////////////////////////////
//                   Ӧ�ò�   syj
///////////////////////////////////////////////////////////////
void InitSMSApp ( )
{
	///////////��ʼ������Ϣ
	/////����1
	g_SystemOption.SMSStatus[0].iLockType = 0; ///����״̬
	strcpy ( g_SystemOption.SMSStatus[0].szPassword,"" ); //����
	strcpy ( g_SystemOption.SMSStatus[0].szName,"˽��1" ); //����
	g_SystemOption.SMSStatus[0].bSendedNumber = 0; ///��������Ŀ
	g_SystemOption.SMSStatus[0].bHavePasswordItem = 0; ///�ѷ������м�������Ŀ
	g_SystemOption.SMSStatus[0].iRingID = 0;///Default ring
	g_SystemOption.SMSStatus[0].bIsCanDelete = FALSE;
	////����2
	g_SystemOption.SMSStatus[1].iLockType = 0;
	g_SystemOption.SMSStatus[1].bIsCanDelete = FALSE;
	strcpy ( g_SystemOption.SMSStatus[1].szName,"˽��2" );
	strcpy ( g_SystemOption.SMSStatus[1].szPassword,"" ); //����
	g_SystemOption.SMSStatus[1].bSendedNumber = 0;
	g_SystemOption.SMSStatus[1].bHavePasswordItem = 0;
	g_SystemOption.SMSStatus[1].iRingID = 0;///Default ring
	////����3
	g_SystemOption.SMSStatus[2].iLockType = 0;
	strcpy ( g_SystemOption.SMSStatus[2].szName,"˽��3" );
	strcpy ( g_SystemOption.SMSStatus[2].szPassword,"" ); //����
	g_SystemOption.SMSStatus[2].bSendedNumber = 0;
	g_SystemOption.SMSStatus[2].bHavePasswordItem = 0;
	g_SystemOption.SMSStatus[2].iRingID = 0;///Default ring
	g_SystemOption.SMSStatus[2].bIsCanDelete = FALSE;
	////��������
	g_SystemOption.SMSStatus[3].iLockType = 0;
	strcpy ( g_SystemOption.SMSStatus[3].szName,"����" );
	strcpy ( g_SystemOption.SMSStatus[3].szPassword,"" ); //����
	g_SystemOption.SMSStatus[3].bSendedNumber = 0;
	g_SystemOption.SMSStatus[3].bHavePasswordItem = 0;
	g_SystemOption.SMSStatus[3].iRingID = 0;///Default ring
	g_SystemOption.SMSStatus[3].bIsCanDelete = FALSE;
	
	/////��ʼ�����͡��رյ������Լ�������
	g_SystemOption.bIsNeedSound = FALSE;
	g_SystemOption.bIsNeedReturn = FALSE;
	CLR_WATCHDOG();
	char *enable[] = {
		"*52*",
		"*54*",
		"*55*",
		"*56#",
		"*57*",
		"*58#",
		"*59#",
		"*40*"
	
	};

	char *disable[] = {
		"#52#",
		"#54*",
		"#55#",
		"#56#",
		"#57#",
		"#58#",
		"#59#",
		"#40#"
	};

	
	for ( int j = 0;j < 8;j ++ )
	{
		strcpy ( g_SystemOption.szEnableCode[j],enable[j] );
		strcpy ( g_SystemOption.szDisableCode[j],disable[j] );
	}

	//////////��ʼ��������
	char *str[] = {
		"����",
		"ף��",
		"����",
		"��л",
		"��Ǹ",
		"��Ĭ"
	};

	////����
	char *str0[] = {  
		"�����쳤������죡�������ճ������������տ��֣�",	
		"�������յ���һ�죬��û�и�����һ��ֻϣ�����ܿ��֡�������������������Ҫ�ܶ�������Ͱ��գ����տ��֣�",
		"ף�����տ��֣�Ը������������Զ������飡",
		"֪���𣿽����������ա�ϣ���������ÿ��ġ���Զ�Ϳ������飡",
		"�ر�İ�,���ر����,�ر������,�����ر��ף��,ף---����ÿһ��!"

	};
	/////ף��
	char *str1[] = {
		"ף����λ��Ȩ�������ᣬǮ��������ҽ���ÿ��˯��������������쵽�ֳ����»�Ǯ�����񣬱��˼Ӱ����н��",
		"��ϲ�㣡�����»���죬�Ҹ��������������ڣ���ͷ���ϣ�",
		"���������㣻���˹����㣻���������㣻����ϵ���㣻����֮����Զ�����㡣",
		"��̫���Լ�,����Լ�ѹ������,�����Լ�,�����Լ��Ż�ӵ��������һ��.",
		"�����ڣ��ξ��ڣ����֮�仹���氮�����ɰܣ�����������ֻ�����Ǵ�ͷ������Ը�����߳������ĵ͹ȣ�"
	};
	
	/////����
	char *str2[] = {
		"���Լ����εģ�ȥ�Լ���ȥ�ģ����Լ������ģ���Ϊ����ֻ��һ�Σ����᲻������!",
		"����æ��,���ǧ��Ҫע����Ϣ,�������ۻ��˿ɲ�������..",
		"�����ĳɹ�,�������õ�һ������,�������������ƴ��.",
		"������;,��������������Ϳ���,���������,һ�����������Ĳʺ�.",
		"������㣬�Ͳ�Ҫ������գ�������񣬾Ͳ�Ҫ��������"
	};
	
	/////��л
	char *str3[] = {
		"��һֱ��лл�㣬лл��������ҵ��������һֱ������㣬����������ĺܰ��㡣",
		"����������л�㣬�����������ʱ����ԭ���ջ�һ�ƴ��磬��ȴ��������������",
		"Ҫ��Ҫ��л��Ҫл���˵һ�����㲻˵����ô֪����~ֻ����Ҫл��ʱ����л���㲻Ҫл��ʱ���ҾͲ�л����",
		"���ͱ������氮������ֻΪ�ٻ��꣡",
		"лл��һ·�����߹��������,����ֿ������."
	};
	
	/////��Ǹ
	char *str4[] = {
		"����ʵ�ܹԡ�",
		"��֪�����������������ÿ�������Ҷ��ú��¡�����ң���ô��ԭ���ң���ô��",
		"����������ˣ�������Ҳ�����ܣ������������˴˴�ϣ������ԭ�£��������ԭ�£����Լ�Ҳ���޷�ԭ���Լ��ˣ�",
		"��ֻ��˵�����Ҵ��ˡ���",
		"�㲻Ҫ�����ң�����ĺ���������ʵ��֪������Ϊ����ں��ң�����Ҳֻ�ܸ����㣺�Ұ��㣡"
	};

	////��Ĭ
	char *str5[] = {
		"���迴����ķ��ѧһ�ؼҾ�������Ӿ��������.����:\"�����ʹ�ź�,�����ȥ��Ӿ��!\"��ķ:\"û�й�ϵ,�ҿ�����������.\"",
		"ĸ��:\"��������ɾ�,������ô������?\"����:\"�Ҹղ����ֲ�������.\"",
		"����Ա:\"�뱨����ĸ߶ȡ�λ��.\" ����Ա:\"�Ҵ�Լ1��8�׸�,���������ڼ�ʻԱ����λ��.\"",
		"���ζ�������˵:\"���Ǵ�ҵĳ�Ʊ����ȥ������,�����˻���ȥ�����Ʒ��.\" \"����?���ǵ�˾�����������Ĵ���?\"",
		"\"�ְ֣�������ʦ����˵,���丸��������,��仰��ʲô��˼��?\",\"��һ���ָ���ʲô��������!\"�ְ�˵."
	};

	int r;
	for ( int i = 0;i < 6;i ++ )
	{
		MODEL model;
		memset ( &model,0,sizeof ( model ) );
		strcpy ( model.szModelName,str[i] );
		r = AppendMODEL ( &model );
		model.iDataID = r;
		ModifyModel ( r,&model );

		for ( int j = 0;j < 5;j ++ )
		{
			CLR_WATCHDOG();
			CONTENT content;
			memset ( &content,0,sizeof ( content ) );
			
			content.iModelID = r;
			if ( i == 0 )
				strcpy ( content.szContent,str0[j] );
			else if ( i == 1 )
				strcpy ( content.szContent,str1[j] );
			else if ( i == 2 )
				strcpy ( content.szContent,str2[j] );
			else if ( i == 3 )
				strcpy ( content.szContent,str3[j] );
			else if ( i == 4 )
				strcpy ( content.szContent,str4[j] );
			else if ( i == 5 )
				strcpy ( content.szContent,str5[j] );


			int r1 = AppendContent ( &content );
			content.iDataID = r1;
			ModifyContent ( r1,&content );

		}
	}

	//////////////// SMS ///////////////////////

}

//////////////////��ҵ����
///class CNewPhone
BEGIN_MESSAGE_MAP(CNewPhone, CAppWnd) 
	ON_MESSAGE ( NEWPHONE_LISTVIEW,CN_COMMAND,OnSelect )
END_MESSAGE_MAP

BOOL CNewPhone::OnCreate ( )
{
	
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;
	//////TODO: your codes
	CRect rc ( m_sRect.left,
			   m_sRect.top + WND_TITLE_HEIGHT,
			   m_sRect.right,
			   m_sRect.bottom );
	
	
	m_ListView.Create ( rc,this,LS_SMALLICON,NEWPHONE_LISTVIEW );
	m_ListView.SetStatus ( LS_SMALLICON ,TRUE );
	
	/////��ʼ���б�
	m_iMenuType = 0;
	char *strmenu[] = {
			"���߷���",
			"��������",
			"���ӷ���",
			"����ŷ���",
			"ת�ƺ���",
			"���еȴ�",
			"��æ�ؽ�",
			"��æת��"
			
		};

	

		for ( int i = 0;i < 8;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );


	return TRUE;
}

BOOL CNewPhone::OnDestroy ( )
{

	return CAppWnd::OnDestroy ( );
}

void CNewPhone::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );

	///// codes
}

void CNewPhone::UpdateMenu (  )
{
	int i;
	m_ListView.DeleteAllItems ( );

	if ( m_iMenuType == 0 ){ ////0���˵�
	
		m_nHelpId = HELP_SMS_NEW;
		SetWindowText ( "�绰��ҵ��" );
		PaintTitle ( CDC ( ) );

		char *strmenu[] = {
			"���߷���",
			"��������",
			"���ӷ���",
			"����ŷ���",
			"ת�ƺ���",
			"���еȴ�",
			"��æ�ؽ�",
			"��æת��"
			
		};

	

		for ( i = 0;i < 8;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );
	
	}else if ( m_iMenuType == 1 ) { ///�������Ƶ����˵�
		
		char *strmenu[] = {
			"��ȫ������",
			"�޹��ڹ��ʳ�;",
			"�޹��ʳ�;",
			"����"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

		
	}else if ( m_iMenuType == 2 ) { ///�������Ƶ��Ӳ˵�
	
		char *strmenu[] = {
			"�Ǽ�",
			"ע��",
			"���Ĳ�����",
			"����"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

	
	}else if ( m_iMenuType == 3 ) { ////����������Ӳ˵�
	
		
		char *strmenu[] = {
			"�Ǽ�",
			"ע��",
			"���Ĳ�����",
			"����"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

	}else if ( m_iMenuType == 4 ) { /////���Ĳ�����
	
		char *strmenu[] = {
			"���ĵǼǲ�����",
			"����ע��������",
			"�ָ�ȱʡ������",
			"����"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

	}

	m_ListView.OnPaint ( CDC ( ) );
}

void CNewPhone::OnSelect ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	extern void DialOut(char *strDial);	// ����
	extern void OpenPhoneMic();			// ���Ὺ

	
	UINT32 lItem = lParam;/////ѡ����
	char szTemp[16],szPhone[32];

	strcpy ( szTemp,"" );
	strcpy ( szPhone,"" );
	
	if ( m_iMenuType == 0 ) { ///��ǰΪ0���˵�
			
		m_iSelectedItem = lItem; ////��¼��ǰ��ѡ��
		///////////////���°���
		UINT32 help[] = {
			
			HELP_SMS_NEW_HOTLINE,
			HELP_SMS_NEW_LIMIT,
			HELP_SMS_NEW_ALARM,
			HELP_SMS_NEW_REFUSE,
			HELP_SMS_NEW_TRANSFER,
			HELP_SMS_NEW_WAIT,
			HELP_SMS_NEW_BUSY,
			HELP_SMS_NEW_BUSY2TRANSFER
		
		};

		m_nHelpId = help[lItem];

		switch ( lItem )
		{
		
		case 1:////�����������˵�
			m_iMenuType = 1;
			break;
		default: ////�����˵�
			m_iMenuType = 3;
			
		}

		UpdateMenu ( );

		//////////���Ĵ��ڵı���
		char *strmenu[] = {
			"���߷���",
			"��������",
			"���ӷ���",
			"����ŷ���",
			"ת�ƺ���",
			"���еȴ�",
			"��æ�ؽ�",
			"��æת��"
			
		};

		SetWindowText ( strmenu[lItem] );
		PaintTitle ( CDC ( ) );
		
	}else if ( m_iMenuType == 1 ) { ////��ǰΪ�������Ƶ����˵�
	

		switch ( lItem )
		{
		case 0://///����ȫ������
			m_nHelpId = HELP_SMS_NEW_LIMIT_ALL;
			m_iMenuType = 2;
			m_iLimite = 1;
			break;
		case 1://///���ƹ��ڹ���
			m_nHelpId = HELP_SMS_NEW_LIMIT_2;
			m_iMenuType = 2;
			m_iLimite = 2;
			break;
		case 2:////���ƹ��ʳ�;
			m_nHelpId= HELP_SMS_NEW_LIMIT_1;
			m_iMenuType = 2;
			m_iLimite = 3;
			break;
		case 3:   ////����0���˵�
			m_iMenuType = 0;
			
		}

		UpdateMenu ( );

	}else if ( m_iMenuType == 2 ) {  ////��ǰΪ�������Ƶ��Ӳ˵�
	
		switch ( lItem )
		{
		case 0:////����
			
			strcpy ( szPhone,g_SystemOption.szEnableCode[m_iSelectedItem] );
			
			
			if ( InputPassword ( szTemp,
					4,0 ) == IDOK )
			{
			      if(IsLegalWnd())
			      {

				strcat ( szPhone,szTemp );

				if ( m_iLimite == 1 ) 
					strcat ( szPhone,"*1" );
				else if ( m_iLimite == 2 )
					strcat ( szPhone,"*2" );
				else if ( m_iLimite == 3 )
					strcat ( szPhone,"*3" );

				strcat ( szPhone,"#" );

				///Call
				SOFTDIAL	todial;
				todial.areacode[0] = '\0';
				todial.ip = -1;
				strcpy(todial.name, "��ҵ��");
				strcpy(todial.number, szPhone);
				todial.type = CARD_NONE;
				AppDialOut(todial);
				  
				}

			}

			return;
		case 1:////�ر�
			strcpy ( szPhone,g_SystemOption.szDisableCode[m_iSelectedItem] );
			
			if ( InputPassword ( szTemp,
					4,0 ) == IDOK )
			{
			   if(IsLegalWnd())
			   {

				strcat ( szPhone,szTemp );
				strcat ( szPhone,"#" );

				///Call
				SOFTDIAL	todial;
				todial.areacode[0] = '\0';
				todial.ip = -1;
				strcpy(todial.name, "��ҵ��");
				strcpy(todial.number, szPhone);
				todial.type = CARD_NONE;
				AppDialOut(todial);

				}	
			}

			return;
		case 2:////����
			m_nHelpId = HELP_SMS_NEW_CODE;
			m_iMenuType = 4;
			break;
		case 3: //////���غ����������˵�
			m_iMenuType = 1;
			
		}

		UpdateMenu ( );
	
	}else if ( m_iMenuType == 3 ) { ////��ǰΪ�����Ӳ˵�
	
		switch ( lItem )
		{
		case 0:///������Ӧ�ķ���
			strcpy ( szPhone,g_SystemOption.szEnableCode[m_iSelectedItem] );
			//////////////����Ҫ�������
			if ( m_iSelectedItem == 3 || ///�����
				 m_iSelectedItem == 5 || ///���еȴ�
				 m_iSelectedItem == 6) { ///��æ�ؽ�
			
				///Call
				SOFTDIAL	todial;
				todial.areacode[0] = '\0';
				todial.ip = -1;
				strcpy(todial.name, "��ҵ��");
				strcpy(todial.number, szPhone);
				todial.type = CARD_NONE;
				AppDialOut(todial);


				return;
			}

			/////////////��Ҫ�������
			if ( m_iSelectedItem == 0 || ///���߷���
				 m_iSelectedItem == 4 || ///����ת��
				 m_iSelectedItem == 7 ) { ///��æת��
					
				if ( NumInputBox ( "����绰����",szTemp,16 ) == IDOK )
				{
					strcat ( szPhone,szTemp );
					strcat ( szPhone,"#" );

					///Call
					SOFTDIAL	todial;
					todial.areacode[0] = '\0';
					todial.ip = -1;
					strcpy(todial.name, "��ҵ��");
					strcpy(todial.number, szPhone);
					todial.type = CARD_NONE;
					AppDialOut(todial);


				}

				return;
			}
			
			/////���������ʽ
			if ( m_iSelectedItem == 2 ) {////���ӷ���
				
				if ( NumInputBox ( "����ʱ��(HHMM)",szTemp,5 ) == IDOK )
				{
					for ( int i = 0;i < strlen ( szTemp );i ++ )
						if ( szTemp[i] < '0' ||
							 szTemp[i] > '9' )
						{
							MessageBox ( "���������֣�",
								"����",MB_OK );

							return ;
						
						}
					
					
					strcat ( szPhone,szTemp );
					strcat ( szPhone,"#" );

					///Call
					SOFTDIAL	todial;
					todial.areacode[0] = '\0';
					todial.ip = -1;
					strcpy(todial.name, "��ҵ��");
					strcpy(todial.number, szPhone);
					todial.type = CARD_NONE;
					AppDialOut(todial);


				}

			}
			
			return;
		case 1:///�ر���Ӧ�ķ���
			strcpy ( szPhone,g_SystemOption.szDisableCode[m_iSelectedItem] );
			///Call
			SOFTDIAL	todial;
			todial.areacode[0] = '\0';
			todial.ip = -1;
			strcpy(todial.name, "��ҵ��");
			strcpy(todial.number, szPhone);
			todial.type = CARD_NONE;
			AppDialOut(todial);
			return;
		case 2:///���Ĳ�����
			m_nHelpId = HELP_SMS_NEW_CODE;
			m_iMenuType = 4;
			break;
		case 3:////����0���˵�
			m_iMenuType = 0;
			
		
		}

		UpdateMenu ( );
	
	
	}else if ( m_iMenuType == 4 ) { /////��ǰΪ���Ĳ�����˵�
	
		if ( lItem == 0 ){////������Ӧ�ĵǼ���
			
			if ( NumInputBox ( "����Ǽǲ�����",
				 g_SystemOption.szEnableCode[m_iSelectedItem],5 ) == IDOK )
				/////����ϵͳ����
				SaveSystemOption ( &g_SystemOption );
			
			
			return;

		} else if ( lItem == 1 ) {/////������Ӧ��ע����
			
			if ( NumInputBox ( "����ע��������",
				 g_SystemOption.szDisableCode[m_iSelectedItem],5 ) == IDOK )
				/////����ϵͳ����
				SaveSystemOption ( &g_SystemOption );
			
			return;

		} else if ( lItem == 2 ) { /////�ָ�ȱʡ������
			
			char *enable[] = {
				"*52*",
				"*54*",
				"*55*",
				"*56#",
				"*57*",
				"*58#",
				"*59#",
				"*40*"
			
			};

			char *disable[] = {
				"#52#",
				"#54*",
				"#55#",
				"#56#",
				"#57#",
				"#58#",
				"#59#",
				"#40#"
			};

			strcpy ( g_SystemOption.szEnableCode[m_iSelectedItem],enable[m_iSelectedItem] );
			strcpy ( g_SystemOption.szDisableCode[m_iSelectedItem],disable[m_iSelectedItem] );

			/////����ϵͳ����
			SaveSystemOption ( &g_SystemOption );

			MessageBox ( "�ָ��ɹ���","��ʾ",MB_OK );

			return;
		
		}else if ( lItem == 3 ) {////����0���˵�
		
			if ( m_iSelectedItem != 1 )
				m_iMenuType = 3;
			else m_iMenuType = 2;
		}
		
		UpdateMenu ( );
	
	}
	
}

///////////////////�˵�������
/////class CMoneyWnd
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CMoneyWnd, CAppWnd) 
	ON_MESSAGE ( MONEY_GRID,CN_GRIDSEL,OnGridCmd ) ////����ؼ���Ϣ
	ON_MESSAGE ( MONEY_DELETEALL,CN_COMMAND,OnDeleteAll ) /////ɾ����¼
END_MESSAGE_MAP

CMoneyWnd::CMoneyWnd ( )
{
//	m_pGrid = NULL;
}

CMoneyWnd::~CMoneyWnd ( )
{

}

///////// create message
BOOL CMoneyWnd::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) )   return FALSE;

	/////// TODO:
//	if ( !m_pGrid )
//		m_pGrid = new CGrid;

	CRect rc ( m_sRect.left + 1,
			   m_sRect.top + WND_TITLE_HEIGHT,
			   144,159 );
	m_pGrid.Create ( this,rc,
		TBS_ROWSELECT|UNS_TEXT|WS_CHILD|TBS_SPACELINE,
		0,2,MONEY_GRID );

	
	UINT16 iWidthArray[] = {18,122};
	m_pGrid.SetColsWidth ( iWidthArray ,TRUE );////�����п�


	////����ɾ����ť
	rc.left = m_sRect.left +  109;
	rc.right = rc.left + 32;
	rc.top = 0;
	rc.bottom = SMS_APP_TITLE_HEIGHT;
	m_DeleteBtn.Create ( "ȫɾ",rc,this,MONEY_DELETEALL );
	return TRUE;
}
////ɾ����Ϣ
void CMoneyWnd::OnDeleteAll ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( MessageBox ( "ȫ��ɾ����","����",
		      MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
	
		int i;
		if ( m_iMoneyStyle == 0 ) ////�򵥻����˵�
		{
		
			for ( i = 0;i < SMS_MONEY_MAX_NUMBER;i ++ )
				g_Money[i].bIsData = 0;

		}else if ( m_iMoneyStyle == 1 ){ ////��ϸ�˵�
		
			for ( i = 0;i < SMS_MONEYEX_MAX_NUMBER;i ++ )
				g_MoneyEx[i].bIsData = 0;
		
		}
	}

}
////����ؼ���Ϣ
void CMoneyWnd::OnGridCmd ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	UINT16 iItem = (int)lParam&0x0000ffff;
	///////////ȡ�ö�������
	int pos = m_pGrid.GetTotalRows ( ) - iItem;
	
	MONEY money;

	if ( m_iMoneyStyle == 0 ){
	
		money = g_Money[pos];
		g_Money[pos].iconID = ICON_LETTERREADED;
	
	}else if ( m_iMoneyStyle == 1 ){
		
		money = g_MoneyEx[pos];
		g_MoneyEx[pos].iconID = ICON_LETTERREADED;
	}
	
	///////��ʾ������Ϣ
	if ( !m_pShowMsg )
			m_pShowMsg = new CShowMessageEx;
		
		
	m_pShowMsg->SetTimerFlag ( 0 );
	m_pShowMsg->SetString ( money.szContent ); 
	m_pShowMsg->SetDestroyFlag ( 2 );
	m_pShowMsg->Create ( "������Ϣ",
		WS_VISIBLE | WS_CAPTION | WS_CHILD,
		CRect ( 0,0,159,159 ),0 );

	
	m_pShowMsg->ShowWindow ( );

	

}
///////// Destroy message
BOOL CMoneyWnd::OnDestroy ( )
{
	/////// TODO:

	return CAppWnd::OnDestroy ( );
}

////////  Paint message
void CMoneyWnd::OnPaint ( CDC &dc )
{
	

	/////////��������ؼ�
	m_pGrid.RemoveAll ( );
	int i;
	char *szItem[2];
	char pStyle[] = { 0 ,0};
    UINT32 pIconID[] = { 0 ,0};

	szItem[0] = new char[2];
	strcpy ( szItem[0],"" );
	szItem[1] = new char[21];
	
	if ( m_iMoneyStyle == 0 ){  /////��ͨ����
	
		for ( i = 0;i < SMS_MONEY_MAX_NUMBER;i ++ )
		{
			if ( g_Money[i].bIsData )
			{
				pIconID[0] = g_Money[i].iconID;
				pIconID[1] = 0;

				strcpy ( szItem[1],g_Money[i].szTitle );
				m_pGrid.InsertRow (0, szItem,pStyle,pIconID );
			}
		}
		
	
	}else if ( m_iMoneyStyle == 1 ) { ////��ϸ����
	
		for ( i = 0;i < SMS_MONEYEX_MAX_NUMBER;i ++ )
		{
			if ( g_MoneyEx[i].bIsData )
			{
				pIconID[0] = g_MoneyEx[i].iconID;
				pIconID[1] = 0;

				strcpy ( szItem[1],g_MoneyEx[i].szTitle );
				m_pGrid.InsertRow (0, szItem,pStyle,pIconID );
			}
		}
	}


	delete []szItem[0];
	delete []szItem[1];

	CAppWnd::OnPaint ( dc );
}

///// class CReadSubscribeSMS
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CReadSubscribeSMS, CAppWnd) 
	ON_MESSAGE ( READINGSUBSCRIBE_SHOWMENU,CN_COMMAND,OnShowMenu ) ////��ʾ�˵�
	ON_MESSAGE ( READINGSUBSCRIBE_DEL,CN_COMMAND,OnDel ) ////ɾ��
	ON_MESSAGE ( READINGSUBSCRIBE_SAVEAS,CN_COMMAND,OnSaveAs ) //���Ϊ
	ON_MESSAGE ( READINGSUBSCRIBE_SENDTO,CN_COMMAND,OnSendTo ) //����
	ON_MESSAGE ( READINGSUBSCRIBE_NAME,CN_COMMAND,OnShowTitle ) 
END_MESSAGE_MAP

CReadSubscribeSMS::CReadSubscribeSMS ( )
{
	m_pShowTitle = NULL;
}

CReadSubscribeSMS::~CReadSubscribeSMS ( )
{
}

void CReadSubscribeSMS::OnSendTo ( UINT32 uMessage,UINT32 wParam,UINT32 lParam ) 
{

	
	 //////���ô��ڵ�һЩ����
	////////�õ���ǰ�Ķ���Ϣ����
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	
	strcpy ( g_ST.szPhone,"�����������" );
	if ( strlen ( m_pst.szContent ) > 140 )
	{

		if ( ::MessageBox ( "ֻ�ܴ���ǰ70�����֣��Ƿ������",
			"��ʾ",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			char szTemp[SMS_CONTENT_MAX_NUMBER];
			memcpy ( szTemp,m_pst.szContent,140 );
			szTemp[140] = '\0';
			strcpy ( g_ST.szMsg,szTemp );
					
		}else return;
	
	}else strcpy ( g_ST.szMsg, m_pst.szContent );

	if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

	g_bIsModel = FALSE;
	m_pModifyLetter->SetFlag ( false ); 
	m_pModifyLetter->Create("д����Ϣ", 
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
		CRect(0 ,  0, 159 , 159) , 0);
			

	m_pModifyLetter->ShowWindow ( );

}
/////ɾ��
void CReadSubscribeSMS::OnDel ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	//////////////ɾ����¼
	if ( ::MessageBox ( "ȷ��ɾ���ü�¼��","ɾ��",
		MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
		
		memmove ( &g_SubscibeSMS[m_CurrentPos],
			&g_SubscibeSMS[m_CurrentPos+1],
			(g_iSubscribePos-m_CurrentPos-1)*sizeof ( SUBSCIBESTRUCT ) );
		g_iSubscribePos --;
		Destroy ( );
	}
	
}
/////���Ϊ
void CReadSubscribeSMS::OnSaveAs ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	
	////////�õ���ǰ�Ķ���Ϣ����
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	
	strcpy ( g_ST.szPhone,"����e��Ϣ" );
	if ( strlen ( m_pst.szContent ) > 140 )
	{

		if ( ::MessageBox ( "ֻ�ܴ���ǰ70�����֣��Ƿ������",
			"��ʾ",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			char szTemp[SMS_CONTENT_MAX_NUMBER];
			memcpy ( szTemp,m_pst.szContent,140 );
			szTemp[140] = '\0';
			strcpy ( g_ST.szMsg,szTemp );
					
		}else return;
	
	}else strcpy ( g_ST.szMsg, m_pst.szContent );

	
	////// ���ʱ��
	if ( !strlen ( g_ST.szTime ) ) { //////���ʱ���ǿգ���䵱ǰʱ��
									////��������ǰʱ��Ϊ׼
		
		FillTime ( g_ST.szTime );
		/*
		DateTime	date;
		GetSysTime ( &date );
		sprintf ( g_ST.szTime,"%d-%d-%d %d:%d",
			  date.year,
			  date.month,
			  date.day,
			  date.hour,
			  date.minute );*/
	
	}

	g_ST.Flag = 0;///ֻ��
	g_ST.iChildMailBox = 0;/////���浽������
	g_ST.iLetterStatus = ICON_LETTERUNREAD;////δ��
	g_ST.bReadStatus = 0;

	////// ��ʾ���Ϊ���ô���
	if ( m_pSaveAsWnd == NULL )
		m_pSaveAsWnd = new CSaveAsWnd;
	m_pSaveAsWnd->Create ( "ѡ��Ŀ������",
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_OKBOX,
		CRect ( 0,0,159,159 ) );

	CloseCursor ( );///Close cursor syj
	m_pSaveAsWnd->ShowWindow ( );

	
}
//////��ʾ����
void CReadSubscribeSMS::OnShowTitle ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	if ( !m_pShowTitle )
		m_pShowTitle = new CDisplayInformation;

	m_pShowTitle->SetStyle ( 1 );//������ʾ
	m_pShowTitle->SetPhone ( m_pst.szTitle );

	/////���㴰�ڸ߶�
	int len = strlen ( m_pst.szTitle );
	int iNum = len/16;
	if ( 16*iNum < len )
		iNum ++;

	int iHeight = 16*iNum + 8;
	m_pShowTitle->Create ( "����",
			WS_CHILD | WS_VISIBLE,
			CRect ( 10,30,146,iHeight+30 ) ,0 );

	m_pShowTitle->ShowWindow ( );

} 
/////��ʾ�˵�
void CReadSubscribeSMS::OnShowMenu ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	////////���˵���ʾ����
	m_Menu.PopupMenu ( m_sRect.left + 1,
					m_sRect.top + WND_TITLE_HEIGHT  );
}
////////��ʼ����Ϣ
BOOL CReadSubscribeSMS::OnCreate ( )
{

	if ( !CAppWnd::OnCreate ( ) )
		return FALSE;
	int i;
	///// Codes
	/////////////�����˵����ť
	CRect rc( m_sRect.left,
			m_sRect.top ,
			m_sRect.left + 76,
			m_sRect.top+ SMS_APP_TITLE_HEIGHT  );

	m_ShowMenuBtn.Create ( "����Ϣ",rc,
								this,READINGSUBSCRIBE_SHOWMENU );

	///////����ת����ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	
	m_AnswerBtn.Create ( "ת��",rc,this,READINGSUBSCRIBE_SENDTO );
	///////����ɾ����ť 
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;

	m_DelBtn.Create ( "ɾ��",rc,this,READINGSUBSCRIBE_DEL );

	/////////////��������ؼ�
	rc.left = m_sRect.right - 17;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 2;
	rc.right = m_sRect.right;
	rc.bottom = rc.top + 16;
	m_Name.Create ( "��",rc,this,
		READINGSUBSCRIBE_NAME ); 
	///////��������Ϣ������
	rc.left = m_sRect.left + 1;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 21;
	rc.right = m_sRect.right;
	rc.bottom = m_sRect.bottom;
	m_Content.Create ( m_pst.szContent,rc,this,WS_CHILD|WS_VISIBLE,
		READINGSUBSCRIBE_CONTENT,201 );

	////////////////�����˵�
	m_Menu.Create ( this );
	m_Menu.AppendItem ( "���Ϊ",READINGSUBSCRIBE_SAVEAS );

	return TRUE;
}
////////������Ϣ
BOOL CReadSubscribeSMS::OnDestroy ( )
{
	if ( m_pShowTitle )
	{
		delete m_pShowTitle;
		m_pShowTitle = NULL;
	}

	return CAppWnd::OnDestroy ( );
}

/////�ػ���Ϣ
void CReadSubscribeSMS::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );

	//// Your codes
	dc.SetBackColor ( );
	dc.TextOut ( m_sRect.left,
		m_sRect.top + WND_TITLE_HEIGHT + 2,"�� ��:" );

	/////���ñ���
	char szTemp[3];
	int x = m_sRect.left + 54;
	if ( strlen ( m_pst.szTitle ) > 10 )
	{
		for ( int i = 0;i < 10;i ++ )
		{
			if ( m_pst.szTitle[i] & 0x80 )
			{
				if ( x+16 > m_sRect.right - 17 ) break;
				strcpy ( szTemp,"" );
				szTemp[0] = m_pst.szTitle[i];
				szTemp[1] = m_pst.szTitle[i+1];
				szTemp[2] = '\0';

				dc.TextOut ( x,m_sRect.top + WND_TITLE_HEIGHT + 2,
					szTemp );

				x += 16;
				i ++;
				
			}else {
			
				if ( x+8 > m_sRect.right - 18 ) break;
				szTemp[0] = m_pst.szTitle[i];
				szTemp[1] = '\0';

				dc.TextOut ( x,m_sRect.top + WND_TITLE_HEIGHT + 2,
					szTemp );

				x += 8;
			}
		}
	
	}else dc.TextOut ( m_sRect.left + 54,
		m_sRect.top + WND_TITLE_HEIGHT + 2,m_pst.szTitle );

}
////////////////e��Ϣ������
BEGIN_MESSAGE_MAP(CSubscribeSMS, CAppWnd) 
	ON_MESSAGE ( SUBSCRIBE_GRID,CN_GRIDSEL,OnGridCmd ) ////����ؼ���Ϣ
	ON_MESSAGE ( SUBSCRIBE_DELETEALL,CN_COMMAND,OnDeleteAll ) ///ȫ��ɾ��
END_MESSAGE_MAP

/////////Init
BOOL CSubscribeSMS::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;

	/////TODO: Add your codes

	CRect rc ( m_sRect.left + 1,
			   m_sRect.top + WND_TITLE_HEIGHT,
			   144,159 );
	m_pGrid.Create ( this,rc,
		TBS_ROWSELECT|UNS_TEXT|WS_CHILD|TBS_SPACELINE,
		0,3,SUBSCRIBE_GRID );

	
	UINT16 iWidthArray[] = {18,80,40};

	m_pGrid.SetColsWidth ( iWidthArray ,TRUE );////�����п�


	//////����ȫɾ��ť
	m_DeleteAll.Create ( "ȫɾ",
		                 CRect ( m_sRect.left+110,
								 m_sRect.top,
								 m_sRect.left + 142,
								 m_sRect.top + SMS_APP_TITLE_HEIGHT ),
						  this,SUBSCRIBE_DELETEALL );
	/*SUBSCIBESTRUCT st;
	FillTime ( st.szTime );
	strcpy ( st.szTitle,"gfhdsfjghds" );
	strcpy ( st.szContent,"&&&*fgysfghdfs" );
	AddSubscibeRecord ( st );*/
	
}

//////// Paint
void CSubscribeSMS::OnPaint ( CDC &dc )
{
	
	///// TODO: ADD YOUR CODES
	int i,j,len,n;
	char *szItem[3];
	szItem[0] = new char[2];/////ͼ��
	szItem[1] = new char[17];////ʱ��
	szItem[2] = new char[22];////����

	char pStyle[] = { 0,0,0 };
//	for ( i = 0;i < SMS_SUBSCIBE_MAX_NUMBER;i ++ )
//		m_pGrid.DeleteRow ( 1 );
	m_pGrid.RemoveAll ( );

	char szTemp[17];
	for ( i = 0;i < g_iSubscribePos;i ++ )
	{
		if ( strlen ( g_SubscibeSMS[i].szTitle ) < 1 )
			continue;
		strcpy ( szItem[0],"" );
		

		////����ʱ��
		strcpy ( szTemp,"" );
		if ( ( len = strlen ( g_SubscibeSMS[i].szTime ) ) > 5 ){
			
			for ( n = 0;n < len;n ++ )
				if ( g_SubscibeSMS[i].szTime[n] == 0x20 ) break;

			memcpy ( szTemp,&g_SubscibeSMS[i].szTime[n+1],( len - n -1 )*sizeof ( char ) );
			szTemp[len - n -1] = '\0';
			strcpy ( szItem[2],szTemp );

		}else strcpy ( szItem[2],g_SubscibeSMS[i].szTime );////ʱ��


		
		/////�ü�����
		strcpy ( szTemp,"" );
		if ( strlen ( g_SubscibeSMS[i].szTitle ) > 9 )
		{
			for ( j = 0;j < 9;j ++ )
			{
				if ( g_SubscibeSMS[i].szTitle[j] & 0x80 )
				{
					szTemp[j] = g_SubscibeSMS[i].szTitle[j];
					szTemp[j+1] = g_SubscibeSMS[i].szTitle[j+1];
					j ++;
					if ( j > 8 ) break;

				}else {
				
					szTemp[j] = g_SubscibeSMS[i].szTitle[j];
				}
			}

			szTemp[j] = '\0';
			strcpy ( szItem[1],szTemp );
			
		}else strcpy ( szItem[1],g_SubscibeSMS[i].szTitle );

		UINT32 pIconID[]= { g_SubscibeSMS[i].iconID,0,0};
		m_pGrid.InsertRow (0, szItem,pStyle,pIconID );

	}

	delete []szItem[0];
	delete []szItem[1];
	delete []szItem[2];
	
	
	m_pGrid.SetStartRow ( 1 );
	m_pGrid.ScrollTo ( 1 );
	
	CAppWnd::OnPaint ( dc );
}

////// Destroy
BOOL CSubscribeSMS::OnDestroy ( )
{
	/// TODO: your codes
	if ( m_pRaedSubscribe )
	{
		delete m_pRaedSubscribe;
		m_pRaedSubscribe = NULL;
	}

	return CAppWnd::OnDestroy ( );
}
///ɾ��
void CSubscribeSMS::OnDeleteAll  ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( ::MessageBox ( "ȷ�������ж����ż�ɾ����","����",
		      MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
		int i;	
		for ( i = 0;i < SMS_SUBSCIBE_MAX_NUMBER;i ++ )
			memset ( &g_SubscibeSMS[i],0,sizeof ( SUBSCIBESTRUCT ) );
		
		for ( i = 0;i < SMS_SUBSCIBE_MAX_NUMBER;i ++ )
				m_pGrid.DeleteRow ( 1 );

		m_pGrid.OnPaint ( CDC ( ) );
	}	

}
/////// ����ؼ���Ϣ��Ӧ
void CSubscribeSMS::OnGridCmd  ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	UINT16 iItem = (int)lParam&0x0000ffff;
	///////////ȡ�ö�������
	int pos = g_iSubscribePos - iItem;
	SUBSCIBESTRUCT st = g_SubscibeSMS[pos];
	
	if ( !m_pRaedSubscribe )
		m_pRaedSubscribe = new CReadSubscribeSMS;
	g_SubscibeSMS[pos].iconID = ICON_LETTERREADED;
	m_pRaedSubscribe->SetInformation ( pos,st );
	m_pRaedSubscribe->Create ( "����Ϣ",WS_NORMAL,
		CRect ( 0,0,159,159 ) );

	m_pRaedSubscribe->ShowWindow ( );
	
}

/////////////class CDisplayInformation
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP( CDisplayInformation, CFrameWnd ) 

END_MESSAGE_MAP

CDisplayInformation::CDisplayInformation ( )
{
	strcpy ( m_szPhone,"" );

}

CDisplayInformation::~CDisplayInformation ( )
{


}

void CDisplayInformation::SetPhone ( char *str )
{
	char temp[81];
	strcpy ( temp,str );
	
	strcpy ( m_szPhone,temp );
}
//////Create message
BOOL CDisplayInformation::OnCreate ( )
{
	if ( !CFrameWnd::OnCreate ( ) ) 
		return FALSE;

	////TODO: your codes
}

BOOL CDisplayInformation::OnDestroy ( )
{

	//////TODO:your codes

	return CFrameWnd::OnDestroy ( );
}
////// Paint message
void CDisplayInformation::OnPaint ( CDC &dc )
{

	CFrameWnd::OnPaint ( dc );


	////TODO: your codes
	if ( m_DisplayStyle == 0 ){ ////�ı���ʽ
		
		dc.Draw3dButton ( m_sRect,"" );
		
		int i;
		
		int y = m_sRect.top  + 3;

		dc.SetBackColor ( LGRAY );

		dc.TextOut ( m_sRect.left + 12,y,"����" );
		dc.TextOut ( m_sRect.left + 70,y,"δ����" );
		
		y += 18;
		dc.MoveTo ( m_sRect.left + 3,y  );
		dc.LineTo ( m_sRect.right - 3,y );


		dc.MoveTo ( m_sRect.left + 54,y  );
		dc.LineTo ( m_sRect.left + 54,m_sRect.bottom - 3 );

		y += 2;
		for ( i = 0;i < 4;i ++ )
		{
			dc.TextOut ( m_sRect.left + 3,
				y , g_SystemOption.SMSStatus[i].szName );
		
			dc.PutIcon ( m_sRect.left + 60,y,ICON_LETTERUNREAD1 );
			dc.TextOut ( m_sRect.left + 86,y,"-" );
			char szTemp[20];
			sprintf ( szTemp,"%d��",m_LetterNumber[i] );
			dc.TextOut ( m_sRect.left + 102,y,szTemp );

			y += 18;
		}

		
	}else if ( m_DisplayStyle == 1 ){ ////��ʾ�绰����
	
		dc.Draw3dButton ( m_sRect,"" );

		dc.SetBackColor ( LGRAY );
		
		////����
		int iLen = strlen ( m_szPhone );
		if ( iLen > 16 )
		{
			char szTemp[3];	
			int x = m_sRect.left + 4,
					y = m_sRect.top +4;

			for ( int i = 0; m_szPhone[i]!='\0'; i ++ )
			{
				if ( m_szPhone[i] & 0x80 ) ///����
				{
					szTemp[0] = m_szPhone[i];
					szTemp[1] = m_szPhone[i+1];
					szTemp[2] = '\0';
					i ++;
					dc.TextOut ( x,y,szTemp );
					x += 16;
					if ( x+16 > ( m_sRect.right - 4 ) )
					{
						x = m_sRect.left + 4;	
						y += 16;
			
					}
					
				
				}else {
				
					szTemp[0] = m_szPhone[i];
					szTemp[1] = '\0';

					dc.TextOut ( x,y,szTemp );
					x += 8;

					if ( x+8 > ( m_sRect.right - 4 ) )
					{
						x = m_sRect.left + 4;	
						y += 16;
					}
				}

			}
			

		}else dc.TextOut ( m_sRect.left + 4,m_sRect.top + 4,
				m_szPhone );			
	
	}else if ( m_DisplayStyle ==  2 ){ ////ͼ��
	
	
	}
}

void CDisplayInformation::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;
	if(!IsLegalWnd())  //�����Ѳ���������!        hqf
		return;
	switch(nMessage)
	{
	case WM_ACTIVE:
		OnActive();
		break;
	case WM_DACTIVE:
		OnDactive();
		break;
	case WM_PENDOWN:
		DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
		Destroy ( );
		break;
	case WM_PENMOVE:
		DoPenMove(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	case WM_PENUP:
        DoPenUp();
		break;
	case WM_PAINT:
		Paint();
		break;
	case WM_CLOSE:
		Destroy();
		break;
	case WM_RTC:
		OnRtc(UINT16(wParam));
		break;
	default:
        ProcessMessageMap(nMessage, wParam, lParam);
		break;
	}
}
//////////// class CShowQuestionWnd �Ҽ�e��ѯ�ʷ�ʽ����ʵ

BEGIN_MESSAGE_MAP(CShowQuestionWnd, CAppWnd) 
	ON_MESSAGE ( SHOWQUESTION_OK,CN_COMMAND,OnOKBtn ) 
	ON_MESSAGE ( SHOWQUESTION_CANCEL,CN_COMMAND,OnCloseBtn )
	ON_MESSAGE ( SHOWQUESTION_GRID,CN_GRIDSEL,OnGridCmd )
END_MESSAGE_MAP

CShowQuestionWnd::CShowQuestionWnd ( )
{
//	m_pGrid = NULL;
}

CShowQuestionWnd::~CShowQuestionWnd ( )
{

}
//// Init message
BOOL CShowQuestionWnd::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) )
		return FALSE;

	CRect rt;
	
	//////TODO: your codes
	if ( m_iStyle == 0x02 ){ /////ȷ��ѯ��
		
		

		//////������ť
		rt.left = m_sRect.left + 20;
		rt.right = rt.left + 40;
		rt.top = m_sRect.bottom - 24;
		rt.bottom = m_sRect.bottom - 4;
		
		m_OKBtn.Create ( "ȷ��",rt,this,SHOWQUESTION_OK );

		rt.left = m_sRect.right - 60;
		rt.right = m_sRect.right - 20;
		rt.top = m_sRect.bottom - 24;
		rt.bottom = m_sRect.bottom - 4;

		m_CancelBtn.Create ( "ȡ��",rt,this,SHOWQUESTION_CANCEL );

		////�����������ѿ�
		rt.left = m_sRect.left ;
		rt.top = m_sRect.top + WND_TITLE_HEIGHT;
		rt.right = m_sRect.right;
		rt.bottom = m_sRect.bottom - 28;

		
		m_Edit.Create ( m_szMessage,rt,this,
			WS_CHILD|WS_VISIBLE,SHOWQUESTION_EDIT,256 );

	 
	}else if ( m_iStyle == 0x03 ){ /////����ѡ��ʽ
	
		/////����ȷ��
		//////������ť
		rt.left = m_sRect.left + 60;
		rt.right = rt.left + 40;
		rt.top = m_sRect.bottom - 24;
		rt.bottom = m_sRect.bottom - 4;
		
		m_OKBtn.Create ( "ȷ��",rt,this,SHOWQUESTION_OK );

		
		/////��������ѡ��ؼ�
//		if ( !m_pGrid )
//			m_pGrid = new CGrid;

		rt.left = m_sRect.left,
		rt.top = m_sRect.top + WND_TITLE_HEIGHT ,
		rt.right = 142,
		rt.bottom = 140;

		m_pGrid.Create ( this,
			rt,
			UNS_TEXT|WS_CHILD|TBS_SPACELINE,
			0,2,
			SHOWQUESTION_GRID );

		UINT16 iWidthArray[] = {18,122};

		m_pGrid.SetColsWidth ( iWidthArray ,TRUE );////�����п�

		/////////�������
		m_IconID = ICON_SMSRIGHTARROW;
		FillGrid ( );
		
	
	}else if ( m_iStyle == 0x04 ) { /////�������뷽ʽ
	
	

	}

}

//// Fill grid
void CShowQuestionWnd::FillGrid ( )
{

	int i,j = 0,n = 0;
	UINT8	bLen = g_pMenuBuffer[1]; ////����
	char *szItem = new char[bLen];	

	m_pGrid.RemoveAll ( ); ///Delete all item

	char *szGridItem[2];
	char pUnitStyle[] = { 0,0 };
	UINT32 pIconID[] = { m_IconID,0 };
	szGridItem[0] = new char[2];
	szGridItem[1] = new char[bLen];
	
	for ( i = 4;i < bLen + 2;i ++ )
	{
		szItem[j] = g_pMenuBuffer[i];
		
		if ( g_pMenuBuffer[i] == 0xff ) ////���Ӳ˵�һ��
		{
			szItem[j] = '\0';
			if ( n == 0 ){ //����
				
				SetWindowText ( szItem );
				PaintTitle ( CDC ( ) );

			}else if ( n == 1 ){ ///�˵�����

			
			}else {
				
				strcpy ( szGridItem[1],szItem );
				m_pGrid.AddRow ( szGridItem,pUnitStyle,
					pIconID,FALSE );
			
			}
			
			j = -1;
			n ++;
		}
		
		j ++;
	}

	szItem[j] = '\0';
	strcpy ( szGridItem[1],szItem );
				m_pGrid.AddRow ( szGridItem,pUnitStyle,
					pIconID,FALSE );



	delete []szItem;
	delete []szGridItem[0];
	delete []szGridItem[1];

}
///// Destroy message
BOOL CShowQuestionWnd::OnDestroy ( )
{

	////// TODO: Your codes

	return CAppWnd::OnDestroy ( );
}

///// Paint message
void CShowQuestionWnd::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );

	//////TODO: your codes
}

//// Set style
void CShowQuestionWnd::SetShowStyle ( UINT8 style )
{
	m_iStyle = style;
}

//////����ؼ���Ϣ
void CShowQuestionWnd::OnGridCmd ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	UINT16 iRow =  lParam&0x0000ffff;
	
	//////�ػ�����ؼ�
	m_IconID = m_pGrid.GetUnitIcon ( iRow,1 ) ;
	if ( m_IconID == ICON_SMSRIGHTARROW )
		m_IconID = ICON_OK;
	else m_IconID = ICON_SMSRIGHTARROW;

	m_pGrid.SetUnitIcon ( iRow,1,m_IconID,TRUE );

}
////// ȷ����Ϣ
void CShowQuestionWnd::OnOKBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_iStyle == 0x02 ) { /////ȷ����ʽ
	
#ifdef SMS_DTMF	
		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x00;
		L3Buffer[2] = 0x7f;
#endif

#ifdef SMS_FSK

		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x00;
		L3Buffer[2] = 0x7f;

#endif
		
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );


	}else if ( m_iStyle == 0x03 ) { ///��ѡ��ʽ
	
#ifdef SMS_DTMF

		UINT16 iRows = m_pGrid.GetTotalRows ( );
		UINT8	iNum = 0x00;

		/////������
		int pos = 2;
		for ( UINT16 i = 0x01;i <= iRows;i ++ )
		{
			if ( m_pGrid.GetUnitIcon ( i,1 ) == ICON_OK )
			{
				L3Buffer[pos]	= 0x00;
				L3Buffer[pos+1] = i;
				pos += 2;
				iNum ++;
			}
		
		}

		
		/////ѡ���������
		L3Buffer[0] = 0x00;
		L3Buffer[1] = iNum;

		L3Buffer[pos] = 0x7F;

#endif

#ifdef SMS_FSK

		UINT16 iRows = m_pGrid.GetTotalRows ( );
		UINT8	iNum = 0x00;

		/////������
		int pos = 2;
		for ( UINT16 i = 0x01;i <= iRows;i ++ )
		{
			if ( m_pGrid.GetUnitIcon ( i,1 ) == ICON_OK )
			{
				L3Buffer[pos]	= 0x00;
				L3Buffer[pos+1] = i;
				pos += 2;
				iNum ++;
			}
		
		}

		
		/////ѡ���������
		L3Buffer[0] = 0x00;
		L3Buffer[1] = iNum;

		L3Buffer[pos] = 0x7F;

#endif
	
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );

	}else if ( m_iStyle == 0x04 ) { ////���뷽ʽ 
	
	
	}

}

////// ��ȷ����Ϣ
void CShowQuestionWnd::OnCloseBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_iStyle == 0x02 ) { /////ȷ����ʽ
	
#ifdef SMS_DTMF	
		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x01;
		L3Buffer[2] = 0x7f;
#endif

#ifdef SMS_FSK
		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x01;
		L3Buffer[2] = 0x7f;
#endif
		
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );


	}else if ( m_iStyle == 0x03 ) { ///��ѡ��ʽ
	
	
	}else if ( m_iStyle == 0x04 ) { ////���뷽ʽ 
	
	
	}

}

////// ������Ϣ
void CShowQuestionWnd::SetMessage ( char *str )
{

	strcpy ( m_szMessage,str );
}

//////////// class CShowMessageEx ��ʾ�Ҽ�e�е���Ϣ
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CShowMessageEx, CFrameWnd) 
	ON_MESSAGE ( SHOWMESSAGEEX_OK,CN_COMMAND,OnCloseBtn ) ////����
	ON_MESSAGE ( NULL,WM_PAINT_SECOND,OnPaintSecond ) ////����
END_MESSAGE_MAP

CShowMessageEx::CShowMessageEx ( )
{
	
	m_bIsNeedTimer = FALSE;
	m_bIsDestroy = 0;
}

CShowMessageEx::~CShowMessageEx ( )
{

}

////������
void CShowMessageEx::OnPaintSecond ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	CDC dc;
	
	char sTemp[10];
	sprintf ( sTemp,"%d",m_iSecondNumber );
	/*dc.Draw3dButton ( m_sRect.left + 120,m_sRect.bottom - 17,
		m_sRect.left + 136,m_sRect.bottom - 1 ,sTemp );*/  ////syj modify 2002.1.6

	m_ReturnBtn.SetWindowText ( sTemp );
	m_ReturnBtn.OnPaint ( CDC() );
	m_iSecondNumber --;

}
/////����
void CShowMessageEx::OnCloseBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_bIsDestroy != 2 ) {

	NU_Control_Timer(&SYJTS, NU_DISABLE_TIMER);

#ifdef SMS_DTMF	
		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x00;
		L3Buffer[2] = 0x7f;
#endif

#ifdef SMS_FSK
		L3Buffer[0] = 0x00; //0x00
		L3Buffer[1] = 0x00;
		L3Buffer[2] = 0x7f;
#endif
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 ); 
	
	}else Destroy ( );

	
}


void CShowMessageEx::ChangeText (  )
{
	m_Edit.SetWindowText ( szContent );

	char szButtonTitle[10];
	if ( m_bIsDestroy == 2 )
		strcpy ( szButtonTitle,"�˳�" );
	else strcpy ( szButtonTitle,"����" );

	m_ReturnBtn.SetWindowText ( szButtonTitle );
	m_ReturnBtn.OnPaint ( CDC ( ) );

}

BOOL CShowMessageEx::OnCreate ( )
{
	if ( !CFrameWnd::OnCreate ( ) )
		return FALSE;

	m_iSecondNumber = 5;
	//// TODO: your codes
	/////����EDIT�ؼ�
	m_Edit.Create ( szContent,
					CRect ( m_sRect.left + 1,
					        m_sRect.top + WND_TITLE_HEIGHT,
							m_sRect.right ,
							m_sRect.bottom - 42 ),
							this,
							WS_CHILD|WS_VISIBLE,
							SHOWMESSAGEEX_EDIT,
							256 );

	char szButtonTitle[10];
	if ( m_bIsDestroy == 2 )
		strcpy ( szButtonTitle,"�˳�" );
	else strcpy ( szButtonTitle,"����" );

	m_ReturnBtn.Create ( szButtonTitle,
			CRect ( m_sRect.left + 50,
					m_sRect.bottom - 30,
					m_sRect.left + 110,
					m_sRect.bottom-10 ),this,
					SHOWMESSAGEEX_OK );

}

BOOL CShowMessageEx::OnDestroy ( )
{
	///TODO: your codes
	
	return CFrameWnd::OnDestroy ( );
}

void CShowMessageEx::OnPaint ( CDC &dc )
{
	CFrameWnd::OnPaint ( dc );
	////TODO: your codes

	/*if ( m_bIsNeedTimer )
	{
		dc.SetBackColor ( );
		dc.TextOut ( m_sRect.left + 24,m_sRect.bottom - 17,
			"5���Զ�����" );
	}*/

	CloseCursor ( );
}


/////////// class CSMSServiceWnd �Ҽ�e
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CSMSServiceWnd, CAppWnd) 
	ON_MESSAGE ( SERVICE_GRID,CN_COMMAND,OnGridCmd ) /////����ؼ�����Ϣ����
	ON_MESSAGE ( NULL,WM_UPDATE_MENU,OnUpdateMenu ) ////���¿ͷ����Ĳ˵�
	ON_MESSAGE ( NULL,WM_DISPLAY_MSG,OnDisplayMessage ) ///��ʾ���ĵĶ���Ϣ����
	ON_MESSAGE ( SERVICE_INPUTEDIT,CN_COMMAND,OnUserInput ) ///�û�����
	ON_MESSAGE ( SERVICE_OK,CN_COMMAND,OnOKBtn ) ////�û�ѡ���ύ
END_MESSAGE_MAP

CSMSServiceWnd::CSMSServiceWnd ( )
{ 
//	m_pGrid = NULL; 
	m_InputLength = 0;
	m_InputNumberFlag = 0;
	m_pQuestionWnd = NULL;/////ѯ��
}

CSMSServiceWnd::~CSMSServiceWnd ( )
{

}

/////// ��ʼ����Ϣ
BOOL CSMSServiceWnd::OnCreate ( )
{
	
	if ( !CAppWnd::OnCreate ( ) )
		return  FALSE;
	//// TODO: your coeds
	////// ��������ؼ�
	CRect rc ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT,
		159,159 );

	m_pGrid.Create ( rc,this,LS_SMALLICON,SERVICE_GRID );
	m_pGrid.SetStatus ( LS_SMALLICON ,TRUE );

	/////������ʾ��ʾ�ؼ�
	m_Edit.Create ( "",
		CRect ( 5,
		m_sRect.top+WND_TITLE_HEIGHT+4,
		140,
		110 ),this,WS_CHILD,SERVICE_EDIT,247 );

	////��������ؼ�
	/*m_InputEdit.Create ( "����������...",
		CRect ( 5,120,140,136 ),this,WS_CHILD,
		SERVICE_INPUTEDIT,255 );*/
	m_InputEdit.Create ( "����������...",
		CRect ( 5,118,140,134 ),this,
		SERVICE_INPUTEDIT );

	////�����ύ��ť
	m_OKBtn.Create ( "ȷ��",CRect ( 55,138,100,156 ),this,SERVICE_OK );
	
	m_OKBtn.SetWindowStyle ( m_OKBtn.GetWindowStyle( )&~WS_VISIBLE );

	//////////////���Ͷ���Ϣ.����·�㷢������
	SendMessageToSMSL2 ( Order_Connect_Server_Msg,0,0 );

	return TRUE;
}


///// �ػ���Ϣ
void CSMSServiceWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );

	/// TODO:your coeds
	
	CloseCursor ( );
}

////// ������Ϣ
BOOL CSMSServiceWnd::OnDestroy ( )
{
	/// TODO: your codes
	if ( m_pShowMsg )
	{
		delete m_pShowMsg;
		m_pShowMsg = NULL;
	}

	if ( m_pQuestionWnd )
	{
		delete m_pQuestionWnd;
		m_pQuestionWnd = NULL;
	}

	///////֪ͨ��·��һ�
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	g_pL23MsgBuffer[2] = 0x00;
	SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );


	return CAppWnd::OnDestroy ( );
}

//////����ؼ�����Ϣ
void CSMSServiceWnd::OnGridCmd ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	UINT32 lItem = lParam;/////ѡ����

	/*char ss[10];
	sprintf ( ss,"%d",m_BackModel );
	MessageBox ( "GridMessage",ss,MB_OK );*/
	lItem += 0x01;
	if ( m_BackModel == 0x01 ||
		 m_BackModel == 0x00 ){ ///// �˵�ģʽ

#ifdef SMS_DTMF	
		UINT8 b = lItem&0x000000ff;		
		UINT8 l = b&0x0f;
		UINT8 h = (b>>4)&0x0f;
		L3Buffer[0] = h;
		L3Buffer[1] = l;
		L3Buffer[2] = 0x7f;
#endif

#ifdef SMS_FSK
		UINT8 b = lItem&0x000000ff;		
		UINT8 l = b&0x0f;
		UINT8 h = (b>>4)&0x0f;
		L3Buffer[0] = h;
		L3Buffer[1] = l;
		L3Buffer[2] = 0x7f;
#endif



		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );


	}else if ( m_BackModel == 0x02 ) {

		
	
	
	}else if ( m_BackModel == 0x03 ) {

		
	
	
	}else if ( m_BackModel == 0x04 ) {
	
		
	
	}

}

///// ���¿ͷ����Ĳ˵�
void CSMSServiceWnd::OnUpdateMenu ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	
	//// Close cursor
	CloseCursor ( );
	if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );
	//////������ʾ����
	if ( m_pShowMsg )
	{
		m_pShowMsg->Destroy ( );
		delete m_pShowMsg;
		m_pShowMsg = NULL;

		//MessageBox ( "Destroy","",MB_OK );
	}
		

	if ( m_pQuestionWnd )
	{
		m_pQuestionWnd->Destroy ( );
		delete m_pQuestionWnd;
		m_pQuestionWnd = NULL;
	}

	/////////////////////////////////////////////////////
	UINT8	bLen = g_pMenuBuffer[1]; ////����
	if ( bLen < 0 || bLen >255 ) bLen = 256;
	UINT8	bEnquire = g_pMenuBuffer[2];//ѯ�ʷ�ʽ
	UINT8	bSendback = g_pMenuBuffer[3];///���ͷ�ʽ
	m_BackModel = bSendback;
	m_EnquireModel = bEnquire; 
	
	char *szItem = new char[bLen];
	/*char ss[10];
	sprintf ( ss,"%d",bEnquire );
	MessageBox ( "4559",ss,MB_OK );   syj 2002.2.26*/ 

	int i,j,n = 0;
	UINT32 style;
	////// ɾ��������
	m_pGrid.DeleteAllItems ( );
	

	if  ( bEnquire == 0x01 ){ ////�˵�ѯ�ʻ���ѡ��ѯ������
		//////�л�����
		style = m_pGrid.GetWindowStyle ( );
		m_pGrid.SetWindowStyle ( style|WS_VISIBLE );
		
		style = m_Edit.GetWindowStyle ( );
		m_Edit.SetWindowStyle ( style&~WS_VISIBLE );
		
		style = m_InputEdit.GetWindowStyle ( );
		m_InputEdit.SetWindowStyle ( style&~WS_VISIBLE );

		style = m_OKBtn.GetWindowStyle ( );
		m_OKBtn.SetWindowStyle ( style&~WS_VISIBLE );


		j = 0;
		for ( i = 4;i < bLen + 2;i ++ )
		{
			szItem[j] = g_pMenuBuffer[i];
			
			if ( g_pMenuBuffer[i] == 0xff ) ////���Ӳ˵�һ��
			{
				szItem[j] = '\0';
				if ( n == 0 ){ //����
					
					SetWindowText ( szItem );
					PaintTitle ( CDC ( ) );

				}else if ( n == 1 ){ ///�˵�����
				
				}else m_pGrid.AppendItem ( 
					szItem,0,NULL,ICON_SMSRIGHTARROW );
				
				j = -1;
				n ++;
			}
			
			j ++;
		}

		szItem[j] = '\0';
		m_pGrid.AppendItem ( 
					szItem,0,NULL,ICON_SMSRIGHTARROW );


	}else if ( bEnquire == 0x02 ) { ////ȷ��ѯ������
	
		////��ȡ�ִ�
		for ( i = 4;i < bLen + 2;i ++ )
			szItem[i-4] = g_pMenuBuffer[i];
		szItem[i-4] = '\0';

		
		if ( !m_pQuestionWnd ) ///
			m_pQuestionWnd = new CShowQuestionWnd;
		
		m_pQuestionWnd->SetMessage ( szItem );
		m_pQuestionWnd->SetShowStyle ( bEnquire );
		
			
		m_pQuestionWnd->Create ( "ѯ��",
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD,
			CRect ( 0,0,159,159 ) );

		m_pQuestionWnd->ShowWindow ( );

		memset ( g_pMenuBuffer,0,256 ); /// Clear 0
		delete []szItem;
		return;
		
		
	}else if ( bEnquire == 0x03 ) { ////����ѡ������
	
		if ( !m_pQuestionWnd ) ///���ڲ�����
			m_pQuestionWnd = new CShowQuestionWnd;

		m_pQuestionWnd->SetShowStyle ( bEnquire );

		m_pQuestionWnd->Create ( "ѡ��",
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD,
			CRect ( 0,0,159,159 ) );

		m_pQuestionWnd->ShowWindow ( );
		
		memset ( g_pMenuBuffer,0,256 ); /// Clear 0
		delete []szItem;

		return;

	}else if ( bEnquire == 0x04 ) { ////��ѡ������������
	
		style = m_pGrid.GetWindowStyle ( );
		m_pGrid.SetWindowStyle ( style&~WS_VISIBLE );
		
		style = m_Edit.GetWindowStyle ( );
		m_Edit.SetWindowStyle ( style|WS_VISIBLE );
		
		style = m_InputEdit.GetWindowStyle ( );
		m_InputEdit.SetWindowStyle ( style|WS_VISIBLE );
		m_InputEdit.SetWindowText ( "����������..." );
		m_InputEdit.OnPaint ( CDC ( ) );


		style = m_OKBtn.GetWindowStyle ( );
		m_OKBtn.SetWindowStyle ( style|WS_VISIBLE );

		/////��ȡ�ַ�����Ϣ
		for ( i = 4;i < bLen+2;i ++ )
		{
			szItem[i-4] = g_pMenuBuffer[i];
			if ( g_pMenuBuffer[i] == 0xff ) break;
		}

		szItem[i-4] = '\0';
		m_Edit.SetWindowText ( szItem );

		i ++;
		m_InputLength = g_pMenuBuffer[i++];////ȡ��������볤��
		
		m_InputNumberFlag = g_pMenuBuffer[i];////ȡ����ϸ������Ϣ
		
		//OnPaint ( CDC() );

	}
	

	memset ( g_pMenuBuffer,0,256 ); /// Clear 0
	delete []szItem;
	OnPaint ( CDC() );

}

//////////////////��ʾ���ĵĶ���Ϣ����
void CSMSServiceWnd::OnDisplayMessage ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );

	UINT8	bLen = g_pMenuBuffer[1],////����
			bChildCmd = g_pMenuBuffer[2];//��������

	char    szTitle[40];
	char    szTemp[256];
	int i;

	GetWindowText ( szTitle,40 ); ///ȡ�ø����ڱ���
	switch ( bChildCmd )
	{
	
	case 0x00: ////����
		//MessageBox ( "����","",MB_OK );
		break;
	case 0x01://///�ı���ʾ��λ
		//MessageBox ( "�ı���ʾ��λ","",MB_OK );
		break;
	case 0x02://///�ı���ʾ������Ļ�ƶ�����
		break;
	case 0x03:////�ı���ʾ����ʾ����
		for ( i = 4;i < bLen + 2;i ++ )
		{
			szTemp[i-4] = g_pMenuBuffer[i];		

			if ( szTemp[i-4] == 0xff ) break;
		}

		szTemp[i-4] = '\0';
		/////��ʾe��Ϣ
		if ( !m_pShowMsg )
		{
			m_pShowMsg = new CShowMessageEx;
		
			/////�Ƿ�������ʱ��
			if ( g_pMenuBuffer[3] == 0x01 )
				NU_Reset_Timer(&SYJTS, SYJTimerOver, 1000, 200, NU_ENABLE_TIMER);				
			
			m_pShowMsg->SetTimerFlag ( g_pMenuBuffer[3] );
			m_pShowMsg->SetString ( szTemp ); 
			m_pShowMsg->SetDestroyFlag ( 0 );
			m_pShowMsg->Create ( szTitle,
				WS_VISIBLE | WS_CAPTION | WS_CHILD,
				CRect ( 0,0,159,159 ) ,0 );

			
			m_pShowMsg->ShowWindow ( );
		
		}else {
			
			NU_Control_Timer(&SYJTS, NU_DISABLE_TIMER);

			/////�Ƿ�������ʱ��
			if ( g_pMenuBuffer[3] == 0x01 )
				NU_Reset_Timer(&SYJTS, SYJTimerOver, 1000, 200, NU_ENABLE_TIMER);				
			
			m_pShowMsg->SetTimerFlag ( g_pMenuBuffer[3] );
			m_pShowMsg->SetString ( szTemp ); 
			m_pShowMsg->SetDestroyFlag ( 0 );

			m_pShowMsg->ChangeText (  );
			
		}

		break;
	case 0x04://///��Ļ��ʾ����
		for ( i = 5;i < bLen + 2;i ++ )
		{
			szTemp[i-5] = g_pMenuBuffer[i];		
			if ( szTemp[i-5] == 0xff ) break;
		}

		szTemp[i-5] = '\0';
		
		/////��ʾe��Ϣ
		if ( !m_pShowMsg )
		{
			m_pShowMsg = new CShowMessageEx;
		
			m_pShowMsg->SetString ( szTemp ); 
			m_pShowMsg->SetDestroyFlag ( 0 );
			m_pShowMsg->Create ( szTitle,
				WS_VISIBLE | WS_CAPTION | WS_CHILD,
				CRect ( 0,0,159,159 ) ,0 );

			m_pShowMsg->ShowWindow ( );
		
		}else {
		
			m_pShowMsg->SetString ( szTemp ); 
			m_pShowMsg->SetDestroyFlag ( 0 );
			

			m_pShowMsg->ChangeText ( );
		
		}
		
		break;	
	}
	

	
}


void CSMSServiceWnd::OnUserInput ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	char *szTemp = new char[m_InputLength+1];
	memset ( szTemp,0,(m_InputLength+1)*sizeof ( char ) );
	if ( InputBox ( "����������",szTemp,m_InputLength ) == IDOK )
	{
		m_InputEdit.SetWindowText ( szTemp );
		m_InputEdit.OnPaint ( CDC ( ) );
		delete []szTemp;
	} 
}

///////////////�û�ѡ���ύ
void CSMSServiceWnd::OnOKBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	char *szTemp = new char[m_InputLength+1];
	m_InputEdit.GetWindowText ( szTemp,m_InputLength );
	UINT8  bLen = strlen ( szTemp );
	if ( m_InputNumberFlag&0x0f == 0x00 ){ ////���������û����벻Ҫ��
	
		//TODO:your codes


	}else if ( m_InputNumberFlag&0x0f == 0x0f ){///��������
		
		/*if (  bLen != m_InputLength )
		{
			MessageBox ( "�밴Ҫ������.","����",MB_OK );
			delete []szTemp;
			return ;
		}*/

	}else {
	
		if ( bLen <= 0 )
		{
			MessageBox ( "�������Ϊ��!","����",MB_OK );
			delete []szTemp;
			return ;
		
		}
	}


	if ( m_EnquireModel == 0x01 ){ ///// �˵�ģʽ
	
		/*UINT8 b = lItem&0x000000ff;		
		UINT8 l = b&0x0f;
		UINT8 h = (b>>4)&0x0f;
		L3Buffer[0] = h;
		L3Buffer[1] = l;
		L3Buffer[2] = 0x7f;
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
		*/

	}else if ( m_EnquireModel == 0x02 ) {
	
	
	}else if ( m_EnquireModel == 0x03 ) {
	
	
	}else if ( m_EnquireModel == 0x04 ) {

#ifdef SMS_DTMF		
		////////////////��UINT8��������ɵ��ֽ�
		int iLen;
		UINT8 *buffer = new UINT8[bLen+1];
		buffer[0] = bLen; ///����
		for ( iLen = 0;iLen < bLen;iLen ++ )
			buffer[iLen+1] = szTemp[iLen];

		UINT8 *buf = new UINT8[2*(bLen+1)];
		for ( iLen = 0;iLen < bLen + 1 ;iLen ++ )
		{
			UINT8 uc = (UINT8)buffer[iLen];
			UINT8 luc = uc&0x0f;
			UINT8 huc = (uc>>4)&0x0f;
			buf[2*iLen] = huc;
			buf[2*iLen+1] = luc;
		}

		////��buffer������ָ��������
		for ( iLen = 0;iLen < 2*(bLen+1);iLen ++ )
			L3Buffer[iLen] = buf[iLen];
		L3Buffer[iLen] = 0x7F;

		
		delete []buf;
		delete []buffer;
		delete []szTemp;
#endif

#ifdef SMS_FSK
		////////////////��UINT8��������ɵ��ֽ�
		int iLen;
		UINT8 *buffer = new UINT8[bLen+1];
		buffer[0] = bLen; ///����
		for ( iLen = 0;iLen < bLen;iLen ++ )
			buffer[iLen+1] = szTemp[iLen];

		UINT8 *buf = new UINT8[2*(bLen+1)];
		for ( iLen = 0;iLen < bLen + 1 ;iLen ++ )
		{
			UINT8 uc = (UINT8)buffer[iLen];
			UINT8 luc = uc&0x0f;
			UINT8 huc = (uc>>4)&0x0f;
			buf[2*iLen] = huc;
			buf[2*iLen+1] = luc;
		}

		////��buffer������ָ��������
		for ( iLen = 0;iLen < 2*(bLen+1);iLen ++ )
			L3Buffer[iLen] = buf[iLen];
		L3Buffer[iLen] = 0x7F;

		
		delete []buf;
		delete []buffer;
		delete []szTemp;

#endif
	
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
	}
}
/////////// class CShowMessage
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CShowMessage, CFrameWnd) 
END_MESSAGE_MAP


void CShowMessage::ShowWindow(UINT32 nCmdShow)
{
	CFrameWnd::ShowWindow ( nCmdShow );
	switch(nCmdShow)
	{
	case SW_SHOW:
		m_bIsVisible = TRUE;
		break;
	case SW_HIDE:
		m_bIsVisible = FALSE;
		break;
	}

}

void CShowMessage::OnPaint ( CDC &dc )
{
	CFrameWnd::OnPaint ( dc );
		
		int len;
		if ( ( len = strlen ( szMessage ) ) > 12 )
		{
		
			char szTemp1[15],szTemp2[15];
			int j,i,ic = 0;
			for ( i = 0;i < 12;i ++ )
			{
				if ( ! (szMessage[i] & 0x80) ) 
					ic ++;
				szTemp1[i] = szMessage[i];
			}

			if ( (ic%2) )
			{
				szTemp1[i-1] = '\0';
				j = i - 1;	
				i -= 1;

			}else {
			
				szTemp1[i] = '\0';
				j = i;
			}
			
			
			for ( ;i < len;i ++ )
				szTemp2[i-j] = szMessage[i];
			szTemp2[i-j] = '\0';

			int width = m_sRect.right - m_sRect.left;
			int height = m_sRect.bottom - m_sRect.top;
			int x1 = m_sRect.left + ( width - 8*strlen(szTemp1) )/2,
				x2 = m_sRect.left + ( width - 8*strlen(szTemp2) )/2,
				y1 = m_sRect.top + ( height - 32 )/3,
				y2 = m_sRect.top + 2*( height - 32 )/3 + 16;
			
			::HwPaintRect(m_sRect.left, m_sRect.top, 
				m_sRect.right, m_sRect.bottom, LGRAY);
			
			dc.SetBackColor ( LGRAY );
			dc.TextOut ( x1,y1,szTemp1 );
			dc.TextOut ( x2,y2,szTemp2 );
			dc.Draw3dRect(m_sRect.left, m_sRect.top, 
				m_sRect.right, m_sRect.bottom );
		}else {
		
			dc.Draw3dButton ( m_sRect,szMessage );
		}


	CloseCursor ( );
}
///////////// class CChoiceModel
BEGIN_MESSAGE_MAP ( CChoiceModel,CFrameWnd )
	ON_MESSAGE ( CHOICEMODEL_LISTVIEW,CN_COMMAND,OnCurSel ) ///CChoiceModel	
	ON_MESSAGE ( CHOICEMODEL_OK,CN_COMMAND,OnOK )
	ON_MESSAGE ( CHOICEMODEL_CLOSE,CN_COMMAND,OnClose )
END_MESSAGE_MAP

CChoiceModel::CChoiceModel ( )
{
//	m_pList = NULL; ///INIT
	
}

CChoiceModel::~CChoiceModel ( )
{

}

BOOL CChoiceModel::OnCreate ( )
{

	if ( !CFrameWnd::OnCreate ( ) ) 
		return FALSE;
	
	m_iSelected = -1;
	///////����ListView
	m_pList.Create ( CRect ( m_sRect.left + 5,
							  m_sRect.top + 21,
							  m_sRect.right-4,
							  m_sRect.bottom-4 ),
							  this, LS_SMALLICON, 
							  CHOICEMODEL_LISTVIEW );
    
	m_pList.SetStatus(LS_SMALLICON, TRUE);
	
	/////����OK,Close button
	m_OKBtn.Create ( "",ICON_OK,
		CRect ( m_sRect.right - 34,
		        m_sRect.top + 1,
				m_sRect.right - 18,
				m_sRect.top + 17),this,CHOICEMODEL_OK );

	m_CloseBtn.Create ( "",ICON_CANCEL,
		CRect ( m_sRect.right - 17,
		        m_sRect.top + 1,
				m_sRect.right - 1,
				m_sRect.top + 17),this,CHOICEMODEL_CLOSE );
	///////////���
	int r = FindMODEL ( g_Model_Locked,MAX_MODEL_NUMBER );
		
	for(int i = 0; i< r; i++) 
	{
	   MODEL *pTemp = GetModelPointer(g_Model_Locked[i]);	   	 
	   m_pList.AppendItem (pTemp->szModelName,
		   0,0,ICON_MODEL_Content, g_Model_Locked[i]); 	 
	}

}

BOOL CChoiceModel::OnDestroy ( )
{
	//////TODO:your codes

	return CFrameWnd::OnDestroy ( );
	
}


void CChoiceModel::OnPaint ( CDC &dc )
{
	
	CFrameWnd::OnPaint ( dc );
	

	dc.SetBackColor (  );
	dc.TextOut ( m_sRect.left + 1,m_sRect.top + 1,
		"��ѡ��" );
	dc.Draw3dRect(m_sRect.left, m_sRect.top, 
				m_sRect.right, m_sRect.bottom );

	
	CRect rt ( m_sRect.left+4,m_sRect.top + 20,
		m_sRect.right - 4,m_sRect.bottom - 4 );

	dc.Draw3dRect ( rt,BLACK,BLACK );
	dc.Draw3dRect ( m_sRect,BLACK,BLACK );

}

void CChoiceModel::OnOK ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_iSelected == -1 )
	{
		MessageBox ( "��ѡ���������","����",
			MB_OK|MB_ICONSTOP );

		return;
	}

	Destroy ( );
}

void CChoiceModel::OnClose ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	m_pSaveAsWnd->SetCheckButton ( 5,-1 );
	
	Destroy ( );
}

void CChoiceModel::OnCurSel ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	/// Codes
	m_iSelected = lParam&0x0000ffff;
	m_pSaveAsWnd->SetID ( m_iSelected );
}
///////////////////////////   class CSaveAsWnd
BEGIN_MESSAGE_MAP ( CSaveAsWnd,CAppWnd )
	ON_MESSAGE ( SAVEAS_MODLE,CN_COMMAND,OnModle )
END_MESSAGE_MAP

//////// ��ʼ����Ϣ
BOOL CSaveAsWnd::OnCreate ( )
{

	
	m_iItem = -1;
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;

	///////// TODO: your codes
	CRect rc ( m_sRect.left + 2,
			   m_sRect.top + WND_TITLE_HEIGHT + 4,
			   m_sRect.left + 22,
			   m_sRect.top + WND_TITLE_HEIGHT + 20 );
	/////˽������1
	m_mailbox1.Create ( "",rc,this );

	/////˽������2
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox2.Create ( "",rc,this );
	
	//// ˽������3
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox3.Create ( "",rc,this );

	//////��������
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox4.Create ( "",rc,this );

	///�Զ��峣����
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox5.Create ( "",rc,this ,false,SAVEAS_MODLE );


	CloseCursor ( );

	return TRUE;
}

//////// ������Ϣ
BOOL CSaveAsWnd::OnDestroy ( )
{
	////// TODO: your codes
	if ( m_pChoiceModel )
	{
		delete m_pChoiceModel;
		m_pChoiceModel = NULL;
	}

	return CAppWnd::OnDestroy ( );
}

void CSaveAsWnd::OnModle ( UINT32 message,UINT32 wParam,UINT32 lParam )
{


	if ( !m_mailbox5.GetValue ( ) ) {
		
		m_iItem = -1;
		OnPaint ( CDC ( ) );
		return;
	}

	int r = FindMODEL ( g_Model_Locked,MAX_MODEL_NUMBER );
	if ( r == 0 )
	{
		MessageBox ( "û�н����������顣",
			"����",MB_OK );
		m_mailbox5.SetCheck ( FALSE );
		return;
	}

	
	if ( !m_pChoiceModel )
		m_pChoiceModel = new CChoiceModel;
	
	m_pChoiceModel->Create ( "",
			 WS_CHILD|WS_VISIBLE|WS_BORDER,
			 CRect ( 60,30,150,150 ),0 );
		
	m_pChoiceModel->ShowWindow ( );

	
}
////// �ػ���Ϣ
void CSaveAsWnd::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );
	///// TODO: your codes
	////// ������������,	��������״̬
	int y = m_sRect.top + WND_TITLE_HEIGHT + 2;
	dc.SetBackColor ( );

	if ( g_SystemOption.SMSStatus[0].iLockType != 0 )
		dc.PutIcon ( 30,y + 2,ICON_LETTERLOCK );
	dc.TextOut ( 50,y,g_SystemOption.SMSStatus[0].szName );
	
	y += 24;
	if ( g_SystemOption.SMSStatus[1].iLockType != 0 )
		dc.PutIcon ( 30,y + 2,ICON_LETTERLOCK );
	dc.TextOut ( 50,y,g_SystemOption.SMSStatus[1].szName );

	y += 24;
	if ( g_SystemOption.SMSStatus[2].iLockType != 0 )
		dc.PutIcon ( 30,y + 2,ICON_LETTERLOCK );
	dc.TextOut ( 50,y,g_SystemOption.SMSStatus[2].szName );

	y += 24;
	dc.TextOut ( 50,y,"��������" );

	y += 24;
	if ( m_iItem == -1 )
		dc.TextOut ( 50,y,"������" );
	else {
	
		MODEL *pTemp = GetModelPointer(g_Model_Locked[m_iItem]);	   	 
		char temp[20];
		
		strcpy ( temp,"������(" );
		strcat ( temp,pTemp->szModelName );
		strcat ( temp,")" );
		dc.TextOut ( 50,y,temp );
	}


}
////////Ok��ť
void CSaveAsWnd::OnOk ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	/////// TODO: your codes
	
	int r;

	if ( m_mailbox1.GetValue ( ) )///˽��1ѡ��
	{
	
		g_ST.bMailBox = 0;///��ǰѡ�е�����
		g_ST.iChildMailBox = 0;/////���浽������
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;

		r = AddRevRecord ( g_ST ,FALSE );
		if ( r == -1 )
			goto p2;
		
	}


p2:	if ( m_mailbox2.GetValue ( ) )///˽��2ѡ��
	{

			
				
		g_ST.bMailBox = 1;///��ǰѡ�е�����
		g_ST.iChildMailBox = 0;/////���浽������
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;


		r = AddRevRecord ( g_ST,FALSE );
		if ( r == -1 ) 
			goto p3;

		

			
	}

p3:	if ( m_mailbox3.GetValue ( ) )///˽��3ѡ��
	{

		g_ST.bMailBox = 2;///��ǰѡ�е�����
		g_ST.iChildMailBox = 0;/////���浽������
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;

		r = AddRevRecord ( g_ST,FALSE );
		if ( r == -1 ) 
			goto p4;

		

			
	}

p4:	if ( m_mailbox4.GetValue ( ) )///��������ѡ��
	{

		g_ST.bMailBox = 3;///��ǰѡ�е�����
		g_ST.iChildMailBox = 0;/////���浽������
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;

		r = AddRevRecord ( g_ST,FALSE );
		if ( r == -1 ) 
			goto p5;

		

			
	}

p5:	if ( m_mailbox5.GetValue ( ) )///�Զ��峣����ѡ��
	{

		//////���浽������
		CContentWnd dd;
		dd.OnSMSaveAs( g_Model_Locked[m_iItem], 
			g_ST.szMsg );
		
	}
	
	m_iItem = -1;
	Destroy ( );
}

/*============================================================================*/
//  CButton�ຯ������                                                         //
/*============================================================================*/
BOOL CButtonEx::Create(char *strCaption,CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId,BOOL bStyle )
{
	m_bPressed = 0;
	m_bStyle = bStyle;
	return CCtrl::Create(strCaption, ctrlRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CButtonEx::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;

    switch(nMessage)
    {
    case WM_PENMOVEIN:
    case WM_PENDOWN:
    case WM_PENMOVE:
        if(!m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 1;
        }
        break;
    case WM_PENMOVEOUT:
        if(m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 0;
        }
         break;
    case WM_PENUP:
        if(m_bPressed)
        {
        CDC::InvertRect (m_sRect); 
        m_bPressed = 0;
        SendNotifyMessage(CN_COMMAND, 0);
        }
        break; 
    }
}

////////////////////////////////////////////////////////////////////////////////
//
void CButtonEx::OnPaint(CDC &dc)
{
	if ( m_bStyle == 1 )
	{
		dc.SetBackColor();
		int y = m_sRect.top + ( m_sRect.Height( ) -16 )/2;
		dc.TextOut ( m_sRect.left,y,"          " );
		
		char szTemp[20];

		int len = strlen ( m_strName ),i;
		for ( i = 0;i < len;i ++ )
			szTemp[i] = '*';
		szTemp[i] = '\0';

		dc.TextOut ( m_sRect.left,y,szTemp );
	
	}else if ( m_bStyle == 2 ) {
	
		dc.SetBackColor();
		int y = m_sRect.top + ( m_sRect.Height( ) -16 )/2;
		dc.TextOut ( m_sRect.left,y,"          " );
		dc.TextOut ( m_sRect.left,y,m_strName );
	
	}else dc.Draw3dButton(m_sRect, m_strName);  
}

///////////////////////////   class CSetNameAndPassword
BEGIN_MESSAGE_MAP ( CSetNamePassword,CAppWnd )
	ON_MESSAGE ( SETNAMEPASSWORD_NAME,CN_COMMAND,OnNameInput ) /////��������
	ON_MESSAGE ( SETNAMEPASSWORD_PASSWORD,CN_COMMAND,OnPasswordInput ) /////��������
	ON_MESSAGE ( SETNAMEPASSWORD_SHOWPOPUPLV,CN_COMMAND,OnShowLV ) ///��ʾListBox
	ON_MESSAGE ( SETNAMEPASSWORD_POPUPLV,CN_LISTSEL,OnCurSel ) ///ListBoxѡ������Ϣ
	ON_MESSAGE ( SETNAMEPASSWORD_SOUNDSTOP,CN_COMMAND,OnSoundCtrl )//control sound
END_MESSAGE_MAP

CSetNamePassword::CSetNamePassword ( )
{
	iItem = 0;
	m_bIsPublic = FALSE;
	
}

CSetNamePassword::~CSetNamePassword () 
{


}
/////// ��ʼ��
BOOL CSetNamePassword::OnCreate ( )
{
	
	bSoundCtrlFlag = FALSE; ////�������ſ���

	if ( CAppWnd::OnCreate ( ) == FALSE ) return FALSE;

	if ( m_iStyle == 0 ) {  ////��������
		////��������༭�ؼ�
		if ( !m_bIsPublic )
			m_Password.Create ( g_SystemOption.SMSStatus[iItem].szPassword,
							CRect ( 46,
									m_sRect.top + WND_TITLE_HEIGHT + 22,
									m_sRect.right - 1,
									m_sRect.top + WND_TITLE_HEIGHT + 38 ),
							this,
							SETNAMEPASSWORD_PASSWORD ,TRUE );
		m_Password.SetStyle ( 1 );
		/////���������༭�ؼ�
		m_Name.Create ( g_SystemOption.SMSStatus[iItem].szName,
			CRect ( 46,
					m_sRect.top + WND_TITLE_HEIGHT + 2,
					m_sRect.right - 1,
					m_sRect.top + WND_TITLE_HEIGHT + 18 ),this,
					SETNAMEPASSWORD_NAME ,TRUE );
		m_Name.SetStyle ( 2 );
		////�������ý��շ�ʽ�ؼ�
		m_RevType.Create ( "����Ϣ�ռ���ʱ,�����¶���Ϣ",
						   CRect ( m_sRect.left,
						   m_sRect.top + WND_TITLE_HEIGHT + 42,
						   m_sRect.right - 1,
						   m_sRect.top + WND_TITLE_HEIGHT + 74 ),
						   this,!g_SystemOption.SMSStatus[iItem].bIsCanDelete,
						   SETNAMEPASSWORD_REVTYPE );


		/////����������ʾ�ؼ�
		BOOL bIsSelect = TRUE;
		if ( g_SystemOption.SMSStatus[iItem].iRingID == 0xff )
			bIsSelect = FALSE;
		m_Sound.Create (  "�ж���Ϣ����ʱ,ʹ��������ʾ",
						   CRect ( m_sRect.left,
								   m_sRect.top + WND_TITLE_HEIGHT + 78,
								   m_sRect.right - 1,
								   m_sRect.top + WND_TITLE_HEIGHT + 110 ),
						   this,bIsSelect,
						   SETNAMEPASSWORD_SOUND );

		//////�����������ƿؼ�
		m_SoundName.Create ( "",
							 CRect ( m_sRect.left + 16,
									 m_sRect.top + WND_TITLE_HEIGHT + 114,
									 m_sRect.left + 120,
									 m_sRect.top + WND_TITLE_HEIGHT + 130 ),
							 this,WS_CHILD|WS_VISIBLE,
							 SETNAMEPASSWORD_SOUNDPATH ,20);

		/////��������ListBox�İ�ť
		m_ShowPopupLV.Create ( "",ICON_DOWNARROW,
							   CRect ( m_sRect.left + 121,
									   m_sRect.top + WND_TITLE_HEIGHT + 112,
									   m_sRect.left + 133 ,
									   m_sRect.top + WND_TITLE_HEIGHT + 130 ),
									   this,
									   SETNAMEPASSWORD_SHOWPOPUPLV );

		/////���������������Ű�ť
		m_SoundCtrl.Create ( "��",
			CRect ( m_sRect.left + 134 ,
					m_sRect.top + WND_TITLE_HEIGHT + 112,
					m_sRect.left + 150 ,
		    		m_sRect.top + WND_TITLE_HEIGHT + 130 ),
					this,
					SETNAMEPASSWORD_SOUNDSTOP,FALSE );
		////// ����ListBox
		m_PopupLV.Create ( CRect ( 0,0,124,100 ),this,NULL,SETNAMEPASSWORD_POPUPLV );

		
		///////�����ɫ����Ϣ����
		int r = FindRing ( g_pSMSDataIDBuf,RING_MAX_COUNT );
		for ( int i = 0;i < r;i ++ )
		{
			RING *pRingTemp = GetRingPointer ( g_pSMSDataIDBuf[i] );
			m_PopupLV.AppendItem ( pRingTemp->Name,0,0 );
		}
		////����ȱʡ������
		if ( g_SystemOption.SMSStatus[iItem].iRingID != 0xff ){
			
			RING *pRingTemp = GetRingPointer ( g_SystemOption.SMSStatus[iItem].iRingID );
			m_SoundName.SetWindowText ( pRingTemp->Name );

		}else m_SoundName.SetWindowText ( m_PopupLV.GetListItemText(0) );

	
	}else if ( m_iStyle == 1 ) { ////��������

		/////////�����ڶ�ҳ�Ŀؼ���ȱʡΪ����
		UINT32 lStyle;
		m_EnableSound.Create ( "�շ�ʱ���Ƿ���Ҫ����",
			CRect ( m_sRect.left,
					m_sRect.top + WND_TITLE_HEIGHT,
					m_sRect.right - 1,
					m_sRect.top + WND_TITLE_HEIGHT + 32 ),
					this,
					g_SystemOption.bIsNeedSound );

	
	}else if ( m_iStyle == 2 ) { /////��ִ����
		//////////////////////////////////
		m_EnableReturn.Create ( "�Ƿ���Ҫ��������ִ",
			CRect ( m_sRect.left,
					m_sRect.top + WND_TITLE_HEIGHT,
					m_sRect.right - 1 ,
					m_sRect.top + WND_TITLE_HEIGHT + 32 ),
					this,
					g_SystemOption.bIsNeedReturn );


	}

	return TRUE;
}

////������������
void CSetNamePassword::OnSoundCtrl ( UINT32 mesage,UINT32 wParam,UINT32 lParam )
{
	
	if ( !bSoundCtrlFlag )
	{
		m_SoundCtrl.SetButtonCaption ( "ͣ" );
		::sndPlayMusic(g_SystemOption.SMSStatus[iItem].iRingID,true,-1);
	
	}else {

		m_SoundCtrl.SetButtonCaption ( "��" );
		////ֹͣ��ǰ����
		StopRing();	
	}
	
	bSoundCtrlFlag = !bSoundCtrlFlag;
}
///////ListBox������Ϣ
void CSetNamePassword::OnCurSel ( UINT32 mesage,UINT32 wParam,UINT32 lParam )
{
	g_SystemOption.SMSStatus[iItem].iRingID = g_pSMSDataIDBuf[lParam&0x0000ffff];
	m_SoundName.SetWindowText ( m_PopupLV.GetListItemText(INT32(lParam)) );

	////�ı���������״̬
	bSoundCtrlFlag = FALSE;
	m_SoundCtrl.SetButtonCaption ( "��" );
	////ֹͣ��ǰ����
	StopRing();	
}
///////��ʾListBox
void CSetNamePassword::OnShowLV ( UINT32 mesage,UINT32 wParam,UINT32 lParam )
{
	
	m_PopupLV.PopUpList ( m_sRect.left + 20,
					m_sRect.top + WND_TITLE_HEIGHT + 12 );
	
}
////// �ػ���Ϣ
void CSetNamePassword::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );
	///// Add your codes
	
	if ( m_iStyle == 0 ){ ///��һҳ
		
		dc.SetBackColor ( );
		dc.SetLineStyle ( 1 );

		dc.TextOut ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 2,
			"����:" );

		if ( !m_bIsPublic ) 
			dc.TextOut ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 22,
				"����:" );
		else dc.TextOut ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 22,
				"�������䲻���Լ���" );

		
		dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 20 );
		dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 20 );
		
		dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 40 );
		dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 40 );

		
		dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 76 );
		dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 76 );

		
		dc.SetLineStyle (  );
	}


	
}
///// ����
BOOL CSetNamePassword::OnDestroy ( )
{

	////ֹͣ��ǰ����
	StopRing();

	return CAppWnd::OnDestroy ( );
}
////// OK
void CSetNamePassword::OnOk ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	if ( m_iStyle == 0 ) { ///��������

		char name[7],password[10];
		m_Name.GetWindowText ( name,7 );
		if ( iItem != 3 )
			m_Password.GetWindowText ( password,10 );

		strcpy ( g_SystemOption.SMSStatus[iItem].szName,name );
		if ( iItem != 3 ) ///��������ֻ�ܸ�������,���ܸ�������
			strcpy ( g_SystemOption.SMSStatus[iItem].szPassword,password );

		if ( iItem != 3 )
			g_SystemOption.SMSStatus[iItem].iLockType = ( strlen ( password ) > 0 )?2:0 ;
	
	

		/////���շ�ʽ
		g_SystemOption.SMSStatus[iItem].bIsCanDelete = !m_RevType.GetValue ( );

		////���巽ʽ��oxff = ������
		if ( !m_Sound.GetValue ( ) ) 
			g_SystemOption.SMSStatus[iItem].iRingID = 0xff;

		
	
	}else if ( m_iStyle == 1 ) { 
		
		g_SystemOption.bIsNeedSound = m_EnableSound.GetValue ( );
		
	}else if ( m_iStyle == 2 ) {
	
		///�Ƿ���Ҫ��ִ
		g_SystemOption.bIsNeedReturn = m_EnableReturn.GetValue ( );

	}

	/////�������Ϣ��ϵͳ
	SaveSystemOption ( &g_SystemOption );

	////ֹͣ��ǰ����
	StopRing();
	////���ٴ���
	Destroy ( );
}
//////��������
void CSetNamePassword::OnNameInput ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	char szTemp[7];
	m_Name.GetWindowText ( szTemp,7 );
	if ( InputBox ( "����������",szTemp,7 ) == IDOK )
	{
		m_Name.SetButtonCaption ( szTemp );

	}
}
//////��������
void CSetNamePassword::OnPasswordInput ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	char szTemp[10];
	memset ( szTemp,0,10 );
	
	if ( InputPassword ( szTemp,
					PASSWORDLENGTH,0 ) == IDOK )
	{
	if(IsLegalWnd())
		m_Password.SetButtonCaption ( szTemp );

	}

}
///////////////////////////   class CSetLetterBox
BEGIN_MESSAGE_MAP ( CSetLetterBox,CAppWnd )
	ON_MESSAGE ( SETLETTERBOX_LISTVIEW,CN_COMMAND,OnListViewCmd ) ///�б���Ϣ
END_MESSAGE_MAP

CSetLetterBox::CSetLetterBox ( )
{

	iSelectedItem = 0;
	m_pSetNamePassword = NULL;

	bIsSetting = FALSE;
}

CSetLetterBox::~CSetLetterBox ( )
{

}

//////�б���Ϣ����
void CSetLetterBox::OnListViewCmd ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	iSelectedItem = lParam;///////�õ���ѡ�� 
	OnSet ( uMessage,wParam,lParam );
}
////// �趨��Ϣ
void CSetLetterBox::OnSet ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	
	////// ���ԭ��������Ļ�����ʾ��������
	if ( strlen ( g_SystemOption.SMSStatus[iSelectedItem].szPassword ) > 0 )
	{
		char password[PASSWORDLENGTH];
		strcpy ( password,g_SystemOption.SMSStatus[iSelectedItem].szPassword );
		if ( InputPassword ( password,
			              PASSWORDLENGTH,1 ) == IDCANCEL )
						  return;
	
	}

	bIsSetting = TRUE;
	if ( m_pSetNamePassword == NULL )
		m_pSetNamePassword = new CSetNamePassword;

	m_pSetNamePassword->SetSettingStyle ( 0 );////��������
	m_pSetNamePassword->SetItem ( iSelectedItem );
	if ( iSelectedItem == 3 )
		m_pSetNamePassword->SetMailBoxFlag ( TRUE );
	else m_pSetNamePassword->SetMailBoxFlag ( FALSE );
	m_pSetNamePassword->Create ( "�趨��������",
						WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD|WS_OKBOX,
						CRect ( 0,0,159,159 ),
						HELP_SMS_BOX_SETUP_DETAIL );


	m_pSetNamePassword->ShowWindow ( );
		
	
}
///// ��ʼ����Ϣ
BOOL CSetLetterBox::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;

	/////// INIT here
	m_pListView.Create ( CRect ( m_sRect.left,
								m_sRect.top + WND_TITLE_HEIGHT,
								159,159 ),this,
								SETLETTERBOX_LISTVIEW );

	if ( g_SystemOption.SMSStatus[0].iLockType == 0 )
		m_pListView.AppendItem ( g_SystemOption.SMSStatus[0].szName,
			ICON_SMS1 );
	else m_pListView.AppendItem ( g_SystemOption.SMSStatus[0].szName,
			ICON_SMS3 );

	if ( g_SystemOption.SMSStatus[1].iLockType == 0 )
		m_pListView.AppendItem ( g_SystemOption.SMSStatus[1].szName,
			ICON_SMS1 );
	else m_pListView.AppendItem ( g_SystemOption.SMSStatus[1].szName,
			ICON_SMS3 );

	if ( g_SystemOption.SMSStatus[2].iLockType == 0 )
		m_pListView.AppendItem ( g_SystemOption.SMSStatus[2].szName,
			ICON_SMS1 );
	else m_pListView.AppendItem ( g_SystemOption.SMSStatus[2].szName,
			ICON_SMS3 );

	m_pListView.AppendItem ( g_SystemOption.SMSStatus[3].szName,
		ICON_SMS0 );	

	return TRUE;
}
///// ������Ϣ
BOOL CSetLetterBox::OnDestroy ( )
{

	////// codes

	return CAppWnd::OnDestroy ( );
}
//// �ػ���Ϣ
void CSetLetterBox::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );
	//// Add your codes
	////////////�޸�ListView��ͼ��״̬
	if ( bIsSetting )
	{
		bIsSetting = FALSE;

		for ( int i = 0;i < 4;i ++ )
		{
			CListItem oldListItem;
			m_pListView.GetItem ( &oldListItem,i );
			
			if ( i == 3 )
				oldListItem.nLageIcon = ICON_SMS0;
			else oldListItem.nLageIcon = ICON_SMS1 + g_SystemOption.SMSStatus[i].iLockType;

			strcpy ( oldListItem.strText,g_SystemOption.SMSStatus[i].szName );
			m_pListView.ModifyItem ( &oldListItem,i );
		}

		
	}

}
///////////////////////////   class CDXXWnd
BEGIN_MESSAGE_MAP(CDXXWnd, CAppWnd) 
	ON_MESSAGE(DXX_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage
END_MESSAGE_MAP


CDXXWnd::CDXXWnd()
{
	/////��ʼ������

	m_pLetterBoxWnd = NULL;
	bIsDisplay = TRUE;////��һ�ν���������Ҫ��ʾ�����е�δ���ż�
	m_pMailBoxInfo = NULL;
}

CDXXWnd::~CDXXWnd()
{
	
}

void CDXXWnd::OnLVCommand ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
	UINT32 uiItem = (UINT32)lParam;
	iCurrentLetterBox = uiItem;	
	
	
	//////˽������
	if ( g_SystemOption.SMSStatus[uiItem].iLockType == 2 )////��Ҫ����
	{
		char szTemp[PASSWORDLENGTH];
		strcpy ( szTemp,g_SystemOption.SMSStatus[uiItem].szPassword );
		if ( InputPassword ( szTemp,
						PASSWORDLENGTH,1 ) == IDOK )
		{
			g_SystemOption.SMSStatus[uiItem].iLockType = 1;///�Ѿ�����
			CListItem NewItem;
			m_pDXXListView.GetItem ( &NewItem,uiItem );
			if ( uiItem == 0 )
				NewItem.nLageIcon = ICON_SMS1 + g_SystemOption.SMSStatus[uiItem].iLockType;
			else if ( uiItem == 1 )
				NewItem.nLageIcon = ICON_SMS1 + g_SystemOption.SMSStatus[uiItem].iLockType;
			else if ( uiItem == 2 )
				NewItem.nLageIcon = ICON_SMS1 + g_SystemOption.SMSStatus[uiItem].iLockType;

			m_pDXXListView.ModifyItem ( &NewItem,uiItem );
		
		}else return;


	}

	///////�������䴰��
	if(!m_pLetterBoxWnd)
		m_pLetterBoxWnd = new CLetterBoxWnd;

	m_pLetterBoxWnd->SetFlag ( TRUE );
	m_pLetterBoxWnd->Create( g_SystemOption.SMSStatus[uiItem].szName, 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_BOX_DETAIL );
	
	m_pLetterBoxWnd->ShowWindow ( );

}

//////WM_CREATE Message 
BOOL CDXXWnd::OnCreate ( )
{

	if ( !CAppWnd::OnCreate ( ) ) return FALSE;


	m_pDXXListView.Create ( CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT,
								159,159 ),this,DXX_LISTVIEW );


	/////����ͼ��1��״̬
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[0].szName,
		ICON_SMS1 + g_SystemOption.SMSStatus[0].iLockType );

	////����ͼ��2��״̬
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[1].szName,
		ICON_SMS1 + g_SystemOption.SMSStatus[1].iLockType );
	///����ͼ��3��״̬
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[2].szName,
		ICON_SMS1 + g_SystemOption.SMSStatus[2].iLockType );
	
	///////���ù�������
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[3].szName,
		ICON_SMS0 );

	return TRUE;
}


void CDXXWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );
	/////// �޸��б��ͼ��״̬
	
	if ( bIsDisplay ) ///�Ƿ���ʾ
	{
		bIsDisplay = FALSE;
		int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////���Ҽ�¼		
		UINT8 iNumber[4] = {0,0,0,0};
		int i;
		
		for ( i = 0;i < r;i ++ )
		{
			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///�õ���¼ָ��
			if ( lpST->iLetterStatus != ICON_LETTERREADED &&
				 lpST->iChildMailBox == 0 &&
				 lpST->bReadStatus != 1 )
				iNumber[lpST->bMailBox] ++;
		}

		
		if ( iNumber[0] || 
			 iNumber[1] ||
			 iNumber[2] ||
			 iNumber[3] )
		{
			
			if ( !m_pMailBoxInfo )
				m_pMailBoxInfo = new CDisplayInformation;
			
			m_pMailBoxInfo->SetStyle ( 0 );
			m_pMailBoxInfo->SetLetterNumber ( iNumber );
			m_pMailBoxInfo->Create ( "������Ϣ",
				WS_CHILD | WS_VISIBLE,
				CRect ( 5,45,146,143 ) ,0 );

			m_pMailBoxInfo->ShowWindow ( );

		}
	
	}

}



BOOL CDXXWnd::OnDestroy ( )
{
	bIsDisplay = TRUE;////�ָ��ż����ѹ���
	///////////�ָ�����״̬
	for ( int i = 0;i < 4;i ++ )
		if ( g_SystemOption.SMSStatus[i].iLockType  == 1 )
			g_SystemOption.SMSStatus[i].iLockType = 2;

	if ( m_pLetterBoxWnd != NULL )
	{
		delete m_pLetterBoxWnd;
		m_pLetterBoxWnd = NULL;
	}

	
	if ( m_pMailBoxInfo )
	{
		delete m_pMailBoxInfo;
		m_pMailBoxInfo = NULL;
	}
	
	return CAppWnd::OnDestroy ( );

}

/////////////////////// class CChoiceMoneyWnd
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CChoiceMoneyWnd, CAppWnd) 
	ON_MESSAGE(CHOICEMONEY_LISTVIEW, CN_COMMAND , OnLVCommand) ///����ListView
END_MESSAGE_MAP

CChoiceMoneyWnd::CChoiceMoneyWnd ( )
{

	m_pMoney			= NULL;

}

CChoiceMoneyWnd::~CChoiceMoneyWnd ( )
{


}

BOOL CChoiceMoneyWnd::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;
	//////// TODO: your codes
	///////////����ListView�ؼ�
	m_ListView.Create ( CRect ( m_sRect.left,
								 m_sRect.top + WND_TITLE_HEIGHT ,
								 159,159 ),
								 this,
								 CHOICEMONEY_LISTVIEW );

	
	m_ListView.AppendItem ( "��",
			ICON_MONEY  );
	
	m_ListView.AppendItem ( "��ϸ",
			ICON_MONEYEX );
	
	return TRUE;
}

BOOL CChoiceMoneyWnd::OnDestroy ( )
{
	////////TODO: Your codes
	
	//////���ٻ�����ʾ����
	if ( m_pMoney )
	{
		delete m_pMoney;
		m_pMoney = NULL;
	}

	return CAppWnd::OnDestroy ( );
}

void CChoiceMoneyWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );

	/////// TODO: codes
}

void CChoiceMoneyWnd::OnLVCommand ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{
	UINT32 uItem = ( UINT32 )lParam;
	
	switch ( uItem )
	{
	case 0: ////�򵥻���
		ShowMoney  ( 0 );
		break;
	case 1:
		ShowMoney  ( 1 );
		break;
	}
}

///////�����˵�
void CChoiceMoneyWnd::ShowMoney ( UINT8 iStyle )
{

	if ( !m_pMoney )
		m_pMoney = new CMoneyWnd;

	if ( iStyle == 0 ){ ////�򵥻���
	
		m_pMoney->SetStyle ( 0 );
		m_pMoney->Create("�򵥻�����Ϣ", 
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
					CRect(0 ,  0, 159 , 159) , 
					HELP_SMS_MONEY_DETAIL );
	}else {  ////��ϸ����
	
		m_pMoney->SetStyle ( 1 );
		m_pMoney->Create("��ϸ������Ϣ", 
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
					CRect(0 ,  0, 159 , 159) , 
					HELP_SMS_MONEY_DETAIL );

	}

	m_pMoney->ShowWindow ( );

}

////////////////////////  class HelloWorld 
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CHelloWorld, CAppWnd) 
	ON_MESSAGE(SM_MAININTERFACE_LISTVIEW, CN_COMMAND , OnLVCommand) ///����ListView
END_MESSAGE_MAP


CHelloWorld::CHelloWorld ( )
{
	m_pDXXWnd			= NULL; //////��ʼ��
	m_pSETWnd			= NULL;
	m_pSubscribeWnd		= NULL;
	m_pChoiceMoney		= NULL;
	m_pNewPhone			= NULL;
}

CHelloWorld::~CHelloWorld ( )
{

}

//////////////��Ϣ���̺���
void CHelloWorld::OnLVCommand ( UINT32 nMessage,UINT32 wParam,UINT32 lParam )
{

	UINT32 uItem = ( UINT32 )lParam;

	switch ( uItem )
	{
	
	case 0:  /////����Ϣ
		
		g_iServerType = 1;/////���÷�������Ϊ����Ϣ
		if ( m_pDXXWnd == NULL )
			m_pDXXWnd = new CDXXWnd;
		m_pDXXWnd->Create("����", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_BOX );
		
		
		m_pDXXWnd->ShowWindow();
		break;
	case 1://////������
		
		ModelCall = WYCALL;
		if ( m_pMODELWnd == NULL )
			m_pMODELWnd = new CMODELWnd;
		m_pMODELWnd->Create("������ ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , HELP_SMS_COLLECTION); 
		m_pMODELWnd->ShowWindow();
		break;
	case 2:////Ⱥ��
		g_bIsGroup = TRUE;
		GroupCall = WYCALL;
		if ( m_pGRPSDWnd == NULL )
			m_pGRPSDWnd = new CGRPSDWnd;
		m_pGRPSDWnd->Create("Ⱥ��", 
						 WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
						 CRect(0 ,  0, 159 , 159) , 
						 HELP_SMS_SEND_GROUP ); 
		m_pGRPSDWnd->ShowWindow();
			break;
	case 3:////�Ҽ�e
		/////����Դ
		if ( TestLowPower ( ) )
		{
			MessageBox ( "����ӵ�Դ���޷����ӣ�",
				"����",MB_OK|MB_ICONSTOP ); 
			return;
		}

		bIsSuccessSend = TRUE;
		g_iServerType = 0;///����
		if ( m_pService == NULL )
			m_pService = new CSMSServiceWnd;
		m_pService->Create("������Ϣ����...", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_JIAJIAE );
				
		m_pService->ShowWindow();
		break;
	case 4: ////e��Ϣ
		if ( m_pSubscribeWnd == NULL )
			m_pSubscribeWnd = new CSubscribeSMS;

		m_pSubscribeWnd->Create("���ĵĶ���Ϣ", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_ESMS);
				
		m_pSubscribeWnd->ShowWindow();
		break;
	case 5:////�绰��ҵ��
		if ( !m_pNewPhone )
			m_pNewPhone = new CNewPhone;
		
		m_pNewPhone->Create("�绰��ҵ��", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_NEW );

		m_pNewPhone->ShowWindow ( );

		break;
	case 6:  ///////���Ѳ�ѯ
		if ( !m_pChoiceMoney )
			m_pChoiceMoney = new CChoiceMoneyWnd;

		m_pChoiceMoney->Create("��������ѡ��", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				0 );

		m_pChoiceMoney->ShowWindow ( );
		
		break;
	case 7: /////����
		
		if ( m_pSETWnd == NULL )
			m_pSETWnd = new CSETWnd;
		m_pSETWnd->Create("����", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_SETUP );
				
		m_pSETWnd->ShowWindow();
		break;

	}
	
}

/////////////////ͨ�ú���
BOOL CHelloWorld::InitSMMainInterface ( )
{
	if(!CAppWnd::OnCreate())
		return FALSE;

	///////////����ListView�ؼ�
	m_MainInterfaceListView.Create ( CRect ( m_sRect.left,
											 m_sRect.top + WND_TITLE_HEIGHT ,
											 159,159 ),
											 this,
											 SM_MAININTERFACE_LISTVIEW );

	char *szLVTitle[] = {
			"����",
			"������",
			"Ⱥ��",
			"�Ҽ�e",
			"e��Ϣ",
			"��ҵ��",
			"����",
			"����"
		};

	for ( int i = 0;i < 8; i ++ )
	{
		m_MainInterfaceListView.AppendItem ( szLVTitle[i],
			ICON_SMMAINLV01 + i );
	}



	return TRUE;

}

BOOL CHelloWorld::OnDestroy()
{
	
	/////���ٳ�������á�Ⱥ��
	if ( m_pMODELWnd )
	{
		delete m_pMODELWnd;
		m_pMODELWnd  = NULL;
	}
	
	if ( m_pGRPSDWnd )
	{
		delete m_pGRPSDWnd ;
		m_pGRPSDWnd = NULL;
	}


	if ( m_pSETWnd )
	{
		delete m_pSETWnd;
		m_pSETWnd = NULL;
	}

	/////syj
	if ( m_pDXXWnd != NULL )
	{
		delete m_pDXXWnd;
		m_pDXXWnd = NULL;
	}

	/////���ټҼ�e����
	if ( m_pService != NULL )
	{
		delete m_pService;
		m_pService = NULL;
	}


	///// �����Ķ�����
	if ( m_pReadingLetter != NULL )
	{
		delete m_pReadingLetter;
		m_pReadingLetter = NULL;
	}

	//// �����޸Ĵ���
	if ( m_pModifyLetter != NULL )
	{
		delete m_pModifyLetter;
		m_pModifyLetter = NULL;

	}

	/// Codes
	if ( m_pSaveAsWnd )
	{
		delete m_pSaveAsWnd;
		m_pSaveAsWnd = NULL;
	}


	///////������Ϣ��
	if ( g_pShowMsg )
		g_pShowMsg->Destroy ( );


	////����ͼ����ʾ��
	if ( g_pGraphicsWnd ){
		
		delete g_pGraphicsWnd;
		g_pGraphicsWnd = NULL;
	
	}

	//////���ٻ���ѡ�񴰿�
	if ( m_pChoiceMoney )
	{
		delete m_pChoiceMoney;
		m_pChoiceMoney = NULL;
	}

	////������ҵ�񴰿�
	if ( m_pNewPhone )
	{
		delete m_pNewPhone;
		m_pNewPhone = NULL;
	}

	///////֪ͨ��·��һ�
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);  
	g_pL23MsgBuffer[2] = 0x00;
	SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 ); 
	CAppWnd::OnDestroy();
	::SetCurrentApp();
	return TRUE;
	
}


void CHelloWorld::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	/////����ȱʡ����Ϊ��������
	iCurrentLetterBox = 3;
}


BOOL CHelloWorld::OnCreate()
{
		
	BOOL bReturn = InitSMMainInterface ( );

	return bReturn;
}


void SMAppRun()
{
	CDC		dc;
	if(g_SystemOption.nSmsInitFlag!= 0x55AA00FF){ // hqf
		CRect r2(24,74,159,94);
		dc.EraseRect(r2);
		dc.FrameRect(r2);
		dc.TextOut(32,76,"����װ������...");
		InitSMSApp ( );
		g_SystemOption.nSmsInitFlag = 0x55AA00FF;
		SaveSystemOption ( &g_SystemOption );
	}
	//// ��ʼ������Ϣ������
	if(!g_pHelloWorld)
		g_pHelloWorld = new CHelloWorld;
	::SetCurrentApp(APPID_SMS);
	g_pHelloWorld->Create("����Ϣ����", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX, 
			CRect(0 ,  0, 159 , 159) , HELP_SMS_MAIN );
	

	g_pHelloWorld->ShowWindow();
	
	
}
////////////////////////////////////////////////////////////////
///// class CReadingSMS
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CReadingSMS, CAppWnd) 
	ON_MESSAGE ( READING_SHOWMENU,CN_COMMAND,OnShowMenu ) ////��ʾ�˵�
	ON_MESSAGE ( READING_DEL,CN_COMMAND,OnDel ) ////ɾ��
	ON_MESSAGE ( READING_ANSWER,CN_COMMAND,OnAnswer ) //�ظ��ż�
	ON_MESSAGE ( READING_SENDTO,CN_COMMAND,OnSendTo ) //ת��
	ON_MESSAGE ( READING_SAVEAS,CN_COMMAND,OnSaveAs )//���Ϊ 
	ON_MESSAGE ( READING_SHOWPHONE,CN_COMMAND,OnShowPhone ) //��ʾ�绰����
END_MESSAGE_MAP

CReadingSMS::CReadingSMS ( )
{
	m_pShowPhone = NULL;
}

CReadingSMS::~CReadingSMS ( )
{
	
}
//////��ʾ����
void CReadingSMS::OnShowPhone ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	if ( !m_pShowPhone )
		m_pShowPhone = new CDisplayInformation;

	m_pShowPhone->SetStyle ( 1 );//������ʾ
	m_pShowPhone->SetPhone ( m_pst.szPhone );
	/////���㴰�ڸ߶�
	int len = strlen ( m_pst.szPhone );
	int iNum = len/16;
	if ( 16*iNum < len )
		iNum ++;

	int iHeight = 16*iNum + 8;
	m_pShowPhone->Create ( "�绰����",
			WS_CHILD | WS_VISIBLE,
			CRect ( 10,30,146,iHeight+30 ) ,0 );

	m_pShowPhone->ShowWindow ( );

}
////���Ϊ
void CReadingSMS::OnSaveAs ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	////// ��ʾ���Ϊ���ô���
	g_ST = m_pst;
	
	if ( m_pSaveAsWnd == NULL )
		m_pSaveAsWnd = new CSaveAsWnd;
	m_pSaveAsWnd->Create ( "ѡ��Ŀ������",
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_OKBOX,
		CRect ( 0,0,159,159 ) );
	m_pSaveAsWnd->ShowWindow ( );

}
/////ת��
void CReadingSMS::OnSendTo ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

	 //////���ô��ڵ�һЩ����
	 g_bIsModel = FALSE;
	 g_ST = m_pst;
	 strcpy ( g_ST.szPhone,"�����������" );
	 m_pModifyLetter->SetFlag ( false ); 
	 m_pModifyLetter->Create("д����Ϣ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_WRITE);
			

	m_pModifyLetter->ShowWindow ( );

	
}
/////ɾ��
void CReadingSMS::OnDel ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	//////////////ɾ����¼
	if ( MessageBox ( "ȷ��ɾ���ü�¼��","ɾ��",
		MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
		bool b = DeleteSMS ( m_iDataID );
		
		Destroy ( );
	}
	
}
/////��ʾ�˵�
void CReadingSMS::OnShowMenu ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	////////���˵���ʾ����
	m_Menu.PopupMenu ( m_sRect.left + 1,
					m_sRect.top + WND_TITLE_HEIGHT  );
}
////////��ʼ����Ϣ
BOOL CReadingSMS::OnCreate ( )
{
	/////�ر�����
	StopRing();	


	if ( !CAppWnd::OnCreate ( ) )
		return FALSE;
	int i;
	///// Codes
	/////////////�����˵����ť
	CRect rc( m_sRect.left,
			m_sRect.top ,
			m_sRect.left + 76,
			m_sRect.top+ SMS_APP_TITLE_HEIGHT  );

	//////////////////ת���ռ����ַ�
	char szTemp[9];
	if ( strlen ( m_pst.szPhone ) > 8 )
	{
		for ( i = 0;i < 8;i ++ )
			szTemp[i] = m_pst.szPhone[i];
		szTemp[i] = '\0';
		m_ShowMenuBtn.Create ( szTemp,rc,this,READING_SHOWMENU );

	}else m_ShowMenuBtn.Create ( m_pst.szPhone,rc,
								this,READING_SHOWMENU );

	///////�����ظ���ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	
	m_AnswerBtn.Create ( "�ظ�",rc,this,READING_ANSWER );
	///////����ɾ����ť 
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;

	m_DelBtn.Create ( "ɾ��",rc,this,READING_DEL );

	/////////////������ʾ�����绰���밴ť
	rc.left = m_sRect.right - 17;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 1;
	rc.right = m_sRect.right;
	rc.bottom = rc.top + 16;
	m_ShowPhone.Create ( "��",rc,this,READING_SHOWPHONE );
	///////��������Ϣ������
	rc.left = m_sRect.left + 1;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 41;
	rc.right = 159;
	rc.bottom = 159;/////SYJ modisy
	m_Content.Create ( m_pst.szMsg,rc,this,WS_CHILD|WS_VISIBLE,
		READING_CONTENT,SMS_CONTENT_MAX_NUMBER );

	////////////////�����˵�
	m_Menu.Create ( this );
	m_Menu.AppendItem ( "ת��",READING_SENDTO );
	m_Menu.AppendItem ( "-",0 );
	m_Menu.AppendItem ( "���Ϊ",READING_SAVEAS );

	return TRUE;
}
////////������Ϣ
BOOL CReadingSMS::OnDestroy ( )
{
	/// Codes
	if ( m_pShowPhone )
	{
		delete m_pShowPhone;
		m_pShowPhone = NULL;
	}

	return CAppWnd::OnDestroy ( );
}

/////�ػ���Ϣ
void CReadingSMS::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );


	//// Your codes
	dc.SetBackColor ( );/////���ñ�����ɫΪ��
	dc.TextOut ( m_sRect.left,
		m_sRect.top + WND_TITLE_HEIGHT + 2,"������:" );

	///////����ռ�������
	dc.TextOut ( 62,
		m_sRect.top + WND_TITLE_HEIGHT + 2,m_szName );

	dc.SetLineStyle ( 1 );
	dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 20 );
	dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 20 );


	
	dc.TextOut ( m_sRect.left,
				 m_sRect.top + WND_TITLE_HEIGHT + 22,
				 m_pst.szTime );


	dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 40 );
	dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 40 );

	
	dc.SetLineStyle (  );
}

//////////////�ظ���Ϣ
void CReadingSMS::OnAnswer ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

	 //////���ô��ڵ�һЩ����
	 g_ST = m_pst;
	 g_bIsModel = FALSE;
	 m_pModifyLetter->SetFlag ( false ); 
	 m_pModifyLetter->Create("д����Ϣ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_WRITE );
			

	m_pModifyLetter->ShowWindow ( );

}

//////////////////// class LetterBoxWnd 
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CLetterBoxWnd, CAppWnd) 
	ON_MESSAGE ( LETTERBOX_NEW_BTN, CN_COMMAND , OnNew) ///��ʾ�޸Ĵ���
	ON_MESSAGE ( LETTERBOX_REV_BTN, CN_COMMAND, OnRevBtn ) ///���հ�ť
	ON_MESSAGE ( LETTERBOX_SEND_BTN,CN_COMMAND,OnSendBtn ) ///���Ͱ�ť
	ON_MESSAGE ( LETTERBOX_SENDED_BTN,CN_COMMAND,OnSendedBtn )//�ѷ���ť
	ON_MESSAGE ( LETTERBOX_GRIDVIEW,CN_GRIDSEL,OnGridCmd ) /////������Ϣ
	ON_MESSAGE ( LETTERBOX_DELETE_BTN,CN_COMMAND,OnDelete ) ////ɾ����Ϣ
END_MESSAGE_MAP


CLetterBoxWnd::CLetterBoxWnd()
{

	////////////// Init
//	m_pGridView = NULL;
	
}

CLetterBoxWnd::~CLetterBoxWnd()
{

}

//////ɾ����Ϣ����
void CLetterBoxWnd::OnDelete ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	/////ɾ��ѡ�е�������
	if ( MessageBox ( "ȷ�����������ż�ȫ��ɾ����","����",
		      MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
	
		int i;
		if ( iCurrentChildLetterBox != 2 ) {
		
			int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////���Ҽ�¼			
			////////�������ҷ��������ļ�¼����ɾ��
			for ( i = 0;i < r;i ++ )
			{
			
				LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///�õ���¼ָ��
				if ( lpST->bMailBox == iCurrentLetterBox && /////�ж������Ƿ����ڵ�ǰѡ������
					lpST->iChildMailBox == iCurrentChildLetterBox ) 
				{
				
					if ( !lpST->iLockStatus  ) ////û�м���������ɾ��
						DeleteSMS ( g_pSMSDataIDBuf[i] );
				
				}				
			}
		}


		if ( iCurrentChildLetterBox == 2 ) ///////////�ѷ���������ڴ���Flash������
		{
			for ( i = 0;i < SMS_MAX_SAVE_NUMBER;i ++ )			
				if ( !g_SendedSMS[iCurrentLetterBox][i].iLockStatus )//����ɾ��
				{
					
					if ( g_SendedSMS[iCurrentLetterBox][i].bIsFlash )
					{
						DeleteSMS ( g_SendedSMS[iCurrentLetterBox][i].dataID );
						g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem --;					
					}

					memset ( &g_SendedSMS[iCurrentLetterBox][i],0,sizeof ( SMS ) );
				
				}


			/////���ڴ��еļ�¼��������
			int pos = 0;
			for ( i = 0;i < SMS_MAX_SAVE_NUMBER;i ++ )
			{
			
				if ( g_SendedSMS[iCurrentLetterBox][i].iLockStatus )
				{
					
					if ( pos != i )
					{
						g_SendedSMS[iCurrentLetterBox][pos] = g_SendedSMS[iCurrentLetterBox][i];
						memset ( &g_SendedSMS[iCurrentLetterBox][i],0,sizeof ( SMS ) );
					
					}
					
					pos ++;
				}
			}
			
		}
	
		/////�����Ӵ���
		PaintGrid ( CDC ( ),TRUE  );

	}


}
////////////////������Ϣ����
void CLetterBoxWnd::OnGridCmd ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{


	///////// �ж���ѡ���FLash�ж�ȥ����
	UINT16 iRow =  lParam&0x0000ffff;
	UINT16 iCol =  (lParam>>16)&0x0000ffff;

	char password[PASSWORDLENGTH];
	
	UINT16 pos;
	SMS ss;
	
	if ( iCurrentChildLetterBox == 0 ||
		 iCurrentChildLetterBox == 1 )
	{
		g_CurrentID = g_pSMSDataTempIDBuf[iRow-1] ;
		LPSMS lpST = GetSMSPointer ( g_CurrentID  ); ///�õ���¼ָ��

		ss.Flag = lpST->Flag;
		ss.bMailBox = lpST->bMailBox;/////////////���շ�������
		ss.iChildMailBox = lpST->iChildMailBox;/////������������ı�ʾ
		ss.iLockStatus = lpST->iLockStatus;///����
		ss.iLetterStatus = lpST->iLetterStatus;
		ss.bReadStatus = lpST->bReadStatus;

		strcpy ( ss.szPhone,lpST->szPhone );//////���5���绰����
		strcpy ( ss.szTime,lpST->szTime );///////ʱ��
		strcpy ( ss.szMsg,lpST->szMsg );//////����Ϣ�������140��char
					/////����70������
		
			
	}else {
	
		pos = g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber - iRow;
		g_iCurrentPos = pos;///���������±�
		ss.Flag = g_SendedSMS[iCurrentLetterBox][pos].Flag;
		ss.bMailBox = g_SendedSMS[iCurrentLetterBox][pos].bMailBox;/////////////���շ�������
		ss.iChildMailBox = g_SendedSMS[iCurrentLetterBox][pos].iChildMailBox;/////������������ı�ʾ
		ss.iLockStatus = g_SendedSMS[iCurrentLetterBox][pos].iLockStatus; //Lock status
		ss.iLetterStatus = g_SendedSMS[iCurrentLetterBox][pos].iLetterStatus;
		ss.bReadStatus = g_SendedSMS[iCurrentLetterBox][pos].bReadStatus;

		ss.dataID = g_SendedSMS[iCurrentLetterBox][pos].dataID;
		strcpy ( ss.szPhone,g_SendedSMS[iCurrentLetterBox][pos].szPhone );//////���5���绰����
		strcpy ( ss.szTime,g_SendedSMS[iCurrentLetterBox][pos].szTime );///////ʱ��
		strcpy ( ss.szMsg,g_SendedSMS[iCurrentLetterBox][pos].szMsg );//////����Ϣ�������140��char
					/////����70������
	}


	if ( iCol == 1 )
	{
		/////ȷ������			
		if ( iCurrentChildLetterBox == 0 ||
			iCurrentChildLetterBox == 1 )
		{

			if ( ss.iLockStatus )
			{
				ss.iLockStatus = 0;///����
				ss.iLetterStatus = ICON_LETTERUNLOCK;
			
			}else {
			
				ss.iLockStatus = 1;///����
				ss.iLetterStatus = ICON_LETTERLOCK; ///////�ż����Ķ�״̬
			
			}

			
			///////���仯�ļ�¼д��Flash
			bool b = ModifySMS ( g_CurrentID,&ss );
		
		
		}else {
		
			
			/////����������ѡ�е��ż����浽Flash
			/////���ü���״̬ 
			if ( ss.iLockStatus )
			{
				ss.iLockStatus = 0;///����
				g_SendedSMS[iCurrentLetterBox][pos].iLockStatus = 0;///����
				ss.iLetterStatus = ICON_LETTERUNLOCK;
				g_SendedSMS[iCurrentLetterBox][pos].iLetterStatus = ICON_LETTERUNLOCK; ///////�ż����Ķ�״̬
				g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem --;

			}else {
			
				
				if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem>
					SMS_MAX_HAVEPASSWORD_NUMBER )
				{
					MessageBox ( "����ʧ�ܡ�","����",MB_OK );
					return;
				}

				ss.iLockStatus = 1;///����
				g_SendedSMS[iCurrentLetterBox][pos].iLockStatus = 1;///����
				ss.iLetterStatus = ICON_LETTERLOCK; ///////�ż����Ķ�״̬
				g_SendedSMS[iCurrentLetterBox][pos].iLetterStatus = ICON_LETTERLOCK; ///////�ż����Ķ�״̬

				g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem ++;	
				
			}

			////////����һ����¼
			ss.bIsFlash = 1;//���浽Flash
			g_SendedSMS[iCurrentLetterBox][pos].bIsFlash = 1;//���浽Flash
			if ( ss.dataID == -1 )
			{
				int r = AppendSMS  ( &ss );
				if ( r == -1 ) 
					MessageBox ( "�洢����������","����",MB_OK|MB_ICONSTOP );

				//����¼��ID��д��Flash
				ss.dataID = r;
				g_SendedSMS[iCurrentLetterBox][pos].dataID = r;
				ModifySMS ( r,&ss );

				
			}else {

				ModifySMS ( g_SendedSMS[iCurrentLetterBox][pos].dataID,&ss );
				//ModifySMS ( ss.dataID,&ss );
			
			}

		}
		
		

		//////�ػ�����ؼ�
		m_pGridView.SetUnitIcon ( iRow,iCol,ss.iLetterStatus,TRUE );
		
		return;
	}


	///////�Ķ��ż�
	if ( iCurrentChildLetterBox == 0 ){
	
		///////���仯�ļ�¼д��Flash
		if ( ss.iLetterStatus != ICON_LETTERUNLOCK &&
			 ss.iLetterStatus != ICON_LETTERLOCK )
				ss.iLetterStatus = ICON_LETTERREADED;////�Ѷ�

		ss.bReadStatus = 1;////�Ѷ�
		bool b = ModifySMS ( g_CurrentID,&ss );


		if ( m_pReadingLetter == NULL )
			m_pReadingLetter = new CReadingSMS;
		//////��ʼ���ż�
		m_pReadingLetter->SetSMSInformation ( ss );
		m_pReadingLetter->SetDataID ( g_CurrentID );
		m_pReadingLetter->Create ( "",
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX,
					CRect ( 0,0,159,159 ),0 );
		
		m_pReadingLetter->ShowWindow ( );

		
		
	}else { /////�����䡢�ѷ�����
	
		if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

		//////���ô��ڵ�һЩ����
		g_bIsModel = FALSE;
		g_ST = ss;
		m_pModifyLetter->SetFlag ( true );
		m_pModifyLetter->Create("�¶���Ϣ", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_WRITE );
		

		m_pModifyLetter->ShowWindow ( );

	
	}
	
	
}

////////��������ؼ�
void CLetterBoxWnd::PaintGrid ( CDC &dc ,BOOL bIsDraw )
{
	/////// �ָ���ȱʡ״̬
	int i,r,len,n;
	char sTemp[81];
	iUnreadNumber = 0;


	m_pGridView.RemoveAll ( );
	if ( iCurrentChildLetterBox == 0 )////�ռ���
	{
		g_SystemOption.SMSStatus[iCurrentLetterBox].bRevNumber = 0;

	}else if ( iCurrentChildLetterBox == 1 ){///������
	
		
		g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber = 0;
	
	}else if ( iCurrentChildLetterBox == 2 ) {
		///�ѷ�
		
		g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber = 0;
		//g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem = 0;
	}

	if ( iCurrentChildLetterBox == 2 )  ////ˢ���ѷ�����
	{
		char *szItem[3];
		szItem[0] = new char[16];
		szItem[1] = new char[81];
		szItem[2] = new char[16];
		for ( i = 0;i < SMS_MAX_SAVE_NUMBER;i ++ )
		{
			if ( strlen ( g_SendedSMS[iCurrentLetterBox][i].szPhone ) > 0 )
			{
				strcpy ( szItem[0], "" );
				
				////����ʱ��
				strcpy ( sTemp,"" );
				if ( ( len = strlen ( g_SendedSMS[iCurrentLetterBox][i].szTime ) ) > 5 ){
					
					for ( n = 0;n < len;n ++ )
						if ( g_SendedSMS[iCurrentLetterBox][i].szTime[n] == 0x20 ) break;

					memcpy ( sTemp,&g_SendedSMS[iCurrentLetterBox][i].szTime[n+1],( len - n -1 )*sizeof ( char ) );
					sTemp[len - n -1] = '\0';
					strcpy ( szItem[2],sTemp );

				}else strcpy ( szItem[2],g_SendedSMS[iCurrentLetterBox][i].szTime );////ʱ��


				////���ñ���
				if ( ( len = strlen ( g_SendedSMS[iCurrentLetterBox][i].szPhone ) ) > 9 ){
				
					for ( n = 0;n < 9;n ++ )
					{
						sTemp[n] = g_SendedSMS[iCurrentLetterBox][i].szPhone[n];
						if ( sTemp[n] == '\0' ||
							 sTemp[n] == '*' ||
							 sTemp[n] == '#' ) 
								 if ( n != 0 ) 
									 break;
					}

					sTemp[n] = '\0';
					strcpy ( szItem[1],sTemp );

				}else strcpy ( szItem[1],g_SendedSMS[iCurrentLetterBox][i].szPhone ); ////������

				char pStyle[]= {0,0,0};
				UINT32 pIconID[]= {g_SendedSMS[iCurrentLetterBox][i].iLetterStatus,0,0};

				if ( strlen ( szItem[1] ) > 0 )
				{
					m_pGridView.InsertRow (1, szItem,pStyle,pIconID );
						
					////����Ӧ���������������
					g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber ++;
				}
				
			}
			
		}
	
		m_pGridView.SetStartRow ( 1 );
		m_pGridView.ScrollTo ( 1 );
		
		if ( bIsDraw ) m_pGridView.OnPaint ( CDC() );


		delete []szItem[0];
		delete []szItem[1];
		delete []szItem[2];
		return ;
	}


	//////////////////////��������	
	r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////���Ҽ�¼
	/*int pos,j;
	short *iTemp = new short[r];
	///In
	for ( j = 0;j < r;j ++ )
		iTemp[j] = g_pSMSDataIDBuf[j];


	for ( i = 0;i < r;i ++ )
		for ( j = i + 1;j < r;j ++ )
		{
			if ( iTemp[i] >iTemp[j] )
			{
				int temp = iTemp[i];
				iTemp[i] = iTemp[j];
				iTemp[j] = temp;
			}

		}

	////////// Out
	for ( j = 0;j < r;j ++ )
		g_pSMSDataIDBuf[j] = iTemp[j];

	
	delete []iTemp;*/

	if ( r != 0 ) { ////�ҵ�����
		
		BOOL  bIsShutOffLED = TRUE;
		for ( i = 0;i < r;i ++ ) /////����
		{
		
			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///�õ���¼ָ��
			if ( lpST->iLetterStatus != ICON_LETTERREADED &&
				 lpST->iChildMailBox == 0 &&
				 lpST->bReadStatus != 1 )
				bIsShutOffLED = FALSE;///�����δ������

			if ( lpST->bMailBox == iCurrentLetterBox && /////�ж������Ƿ����ڵ�ǰѡ������
				lpST->iChildMailBox == iCurrentChildLetterBox ) {
				
				char *szItem[3];
				szItem[0] = new char[16];
				strcpy ( szItem[0] , "" );
				szItem[1] = new char[81];
				szItem[2] = new char[16];
				
				
				////����ʱ��
				strcpy ( sTemp,"" );
				if ( ( len = strlen ( lpST->szTime ) ) > 5 ){
					
					for ( n = 0;n < len;n ++ )
						if ( lpST->szTime[n] == 0x20 ) break;

					memcpy ( sTemp,&lpST->szTime[n+1],( len - n -1 )*sizeof ( char ) );
					sTemp[len - n -1] = '\0';
					strcpy ( szItem[2],sTemp );

				}else strcpy ( szItem[2],lpST->szTime );

				////���ñ���
				if ( ( len = strlen ( lpST->szPhone ) ) > 9 ){
				
					for ( n = 0;n < 9;n ++ )
					{
						sTemp[n] = lpST->szPhone[n];
						if ( sTemp[n] == '\0' ||
							 sTemp[n] == '*' ||
							 sTemp[n] == '#' ) 
							   if ( n != 0 ) break;
					}

					sTemp[n] = '\0';
					strcpy ( szItem[1],sTemp );

				}else strcpy ( szItem[1],lpST->szPhone ); ////������


				char pStyle[]= {0,0,0};
				UINT32 pIconID[]= {lpST->iLetterStatus,0,0};

				if ( lpST->iLetterStatus != ICON_LETTERREADED )
					iUnreadNumber ++;

				m_pGridView.InsertRow (1, szItem,pStyle,pIconID );
				///////�����¼IDֵ
				for ( int j = SMS_MAX_SAVE_NUMBER - 1;j > 0;j -- )
				{
					g_pSMSDataTempIDBuf[j] = g_pSMSDataTempIDBuf[j-1];
					
				}
				
				////����¼��ID�ű��浽��ʱ������
				g_pSMSDataTempIDBuf[0] = g_pSMSDataIDBuf[i];

				////����Ӧ���������������
				if ( iCurrentChildLetterBox == 0 )////�ռ���
					g_SystemOption.SMSStatus[iCurrentLetterBox].bRevNumber ++;
				else if ( iCurrentChildLetterBox == 1 )///������
					g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber ++;
				//else if ( iCurrentChildLetterBox == 2 ) ///�ѷ�
				//	g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber ++;


				delete []szItem[0];
				delete []szItem[1];
				delete []szItem[2];
			}
		}
	
	
	if ( bIsShutOffLED ) ClearLED ( LED_SMS ); /// Shut off LED
	
	}

	m_pGridView.SetStartRow ( 1 );
	m_pGridView.ScrollTo ( 1 );
	
	if ( bIsDraw ) m_pGridView.OnPaint ( CDC() );
}
//////////////���հ�ť
void CLetterBoxWnd::OnRevBtn ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	
	iCurrentChildLetterBox = 0;
	m_RevBtn.SetSelected ( TRUE );
	m_SendBtn.SetSelected ( FALSE );
	m_SendedBtn.SetSelected ( FALSE );

	PaintGrid ( CDC(),FALSE );///�ػ�
	PaintChildWnd ( );
	
	 
}
/////////���Ͱ�ť
void CLetterBoxWnd::OnSendBtn ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	iCurrentChildLetterBox = 1; 
	
	m_RevBtn.SetSelected ( FALSE );
	m_SendBtn.SetSelected ( TRUE );
	m_SendedBtn.SetSelected ( FALSE );
	
	PaintGrid ( CDC(),FALSE );///�ػ�
	PaintChildWnd ( );
	
}

/////////�ѷ��Ͱ�ť
void CLetterBoxWnd::OnSendedBtn ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	iCurrentChildLetterBox = 2;
	m_RevBtn.SetSelected ( FALSE );
	m_SendBtn.SetSelected ( FALSE );
	m_SendedBtn.SetSelected ( TRUE );

	
	PaintGrid ( CDC(),FALSE );///�ػ�
	PaintChildWnd ( );
	
}

void CLetterBoxWnd::OnNew ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{

	if ( m_pModifyLetter == NULL )
		m_pModifyLetter = new CModifyLetterWnd;
	
	g_bIsModel = FALSE;
	strcpy ( g_ST.szPhone,"�����������" );
	strcpy ( g_ST.szMsg,"<�����������������>" );
	m_pModifyLetter->SetFlag ( false ); 
	g_CurrentID = -1;////��ʼ��ȫ������IDֵ

	m_pModifyLetter->Create("д����Ϣ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_WRITE );
	

	m_pModifyLetter->ShowWindow ( );

	
}




BOOL CLetterBoxWnd::OnCreate ( )
{

	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;

	iCurrentChildLetterBox = 0;////����ȱʡ����
	//////�����½���ť
	CRect rc ( m_sRect.left + 78,
		       0,
			   m_sRect.left + 110,
			   SMS_APP_TITLE_HEIGHT );

	m_NewBtn.Create ( "׫д",rc,this,LETTERBOX_NEW_BTN );

	///////����ȫɾ����ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	m_DeleteAll.Create ( "ȫɾ",rc,this,LETTERBOX_DELETE_BTN );
	/////�������հ�ť
	rc.left = m_sRect.left ;
	rc.right = rc.left + 48;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 1;
	rc.bottom = rc.top + SMS_APP_TITLE_HEIGHT;

	m_RevBtn.Create ( "������",rc,this,LETTERBOX_REV_BTN );
	m_RevBtn.SetSelected ( TRUE );
	////�������Ͱ�ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 48;
	
	m_SendBtn.Create ( "������",rc,this,LETTERBOX_SEND_BTN );
	m_SendBtn.SetSelected ( FALSE );
	/////�����Ѿ����͵İ�ť
	rc.left = rc.right + 1;
	rc.right = 158;
	m_SendedBtn.Create ( "�ѷ���",rc,this,LETTERBOX_SENDED_BTN );
	m_SendedBtn.SetSelected ( FALSE );

	////����ListView�ؼ�
//	if ( m_pGridView == NULL )
//		m_pGridView = new CGrid;
	m_pGridView.Create ( this,CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT + 21,
								142,160 ),
			UNS_TEXT|WS_CHILD|TBS_SPACELINE,
			0,3,
			LETTERBOX_GRIDVIEW );

	UINT16 iWidthArray[] = {18,80,40};

	m_pGridView.SetColsWidth ( iWidthArray ,TRUE );////�����п�


	return TRUE;
}

BOOL CLetterBoxWnd::OnDestroy ( )
{
	return CAppWnd::OnDestroy();
}	


void CLetterBoxWnd::OnPaint ( CDC &dc )
{
	/////////////////��Ѱ���ݿ⣬�����б�
	PaintGrid ( dc ,FALSE );
	//////////////// Paint
	CAppWnd::OnPaint ( dc );
	
	//////////////����
	if ( m_bIsNeedReDraw )
	{
		m_bIsNeedReDraw = FALSE;

		///////�����û���������
		if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bRevNumber > 0.8*SMS_MAX_SAVE_NUMBER )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( "�뼰ʱ����������" );
			NU_Sleep ( 300 );
			
		}


		if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber > 0.8*SMS_MAX_SAVE_NUMBER )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( "�뼰ʱ��������" );
			NU_Sleep ( 300 );
			
		}

		

		if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber > 0.8*SMS_MAX_SAVE_NUMBER )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( "�뼰ʱ�����ѷ�����" );
			NU_Sleep ( 300 );
			
		}

		if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

		//�����û���δ���ż�
		/*if ( iUnreadNumber > 0 )
		{
			char temp[40];
			sprintf ( temp,"������������%d��δ���ż�",iUnreadNumber );

			g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( temp );
			NU_Sleep ( 300 );
			g_pShowMsg->ShowWindow ( SW_HIDE );
			
			
		}*/

	}

}

////////////////// class ModifyLetterWnd 
//////////////��Ϣӳ��
BEGIN_MESSAGE_MAP(CModifyLetterWnd, CAppWnd) 
	ON_MESSAGE(MODIFYLETTER_SEND_BTN, CN_COMMAND , OnSend)///����
	ON_MESSAGE ( MODIFYLETTER_PHONE,CN_COMMAND,OnComboxInput )//����绰
	ON_MESSAGE ( MODIFYLETTER_EDIT,CN_COMMAND,OnEditInput )//����绰
	ON_MESSAGE ( MODIFYLETTER_MENU_BTN,CN_COMMAND,OnShowMenu ) ////��ʾ�˵�
	ON_MESSAGE ( MODIFYLETTER_WORD,CN_COMMAND,OnSelectWord ) ///ѡ������
	ON_MESSAGE ( MODIFYLETTER_SAVEAS,CN_COMMAND,OnSaveAs ) ///��浽
	ON_MESSAGE ( MODIFYLETTER_SAVE_BTN,CN_COMMAND,OnSave ) ///����
	ON_MESSAGE ( MODIFYLETTER_SHOWLIST,CN_COMMAND,OnShowList ) ///��ʾList
	ON_MESSAGE ( MODIFYLETTER_COMBOX,CN_LISTSEL,OnCurSel )
	ON_MESSAGE ( MODIFYLETTER_GROUP_BTN,CN_COMMAND,OnGroupSend )////Ⱥ��
	ON_MESSAGE ( MODIFYLETTER_SEARCH_BTN,CN_COMMAND,OnSearchCard ) ///��Ƭ
	ON_MESSAGE ( MODIFYLETTER_MENUSAVE,CN_COMMAND,OnMenuSave )
END_MESSAGE_MAP


CModifyLetterWnd::CModifyLetterWnd()
{

}

CModifyLetterWnd::~CModifyLetterWnd()
{
	
}

void CModifyLetterWnd::OnCurSel ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	m_Phone.SetWindowText(m_List.GetListItemText(INT32(lParam)));
}

void CModifyLetterWnd::OnShowList ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	m_List.PopUpList ( m_sRect.left,
					m_sRect.top + WND_TITLE_HEIGHT + 20 );
}

void CModifyLetterWnd::OnSend ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{
	////////////////���ݴ��
	/////����Դ
	if ( TestLowPower ( ) )
	{
		MessageBox ( "����ӵ�Դ���޷����Ͷ���Ϣ��","����",MB_OK|MB_ICONSTOP ); 
		return;
	}

	char pTextBuf[81];
	char pBuf[SMS_CONTENT_MAX_NUMBER];
	m_Phone.GetWindowText ( pTextBuf,81 );  ///Get the phone
	int i;
	//////�жϵ绰����Ϸ���
	if ( !strlen ( pTextBuf ) )
	{
		MessageBox ( "�����˺��벻��Ϊ�գ�","����Ƿ�",
					MB_OK|MB_ICONSTOP );
		return;
	}


	
	for ( i = 0;i < strlen ( pTextBuf );i ++ )
		if ( ( pTextBuf[i] == '*' && pTextBuf[i+1] == '*' ) ||
			 ( pTextBuf[i] == '#' && pTextBuf[i+1] == '#' ) ||
			 ( pTextBuf[i] == '#' && pTextBuf[i+1] == '*' ) ||
			 ( pTextBuf[i] == '*' && pTextBuf[i+1] == '#' ) ||
			 ( pTextBuf[i]&0x80 ) )
		{
		
			MessageBox ( "���������������˺��룡","����Ƿ�",
					MB_OK|MB_ICONSTOP );
			return;
		}
		


	////Get content
	m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
	
	
	//// backup 
	memset ( &g_SendST,0,sizeof ( g_SendST ) );
	strcpy ( g_SendST.szPhone,pTextBuf );
	strcpy ( g_SendST.szMsg,pBuf );
		
	///////// ��ʾ��Ϣ��
	//////////////���Ͷ���Ϣ.����·�㷢������
	SendMessageToSMSL2 ( Order_Connect_Server_Msg,0,0 );
	bIsSuccessSend = FALSE; ////��־����
}
/////��Ͽ������
void CModifyLetterWnd::OnComboxInput ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{
	CloseCursor ( );
	char szTemp[81];
	m_Phone.GetWindowText ( szTemp,81 );
	if ( !strcmp ( szTemp,"�����������" ) ||
		 !strcmp ( szTemp," <�պ�> " ) )
		strcpy ( szTemp,"" );

	if ( NumInputBox ( "���������",szTemp,81 ) == IDOK )
	{
		if ( !strlen ( szTemp ) )
		{
			MessageBox ( "��������绰���룡",
				"����Ƿ�",MB_OK|MB_ICONSTOP );
			return;
		}
		
		m_List.AppendItem ( szTemp,0,0 );
		m_Phone.SetWindowText ( szTemp );
		bIsSaved = false;
		
	}
}

void CModifyLetterWnd::OnEditInput ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{
	char szTemp[SMS_CONTENT_MAX_NUMBER];

	
	m_Edit.GetWindowText ( szTemp ,SMS_CONTENT_MAX_NUMBER );
	if ( strcmp ( szTemp,"<�����������������>" ) == 0 )
		strcpy ( szTemp,"" );

	if ( InputBox ( "����������",szTemp,SMS_CONTENT_MAX_NUMBER,INIT_WRITE, TRUE ) == IDOK )
	{
		m_Edit.SetWindowText ( szTemp );
		bIsSaved = false;
	}

	
}

BOOL CModifyLetterWnd::OnDestroy ( )
{

	
	/////���ٴ��ھ䲢
	if ( m_pShowMsg )
	{
		delete m_pShowMsg;
		m_pShowMsg = NULL;
	}

	//////ѯ���Ƿ񱣴�
	if ( !bIsSaved )
		if ( MessageBox ( "����Ϣ�Ѿ��ı䣬������",
			"��ʾ",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			
			if ( bIsSendStatus ) ///�˵�����
				OnMenuSave ( 0,0,0 );
			else OnSave ( 0,0,0 );
			
		}
		
	g_CurrentID = -1;

	return CAppWnd::OnDestroy ( );
}

BOOL CModifyLetterWnd::OnCreate ( )
{

	bIsSaved = true;////ȱʡ��Ϊ�Ѿ�����
	g_iServerType = 1;/////���÷�������Ϊ����Ϣ

	if ( CAppWnd::OnCreate ( ) == FALSE )
		return FALSE;

	int i;
	/////�����˵����ť
	CRect rc( m_sRect.left,
			m_sRect.top ,
			m_sRect.left + 76,
			m_sRect.top+ SMS_APP_TITLE_HEIGHT  );

	if ( bIsSendStatus ){
		char szTemp[9];
		if ( strlen ( g_ST.szPhone ) > 8 )
		{

			for ( i = 0;i < 8;i ++ )
				szTemp[i] = g_ST.szPhone[i];
			szTemp[i] = '\0';

			m_MenuBtn.Create ( szTemp,rc,this,
				MODIFYLETTER_MENU_BTN );
		}else m_MenuBtn.Create ( g_ST.szPhone,rc,this,
			MODIFYLETTER_MENU_BTN );
	}else {
	
		m_MenuBtn.Create ( "�¶���Ϣ",rc,this,
			MODIFYLETTER_MENU_BTN );
	}
	
	
	//////�������Ͱ�ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	m_SendBtn.Create ( "����",rc,this,MODIFYLETTER_SEND_BTN );
	
	//////�������水ť��ɾ����ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	if ( bIsSendStatus )////ɾ��
		m_SaveBtn.Create ( "ɾ��",rc,this,MODIFYLETTER_SAVE_BTN );
	else m_SaveBtn.Create ( "����",rc,this,MODIFYLETTER_SAVE_BTN );
	/////������Ͽ�
	rc.left = m_sRect.left ,
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 1,
	rc.right = m_sRect.left + 110,
	rc.bottom = m_sRect.top + WND_TITLE_HEIGHT + 20;
	
	m_Phone.Create ( g_ST.szPhone,rc,this,WS_VISIBLE,
			MODIFYLETTER_PHONE,81 );

	////////��������List��ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 12;
	m_PopupListBox.Create ( "",ICON_DOWNARROW,rc,this,MODIFYLETTER_SHOWLIST );
	//////����List
	rc.left = 0,
	rc.top = 0,
	rc.right = 124,
	rc.bottom = 60;
	m_List.Create ( rc,this,NULL,MODIFYLETTER_COMBOX );
	/////// ����Ⱥ��ѡ��ť
	rc.top = m_sRect.top + WND_TITLE_HEIGHT;
	rc.left = rc.right + 1;
	rc.right = rc.left + 16;
	rc.bottom = rc.top + 20;
	m_GroupBtn.Create ( "Ⱥ",rc,this,MODIFYLETTER_GROUP_BTN );
	////������ѯ��ť
	rc.left = rc.right + 1;
	rc.right = rc.left + 16;
	m_SearchBtn.Create ( "��",rc,this,MODIFYLETTER_SEARCH_BTN );
	
	/////�����༭�ؼ�
	rc.left = m_sRect.left + 1 ;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 21;
	rc.right = 159;
	rc.bottom = 159;

	m_Edit.Create ( g_ST.szMsg,rc,this,WS_VISIBLE,
			MODIFYLETTER_EDIT,SMS_CONTENT_MAX_NUMBER );
	
	

	//////�����˵�
	m_Menu.Create ( this );
	
	if ( !g_bIsModel )
	{
		m_Menu.AppendItem ( "ѡ������",MODIFYLETTER_WORD );
		m_Menu.AppendItem ("-" );
	}

	if ( bIsSendStatus )
		m_Menu.AppendItem ( "����",MODIFYLETTER_MENUSAVE );
	m_Menu.AppendItem ( "��浽",MODIFYLETTER_SAVEAS );

	return TRUE;

									
}

///�˵�����
void CModifyLetterWnd::OnMenuSave ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	
	////////�޸�һ����¼
	char pTextBuf[81];
	char pBuf[SMS_CONTENT_MAX_NUMBER];
	m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
	m_Phone.GetWindowText ( pTextBuf,81 );

	////���Դ���
	if ( !strcmp ( pTextBuf,"�����������" ) )
		strcpy ( pTextBuf," <�պ�> " );

	strcpy ( g_ST.szPhone,pTextBuf );
	strcpy ( g_ST.szMsg,pBuf );
	g_ST.Flag = 0;///ֻ��


	AddSendRecord ( g_ST,TRUE );///Add a record
	bIsSaved = true;
}
///��Ƭѡ��
void CModifyLetterWnd::OnSearchCard ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	CloseCursor ( );
	extern short SelectTelNum(char *strTel , short nBufLen);
	char szTempPhone[16],buffer[81];
	
	
	SelectTelNum ( szTempPhone,16 );
	
	if ( strlen ( szTempPhone ) )
	{
		m_Phone.GetWindowText ( buffer,81 );
		if ( !strcmp ( buffer,"�����������" ) )
				strcpy ( buffer,"" );
		
		
		
		int len1 = strlen ( buffer ),
			len2 = strlen ( szTempPhone ),
			n = 0,
			i;
		
		for ( i = 0;i < len1;i ++ )
			if ( buffer[i] == '#' )
				n ++;
		
		if ( len2 > ( 77 - len1 ) ||
			 n >= 4 )
		{
		
			MessageBox("һ�����ֻ����5�����룡" , "����",MB_OK|MB_ICONSTOP);
			return;
		}


		if ( strlen ( buffer ) > 0 )
		{
			if ( buffer[len1-1] == '*' )
				strcat ( buffer,"0" );

			len1 = strlen ( buffer );
			if ( buffer[len1-2] != '*' )
				strcat ( buffer,"*0#" );
			
			if ( buffer[len1-2] == '*' )
				strcat ( buffer,"#" );
		}

		strcat ( buffer,szTempPhone );
		m_Phone.SetWindowText ( buffer );
		m_List.AppendItem ( buffer,0,0 );
		bIsSaved = false;
	}
}
///Ⱥ��
void CModifyLetterWnd::OnGroupSend ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	if ( g_bIsGroup )
	{
		MessageBox ( "���Ѿ�ѡ��Ⱥ����!","ʧ��",
			MB_OK|MB_ICONSTOP );
		return;
	}


	CloseCursor ( );
	GroupCall = SYJCALL;
	if ( m_pGRPSDWnd == NULL )
			m_pGRPSDWnd = new CGRPSDWnd;
	m_pGRPSDWnd->Create("Ⱥ��", 
						 WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
						 CRect(0 ,  0, 159 , 159) , 0); 
	m_pGRPSDWnd->ShowWindow();

	
}
////ѡ������
void CModifyLetterWnd::OnSelectWord ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	/////���õ��ñ�־
	CloseCursor ( );
	ModelCall = SYJCALL;
	
	if ( m_pMODELWnd == NULL )
			m_pMODELWnd = new CMODELWnd;
	m_pMODELWnd->Create("������ ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			
			HELP_SMS_COLLECTION ); 
	m_pMODELWnd->ShowWindow();
}
//// ����
void CModifyLetterWnd::OnSave ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	//////����Ƿ�ﵽ���洢����
	if ( !bIsSendStatus ){ /////////���湦��

				
		////////����һ����¼
		memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
		char pTextBuf[81];
		char pBuf[SMS_CONTENT_MAX_NUMBER];
		m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
		m_Phone.GetWindowText ( pTextBuf,81 );

		////���Դ���
		if ( !strcmp ( pTextBuf,"�����������" ) )
			strcpy ( pTextBuf," <�պ�> " );
		strcpy ( g_ST.szPhone,pTextBuf );
		strcpy ( g_ST.szMsg,pBuf );

		////// ���ʱ��
		FillTime ( g_ST.szTime );

		/*DateTime	date;
		GetSysTime ( &date );
		sprintf ( g_ST.szTime,"%d-%d-%d %d:%d",
			  date.year,
			  date.month,
			  date.day,
			  date.hour,
			  date.minute );*/

		g_ST.Flag = 0;///ֻ��
		g_ST.bMailBox = iCurrentLetterBox;///��ǰѡ�е�����
		g_ST.iChildMailBox = 1;/////���浽������
		g_ST.iLetterStatus = ICON_DRAFT;////�ݸ�
		
		g_ST.dataID = -1;		
		
		AddSendRecord ( g_ST,FALSE );///Add a record
			
		bIsSaved = true; 
		
	}else { //////ɾ������
	
			//////////////ɾ����¼
			if ( MessageBox ( "ȷ��ɾ���ü�¼��","ɾ��",
				MB_YESNO|MB_ICONQUESTION ) == IDYES )
			{
				if ( iCurrentChildLetterBox != 2 )
				{
					bool b = DeleteSMS ( g_CurrentID );	

				}else {
					
					if ( g_SendedSMS[iCurrentLetterBox][g_iCurrentPos].bIsFlash == 1 )
					{
						///////�Ƚ�Flash�еļ�¼ɾ��
						int id = g_SendedSMS[iCurrentLetterBox][g_iCurrentPos].dataID;
						BOOL b = DeleteSMS ( id );
						g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem --;

					}

					///////ɾ���ڴ��еļ�¼
					UINT16 i;
					for ( i = g_iCurrentPos;i < SMS_MAX_SAVE_NUMBER-1;i ++ )
						g_SendedSMS[iCurrentLetterBox][i] = g_SendedSMS[iCurrentLetterBox][i+1];

					memset ( &g_SendedSMS[iCurrentLetterBox][i],0,sizeof ( SMS ) );
					g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber --;

				}
				
				bIsSaved = TRUE;

				Destroy ( );


			}
	}

}
////��浽
void CModifyLetterWnd::OnSaveAs ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	
	////////�õ���ǰ�Ķ���Ϣ����
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	char pTextBuf[81];
	char pBuf[SMS_CONTENT_MAX_NUMBER];
	m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
	m_Phone.GetWindowText ( pTextBuf,81 );

	////���Դ���

	if ( !strcmp ( pTextBuf,"�����������" ) )
		strcpy ( pTextBuf," <�պ�> " );
	strcpy ( g_ST.szPhone,pTextBuf );
	strcpy ( g_ST.szMsg,pBuf );

	////// ���ʱ��
	if ( !strlen ( g_ST.szTime ) ) { //////���ʱ���ǿգ���䵱ǰʱ��
									////��������ǰʱ��Ϊ׼
		
		FillTime ( g_ST.szTime );

		/*DateTime	date;
		GetSysTime ( &date );
		sprintf ( g_ST.szTime,"%d-%d-%d %d:%d",
			  date.year,
			  date.month,
			  date.day,
			  date.hour,
			  date.minute );*/

	
	}

	g_ST.Flag = 0;///ֻ��
	g_ST.iChildMailBox = 1;/////���浽������
	g_ST.iLetterStatus = ICON_DRAFT;////�ݸ�	

	////// ��ʾ���Ϊ���ô���
	if ( m_pSaveAsWnd == NULL )
		m_pSaveAsWnd = new CSaveAsWnd;
	m_pSaveAsWnd->Create ( "ѡ��Ŀ������",
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_OKBOX,
		CRect ( 0,0,159,159 ) );
	m_pSaveAsWnd->ShowWindow ( );
}
///��ʾ�˵�
void CModifyLetterWnd::OnShowMenu ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	m_Menu.PopupMenu ( m_sRect.left + 1,
		m_sRect.top + WND_TITLE_HEIGHT  );
}

void CModifyLetterWnd::OnPaint ( CDC &dc )
{
	CloseCursor ( );

	CAppWnd::OnPaint ( dc );

	if ( ModelCall == ENDCALL )///ѡ������
	{
		ModelCall = WYCALL;
		char szTemp[SMS_CONTENT_MAX_NUMBER];
		strcpy ( szTemp,"" );

		m_Edit.GetWindowText ( szTemp ,SMS_CONTENT_MAX_NUMBER );
		

		if ( strlen ( g_ModelSend ) > 0 )
		{
			if ( strcmp ( szTemp,"<�����������������>" ) == 0 &&
				 !bIsSendStatus )
				strcpy ( szTemp,"" );


			int len1 = strlen ( szTemp ),
				len2 = strlen ( g_ModelSend );
			if (  ( len1+len2 ) > SMS_CONTENT_MAX_NUMBER )
			{
				MessageBox ( "���ݹ��������ʧ�ܣ�",
					"����",MB_OK|MB_ICONQUESTION );
				
				
			}else strcat ( szTemp,g_ModelSend );
		}
		
		m_Edit.SetWindowText ( szTemp );

		bIsSaved = false;
		
	}

	if ( GroupCall == ENDCALL ){ /////ѡ��Ⱥ��
	
		GroupCall = WYCALL;
		char szTemp[81];
		int len;
		strcpy ( szTemp,"" );////Init
		for ( int i = 0;i < 5;i ++ ) ///���绰�����Ƿ�Ϸ�
		{							////����������ʱBuffer
			len = strlen ( g_pGroupSend.Numbers[i].szPhone );
			if ( !len ) continue;
			if ( g_pGroupSend.Numbers[i].szPhone[len-2] != '*' )
				strcat ( g_pGroupSend.Numbers[i].szPhone,"*0" );

			strcat ( szTemp,g_pGroupSend.Numbers[i].szPhone );
			strcat ( szTemp,"#" );
		}
		
		len = strlen ( szTemp );
		if ( !len ) 
		{
			MessageBox ( "���벻��Ϊ�գ�","����",
				MB_OK|MB_ICONSTOP );
			return;
		}


		if ( szTemp[len-1] == '#' )
			szTemp[len-1] = '\0';


		m_Phone.SetWindowText ( szTemp );
		bIsSaved = false;
		m_List.AppendItem ( szTemp,0,0 ); ///��ͨ����¼������һ��

	}
}
 
//////////////////////// ����ģ�� -------- ����
/////////// class CSETWnd   09_����  ����ʵ��  start  wangyu ///////
BEGIN_MESSAGE_MAP(CSETWnd, CAppWnd) 
	ON_MESSAGE(SET_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage     
END_MESSAGE_MAP

CSETWnd::CSETWnd()
{
	/////��ʼ������

	m_pCenterNoWnd  = NULL;
	m_pSetOther		= NULL;
	m_pSetLetterBox = NULL;  ///��������
}

CSETWnd::~CSETWnd()
{
}

BOOL CSETWnd::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;
	////////����ListView�ؼ�
	m_pSETListView.Create ( CRect ( m_sRect.left ,
								     m_sRect.top + WND_TITLE_HEIGHT,
								     159,159 ), this, SET_LISTVIEW );
	
	m_pSETListView.AppendItem ( "��Ϣ��",ICON_SETTING_CENTER_No);////��Ϣ���ĺ���
	m_pSETListView.AppendItem ( "����",ICON_SMSSETSOUND );  ////��������///	
	m_pSETListView.AppendItem ( "��ִ",ICON_SMSSETRETURN );  ////��������///	
	m_pSETListView.AppendItem ( "����",ICON_SMMAINLV01 );  ////��������///	


	return TRUE;
}

void CSETWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );	
}

void CSETWnd::OnLVCommand ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
	UINT32 iItem = ( UINT32 )lParam;
	switch ( iItem )
	{
	case 0:  //// ��Ϣ���ĺ���	 
		    if(!m_pCenterNoWnd)
			  m_pCenterNoWnd = new CCENTERNoWnd;
	     	m_pCenterNoWnd->Create( "��Ϣ���ĺ���", 
					                 WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX |
									 WS_CHILD | WS_OKBOX, 
						             CRect(0 ,  0, 159 , 159) ,
									 HELP_SMS_SETUP_NUM);		
	   	    m_pCenterNoWnd->ShowWindow ( );
		   	break; 
	
	case 1: /////����
			if ( m_pSetOther == NULL )
				m_pSetOther = new CSetNamePassword;

			m_pSetOther->SetSettingStyle ( 1 );////��������
			m_pSetOther->Create ( "��������",
								WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD|WS_OKBOX,
								CRect ( 0,0,159,159 ),
								HELP_SMS_SETUP_SOUND );


			m_pSetOther->ShowWindow ( );

		    break;
	case 2:  /////��ִ		 
			if ( m_pSetOther == NULL )
				m_pSetOther = new CSetNamePassword;

			m_pSetOther->SetSettingStyle ( 2 );////��ִ����
			m_pSetOther->Create ( "��ִ����",
								WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD|WS_OKBOX,
								CRect ( 0,0,159,159 ),
								HELP_SMS_SETUP_REPLY );


			m_pSetOther->ShowWindow ( );

			 break;
	case 3: /////
		//////����˽������
		if ( m_pSetLetterBox == NULL )
			m_pSetLetterBox = new CSetLetterBox;

		m_pSetLetterBox->Create ( "�����趨",
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
					CRect(0 ,  0, 159 , 159) , 
					HELP_SMS_SETUP_BOX ); 

		m_pSetLetterBox->ShowWindow ( );	
		break;
	
	}
}
 
BOOL CSETWnd::OnDestroy ( )
{	
	//////�Լ������ٴ���	 
	if ( m_pSetLetterBox != NULL )
	{
	
		delete m_pSetLetterBox;
		m_pSetLetterBox = NULL;
	}

	if ( m_pSetOther )
	{
		delete m_pSetOther;
		m_pSetOther = NULL;
	}
	
	return CAppWnd::OnDestroy ( );
} 
////////  class CSETWnd        09_����  ����ʵ��        end   wangyu ///////

////////  class CCENTERNoWnd   09_����_��Ϣ�� ʵ�ֺ���  start wangyu ///////
BEGIN_MESSAGE_MAP(CCENTERNoWnd, CAppWnd) 
	ON_MESSAGE ( CENTERNoWND_REV,CN_COMMAND,OnRevEditInput )	 
	ON_MESSAGE ( CENTERNoWND_SEND,CN_COMMAND,OnSendEditInput )
END_MESSAGE_MAP

void CCENTERNoWnd::OnRevEditInput ( UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
	char szTemp[32];
	m_RevEdit.GetWindowText ( szTemp,32 );
	if ( NumInputBox ( "������պ���",szTemp,32 ) == IDOK )	{
		m_RevEdit.SetWindowText ( szTemp );
		m_RevEdit.OnPaint ( CDC ( ) );
	}
}

void CCENTERNoWnd::OnSendEditInput ( UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{

	char szTemp[32];
	m_SendEdit.GetWindowText ( szTemp,32 );
	if ( NumInputBox ( "���뷢�ͺ���",szTemp,32 ) == IDOK )	{
		m_SendEdit.SetWindowText ( szTemp );
		m_SendEdit.OnPaint ( CDC ( ) );
	}
}

void CCENTERNoWnd::OnOk(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{  
	char rev[32],send[32];
	m_RevEdit.GetWindowText ( rev,32 );
	m_SendEdit.GetWindowText ( send,32 );

	strcpy ( g_SystemOption.g_pCenterReceivePhone,rev );
	strcpy ( g_SystemOption.g_pCenterSendPhone,send );


	////////���浽ϵͳ����
	SaveSystemOption ( &g_SystemOption );

	Destroy();
}

BOOL CCENTERNoWnd:: OnCreate()
{
	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;



	m_RevEdit.Create ( g_SystemOption.g_pCenterReceivePhone,
						CRect (m_sRect.left + 2,
		                       SMS_APP_TITLE_HEIGHT+22,
			                   m_sRect.right - 2,
			                   SMS_APP_TITLE_HEIGHT+42 ),
						this,CENTERNoWND_REV );

	m_SendEdit.Create ( g_SystemOption.g_pCenterSendPhone,
					 	CRect (m_sRect.left + 2,
						100,  
						m_sRect.right -2,
						120),
					 	this,CENTERNoWND_SEND );

	return TRUE;
}

CCENTERNoWnd::CCENTERNoWnd()
{
}

CCENTERNoWnd::~CCENTERNoWnd()
{ 
}  
void CCENTERNoWnd::OnPaint ( CDC &dc )
{
  	CAppWnd::OnPaint ( dc );	
	dc.SetBackColor ( );
  	dc.TextOut(2,20,"��Ϣ���Ľ��պ��룺" );    

	dc.TextOut(2,80,"��Ϣ���ķ��ͺ��룺" ); 
}
void CCENTERNoWnd::Destroy ( )
{
	CFrameWnd::Destroy();
}
////////  class CCENTERNoWnd   09_����_��Ϣ�� ʵ�ֺ���  end   wangyu   ////////

 /////////////////////////////////////////////////////////////////////////
//////////
//////////  class CMODELWnd  01_������     ʵ�ֺ���  start   wangyu    ////  
//////////
/////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMODELWnd, CAppWnd) 
	ON_MESSAGE(MODEL_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage  
	ON_MESSAGE(MODEL_NewButton,CN_COMMAND,OnGRPSDNewBtn)
	ON_MESSAGE(MODEL_ComBox,CN_COMMAND,OnShowMenuBtn) 
	ON_MESSAGE(MODEL_Delete,CN_COMMAND,OnDelete)///ɾ��  
    ON_MESSAGE(MODEL_Rename,CN_COMMAND,OnRename)///����  
    ON_MESSAGE(MODEL_Taxis,CN_COMMAND,OnTaxis)///����
END_MESSAGE_MAP

CMODELWnd::CMODELWnd()
{	/////��ʼ������
//	m_pModelListView = NULL; 
    m_ContentWnd = NULL,
	m_pDeleteWnd = NULL,
	m_pRenameWnd = NULL,
	m_pTaxisWnd = NULL;
	memset(g_Model_Locked , 0, MAX_MODEL_NUMBER);
}

CMODELWnd::~CMODELWnd()
{
}

BOOL CMODELWnd::OnCreate ( )
{    
	
	if ( !CAppWnd::OnCreate ( ) )
		return FALSE;
	////////����ListView�ؼ�
//	if(m_pModelListView==NULL) 
//	m_pModelListView = new CListView;
	m_pModelListView.Create ( CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT,
								159,159 ),this,MODEL_LISTVIEW );     
	///////������������    
	m_FirstPaint=TRUE;    
	MODEL *pTemp;
	g_nLocal = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);
	for(short i = 0; i< g_nLocal; i++)	{
	   pTemp = GetModelPointer(g_Model_Locked[i]);	 	   
	   m_pModelListView.AppendItem (pTemp->szModelName, ICON_SMMAINLV02 ); 	 
	}    
	//////////�����˵�������ť
	m_ConMB.Create("������",CRect (m_sRect.left ,
		                           m_sRect.top  , 
								   m_sRect.left + TITLE_MENU_WIDTH - 1, 
								   m_sRect.top + TITLE_BAR_HEIGHT - 1),
    								this,MODEL_ComBox);
    //////////���������˵�ѡ��	
	if(!m_Content.Create(this))
		return FALSE;
	m_Content.AppendItem("���������" ,MODEL_Rename, 0);
	m_Content.AppendItem("������ɾ��" ,MODEL_Delete, 0);
	m_Content.AppendItem("����������" , MODEL_Taxis,0);  
		//////�����½���ť
	CRect rc ( m_sRect.left + 75,
		       0,
			   m_sRect.left + 109,
			   SMS_APP_TITLE_HEIGHT );
	m_pConNewButton.Create ( "�½�",rc,this,MODEL_NewButton ); 
  return TRUE;
}

////////////��ʾ������������˵��е�ѡ��
void CMODELWnd::OnShowMenuBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{ 
	m_Content.PopupMenu ( m_sRect.left,
				m_sRect.top + WND_TITLE_HEIGHT );
}
                                        
	                    
////////// "������" "����"ѡ��--����
void CMODELWnd::OnRename( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{      
             ModelModifyFlag =  0;
		     if(!m_pRenameWnd)
			  m_pRenameWnd = new CContent_ModWnd;
	     	m_pRenameWnd->Create("����", 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
						          CRect(0 ,  0, 159 , 159) , 
								  HELP_SMS_COLLECTION_RENAME );		
	   	    m_pRenameWnd->ShowWindow(); 			 
}
//////////������  "ɾ��"ѡ��--����
void CMODELWnd::OnDelete( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
             ModelModifyFlag =  1;
		     if(!m_pDeleteWnd)
			  m_pDeleteWnd = new CContent_ModWnd;
	     	m_pDeleteWnd->Create( "ɾ��", 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
						             CRect(0 ,  0, 159 , 159) , 
									 HELP_SMS_COLLECTION_DEL );	
			
	   	    m_pDeleteWnd->ShowWindow ( ); 
}
//////////������  "����"ѡ��--����
void CMODELWnd::OnTaxis( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	         ModelModifyFlag =  2;
		     if(!m_pTaxisWnd)
			  m_pTaxisWnd = new CContent_ModWnd;
	     	m_pTaxisWnd->Create( "����", 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD| WS_OKBOX, 
						             CRect(0 ,  0, 159 , 159) , HELP_SMS_COLLECTION_SORT);	
			

	   	    m_pTaxisWnd->ShowWindow ( ); 
}
//////////////�½���������
BOOL CMODELWnd::OnGRPSDNewBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
    MODEL  cc, *pTemp;
    short ModelID;    
	char  ModelName[10];

	g_nLocal = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);
	if(g_nLocal >= MAX_MODEL_NUMBER){
	  	MessageBox("�洢������!" , "�½���������",MB_OK|MB_ICONSTOP);
		return FALSE; 
	}

	strcpy ( ModelName,"" );
	if(::InputBox("���볣��������", ModelName , 10)== IDOK)
	{
		memset(&cc, 0 ,sizeof(cc));
		if( strlen ( ModelName ) == 0 )		{
			MessageBox("���Ʋ���Ϊ��!" , "�½���������",MB_OK|MB_ICONSTOP);
			return FALSE;
		}		

		strcpy(cc.szModelName , ModelName);	
		for(short i = 0; i< g_nLocal; i++){
			pTemp = GetModelPointer(g_Model_Locked[i]);
			if(strcmp(cc.szModelName , pTemp->szModelName)==0){
				::MessageBox("�������鲻������!" , "�½���������",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}   
		
		ModelID = AppendMODEL(&cc);
		if(ModelID >=0)	{			
			cc.iDataID = ModelID;			 
			g_Model_Locked[g_nLocal++] = ModelID;			 
			bool modify = ModifyModel(ModelID, &cc);
			OnPaint(CDC());		 
		}else
			MessageBox("�ռ�����","�½���������" ,  MB_OK|MB_ICONEXCLAMATION);
	}
 return TRUE;
}

//////////////// ĳ�����ﱻѡ��
void CMODELWnd::OnLVCommand ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	MODEL *pTemp;
    UINT32 iItem = ( UINT32 )lParam;
	
	ModelFlagID=g_Model_Locked[(short)iItem];
	
	pTemp = GetModelPointer(g_Model_Locked[(short)iItem]);	
 	 
    if(!m_ContentWnd)
		m_ContentWnd = new CContentWnd;
	m_ContentWnd->Create( pTemp->szModelName, 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD , 
						          CRect(0 ,  0, 159 , 159) ,0);		
	m_ContentWnd->ShowWindow ( ); 
}
 
void CMODELWnd::OnPaint ( CDC &dc )
{   
  // �ػ���������	 
  if(!m_FirstPaint)
  {
	 m_pModelListView.DeleteAllItems();
	 for(short i = 0; i< g_nLocal; i++)
	 {
	    MODEL *pTemp = GetModelPointer(g_Model_Locked[i]);	    
		m_pModelListView.AppendItem (pTemp->szModelName,ICON_SMMAINLV02 ); 	 
	 }
  }
  m_FirstPaint=FALSE;
  CAppWnd::OnPaint ( dc );	 
}

BOOL CMODELWnd::OnDestroy ( )

{ 
/*	if ( m_pModelListView != NULL )    
	{
		delete m_pModelListView;
		m_pModelListView = NULL;
	}
*/
 
if ( m_ContentWnd != NULL )    
	{
		delete m_ContentWnd;
		m_ContentWnd = NULL;
	}

	if ( m_pDeleteWnd != NULL )    
	{
		delete m_pDeleteWnd;
		m_pDeleteWnd = NULL;
	}
	if ( m_pRenameWnd != NULL )    
	{
		delete m_pRenameWnd;
		m_pRenameWnd = NULL;
	}
	if ( m_pTaxisWnd != NULL )    
	{
		delete m_pTaxisWnd;
		m_pTaxisWnd = NULL;
	}

	return CAppWnd::OnDestroy ( );
} 
/////////    class CMODELWnd  02_������   ʵ�ֺ���  end   wangyu ////


/////////    class CContent_ModWnd      02_������_�޸�     ʵ�ֺ���  start   wangyu ////
BEGIN_MESSAGE_MAP(CContent_ModWnd, CAppWnd) 
    ON_MESSAGE ( MODELMod_LISTVIEW,CN_COMMAND,OnModifyModel )    
    ON_MESSAGE ( MODEL_TaxisUpButton,CN_COMMAND,OnTaxisModelUp )
	ON_MESSAGE ( MODEL_TaxisDwButton,CN_COMMAND,OnTaxisModelDw )
END_MESSAGE_MAP 
////  ���Ƴ������޸ĵ�ѡ��
UINT32 CContent_ModWnd::OnModifyModel(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    switch(ModelModifyFlag)	{
	    case 0:  // ����	         
		      OnRenameModel(0, 0, lParam);
		      break;	
	    case 1:  // ɾ��		   
		      OnDeleteModel(0, 0, lParam);
		      break;	     
	    default : 
		        break;	 
	}
}
//////////////////  ɾ�� ��������
UINT32 CContent_ModWnd::OnDeleteModel(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{	
	UINT32 uItem = lParam;  
    if( ::MessageBox("ȷ��Ҫɾ��������?",
		           "ɾ��!",MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{

		//m_pConModifyListView.DeleteItem(uItem );  	
		 
		g_nLocal = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);
		MODEL *pTemp = GetModelPointer(g_Model_Locked[(short)uItem]);
		short g_nAllNumber = FindAllContent(g_Content_locked, MAX_CONTENT_NUMBER);// ���������
		// ɾ���ڴ����������
		for(short i=0; i<g_nAllNumber; i++)	{
			CONTENT *pcTemp = GetContentPointer(LOWORD(g_Content_locked[i]));  
			if(pcTemp->iModelID == pTemp->iDataID) {
				bool del = DeleteContent(pcTemp->iDataID);	
				g_Content_locked[i] =  g_Content_locked[i+1]; 
				g_nNumber--;
			} 
		}
		// ɾ���������� 
		bool del = DeleteMODEL(g_Model_Locked[(short)uItem]);
		g_nLocal--;
		for(short i = uItem; i<= g_nLocal; i++)
		   g_Model_Locked[i] =  g_Model_Locked[i+1];	  
		// �ػ�
		m_pConModifyListView.DeleteAllItems();
		for(short i = 0; i< g_nLocal; i++) {
			pTemp = GetModelPointer(g_Model_Locked[i]);	    
	   		m_pConModifyListView.AppendItem (pTemp->szModelName,0,0,ICON_MODEL_Content,LOWORD(g_Model_Locked[i]) ); 	 
		 }
		
		OnPaint(CDC());
	}
		
	return TRUE;
}
////////////////// �������� ���� ��
UINT32 CContent_ModWnd::OnRenameModel(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{ 	
    char SetTemp[10];
 	UINT32 uItem = lParam; 
	
	MODEL cc, *pTemp;
    pTemp = GetModelPointer(g_Model_Locked[(short)uItem]);	 

	strcpy (  SetTemp, pTemp->szModelName );
 	if( InputBox ( "�������µ�����",SetTemp,10 ) == IDOK ) {   
		if( SetTemp[0]== '\0'){
			MessageBox("���Ʋ���Ϊ��" , "�޸ĳ���������",MB_OK|MB_ICONSTOP);
			return FALSE;
		}
        strcpy ( cc.szModelName, SetTemp );
        
		
		/*for( short i = 0; i< g_nLocal; i++){
			 pTemp = GetModelPointer(g_Model_Locked[i]);
			 if( strcmp(cc.szModelName , pTemp->szModelName)==0){
				 MessageBox("�������鲻������!" , "�½���������",MB_OK|MB_ICONSTOP);
				 return FALSE;
			 }
		} */  


        bool modify = ModifyModel(g_Model_Locked[(short)uItem], &cc);
       // �ػ�
	   m_pConModifyListView.DeleteAllItems();
 	   for(short i = 0; i< g_nLocal; i++)  {
	      pTemp = GetModelPointer(g_Model_Locked[i]);	    
	      m_pConModifyListView.AppendItem (pTemp->szModelName,0,0,ICON_MODEL_Content,LOWORD(g_Model_Locked[i]) ); 	 
	   }
 	   OnPaint(CDC());	
	}
	return TRUE;    
} 
//////////////////  ���������� 
UINT32 CContent_ModWnd::OnTaxisModelUp(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{ 
    INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pConModifyListView.GetSelItem();   
 
	if(uItem == -1 ){
		MessageBox("����ѡ��Ҫ�����ĳ�����!","��ʾ",MB_OK);
	    return FALSE;
	}
    if(uItem==0) {	
		MessageBox("��ѡ�еĳ��������ǵ�һ��!","��ʾ",MB_OK);
		return FALSE;
    }   
	m_pConModifyListView.GetItem(&list1 , uItem-1);
	m_pConModifyListView.GetItem(&list2, uItem);
	m_pConModifyListView.ModifyItem(&list2, uItem-1 , FALSE);
	m_pConModifyListView.ModifyItem(&list1, uItem, FALSE);
	m_pConModifyListView.SetSelItem(uItem-1);
	nItemStart = m_pConModifyListView.GetStartItem();
	nScreenRows = m_pConModifyListView.GetScreenItems();
	if(uItem > nItemStart + nScreenRows)
		m_pConModifyListView.ScrollTo(uItem -nScreenRows , TRUE);
	else if(uItem <= nItemStart)
		m_pConModifyListView.ScrollTo(uItem -1, TRUE);
	m_pConModifyListView.OnPaint(CDC());
 return TRUE;
}
//////////////////////���ư�ť ��Ӧ����
UINT32 CContent_ModWnd::OnTaxisModelDw(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{    
	INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pConModifyListView.GetSelItem();   
 
	if(uItem == -1 )	{
		MessageBox("����ѡ��Ҫ�����ĳ�����!","��ʾ",MB_OK);
	    return FALSE;
	}
    if(uItem==(g_nLocal-1)) {    ////���һ��	
		MessageBox("��ѡ�еĳ������������һ��!","��ʾ",MB_OK);
		return FALSE;
    } 

	m_pConModifyListView.GetItem(&list1 , uItem+1);
	m_pConModifyListView.GetItem(&list2, uItem);
	m_pConModifyListView.ModifyItem(&list2, uItem+1 , FALSE);
	m_pConModifyListView.ModifyItem(&list1, uItem , FALSE);
	m_pConModifyListView.SetSelItem(uItem+1);
	nItemStart = m_pConModifyListView.GetStartItem();
	nScreenRows = m_pConModifyListView.GetScreenItems();
	
	if(uItem >= nItemStart + nScreenRows -1)
		m_pConModifyListView.ScrollTo(uItem+2 - nScreenRows , TRUE);
	else if(uItem < nItemStart -1 ) // ???
		m_pConModifyListView.ScrollTo(uItem+1, TRUE);
	m_pConModifyListView.OnPaint(CDC());
 return TRUE;
}

void CContent_ModWnd::OnOk(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    CListItem list;
	for(short i = 0 ; i< g_nLocal; i++)	{
    	m_pConModifyListView.GetItem(&list, i);
		g_Model_Locked[i] = list.nExtData;		 
	}
	SortModel(g_Model_Locked , g_nLocal);		 
	Destroy(); 
}

BOOL CContent_ModWnd:: OnCreate()
{
	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;
//	if(m_pConModifyListView==NULL) 
// 	m_pConModifyListView = new CListView;    
  
	switch(ModelModifyFlag)	{
	case 0:  // ���������	
	case 1:  // ������ɾ��		   
		   m_pConModifyListView.Create ( CRect ( m_sRect.left, m_sRect.top + WND_TITLE_HEIGHT, 159,159 ),
								          this , MODELMod_LISTVIEW );    	  	       
		 break;
	case 2:  // ����������
		   m_pConModifyListView.Create ( CRect ( m_sRect.left, m_sRect.top + WND_TITLE_HEIGHT, 159,159 ),
								          this, LS_SMALLICON,  MODELMod_LISTVIEW );  
	       m_pUpButton.Create ( "����",CRect( m_sRect.left + 58, 0, m_sRect.left + 91,  SMS_APP_TITLE_HEIGHT ),
									this,MODEL_TaxisUpButton );  
		   m_pDwButton.Create ( "����",CRect( m_sRect.left + 92, 0, m_sRect.left + 125, SMS_APP_TITLE_HEIGHT ),
									this,MODEL_TaxisDwButton ); 
		   break;	
	default : 
	       break;	 
	}
	m_pConModifyListView.SetStatus(LS_SMALLICON, TRUE);    
	 
	for(int i = 0; i< g_nLocal; i++) {
	    MODEL *pTemp = GetModelPointer(g_Model_Locked[i]);	   	 
	    m_pConModifyListView.AppendItem (pTemp->szModelName,0,0,ICON_MODEL_Content, g_Model_Locked[i]); 	 
	}    
	return TRUE;
}
/////
CContent_ModWnd::CContent_ModWnd()
{
//	m_pConModifyListView = NULL;
}

CContent_ModWnd::~CContent_ModWnd()
{ 
}
void CContent_ModWnd::Destroy ( )
{
 	CFrameWnd::Destroy();
} 

void CContent_ModWnd::OnPaint ( CDC &dc )
{
   	CAppWnd::OnPaint ( dc );
 
}
/////////    class CContent_ModWnd  02_������_�޸�     ʵ�ֺ���  end    wangyu ////

/////////    class CContentWnd      02_������_����     ʵ�ֺ���  start   wangyu ////
BEGIN_MESSAGE_MAP(CContentWnd, CAppWnd) 
	ON_MESSAGE ( MODIFYCONTENT,CN_GRIDSEL,OnModifyContent )
	ON_MESSAGE ( Content_NewButton,CN_COMMAND,OnNewContent )
END_MESSAGE_MAP

CContentWnd::CContentWnd()
{
//	m_pGridView = NULL;
	m_ContentList=NULL;
	memset(g_Content_locked , 0, MAX_CONTENT_NUMBER);
}

CContentWnd::~CContentWnd()
{ 
}  

BOOL CContentWnd:: OnCreate()
{
   if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE; 
     
	CONTENT cc, *pTemp;   
	g_nNumber = FindContent(ModelFlagID, g_Content_locked, MAX_CONTENT_NUMBER); 
    // ����
//    if ( m_pGridView == NULL )
//		 m_pGridView = new CGrid;
	m_pGridView.Create ( this, CRect( m_sRect.left, m_sRect.top + WND_TITLE_HEIGHT,142,159 ),
		                  TBS_SPACELINE|WS_VISIBLE, 0, 2, MODIFYCONTENT,FALSE );
	UINT16 width[] = { 20, 122 };
	m_pGridView.SetColsWidth ( width ); 
    
	m_First=TRUE;
/*	char szTemp[10];  syj modify
	char *szItem[2];
	char pUnitStyle[] = { 0 ,0};
    UINT32 pIconID[] = { 0 ,0};
    szItem[0] = new char[16];    
	szItem[1] = new char[SMS_CONTENT_MAX_NUMBER];

	for(short i=1; i<=g_nNumber; i++) {   	   
	   pTemp = GetContentPointer(g_Content_locked[i-1]); 
	   if( pTemp->iModelID==ModelFlagID) {   
		   sprintf ( szTemp,"%d",i);
           strcpy ( szItem[0], szTemp);   ///���
           strcpy ( szItem[1], pTemp->szContent); ///����		   
		   m_pGridView.AddRow(szItem, pUnitStyle, pIconID); 		     
	   } 	   
	}  
	delete []szItem[0];
	delete []szItem[1];*/

	CRect rc ( m_sRect.left + 75, 0, m_sRect.left + 109, SMS_APP_TITLE_HEIGHT );    
	m_pNewButton.Create ( "�½�", rc, this, Content_NewButton );
	return TRUE;
}
///////////���
BOOL CContentWnd::OnSMSaveAs(short ModelID, char *content)
{   
   CONTENT cc,*pTemp;
   //int  nModel = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);///���ҳ�������
   int nContent = FindContent(ModelID, g_Content_locked, MAX_CONTENT_NUMBER);
   strcpy(cc.szContent , content);	
   for(short i = 0; i< nContent;  i++){
	   pTemp = GetContentPointer(g_Content_locked[i]);
	   if(strcmp(cc.szContent , pTemp->szContent)==0){
	   ::MessageBox("�˳����������Ѵ���!" , "���......",MB_OK|MB_ICONSTOP);
	   return FALSE;
	   }
   }   
   short ContentID = AppendContent(&cc);
   if(ContentID >=0){			 
      cc.iModelID = ModelID;  // �������ID
	  cc.iDataID =  ContentID;    // ���������ID
	  g_Content_locked[g_nNumber++] = ContentID;			 
	  bool modify = ModifyContent(ContentID, &cc);		 
   }else
	  MessageBox("�ռ�����!","���......" ,  MB_OK|MB_ICONEXCLAMATION);
}
/////////////// �½�һ����������
UINT32 CContentWnd::OnNewContent(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    CONTENT  cc, *pTemp;
    short ContentID;    
	char  content[SMS_CONTENT_MAX_NUMBER];
 
	int g_nAllNumber = FindAllContent(g_Content_locked, MAX_CONTENT_NUMBER);
	if( g_nAllNumber == MAX_CONTENT_NUMBER){
		MessageBox("�洢������!" , "�½���������",MB_OK|MB_ICONSTOP);
		return FALSE;
	}    
	g_nNumber = FindContent(ModelFlagID, g_Content_locked, MAX_CONTENT_NUMBER);
	content[0] = '\0';
	if(::InputBox("���볣��������", content , SMS_CONTENT_MAX_NUMBER)== IDOK)
	{
		memset(&cc, 0 ,sizeof(cc));
		if(content[0]== '\0'){
			MessageBox("���ݲ���Ϊ��" , "�½���������",MB_OK|MB_ICONSTOP);
			return FALSE;
		}
		strcpy(cc.szContent , content);	
		for(short i = 0; i< g_nNumber; i++)	{
			pTemp = GetContentPointer(g_Content_locked[i]);
			if(strcmp(cc.szContent , pTemp->szContent)==0) {
			   ::MessageBox("�˳����������Ѵ���!" , "�½���������",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}   
		ContentID = AppendContent(&cc);
		if(ContentID >=0){			 
		   cc.iModelID = ModelFlagID;  // �������ID
		   cc.iDataID =  ContentID;    // ���������ID
		   g_Content_locked[g_nNumber++] = ContentID;			
		   bool modify = ModifyContent(ContentID, &cc);		 
		}
		else
			MessageBox("�ռ�����","�½���������" ,  MB_OK|MB_ICONEXCLAMATION);
	}	  
	 OnPaint(CDC());      	 
}

void CContentWnd::OnModifyContent(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    UINT32  Row=lParam & 0x0000ffff;
	UINT32  Col=(lParam>>16) & 0x0000ffff;
	CONTENT   *pTemp;
	
	if(Col==2){ 

	   ContentNO = Row-1;  ////
       pTemp = GetContentPointer(g_Content_locked[(short)ContentNO]);
       
	   if(ModelCall== WYCALL) {
	      if(!m_ContentList)
	          m_ContentList = new CContentListWnd;
	      m_ContentList->Create( "�鿴", WS_NORMAL | WS_CHILD, CRect(0 ,  0, 159 , 159) ,HELP_SMS_COLLECTION_PHRASE);		
	      m_ContentList->ShowWindow ( );
		
		}else if(ModelCall== SYJCALL){		   
			strcpy(g_ModelSend, pTemp->szContent );
			ModelCall = ENDCALL;
            Destroy();
		    m_pMODELWnd->Destroy();
		}
	}  
} 

void CContentWnd::Destroy ( )
{
	if ( m_ContentList != NULL ) {
		delete m_ContentList;
		m_ContentList = NULL;
	}

	CFrameWnd::Destroy();
}	

void CContentWnd::OnPaint ( CDC &dc )
{ 
	CONTENT *pTemp;
 	//if(!m_First){	
	   char szTemp[10]; 
	   char *szItem[2];
	   char pUnitStyle[] = { 0 ,0};
       UINT32 pIconID[] = { 0 ,0};
       szItem[0] = new char[16];   
	   szItem[1] = new char[SMS_CONTENT_MAX_NUMBER];

       g_nNumber = FindContent(ModelFlagID, g_Content_locked, MAX_CONTENT_NUMBER); 
	   for(short i=1; i<=g_nNumber+1; i++)	//ɾ��	    	   
	        m_pGridView.DeleteRow(1);   
	   for(short i=1; i<=g_nNumber; i++){   //�ػ�	
		   pTemp = GetContentPointer(g_Content_locked[i-1]); 
	       if( pTemp->iModelID==ModelFlagID) {   
		       sprintf ( szTemp,"%d",i );
               strcpy ( szItem[0], szTemp);   ///���


			   //////���ñ��� syj 2002.1.15
			   char szTemp1[30];
			   int j;
			   if ( strlen ( pTemp->szContent ) > 14 )
			   {
					for ( j = 0;j < 14;j ++ )
					{
						if ( pTemp->szContent[j] & 0x80 )
						{
							if ( j > 13 ) break; 
							szTemp1[j] = pTemp->szContent[j];
							szTemp1[j+1] = pTemp->szContent[j+1];
							j ++;
						
						}else szTemp1[j] = pTemp->szContent[j];
					}

					szTemp1[j] = '\0';
					strcpy ( szItem[1], szTemp1); ///����	
				
			   }else strcpy ( szItem[1],pTemp->szContent );

               	   
		       m_pGridView.AddRow(szItem, pUnitStyle, pIconID); 		     
			} 	   
		}  
     	delete []szItem[0];	 
		delete []szItem[1];	 
//	}	 
	//m_First=FALSE;
	CAppWnd::OnPaint ( dc );
}
/////////    class CContentWnd          02_������_����         ʵ�ֺ���  end   wangyu ////

/////////    class CContentListWnd      02_������_����_�鿴    ʵ�ֺ���  start   wangyu /////
BEGIN_MESSAGE_MAP(CContentListWnd, CAppWnd) 
    ON_MESSAGE ( ConList_DeleteButton, CN_COMMAND, OnDelete )
    ON_MESSAGE ( ConList_ModifyButton, CN_COMMAND, OnModify )
    ON_MESSAGE ( ConList_SendButton,   CN_COMMAND, OnSend )   
END_MESSAGE_MAP   

 CContentListWnd::CContentListWnd()
{ 
//	m_pGridView=NULL;
}  
 CContentListWnd::~CContentListWnd()
{ 
}  
BOOL  CContentListWnd :: OnCreate()
{
    if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;
	CONTENT *pTemp;
	pTemp = GetContentPointer(g_Content_locked[(short)ContentNO]);
	////���� �༭�ؼ�
	CRect rc(m_sRect.left + 1, m_sRect.top + WND_TITLE_HEIGHT + 1, 159, 159);
	char szBuffer[256];
	//strcpy ( szBuffer,"<����>" );  syj modify
	strcpy ( szBuffer,pTemp->szContent );
	m_Edit.Create ( szBuffer, rc, this, WS_VISIBLE, LISTCONTENT, 147 );
	////////////��ť
	CRect Delrc ( m_sRect.left + 44,  0, m_sRect.left + 76,  SMS_APP_TITLE_HEIGHT );
	CRect Modrc ( m_sRect.left + 77,  0, m_sRect.left + 109, SMS_APP_TITLE_HEIGHT );
	CRect Sedrc ( m_sRect.left + 110, 0, m_sRect.left + 142, SMS_APP_TITLE_HEIGHT );
    
	m_pListDelete.Create ( "ɾ��", Delrc, this, ConList_DeleteButton );
	m_pListModify.Create ( "�޸�", Modrc, this, ConList_ModifyButton );
	m_pListSend.Create   ( "����", Sedrc, this, ConList_SendButton );
}
 
///////////  ɾ��һ����������
UINT32 CContentListWnd :: OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ) 
{  
 	short Item= (short)ContentNO; 
    
	if(MessageBox("ȷ��ɾ��������������?","ɾ��",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		CONTENT cc,*pTemp;
		pTemp = GetContentPointer(g_Content_locked[Item]);    
		g_nNumber = FindContent(ModelFlagID, g_Content_locked, MAX_CONTENT_NUMBER);  

		
		bool del = DeleteContent(pTemp->iDataID);	  
		 g_nNumber--;	   
   
		 for(short i = Item; i<= g_nNumber; i++)	    
			 g_Content_locked[i] =  g_Content_locked[i+1]; 
 
		 m_Edit.SetWindowText ( "" ); 

		 Destroy ( );
	}


   return TRUE;
}
UINT32 CContentListWnd :: OnModify ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ) 
{
    char szTemp[SMS_CONTENT_MAX_NUMBER];
	char Buffer[256];      
    CONTENT cc, *pTemp;

	pTemp = GetContentPointer(g_Content_locked[(short)ContentNO]);    
	g_nNumber = FindContent(ModelFlagID, g_Content_locked, MAX_CONTENT_NUMBER);    
     
	strcpy(szTemp, pTemp->szContent);	 
	if(InputBox ( "������������",szTemp,140 ) == IDOK )
	{   
		if(szTemp[0]== '\0'){
			MessageBox("���ݲ���Ϊ��!" , "�޸�",MB_OK|MB_ICONSTOP);
			return FALSE;
		}

		cc = *pTemp;
        strcpy ( cc.szContent, szTemp );
        
		/*for(short i = 0; i< g_nNumber; i++)	{
			
			pTemp = GetContentPointer(g_Content_locked[i]);

			if(strcmp(cc.szContent , pTemp->szContent)==0){
				MessageBox("�������Ѵ���!" , "�޸�",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		} */  

		cc.iModelID = ModelFlagID;
		bool modify = ModifyContent(g_Content_locked[(short)ContentNO], &cc);
        ///////�ػ�
	    //strcpy ( Buffer,"<����>");
	    strcpy ( Buffer,szTemp);
		m_Edit.SetWindowText ( Buffer );
 	    PaintChildWnd();	 
	}   
	return TRUE;
}
///////////���͸���Ӣ��
UINT32 CContentListWnd :: OnSend ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ) 
{
    CONTENT   *pTemp = GetContentPointer(g_Content_locked[(short)ContentNO]);    
      
    if ( m_pModifyLetter == NULL )
		 m_pModifyLetter = new CModifyLetterWnd;
	//////���ô��ڵ�һЩ����	  
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	g_bIsModel = TRUE;
	strcpy(g_ST.szMsg , pTemp->szContent);
	strcpy(g_ST.szPhone,"�����������");
	m_pModifyLetter->SetFlag ( false ); 
	g_CurrentID = -1;////��ʼ��ȫ������IDֵ
	m_pModifyLetter->Create("д����Ϣ", 
					        WS_NORMAL|WS_CHILD, 
					        CRect(0 ,  0, 159 , 159) , 
							HELP_SMS_WRITE);

	m_pModifyLetter->ShowWindow ( );
  return TRUE;
}

void CContentListWnd::Destroy ( )
{	 
	CFrameWnd::Destroy();
} 
void CContentListWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );  	 
} 
////////     class CContenListtWnd      02_������_����_�鿴     ʵ�ֺ���  end   wangyu /////
////////
////////     ���������
////////////////////////////////////// 
 
//////////////////////////////////////////////////////////////////////
/////////
/////////   class CGRPSDWnd  01_Ⱥ��     ʵ�ֺ���  start   wangyu //// 
/////////
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGRPSDWnd, CAppWnd) 
	ON_MESSAGE(GRPSD_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage  
	ON_MESSAGE(GRPSD_NewButton,CN_COMMAND,OnGRPSDNewBtn)
	ON_MESSAGE(GRPSD_ComBox,CN_COMMAND,OnShowMenuBtn) 
	ON_MESSAGE(GRPSD_Delete,CN_COMMAND,OnDelete)///ɾ��  
    ON_MESSAGE(GRPSD_Rename,CN_COMMAND,OnRename)///����  
    ON_MESSAGE(GRPSD_Taxis,CN_COMMAND,OnTaxis)///����
END_MESSAGE_MAP

CGRPSDWnd::CGRPSDWnd()
{	/////��ʼ������
//	m_pGrpSdListView = NULL; 
    m_pLeaderWnd = NULL,
	m_pDeleteWnd = NULL,
	m_pRenameWnd = NULL,
	m_pTaxisWnd = NULL;
}

CGRPSDWnd::~CGRPSDWnd()
{
}

BOOL CGRPSDWnd::OnCreate ( )
{    
    if ( !CAppWnd::OnCreate ( ) )
		return FALSE;
	////////����ListView�ؼ�
//	if(m_pGrpSdListView==NULL) 
//	m_pGrpSdListView = new CListView;
	m_pGrpSdListView.Create ( CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT,
								159,159 ),this,GRPSD_LISTVIEW );
	///////����Ⱥ����
	ModifyFlag=TRUE;
	for(int i=0; i<GroupNumber; i++){		 
        m_pGrpSdListView.AppendItem ( g_pGroup[i].szName,ICON_SMMAINLV03 ); 
	}
	//////////�����˵�������ť
	m_GrpSdMB.Create("Ⱥ��",CRect (m_sRect.left ,
		                           m_sRect.top  , 
								   m_sRect.left + TITLE_MENU_WIDTH - 1, 
								   m_sRect.top + TITLE_BAR_HEIGHT - 1),
    								this,GRPSD_ComBox);
    //////////���������˵�ѡ��	
	if(!m_GrpSd.Create(this))
		return FALSE;
	m_GrpSd.AppendItem("Ⱥ������" ,GRPSD_Rename, 0);
	m_GrpSd.AppendItem("Ⱥ��ɾ��" ,GRPSD_Delete, 0);
	m_GrpSd.AppendItem("Ⱥ������" , GRPSD_Taxis,0);  
		//////�����½���ť
	CRect rc ( m_sRect.left + 75,  0,
			   m_sRect.left + 109, SMS_APP_TITLE_HEIGHT );
	m_pGSNewButton.Create ( "�½�",rc,this,GRPSD_NewButton ); 
  return TRUE;
}

////////////��ʾ��Ⱥ���������˵��е�ѡ��
void CGRPSDWnd::OnShowMenuBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{ 
	m_GrpSd.PopupMenu ( m_sRect.left,
				        m_sRect.top + WND_TITLE_HEIGHT );
}           
///////// ����"������"����
void CGRPSDWnd::OnRename( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{   
	if(GroupNumber > 0){
       GrpSDModifyFlag =  0;
	   if(!m_pRenameWnd)
	       m_pRenameWnd = new CGroup_ModWnd;
	       m_pRenameWnd->Create( "����", 
		     	              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
			                  CRect(0 ,  0, 159 , 159) , HELP_SMS_SEND_GROUP_RENAME);		
	   m_pRenameWnd->ShowWindow ( ); 			
	}
}
////////// ����"���ɾ��"����
void CGRPSDWnd::OnDelete( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	if(GroupNumber > 0){
       GrpSDModifyFlag =  1;
	   if(!m_pDeleteWnd)
	      m_pDeleteWnd = new CGroup_ModWnd;
	      m_pDeleteWnd->Create( "ɾ��", 
			              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
			              CRect(0 ,  0, 159 , 159) , HELP_SMS_SEND_GROUP_DEL);	
	   m_pDeleteWnd->ShowWindow ( ); 
	}
}
////////// ����"�������"����
void CGRPSDWnd::OnTaxis( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	if(GroupNumber > 0){
	   GrpSDModifyFlag =  2;
	   if(!m_pTaxisWnd)
	      m_pTaxisWnd = new CGroup_ModWnd;
	      m_pTaxisWnd->Create( "����", 
			              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD| WS_OKBOX, 
			             CRect(0 ,  0, 159 , 159) , 0);	
	   m_pTaxisWnd->ShowWindow ( ); 
	}
}
//////////////�½�Ⱥ��
BOOL CGRPSDWnd::OnGRPSDNewBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
    char GroupName[11];
	if( GroupNumber == MAX_GROUP_NUMBER)	{
		MessageBox("Ⱥ��洢�ռ����� !","�½�Ⱥ��",MB_OK);
		return FALSE;
	} 
	GroupName[0] = '\0';
	if(::InputBox("����Ⱥ������", GroupName , 10)== IDOK)
	{
	     if(GroupName[0]== '\0') {
			MessageBox("Ⱥ�����Ʋ���Ϊ��!" , "�½�Ⱥ��",MB_OK|MB_ICONSTOP);
			return FALSE;
		 }	
		 ////////////
         for( short i = 0; i< GroupNumber; i++){
			  
			  if( strcmp(GroupName , g_pGroup[i].szName)==0){
				 MessageBox("Ⱥ�鲻������!" , "�½�Ⱥ��",MB_OK|MB_ICONSTOP);
				 return FALSE;
			 }
		 }
		 ////////////

		 strcpy(g_pGroup[GroupNumber].szName , GroupName);	
	     GroupNumber++; 
		 ///  save to flash
	     for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	     SaveGroupSend(g_pGroup);
	     OnPaint(CDC());	
	}
	return TRUE;
}
///////////////////ĳȺ�鱻����
void CGRPSDWnd::OnLVCommand ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
   UINT32 iItem = ( UINT32 )lParam;
   LEADERFLAG = ( UINT32 )lParam;

   if(GroupCall==WYCALL) {

	  if(!m_pLeaderWnd)
	      m_pLeaderWnd = new CLeaderWnd;
	  m_pLeaderWnd->Create( g_pGroup[iItem].szName, 
	 		                WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | 
							WS_CHILD   | WS_OKBOX, 
						    CRect(0 ,  0, 159 , 159) , HELP_SMS_SEND_GROUP_EDIT);		
	  
	  m_pLeaderWnd->ShowWindow ( );

   }else if(GroupCall==SYJCALL)  {   

	  g_pGroupSend = g_pGroup[iItem]; ///// ����Ӧ���ݸ���Ӣ��
      GroupCall = ENDCALL;
	  Destroy();
   }
}
 
void CGRPSDWnd::OnPaint ( CDC &dc )
{  
  if(!ModifyFlag)  {
      m_pGrpSdListView.DeleteAllItems();
	  for(int i=0;i<GroupNumber ; i++)
	  	  if ( strlen ( g_pGroup[i].szName ) > 0 )
			  m_pGrpSdListView.AppendItem ( g_pGroup[i].szName,ICON_SMMAINLV03); 
		  else continue;
  }

  ModifyFlag=FALSE; 
  CAppWnd::OnPaint ( dc );
}

BOOL CGRPSDWnd::OnDestroy ( )

{ 
/*	if ( m_pGrpSdListView != NULL )  {
		delete m_pGrpSdListView;
		m_pGrpSdListView = NULL;
	}
*/

	g_bIsGroup = FALSE;

	if ( m_pLeaderWnd != NULL ) {
		delete m_pLeaderWnd;
		m_pLeaderWnd = NULL;
	}
	if ( m_pDeleteWnd != NULL ) {
		delete m_pDeleteWnd;
		m_pDeleteWnd = NULL;
	}
	if ( m_pRenameWnd != NULL )	{
		delete m_pRenameWnd;
		m_pRenameWnd = NULL;
	}
	if ( m_pTaxisWnd != NULL ) 	{
		delete m_pTaxisWnd;
		m_pTaxisWnd = NULL;
	}
	return CAppWnd::OnDestroy ( );
} 
/////////    class CGRPSDWnd  01_Ⱥ��     ʵ�ֺ���  end   wangyu ////

/////////    class CLeaderWnd  01_Ⱥ��_�޸�     ʵ�ֺ���  start   wangyu ////
BEGIN_MESSAGE_MAP(CGroup_ModWnd, CAppWnd) 
    ON_MESSAGE ( GRPSDModify_LISTVIEW,CN_COMMAND,OnModifyGroup )  
    ON_MESSAGE ( GRPSD_TaxisUpButton,CN_COMMAND,OnTaxisGroupUp )
	ON_MESSAGE ( GRPSD_TaxisDwButton,CN_COMMAND,OnTaxisGroupDw )
END_MESSAGE_MAP 
////////  ���� Ⱥ���޸� ��ѡ��
UINT32 CGroup_ModWnd::OnModifyGroup(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    switch(GrpSDModifyFlag)	{
	    case 0:  // ����	         
		      OnRenameGroup(0,0,lParam);
		      break;	
	    case 1:  // ɾ��		   
		      OnDeleteGroup(0,0,lParam);
		      break;	     	
	    default : 
		        break;	 
	}
}
//////////////////  ɾ��Ⱥ��  
UINT32 CGroup_ModWnd::OnDeleteGroup(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    UINT32 uItem =  lParam; 	 
	
	if(MessageBox("ȷ��ɾ����Ⱥ����?","ɾ��Ⱥ��",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		int i;

		for( i=uItem; i<GroupNumber-1 ; i++)	 
			 g_pGroup[i]=g_pGroup[i+1] ;  
		memset ( &g_pGroup[GroupNumber-1],0,sizeof ( GROUP ) );

		GroupNumber--; // Ⱥ�������1 
		// save to flash
		for( i=0; i<GroupNumber; i++)
			g_pGroup[i].szGroupNumber = GroupNumber;
		SaveGroupSend(g_pGroup);

		m_pGrpModifyListView.DeleteAllItems(); //// modify		
		for( i=0; i<GroupNumber ; i++)
		{
			if ( strlen ( g_pGroup[i].szName ) > 0 )
				m_pGrpModifyListView.AppendItem (g_pGroup[i].szName, 0,NULL,ICON_GROUPSEND_LEADER2,0);
			else continue;
		}
	}

    OnPaint(CDC());
    return TRUE;
}
//////////////////  Ⱥ����� 
UINT32 CGroup_ModWnd::OnRenameGroup(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{   
	char SetTemp[10];    
	UINT32 uItem =lParam;  
	
	strcpy ( SetTemp, g_pGroup[uItem].szName );
	if (InputBox ( "����������",SetTemp,10 ) == IDOK )	{		 
        
		/*for( short i = 0; i< GroupNumber; i++){			  
		     if( strcmp(SetTemp , g_pGroup[i].szName)==0){
				 MessageBox("Ⱥ�鲻������!" , "�½�Ⱥ��",MB_OK|MB_ICONSTOP);
				 return FALSE;
			 }
		}*/
		
		strcpy ( g_pGroup[uItem].szName, SetTemp );
  	   	m_pGrpModifyListView.DeleteAllItems(); //// modify		
		for(int i=0; i<GroupNumber ; i++)
			m_pGrpModifyListView.AppendItem (g_pGroup[i].szName, 0,NULL,ICON_GROUPSEND_LEADER2,0);
   		
		OnPaint(CDC());	
	}
	// save to flash
	for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	SaveGroupSend(g_pGroup);	
  	return TRUE;     
}

//////////////////  Ⱥ������ 
UINT32 CGroup_ModWnd::OnTaxisGroupUp(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{ 
	  GROUP *temp;   
	temp = new GROUP;
    INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pGrpModifyListView.GetSelItem();   
 
	if(uItem == -1 )	{ 
	    MessageBox("����ѡ��Ҫ������Ⱥ��!","��ʾ",MB_OK);
	    return FALSE;
	}
    if(uItem==0) {	
		MessageBox("��ѡ�е�Ⱥ���Ѿ��ǵ�һ��!","��ʾ",MB_OK);
		return FALSE;
    }   
	m_pGrpModifyListView.GetItem(&list1 , uItem-1);
	m_pGrpModifyListView.GetItem(&list2, uItem);
	m_pGrpModifyListView.ModifyItem(&list2, uItem-1 , FALSE);
	m_pGrpModifyListView.ModifyItem(&list1, uItem, FALSE);
		*temp = g_pGroup[uItem] ;	
     g_pGroup[uItem] = g_pGroup[uItem-1] ;
     g_pGroup[uItem-1] = *temp ;
	 delete temp;
	m_pGrpModifyListView.SetSelItem(uItem-1);
	nItemStart = m_pGrpModifyListView.GetStartItem();
	nScreenRows = m_pGrpModifyListView.GetScreenItems();
	if(uItem > nItemStart + nScreenRows)
		m_pGrpModifyListView.ScrollTo(uItem -nScreenRows , TRUE);
	else if(uItem <= nItemStart)
		m_pGrpModifyListView.ScrollTo(uItem -1, TRUE);
	m_pGrpModifyListView.OnPaint(CDC());
 return TRUE; 
}
///////////////////// ����Ⱥ��  
UINT32 CGroup_ModWnd::OnTaxisGroupDw(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{   GROUP *temp;    
	temp = new GROUP;
    INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pGrpModifyListView.GetSelItem();   
 
	if(uItem == -1 )	{
		MessageBox("����ѡ��Ҫ�����ĳ�����!","��ʾ",MB_OK);
	    return FALSE;
	}
    if(uItem==(GroupNumber-1)) {    ////���һ��	
		MessageBox("��ѡ�еĳ������������һ��!","��ʾ",MB_OK);
		return FALSE;
    } 

	m_pGrpModifyListView.GetItem(&list1 , uItem+1);
	m_pGrpModifyListView.GetItem(&list2, uItem);
	m_pGrpModifyListView.ModifyItem(&list2, uItem+1 , FALSE);
	m_pGrpModifyListView.ModifyItem(&list1, uItem , FALSE);
	*temp = g_pGroup[uItem];	
	 g_pGroup[uItem] = g_pGroup[uItem+1];
     g_pGroup[uItem+1] = *temp ; 	
	delete  temp;	

	m_pGrpModifyListView.SetSelItem(uItem+1);
	nItemStart = m_pGrpModifyListView.GetStartItem();
	nScreenRows = m_pGrpModifyListView.GetScreenItems();
	
	if(uItem >= nItemStart + nScreenRows -1)
		m_pGrpModifyListView.ScrollTo(uItem+2 - nScreenRows , TRUE);
	else if(uItem < nItemStart -1 )  
		m_pGrpModifyListView.ScrollTo(uItem+1, TRUE);
	m_pGrpModifyListView.OnPaint(CDC());
 return TRUE; 
}
/////////// �ƶ�OK
void CGroup_ModWnd::OnOk(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{   /// save to flash
	for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	SaveGroupSend(g_pGroup); 
	Destroy();
}
///////////////////
BOOL CGroup_ModWnd:: OnCreate()
{
	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;
//	if(m_pGrpModifyListView==NULL) 
 //	m_pGrpModifyListView = new CListView;  
	
	switch(GrpSDModifyFlag)	{
	case 0:  /////����	
	case 1:  /////ɾ��
	       m_pGrpModifyListView.Create ( CRect ( m_sRect.left , m_sRect.top + WND_TITLE_HEIGHT,159,159 ), 
			                              this,  GRPSDModify_LISTVIEW );
	       break;		    
	case 2:  /////����
	       m_pGrpModifyListView.Create ( CRect ( m_sRect.left , m_sRect.top + WND_TITLE_HEIGHT, 159,159 ),
								          this, LS_SMALLICON, GRPSDModify_LISTVIEW );
	       m_pGSUpButton.Create ( "����",CRect( m_sRect.left + 58, 0, m_sRect.left + 91,  SMS_APP_TITLE_HEIGHT ),
		   						  this,GRPSD_TaxisUpButton );  
		   m_pGSDwButton.Create ( "����",CRect( m_sRect.left + 92, 0, m_sRect.left + 125, SMS_APP_TITLE_HEIGHT ),
								  this,GRPSD_TaxisDwButton );  
		   break;
	default : 
		break;	 
	} 
	m_pGrpModifyListView.SetStatus(LS_SMALLICON, TRUE);     
	 
	for(int i=0;i<GroupNumber ; i++)
  	    m_pGrpModifyListView.AppendItem (g_pGroup[i].szName, 0,NULL,ICON_GROUPSEND_LEADER2,0);     
 	return TRUE;
}

CGroup_ModWnd::CGroup_ModWnd()
{
//	m_pGrpModifyListView = NULL;	 
}

CGroup_ModWnd::~CGroup_ModWnd()
{ 
}  

void CGroup_ModWnd::Destroy ( )
{	 
	CFrameWnd::Destroy();
}
 
void CGroup_ModWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );  	 
}
 
/////////    class CLeaderWnd  01_Ⱥ��_�޸�     ʵ�ֺ���  start   wangyu ////


/////////    class CLeaderWnd  01_Ⱥ��_�쵼     ʵ�ֺ���  start   wangyu ////
BEGIN_MESSAGE_MAP(CLeaderWnd, CAppWnd) 
 	ON_MESSAGE ( MODIFYLEADER_EDIT,CN_GRIDSEL,OnModifyLeader )
	ON_MESSAGE ( MODIFYLEADER_SEND,CN_COMMAND,OnSend )
END_MESSAGE_MAP

BOOL CLeaderWnd:: OnCreate()
{
	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;
	
	m_SendBtn.Create ( "����",
		CRect ( m_sRect.left + 78,
		        m_sRect.top,
				m_sRect.left + 110,
				m_sRect.top + SMS_APP_TITLE_HEIGHT ),
				this,MODIFYLEADER_SEND );
	
	// ����
	m_pGridView.Create ( this,CRect ( m_sRect.left,
							           m_sRect.top + WND_TITLE_HEIGHT ,
								       142,159 ),
			                           TBS_HAVFIXROW | 
									   UNS_TEXT | WS_CHILD | TBS_SPACELINE,
			                           10,2,MODIFYLEADER_EDIT,FALSE );
	UINT16 width[] = { 20, 122 };
	m_pGridView.SetColsWidth ( width ); 
    int j=0;

	for(int i=1; i<10; i+=2){
      m_pGridView.SetUnitText(i, 2, g_pGroup[LEADERFLAG].Numbers[j].szName,0);//��	  
	  m_pGridView.SetUnitText(i, 1, 0,ICON_GROUPSEND_NameCard);//ͼ��	  
	  m_pGridView.SetUnitText(i+1,2,g_pGroup[LEADERFLAG].Numbers[j].szPhone,0);//����

	  j++;
	}

	return TRUE;
}
////ֱ�ӷ��� 
void  CLeaderWnd::OnSend ( UINT32 nMessage,UINT32 nSrcid,UINT32 lParam )
{
	
	//////���ô��ڵ�һЩ����	  
	int len;
	///////////����ת��
	strcpy ( g_ST.szPhone,"" );
	for ( int i = 0;i < 5;i ++ )
	{
		if ( strlen ( g_pGroup[LEADERFLAG].Numbers[i].szPhone ) == 0 )
			continue;
		strcat ( g_ST.szPhone,g_pGroup[LEADERFLAG].Numbers[i].szPhone );
		
		len = strlen ( g_ST.szPhone );
		if ( g_ST.szPhone[len-1] == '#' )
			g_ST.szPhone[len-1] = '\0';
		
		len = strlen ( g_ST.szPhone );
		if ( g_ST.szPhone[len-1] == '*' )
			strcat ( g_ST.szPhone,"0" );

		len = strlen ( g_ST.szPhone );
		if ( g_ST.szPhone[len-2] != '*' )
			strcat ( g_ST.szPhone,"*0#" );
		else strcat ( g_ST.szPhone,"#" );
	}

	len = strlen ( g_ST.szPhone );
	if ( !len )
	{
		MessageBox ( "����Ϊ�գ�","����",MB_OK|MB_ICONSTOP );
		return;
	}

	if ( g_ST.szPhone[len-1] == '#' )
		g_ST.szPhone[len-1] = '\0';

	strcpy(g_ST.szMsg , "<�����������������>" );


	if ( m_pModifyLetter == NULL )
		 m_pModifyLetter = new CModifyLetterWnd;
	m_pModifyLetter->SetFlag ( false ); 
	m_pModifyLetter->Create("д����Ϣ", 
					        WS_NORMAL|WS_CHILD, 
					        CRect(0 ,  0, 159 , 159) , HELP_SMS_WRITE);
	m_pModifyLetter->ShowWindow ( );

}
 
UINT32 CLeaderWnd::OnModifyLeader(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{   
    UINT32  Row=lParam & 0x0000ffff;
	UINT32  Col=(lParam>>16) & 0x0000ffff;	
	int i=-1;

	if(Col==1)
	{  //��Ƭ--- ������ӿ�   
       switch(Row)
		{			
		  case 1:  i=0;  OnCallFormCard(i);				  
		   	    break;		 
		  case 3:  i=1;  OnCallFormCard(i); 			       
				break;		  
		  case 5:  i=2;  OnCallFormCard(i);				  
			    break;		  
		  case 7:  i=3;  OnCallFormCard(i);
				break;
		  case 9:  i=4;  OnCallFormCard(i);
			    break;
          default:
			  break;
	   }
	    PaintChildWnd();
	}
	else if(Col==2)
	{	   
		switch(Row)
		{			
		  case 1:  i=0;  OnModifyName(i);
		   	       break;		 
		  case 3:  i=1;  OnModifyName(i);
				   break;		  
		  case 5:  i=2;  OnModifyName(i);
			       break;		  
		  case 7:  i=3;  OnModifyName(i);
			       break;		  
		  case 9:  i=4;  OnModifyName(i); 				 
			       break;
		  case 2:  i=0;  OnModifyPhone(i);
			       break;
		  case 4:  i=1;  OnModifyPhone(i);
			       break;
		  case 6:  i=2;  OnModifyPhone(i);
			       break;
		  case 8:  i=3;  OnModifyPhone(i);
			       break;
		  case 10: i=4;  OnModifyPhone(i); 				   
			   	   break;
		  default:
			       break;
		}//end switch			 
		 PaintChildWnd();
	}//end else if
  return TRUE;	 
}
///////////// ����Ƭ��ѡ�� �������������
BOOL CLeaderWnd:: OnCallFormCard(short mNameNO )
{ 
	char number[16];
	CARDFIELD cTelType;   	 
		
	

    INT16 cardid = SelectTelNum(number, 16, cTelType);
	if(cardid >=0)	{	   
	   strcpy(g_pGroup[LEADERFLAG].Numbers[mNameNO].szName, GetCardPointer(cardid)->Name);
	   strcpy(g_pGroup[LEADERFLAG].Numbers[mNameNO].szPhone,  number);
	   Repaint();
	}  
	/// save to flash
	for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	SaveGroupSend(g_pGroup);
	return TRUE;
}
///////////// �ֶ��������
BOOL CLeaderWnd::OnModifyName(short mNameNO)
{
	char SetTemp[32];

    
	strcpy ( SetTemp, g_pGroup[LEADERFLAG].Numbers[mNameNO].szName   );   
    if ( InputBox ( "������������",SetTemp,10 ) == IDOK ) {
	     strcpy ( g_pGroup[LEADERFLAG].Numbers[mNameNO].szName , 
			 SetTemp );   
	     Repaint();
	}	
    for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	SaveGroupSend(g_pGroup);
	return TRUE;
} 
////////// �ֶ���Ӻ���
BOOL CLeaderWnd::OnModifyPhone(short mPhoneNO)
{
	char szTemp[16];

    
    strcpy ( szTemp ,g_pGroup[LEADERFLAG].Numbers[mPhoneNO].szPhone ); 
    if ( NumInputBox ( "�������º���",szTemp,16 ) == IDOK )	{
	     strcpy ( g_pGroup[LEADERFLAG].Numbers[mPhoneNO].szPhone , szTemp );   
	     Repaint();		 	   
	}
 	for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	SaveGroupSend(g_pGroup);
	return TRUE;
}

void CLeaderWnd::Repaint(void)
{
	int j=0;
	for(int i=1; i<10; i+=2) {
        m_pGridView.SetUnitText(i, 2, g_pGroup[LEADERFLAG].Numbers[j].szName);	      	  
	    m_pGridView.SetUnitText(i+1,2,g_pGroup[LEADERFLAG].Numbers[j].szPhone); 
	    j++;
	}	   
}
	  
void CLeaderWnd::OnOk(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
  Destroy(); 
}

CLeaderWnd::CLeaderWnd()
{
// 	m_pGridView=NULL;
}

CLeaderWnd::~CLeaderWnd()
{ 
}  

void CLeaderWnd::Destroy ( )
{
	CFrameWnd::Destroy();
}	

void CLeaderWnd::OnPaint ( CDC &dc )
{
  	CAppWnd::OnPaint ( dc );	
}
/////////    class CLeaderWnd  01_Ⱥ��_�쵼     ʵ�ֺ���  end   wangyu ////

////wangyu1228
///////////////// class CDrawPicWnd   ͼ�ι��� ʵ�ֺ���   start wangyu  ////
BEGIN_MESSAGE_MAP(CDrawPicWnd,CAppWnd)
END_MESSAGE_MAP

CDrawPicWnd:: CDrawPicWnd()
{
	DrawReset();
}

CDrawPicWnd::~CDrawPicWnd()
{
}

BOOL CDrawPicWnd :: OnCrate()
{
}

void CDrawPicWnd::Destroy ( )
{
	CFrameWnd::Destroy();
}

void CDrawPicWnd:: OnPaint(CDC &dc)
{

	CAppWnd::OnPaint(dc);


	UINT8 length;
    UINT8 SubCode = g_pMenuBuffer[2]; 
	INT16 x0 , x1; 
	INT16 y0 , y1;

	switch(SubCode)
	{
	case 0x01:  // ����
	    DrawClear();
        break;
	
	case 0x02:  // ��ͼ��λ		
		x0 = g_pMenuBuffer[4];
		x0 = (x0 << 8) | g_pMenuBuffer[3];
		y0 = g_pMenuBuffer[5];
		
		SetOrigin(x0 , y0);
		break;
	
	case 0x03:  // ����		 
		x0 = g_pMenuBuffer[4];
		x0 = (x0 << 8) | g_pMenuBuffer[3];
		y0 = g_pMenuBuffer[5];
        
		DrawStyle( BLACK, g_pMenuBuffer[3], 1, 0);
		DrawPoint(  x0 + iX0 , y0 + iY0);
		break;
	
	case 0x04:  // ����
		x0 = g_pMenuBuffer[6];
		x0 = (x0 << 8) | g_pMenuBuffer[5];
		y0 = g_pMenuBuffer[7]; 

		x1 = g_pMenuBuffer[9];
		x1 = (x1 << 8) | g_pMenuBuffer[8];
		y1 = g_pMenuBuffer[10];
		 
        DrawStyle( BLACK, g_pMenuBuffer[3], 1, g_pMenuBuffer[4]);
		Drawline(x0, y0, x1,  y1,m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle);
        break;
	
	case 0x05:  // ������
        x0 = g_pMenuBuffer[6];
		x0 = (x0 << 8) | g_pMenuBuffer[5];
		y0 = g_pMenuBuffer[7]; 

		x1 = g_pMenuBuffer[8] + x0;		 
		y1 = g_pMenuBuffer[9] + y0 ;
		 
        DrawStyle( BLACK, g_pMenuBuffer[3], 1, g_pMenuBuffer[4]);
		DrawRect(x0, y0, x1, y1);
		break;
	
	case 0x06:  // ��Kͼ��
		x0 = g_pMenuBuffer[6];
		x0 = (x0 << 8) | g_pMenuBuffer[5];
		y0 = L3Buffer[7]; 

		x1 = g_pMenuBuffer[11] + x0;
		y1 = 159 - g_pMenuBuffer[10];
 
        DrawStyle( BLACK, g_pMenuBuffer[3], 1, g_pMenuBuffer[4]);
		DrawKPoint(x0, y0, x1, y1);
	    break;
	
	case 0x07:  // ��ʾģʽͼ
		break;
	
	case 0x08:  // ��˳���
		length = g_pMenuBuffer[1];
		m_iDrawMode = g_pMenuBuffer[3];

		x0 = g_pMenuBuffer[5];
		x0 = (x0 << 8) | g_pMenuBuffer[4];
	   
		DrawStyle( BLACK, g_pMenuBuffer[3], 1, 0); //
		for(short i=6; i<=length+1 ; i++ ){
		   DrawPoint(x0 , g_pMenuBuffer[i]);
		   x0++;
		}
	    break;
	
	case 0x09:  // ����Բ
        INT16 x, y;
		INT8  a, b;
		x = g_pMenuBuffer[5];
		x = (x << 8) | g_pMenuBuffer[4];
		y = g_pMenuBuffer[6];
		
        a = g_pMenuBuffer[7];
		b = g_pMenuBuffer[8];

		x0 = x - a;
		y0 = y - b;
		
		x1 = x + a;
		y1 = y + b;
		
		DrawStyle( BLACK, g_pMenuBuffer[3], 1, 0);
        DrawEllipse(x0, y0, x1, y1);
	    break;
	
	case 0x0a:  // ����ͼ�λ��ƺ�����
	    break;
	
	case 0x0b:  // ����ͼ�κ�����ͼ����
		break;
	}	
	
} 
//// ���Ը�λ
void CDrawPicWnd::DrawReset()
{   m_iCurX =0; 
    m_iCurY =0; 
	m_iBackColor = WHITE;
    m_iPenColor  = BLACK;
    m_iDrawMode  = DRAWMODE_REP;
    m_iPenWidth  = 1;
    m_iLineStyle = 0;
}
//// ���ñ���ɫ�� 
void CDrawPicWnd::DrawStyle(UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle)
{
	CDC dc;
  m_iPenColor = iColor;//BLACK 0; GRAY  1; LGRAY 2; WHITE 3
  m_iPenWidth = iSize; // 1
  /// ����ģʽ
  if(iAttrib == 0x00)
     dc.SetROP2(DRAWMODE_REP);
  else if(iAttrib == 0x01 )
	  dc.SetROP2(DRAWMODE_AND);
  else if(iAttrib == 0x02 )
	  dc.SetROP2(DRAWMODE_OR);
  else if(iAttrib == 0x03 )
	  dc.SetROP2(DRAWMODE_XOR); 
  ///���û�������
  if(iLineStyle == 0xFF)
     dc.SetLineStyle(0);   //0 ʵ�� 
  else if(iLineStyle == 0x55)
     dc.SetLineStyle(1);  //���� ���� 
  
} 
//// ��������ԭ��
void CDrawPicWnd:: SetOrigin(INT16 x0, INT16 y0) 
{
	 iX0 = x0;
	 iY0 = y0;
}
//// ����
void CDrawPicWnd:: DrawClear()
{
	CDC dc;
	UINT8 iColor = WHITE;
	dc.ClearDevice( iColor );
}
//// ����
void CDrawPicWnd :: DrawPoint(INT16 x0, INT16 y0) 
{
	 CDC dc;
	 dc.SetPixel( x0 ,  y0 );
}
//// ����
void CDrawPicWnd :: Drawline(INT16 x0, INT16 y0, INT16 x1, INT16 y1, 
                    UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle)
 {
    CDC dc;
	dc.MoveTo(x0 , y0);
	dc.LineTo(x1 , y1); 
}
//// ������
void CDrawPicWnd :: DrawRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    CDC dc;
	dc.FrameRect(iLeft, iTop,  iRight, iBottom);
}
//// ��Kͼ��	 
void CDrawPicWnd :: DrawKPoint(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    CDC dc;
	dc.FrameRect(iLeft, iTop,  iRight, iBottom);
}
//// ����Բ
void CDrawPicWnd::DrawEllipse(INT16 x0, INT16 y0, INT16 x1, INT16 y1)
{
	CDC dc;
    dc.Ellipse(x0, y0, x1, y1);
}
//////// class CDrawPicWnd   ͼ�ι���  end    wangyu //////

/////////////����Ϣ����
