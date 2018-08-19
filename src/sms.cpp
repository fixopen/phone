////////////////////////////////////////////////////////////////
/////          短消息头文件
////////////////////////////////////////////////////////////////
#include "sms.h"
#include "keydrv.h"

/////////////////wangyu  start
//struct SetRevFlag     m_CheckFlag;      // wangyu 接收设置复选框
static int            GroupCall = WYCALL; // 用于区分调用者:修改内容(WYCALL)
static int            ModelCall = WYCALL; //                调用内容(SYJCALL)
                                          //                销毁窗口(ENDCALL)                     
////////////群发 wangyu
GROUP               g_pGroup[MAX_GROUP_NUMBER];     /// 存放群组的最终数据                   
GROUP               g_pGroupSend;                   /// 存放宋英剑需要的数据                    
static int          GroupNumber  ;                  /// 已存在的 群组的个数

BOOL                ModifyFlag;                     /// 重画标志
UINT32              GrpSDModifyFlag ;               /// 标识 选中的 下拉菜单 中的 选项
UINT32              LEADERFLAG =-1;                 /// 标识 选中的组
////////////常用语  wangyu
// new  2001.12.12
short               g_Model_Locked[MAX_MODEL_NUMBER];     ///存放 常用语组 对应的ID号 
short               g_Content_locked[MAX_CONTENT_NUMBER]; ///存放 常用语项 对应的ID号 
UINT32              g_nLocal = 0;                         ///存放 常用语组 对应的位置及个数
UINT32              g_nNumber = 0; 
BOOL                m_FirstPaint ;    /// 标识第一次(true)进入OnPaint()函数(false)，用于主窗口的重画
UINT32              ContentNO=NULL;   /// 标识 常用语项中的第 ContentNO 条内容    

BOOL                m_First;
char                g_ModelSend[SMS_CONTENT_MAX_NUMBER];         /// 存放宋英剑需要的数据 
short               ModelFlagID;              /// 标识 选中的常用语组
UINT32              ModelModifyFlag = NULL;   /// 标识 选中的 下拉菜单 中的 选项 

///////////时钟
NU_TIMER			TS,  //计时器
					SYJTS;////
/////////////////////   wangyu  end ////////////////////////////////////


//////////////////SongYingJian start
MONEY					g_Money[SMS_MONEY_MAX_NUMBER],////////话费账单
						g_MoneyEx[SMS_MONEYEX_MAX_NUMBER];////////话费详单

SMS						g_ST,			// 短消息存储结构
						g_SendedSMS[4][SMS_MAX_SAVE_NUMBER];///已发信箱结构定义

SUBSCIBESTRUCT			g_SubscibeSMS[SMS_SUBSCIBE_MAX_NUMBER];//订阅短信息结构
int						g_iSubscribePos = 0;/////标示订阅短信的当前指针

SENDSMS					g_SendST;	/////发送信息存储 
int 					iCurrentLetterBox = 3, ////当前选中的信箱
										// 0 - 3 私人1-3
						iCurrentChildLetterBox = 0;///信箱中子信箱
										// 0 - 2 收信箱、发信箱、已发信箱

BOOL					bIsSuccessSend = FALSE;////标示TS6是否需要显示	
short					g_pSMSDataIDBuf[SMS_LETTER_MAX_NUMBER];
short					g_pSMSDataTempIDBuf[SMS_MAX_SAVE_NUMBER];////存储每个短消息信箱
short					g_CurrentID = 0;/////纪录当前数据的ID号
UINT16					g_iCurrentPos = 0;////纪录当前数组下标
UINT8					g_iServerType = -1;//////服务类型 0-在线信息交互；1-离线信息交互；2-Email信息交互;
BOOL                    g_icommuicationstate = FALSE;  //短消息发送状态标志 FALSE-尚未成功 TRUE-成功 
UINT8					g_pMenuBuffer[256];//////屏幕询问菜单缓冲区
CSaveAsWnd				*m_pSaveAsWnd = NULL;/////另存为窗口
CShowMessageEx			*m_pShowMsg = NULL;////显示信息
BOOL					g_bIsModel = FALSE;////常用语是否已经打开
BOOL					g_bIsGroup = FALSE;////群发是否已经打开
char					g_szHaveNewMessage[80];

///// SongYingJian End

///////公共变量 START
extern SYSTEM_OPTION	g_SystemOption;       ////系统设置
UINT32					g_pL12MsgBuffer[3],     // 物理层与链路层交换参数的缓冲区
						g_pL23MsgBuffer[3];     // 消息层与链路层交换参数的缓冲区
UINT8					L3Buffer[ArrayLength];         // 数据缓冲区

CHelloWorld				*g_pHelloWorld = NULL;  // 应用层全局指针
CSMSMessage				*m_pSMSMessage = NULL;  // 消息层全局指针
CSMSL2Wnd				*m_pSMSL2 = NULL;			// 链路层全局指针
CSmsPhy					*g_pSmsPhy=NULL;            // 物理层全局指针
CReadingSMS				*m_pReadingLetter = NULL;///阅读窗口
CShowMessage			*g_pShowMsg = NULL;///消息窗口
CSMSServiceWnd			*m_pService = NULL; ////家家e设置窗口
CMODELWnd				*m_pMODELWnd = NULL;////常用语
CGRPSDWnd				*m_pGRPSDWnd = NULL;////群发
CModifyLetterWnd		*m_pModifyLetter = NULL;/////新增与修改窗口
CDrawPicWnd				*g_pGraphicsWnd = NULL;//////图形管理模块
extern BOOL             g_pPhoneSmsState;   //true is sms state
BOOL					g_bIsRing = FALSE; ////物理层是否播放音乐
BOOL					g_bIsCanReset = FALSE;//////是否允许切换
///////公共变量 END

////////与名片的接口
extern short SelectTelNum(char * , short , CARDFIELD & );

///////////////////////////////// ////////////////////////////////////////
////    物理层     作 者:向 国 威        xgw
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

/////发送消息
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
//	第一个消息处理函数
//	消息为:Order_Smsdown_Chain_VZ
//	相关参数在 (g_pL12MsgBuffer[0])(g_pL12MsgBuffer[1])
//	DTMF_AA		0x0D 0x0D /////下载成功
//	DTMF_A0		0x0D 0x0A /////不存在该存储部件
//	DTMF_A1		0x0D 0x01 /////存储部件满
//	DTMF_A2		0x0D 0x02 /////私人账号不存在
//	DTMF_A3		0x0D 0x03 /////用户因以外，终止下载
//	DTMF_A4		0x0D 0x04 /////其他，用户拒绝
void CSmsPhy::OnOrderSmsdownChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	UINT8  temp[2];
	temp[0]=g_pL12MsgBuffer[0];
	temp[1]=g_pL12MsgBuffer[1];
	OrderSendDTMFData(temp,2,-1);
}
//	第二个消息处理函数
//	消息为:Order_CAS_ACK_Chain_VZ
//	相关参数在 (g_pL12MsgBuffer[0])
void CSmsPhy::OnOrderCASACKChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	/*UINT8  temp[2];/////syj 2002.2.26
	temp[0]=g_pL12MsgBuffer[0];   
	//temp[1]=g_pL12MsgBuffer[1];
	OrderSendDTMFChainVZ(temp,1); */
}
//	第三个消息处理函数
//	消息为:Order_FSK_ACK_Chain_VZ
//	相关参数在 (g_pL12MsgBuffer[0])(g_pL12MsgBuffer[1])
//	DTMF_D0		0x00 0x0A /////FSK数据传送正确
//	DTMF_D1		0x00 0x01 /////FSK数据传送错误

void CSmsPhy::OnOrderFSKACKChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	UINT8  temp[2];
	temp[0] =  g_pL12MsgBuffer[0];  
	temp[1] =  g_pL12MsgBuffer[1];
	temp[0] &= 0x0f;
	temp[1] &= 0x0f;
	SendDTMFFskAck(temp);
}
//	第四个消息处理函数
//	消息为:Order_Send_DTMF_Chain_VZ
//	相关参数在 L3Buffer[512],数据段最后一位是0x7F

void CSmsPhy::OnOrderSendDTMFChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	int i;
	
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow();
	g_bIsCanReset = TRUE;
	g_pShowMsg->SetMessage ( "正在发送数据 请稍候..." );
	UINT8  temp[ArrayLength];
	for(i=0;( L3Buffer[i]!=0x7F )&&( i<ArrayLength );i++){
		temp[i]=L3Buffer[i];
	}

	OrderSendDTMFData(temp,i,g_iServerType);
}
//	第五个消息处理函数
void CSmsPhy::OnOrderSendFSKChainVZ(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	//给平台用FSK发送数据;
}
//	第六个消息处理函数
//  与平台建立连接
//	g_SystemOption.g_pCenterReceivePhone[32]为中心接入号码
//	g_SystemOption.g_pCenterSendPhone[32]为中心发送号码

void CSmsPhy::OnOrderConnectServerChain(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow();
	g_bIsCanReset = TRUE;
	g_pShowMsg->SetMessage ( "正连接服务器 请稍侯..." );
	SmsDialOut(g_SystemOption.g_pCenterReceivePhone);
}
//	第七个消息处理函数:与平台断开连接
void CSmsPhy::OnOrderDisconnectServerChain(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{

	DisconnectServer(); ///挂机
	if ( g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow( SW_HIDE );
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	
	
	//////通知消息层播放音乐
	if ( g_bIsRing )
	{

		g_bIsRing = FALSE;
		
		///////播放特色振铃
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

//	第八个消息处理函数:定时溢出,需要挂机
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
		g_pShowMsg->SetMessage ( "网络繁忙,请稍候再试!" );
		DisconnectServer();
		NU_Sleep ( 100 );
		if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );

		///////////如果家家e窗口存在将他销毁
		///////////如果家家e窗口存在将他销毁
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
////////////////////////////////物理层 END/////////////////////////////////// 


///////////////////////////////// 链路层
/******************************************************************************/
/*函数名：unsigned char Sms_alternate(unsigned data)                          */
/*作  者：王  钰                                                              */
/*时  间：2001.10.30                                                          */
/*功  能：恢复数据原型：交换数据的高、低位                                    */ 
/*备  注：                                                                    */
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
/*函数名：void Sms_Unpacking( )                                      */
/*作  者：王  钰                                                              */
/*时  间：2001.10.31                                                          */
/*功  能：此函数用于解包： 1 从物理层缓冲区读出数据                           */ 
/*                         2 分析结构错误，并进行响应                         */
/*                         3 把消息层所需要的内容放入与消息层的缓冲区中。     */
/*备  注：                                                                    */
/******************************************************************************/ 
bool Sms_Unpacking(void)  
{
	int i; 
	UINT8  length=0,verify=0;      
    UINT8  temp[ArrayLength];	 
	//memset(temp, 0, ArrayLength);

    // 1 从缓冲区读出数据 
    for(i=0; i<ArrayLength; i++) 
	{      
        temp[i]= L3Buffer[i];	             
	}
    //memset(L3Buffer, 0, ArrayLength);  // 清除缓冲区
     
     //   2  分析结构错误
   length=(temp[1]-0x01);   // 消息包长度   
   for(i=0; i<=length+3; i++) // 计算校验位值
	  {
		verify+=temp[i];
	  }
   
   
   if((temp[0]==0x84 )/*&& (temp[2]==0x01 ) */&& (verify == 0x00)){
      
	   for(i=0; i<length; i++)
	     L3Buffer[i]=temp[i+3];  // 3 把消息层所需内容放入缓冲区         
	  
	   g_pL12MsgBuffer[0] = 0x00; // 数据包正确 'DTMF d1' 
	   g_pL12MsgBuffer[1] = 0x01;
	   SendMessageToSMSPhyLayer(Order_FSK_ACK_Chain_VZ, 0, 0); 
	  
   }else {

      g_pL12MsgBuffer[0] = 0x00; // 数据包错误 'DTMF d0'
      g_pL12MsgBuffer[1] = 0x0a;
      SendMessageToSMSPhyLayer(Order_FSK_ACK_Chain_VZ, 0, 0);     
	  
	  return false;
   }


   return true;

}
///////////////////////////////////////////////////
///定时器过程
void TimerOver( unsigned long lParam)
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	SendMessageToSMSPhyLayer(Order_TimeOver_Chain_VZ, 0, 0); // 1 通知消息层：时间超时
	SendMessageToSMSMsgLayer(Msg_TimeOver_Chain_Msg, 0, 0);  // 2 通知消息层：时间超时
}

void SYJTimerOver( unsigned long lParam)
{
	
	SYSTEM_MESSAGE message;
	message.handle = (UINT32)m_pShowMsg;
	message.message = WM_PAINT_SECOND;
	message.wparam = 0;
    message.lparam = 0;
    
	PostMessageToQueue ( &message );

	
	if ( m_pShowMsg->GetSecond ( ) == 0 ) ////定时器到时
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
//////////////消息映射
BEGIN_MESSAGE_MAP(CSMSL2Wnd, CFrameWnd) 
    // 1 来自消息层的消息
	ON_MESSAGE(NULL, WM_SMS_ML2_MESSAGE , OnUpload)              // 消息层--〉链路层：收FSK包。仅用于测试 
    ON_MESSAGE(NULL, Order_DTMF_Data_Msg_Chain, OnDTMF_Data)     // 消息层--〉链路层：上传DTMF包
    ON_MESSAGE(NULL, Order_FSK_Data_Msg_Chain, OnFSK_Data)       // 消息层--〉链路层：上传FSK包
    ON_MESSAGE(NULL, Order_Smsdown_Msg_Chain, OnSmsdown_Msg)     // 消息层--〉链路层：对下载FSK包的响应
	ON_MESSAGE(NULL, Order_Connect_Server_Msg, OnConnect_Server_Chain) // 消息层--〉链路层：请求与服务器连接
    ON_MESSAGE(NULL, Order_Disconnect_Server_Msg, OnDisconnect_Server_Chain) // 消息层--〉链路层：请求与服务器连接
    // 2 来自物理层的消息
	ON_MESSAGE(NULL, Msg_CID_Coming_Phone_VZ_Chain, OnSMS_Come)  // 物理层--〉链路层：
	ON_MESSAGE(NULL, Msg_EM_VZ_Chain, OnDeal_L1Msg)              // 物理层--〉链路层：
	ON_MESSAGE(NULL, Msg_Receive_FSK_VZ_Chain, OnReceive_FSK)    // 物理层--〉链路层：
	ON_MESSAGE(NULL, Msg_Hook_Off_VZ_Chain, OnHook_Off)          // 物理层--〉链路层：
	ON_MESSAGE(NULL, Msg_Key_EM_VZ, OnKey_EM)                    // 物理层--〉链路层：
    ON_MESSAGE(NULL, Msg_Unsuccess, OnUnsuccess)                    // 物理层--〉链路层：
	
	// 3 
	
END_MESSAGE_MAP

CSMSL2Wnd::CSMSL2Wnd()
{
  
}

CSMSL2Wnd::~CSMSL2Wnd()
{

}

//////////////////   来自于消息层的消息  start  wy  ///////////////////////////////
// 1  请求连接平台消息
void CSMSL2Wnd::OnConnect_Server_Chain( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
  	SendMessageToSMSPhyLayer(Order_Connect_Server_Chain, 0, 0);// 通知物理层请求连接平台
}

// 2 请求断开连接
void CSMSL2Wnd::OnDisconnect_Server_Chain( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
    SendMessageToSMSPhyLayer(Order_Disconnect_Server_Chain,0,0);
}

// 3  请求上传DTMF包消息
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
 
	 //memset(L3Buffer, 0, ArrayLength);  // 清除缓冲区
	 for(i=0; i<ArrayLength; i++){
		L3Buffer[i]=temp[i];	 
	 }

#endif
	 
	 SendMessageToSMSPhyLayer(Order_Send_DTMF_Chain_VZ,0,0); // 1 上传的DTMF包以放在缓冲区；
}

// 4 请求上传FSK包消息(暂时不考虑实现)
void CSMSL2Wnd::OnFSK_Data ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{	 
	//::MessageBox ( "上传FSK包","Msg",MB_OK );
}

// 5  对下载FSK包的响应
void CSMSL2Wnd::OnSmsdown_Msg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{	 
	UINT8  temp[2];
	temp[0]=g_pL23MsgBuffer[0];
	temp[1]=g_pL23MsgBuffer[1];
	OrderSendDTMFData(temp,2,-1);
	//SendMessageToSMSPhyLayer(Order_Smsdown_Chain_VZ, wParam, 0);//通知物理层AA,A0,ect;
}

//  6 此消息仅用来测试，虚拟地接收FSK包 
void CSMSL2Wnd::OnUpload ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
		
	SendMessageToSMSMsgLayer (Msg_Data_Chain_Msg,0,0 ); 
     
}
///////////////////   来自于消息层的消息    end    /////////////////////////////////

 
//////////////////    来自于物理层的消息    start wy  /////////////////////////////////
// 1 短消息中心来电
void CSMSL2Wnd::OnSMS_Come ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{	
	SmsPickUpPhone();
	
	g_iServerType = 1;
	g_pL23MsgBuffer[0] = SMS_CENTER_CALL;
	g_pL23MsgBuffer[1] = 0;
    SendMessageToSMSMsgLayer(Msg_Chain_Msg,0,0); // 通知消息层
				// 设置定时器：启动 TS1 ，准备接受CAS
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	NU_Reset_Timer(&TS, TimerOver, TS1*250, TS1*250, NU_ENABLE_TIMER);
}

// 2 
void CSMSL2Wnd::OnDeal_L1Msg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{ 
     switch( g_pL12MsgBuffer[0] )
	 {
	 case Msg_CAS_EM: // 收到CAS信号
		        NU_Control_Timer(&TS, NU_DISABLE_TIMER); // 接收正常，停止TS1        
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
				g_pShowMsg->SetMessage ( "连接成功!" );
				g_pL12MsgBuffer[0] = DTMF_A;
				g_pL12MsgBuffer[1] = 0;
   				SendMessageToSMSPhyLayer(Order_CAS_ACK_Chain_VZ,0 , 0);				
				g_pL23MsgBuffer[0] = SMS_CAS;
				g_pL23MsgBuffer[1] = 0;
				SendMessageToSMSMsgLayer(Msg_Chain_Msg, 0, 0);    
					  // 重新设置定时器：启动 TS5 ，准备接受FSK包            
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				NU_Reset_Timer(&TS, TimerOver, TS5*300, TS5*300, NU_ENABLE_TIMER);
		  break;
	case Msg_TS6_EM:
	   				 // 重新设置定时器：启动 TS6 ，准备接受后续FSK包            
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				if( g_iServerType == 0)
					NU_Reset_Timer(&TS, TimerOver, TS6*250*3, TS6*250*3, NU_ENABLE_TIMER);
				if(( g_iServerType == 1)||( g_iServerType == 2))
					NU_Reset_Timer(&TS, TimerOver, TS6*300, TS6*300, NU_ENABLE_TIMER);
		  break;
	case Msg_TS1_EM:
				     // 设置定时器：启动 TS1 ，准备接受CAS
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				NU_Reset_Timer(&TS, TimerOver, TS1*450, TS1*450, NU_ENABLE_TIMER);
		  break;
	default: 	      
		  break; 
	 }
}

//  3  接收、分析FSK数据包
void CSMSL2Wnd::OnReceive_FSK ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);  // FSK接收正常，停止TS5/6
	
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ();
	g_pShowMsg->SetMessage ("正在接收数据 请稍候..." );
	if ( Sms_Unpacking() )   // 链路层分析FSK包
		SendMessageToSMSMsgLayer(Msg_Data_Chain_Msg, 0, 0);

	g_bIsCanReset = FALSE;
}

//  4  中断短消息
void CSMSL2Wnd::OnHook_Off ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{ 
	g_pL23MsgBuffer[0] = SMS_USER_SHUT;
	g_pL23MsgBuffer[1] = 0;
	SendMessageToSMSMsgLayer(Msg_Chain_Msg,0 , 0);
}

  // 5 电话按键
void CSMSL2Wnd::OnKey_EM ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{ 
	g_pL23MsgBuffer[0] = SMS_PHONE_BUTTON;
	g_pL23MsgBuffer[1] = g_pL12MsgBuffer[0];
	SendMessageToSMSMsgLayer(Msg_Chain_Msg, 0, 0);
}
 // 6 EM-VZ之间数据传输错误   
void CSMSL2Wnd::OnUnsuccess( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER); 
    SendMessageToSMSMsgLayer(Msg_Unsuccess_Chain_Msg,0,0);
} 
//////////////////    来自于物理层的消息   end  /////////////////////////////////



BOOL CSMSL2Wnd::OnCreate ( )
{
	if ( CFrameWnd::OnCreate ( ) == FALSE )
		return FALSE;
	/////////////初始化链路层的一些事情

	return TRUE;
}

void RunSMSL2 ( )
{
	/////初始化数据
	if(!m_pSMSL2)
	{
		m_pSMSL2 = new CSMSL2Wnd;
		m_pSMSL2->Create("SMSL2", 
			WS_BORDER, 
			CRect(0 ,  0, 1, 1) , 0);
	}

	/////////////初始化群发
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
//////////////////////// 消息层
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

////翻译命令模块
int TranslateCommand ( UINT8 *buffer )
{

	UINT8 bCommand = buffer[0];//////得到命令码
	int iCommandCode;

	switch ( bCommand )
	{
	case 0xb0: //////////////////////业务查询
		iCommandCode = SMS_QUERY;
		break;
	case 0xb1: ////////////////////信息下载
		iCommandCode = SMS_DOWNLOAD;
		break;
	case 0xb2: /////////////////////信息上传
		iCommandCode = SMS_UPLOAD;
		g_icommuicationstate = TRUE;  
		break;
	case 0xb3: ///////////////////屏幕询问
		iCommandCode = SMS_INFORMATION;
		break;
	case 0xb4: //////////////////屏幕输出
		iCommandCode =SMS_SCREENOUT;
		break;
	case 0xb5: //////////////////安全管理
		break;
	case 0xb6: /////////////////图形制作管理
		iCommandCode = SMS_GRAPHICS;
		break;
	case 0xb7: ////////////////改变CPE的接收状态
		iCommandCode = SMS_CHANGE;
		break;
	case 0xb8: ////////////////CPE配置参数询问
		if ( buffer[2] == 0x01 )
			iCommandCode = SMS_IDSETTING;
		else if ( buffer[2] == 0x02 )
			iCommandCode = SMS_CPESETTING;
		break;
	}


	return iCommandCode;
}
//////////////////////////////////////////////////////////
///短消息上传模块 ,数据打包
int SMSInformationUpload ( LPSENDSMS pst )
{
	
	
	/////////电话号码
	char szPhoto[81];
	////////////////电话1
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
	
	
	 //////去掉大于4的信向设置
	 iLen = strlen ( pst->szPhone );
	 for ( int i = 0;i < iLen;i ++ )
	 {
		if ( pst->szPhone[i] == '*' )
			if ( pst->szPhone[i+1] > '3' ||
				 pst->szPhone[i+1] < '0' )
					pst->szPhone[i+1] = '0';
	 }

	 strcpy ( szPhoto,pst->szPhone );
		
	//////////////短消息
	UINT8 bLen = strlen ( szPhoto ) + strlen ( pst->szMsg );
	UINT8 *buffer = new UINT8[bLen+3]; ////DTMF包

	UINT8 bBufferLen = 0;
	buffer[bBufferLen] = bLen + 2;
	bBufferLen ++;//////后续字节长度

	if ( g_SystemOption.bIsNeedReturn )
		buffer[bBufferLen] = 0x01; /////通过APP设置,bit0
	else buffer[bBufferLen] = 0x00;
	
	bBufferLen ++;/////////回执标志,0x00-No;0x01-Yes

	UINT8 bPos = 0;
	while ( szPhoto[bPos] != '\0' ) //////电话号码
	{
		buffer[bBufferLen] = szPhoto[bPos];
		bPos ++;
		bBufferLen ++;
	}
	
	buffer[bBufferLen] = 0xff;
	bBufferLen ++;//////分隔符 0xff

	bPos = 0;
	while ( pst->szMsg[bPos] != '\0' )////短消息内容
	{
		buffer[bBufferLen] = pst->szMsg[bPos];
		bPos ++;
		bBufferLen ++;
	}

	
	////////////////将UINT8缓冲区拆成单字节
	UINT8 *buf = new UINT8[2*(bLen+3)];
	for ( iLen = 0;iLen < bLen + 3;iLen ++ )
	{
		UINT8 uc = buffer[iLen];
		UINT8 luc = uc&0x0f;
		UINT8 huc = (uc>>4)&0x0f;
		buf[2*iLen] = huc;
		buf[2*iLen+1] = luc;
	}

	////将buffer拷贝到指定缓冲区
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
	/////////电话号码
	char szPhoto[81];
	////////////////电话1
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
	
	
	 //////去掉大于4的信向设置
	 iLen = strlen ( pst->szPhone );
	 for ( int i = 0;i < iLen;i ++ )
	 {
		if ( pst->szPhone[i] == '*' )
			if ( pst->szPhone[i+1] > '3' ||
				 pst->szPhone[i+1] < '0' )
					pst->szPhone[i+1] = '0';
	 }

	 strcpy ( szPhoto,pst->szPhone );
		
	//////////////短消息
	UINT8 bLen = strlen ( szPhoto ) + strlen ( pst->szMsg );
	UINT8 *buffer = new UINT8[bLen+3]; ////DTMF包

	UINT8 bBufferLen = 0;
	buffer[bBufferLen] = bLen + 2;
	bBufferLen ++;//////后续字节长度

	if ( g_SystemOption.bIsNeedReturn )
		buffer[bBufferLen] = 0x01; /////通过APP设置,bit0
	else buffer[bBufferLen] = 0x00;
	
	bBufferLen ++;/////////回执标志,0x00-No;0x01-Yes

	UINT8 bPos = 0;
	while ( szPhoto[bPos] != '\0' ) //////电话号码
	{
		buffer[bBufferLen] = szPhoto[bPos];
		bPos ++;
		bBufferLen ++;
	}
	
	buffer[bBufferLen] = 0xff;
	bBufferLen ++;//////分隔符 0xff

	bPos = 0;
	while ( pst->szMsg[bPos] != '\0' )////短消息内容
	{
		buffer[bBufferLen] = pst->szMsg[bPos];
		bPos ++;
		bBufferLen ++;
	}

	
	////////////////将UINT8缓冲区拆成单字节
	UINT8 *buf = new UINT8[2*(bLen+3)];
	for ( iLen = 0;iLen < bLen + 3;iLen ++ )
	{
		UINT8 uc = buffer[iLen];
		UINT8 luc = uc&0x0f;
		UINT8 huc = (uc>>4)&0x0f;
		buf[2*iLen] = huc;
		buf[2*iLen+1] = luc;
	}

	////将buffer拷贝到指定缓冲区
	for ( iLen = 0;iLen < 2*(bLen+3);iLen ++ )
		L3Buffer[iLen] = buf[iLen];
	L3Buffer[iLen] = 0x7F;

	delete []buffer;
	delete []buf;
		
	return SMS_OK;
}

///////////////////////////////////////////////////////
////提取短消息
SMS GetSMSIntoBuffer ( UINT8 *buffer,int &iFlag )
{
	SMS	ss;
	memset ( &ss,0,sizeof ( ss ) );

	UINT8 bLen = buffer[1]; ///////得到长度
	
	UINT8 bFlag = buffer[2]; //////判断数据是否结束
	if ( bFlag & 0x40 )
		iFlag = SMS_INFORMATION_UEND;
	else iFlag = SMS_INFORMATION_END;

	UINT8 bBufferPos = 4;////////取得发送方号码
	UINT8 bTempPos = 0;
	while ( buffer[bBufferPos] != 0xff )
	{
		ss.szPhone[bTempPos] = buffer[bBufferPos];
		bTempPos ++;
		bBufferPos ++;
	}

	ss.szPhone [bTempPos] = '\0';

	bBufferPos ++; //////////得到接收方的子信箱
	ss.bMailBox = buffer[bBufferPos++] - 0x30; 
	if ( ss.bMailBox > 3 || ss.bMailBox < 0 ) 
		ss.bMailBox = 0;
		
	//////////////取得时间
	for ( bTempPos = 0;bTempPos < 14;bTempPos ++,bBufferPos ++ )
		ss.szTime[bTempPos] = buffer[bBufferPos];
	
	for ( int j = 15;j > 1;j -- )
		ss.szTime[j] = ss.szTime[j-2];
	//ss.szTime [14] = '\0';
	ss.szTime [0] = '2';
	ss.szTime [1] = '0';
	ss.szTime [16] = '\0';

	///////////////取得短消息内容
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
///// 业务询问处理模块
void SMSQueryServerID (  )
{
	////////发送消息给链路层，说明短消息类型
	//////将一个字节转换成二个字节
	int i;
	for ( i = 0;i < 7;i ++ )
		L3Buffer[i] = 0;
		
	if ( g_iServerType == 0 ){ ////在线信息；
	
		//// 4B 0x00000000
		L3Buffer[7] = 0x00;

	}else if ( g_iServerType == 1 ){///离线信息交互
	
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
//// 提取EMail
MAILSTRUCT GetMailIntoBuffer ( UINT8 *buffer )
{	
		
	MAILSTRUCT	ms; ////声明Mail的临时结构
	memset ( &ms,0,sizeof ( MAILSTRUCT ) );


	UINT8	bLen = buffer[1]; /////得到缓冲区的长度
	int iPos = 4; 
	while ( true )  //////EMail发送方地址
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
/////提取订阅的短消息
SUBSCIBESTRUCT GetSubscibeIntoBuffer ( UINT8 *buffer )
{

	SUBSCIBESTRUCT ss;
	memset ( &ss,0,sizeof ( ss ) ); 

	UINT8 bLen = buffer[1]; //// 得到参数的长度

	int i;
	UINT8 pos = 4;
	for ( UINT8 i = 0;i < 8;i ++,pos ++ ) //////ICP代码
		ss.ICPCode[i] = buffer[pos];

	for ( i = 0;i < 10;i ++,pos ++ ) /////业务代码
		ss.ICPOperationCode[i] = buffer[pos];

			
	for ( i = 0;buffer[pos] != 0xFF;i ++,pos++ )//////////信息的标题
		ss.szTitle[i] = buffer[pos];
	ss.szTitle[i] = '\0';

	pos ++;///////跳过分隔符
	for ( i = 0;pos < bLen;i ++,pos ++ )  //////订阅短信的内容
		ss.szContent[i] = buffer[pos];
	ss.szContent[i] = '\0';


	return ss;
}

//////////////////////////////////////////////////////////////////////////
/////// 取得话费账单
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
////// 提取常用语
MODELDOWNLOAD GetModel ( UINT8 *buffer )
{
	MODELDOWNLOAD model;
	/////fomat: 操作码2B|长度|总包数|序号|下载信息
	UINT8	iLen = buffer[4]; 

	if ( iLen > 40 ) iLen = 40;
	UINT8 i,pos = 0;
	for ( i = 5;i < iLen + 5  ;i ++,pos ++ )
		model.szContent[pos] = buffer[i];

	model.szContent[pos] = '\0';

	return model;

}
////////////////////////////////////////////////////////////////////////////
///// 信息下载模块:解释部件类型
int SMSInformationDownload ( UINT8 *buffer )
{

	int iCmd;	

	switch ( buffer[3] ) //////部件号
	{
	case EMM_Part: ///////////Email
		g_pL23MsgBuffer[0] = 0x0D;//DTMF_A0; //////不存在该部件
		g_pL23MsgBuffer[1] = 0x0A;
		SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		iCmd = SMS_TYPE_ERROR;
		break;
	case SMM_Part: /////////普通短消息
		iCmd = SMS_TYPE_NORMAL;
		break;
	case SSM_Part: /////////订阅短消息
		iCmd = SMS_TYPE_USERMSG;
		break;
	case 0x04: /////////Memory卡写入区
		
		g_pL23MsgBuffer[0] = 0x0D;//DTMF_A0; //////不存在该部件
		g_pL23MsgBuffer[1] = 0x0A;
		SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		iCmd = SMS_TYPE_ERROR;

		break;
	case 0x05://///////IC卡写入区
		g_pL23MsgBuffer[0] = 0x0D;//DTMF_A0; //////不存在该部件
		g_pL23MsgBuffer[1] = 0x0A;
		SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		iCmd = SMS_TYPE_ERROR;
		
		break;
	case CSM_Part://///////常用语存储区
		iCmd = SMS_MODEL;
		break;
	case SRM_Part://///////特色铃声存储区
	     iCmd = SMS_RING;	
		break;
	case FBM_Part://///////话费账单存储区
		iCmd = SMS_MONEY;
		break;
	case FDBM_Part://///////话费祥单存储区
		iCmd = SMS_MONEYEX;
		break;
	}

	return iCmd;
}

//////////////////////////////////////////////////////////////////
/////改变CPE的接收状态模块
void SMSChangeReceiveStatus ( UINT8 *buffer )
{
	
	if ( buffer[2] == 0x00 ) { ////////////////挂机的准备工作
	
		
		g_pL23MsgBuffer[2] = 0x00;
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );///通知链路层挂机
			// g_iServerType = -1 表示无效;///服务类型 0-在线信息交互；1-离线信息交互；2-Email信息交互
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
			
			
			g_pShowMsg->SetMessage ("挂机!");
			NU_Sleep ( 400 );  // waiting 400*5 =2000ms
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
		}
	
	}else if ( buffer[2] == 0xFF ) { //////脱离数据通信，进入话音方式
	
		g_pL23MsgBuffer[2] = 0xFF;
		
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );
		
	
	}else {
	
		g_pL23MsgBuffer[2] = buffer[2];
		
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );
	}


	///////////如果家家e窗口存在将他销毁
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
////系统状态报告模块
void SMSQueryCPEConfig ( UINT8 statuscode )  /////DTMF
{
	if ( statuscode == 0x01 ) ///////终端ID报告
	{
		UINT8	bufferID[8]; /////终端ID报告数据包9各字节
		int i;

		bufferID[0] = 0x18; ///////厂家代码
		bufferID[1] = 0x02;////////终端类型
						  ////////0x01 - 标准型
		                  ////////0x02 - 豪华型
		bufferID[2] = 0x01;////////年：用BDC编码方式 （1999年）
		bufferID[3] = 0x01;////////月: .............  (12月)
		bufferID[4] = 0x01; ///////版本号，厂家自定
		
		bufferID[5] = 0x01; ///////序列号，厂家自定。3个字节
		bufferID[6] = 0x02;
		bufferID[7] = 0x03;
		
		//////////////将一个字节转化成二个字节
		UINT8    tempBuffer[17];
		for ( i = 0;i < 8;i ++ )
		{
		
			UINT8 h = (bufferID[i]>>4)&0x0f;
			UINT8 l = bufferID[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[16] = 0x7F;
		/////////拷贝到指定缓冲区
		for ( i = 0;i < 17;i ++ )
			L3Buffer[i] = tempBuffer[i];
		
	}else if ( statuscode == 0x02 ){ /////////////终端配置信息报告
	
		UINT8	bufferCPE[3];//////终端配置信息报告
		int i;
		
		bufferCPE[0] = 0x0A;//////终端屏列数
		bufferCPE[1] = 0x09;//////终端屏行数
		bufferCPE[2] = 0x0B;//////能力标志
		                   //////bit0:普通终端
		                   //////bit1:图形能力
						   //////bit2:安全能力
						   //////bit3:Email能力
						   //////bit4:FSK上传能力
						   //////bit5:保留
						   //////bit6:保留
						   //////bit7:保留

		//////////////将一个字节转化成二个字节
		UINT8    tempBuffer[7];
		for ( i = 0;i < 3;i ++ )
		{
		
			UINT8 h = (bufferCPE[i]>>4)&0x0f;
			UINT8 l = bufferCPE[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[6] = 0x7F;
		/////////拷贝到指定缓冲区
		for ( i = 0;i < 7;i ++ )
			L3Buffer[i] = tempBuffer[i];

	}

}

//////////////// FSK
void SMSQueryCPEConfigEx ( UINT8 statuscode )  
{
	if ( statuscode == 0x01 ) ///////终端ID报告
	{
		UINT8	bufferID[8]; /////终端ID报告数据包9各字节
		int i;

		bufferID[0] = 0x18; ///////厂家代码
		bufferID[1] = 0x02;////////终端类型
						  ////////0x01 - 标准型
		                  ////////0x02 - 豪华型
		bufferID[2] = 0x01;////////年：用BDC编码方式 （1999年）
		bufferID[3] = 0x01;////////月: .............  (12月)
		bufferID[4] = 0x01; ///////版本号，厂家自定
		
		bufferID[5] = 0x01; ///////序列号，厂家自定。3个字节
		bufferID[6] = 0x02;
		bufferID[7] = 0x03;
		
		//////////////将一个字节转化成二个字节
		UINT8    tempBuffer[17];
		for ( i = 0;i < 8;i ++ )
		{
		
			UINT8 h = (bufferID[i]>>4)&0x0f;
			UINT8 l = bufferID[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[16] = 0x7F;
		/////////拷贝到指定缓冲区
		for ( i = 0;i < 17;i ++ )
			L3Buffer[i] = tempBuffer[i];
		
	}else if ( statuscode == 0x02 ){ /////////////终端配置信息报告
	
		UINT8	bufferCPE[3];//////终端配置信息报告
		int i;
		
		bufferCPE[0] = 0x0A;//////终端屏列数
		bufferCPE[1] = 0x09;//////终端屏行数
		bufferCPE[2] = 0x0B;//////能力标志
		                   //////bit0:普通终端
		                   //////bit1:图形能力
						   //////bit2:安全能力
						   //////bit3:Email能力
						   //////bit4:FSK上传能力
						   //////bit5:保留
						   //////bit6:保留
						   //////bit7:保留

		//////////////将一个字节转化成二个字节
		UINT8    tempBuffer[7];
		for ( i = 0;i < 3;i ++ )
		{
		
			UINT8 h = (bufferCPE[i]>>4)&0x0f;
			UINT8 l = bufferCPE[i]&0x0f;
			tempBuffer[2*i] = h;
			tempBuffer[2*i+1] = l;
		}

		tempBuffer[6] = 0x7F;
		/////////拷贝到指定缓冲区
		for ( i = 0;i < 7;i ++ )
			L3Buffer[i] = tempBuffer[i];

	}

}


///////////////////////////屏幕询问模块
BOOL ScreenEnquire ( UINT8 *buffer )
{
	
	int i;
	for ( i = 0;i < 256;i ++ )
		g_pMenuBuffer[i] = buffer[i];
	
	m_pService->SendMessage ( WM_UPDATE_MENU,0,0 );///通知更新客服中心菜单

	return TRUE;
}
//////////////////////////屏幕输出模块
BOOL ScreenPutOut ( UINT8 *buffer )
{
	extern void SMSOpenFSK ( );
	SMSOpenFSK ( );

	int i;
	for ( i = 0;i < 256;i ++ )
		g_pMenuBuffer[i] = buffer[i];
	
	if ( m_pService )
		m_pService->SendMessage ( WM_DISPLAY_MSG,0,0 );///通知更新客服中心菜单
	else {
	
		UINT8	bLen = g_pMenuBuffer[1],////长度
				bChildCmd = g_pMenuBuffer[2];//子命令码

		char    szTitle[40];
		char    szTemp[256];
		int i;


		for ( i = 4;i < bLen + 2;i ++ )
			szTemp[i-4] = g_pMenuBuffer[i];		

		szTemp[i-4] = '\0';
		/////显示e消息
		if ( !m_pShowMsg )
			m_pShowMsg = new CShowMessageEx;
		
		/////是否启动定时器
		if ( g_pMenuBuffer[3] == 0x01 )
			NU_Reset_Timer(&SYJTS, SYJTimerOver, 1000, 200, NU_ENABLE_TIMER);				
		
		m_pShowMsg->SetTimerFlag ( g_pMenuBuffer[3] );
		m_pShowMsg->SetString ( szTemp ); 
		m_pShowMsg->SetDestroyFlag ( 1 );
		m_pShowMsg->Create ( "提示",
			WS_VISIBLE | WS_CAPTION | WS_CHILD,
			CRect ( 0,0,159,159 ) ,0 );

		
		m_pShowMsg->ShowWindow ( );
	
	}

	return TRUE;
	
}

////////////////////////////////// class CSMSMessage
/////////删除Memory中的多余纪录
BOOL DeleteRecordInMemory ( int iMailBox )
{
	
	int i;
	for ( i = 0;i < SMS_MAX_SAVE_NUMBER;i ++ )
	{
		if ( !g_SendedSMS[iMailBox][i].iLockStatus  )  ////没有加锁，可以删除
		{
			
			if ( g_SendedSMS[iMailBox][i].bIsFlash )
			{
				//////将最小的ID对应的纪录删掉
				DeleteSMS ( g_SendedSMS[iMailBox][i].dataID );	
				g_SystemOption.SMSStatus[iMailBox].bHavePasswordItem --;
			}

			////移动Memory中的数据
			memmove ( &g_SendedSMS[iMailBox][i],
				&g_SendedSMS[iMailBox][i+1],
				(SMS_MAX_SAVE_NUMBER-i-1)*sizeof ( SMS ) );
			
			g_SystemOption.SMSStatus[iMailBox].bSendedNumber --;

			return TRUE;
		}
	}
	//////没有符合条件的纪录
	return FALSE;
}


/////////删除Flash中的多余纪录
BOOL DeleteRecord ( int iMailBox,int iChildBox )
{
		/////////发信箱的方式是先进先出
		//////将最先进入的纪录删除
		int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////查找纪录			
		int pos = 0,i,j;
		
		for ( i = 0;i < r;i ++ ) ///遍历
		{
		
			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///得到纪录指针
			if ( lpST->bMailBox == iMailBox && /////判断数据是否属于当前选中信箱
				lpST->iChildMailBox == iChildBox ) {			////中的发信箱
			
				if ( !lpST->iLockStatus &&
					 lpST->iLetterStatus == ICON_LETTERREADED )  ////没有加锁，可以删除
				{
					DeleteSMS ( g_pSMSDataTempIDBuf[i] );
					pos ++;
					break;

				}
			}
		
		}

		if ( !pos ) return FALSE;////找不到符合条件的纪录

		if ( iChildBox == 0 )
			g_SystemOption.SMSStatus[iMailBox].bRevNumber --;
		else if ( iChildBox == 1 )
			g_SystemOption.SMSStatus[iMailBox].bSendNumber --;
		//else if ( iChildBox == 2 )
		//	g_SystemOption.SMSStatus[iMailBox].bHavePasswordItem --;

		return TRUE;

}
///////添加话费纪录
int AddMoneyRecord ( MONEY money ,BOOL bStyle )
{

	int i;
	if ( bStyle ){  /////普通话费
	
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
	
	}else { //////详细话费
	
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
///////添加订阅的短消息
int AddSubscibeRecord ( SUBSCIBESTRUCT st )
{
	/////////订阅短消息数量达到最大
	if ( g_iSubscribePos > SMS_SUBSCIBE_MAX_NUMBER - 1 )
	{
		for ( int i = 0;i < SMS_SUBSCIBE_MAX_NUMBER - 2;i ++ )
			g_SubscibeSMS[i] = g_SubscibeSMS[i+1];
		
		g_SubscibeSMS[SMS_SUBSCIBE_MAX_NUMBER-1] = st;
	
	}else { ////未达到最大值
	
		g_SubscibeSMS[g_iSubscribePos] = st;
		g_iSubscribePos ++;	
	
	}


	return 1;
}

///////添加已发信箱纪录
BOOL AddSendedRecord ( SMS ss )
{

	if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber>SMS_MAX_SAVE_NUMBER-1 )
		if ( DeleteRecordInMemory  ( iCurrentLetterBox ) == FALSE ) /////删除没用的纪录
			return FALSE;

	
	g_SendedSMS[ss.bMailBox][g_SystemOption.SMSStatus[ss.bMailBox].bSendedNumber] = ss;
	g_SystemOption.SMSStatus[ss.bMailBox].bSendedNumber ++;

	return TRUE;
}
/////////添加发送失败的纪录
BOOL AddSendFailedRecord ( )
{
	if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber>SMS_MAX_SAVE_NUMBER-1 )
		if ( DeleteRecord ( iCurrentLetterBox, 1 ) == FALSE ) /////删除没用的纪录
			return FALSE;


	///////// 向发件箱增加一条发送失败的纪录
	if ( strlen ( g_SendST.szPhone ) <= 0 )
		return FALSE;

	SMS ss;
	memset ( &ss,0,sizeof ( ss ) );///// clear 0
	strcpy ( ss.szPhone,g_SendST.szPhone ); ////Phone
	strcpy ( ss.szMsg,g_SendST.szMsg );  /////Msg

	
	////// 填充时间
	FillTime ( ss.szTime );

	
	ss.Flag = 0;///只读
	ss.bMailBox = iCurrentLetterBox;///当前选中的信箱
	ss.iChildMailBox = 1;/////保存到发信箱
	ss.iLetterStatus = ICON_SENDFAILED;////失败

	int r = AppendSMS  ( &ss );
	if ( r == -1 ) 
		::MessageBox ( "存储部件已满！","错误",MB_OK|MB_ICONSTOP );

	g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber ++;
	
	return TRUE;
}

/////////向发送箱添加纪录
BOOL AddSendRecord ( SMS ss ,BOOL bIsChange )
{
	ss.bIsFlash = 0;
	if ( strlen ( ss.szPhone ) <= 0 ) return FALSE;

	if ( bIsChange ) /////更改模式
	{
	
		ModifySMS ( g_CurrentID,&ss ); 
		
		return TRUE;
	}

	/////////// 提示用户将要达到存储上限
	if ( g_SystemOption.SMSStatus[ss.bMailBox].bSendNumber>SMS_MAX_SAVE_NUMBER-1 )
	{
		if ( MessageBox ( "信箱已满，继续将覆盖原有一条数据，确认吗?",
		             "警告",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			if ( DeleteRecord ( ss.bMailBox, ss.iChildMailBox ) == FALSE ) /////删除没用的纪录
			{
				MessageBox ( "存储失败！","错误",MB_OK|MB_ICONSTOP ); 
				return FALSE;
			}
		
		}else {
		
			return FALSE;
		}

	}


	static short iCmpID = -2;

	if ( g_CurrentID != iCmpID ) {////新增纪录
	
		int r = AppendSMS  ( &ss );
		if ( r == -1 ) 
		{
			MessageBox ( "存储部件已满！","错误",MB_OK|MB_ICONSTOP );
			return FALSE;
		}

		g_CurrentID = r;
		iCmpID = r;

	}else {//////////修改纪录
	
		ModifySMS ( g_CurrentID,&ss ); 
	}
	

	 
	g_SystemOption.SMSStatus[ss.bMailBox].bSendNumber ++;
	
	return TRUE;
}
////////向收件箱添加纪录
short AddRevRecord ( SMS ss ,BOOL bIsDownload )
{
	int r;
	char szName[81];
	if ( g_SystemOption.SMSStatus[ss.bMailBox].bRevNumber>SMS_MAX_SAVE_NUMBER-1 ) ////信箱已满
	{
	
		if ( !g_SystemOption.SMSStatus[g_ST.bMailBox].bIsCanDelete ) ///拒接模式
		{
			if ( bIsDownload ) ///是否下载
			{
				///////通知链路层挂机
				NU_Control_Timer(&TS, NU_DISABLE_TIMER);

				UINT8  temp[2];


				g_pL23MsgBuffer[0] = 0x0D;//D1 /////存储部件满
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
			strcat ( szName,"存储空间已满" );
			g_pShowMsg->SetMessage ( szName );
			NU_Sleep ( 200 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			return -1;
			
		}else { ///先进先出模式
		
			if ( DeleteRecord ( ss.bMailBox, ss.iChildMailBox ) == FALSE ) /////删除没用的纪录
			{
				if ( bIsDownload ) ///是否下载
				{
					///////通知链路层挂机
					NU_Control_Timer(&TS, NU_DISABLE_TIMER);

					g_pL23MsgBuffer[0] = 0x0D;//D1 /////存储部件满
					g_pL23MsgBuffer[1] = 0x01;
					SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

					g_pL23MsgBuffer[2] = 0x00;
					SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

				}

				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
				
				strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
				strcat ( szName,"存储空间已满" );
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
			
				if ( bIsDownload ) ///是否下载
				{
					///////通知链路层挂机
					NU_Control_Timer(&TS, NU_DISABLE_TIMER);

					g_pL23MsgBuffer[0] = 0x0D;//D1 /////存储部件满
					g_pL23MsgBuffer[1] = 0x01;
					SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

					g_pL23MsgBuffer[2] = 0x00;
					SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

				}
				
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();

				strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
				strcat ( szName,"存储空间已满" );
				g_pShowMsg->SetMessage ( szName );

				NU_Sleep ( 200 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );
				

			}else { /////增加纪录成功

				if ( bIsDownload ) ///是否下载
				{
					g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
					g_pL23MsgBuffer[1] = 0x0D;

					UINT8 temp[2];
					temp[0]=g_pL23MsgBuffer[0];
					temp[1]=g_pL23MsgBuffer[1];
					OrderSendDTMFData(temp,2,-1);

					//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
				}
			}
		
		}
	
	}else { //////信箱未满，直接存储
	
			ss.bIsFlash = 0;
			r = AppendSMS  ( &ss );//// Add a record
			if ( r == -1 ) 
			{
			
				if ( bIsDownload ) ///是否下载
				{
					///////通知链路层挂机
					NU_Control_Timer(&TS, NU_DISABLE_TIMER);

					g_pL23MsgBuffer[0] = 0x0D;//D1 /////存储部件满
					g_pL23MsgBuffer[1] = 0x01;
					SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

					g_pL23MsgBuffer[2] = 0x00;
					SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

				}
				
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
			
				strcpy ( szName,g_SystemOption.SMSStatus[ss.bMailBox].szName );
				strcat ( szName,"存储空间已满" );
				g_pShowMsg->SetMessage ( szName );

				NU_Sleep ( 200 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );
				

			}else { /////增加纪录成功

				if ( bIsDownload ) ///是否下载
				{
					g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
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
	SetLED ( LED_SMS ); ////当有段信时，将LED电亮

	return r;
	
}

//////////////消息映射
BEGIN_MESSAGE_MAP(CSMSMessage, CFrameWnd) 
	ON_MESSAGE( NULL, Msg_Data_Chain_Msg , OnDataChainMsg) ///下传的FSK消息域包
	ON_MESSAGE( NULL,Msg_Chain_Msg,OnChainMsg)////////////////其他消息
	ON_MESSAGE ( NULL,Msg_TimeOver_Chain_Msg,OnTimerOver )////定时益处
	ON_MESSAGE ( NULL,Msg_Unsuccess_Chain_Msg,OnUnsuccessChainMsg ) ////发送失败
END_MESSAGE_MAP

CSMSMessage::CSMSMessage()
{

}

CSMSMessage::~CSMSMessage()
{

}

///////////////////////////////////////////////////////////////
/////消息模块
//////////////////超时消息
void CSMSMessage::OnTimerOver ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{		
	if ( g_pShowMsg->GetVisibleFlag ( ) )
		g_pShowMsg->ShowWindow ( SW_HIDE ); // ????
	if ( g_iServerType != 0 ) ////不是在线交互
	{
		AddSendFailedRecord ( ); ////向发信箱中添加一条失败的信件			
	}
}

/////// 发送失败
void CSMSMessage::OnUnsuccessChainMsg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	if ( !g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ();
	g_pShowMsg->SetMessage ( "数据传输错误!" );
	
	g_pL23MsgBuffer[2] = 0x00;
	SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );///挂机
	if ( g_iServerType != 0 ) ////不是在线交互
	{
		AddSendFailedRecord ( ); ////向发信箱中添加一条失败的信件			
	}
	NU_Sleep ( 100 );
	if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );
}

void CSMSMessage::OnDataChainMsg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	///////////////////从链路层过来的消息
	///////////////////翻译消息
	int i;
	UINT8 buffer[ArrayLength];
	for ( i = 0;i < ArrayLength;i ++)
		buffer[i]  = L3Buffer[i];
	//memset ( L3Buffer,0,ArrayLength );

	int iFlag;
	int r = TranslateCommand ( buffer );

	if ( SMS_QUERY == r )				///////////业务查询
	{
		SMSQueryServerID ( );

	}else if ( SMS_DOWNLOAD == r ){		////////////信息下载

		
		r = SMSInformationDownload ( buffer );
		if ( r == SMS_TYPE_NORMAL ){
			
			/////提醒用户有新的短消息
			/////将短消息内容从缓冲区中提取
			g_ST = GetSMSIntoBuffer ( buffer,iFlag );

			///////////回馈给链路层消息
			if ( iFlag == SMS_INFORMATION_END ) /////下载结束
			{
				///// 显示短消息内容，并保存到收件箱
				/////保存信件
				//char str[80];
				if ( g_ST.bMailBox == 0 ) ////公共信箱
					g_ST.bMailBox = 3;
				else if ( g_ST.bMailBox == 1 ) ///私人1
					g_ST.bMailBox = 0;
				else if ( g_ST.bMailBox == 2 )
					g_ST.bMailBox = 1;
				else if ( g_ST.bMailBox == 3 ) 
					g_ST.bMailBox = 2;

				g_ST.bReadStatus = 0;
				g_ST.iChildMailBox = 0;
				g_ST.iLetterStatus = ICON_LETTERUNREAD;////未读;
				g_ST.Flag = 0;///只读
				


				//////增加一条记录
				if ( ( r = AddRevRecord ( g_ST ,TRUE ) ) != -1 )
					g_CurrentID = r;
				else return;
				
				

				///////通知链路层挂机
				//NU_Control_Timer(&TS, NU_DISABLE_TIMER);
				//g_pL23MsgBuffer[2] = 0x00;
				//SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );				
				///通知用户有新的信件
				strcpy ( g_szHaveNewMessage,"[" );
				strcat ( g_szHaveNewMessage,g_SystemOption.SMSStatus[g_ST.bMailBox].szName );
				strcat ( g_szHaveNewMessage,"]信箱有新短信!" );

				g_bIsRing = TRUE;

				}else if ( iFlag == SMS_INFORMATION_UEND ){////下载失败
				
				////////////具体问题再分析
				//////////显示给用户
				if ( !g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ();
				g_pShowMsg->SetMessage ( "下载失败." );
				NU_Sleep ( 400 );
				if ( g_pShowMsg->GetVisibleFlag ( ) )
					g_pShowMsg->ShowWindow ( SW_HIDE );
				
			}


		}else if ( r == SMS_TYPE_USERMSG ){ ////订阅的短消息
		
			
			
			SUBSCIBESTRUCT st = GetSubscibeIntoBuffer ( buffer );
			st.iconID = ICON_LETTERUNREAD;
			////// 填充时间
			FillTime ( st.szTime );

			AddSubscibeRecord ( st );
			
			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
			g_pL23MsgBuffer[1] = 0x0D;

			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);

			///// hide windows

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "有新订阅消息" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			///////通知链路层挂机
			

			//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

		}else if ( r == SMS_MONEY ) { /////话费账单下载
		
			MONEY money = GetMoney ( buffer );
			money.iconID = ICON_LETTERUNREAD;
			money.bIsData = 1;
			AddMoneyRecord ( money, TRUE );

			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
			g_pL23MsgBuffer[1] = 0x0D;

			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);
			

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "简单话费账单下载成功" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			

			//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );
		
		}else if ( r == SMS_MONEYEX ) { /////话费详单下载
		
			MONEY money = GetMoney ( buffer );
			money.bIsData = 1;
			money.iconID = ICON_LETTERUNREAD;
			AddMoneyRecord ( money, FALSE );

			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
			g_pL23MsgBuffer[1] = 0x0D;

			
			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);
		

			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "详细话费账单下载成功" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			

			//SendMessageToSMSL2 ( Order_Smsdown_Msg_Chain,0,0 );

		}else if ( r == SMS_RING ){		//////特色铃声
	

			extern int SMS_SAVE_RING(BYTE * );

			UINT8 tempBuffer[ArrayLength];
			for ( i = 4;i < ArrayLength;i ++ )
				tempBuffer[i-4] = buffer[i];
			SMS_SAVE_RING ( tempBuffer );

			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
			g_pL23MsgBuffer[1] = 0x0D;

			
			UINT8 temp[2];
			temp[0]=g_pL23MsgBuffer[0];
			temp[1]=g_pL23MsgBuffer[1];
			OrderSendDTMFData(temp,2,-1);
			
			/////////提示新的铃声到达
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ();
			g_pShowMsg->SetMessage ( "有新的特色铃声" );
			NU_Sleep ( 400 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
			
	
			


		}else if ( r == SMS_MODEL ){ /////常用语下载
		
			MODELDOWNLOAD model = GetModel ( buffer );
			SUBSCIBESTRUCT st;
			st.iconID = ICON_LETTERUNREAD;
			strcpy ( st.szTitle,"常用语" );
			strcpy ( st.szContent,model.szContent );
			////// 填充时间
			FillTime ( st.szTime );

			/*DateTime	date;
			GetSysTime ( &date );
			sprintf ( st.szTime,"%d:%d",
				  date.hour,
				  date.minute );*/

			AddSubscibeRecord ( st );
			
			g_pL23MsgBuffer[0] = 0x0D;//DTMF_AA; 下载成功
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
			g_pShowMsg->SetMessage ( "有新常用语" );
			NU_Sleep ( 500 );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

			
		
		}else if ( r == SMS_TYPE_ERROR ){ /////短消息错误
		
		
		}

		
	}else if ( SMS_UPLOAD ==  r ){				////////////信息上传


#ifdef SMS_DTMF
		if ( SMSInformationUpload ( &g_SendST ) == SMS_EMPTY_PHOTO ) /////空电话号码
		{
			::MessageBox ( "电话号码不能为空，请重新输入。","错误",MB_OK );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
			return ;
		}
#endif

#ifdef SMS_FSK
		if ( SMSInformationUploadEx ( &g_SendST ) == SMS_EMPTY_PHOTO ) /////空电话号码
		{
			::MessageBox ( "电话号码不能为空，请重新输入。","错误",MB_OK );
			if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );
			return ;
		}
#endif

		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );///通知链路层去读缓冲区
		bIsSuccessSend = TRUE; ////标志置真

		//////认为发送成功，将本条短消息存储到已发信箱
		SMS ss;
		memset ( &ss,0,sizeof ( SMS ) );///// clear 0
		strcpy ( ss.szPhone,g_SendST.szPhone ); ////Phone
		strcpy ( ss.szMsg,g_SendST.szMsg );  /////Msg

		
		////// 填充时间
		FillTime ( ss.szTime );
		
		ss.Flag = 0;///只读
		ss.bMailBox = iCurrentLetterBox;///当前选中的信箱
		ss.iChildMailBox = 2;/////保存到发信箱
		ss.iLetterStatus = ICON_SENDOK;////成功
		ss.dataID = -1;		
			
		///向已发信箱中增加一条记录
		AddSendedRecord ( ss );				

	}else if ( SMS_IDSETTING == r ){		////询问ID设置

#ifdef SMS_DTMF	
		SMSQueryCPEConfig ( 0x01 );
#endif

#ifdef SMS_FSK
		SMSQueryCPEConfigEx ( 0x01 );
#endif
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
	
	}else if ( SMS_CPESETTING == r ) {		/////询问CPE设置

#ifdef SMS_DTMF
		SMSQueryCPEConfig ( 0x02 );
#endif

#ifdef SMS_FSK
		SMSQueryCPEConfigEx ( 0x02 );
#endif
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );
	
	}else if ( SMS_CHANGE == r ){ ///////改变话机状态
	
		SMSChangeReceiveStatus ( buffer );//////挂机准备
		///接到挂机命令，关闭消息框
		
	}else if ( SMS_INFORMATION == r ){////////////屏幕询问，用户可以输入
	
		////////暂时未定
		ScreenEnquire ( buffer );

	}else if ( r == SMS_SCREENOUT ){ /////屏幕输出,0xB4
	
		ScreenPutOut ( buffer );		
	
	}else if ( r == SMS_GRAPHICS ) {  /////图形管理模块
	
		for ( i = 0;i < 256;i ++ )
			g_pMenuBuffer[i] = buffer[i];
		
		/////创建图形窗口
		if ( !g_pGraphicsWnd )
			g_pGraphicsWnd = new CDrawPicWnd;

		g_pGraphicsWnd->Create ( "视图",
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD,
			CRect ( 0,0,159,159 ) );

		g_pGraphicsWnd->ShowWindow ( );

	
	} 


}

///////其它消息命令
void CSMSMessage::OnChainMsg ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	switch ( g_pL23MsgBuffer[0] )
	{
	case SMS_CENTER_CALL: /////短消息中心来电
		break;
	case SMS_CAS: ///////成功收到CAS信号
		//////////通知用户连接成功
		break;
	case SMS_PHONE_STATUS:////电话状态,lParam携带状态字
		break;
	case SMS_PHONE_BUTTON: ////电话按键,lParam携带按健值
		switch ( g_pL23MsgBuffer[1] ) //////判断用户按键
		{
		
		}
		
		break;
	case SMS_USER_SHUT://///用户终止
		/////////////通知用户，并且交出系统控制权
		///////通知链路层挂机
		NU_Control_Timer(&TS, NU_DISABLE_TIMER);
		g_pL23MsgBuffer[2] = 0x00;
		SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );

		break;
	}
}

///////////////Create消息
BOOL CSMSMessage::OnCreate ( )
{
	if ( CFrameWnd::OnCreate ( ) == FALSE )
		return FALSE;

	////////////////初始化


	return TRUE;
}


////////启动消息层
void RunSMSMessage ( )
{
	

	////创建定时器
	NU_Create_Timer(&SYJTS, "syjTS", SYJTimerOver, 8, 1000, 200, NU_DISABLE_TIMER);//Init 			
	
	//////在这里恢复信箱设置
	///// TODO: Your codes
	/////新乡1
		g_SystemOption.SMSStatus[0].bRevNumber = 0;
		g_SystemOption.SMSStatus[0].bSendNumber = 0;
		g_SystemOption.SMSStatus[0].bSendedNumber = 0;
		g_SystemOption.SMSStatus[0].bHavePasswordItem = 0;

		////信箱2
		g_SystemOption.SMSStatus[1].bSendNumber = 0;
		g_SystemOption.SMSStatus[1].bRevNumber = 0;
		g_SystemOption.SMSStatus[1].bSendedNumber = 0;
		g_SystemOption.SMSStatus[1].bHavePasswordItem = 0;

		////信箱3
		g_SystemOption.SMSStatus[2].bSendNumber = 0;
		g_SystemOption.SMSStatus[2].bRevNumber = 0;
		g_SystemOption.SMSStatus[2].bSendedNumber = 0;
		g_SystemOption.SMSStatus[2].bHavePasswordItem = 0;

		////公共信箱
		g_SystemOption.SMSStatus[3].bSendNumber = 0;
		g_SystemOption.SMSStatus[3].bRevNumber = 0;
		g_SystemOption.SMSStatus[3].bSendedNumber = 0;
		g_SystemOption.SMSStatus[3].bHavePasswordItem = 0;

	/////将Flash中的数据导到Memory
	int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////查找纪录
	BOOL bIsShutOffLED = TRUE;
	if ( r != 0 ){//////Finded data in flash
		
		
		for ( int i = 0; i < r;i ++ )
		{

			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///得到纪录指针

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

	if ( bIsShutOffLED ) ///如果有未读短信将LED电量
		ClearLED ( LED_SMS );
	else SetLED ( LED_SMS );

	///////初始化消息框
	if ( g_pShowMsg == NULL )
		g_pShowMsg = new CShowMessage;
	g_pShowMsg->Create ( "消息",
		0,CRect ( 14,60,142,100 ),0 );

	///// 启动消息层
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
//                   应用层   syj
///////////////////////////////////////////////////////////////
void InitSMSApp ( )
{
	///////////初始化短消息
	/////信箱1
	g_SystemOption.SMSStatus[0].iLockType = 0; ///锁的状态
	strcpy ( g_SystemOption.SMSStatus[0].szPassword,"" ); //密码
	strcpy ( g_SystemOption.SMSStatus[0].szName,"私人1" ); //名称
	g_SystemOption.SMSStatus[0].bSendedNumber = 0; ///发件箱数目
	g_SystemOption.SMSStatus[0].bHavePasswordItem = 0; ///已发信箱中加锁的数目
	g_SystemOption.SMSStatus[0].iRingID = 0;///Default ring
	g_SystemOption.SMSStatus[0].bIsCanDelete = FALSE;
	////信箱2
	g_SystemOption.SMSStatus[1].iLockType = 0;
	g_SystemOption.SMSStatus[1].bIsCanDelete = FALSE;
	strcpy ( g_SystemOption.SMSStatus[1].szName,"私人2" );
	strcpy ( g_SystemOption.SMSStatus[1].szPassword,"" ); //密码
	g_SystemOption.SMSStatus[1].bSendedNumber = 0;
	g_SystemOption.SMSStatus[1].bHavePasswordItem = 0;
	g_SystemOption.SMSStatus[1].iRingID = 0;///Default ring
	////信箱3
	g_SystemOption.SMSStatus[2].iLockType = 0;
	strcpy ( g_SystemOption.SMSStatus[2].szName,"私人3" );
	strcpy ( g_SystemOption.SMSStatus[2].szPassword,"" ); //密码
	g_SystemOption.SMSStatus[2].bSendedNumber = 0;
	g_SystemOption.SMSStatus[2].bHavePasswordItem = 0;
	g_SystemOption.SMSStatus[2].iRingID = 0;///Default ring
	g_SystemOption.SMSStatus[2].bIsCanDelete = FALSE;
	////公共信箱
	g_SystemOption.SMSStatus[3].iLockType = 0;
	strcpy ( g_SystemOption.SMSStatus[3].szName,"公共" );
	strcpy ( g_SystemOption.SMSStatus[3].szPassword,"" ); //密码
	g_SystemOption.SMSStatus[3].bSendedNumber = 0;
	g_SystemOption.SMSStatus[3].bHavePasswordItem = 0;
	g_SystemOption.SMSStatus[3].iRingID = 0;///Default ring
	g_SystemOption.SMSStatus[3].bIsCanDelete = FALSE;
	
	/////初始化发送、关闭的声音以及回送码
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

	//////////初始化常用语
	char *str[] = {
		"生日",
		"祝贺",
		"鼓励",
		"感谢",
		"道歉",
		"幽默"
	};

	////生日
	char *str0[] = {  
		"猪，你快快长，长快快！猪，你早日出栏！猪，你生日快乐！",	
		"在你生日的这一天，我没有跟你在一起，只希望你能快乐、健康、美丽，生命需要奋斗、创造和把握！生日快乐！",
		"祝你生日快乐！愿健康、快乐永远和你相伴！",
		"知道吗？今天是你生日。希望你今天过得开心。永远和快乐做伴！",
		"特别的爱,给特别的你,特别的日子,给你特别的祝福,祝---快乐每一天!"

	};
	/////祝贺
	char *str1[] = {
		"祝您：位高权重责任轻，钱多事少离家近，每日睡到天大亮，工资领到手抽筋，手下花钱你收礼，别人加班你加薪！",
		"恭喜你！！！新婚愉快，幸福美满，激情永在，白头偕老！",
		"春风洋溢你；家人关心你；爱情滋润你；财神系着你；幸运之星永远照着你。",
		"别太苦自己,别给自己压力过大,放松自己,保重自己才会拥有其他的一切.",
		"心若在，梦就在，天地之间还有真爱，看成败，人生豪迈，只不过是从头再来！愿您能走出人生的低谷！"
	};
	
	/////鼓励
	char *str2[] = {
		"梦自己想梦的，去自己想去的，做自己想做的，因为生命只有一次，机会不会再来!",
		"又在忙吗,你可千万要注意休息,把身体累坏了可不划算了..",
		"人生的成功,不在于拿到一幅好牌,而是怎样将坏牌打好.",
		"人在旅途,难免会遇到荆棘和坎坷,但风雨过后,一定会有美丽的彩虹.",
		"如果是鱼，就不要迷恋天空，如果是鸟，就不要迷恋海洋。"
	};
	
	/////感谢
	char *str3[] = {
		"我一直想谢谢你，谢谢你出现在我的生命里。我一直想告诉你，告诉你我真的很爱你。",
		"让我怎样感谢你，当我走向你的时候，我原想收获一缕春风，你却给了我整个春天",
		"要不要我谢，要谢你就说一声，你不说我怎么知道呢~只有你要谢的时候我谢，你不要谢的时候我就不谢……",
		"涅磐本欲赴真爱，化蝶只为觅花魂！",
		"谢谢你一路陪我走过风风雨雨,我真挚的朋友."
	};
	
	/////道歉
	char *str4[] = {
		"我其实很乖。",
		"我知道你很生气。而且你每次生气我都好害怕。理解我，好么？原谅我，好么？",
		"你好象生气了，我心里也不好受！如果是我酿成了此错，希望你能原谅！如果不能原谅，我自己也就无法原谅自己了！",
		"我只想说：“我错了”。",
		"你不要告诉我，你真的很生气。其实我知道，因为你很在乎我！而我也只能告诉你：我爱你！"
	};

	////幽默
	char *str5[] = {
		"妈妈看见汤姆放学一回家就拿着游泳衣往外跑.妈妈:\"你肚子痛才好,今天别去游泳啦!\"汤姆:\"没有关系,我可以躺着仰游.\"",
		"母亲:\"你的脸还干净,可手怎么这样脏?\"儿子:\"我刚才用手擦脸来着.\"",
		"导航员:\"请报告你的高度、位置.\" 飞行员:\"我大约1．8米高,现在正坐在驾驶员的座位上.\"",
		"导游对旅游者说:\"你们大家的车票都是去基辅的,可这趟火车是去哈尔科夫的.\" \"是吗?你们的司机常出这样的错吗?\"",
		"\"爸爸．今天老师对我说,有其父必有其子,这句话是什么意思呢?\",\"你一定又干了什么混帐事情!\"爸爸说."
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

//////////////////新业务处理
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
	
	/////初始化列表
	m_iMenuType = 0;
	char *strmenu[] = {
			"热线服务",
			"呼出限制",
			"闹钟服务",
			"免打扰服务",
			"转移呼叫",
			"呼叫等待",
			"遇忙回叫",
			"遇忙转移"
			
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

	if ( m_iMenuType == 0 ){ ////0级菜单
	
		m_nHelpId = HELP_SMS_NEW;
		SetWindowText ( "电话新业务" );
		PaintTitle ( CDC ( ) );

		char *strmenu[] = {
			"热线服务",
			"呼出限制",
			"闹钟服务",
			"免打扰服务",
			"转移呼叫",
			"呼叫等待",
			"遇忙回叫",
			"遇忙转移"
			
		};

	

		for ( i = 0;i < 8;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );
	
	}else if ( m_iMenuType == 1 ) { ///呼出限制的主菜单
		
		char *strmenu[] = {
			"限全部呼叫",
			"限国内国际长途",
			"限国际长途",
			"返回"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

		
	}else if ( m_iMenuType == 2 ) { ///呼出限制的子菜单
	
		char *strmenu[] = {
			"登记",
			"注销",
			"更改操作码",
			"返回"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

	
	}else if ( m_iMenuType == 3 ) { ////其他服务的子菜单
	
		
		char *strmenu[] = {
			"登记",
			"注销",
			"更改操作码",
			"返回"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

	}else if ( m_iMenuType == 4 ) { /////更改操作码
	
		char *strmenu[] = {
			"更改登记操作码",
			"更改注销操作码",
			"恢复缺省操作码",
			"返回"
		
		};

		for ( i = 0;i < 4;i ++ )
			m_ListView.AppendItem ( strmenu[i],0,NULL,ICON_SMSRIGHTARROW );

	}

	m_ListView.OnPaint ( CDC ( ) );
}

void CNewPhone::OnSelect ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	extern void DialOut(char *strDial);	// 拨号
	extern void OpenPhoneMic();			// 免提开

	
	UINT32 lItem = lParam;/////选中项
	char szTemp[16],szPhone[32];

	strcpy ( szTemp,"" );
	strcpy ( szPhone,"" );
	
	if ( m_iMenuType == 0 ) { ///当前为0级菜单
			
		m_iSelectedItem = lItem; ////纪录当前所选项
		///////////////更新帮助
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
		
		case 1:////呼出限制主菜单
			m_iMenuType = 1;
			break;
		default: ////其他菜单
			m_iMenuType = 3;
			
		}

		UpdateMenu ( );

		//////////更改窗口的标题
		char *strmenu[] = {
			"热线服务",
			"呼出限制",
			"闹钟服务",
			"免打扰服务",
			"转移呼叫",
			"呼叫等待",
			"遇忙回叫",
			"遇忙转移"
			
		};

		SetWindowText ( strmenu[lItem] );
		PaintTitle ( CDC ( ) );
		
	}else if ( m_iMenuType == 1 ) { ////当前为呼出限制的主菜单
	

		switch ( lItem )
		{
		case 0://///限制全部呼叫
			m_nHelpId = HELP_SMS_NEW_LIMIT_ALL;
			m_iMenuType = 2;
			m_iLimite = 1;
			break;
		case 1://///限制国内国际
			m_nHelpId = HELP_SMS_NEW_LIMIT_2;
			m_iMenuType = 2;
			m_iLimite = 2;
			break;
		case 2:////限制国际长途
			m_nHelpId= HELP_SMS_NEW_LIMIT_1;
			m_iMenuType = 2;
			m_iLimite = 3;
			break;
		case 3:   ////返回0级菜单
			m_iMenuType = 0;
			
		}

		UpdateMenu ( );

	}else if ( m_iMenuType == 2 ) {  ////当前为呼出限制的子菜单
	
		switch ( lItem )
		{
		case 0:////开启
			
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
				strcpy(todial.name, "新业务");
				strcpy(todial.number, szPhone);
				todial.type = CARD_NONE;
				AppDialOut(todial);
				  
				}

			}

			return;
		case 1:////关闭
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
				strcpy(todial.name, "新业务");
				strcpy(todial.number, szPhone);
				todial.type = CARD_NONE;
				AppDialOut(todial);

				}	
			}

			return;
		case 2:////更改
			m_nHelpId = HELP_SMS_NEW_CODE;
			m_iMenuType = 4;
			break;
		case 3: //////返回呼出限制主菜单
			m_iMenuType = 1;
			
		}

		UpdateMenu ( );
	
	}else if ( m_iMenuType == 3 ) { ////当前为其他子菜单
	
		switch ( lItem )
		{
		case 0:///开启相应的服务
			strcpy ( szPhone,g_SystemOption.szEnableCode[m_iSelectedItem] );
			//////////////不需要输入号码
			if ( m_iSelectedItem == 3 || ///免打扰
				 m_iSelectedItem == 5 || ///呼叫等待
				 m_iSelectedItem == 6) { ///遇忙回叫
			
				///Call
				SOFTDIAL	todial;
				todial.areacode[0] = '\0';
				todial.ip = -1;
				strcpy(todial.name, "新业务");
				strcpy(todial.number, szPhone);
				todial.type = CARD_NONE;
				AppDialOut(todial);


				return;
			}

			/////////////需要输入号码
			if ( m_iSelectedItem == 0 || ///热线服务
				 m_iSelectedItem == 4 || ///呼叫转移
				 m_iSelectedItem == 7 ) { ///遇忙转移
					
				if ( NumInputBox ( "输入电话号码",szTemp,16 ) == IDOK )
				{
					strcat ( szPhone,szTemp );
					strcat ( szPhone,"#" );

					///Call
					SOFTDIAL	todial;
					todial.areacode[0] = '\0';
					todial.ip = -1;
					strcpy(todial.name, "新业务");
					strcpy(todial.number, szPhone);
					todial.type = CARD_NONE;
					AppDialOut(todial);


				}

				return;
			}
			
			/////其他输入格式
			if ( m_iSelectedItem == 2 ) {////闹钟服务
				
				if ( NumInputBox ( "输入时间(HHMM)",szTemp,5 ) == IDOK )
				{
					for ( int i = 0;i < strlen ( szTemp );i ++ )
						if ( szTemp[i] < '0' ||
							 szTemp[i] > '9' )
						{
							MessageBox ( "请输入数字！",
								"错误",MB_OK );

							return ;
						
						}
					
					
					strcat ( szPhone,szTemp );
					strcat ( szPhone,"#" );

					///Call
					SOFTDIAL	todial;
					todial.areacode[0] = '\0';
					todial.ip = -1;
					strcpy(todial.name, "新业务");
					strcpy(todial.number, szPhone);
					todial.type = CARD_NONE;
					AppDialOut(todial);


				}

			}
			
			return;
		case 1:///关闭相应的服务
			strcpy ( szPhone,g_SystemOption.szDisableCode[m_iSelectedItem] );
			///Call
			SOFTDIAL	todial;
			todial.areacode[0] = '\0';
			todial.ip = -1;
			strcpy(todial.name, "新业务");
			strcpy(todial.number, szPhone);
			todial.type = CARD_NONE;
			AppDialOut(todial);
			return;
		case 2:///更改操作码
			m_nHelpId = HELP_SMS_NEW_CODE;
			m_iMenuType = 4;
			break;
		case 3:////返回0级菜单
			m_iMenuType = 0;
			
		
		}

		UpdateMenu ( );
	
	
	}else if ( m_iMenuType == 4 ) { /////当前为更改操作码菜单
	
		if ( lItem == 0 ){////更改相应的登记码
			
			if ( NumInputBox ( "输入登记操作码",
				 g_SystemOption.szEnableCode[m_iSelectedItem],5 ) == IDOK )
				/////保存系统设置
				SaveSystemOption ( &g_SystemOption );
			
			
			return;

		} else if ( lItem == 1 ) {/////更改相应的注销码
			
			if ( NumInputBox ( "输入注销操作码",
				 g_SystemOption.szDisableCode[m_iSelectedItem],5 ) == IDOK )
				/////保存系统设置
				SaveSystemOption ( &g_SystemOption );
			
			return;

		} else if ( lItem == 2 ) { /////恢复缺省操作码
			
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

			/////保存系统设置
			SaveSystemOption ( &g_SystemOption );

			MessageBox ( "恢复成功！","提示",MB_OK );

			return;
		
		}else if ( lItem == 3 ) {////返回0级菜单
		
			if ( m_iSelectedItem != 1 )
				m_iMenuType = 3;
			else m_iMenuType = 2;
		}
		
		UpdateMenu ( );
	
	}
	
}

///////////////////账单处理类
/////class CMoneyWnd
//////////////消息映射
BEGIN_MESSAGE_MAP(CMoneyWnd, CAppWnd) 
	ON_MESSAGE ( MONEY_GRID,CN_GRIDSEL,OnGridCmd ) ////网格控件消息
	ON_MESSAGE ( MONEY_DELETEALL,CN_COMMAND,OnDeleteAll ) /////删除纪录
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
	m_pGrid.SetColsWidth ( iWidthArray ,TRUE );////设置列宽


	////创建删除按钮
	rc.left = m_sRect.left +  109;
	rc.right = rc.left + 32;
	rc.top = 0;
	rc.bottom = SMS_APP_TITLE_HEIGHT;
	m_DeleteBtn.Create ( "全删",rc,this,MONEY_DELETEALL );
	return TRUE;
}
////删除消息
void CMoneyWnd::OnDeleteAll ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( MessageBox ( "全部删除吗？","警告",
		      MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
	
		int i;
		if ( m_iMoneyStyle == 0 ) ////简单话费账单
		{
		
			for ( i = 0;i < SMS_MONEY_MAX_NUMBER;i ++ )
				g_Money[i].bIsData = 0;

		}else if ( m_iMoneyStyle == 1 ){ ////详细账单
		
			for ( i = 0;i < SMS_MONEYEX_MAX_NUMBER;i ++ )
				g_MoneyEx[i].bIsData = 0;
		
		}
	}

}
////网格控件消息
void CMoneyWnd::OnGridCmd ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	UINT16 iItem = (int)lParam&0x0000ffff;
	///////////取得短信内容
	int pos = m_pGrid.GetTotalRows ( ) - iItem;
	
	MONEY money;

	if ( m_iMoneyStyle == 0 ){
	
		money = g_Money[pos];
		g_Money[pos].iconID = ICON_LETTERREADED;
	
	}else if ( m_iMoneyStyle == 1 ){
		
		money = g_MoneyEx[pos];
		g_MoneyEx[pos].iconID = ICON_LETTERREADED;
	}
	
	///////显示话单信息
	if ( !m_pShowMsg )
			m_pShowMsg = new CShowMessageEx;
		
		
	m_pShowMsg->SetTimerFlag ( 0 );
	m_pShowMsg->SetString ( money.szContent ); 
	m_pShowMsg->SetDestroyFlag ( 2 );
	m_pShowMsg->Create ( "话费信息",
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
	

	/////////更新网格控件
	m_pGrid.RemoveAll ( );
	int i;
	char *szItem[2];
	char pStyle[] = { 0 ,0};
    UINT32 pIconID[] = { 0 ,0};

	szItem[0] = new char[2];
	strcpy ( szItem[0],"" );
	szItem[1] = new char[21];
	
	if ( m_iMoneyStyle == 0 ){  /////普通话费
	
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
		
	
	}else if ( m_iMoneyStyle == 1 ) { ////详细话费
	
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
//////////////消息映射
BEGIN_MESSAGE_MAP(CReadSubscribeSMS, CAppWnd) 
	ON_MESSAGE ( READINGSUBSCRIBE_SHOWMENU,CN_COMMAND,OnShowMenu ) ////显示菜单
	ON_MESSAGE ( READINGSUBSCRIBE_DEL,CN_COMMAND,OnDel ) ////删除
	ON_MESSAGE ( READINGSUBSCRIBE_SAVEAS,CN_COMMAND,OnSaveAs ) //另存为
	ON_MESSAGE ( READINGSUBSCRIBE_SENDTO,CN_COMMAND,OnSendTo ) //发送
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

	
	 //////设置窗口的一些参数
	////////得到当前的短消息内容
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	
	strcpy ( g_ST.szPhone,"点这里输号码" );
	if ( strlen ( m_pst.szContent ) > 140 )
	{

		if ( ::MessageBox ( "只能存入前70个汉字，是否继续？",
			"提示",MB_YESNO|MB_ICONQUESTION ) == IDYES )
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
	m_pModifyLetter->Create("写短消息", 
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
		CRect(0 ,  0, 159 , 159) , 0);
			

	m_pModifyLetter->ShowWindow ( );

}
/////删除
void CReadSubscribeSMS::OnDel ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	//////////////删除纪录
	if ( ::MessageBox ( "确认删除该记录吗？","删除",
		MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
		
		memmove ( &g_SubscibeSMS[m_CurrentPos],
			&g_SubscibeSMS[m_CurrentPos+1],
			(g_iSubscribePos-m_CurrentPos-1)*sizeof ( SUBSCIBESTRUCT ) );
		g_iSubscribePos --;
		Destroy ( );
	}
	
}
/////另存为
void CReadSubscribeSMS::OnSaveAs ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	
	////////得到当前的短消息内容
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	
	strcpy ( g_ST.szPhone,"来自e消息" );
	if ( strlen ( m_pst.szContent ) > 140 )
	{

		if ( ::MessageBox ( "只能存入前70个汉字，是否继续？",
			"提示",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			char szTemp[SMS_CONTENT_MAX_NUMBER];
			memcpy ( szTemp,m_pst.szContent,140 );
			szTemp[140] = '\0';
			strcpy ( g_ST.szMsg,szTemp );
					
		}else return;
	
	}else strcpy ( g_ST.szMsg, m_pst.szContent );

	
	////// 填充时间
	if ( !strlen ( g_ST.szTime ) ) { //////如果时间是空，填充当前时间
									////否则以以前时间为准
		
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

	g_ST.Flag = 0;///只读
	g_ST.iChildMailBox = 0;/////保存到收信箱
	g_ST.iLetterStatus = ICON_LETTERUNREAD;////未读
	g_ST.bReadStatus = 0;

	////// 显示另存为设置窗口
	if ( m_pSaveAsWnd == NULL )
		m_pSaveAsWnd = new CSaveAsWnd;
	m_pSaveAsWnd->Create ( "选择目的信箱",
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_OKBOX,
		CRect ( 0,0,159,159 ) );

	CloseCursor ( );///Close cursor syj
	m_pSaveAsWnd->ShowWindow ( );

	
}
//////显示标题
void CReadSubscribeSMS::OnShowTitle ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	if ( !m_pShowTitle )
		m_pShowTitle = new CDisplayInformation;

	m_pShowTitle->SetStyle ( 1 );//号码显示
	m_pShowTitle->SetPhone ( m_pst.szTitle );

	/////计算窗口高度
	int len = strlen ( m_pst.szTitle );
	int iNum = len/16;
	if ( 16*iNum < len )
		iNum ++;

	int iHeight = 16*iNum + 8;
	m_pShowTitle->Create ( "标题",
			WS_CHILD | WS_VISIBLE,
			CRect ( 10,30,146,iHeight+30 ) ,0 );

	m_pShowTitle->ShowWindow ( );

} 
/////显示菜单
void CReadSubscribeSMS::OnShowMenu ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	////////将菜单显示出来
	m_Menu.PopupMenu ( m_sRect.left + 1,
					m_sRect.top + WND_TITLE_HEIGHT  );
}
////////初始化消息
BOOL CReadSubscribeSMS::OnCreate ( )
{

	if ( !CAppWnd::OnCreate ( ) )
		return FALSE;
	int i;
	///// Codes
	/////////////创建菜单激活按钮
	CRect rc( m_sRect.left,
			m_sRect.top ,
			m_sRect.left + 76,
			m_sRect.top+ SMS_APP_TITLE_HEIGHT  );

	m_ShowMenuBtn.Create ( "短消息",rc,
								this,READINGSUBSCRIBE_SHOWMENU );

	///////创建转发按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	
	m_AnswerBtn.Create ( "转发",rc,this,READINGSUBSCRIBE_SENDTO );
	///////创建删除按钮 
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;

	m_DelBtn.Create ( "删除",rc,this,READINGSUBSCRIBE_DEL );

	/////////////创建主题控件
	rc.left = m_sRect.right - 17;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 2;
	rc.right = m_sRect.right;
	rc.bottom = rc.top + 16;
	m_Name.Create ( "详",rc,this,
		READINGSUBSCRIBE_NAME ); 
	///////创建短消息的内容
	rc.left = m_sRect.left + 1;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 21;
	rc.right = m_sRect.right;
	rc.bottom = m_sRect.bottom;
	m_Content.Create ( m_pst.szContent,rc,this,WS_CHILD|WS_VISIBLE,
		READINGSUBSCRIBE_CONTENT,201 );

	////////////////创建菜单
	m_Menu.Create ( this );
	m_Menu.AppendItem ( "另存为",READINGSUBSCRIBE_SAVEAS );

	return TRUE;
}
////////销毁消息
BOOL CReadSubscribeSMS::OnDestroy ( )
{
	if ( m_pShowTitle )
	{
		delete m_pShowTitle;
		m_pShowTitle = NULL;
	}

	return CAppWnd::OnDestroy ( );
}

/////重画消息
void CReadSubscribeSMS::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );

	//// Your codes
	dc.SetBackColor ( );
	dc.TextOut ( m_sRect.left,
		m_sRect.top + WND_TITLE_HEIGHT + 2,"标 题:" );

	/////剪裁标题
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
////////////////e消息窗口类
BEGIN_MESSAGE_MAP(CSubscribeSMS, CAppWnd) 
	ON_MESSAGE ( SUBSCRIBE_GRID,CN_GRIDSEL,OnGridCmd ) ////网格控件消息
	ON_MESSAGE ( SUBSCRIBE_DELETEALL,CN_COMMAND,OnDeleteAll ) ///全部删除
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

	m_pGrid.SetColsWidth ( iWidthArray ,TRUE );////设置列宽


	//////创建全删按钮
	m_DeleteAll.Create ( "全删",
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
	szItem[0] = new char[2];/////图标
	szItem[1] = new char[17];////时间
	szItem[2] = new char[22];////标题

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
		

		////剪裁时间
		strcpy ( szTemp,"" );
		if ( ( len = strlen ( g_SubscibeSMS[i].szTime ) ) > 5 ){
			
			for ( n = 0;n < len;n ++ )
				if ( g_SubscibeSMS[i].szTime[n] == 0x20 ) break;

			memcpy ( szTemp,&g_SubscibeSMS[i].szTime[n+1],( len - n -1 )*sizeof ( char ) );
			szTemp[len - n -1] = '\0';
			strcpy ( szItem[2],szTemp );

		}else strcpy ( szItem[2],g_SubscibeSMS[i].szTime );////时间


		
		/////裁剪标题
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
///删除
void CSubscribeSMS::OnDeleteAll  ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( ::MessageBox ( "确定将所有订阅信件删除吗？","警告",
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
/////// 网格控件消息相应
void CSubscribeSMS::OnGridCmd  ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	UINT16 iItem = (int)lParam&0x0000ffff;
	///////////取得短信内容
	int pos = g_iSubscribePos - iItem;
	SUBSCIBESTRUCT st = g_SubscibeSMS[pos];
	
	if ( !m_pRaedSubscribe )
		m_pRaedSubscribe = new CReadSubscribeSMS;
	g_SubscibeSMS[pos].iconID = ICON_LETTERREADED;
	m_pRaedSubscribe->SetInformation ( pos,st );
	m_pRaedSubscribe->Create ( "短消息",WS_NORMAL,
		CRect ( 0,0,159,159 ) );

	m_pRaedSubscribe->ShowWindow ( );
	
}

/////////////class CDisplayInformation
//////////////消息映射
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
	if ( m_DisplayStyle == 0 ){ ////文本方式
		
		dc.Draw3dButton ( m_sRect,"" );
		
		int i;
		
		int y = m_sRect.top  + 3;

		dc.SetBackColor ( LGRAY );

		dc.TextOut ( m_sRect.left + 12,y,"信箱" );
		dc.TextOut ( m_sRect.left + 70,y,"未读信" );
		
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
			sprintf ( szTemp,"%d封",m_LetterNumber[i] );
			dc.TextOut ( m_sRect.left + 102,y,szTemp );

			y += 18;
		}

		
	}else if ( m_DisplayStyle == 1 ){ ////显示电话号码
	
		dc.Draw3dButton ( m_sRect,"" );

		dc.SetBackColor ( LGRAY );
		
		////多行
		int iLen = strlen ( m_szPhone );
		if ( iLen > 16 )
		{
			char szTemp[3];	
			int x = m_sRect.left + 4,
					y = m_sRect.top +4;

			for ( int i = 0; m_szPhone[i]!='\0'; i ++ )
			{
				if ( m_szPhone[i] & 0x80 ) ///汉字
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
	
	}else if ( m_DisplayStyle ==  2 ){ ////图形
	
	
	}
}

void CDisplayInformation::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;
	if(!IsLegalWnd())  //窗口已不在链表中!        hqf
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
//////////// class CShowQuestionWnd 家家e中询问方式的现实

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
	if ( m_iStyle == 0x02 ){ /////确认询问
		
		

		//////创建按钮
		rt.left = m_sRect.left + 20;
		rt.right = rt.left + 40;
		rt.top = m_sRect.bottom - 24;
		rt.bottom = m_sRect.bottom - 4;
		
		m_OKBtn.Create ( "确定",rt,this,SHOWQUESTION_OK );

		rt.left = m_sRect.right - 60;
		rt.right = m_sRect.right - 20;
		rt.top = m_sRect.bottom - 24;
		rt.bottom = m_sRect.bottom - 4;

		m_CancelBtn.Create ( "取消",rt,this,SHOWQUESTION_CANCEL );

		////创建文字提醒框
		rt.left = m_sRect.left ;
		rt.top = m_sRect.top + WND_TITLE_HEIGHT;
		rt.right = m_sRect.right;
		rt.bottom = m_sRect.bottom - 28;

		
		m_Edit.Create ( m_szMessage,rt,this,
			WS_CHILD|WS_VISIBLE,SHOWQUESTION_EDIT,256 );

	 
	}else if ( m_iStyle == 0x03 ){ /////多项选择方式
	
		/////创建确定
		//////创建按钮
		rt.left = m_sRect.left + 60;
		rt.right = rt.left + 40;
		rt.top = m_sRect.bottom - 24;
		rt.bottom = m_sRect.bottom - 4;
		
		m_OKBtn.Create ( "确定",rt,this,SHOWQUESTION_OK );

		
		/////创建多项选择控件
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

		m_pGrid.SetColsWidth ( iWidthArray ,TRUE );////设置列宽

		/////////填充网格
		m_IconID = ICON_SMSRIGHTARROW;
		FillGrid ( );
		
	
	}else if ( m_iStyle == 0x04 ) { /////数据输入方式
	
	

	}

}

//// Fill grid
void CShowQuestionWnd::FillGrid ( )
{

	int i,j = 0,n = 0;
	UINT8	bLen = g_pMenuBuffer[1]; ////长度
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
		
		if ( g_pMenuBuffer[i] == 0xff ) ////增加菜单一项
		{
			szItem[j] = '\0';
			if ( n == 0 ){ //标题
				
				SetWindowText ( szItem );
				PaintTitle ( CDC ( ) );

			}else if ( n == 1 ){ ///菜单项数

			
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

//////网格控件消息
void CShowQuestionWnd::OnGridCmd ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	UINT16 iRow =  lParam&0x0000ffff;
	
	//////重画网格控件
	m_IconID = m_pGrid.GetUnitIcon ( iRow,1 ) ;
	if ( m_IconID == ICON_SMSRIGHTARROW )
		m_IconID = ICON_OK;
	else m_IconID = ICON_SMSRIGHTARROW;

	m_pGrid.SetUnitIcon ( iRow,1,m_IconID,TRUE );

}
////// 确定消息
void CShowQuestionWnd::OnOKBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_iStyle == 0x02 ) { /////确定方式
	
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


	}else if ( m_iStyle == 0x03 ) { ///多选方式
	
#ifdef SMS_DTMF

		UINT16 iRows = m_pGrid.GetTotalRows ( );
		UINT8	iNum = 0x00;

		/////计算编号
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

		
		/////选择的总项数
		L3Buffer[0] = 0x00;
		L3Buffer[1] = iNum;

		L3Buffer[pos] = 0x7F;

#endif

#ifdef SMS_FSK

		UINT16 iRows = m_pGrid.GetTotalRows ( );
		UINT8	iNum = 0x00;

		/////计算编号
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

		
		/////选择的总项数
		L3Buffer[0] = 0x00;
		L3Buffer[1] = iNum;

		L3Buffer[pos] = 0x7F;

#endif
	
		SendMessageToSMSL2 ( Order_DTMF_Data_Msg_Chain,0,0 );

	}else if ( m_iStyle == 0x04 ) { ////输入方式 
	
	
	}

}

////// 不确定消息
void CShowQuestionWnd::OnCloseBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_iStyle == 0x02 ) { /////确定方式
	
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


	}else if ( m_iStyle == 0x03 ) { ///多选方式
	
	
	}else if ( m_iStyle == 0x04 ) { ////输入方式 
	
	
	}

}

////// 设置消息
void CShowQuestionWnd::SetMessage ( char *str )
{

	strcpy ( m_szMessage,str );
}

//////////// class CShowMessageEx 显示家家e中的信息
//////////////消息映射
BEGIN_MESSAGE_MAP(CShowMessageEx, CFrameWnd) 
	ON_MESSAGE ( SHOWMESSAGEEX_OK,CN_COMMAND,OnCloseBtn ) ////返回
	ON_MESSAGE ( NULL,WM_PAINT_SECOND,OnPaintSecond ) ////返回
END_MESSAGE_MAP

CShowMessageEx::CShowMessageEx ( )
{
	
	m_bIsNeedTimer = FALSE;
	m_bIsDestroy = 0;
}

CShowMessageEx::~CShowMessageEx ( )
{

}

////画秒数
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
/////返回
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
		strcpy ( szButtonTitle,"退出" );
	else strcpy ( szButtonTitle,"返回" );

	m_ReturnBtn.SetWindowText ( szButtonTitle );
	m_ReturnBtn.OnPaint ( CDC ( ) );

}

BOOL CShowMessageEx::OnCreate ( )
{
	if ( !CFrameWnd::OnCreate ( ) )
		return FALSE;

	m_iSecondNumber = 5;
	//// TODO: your codes
	/////创建EDIT控件
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
		strcpy ( szButtonTitle,"退出" );
	else strcpy ( szButtonTitle,"返回" );

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
			"5秒自动返回" );
	}*/

	CloseCursor ( );
}


/////////// class CSMSServiceWnd 家家e
//////////////消息映射
BEGIN_MESSAGE_MAP(CSMSServiceWnd, CAppWnd) 
	ON_MESSAGE ( SERVICE_GRID,CN_COMMAND,OnGridCmd ) /////网格控件的消息处理
	ON_MESSAGE ( NULL,WM_UPDATE_MENU,OnUpdateMenu ) ////更新客服中心菜单
	ON_MESSAGE ( NULL,WM_DISPLAY_MSG,OnDisplayMessage ) ///显示订阅的短消息内容
	ON_MESSAGE ( SERVICE_INPUTEDIT,CN_COMMAND,OnUserInput ) ///用户输入
	ON_MESSAGE ( SERVICE_OK,CN_COMMAND,OnOKBtn ) ////用户选定提交
END_MESSAGE_MAP

CSMSServiceWnd::CSMSServiceWnd ( )
{ 
//	m_pGrid = NULL; 
	m_InputLength = 0;
	m_InputNumberFlag = 0;
	m_pQuestionWnd = NULL;/////询问
}

CSMSServiceWnd::~CSMSServiceWnd ( )
{

}

/////// 初始化消息
BOOL CSMSServiceWnd::OnCreate ( )
{
	
	if ( !CAppWnd::OnCreate ( ) )
		return  FALSE;
	//// TODO: your coeds
	////// 创建网格控件
	CRect rc ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT,
		159,159 );

	m_pGrid.Create ( rc,this,LS_SMALLICON,SERVICE_GRID );
	m_pGrid.SetStatus ( LS_SMALLICON ,TRUE );

	/////创建显示提示控件
	m_Edit.Create ( "",
		CRect ( 5,
		m_sRect.top+WND_TITLE_HEIGHT+4,
		140,
		110 ),this,WS_CHILD,SERVICE_EDIT,247 );

	////创建输入控件
	/*m_InputEdit.Create ( "点这里输入...",
		CRect ( 5,120,140,136 ),this,WS_CHILD,
		SERVICE_INPUTEDIT,255 );*/
	m_InputEdit.Create ( "点这里输入...",
		CRect ( 5,118,140,134 ),this,
		SERVICE_INPUTEDIT );

	////创建提交按钮
	m_OKBtn.Create ( "确定",CRect ( 55,138,100,156 ),this,SERVICE_OK );
	
	m_OKBtn.SetWindowStyle ( m_OKBtn.GetWindowStyle( )&~WS_VISIBLE );

	//////////////发送短消息.向链路层发送请求
	SendMessageToSMSL2 ( Order_Connect_Server_Msg,0,0 );

	return TRUE;
}


///// 重画消息
void CSMSServiceWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );

	/// TODO:your coeds
	
	CloseCursor ( );
}

////// 销毁消息
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

	///////通知链路层挂机
	NU_Control_Timer(&TS, NU_DISABLE_TIMER);
	g_pL23MsgBuffer[2] = 0x00;
	SendMessageToSMSL2 ( Order_Disconnect_Server_Msg,0,0 );


	return CAppWnd::OnDestroy ( );
}

//////网格控件的消息
void CSMSServiceWnd::OnGridCmd ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	UINT32 lItem = lParam;/////选中项

	/*char ss[10];
	sprintf ( ss,"%d",m_BackModel );
	MessageBox ( "GridMessage",ss,MB_OK );*/
	lItem += 0x01;
	if ( m_BackModel == 0x01 ||
		 m_BackModel == 0x00 ){ ///// 菜单模式

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

///// 更新客服中心菜单
void CSMSServiceWnd::OnUpdateMenu ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	
	//// Close cursor
	CloseCursor ( );
	if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );
	//////销毁显示窗口
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
	UINT8	bLen = g_pMenuBuffer[1]; ////长度
	if ( bLen < 0 || bLen >255 ) bLen = 256;
	UINT8	bEnquire = g_pMenuBuffer[2];//询问方式
	UINT8	bSendback = g_pMenuBuffer[3];///回送方式
	m_BackModel = bSendback;
	m_EnquireModel = bEnquire; 
	
	char *szItem = new char[bLen];
	/*char ss[10];
	sprintf ( ss,"%d",bEnquire );
	MessageBox ( "4559",ss,MB_OK );   syj 2002.2.26*/ 

	int i,j,n = 0;
	UINT32 style;
	////// 删除所有项
	m_pGrid.DeleteAllItems ( );
	

	if  ( bEnquire == 0x01 ){ ////菜单询问或单项选择询问输入
		//////切换界面
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
			
			if ( g_pMenuBuffer[i] == 0xff ) ////增加菜单一项
			{
				szItem[j] = '\0';
				if ( n == 0 ){ //标题
					
					SetWindowText ( szItem );
					PaintTitle ( CDC ( ) );

				}else if ( n == 1 ){ ///菜单项数
				
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


	}else if ( bEnquire == 0x02 ) { ////确认询问输入
	
		////提取字串
		for ( i = 4;i < bLen + 2;i ++ )
			szItem[i-4] = g_pMenuBuffer[i];
		szItem[i-4] = '\0';

		
		if ( !m_pQuestionWnd ) ///
			m_pQuestionWnd = new CShowQuestionWnd;
		
		m_pQuestionWnd->SetMessage ( szItem );
		m_pQuestionWnd->SetShowStyle ( bEnquire );
		
			
		m_pQuestionWnd->Create ( "询问",
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD,
			CRect ( 0,0,159,159 ) );

		m_pQuestionWnd->ShowWindow ( );

		memset ( g_pMenuBuffer,0,256 ); /// Clear 0
		delete []szItem;
		return;
		
		
	}else if ( bEnquire == 0x03 ) { ////多项选择输入
	
		if ( !m_pQuestionWnd ) ///窗口不存在
			m_pQuestionWnd = new CShowQuestionWnd;

		m_pQuestionWnd->SetShowStyle ( bEnquire );

		m_pQuestionWnd->Create ( "选择",
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD,
			CRect ( 0,0,159,159 ) );

		m_pQuestionWnd->ShowWindow ( );
		
		memset ( g_pMenuBuffer,0,256 ); /// Clear 0
		delete []szItem;

		return;

	}else if ( bEnquire == 0x04 ) { ////非选择性数据输入
	
		style = m_pGrid.GetWindowStyle ( );
		m_pGrid.SetWindowStyle ( style&~WS_VISIBLE );
		
		style = m_Edit.GetWindowStyle ( );
		m_Edit.SetWindowStyle ( style|WS_VISIBLE );
		
		style = m_InputEdit.GetWindowStyle ( );
		m_InputEdit.SetWindowStyle ( style|WS_VISIBLE );
		m_InputEdit.SetWindowText ( "点这里输入..." );
		m_InputEdit.OnPaint ( CDC ( ) );


		style = m_OKBtn.GetWindowStyle ( );
		m_OKBtn.SetWindowStyle ( style|WS_VISIBLE );

		/////提取字符串消息
		for ( i = 4;i < bLen+2;i ++ )
		{
			szItem[i-4] = g_pMenuBuffer[i];
			if ( g_pMenuBuffer[i] == 0xff ) break;
		}

		szItem[i-4] = '\0';
		m_Edit.SetWindowText ( szItem );

		i ++;
		m_InputLength = g_pMenuBuffer[i++];////取得最大输入长度
		
		m_InputNumberFlag = g_pMenuBuffer[i];////取得详细输入信息
		
		//OnPaint ( CDC() );

	}
	

	memset ( g_pMenuBuffer,0,256 ); /// Clear 0
	delete []szItem;
	OnPaint ( CDC() );

}

//////////////////显示订阅的短消息内容
void CSMSServiceWnd::OnDisplayMessage ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( g_pShowMsg->GetVisibleFlag ( ) )
			g_pShowMsg->ShowWindow ( SW_HIDE );

	UINT8	bLen = g_pMenuBuffer[1],////长度
			bChildCmd = g_pMenuBuffer[2];//子命令码

	char    szTitle[40];
	char    szTemp[256];
	int i;

	GetWindowText ( szTitle,40 ); ///取得父窗口标题
	switch ( bChildCmd )
	{
	
	case 0x00: ////清屏
		//MessageBox ( "清屏","",MB_OK );
		break;
	case 0x01://///文本显示定位
		//MessageBox ( "文本显示定位","",MB_OK );
		break;
	case 0x02://///文本显示允许屏幕移动命令
		break;
	case 0x03:////文本提示行显示命令
		for ( i = 4;i < bLen + 2;i ++ )
		{
			szTemp[i-4] = g_pMenuBuffer[i];		

			if ( szTemp[i-4] == 0xff ) break;
		}

		szTemp[i-4] = '\0';
		/////显示e消息
		if ( !m_pShowMsg )
		{
			m_pShowMsg = new CShowMessageEx;
		
			/////是否启动定时器
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

			/////是否启动定时器
			if ( g_pMenuBuffer[3] == 0x01 )
				NU_Reset_Timer(&SYJTS, SYJTimerOver, 1000, 200, NU_ENABLE_TIMER);				
			
			m_pShowMsg->SetTimerFlag ( g_pMenuBuffer[3] );
			m_pShowMsg->SetString ( szTemp ); 
			m_pShowMsg->SetDestroyFlag ( 0 );

			m_pShowMsg->ChangeText (  );
			
		}

		break;
	case 0x04://///屏幕显示命令
		for ( i = 5;i < bLen + 2;i ++ )
		{
			szTemp[i-5] = g_pMenuBuffer[i];		
			if ( szTemp[i-5] == 0xff ) break;
		}

		szTemp[i-5] = '\0';
		
		/////显示e消息
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
	if ( InputBox ( "请输入内容",szTemp,m_InputLength ) == IDOK )
	{
		m_InputEdit.SetWindowText ( szTemp );
		m_InputEdit.OnPaint ( CDC ( ) );
		delete []szTemp;
	} 
}

///////////////用户选定提交
void CSMSServiceWnd::OnOKBtn ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	char *szTemp = new char[m_InputLength+1];
	m_InputEdit.GetWindowText ( szTemp,m_InputLength );
	UINT8  bLen = strlen ( szTemp );
	if ( m_InputNumberFlag&0x0f == 0x00 ){ ////不定，对用户输入不要求
	
		//TODO:your codes


	}else if ( m_InputNumberFlag&0x0f == 0x0f ){///必须输满
		
		/*if (  bLen != m_InputLength )
		{
			MessageBox ( "请按要求输满.","错误",MB_OK );
			delete []szTemp;
			return ;
		}*/

	}else {
	
		if ( bLen <= 0 )
		{
			MessageBox ( "输入框不能为空!","错误",MB_OK );
			delete []szTemp;
			return ;
		
		}
	}


	if ( m_EnquireModel == 0x01 ){ ///// 菜单模式
	
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
		////////////////将UINT8缓冲区拆成单字节
		int iLen;
		UINT8 *buffer = new UINT8[bLen+1];
		buffer[0] = bLen; ///长度
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

		////将buffer拷贝到指定缓冲区
		for ( iLen = 0;iLen < 2*(bLen+1);iLen ++ )
			L3Buffer[iLen] = buf[iLen];
		L3Buffer[iLen] = 0x7F;

		
		delete []buf;
		delete []buffer;
		delete []szTemp;
#endif

#ifdef SMS_FSK
		////////////////将UINT8缓冲区拆成单字节
		int iLen;
		UINT8 *buffer = new UINT8[bLen+1];
		buffer[0] = bLen; ///长度
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

		////将buffer拷贝到指定缓冲区
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
//////////////消息映射
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
	///////创建ListView
	m_pList.Create ( CRect ( m_sRect.left + 5,
							  m_sRect.top + 21,
							  m_sRect.right-4,
							  m_sRect.bottom-4 ),
							  this, LS_SMALLICON, 
							  CHOICEMODEL_LISTVIEW );
    
	m_pList.SetStatus(LS_SMALLICON, TRUE);
	
	/////创建OK,Close button
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
	///////////填充
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
		"请选择" );
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
		MessageBox ( "请选择常用语组项！","错误",
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

//////// 初始化消息
BOOL CSaveAsWnd::OnCreate ( )
{

	
	m_iItem = -1;
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;

	///////// TODO: your codes
	CRect rc ( m_sRect.left + 2,
			   m_sRect.top + WND_TITLE_HEIGHT + 4,
			   m_sRect.left + 22,
			   m_sRect.top + WND_TITLE_HEIGHT + 20 );
	/////私人信箱1
	m_mailbox1.Create ( "",rc,this );

	/////私人信箱2
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox2.Create ( "",rc,this );
	
	//// 私人信箱3
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox3.Create ( "",rc,this );

	//////公共信箱
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox4.Create ( "",rc,this );

	///自定义常用语
	rc.top += 24;
	rc.bottom = rc.top + 20;
	m_mailbox5.Create ( "",rc,this ,false,SAVEAS_MODLE );


	CloseCursor ( );

	return TRUE;
}

//////// 销毁消息
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
		MessageBox ( "没有建立常用语组。",
			"警告",MB_OK );
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
////// 重画消息
void CSaveAsWnd::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );
	///// TODO: your codes
	////// 输出信箱的名字,	输出信箱的状态
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
	dc.TextOut ( 50,y,"公共信箱" );

	y += 24;
	if ( m_iItem == -1 )
		dc.TextOut ( 50,y,"常用语" );
	else {
	
		MODEL *pTemp = GetModelPointer(g_Model_Locked[m_iItem]);	   	 
		char temp[20];
		
		strcpy ( temp,"常用语(" );
		strcat ( temp,pTemp->szModelName );
		strcat ( temp,")" );
		dc.TextOut ( 50,y,temp );
	}


}
////////Ok按钮
void CSaveAsWnd::OnOk ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	/////// TODO: your codes
	
	int r;

	if ( m_mailbox1.GetValue ( ) )///私人1选中
	{
	
		g_ST.bMailBox = 0;///当前选中的信箱
		g_ST.iChildMailBox = 0;/////保存到收信箱
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;

		r = AddRevRecord ( g_ST ,FALSE );
		if ( r == -1 )
			goto p2;
		
	}


p2:	if ( m_mailbox2.GetValue ( ) )///私人2选中
	{

			
				
		g_ST.bMailBox = 1;///当前选中的信箱
		g_ST.iChildMailBox = 0;/////保存到发信箱
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;


		r = AddRevRecord ( g_ST,FALSE );
		if ( r == -1 ) 
			goto p3;

		

			
	}

p3:	if ( m_mailbox3.GetValue ( ) )///私人3选中
	{

		g_ST.bMailBox = 2;///当前选中的信箱
		g_ST.iChildMailBox = 0;/////保存到发信箱
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;

		r = AddRevRecord ( g_ST,FALSE );
		if ( r == -1 ) 
			goto p4;

		

			
	}

p4:	if ( m_mailbox4.GetValue ( ) )///公共信箱选中
	{

		g_ST.bMailBox = 3;///当前选中的信箱
		g_ST.iChildMailBox = 0;/////保存到发信箱
		g_ST.iLetterStatus = ICON_LETTERUNREAD;
		g_ST.bReadStatus = 0;

		r = AddRevRecord ( g_ST,FALSE );
		if ( r == -1 ) 
			goto p5;

		

			
	}

p5:	if ( m_mailbox5.GetValue ( ) )///自定义常用语选中
	{

		//////保存到常用语
		CContentWnd dd;
		dd.OnSMSaveAs( g_Model_Locked[m_iItem], 
			g_ST.szMsg );
		
	}
	
	m_iItem = -1;
	Destroy ( );
}

/*============================================================================*/
//  CButton类函数定义                                                         //
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
	ON_MESSAGE ( SETNAMEPASSWORD_NAME,CN_COMMAND,OnNameInput ) /////姓名设置
	ON_MESSAGE ( SETNAMEPASSWORD_PASSWORD,CN_COMMAND,OnPasswordInput ) /////密码设置
	ON_MESSAGE ( SETNAMEPASSWORD_SHOWPOPUPLV,CN_COMMAND,OnShowLV ) ///显示ListBox
	ON_MESSAGE ( SETNAMEPASSWORD_POPUPLV,CN_LISTSEL,OnCurSel ) ///ListBox选择处理消息
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
/////// 初始化
BOOL CSetNamePassword::OnCreate ( )
{
	
	bSoundCtrlFlag = FALSE; ////声音播放控制

	if ( CAppWnd::OnCreate ( ) == FALSE ) return FALSE;

	if ( m_iStyle == 0 ) {  ////信箱设置
		////创建密码编辑控件
		if ( !m_bIsPublic )
			m_Password.Create ( g_SystemOption.SMSStatus[iItem].szPassword,
							CRect ( 46,
									m_sRect.top + WND_TITLE_HEIGHT + 22,
									m_sRect.right - 1,
									m_sRect.top + WND_TITLE_HEIGHT + 38 ),
							this,
							SETNAMEPASSWORD_PASSWORD ,TRUE );
		m_Password.SetStyle ( 1 );
		/////创建姓名编辑控件
		m_Name.Create ( g_SystemOption.SMSStatus[iItem].szName,
			CRect ( 46,
					m_sRect.top + WND_TITLE_HEIGHT + 2,
					m_sRect.right - 1,
					m_sRect.top + WND_TITLE_HEIGHT + 18 ),this,
					SETNAMEPASSWORD_NAME ,TRUE );
		m_Name.SetStyle ( 2 );
		////创建设置接收方式控件
		m_RevType.Create ( "短信息空间满时,拒收新短信息",
						   CRect ( m_sRect.left,
						   m_sRect.top + WND_TITLE_HEIGHT + 42,
						   m_sRect.right - 1,
						   m_sRect.top + WND_TITLE_HEIGHT + 74 ),
						   this,!g_SystemOption.SMSStatus[iItem].bIsCanDelete,
						   SETNAMEPASSWORD_REVTYPE );


		/////创建声音提示控件
		BOOL bIsSelect = TRUE;
		if ( g_SystemOption.SMSStatus[iItem].iRingID == 0xff )
			bIsSelect = FALSE;
		m_Sound.Create (  "有短信息到达时,使用声音提示",
						   CRect ( m_sRect.left,
								   m_sRect.top + WND_TITLE_HEIGHT + 78,
								   m_sRect.right - 1,
								   m_sRect.top + WND_TITLE_HEIGHT + 110 ),
						   this,bIsSelect,
						   SETNAMEPASSWORD_SOUND );

		//////创建声音名称控件
		m_SoundName.Create ( "",
							 CRect ( m_sRect.left + 16,
									 m_sRect.top + WND_TITLE_HEIGHT + 114,
									 m_sRect.left + 120,
									 m_sRect.top + WND_TITLE_HEIGHT + 130 ),
							 this,WS_CHILD|WS_VISIBLE,
							 SETNAMEPASSWORD_SOUNDPATH ,20);

		/////创建弹出ListBox的按钮
		m_ShowPopupLV.Create ( "",ICON_DOWNARROW,
							   CRect ( m_sRect.left + 121,
									   m_sRect.top + WND_TITLE_HEIGHT + 112,
									   m_sRect.left + 133 ,
									   m_sRect.top + WND_TITLE_HEIGHT + 130 ),
									   this,
									   SETNAMEPASSWORD_SHOWPOPUPLV );

		/////创建控制声音播放按钮
		m_SoundCtrl.Create ( "听",
			CRect ( m_sRect.left + 134 ,
					m_sRect.top + WND_TITLE_HEIGHT + 112,
					m_sRect.left + 150 ,
		    		m_sRect.top + WND_TITLE_HEIGHT + 130 ),
					this,
					SETNAMEPASSWORD_SOUNDSTOP,FALSE );
		////// 创建ListBox
		m_PopupLV.Create ( CRect ( 0,0,124,100 ),this,NULL,SETNAMEPASSWORD_POPUPLV );

		
		///////填充特色短消息振铃
		int r = FindRing ( g_pSMSDataIDBuf,RING_MAX_COUNT );
		for ( int i = 0;i < r;i ++ )
		{
			RING *pRingTemp = GetRingPointer ( g_pSMSDataIDBuf[i] );
			m_PopupLV.AppendItem ( pRingTemp->Name,0,0 );
		}
		////设置缺省得铃声
		if ( g_SystemOption.SMSStatus[iItem].iRingID != 0xff ){
			
			RING *pRingTemp = GetRingPointer ( g_SystemOption.SMSStatus[iItem].iRingID );
			m_SoundName.SetWindowText ( pRingTemp->Name );

		}else m_SoundName.SetWindowText ( m_PopupLV.GetListItemText(0) );

	
	}else if ( m_iStyle == 1 ) { ////设置声音

		/////////创建第二页的控件，缺省为隐藏
		UINT32 lStyle;
		m_EnableSound.Create ( "收发时，是否需要声音",
			CRect ( m_sRect.left,
					m_sRect.top + WND_TITLE_HEIGHT,
					m_sRect.right - 1,
					m_sRect.top + WND_TITLE_HEIGHT + 32 ),
					this,
					g_SystemOption.bIsNeedSound );

	
	}else if ( m_iStyle == 2 ) { /////回执设置
		//////////////////////////////////
		m_EnableReturn.Create ( "是否需要服务器回执",
			CRect ( m_sRect.left,
					m_sRect.top + WND_TITLE_HEIGHT,
					m_sRect.right - 1 ,
					m_sRect.top + WND_TITLE_HEIGHT + 32 ),
					this,
					g_SystemOption.bIsNeedReturn );


	}

	return TRUE;
}

////播放声音控制
void CSetNamePassword::OnSoundCtrl ( UINT32 mesage,UINT32 wParam,UINT32 lParam )
{
	
	if ( !bSoundCtrlFlag )
	{
		m_SoundCtrl.SetButtonCaption ( "停" );
		::sndPlayMusic(g_SystemOption.SMSStatus[iItem].iRingID,true,-1);
	
	}else {

		m_SoundCtrl.SetButtonCaption ( "听" );
		////停止当前声音
		StopRing();	
	}
	
	bSoundCtrlFlag = !bSoundCtrlFlag;
}
///////ListBox处理消息
void CSetNamePassword::OnCurSel ( UINT32 mesage,UINT32 wParam,UINT32 lParam )
{
	g_SystemOption.SMSStatus[iItem].iRingID = g_pSMSDataIDBuf[lParam&0x0000ffff];
	m_SoundName.SetWindowText ( m_PopupLV.GetListItemText(INT32(lParam)) );

	////改变声音控制状态
	bSoundCtrlFlag = FALSE;
	m_SoundCtrl.SetButtonCaption ( "听" );
	////停止当前声音
	StopRing();	
}
///////显示ListBox
void CSetNamePassword::OnShowLV ( UINT32 mesage,UINT32 wParam,UINT32 lParam )
{
	
	m_PopupLV.PopUpList ( m_sRect.left + 20,
					m_sRect.top + WND_TITLE_HEIGHT + 12 );
	
}
////// 重画消息
void CSetNamePassword::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );
	///// Add your codes
	
	if ( m_iStyle == 0 ){ ///第一页
		
		dc.SetBackColor ( );
		dc.SetLineStyle ( 1 );

		dc.TextOut ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 2,
			"名字:" );

		if ( !m_bIsPublic ) 
			dc.TextOut ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 22,
				"密码:" );
		else dc.TextOut ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 22,
				"公共信箱不可以加密" );

		
		dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 20 );
		dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 20 );
		
		dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 40 );
		dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 40 );

		
		dc.MoveTo ( m_sRect.left,m_sRect.top + WND_TITLE_HEIGHT + 76 );
		dc.LineTo ( m_sRect.right,m_sRect.top + WND_TITLE_HEIGHT + 76 );

		
		dc.SetLineStyle (  );
	}


	
}
///// 销毁
BOOL CSetNamePassword::OnDestroy ( )
{

	////停止当前声音
	StopRing();

	return CAppWnd::OnDestroy ( );
}
////// OK
void CSetNamePassword::OnOk ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	if ( m_iStyle == 0 ) { ///信箱设置

		char name[7],password[10];
		m_Name.GetWindowText ( name,7 );
		if ( iItem != 3 )
			m_Password.GetWindowText ( password,10 );

		strcpy ( g_SystemOption.SMSStatus[iItem].szName,name );
		if ( iItem != 3 ) ///公共信箱只能更改名字,不能更改密码
			strcpy ( g_SystemOption.SMSStatus[iItem].szPassword,password );

		if ( iItem != 3 )
			g_SystemOption.SMSStatus[iItem].iLockType = ( strlen ( password ) > 0 )?2:0 ;
	
	

		/////接收方式
		g_SystemOption.SMSStatus[iItem].bIsCanDelete = !m_RevType.GetValue ( );

		////振铃方式，oxff = 不振铃
		if ( !m_Sound.GetValue ( ) ) 
			g_SystemOption.SMSStatus[iItem].iRingID = 0xff;

		
	
	}else if ( m_iStyle == 1 ) { 
		
		g_SystemOption.bIsNeedSound = m_EnableSound.GetValue ( );
		
	}else if ( m_iStyle == 2 ) {
	
		///是否需要回执
		g_SystemOption.bIsNeedReturn = m_EnableReturn.GetValue ( );

	}

	/////保存短消息到系统
	SaveSystemOption ( &g_SystemOption );

	////停止当前声音
	StopRing();
	////销毁窗口
	Destroy ( );
}
//////设置姓名
void CSetNamePassword::OnNameInput ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	char szTemp[7];
	m_Name.GetWindowText ( szTemp,7 );
	if ( InputBox ( "请输入姓名",szTemp,7 ) == IDOK )
	{
		m_Name.SetButtonCaption ( szTemp );

	}
}
//////设置密码
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
	ON_MESSAGE ( SETLETTERBOX_LISTVIEW,CN_COMMAND,OnListViewCmd ) ///列表消息
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

//////列表消息函数
void CSetLetterBox::OnListViewCmd ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	iSelectedItem = lParam;///////得到所选项 
	OnSet ( uMessage,wParam,lParam );
}
////// 设定消息
void CSetLetterBox::OnSet ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	
	////// 如果原来有密码的话，提示输入密码
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

	m_pSetNamePassword->SetSettingStyle ( 0 );////信箱设置
	m_pSetNamePassword->SetItem ( iSelectedItem );
	if ( iSelectedItem == 3 )
		m_pSetNamePassword->SetMailBoxFlag ( TRUE );
	else m_pSetNamePassword->SetMailBoxFlag ( FALSE );
	m_pSetNamePassword->Create ( "设定信箱属性",
						WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD|WS_OKBOX,
						CRect ( 0,0,159,159 ),
						HELP_SMS_BOX_SETUP_DETAIL );


	m_pSetNamePassword->ShowWindow ( );
		
	
}
///// 初始化消息
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
///// 销毁消息
BOOL CSetLetterBox::OnDestroy ( )
{

	////// codes

	return CAppWnd::OnDestroy ( );
}
//// 重画消息
void CSetLetterBox::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );
	//// Add your codes
	////////////修改ListView的图标状态
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
	/////初始化变量

	m_pLetterBoxWnd = NULL;
	bIsDisplay = TRUE;////第一次进入信箱需要显示信箱中的未读信件
	m_pMailBoxInfo = NULL;
}

CDXXWnd::~CDXXWnd()
{
	
}

void CDXXWnd::OnLVCommand ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
	UINT32 uiItem = (UINT32)lParam;
	iCurrentLetterBox = uiItem;	
	
	
	//////私人信箱
	if ( g_SystemOption.SMSStatus[uiItem].iLockType == 2 )////需要解密
	{
		char szTemp[PASSWORDLENGTH];
		strcpy ( szTemp,g_SystemOption.SMSStatus[uiItem].szPassword );
		if ( InputPassword ( szTemp,
						PASSWORDLENGTH,1 ) == IDOK )
		{
			g_SystemOption.SMSStatus[uiItem].iLockType = 1;///已经解密
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

	///////创建信箱窗口
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


	/////设置图标1的状态
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[0].szName,
		ICON_SMS1 + g_SystemOption.SMSStatus[0].iLockType );

	////设置图标2的状态
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[1].szName,
		ICON_SMS1 + g_SystemOption.SMSStatus[1].iLockType );
	///设置图标3的状态
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[2].szName,
		ICON_SMS1 + g_SystemOption.SMSStatus[2].iLockType );
	
	///////设置公共信箱
	m_pDXXListView.AppendItem ( g_SystemOption.SMSStatus[3].szName,
		ICON_SMS0 );

	return TRUE;
}


void CDXXWnd::OnPaint ( CDC &dc )
{
	CAppWnd::OnPaint ( dc );
	/////// 修改列表的图标状态
	
	if ( bIsDisplay ) ///是否显示
	{
		bIsDisplay = FALSE;
		int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////查找纪录		
		UINT8 iNumber[4] = {0,0,0,0};
		int i;
		
		for ( i = 0;i < r;i ++ )
		{
			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///得到纪录指针
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
			m_pMailBoxInfo->Create ( "信箱信息",
				WS_CHILD | WS_VISIBLE,
				CRect ( 5,45,146,143 ) ,0 );

			m_pMailBoxInfo->ShowWindow ( );

		}
	
	}

}



BOOL CDXXWnd::OnDestroy ( )
{
	bIsDisplay = TRUE;////恢复信件提醒功能
	///////////恢复加锁状态
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
//////////////消息映射
BEGIN_MESSAGE_MAP(CChoiceMoneyWnd, CAppWnd) 
	ON_MESSAGE(CHOICEMONEY_LISTVIEW, CN_COMMAND , OnLVCommand) ///处理ListView
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
	///////////创建ListView控件
	m_ListView.Create ( CRect ( m_sRect.left,
								 m_sRect.top + WND_TITLE_HEIGHT ,
								 159,159 ),
								 this,
								 CHOICEMONEY_LISTVIEW );

	
	m_ListView.AppendItem ( "简单",
			ICON_MONEY  );
	
	m_ListView.AppendItem ( "详细",
			ICON_MONEYEX );
	
	return TRUE;
}

BOOL CChoiceMoneyWnd::OnDestroy ( )
{
	////////TODO: Your codes
	
	//////销毁话费显示窗口
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
	case 0: ////简单花费
		ShowMoney  ( 0 );
		break;
	case 1:
		ShowMoney  ( 1 );
		break;
	}
}

///////话费账单
void CChoiceMoneyWnd::ShowMoney ( UINT8 iStyle )
{

	if ( !m_pMoney )
		m_pMoney = new CMoneyWnd;

	if ( iStyle == 0 ){ ////简单花费
	
		m_pMoney->SetStyle ( 0 );
		m_pMoney->Create("简单话费信息", 
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
					CRect(0 ,  0, 159 , 159) , 
					HELP_SMS_MONEY_DETAIL );
	}else {  ////详细花费
	
		m_pMoney->SetStyle ( 1 );
		m_pMoney->Create("详细话费信息", 
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
					CRect(0 ,  0, 159 , 159) , 
					HELP_SMS_MONEY_DETAIL );

	}

	m_pMoney->ShowWindow ( );

}

////////////////////////  class HelloWorld 
//////////////消息映射
BEGIN_MESSAGE_MAP(CHelloWorld, CAppWnd) 
	ON_MESSAGE(SM_MAININTERFACE_LISTVIEW, CN_COMMAND , OnLVCommand) ///处理ListView
END_MESSAGE_MAP


CHelloWorld::CHelloWorld ( )
{
	m_pDXXWnd			= NULL; //////初始化
	m_pSETWnd			= NULL;
	m_pSubscribeWnd		= NULL;
	m_pChoiceMoney		= NULL;
	m_pNewPhone			= NULL;
}

CHelloWorld::~CHelloWorld ( )
{

}

//////////////消息过程函数
void CHelloWorld::OnLVCommand ( UINT32 nMessage,UINT32 wParam,UINT32 lParam )
{

	UINT32 uItem = ( UINT32 )lParam;

	switch ( uItem )
	{
	
	case 0:  /////短消息
		
		g_iServerType = 1;/////设置服务类型为短消息
		if ( m_pDXXWnd == NULL )
			m_pDXXWnd = new CDXXWnd;
		m_pDXXWnd->Create("信箱", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_BOX );
		
		
		m_pDXXWnd->ShowWindow();
		break;
	case 1://////常用语
		
		ModelCall = WYCALL;
		if ( m_pMODELWnd == NULL )
			m_pMODELWnd = new CMODELWnd;
		m_pMODELWnd->Create("常用语 ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , HELP_SMS_COLLECTION); 
		m_pMODELWnd->ShowWindow();
		break;
	case 2:////群发
		g_bIsGroup = TRUE;
		GroupCall = WYCALL;
		if ( m_pGRPSDWnd == NULL )
			m_pGRPSDWnd = new CGRPSDWnd;
		m_pGRPSDWnd->Create("群发", 
						 WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
						 CRect(0 ,  0, 159 , 159) , 
						 HELP_SMS_SEND_GROUP ); 
		m_pGRPSDWnd->ShowWindow();
			break;
	case 3:////家家e
		/////检测电源
		if ( TestLowPower ( ) )
		{
			MessageBox ( "无外接电源，无法连接！",
				"警告",MB_OK|MB_ICONSTOP ); 
			return;
		}

		bIsSuccessSend = TRUE;
		g_iServerType = 0;///在线
		if ( m_pService == NULL )
			m_pService = new CSMSServiceWnd;
		m_pService->Create("连接信息中心...", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_JIAJIAE );
				
		m_pService->ShowWindow();
		break;
	case 4: ////e消息
		if ( m_pSubscribeWnd == NULL )
			m_pSubscribeWnd = new CSubscribeSMS;

		m_pSubscribeWnd->Create("订阅的短消息", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_ESMS);
				
		m_pSubscribeWnd->ShowWindow();
		break;
	case 5:////电话新业务
		if ( !m_pNewPhone )
			m_pNewPhone = new CNewPhone;
		
		m_pNewPhone->Create("电话新业务", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_NEW );

		m_pNewPhone->ShowWindow ( );

		break;
	case 6:  ///////话费查询
		if ( !m_pChoiceMoney )
			m_pChoiceMoney = new CChoiceMoneyWnd;

		m_pChoiceMoney->Create("话费类型选择", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				0 );

		m_pChoiceMoney->ShowWindow ( );
		
		break;
	case 7: /////设置
		
		if ( m_pSETWnd == NULL )
			m_pSETWnd = new CSETWnd;
		m_pSETWnd->Create("设置", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_SETUP );
				
		m_pSETWnd->ShowWindow();
		break;

	}
	
}

/////////////////通用函数
BOOL CHelloWorld::InitSMMainInterface ( )
{
	if(!CAppWnd::OnCreate())
		return FALSE;

	///////////创建ListView控件
	m_MainInterfaceListView.Create ( CRect ( m_sRect.left,
											 m_sRect.top + WND_TITLE_HEIGHT ,
											 159,159 ),
											 this,
											 SM_MAININTERFACE_LISTVIEW );

	char *szLVTitle[] = {
			"信箱",
			"常用语",
			"群发",
			"家家e",
			"e消息",
			"新业务",
			"话费",
			"设置"
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
	
	/////销毁常用语、设置、群发
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

	/////销毁家家e窗口
	if ( m_pService != NULL )
	{
		delete m_pService;
		m_pService = NULL;
	}


	///// 销毁阅读窗口
	if ( m_pReadingLetter != NULL )
	{
		delete m_pReadingLetter;
		m_pReadingLetter = NULL;
	}

	//// 销毁修改窗口
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


	///////销毁消息框
	if ( g_pShowMsg )
		g_pShowMsg->Destroy ( );


	////销毁图形显示框
	if ( g_pGraphicsWnd ){
		
		delete g_pGraphicsWnd;
		g_pGraphicsWnd = NULL;
	
	}

	//////销毁话单选择窗口
	if ( m_pChoiceMoney )
	{
		delete m_pChoiceMoney;
		m_pChoiceMoney = NULL;
	}

	////销毁新业务窗口
	if ( m_pNewPhone )
	{
		delete m_pNewPhone;
		m_pNewPhone = NULL;
	}

	///////通知链路层挂机
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
	/////设置缺省信箱为公共信箱
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
		dc.TextOut(32,76,"正在装载数据...");
		InitSMSApp ( );
		g_SystemOption.nSmsInitFlag = 0x55AA00FF;
		SaveSystemOption ( &g_SystemOption );
	}
	//// 初始化短消息主界面
	if(!g_pHelloWorld)
		g_pHelloWorld = new CHelloWorld;
	::SetCurrentApp(APPID_SMS);
	g_pHelloWorld->Create("短消息服务", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX, 
			CRect(0 ,  0, 159 , 159) , HELP_SMS_MAIN );
	

	g_pHelloWorld->ShowWindow();
	
	
}
////////////////////////////////////////////////////////////////
///// class CReadingSMS
//////////////消息映射
BEGIN_MESSAGE_MAP(CReadingSMS, CAppWnd) 
	ON_MESSAGE ( READING_SHOWMENU,CN_COMMAND,OnShowMenu ) ////显示菜单
	ON_MESSAGE ( READING_DEL,CN_COMMAND,OnDel ) ////删除
	ON_MESSAGE ( READING_ANSWER,CN_COMMAND,OnAnswer ) //回复信件
	ON_MESSAGE ( READING_SENDTO,CN_COMMAND,OnSendTo ) //转发
	ON_MESSAGE ( READING_SAVEAS,CN_COMMAND,OnSaveAs )//另存为 
	ON_MESSAGE ( READING_SHOWPHONE,CN_COMMAND,OnShowPhone ) //显示电话号码
END_MESSAGE_MAP

CReadingSMS::CReadingSMS ( )
{
	m_pShowPhone = NULL;
}

CReadingSMS::~CReadingSMS ( )
{
	
}
//////显示号码
void CReadingSMS::OnShowPhone ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	if ( !m_pShowPhone )
		m_pShowPhone = new CDisplayInformation;

	m_pShowPhone->SetStyle ( 1 );//号码显示
	m_pShowPhone->SetPhone ( m_pst.szPhone );
	/////计算窗口高度
	int len = strlen ( m_pst.szPhone );
	int iNum = len/16;
	if ( 16*iNum < len )
		iNum ++;

	int iHeight = 16*iNum + 8;
	m_pShowPhone->Create ( "电话号码",
			WS_CHILD | WS_VISIBLE,
			CRect ( 10,30,146,iHeight+30 ) ,0 );

	m_pShowPhone->ShowWindow ( );

}
////另存为
void CReadingSMS::OnSaveAs ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	////// 显示另存为设置窗口
	g_ST = m_pst;
	
	if ( m_pSaveAsWnd == NULL )
		m_pSaveAsWnd = new CSaveAsWnd;
	m_pSaveAsWnd->Create ( "选择目的信箱",
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_OKBOX,
		CRect ( 0,0,159,159 ) );
	m_pSaveAsWnd->ShowWindow ( );

}
/////转发
void CReadingSMS::OnSendTo ( UINT32 message,UINT32 wParam,UINT32 lParam )
{
	if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

	 //////设置窗口的一些参数
	 g_bIsModel = FALSE;
	 g_ST = m_pst;
	 strcpy ( g_ST.szPhone,"点这里输号码" );
	 m_pModifyLetter->SetFlag ( false ); 
	 m_pModifyLetter->Create("写短消息", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_WRITE);
			

	m_pModifyLetter->ShowWindow ( );

	
}
/////删除
void CReadingSMS::OnDel ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	//////////////删除纪录
	if ( MessageBox ( "确认删除该记录吗？","删除",
		MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
		bool b = DeleteSMS ( m_iDataID );
		
		Destroy ( );
	}
	
}
/////显示菜单
void CReadingSMS::OnShowMenu ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	////////将菜单显示出来
	m_Menu.PopupMenu ( m_sRect.left + 1,
					m_sRect.top + WND_TITLE_HEIGHT  );
}
////////初始化消息
BOOL CReadingSMS::OnCreate ( )
{
	/////关闭声音
	StopRing();	


	if ( !CAppWnd::OnCreate ( ) )
		return FALSE;
	int i;
	///// Codes
	/////////////创建菜单激活按钮
	CRect rc( m_sRect.left,
			m_sRect.top ,
			m_sRect.left + 76,
			m_sRect.top+ SMS_APP_TITLE_HEIGHT  );

	//////////////////转化收件人字符
	char szTemp[9];
	if ( strlen ( m_pst.szPhone ) > 8 )
	{
		for ( i = 0;i < 8;i ++ )
			szTemp[i] = m_pst.szPhone[i];
		szTemp[i] = '\0';
		m_ShowMenuBtn.Create ( szTemp,rc,this,READING_SHOWMENU );

	}else m_ShowMenuBtn.Create ( m_pst.szPhone,rc,
								this,READING_SHOWMENU );

	///////创建回复按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	
	m_AnswerBtn.Create ( "回复",rc,this,READING_ANSWER );
	///////创建删除按钮 
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;

	m_DelBtn.Create ( "删除",rc,this,READING_DEL );

	/////////////创建显示完整电话号码按钮
	rc.left = m_sRect.right - 17;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 1;
	rc.right = m_sRect.right;
	rc.bottom = rc.top + 16;
	m_ShowPhone.Create ( "详",rc,this,READING_SHOWPHONE );
	///////创建短消息的内容
	rc.left = m_sRect.left + 1;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 41;
	rc.right = 159;
	rc.bottom = 159;/////SYJ modisy
	m_Content.Create ( m_pst.szMsg,rc,this,WS_CHILD|WS_VISIBLE,
		READING_CONTENT,SMS_CONTENT_MAX_NUMBER );

	////////////////创建菜单
	m_Menu.Create ( this );
	m_Menu.AppendItem ( "转发",READING_SENDTO );
	m_Menu.AppendItem ( "-",0 );
	m_Menu.AppendItem ( "另存为",READING_SAVEAS );

	return TRUE;
}
////////销毁消息
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

/////重画消息
void CReadingSMS::OnPaint ( CDC &dc )
{

	CAppWnd::OnPaint ( dc );


	//// Your codes
	dc.SetBackColor ( );/////设置背景颜色为白
	dc.TextOut ( m_sRect.left,
		m_sRect.top + WND_TITLE_HEIGHT + 2,"发信人:" );

	///////输出收件人姓名
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

//////////////回复消息
void CReadingSMS::OnAnswer ( UINT32 message,UINT32 wParam,UINT32 lParam )
{

	if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

	 //////设置窗口的一些参数
	 g_ST = m_pst;
	 g_bIsModel = FALSE;
	 m_pModifyLetter->SetFlag ( false ); 
	 m_pModifyLetter->Create("写短消息", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_WRITE );
			

	m_pModifyLetter->ShowWindow ( );

}

//////////////////// class LetterBoxWnd 
//////////////消息映射
BEGIN_MESSAGE_MAP(CLetterBoxWnd, CAppWnd) 
	ON_MESSAGE ( LETTERBOX_NEW_BTN, CN_COMMAND , OnNew) ///显示修改窗口
	ON_MESSAGE ( LETTERBOX_REV_BTN, CN_COMMAND, OnRevBtn ) ///接收按钮
	ON_MESSAGE ( LETTERBOX_SEND_BTN,CN_COMMAND,OnSendBtn ) ///发送按钮
	ON_MESSAGE ( LETTERBOX_SENDED_BTN,CN_COMMAND,OnSendedBtn )//已发按钮
	ON_MESSAGE ( LETTERBOX_GRIDVIEW,CN_GRIDSEL,OnGridCmd ) /////网格消息
	ON_MESSAGE ( LETTERBOX_DELETE_BTN,CN_COMMAND,OnDelete ) ////删除消息
END_MESSAGE_MAP


CLetterBoxWnd::CLetterBoxWnd()
{

	////////////// Init
//	m_pGridView = NULL;
	
}

CLetterBoxWnd::~CLetterBoxWnd()
{

}

//////删除消息处理
void CLetterBoxWnd::OnDelete ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	/////删除选中的子信箱
	if ( MessageBox ( "确定将子信箱信件全部删除吗？","警告",
		      MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
	
		int i;
		if ( iCurrentChildLetterBox != 2 ) {
		
			int r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////查找纪录			
			////////遍历查找符合条件的纪录，并删除
			for ( i = 0;i < r;i ++ )
			{
			
				LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///得到纪录指针
				if ( lpST->bMailBox == iCurrentLetterBox && /////判断数据是否属于当前选中信箱
					lpST->iChildMailBox == iCurrentChildLetterBox ) 
				{
				
					if ( !lpST->iLockStatus  ) ////没有加锁，可以删除
						DeleteSMS ( g_pSMSDataIDBuf[i] );
				
				}				
			}
		}


		if ( iCurrentChildLetterBox == 2 ) ///////////已发信箱包括内存与Flash两部分
		{
			for ( i = 0;i < SMS_MAX_SAVE_NUMBER;i ++ )			
				if ( !g_SendedSMS[iCurrentLetterBox][i].iLockStatus )//可以删除
				{
					
					if ( g_SendedSMS[iCurrentLetterBox][i].bIsFlash )
					{
						DeleteSMS ( g_SendedSMS[iCurrentLetterBox][i].dataID );
						g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem --;					
					}

					memset ( &g_SendedSMS[iCurrentLetterBox][i],0,sizeof ( SMS ) );
				
				}


			/////对内存中的纪录重新排序
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
	
		/////更新子窗口
		PaintGrid ( CDC ( ),TRUE  );

	}


}
////////////////网格消息处理
void CLetterBoxWnd::OnGridCmd ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{


	///////// 判断所选项，从FLash中读去数据
	UINT16 iRow =  lParam&0x0000ffff;
	UINT16 iCol =  (lParam>>16)&0x0000ffff;

	char password[PASSWORDLENGTH];
	
	UINT16 pos;
	SMS ss;
	
	if ( iCurrentChildLetterBox == 0 ||
		 iCurrentChildLetterBox == 1 )
	{
		g_CurrentID = g_pSMSDataTempIDBuf[iRow-1] ;
		LPSMS lpST = GetSMSPointer ( g_CurrentID  ); ///得到纪录指针

		ss.Flag = lpST->Flag;
		ss.bMailBox = lpST->bMailBox;/////////////接收方的信箱
		ss.iChildMailBox = lpST->iChildMailBox;/////信箱中子信箱的标示
		ss.iLockStatus = lpST->iLockStatus;///密码
		ss.iLetterStatus = lpST->iLetterStatus;
		ss.bReadStatus = lpST->bReadStatus;

		strcpy ( ss.szPhone,lpST->szPhone );//////最多5个电话号码
		strcpy ( ss.szTime,lpST->szTime );///////时间
		strcpy ( ss.szMsg,lpST->szMsg );//////短消息内容最多140个char
					/////或者70个汉字
		
			
	}else {
	
		pos = g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber - iRow;
		g_iCurrentPos = pos;///保存数组下标
		ss.Flag = g_SendedSMS[iCurrentLetterBox][pos].Flag;
		ss.bMailBox = g_SendedSMS[iCurrentLetterBox][pos].bMailBox;/////////////接收方的信箱
		ss.iChildMailBox = g_SendedSMS[iCurrentLetterBox][pos].iChildMailBox;/////信箱中子信箱的标示
		ss.iLockStatus = g_SendedSMS[iCurrentLetterBox][pos].iLockStatus; //Lock status
		ss.iLetterStatus = g_SendedSMS[iCurrentLetterBox][pos].iLetterStatus;
		ss.bReadStatus = g_SendedSMS[iCurrentLetterBox][pos].bReadStatus;

		ss.dataID = g_SendedSMS[iCurrentLetterBox][pos].dataID;
		strcpy ( ss.szPhone,g_SendedSMS[iCurrentLetterBox][pos].szPhone );//////最多5个电话号码
		strcpy ( ss.szTime,g_SendedSMS[iCurrentLetterBox][pos].szTime );///////时间
		strcpy ( ss.szMsg,g_SendedSMS[iCurrentLetterBox][pos].szMsg );//////短消息内容最多140个char
					/////或者70个汉字
	}


	if ( iCol == 1 )
	{
		/////确定加密			
		if ( iCurrentChildLetterBox == 0 ||
			iCurrentChildLetterBox == 1 )
		{

			if ( ss.iLockStatus )
			{
				ss.iLockStatus = 0;///枷锁
				ss.iLetterStatus = ICON_LETTERUNLOCK;
			
			}else {
			
				ss.iLockStatus = 1;///枷锁
				ss.iLetterStatus = ICON_LETTERLOCK; ///////信件的阅读状态
			
			}

			
			///////将变化的纪录写回Flash
			bool b = ModifySMS ( g_CurrentID,&ss );
		
		
		}else {
		
			
			/////奖发件箱中选中的信件保存到Flash
			/////设置加锁状态 
			if ( ss.iLockStatus )
			{
				ss.iLockStatus = 0;///枷锁
				g_SendedSMS[iCurrentLetterBox][pos].iLockStatus = 0;///枷锁
				ss.iLetterStatus = ICON_LETTERUNLOCK;
				g_SendedSMS[iCurrentLetterBox][pos].iLetterStatus = ICON_LETTERUNLOCK; ///////信件的阅读状态
				g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem --;

			}else {
			
				
				if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem>
					SMS_MAX_HAVEPASSWORD_NUMBER )
				{
					MessageBox ( "加锁失败。","错误",MB_OK );
					return;
				}

				ss.iLockStatus = 1;///枷锁
				g_SendedSMS[iCurrentLetterBox][pos].iLockStatus = 1;///枷锁
				ss.iLetterStatus = ICON_LETTERLOCK; ///////信件的阅读状态
				g_SendedSMS[iCurrentLetterBox][pos].iLetterStatus = ICON_LETTERLOCK; ///////信件的阅读状态

				g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem ++;	
				
			}

			////////新增一条纪录
			ss.bIsFlash = 1;//保存到Flash
			g_SendedSMS[iCurrentLetterBox][pos].bIsFlash = 1;//保存到Flash
			if ( ss.dataID == -1 )
			{
				int r = AppendSMS  ( &ss );
				if ( r == -1 ) 
					MessageBox ( "存储部件已满！","错误",MB_OK|MB_ICONSTOP );

				//将纪录的ID号写回Flash
				ss.dataID = r;
				g_SendedSMS[iCurrentLetterBox][pos].dataID = r;
				ModifySMS ( r,&ss );

				
			}else {

				ModifySMS ( g_SendedSMS[iCurrentLetterBox][pos].dataID,&ss );
				//ModifySMS ( ss.dataID,&ss );
			
			}

		}
		
		

		//////重画网格控件
		m_pGridView.SetUnitIcon ( iRow,iCol,ss.iLetterStatus,TRUE );
		
		return;
	}


	///////阅读信件
	if ( iCurrentChildLetterBox == 0 ){
	
		///////将变化的纪录写回Flash
		if ( ss.iLetterStatus != ICON_LETTERUNLOCK &&
			 ss.iLetterStatus != ICON_LETTERLOCK )
				ss.iLetterStatus = ICON_LETTERREADED;////已读

		ss.bReadStatus = 1;////已读
		bool b = ModifySMS ( g_CurrentID,&ss );


		if ( m_pReadingLetter == NULL )
			m_pReadingLetter = new CReadingSMS;
		//////初始化信件
		m_pReadingLetter->SetSMSInformation ( ss );
		m_pReadingLetter->SetDataID ( g_CurrentID );
		m_pReadingLetter->Create ( "",
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX,
					CRect ( 0,0,159,159 ),0 );
		
		m_pReadingLetter->ShowWindow ( );

		
		
	}else { /////发件箱、已发信箱
	
		if ( m_pModifyLetter == NULL )
			m_pModifyLetter = new CModifyLetterWnd;

		//////设置窗口的一些参数
		g_bIsModel = FALSE;
		g_ST = ss;
		m_pModifyLetter->SetFlag ( true );
		m_pModifyLetter->Create("新短消息", 
				WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
				CRect(0 ,  0, 159 , 159) , 
				HELP_SMS_WRITE );
		

		m_pModifyLetter->ShowWindow ( );

	
	}
	
	
}

////////更新网格控件
void CLetterBoxWnd::PaintGrid ( CDC &dc ,BOOL bIsDraw )
{
	/////// 恢复到缺省状态
	int i,r,len,n;
	char sTemp[81];
	iUnreadNumber = 0;


	m_pGridView.RemoveAll ( );
	if ( iCurrentChildLetterBox == 0 )////收件箱
	{
		g_SystemOption.SMSStatus[iCurrentLetterBox].bRevNumber = 0;

	}else if ( iCurrentChildLetterBox == 1 ){///发件箱
	
		
		g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber = 0;
	
	}else if ( iCurrentChildLetterBox == 2 ) {
		///已发
		
		g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber = 0;
		//g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem = 0;
	}

	if ( iCurrentChildLetterBox == 2 )  ////刷新已发信箱
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
				
				////剪裁时间
				strcpy ( sTemp,"" );
				if ( ( len = strlen ( g_SendedSMS[iCurrentLetterBox][i].szTime ) ) > 5 ){
					
					for ( n = 0;n < len;n ++ )
						if ( g_SendedSMS[iCurrentLetterBox][i].szTime[n] == 0x20 ) break;

					memcpy ( sTemp,&g_SendedSMS[iCurrentLetterBox][i].szTime[n+1],( len - n -1 )*sizeof ( char ) );
					sTemp[len - n -1] = '\0';
					strcpy ( szItem[2],sTemp );

				}else strcpy ( szItem[2],g_SendedSMS[iCurrentLetterBox][i].szTime );////时间


				////剪裁标题
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

				}else strcpy ( szItem[1],g_SendedSMS[iCurrentLetterBox][i].szPhone ); ////发送人

				char pStyle[]= {0,0,0};
				UINT32 pIconID[]= {g_SendedSMS[iCurrentLetterBox][i].iLetterStatus,0,0};

				if ( strlen ( szItem[1] ) > 0 )
				{
					m_pGridView.InsertRow (1, szItem,pStyle,pIconID );
						
					////将相应的信箱计数器增加
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


	//////////////////////其他信箱	
	r = FindSMS ( g_pSMSDataIDBuf,SMS_LETTER_MAX_NUMBER ); ////查找纪录
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

	if ( r != 0 ) { ////找到数据
		
		BOOL  bIsShutOffLED = TRUE;
		for ( i = 0;i < r;i ++ ) /////遍历
		{
		
			LPSMS lpST = GetSMSPointer ( g_pSMSDataIDBuf[i] ); ///得到纪录指针
			if ( lpST->iLetterStatus != ICON_LETTERREADED &&
				 lpST->iChildMailBox == 0 &&
				 lpST->bReadStatus != 1 )
				bIsShutOffLED = FALSE;///如果有未读点亮

			if ( lpST->bMailBox == iCurrentLetterBox && /////判断数据是否属于当前选中信箱
				lpST->iChildMailBox == iCurrentChildLetterBox ) {
				
				char *szItem[3];
				szItem[0] = new char[16];
				strcpy ( szItem[0] , "" );
				szItem[1] = new char[81];
				szItem[2] = new char[16];
				
				
				////剪裁时间
				strcpy ( sTemp,"" );
				if ( ( len = strlen ( lpST->szTime ) ) > 5 ){
					
					for ( n = 0;n < len;n ++ )
						if ( lpST->szTime[n] == 0x20 ) break;

					memcpy ( sTemp,&lpST->szTime[n+1],( len - n -1 )*sizeof ( char ) );
					sTemp[len - n -1] = '\0';
					strcpy ( szItem[2],sTemp );

				}else strcpy ( szItem[2],lpST->szTime );

				////剪裁标题
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

				}else strcpy ( szItem[1],lpST->szPhone ); ////发送人


				char pStyle[]= {0,0,0};
				UINT32 pIconID[]= {lpST->iLetterStatus,0,0};

				if ( lpST->iLetterStatus != ICON_LETTERREADED )
					iUnreadNumber ++;

				m_pGridView.InsertRow (1, szItem,pStyle,pIconID );
				///////保存纪录ID值
				for ( int j = SMS_MAX_SAVE_NUMBER - 1;j > 0;j -- )
				{
					g_pSMSDataTempIDBuf[j] = g_pSMSDataTempIDBuf[j-1];
					
				}
				
				////将纪录的ID号保存到临时数组中
				g_pSMSDataTempIDBuf[0] = g_pSMSDataIDBuf[i];

				////将相应的信箱计数器增加
				if ( iCurrentChildLetterBox == 0 )////收件箱
					g_SystemOption.SMSStatus[iCurrentLetterBox].bRevNumber ++;
				else if ( iCurrentChildLetterBox == 1 )///发件箱
					g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber ++;
				//else if ( iCurrentChildLetterBox == 2 ) ///已发
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
//////////////接收按钮
void CLetterBoxWnd::OnRevBtn ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	
	iCurrentChildLetterBox = 0;
	m_RevBtn.SetSelected ( TRUE );
	m_SendBtn.SetSelected ( FALSE );
	m_SendedBtn.SetSelected ( FALSE );

	PaintGrid ( CDC(),FALSE );///重画
	PaintChildWnd ( );
	
	 
}
/////////发送按钮
void CLetterBoxWnd::OnSendBtn ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	iCurrentChildLetterBox = 1; 
	
	m_RevBtn.SetSelected ( FALSE );
	m_SendBtn.SetSelected ( TRUE );
	m_SendedBtn.SetSelected ( FALSE );
	
	PaintGrid ( CDC(),FALSE );///重画
	PaintChildWnd ( );
	
}

/////////已发送按钮
void CLetterBoxWnd::OnSendedBtn ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	iCurrentChildLetterBox = 2;
	m_RevBtn.SetSelected ( FALSE );
	m_SendBtn.SetSelected ( FALSE );
	m_SendedBtn.SetSelected ( TRUE );

	
	PaintGrid ( CDC(),FALSE );///重画
	PaintChildWnd ( );
	
}

void CLetterBoxWnd::OnNew ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{

	if ( m_pModifyLetter == NULL )
		m_pModifyLetter = new CModifyLetterWnd;
	
	g_bIsModel = FALSE;
	strcpy ( g_ST.szPhone,"点这里输号码" );
	strcpy ( g_ST.szMsg,"<点这里输入短信内容>" );
	m_pModifyLetter->SetFlag ( false ); 
	g_CurrentID = -1;////初始化全局数据ID值

	m_pModifyLetter->Create("写短消息", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			HELP_SMS_WRITE );
	

	m_pModifyLetter->ShowWindow ( );

	
}




BOOL CLetterBoxWnd::OnCreate ( )
{

	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;

	iCurrentChildLetterBox = 0;////设置缺省信箱
	//////创建新建按钮
	CRect rc ( m_sRect.left + 78,
		       0,
			   m_sRect.left + 110,
			   SMS_APP_TITLE_HEIGHT );

	m_NewBtn.Create ( "撰写",rc,this,LETTERBOX_NEW_BTN );

	///////创建全删除按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	m_DeleteAll.Create ( "全删",rc,this,LETTERBOX_DELETE_BTN );
	/////创建接收按钮
	rc.left = m_sRect.left ;
	rc.right = rc.left + 48;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 1;
	rc.bottom = rc.top + SMS_APP_TITLE_HEIGHT;

	m_RevBtn.Create ( "收信箱",rc,this,LETTERBOX_REV_BTN );
	m_RevBtn.SetSelected ( TRUE );
	////创建发送按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 48;
	
	m_SendBtn.Create ( "发信箱",rc,this,LETTERBOX_SEND_BTN );
	m_SendBtn.SetSelected ( FALSE );
	/////创建已经发送的按钮
	rc.left = rc.right + 1;
	rc.right = 158;
	m_SendedBtn.Create ( "已发信",rc,this,LETTERBOX_SENDED_BTN );
	m_SendedBtn.SetSelected ( FALSE );

	////创建ListView控件
//	if ( m_pGridView == NULL )
//		m_pGridView = new CGrid;
	m_pGridView.Create ( this,CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT + 21,
								142,160 ),
			UNS_TEXT|WS_CHILD|TBS_SPACELINE,
			0,3,
			LETTERBOX_GRIDVIEW );

	UINT16 iWidthArray[] = {18,80,40};

	m_pGridView.SetColsWidth ( iWidthArray ,TRUE );////设置列宽


	return TRUE;
}

BOOL CLetterBoxWnd::OnDestroy ( )
{
	return CAppWnd::OnDestroy();
}	


void CLetterBoxWnd::OnPaint ( CDC &dc )
{
	/////////////////搜寻数据库，更新列表
	PaintGrid ( dc ,FALSE );
	//////////////// Paint
	CAppWnd::OnPaint ( dc );
	
	//////////////提醒
	if ( m_bIsNeedReDraw )
	{
		m_bIsNeedReDraw = FALSE;

		///////提醒用户清理信箱
		if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bRevNumber > 0.8*SMS_MAX_SAVE_NUMBER )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( "请及时清理收信箱" );
			NU_Sleep ( 300 );
			
		}


		if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendNumber > 0.8*SMS_MAX_SAVE_NUMBER )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( "请及时清理发信箱" );
			NU_Sleep ( 300 );
			
		}

		

		if ( g_SystemOption.SMSStatus[iCurrentLetterBox].bSendedNumber > 0.8*SMS_MAX_SAVE_NUMBER )
		{
			if ( !g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( "请及时清理已发信箱" );
			NU_Sleep ( 300 );
			
		}

		if ( g_pShowMsg->GetVisibleFlag ( ) )
				g_pShowMsg->ShowWindow ( SW_HIDE );

		//提醒用户有未读信件
		/*if ( iUnreadNumber > 0 )
		{
			char temp[40];
			sprintf ( temp,"您的信箱中有%d封未读信件",iUnreadNumber );

			g_pShowMsg->ShowWindow ( );
			g_pShowMsg->SetMessage ( temp );
			NU_Sleep ( 300 );
			g_pShowMsg->ShowWindow ( SW_HIDE );
			
			
		}*/

	}

}

////////////////// class ModifyLetterWnd 
//////////////消息映射
BEGIN_MESSAGE_MAP(CModifyLetterWnd, CAppWnd) 
	ON_MESSAGE(MODIFYLETTER_SEND_BTN, CN_COMMAND , OnSend)///发送
	ON_MESSAGE ( MODIFYLETTER_PHONE,CN_COMMAND,OnComboxInput )//输入电话
	ON_MESSAGE ( MODIFYLETTER_EDIT,CN_COMMAND,OnEditInput )//输入电话
	ON_MESSAGE ( MODIFYLETTER_MENU_BTN,CN_COMMAND,OnShowMenu ) ////显示菜单
	ON_MESSAGE ( MODIFYLETTER_WORD,CN_COMMAND,OnSelectWord ) ///选择常用语
	ON_MESSAGE ( MODIFYLETTER_SAVEAS,CN_COMMAND,OnSaveAs ) ///另存到
	ON_MESSAGE ( MODIFYLETTER_SAVE_BTN,CN_COMMAND,OnSave ) ///保存
	ON_MESSAGE ( MODIFYLETTER_SHOWLIST,CN_COMMAND,OnShowList ) ///显示List
	ON_MESSAGE ( MODIFYLETTER_COMBOX,CN_LISTSEL,OnCurSel )
	ON_MESSAGE ( MODIFYLETTER_GROUP_BTN,CN_COMMAND,OnGroupSend )////群发
	ON_MESSAGE ( MODIFYLETTER_SEARCH_BTN,CN_COMMAND,OnSearchCard ) ///名片
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
	////////////////数据打包
	/////检测电源
	if ( TestLowPower ( ) )
	{
		MessageBox ( "无外接电源，无法发送短消息！","警告",MB_OK|MB_ICONSTOP ); 
		return;
	}

	char pTextBuf[81];
	char pBuf[SMS_CONTENT_MAX_NUMBER];
	m_Phone.GetWindowText ( pTextBuf,81 );  ///Get the phone
	int i;
	//////判断电话号码合法性
	if ( !strlen ( pTextBuf ) )
	{
		MessageBox ( "收信人号码不能为空！","号码非法",
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
		
			MessageBox ( "请重新输入收信人号码！","号码非法",
					MB_OK|MB_ICONSTOP );
			return;
		}
		


	////Get content
	m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
	
	
	//// backup 
	memset ( &g_SendST,0,sizeof ( g_SendST ) );
	strcpy ( g_SendST.szPhone,pTextBuf );
	strcpy ( g_SendST.szMsg,pBuf );
		
	///////// 显示消息框
	//////////////发送短消息.向链路层发送请求
	SendMessageToSMSL2 ( Order_Connect_Server_Msg,0,0 );
	bIsSuccessSend = FALSE; ////标志置真
}
/////组合框的输入
void CModifyLetterWnd::OnComboxInput ( UINT32 nMessage, UINT32 wParam , UINT32 lParam )
{
	CloseCursor ( );
	char szTemp[81];
	m_Phone.GetWindowText ( szTemp,81 );
	if ( !strcmp ( szTemp,"点这里输号码" ) ||
		 !strcmp ( szTemp," <空号> " ) )
		strcpy ( szTemp,"" );

	if ( NumInputBox ( "请输入号码",szTemp,81 ) == IDOK )
	{
		if ( !strlen ( szTemp ) )
		{
			MessageBox ( "重新输入电话号码！",
				"号码非法",MB_OK|MB_ICONSTOP );
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
	if ( strcmp ( szTemp,"<点这里输入短信内容>" ) == 0 )
		strcpy ( szTemp,"" );

	if ( InputBox ( "请输入内容",szTemp,SMS_CONTENT_MAX_NUMBER,INIT_WRITE, TRUE ) == IDOK )
	{
		m_Edit.SetWindowText ( szTemp );
		bIsSaved = false;
	}

	
}

BOOL CModifyLetterWnd::OnDestroy ( )
{

	
	/////销毁窗口句并
	if ( m_pShowMsg )
	{
		delete m_pShowMsg;
		m_pShowMsg = NULL;
	}

	//////询问是否保存
	if ( !bIsSaved )
		if ( MessageBox ( "短消息已经改变，保存吗？",
			"提示",MB_YESNO|MB_ICONQUESTION ) == IDYES )
		{
			
			if ( bIsSendStatus ) ///菜单保存
				OnMenuSave ( 0,0,0 );
			else OnSave ( 0,0,0 );
			
		}
		
	g_CurrentID = -1;

	return CAppWnd::OnDestroy ( );
}

BOOL CModifyLetterWnd::OnCreate ( )
{

	bIsSaved = true;////缺省认为已经保存
	g_iServerType = 1;/////设置服务类型为短消息

	if ( CAppWnd::OnCreate ( ) == FALSE )
		return FALSE;

	int i;
	/////创建菜单激活按钮
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
	
		m_MenuBtn.Create ( "新短消息",rc,this,
			MODIFYLETTER_MENU_BTN );
	}
	
	
	//////创建发送按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	m_SendBtn.Create ( "发送",rc,this,MODIFYLETTER_SEND_BTN );
	
	//////创建保存按钮、删除按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 32;
	if ( bIsSendStatus )////删除
		m_SaveBtn.Create ( "删除",rc,this,MODIFYLETTER_SAVE_BTN );
	else m_SaveBtn.Create ( "保存",rc,this,MODIFYLETTER_SAVE_BTN );
	/////创建组合框
	rc.left = m_sRect.left ,
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 1,
	rc.right = m_sRect.left + 110,
	rc.bottom = m_sRect.top + WND_TITLE_HEIGHT + 20;
	
	m_Phone.Create ( g_ST.szPhone,rc,this,WS_VISIBLE,
			MODIFYLETTER_PHONE,81 );

	////////创建激活List按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 12;
	m_PopupListBox.Create ( "",ICON_DOWNARROW,rc,this,MODIFYLETTER_SHOWLIST );
	//////创建List
	rc.left = 0,
	rc.top = 0,
	rc.right = 124,
	rc.bottom = 60;
	m_List.Create ( rc,this,NULL,MODIFYLETTER_COMBOX );
	/////// 创建群发选择按钮
	rc.top = m_sRect.top + WND_TITLE_HEIGHT;
	rc.left = rc.right + 1;
	rc.right = rc.left + 16;
	rc.bottom = rc.top + 20;
	m_GroupBtn.Create ( "群",rc,this,MODIFYLETTER_GROUP_BTN );
	////创建查询按钮
	rc.left = rc.right + 1;
	rc.right = rc.left + 16;
	m_SearchBtn.Create ( "名",rc,this,MODIFYLETTER_SEARCH_BTN );
	
	/////创建编辑控件
	rc.left = m_sRect.left + 1 ;
	rc.top = m_sRect.top + WND_TITLE_HEIGHT + 21;
	rc.right = 159;
	rc.bottom = 159;

	m_Edit.Create ( g_ST.szMsg,rc,this,WS_VISIBLE,
			MODIFYLETTER_EDIT,SMS_CONTENT_MAX_NUMBER );
	
	

	//////创建菜单
	m_Menu.Create ( this );
	
	if ( !g_bIsModel )
	{
		m_Menu.AppendItem ( "选择常用语",MODIFYLETTER_WORD );
		m_Menu.AppendItem ("-" );
	}

	if ( bIsSendStatus )
		m_Menu.AppendItem ( "保存",MODIFYLETTER_MENUSAVE );
	m_Menu.AppendItem ( "另存到",MODIFYLETTER_SAVEAS );

	return TRUE;

									
}

///菜单保存
void CModifyLetterWnd::OnMenuSave ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	
	////////修改一条纪录
	char pTextBuf[81];
	char pBuf[SMS_CONTENT_MAX_NUMBER];
	m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
	m_Phone.GetWindowText ( pTextBuf,81 );

	////测试代码
	if ( !strcmp ( pTextBuf,"点这里输号码" ) )
		strcpy ( pTextBuf," <空号> " );

	strcpy ( g_ST.szPhone,pTextBuf );
	strcpy ( g_ST.szMsg,pBuf );
	g_ST.Flag = 0;///只读


	AddSendRecord ( g_ST,TRUE );///Add a record
	bIsSaved = true;
}
///名片选择
void CModifyLetterWnd::OnSearchCard ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	CloseCursor ( );
	extern short SelectTelNum(char *strTel , short nBufLen);
	char szTempPhone[16],buffer[81];
	
	
	SelectTelNum ( szTempPhone,16 );
	
	if ( strlen ( szTempPhone ) )
	{
		m_Phone.GetWindowText ( buffer,81 );
		if ( !strcmp ( buffer,"点这里输号码" ) )
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
		
			MessageBox("一次最多只能有5个号码！" , "错误",MB_OK|MB_ICONSTOP);
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
///群发
void CModifyLetterWnd::OnGroupSend ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	if ( g_bIsGroup )
	{
		MessageBox ( "您已经选择群发了!","失败",
			MB_OK|MB_ICONSTOP );
		return;
	}


	CloseCursor ( );
	GroupCall = SYJCALL;
	if ( m_pGRPSDWnd == NULL )
			m_pGRPSDWnd = new CGRPSDWnd;
	m_pGRPSDWnd->Create("群发", 
						 WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
						 CRect(0 ,  0, 159 , 159) , 0); 
	m_pGRPSDWnd->ShowWindow();

	
}
////选择常用语
void CModifyLetterWnd::OnSelectWord ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	/////设置调用标志
	CloseCursor ( );
	ModelCall = SYJCALL;
	
	if ( m_pMODELWnd == NULL )
			m_pMODELWnd = new CMODELWnd;
	m_pMODELWnd->Create("常用语 ", 
			WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
			CRect(0 ,  0, 159 , 159) , 
			
			HELP_SMS_COLLECTION ); 
	m_pMODELWnd->ShowWindow();
}
//// 保存
void CModifyLetterWnd::OnSave ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{

	//////检查是否达到最大存储条数
	if ( !bIsSendStatus ){ /////////保存功能

				
		////////新增一条纪录
		memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
		char pTextBuf[81];
		char pBuf[SMS_CONTENT_MAX_NUMBER];
		m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
		m_Phone.GetWindowText ( pTextBuf,81 );

		////测试代码
		if ( !strcmp ( pTextBuf,"点这里输号码" ) )
			strcpy ( pTextBuf," <空号> " );
		strcpy ( g_ST.szPhone,pTextBuf );
		strcpy ( g_ST.szMsg,pBuf );

		////// 填充时间
		FillTime ( g_ST.szTime );

		/*DateTime	date;
		GetSysTime ( &date );
		sprintf ( g_ST.szTime,"%d-%d-%d %d:%d",
			  date.year,
			  date.month,
			  date.day,
			  date.hour,
			  date.minute );*/

		g_ST.Flag = 0;///只读
		g_ST.bMailBox = iCurrentLetterBox;///当前选中的信箱
		g_ST.iChildMailBox = 1;/////保存到发信箱
		g_ST.iLetterStatus = ICON_DRAFT;////草稿
		
		g_ST.dataID = -1;		
		
		AddSendRecord ( g_ST,FALSE );///Add a record
			
		bIsSaved = true; 
		
	}else { //////删除功能
	
			//////////////删除纪录
			if ( MessageBox ( "确认删除该记录吗？","删除",
				MB_YESNO|MB_ICONQUESTION ) == IDYES )
			{
				if ( iCurrentChildLetterBox != 2 )
				{
					bool b = DeleteSMS ( g_CurrentID );	

				}else {
					
					if ( g_SendedSMS[iCurrentLetterBox][g_iCurrentPos].bIsFlash == 1 )
					{
						///////先将Flash中的纪录删掉
						int id = g_SendedSMS[iCurrentLetterBox][g_iCurrentPos].dataID;
						BOOL b = DeleteSMS ( id );
						g_SystemOption.SMSStatus[iCurrentLetterBox].bHavePasswordItem --;

					}

					///////删除内存中的纪录
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
////另存到
void CModifyLetterWnd::OnSaveAs ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	
	////////得到当前的短消息内容
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	char pTextBuf[81];
	char pBuf[SMS_CONTENT_MAX_NUMBER];
	m_Edit.GetWindowText ( pBuf,SMS_CONTENT_MAX_NUMBER );
	m_Phone.GetWindowText ( pTextBuf,81 );

	////测试代码

	if ( !strcmp ( pTextBuf,"点这里输号码" ) )
		strcpy ( pTextBuf," <空号> " );
	strcpy ( g_ST.szPhone,pTextBuf );
	strcpy ( g_ST.szMsg,pBuf );

	////// 填充时间
	if ( !strlen ( g_ST.szTime ) ) { //////如果时间是空，填充当前时间
									////否则以以前时间为准
		
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

	g_ST.Flag = 0;///只读
	g_ST.iChildMailBox = 1;/////保存到发信箱
	g_ST.iLetterStatus = ICON_DRAFT;////草稿	

	////// 显示另存为设置窗口
	if ( m_pSaveAsWnd == NULL )
		m_pSaveAsWnd = new CSaveAsWnd;
	m_pSaveAsWnd->Create ( "选择目的信箱",
		WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_OKBOX,
		CRect ( 0,0,159,159 ) );
	m_pSaveAsWnd->ShowWindow ( );
}
///显示菜单
void CModifyLetterWnd::OnShowMenu ( UINT32 uMessage,UINT32 wParam,UINT32 lParam )
{
	m_Menu.PopupMenu ( m_sRect.left + 1,
		m_sRect.top + WND_TITLE_HEIGHT  );
}

void CModifyLetterWnd::OnPaint ( CDC &dc )
{
	CloseCursor ( );

	CAppWnd::OnPaint ( dc );

	if ( ModelCall == ENDCALL )///选择常用语
	{
		ModelCall = WYCALL;
		char szTemp[SMS_CONTENT_MAX_NUMBER];
		strcpy ( szTemp,"" );

		m_Edit.GetWindowText ( szTemp ,SMS_CONTENT_MAX_NUMBER );
		

		if ( strlen ( g_ModelSend ) > 0 )
		{
			if ( strcmp ( szTemp,"<点这里输入短信内容>" ) == 0 &&
				 !bIsSendStatus )
				strcpy ( szTemp,"" );


			int len1 = strlen ( szTemp ),
				len2 = strlen ( g_ModelSend );
			if (  ( len1+len2 ) > SMS_CONTENT_MAX_NUMBER )
			{
				MessageBox ( "内容过长，添加失败！",
					"警告",MB_OK|MB_ICONQUESTION );
				
				
			}else strcat ( szTemp,g_ModelSend );
		}
		
		m_Edit.SetWindowText ( szTemp );

		bIsSaved = false;
		
	}

	if ( GroupCall == ENDCALL ){ /////选择群组
	
		GroupCall = WYCALL;
		char szTemp[81];
		int len;
		strcpy ( szTemp,"" );////Init
		for ( int i = 0;i < 5;i ++ ) ///检测电话号码是否合法
		{							////并拷贝到临时Buffer
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
			MessageBox ( "号码不能为空！","错误",
				MB_OK|MB_ICONSTOP );
			return;
		}


		if ( szTemp[len-1] == '#' )
			szTemp[len-1] = '\0';


		m_Phone.SetWindowText ( szTemp );
		bIsSaved = false;
		m_List.AppendItem ( szTemp,0,0 ); ///向通话记录中增加一条

	}
}
 
//////////////////////// 设置模块 -------- 王钰
/////////// class CSETWnd   09_设置  函数实现  start  wangyu ///////
BEGIN_MESSAGE_MAP(CSETWnd, CAppWnd) 
	ON_MESSAGE(SET_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage     
END_MESSAGE_MAP

CSETWnd::CSETWnd()
{
	/////初始化变量

	m_pCenterNoWnd  = NULL;
	m_pSetOther		= NULL;
	m_pSetLetterBox = NULL;  ///信箱设置
}

CSETWnd::~CSETWnd()
{
}

BOOL CSETWnd::OnCreate ( )
{
	if ( !CAppWnd::OnCreate ( ) ) return FALSE;
	////////创建ListView控件
	m_pSETListView.Create ( CRect ( m_sRect.left ,
								     m_sRect.top + WND_TITLE_HEIGHT,
								     159,159 ), this, SET_LISTVIEW );
	
	m_pSETListView.AppendItem ( "信息码",ICON_SETTING_CENTER_No);////信息中心号码
	m_pSETListView.AppendItem ( "声音",ICON_SMSSETSOUND );  ////接收设置///	
	m_pSETListView.AppendItem ( "回执",ICON_SMSSETRETURN );  ////接收设置///	
	m_pSETListView.AppendItem ( "信箱",ICON_SMMAINLV01 );  ////接收设置///	


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
	case 0:  //// 信息中心号码	 
		    if(!m_pCenterNoWnd)
			  m_pCenterNoWnd = new CCENTERNoWnd;
	     	m_pCenterNoWnd->Create( "信息中心号码", 
					                 WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX |
									 WS_CHILD | WS_OKBOX, 
						             CRect(0 ,  0, 159 , 159) ,
									 HELP_SMS_SETUP_NUM);		
	   	    m_pCenterNoWnd->ShowWindow ( );
		   	break; 
	
	case 1: /////声音
			if ( m_pSetOther == NULL )
				m_pSetOther = new CSetNamePassword;

			m_pSetOther->SetSettingStyle ( 1 );////声音设置
			m_pSetOther->Create ( "声音设置",
								WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD|WS_OKBOX,
								CRect ( 0,0,159,159 ),
								HELP_SMS_SETUP_SOUND );


			m_pSetOther->ShowWindow ( );

		    break;
	case 2:  /////回执		 
			if ( m_pSetOther == NULL )
				m_pSetOther = new CSetNamePassword;

			m_pSetOther->SetSettingStyle ( 2 );////回执设置
			m_pSetOther->Create ( "回执设置",
								WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD|WS_OKBOX,
								CRect ( 0,0,159,159 ),
								HELP_SMS_SETUP_REPLY );


			m_pSetOther->ShowWindow ( );

			 break;
	case 3: /////
		//////设置私人信箱
		if ( m_pSetLetterBox == NULL )
			m_pSetLetterBox = new CSetLetterBox;

		m_pSetLetterBox->Create ( "信箱设定",
					WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX|WS_CHILD, 
					CRect(0 ,  0, 159 , 159) , 
					HELP_SMS_SETUP_BOX ); 

		m_pSetLetterBox->ShowWindow ( );	
		break;
	
	}
}
 
BOOL CSETWnd::OnDestroy ( )
{	
	//////自己的销毁代码	 
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
////////  class CSETWnd        09_设置  函数实现        end   wangyu ///////

////////  class CCENTERNoWnd   09_设置_信息码 实现函数  start wangyu ///////
BEGIN_MESSAGE_MAP(CCENTERNoWnd, CAppWnd) 
	ON_MESSAGE ( CENTERNoWND_REV,CN_COMMAND,OnRevEditInput )	 
	ON_MESSAGE ( CENTERNoWND_SEND,CN_COMMAND,OnSendEditInput )
END_MESSAGE_MAP

void CCENTERNoWnd::OnRevEditInput ( UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
	char szTemp[32];
	m_RevEdit.GetWindowText ( szTemp,32 );
	if ( NumInputBox ( "输入接收号码",szTemp,32 ) == IDOK )	{
		m_RevEdit.SetWindowText ( szTemp );
		m_RevEdit.OnPaint ( CDC ( ) );
	}
}

void CCENTERNoWnd::OnSendEditInput ( UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{

	char szTemp[32];
	m_SendEdit.GetWindowText ( szTemp,32 );
	if ( NumInputBox ( "输入发送号码",szTemp,32 ) == IDOK )	{
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


	////////保存到系统设置
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
  	dc.TextOut(2,20,"信息中心接收号码：" );    

	dc.TextOut(2,80,"信息中心发送号码：" ); 
}
void CCENTERNoWnd::Destroy ( )
{
	CFrameWnd::Destroy();
}
////////  class CCENTERNoWnd   09_设置_信息码 实现函数  end   wangyu   ////////

 /////////////////////////////////////////////////////////////////////////
//////////
//////////  class CMODELWnd  01_常用语     实现函数  start   wangyu    ////  
//////////
/////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMODELWnd, CAppWnd) 
	ON_MESSAGE(MODEL_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage  
	ON_MESSAGE(MODEL_NewButton,CN_COMMAND,OnGRPSDNewBtn)
	ON_MESSAGE(MODEL_ComBox,CN_COMMAND,OnShowMenuBtn) 
	ON_MESSAGE(MODEL_Delete,CN_COMMAND,OnDelete)///删除  
    ON_MESSAGE(MODEL_Rename,CN_COMMAND,OnRename)///改名  
    ON_MESSAGE(MODEL_Taxis,CN_COMMAND,OnTaxis)///排序
END_MESSAGE_MAP

CMODELWnd::CMODELWnd()
{	/////初始化变量
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
	////////创建ListView控件
//	if(m_pModelListView==NULL) 
//	m_pModelListView = new CListView;
	m_pModelListView.Create ( CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT,
								159,159 ),this,MODEL_LISTVIEW );     
	///////创建常用语项    
	m_FirstPaint=TRUE;    
	MODEL *pTemp;
	g_nLocal = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);
	for(short i = 0; i< g_nLocal; i++)	{
	   pTemp = GetModelPointer(g_Model_Locked[i]);	 	   
	   m_pModelListView.AppendItem (pTemp->szModelName, ICON_SMMAINLV02 ); 	 
	}    
	//////////创建菜单下拉按钮
	m_ConMB.Create("常用语",CRect (m_sRect.left ,
		                           m_sRect.top  , 
								   m_sRect.left + TITLE_MENU_WIDTH - 1, 
								   m_sRect.top + TITLE_BAR_HEIGHT - 1),
    								this,MODEL_ComBox);
    //////////创建下拉菜单选项	
	if(!m_Content.Create(this))
		return FALSE;
	m_Content.AppendItem("常用语改名" ,MODEL_Rename, 0);
	m_Content.AppendItem("常用语删除" ,MODEL_Delete, 0);
	m_Content.AppendItem("常用语排序" , MODEL_Taxis,0);  
		//////创建新建按钮
	CRect rc ( m_sRect.left + 75,
		       0,
			   m_sRect.left + 109,
			   SMS_APP_TITLE_HEIGHT );
	m_pConNewButton.Create ( "新建",rc,this,MODEL_NewButton ); 
  return TRUE;
}

////////////显示“常用语”下拉菜单中的选项
void CMODELWnd::OnShowMenuBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{ 
	m_Content.PopupMenu ( m_sRect.left,
				m_sRect.top + WND_TITLE_HEIGHT );
}
                                        
	                    
////////// "常用语" "改名"选项--按下
void CMODELWnd::OnRename( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{      
             ModelModifyFlag =  0;
		     if(!m_pRenameWnd)
			  m_pRenameWnd = new CContent_ModWnd;
	     	m_pRenameWnd->Create("改名", 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
						          CRect(0 ,  0, 159 , 159) , 
								  HELP_SMS_COLLECTION_RENAME );		
	   	    m_pRenameWnd->ShowWindow(); 			 
}
//////////常用语  "删除"选项--按下
void CMODELWnd::OnDelete( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
             ModelModifyFlag =  1;
		     if(!m_pDeleteWnd)
			  m_pDeleteWnd = new CContent_ModWnd;
	     	m_pDeleteWnd->Create( "删除", 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
						             CRect(0 ,  0, 159 , 159) , 
									 HELP_SMS_COLLECTION_DEL );	
			
	   	    m_pDeleteWnd->ShowWindow ( ); 
}
//////////常用语  "排序"选项--按下
void CMODELWnd::OnTaxis( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	         ModelModifyFlag =  2;
		     if(!m_pTaxisWnd)
			  m_pTaxisWnd = new CContent_ModWnd;
	     	m_pTaxisWnd->Create( "排序", 
					              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD| WS_OKBOX, 
						             CRect(0 ,  0, 159 , 159) , HELP_SMS_COLLECTION_SORT);	
			

	   	    m_pTaxisWnd->ShowWindow ( ); 
}
//////////////新建常用语组
BOOL CMODELWnd::OnGRPSDNewBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
    MODEL  cc, *pTemp;
    short ModelID;    
	char  ModelName[10];

	g_nLocal = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);
	if(g_nLocal >= MAX_MODEL_NUMBER){
	  	MessageBox("存储区已满!" , "新建常用语组",MB_OK|MB_ICONSTOP);
		return FALSE; 
	}

	strcpy ( ModelName,"" );
	if(::InputBox("输入常用语组名", ModelName , 10)== IDOK)
	{
		memset(&cc, 0 ,sizeof(cc));
		if( strlen ( ModelName ) == 0 )		{
			MessageBox("名称不能为空!" , "新建常用语组",MB_OK|MB_ICONSTOP);
			return FALSE;
		}		

		strcpy(cc.szModelName , ModelName);	
		for(short i = 0; i< g_nLocal; i++){
			pTemp = GetModelPointer(g_Model_Locked[i]);
			if(strcmp(cc.szModelName , pTemp->szModelName)==0){
				::MessageBox("常用语组不能重名!" , "新建常用语组",MB_OK|MB_ICONSTOP);
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
			MessageBox("空间已满","新建常用语组" ,  MB_OK|MB_ICONEXCLAMATION);
	}
 return TRUE;
}

//////////////// 某常用语被选中
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
  // 重画常用语项	 
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
/////////    class CMODELWnd  02_常用语   实现函数  end   wangyu ////


/////////    class CContent_ModWnd      02_常用语_修改     实现函数  start   wangyu ////
BEGIN_MESSAGE_MAP(CContent_ModWnd, CAppWnd) 
    ON_MESSAGE ( MODELMod_LISTVIEW,CN_COMMAND,OnModifyModel )    
    ON_MESSAGE ( MODEL_TaxisUpButton,CN_COMMAND,OnTaxisModelUp )
	ON_MESSAGE ( MODEL_TaxisDwButton,CN_COMMAND,OnTaxisModelDw )
END_MESSAGE_MAP 
////  控制常用语修改的选项
UINT32 CContent_ModWnd::OnModifyModel(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    switch(ModelModifyFlag)	{
	    case 0:  // 改名	         
		      OnRenameModel(0, 0, lParam);
		      break;	
	    case 1:  // 删除		   
		      OnDeleteModel(0, 0, lParam);
		      break;	     
	    default : 
		        break;	 
	}
}
//////////////////  删除 常用语组
UINT32 CContent_ModWnd::OnDeleteModel(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{	
	UINT32 uItem = lParam;  
    if( ::MessageBox("确定要删除此项吗?",
		           "删除!",MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{

		//m_pConModifyListView.DeleteItem(uItem );  	
		 
		g_nLocal = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);
		MODEL *pTemp = GetModelPointer(g_Model_Locked[(short)uItem]);
		short g_nAllNumber = FindAllContent(g_Content_locked, MAX_CONTENT_NUMBER);// 所有项个数
		// 删除在此组的所有项
		for(short i=0; i<g_nAllNumber; i++)	{
			CONTENT *pcTemp = GetContentPointer(LOWORD(g_Content_locked[i]));  
			if(pcTemp->iModelID == pTemp->iDataID) {
				bool del = DeleteContent(pcTemp->iDataID);	
				g_Content_locked[i] =  g_Content_locked[i+1]; 
				g_nNumber--;
			} 
		}
		// 删除常用语组 
		bool del = DeleteMODEL(g_Model_Locked[(short)uItem]);
		g_nLocal--;
		for(short i = uItem; i<= g_nLocal; i++)
		   g_Model_Locked[i] =  g_Model_Locked[i+1];	  
		// 重画
		m_pConModifyListView.DeleteAllItems();
		for(short i = 0; i< g_nLocal; i++) {
			pTemp = GetModelPointer(g_Model_Locked[i]);	    
	   		m_pConModifyListView.AppendItem (pTemp->szModelName,0,0,ICON_MODEL_Content,LOWORD(g_Model_Locked[i]) ); 	 
		 }
		
		OnPaint(CDC());
	}
		
	return TRUE;
}
////////////////// 常用语组 改名 　
UINT32 CContent_ModWnd::OnRenameModel(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{ 	
    char SetTemp[10];
 	UINT32 uItem = lParam; 
	
	MODEL cc, *pTemp;
    pTemp = GetModelPointer(g_Model_Locked[(short)uItem]);	 

	strcpy (  SetTemp, pTemp->szModelName );
 	if( InputBox ( "请输入新的名称",SetTemp,10 ) == IDOK ) {   
		if( SetTemp[0]== '\0'){
			MessageBox("名称不能为空" , "修改常用语名称",MB_OK|MB_ICONSTOP);
			return FALSE;
		}
        strcpy ( cc.szModelName, SetTemp );
        
		
		/*for( short i = 0; i< g_nLocal; i++){
			 pTemp = GetModelPointer(g_Model_Locked[i]);
			 if( strcmp(cc.szModelName , pTemp->szModelName)==0){
				 MessageBox("常用语组不能重名!" , "新建常用语组",MB_OK|MB_ICONSTOP);
				 return FALSE;
			 }
		} */  


        bool modify = ModifyModel(g_Model_Locked[(short)uItem], &cc);
       // 重画
	   m_pConModifyListView.DeleteAllItems();
 	   for(short i = 0; i< g_nLocal; i++)  {
	      pTemp = GetModelPointer(g_Model_Locked[i]);	    
	      m_pConModifyListView.AppendItem (pTemp->szModelName,0,0,ICON_MODEL_Content,LOWORD(g_Model_Locked[i]) ); 	 
	   }
 	   OnPaint(CDC());	
	}
	return TRUE;    
} 
//////////////////  常用语上移 
UINT32 CContent_ModWnd::OnTaxisModelUp(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{ 
    INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pConModifyListView.GetSelItem();   
 
	if(uItem == -1 ){
		MessageBox("请先选中要操作的常用语!","提示",MB_OK);
	    return FALSE;
	}
    if(uItem==0) {	
		MessageBox("所选中的常用语已是第一项!","提示",MB_OK);
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
//////////////////////下移按钮 响应函数
UINT32 CContent_ModWnd::OnTaxisModelDw(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{    
	INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pConModifyListView.GetSelItem();   
 
	if(uItem == -1 )	{
		MessageBox("请先选中要操作的常用语!","提示",MB_OK);
	    return FALSE;
	}
    if(uItem==(g_nLocal-1)) {    ////最后一项	
		MessageBox("所选中的常用语已是最后一项!","提示",MB_OK);
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
	case 0:  // 常用语改名	
	case 1:  // 常用语删除		   
		   m_pConModifyListView.Create ( CRect ( m_sRect.left, m_sRect.top + WND_TITLE_HEIGHT, 159,159 ),
								          this , MODELMod_LISTVIEW );    	  	       
		 break;
	case 2:  // 常用语排序
		   m_pConModifyListView.Create ( CRect ( m_sRect.left, m_sRect.top + WND_TITLE_HEIGHT, 159,159 ),
								          this, LS_SMALLICON,  MODELMod_LISTVIEW );  
	       m_pUpButton.Create ( "上移",CRect( m_sRect.left + 58, 0, m_sRect.left + 91,  SMS_APP_TITLE_HEIGHT ),
									this,MODEL_TaxisUpButton );  
		   m_pDwButton.Create ( "下移",CRect( m_sRect.left + 92, 0, m_sRect.left + 125, SMS_APP_TITLE_HEIGHT ),
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
/////////    class CContent_ModWnd  02_常用语_修改     实现函数  end    wangyu ////

/////////    class CContentWnd      02_常用语_内容     实现函数  start   wangyu ////
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
    // 网格
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
           strcpy ( szItem[0], szTemp);   ///序号
           strcpy ( szItem[1], pTemp->szContent); ///内容		   
		   m_pGridView.AddRow(szItem, pUnitStyle, pIconID); 		     
	   } 	   
	}  
	delete []szItem[0];
	delete []szItem[1];*/

	CRect rc ( m_sRect.left + 75, 0, m_sRect.left + 109, SMS_APP_TITLE_HEIGHT );    
	m_pNewButton.Create ( "新建", rc, this, Content_NewButton );
	return TRUE;
}
///////////另存
BOOL CContentWnd::OnSMSaveAs(short ModelID, char *content)
{   
   CONTENT cc,*pTemp;
   //int  nModel = FindMODEL(g_Model_Locked, MAX_MODEL_NUMBER);///查找常用语组
   int nContent = FindContent(ModelID, g_Content_locked, MAX_CONTENT_NUMBER);
   strcpy(cc.szContent , content);	
   for(short i = 0; i< nContent;  i++){
	   pTemp = GetContentPointer(g_Content_locked[i]);
	   if(strcmp(cc.szContent , pTemp->szContent)==0){
	   ::MessageBox("此常用语内容已存在!" , "另存......",MB_OK|MB_ICONSTOP);
	   return FALSE;
	   }
   }   
   short ContentID = AppendContent(&cc);
   if(ContentID >=0){			 
      cc.iModelID = ModelID;  // 所属组的ID
	  cc.iDataID =  ContentID;    // 常用语项的ID
	  g_Content_locked[g_nNumber++] = ContentID;			 
	  bool modify = ModifyContent(ContentID, &cc);		 
   }else
	  MessageBox("空间已满!","另存......" ,  MB_OK|MB_ICONEXCLAMATION);
}
/////////////// 新建一条常用语项
UINT32 CContentWnd::OnNewContent(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    CONTENT  cc, *pTemp;
    short ContentID;    
	char  content[SMS_CONTENT_MAX_NUMBER];
 
	int g_nAllNumber = FindAllContent(g_Content_locked, MAX_CONTENT_NUMBER);
	if( g_nAllNumber == MAX_CONTENT_NUMBER){
		MessageBox("存储区已满!" , "新建常用语项",MB_OK|MB_ICONSTOP);
		return FALSE;
	}    
	g_nNumber = FindContent(ModelFlagID, g_Content_locked, MAX_CONTENT_NUMBER);
	content[0] = '\0';
	if(::InputBox("输入常用语内容", content , SMS_CONTENT_MAX_NUMBER)== IDOK)
	{
		memset(&cc, 0 ,sizeof(cc));
		if(content[0]== '\0'){
			MessageBox("内容不能为空" , "新建常用语项",MB_OK|MB_ICONSTOP);
			return FALSE;
		}
		strcpy(cc.szContent , content);	
		for(short i = 0; i< g_nNumber; i++)	{
			pTemp = GetContentPointer(g_Content_locked[i]);
			if(strcmp(cc.szContent , pTemp->szContent)==0) {
			   ::MessageBox("此常用语内容已存在!" , "新建常用语项",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}   
		ContentID = AppendContent(&cc);
		if(ContentID >=0){			 
		   cc.iModelID = ModelFlagID;  // 所属组的ID
		   cc.iDataID =  ContentID;    // 常用语项的ID
		   g_Content_locked[g_nNumber++] = ContentID;			
		   bool modify = ModifyContent(ContentID, &cc);		 
		}
		else
			MessageBox("空间已满","新建常用语组" ,  MB_OK|MB_ICONEXCLAMATION);
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
	      m_ContentList->Create( "查看", WS_NORMAL | WS_CHILD, CRect(0 ,  0, 159 , 159) ,HELP_SMS_COLLECTION_PHRASE);		
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
	   for(short i=1; i<=g_nNumber+1; i++)	//删除	    	   
	        m_pGridView.DeleteRow(1);   
	   for(short i=1; i<=g_nNumber; i++){   //重画	
		   pTemp = GetContentPointer(g_Content_locked[i-1]); 
	       if( pTemp->iModelID==ModelFlagID) {   
		       sprintf ( szTemp,"%d",i );
               strcpy ( szItem[0], szTemp);   ///序号


			   //////剪裁标题 syj 2002.1.15
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
					strcpy ( szItem[1], szTemp1); ///内容	
				
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
/////////    class CContentWnd          02_常用语_内容         实现函数  end   wangyu ////

/////////    class CContentListWnd      02_常用语_内容_查看    实现函数  start   wangyu /////
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
	////创建 编辑控件
	CRect rc(m_sRect.left + 1, m_sRect.top + WND_TITLE_HEIGHT + 1, 159, 159);
	char szBuffer[256];
	//strcpy ( szBuffer,"<内容>" );  syj modify
	strcpy ( szBuffer,pTemp->szContent );
	m_Edit.Create ( szBuffer, rc, this, WS_VISIBLE, LISTCONTENT, 147 );
	////////////按钮
	CRect Delrc ( m_sRect.left + 44,  0, m_sRect.left + 76,  SMS_APP_TITLE_HEIGHT );
	CRect Modrc ( m_sRect.left + 77,  0, m_sRect.left + 109, SMS_APP_TITLE_HEIGHT );
	CRect Sedrc ( m_sRect.left + 110, 0, m_sRect.left + 142, SMS_APP_TITLE_HEIGHT );
    
	m_pListDelete.Create ( "删除", Delrc, this, ConList_DeleteButton );
	m_pListModify.Create ( "修改", Modrc, this, ConList_ModifyButton );
	m_pListSend.Create   ( "发送", Sedrc, this, ConList_SendButton );
}
 
///////////  删除一条常用语项
UINT32 CContentListWnd :: OnDelete ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ) 
{  
 	short Item= (short)ContentNO; 
    
	if(MessageBox("确定删除此条常用语吗?","删除",MB_YESNO|MB_ICONQUESTION)==IDYES)
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
	if(InputBox ( "请输入新内容",szTemp,140 ) == IDOK )
	{   
		if(szTemp[0]== '\0'){
			MessageBox("内容不能为空!" , "修改",MB_OK|MB_ICONSTOP);
			return FALSE;
		}

		cc = *pTemp;
        strcpy ( cc.szContent, szTemp );
        
		/*for(short i = 0; i< g_nNumber; i++)	{
			
			pTemp = GetContentPointer(g_Content_locked[i]);

			if(strcmp(cc.szContent , pTemp->szContent)==0){
				MessageBox("常用语已存在!" , "修改",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		} */  

		cc.iModelID = ModelFlagID;
		bool modify = ModifyContent(g_Content_locked[(short)ContentNO], &cc);
        ///////重画
	    //strcpy ( Buffer,"<内容>");
	    strcpy ( Buffer,szTemp);
		m_Edit.SetWindowText ( Buffer );
 	    PaintChildWnd();	 
	}   
	return TRUE;
}
///////////发送给宋英剑
UINT32 CContentListWnd :: OnSend ( UINT32 nMessage, UINT32 wParam , UINT32 lParam ) 
{
    CONTENT   *pTemp = GetContentPointer(g_Content_locked[(short)ContentNO]);    
      
    if ( m_pModifyLetter == NULL )
		 m_pModifyLetter = new CModifyLetterWnd;
	//////设置窗口的一些参数	  
	memset ( &g_ST,0,sizeof ( g_ST ) );///// clear 0
	g_bIsModel = TRUE;
	strcpy(g_ST.szMsg , pTemp->szContent);
	strcpy(g_ST.szPhone,"点这里输号码");
	m_pModifyLetter->SetFlag ( false ); 
	g_CurrentID = -1;////初始化全局数据ID值
	m_pModifyLetter->Create("写短消息", 
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
////////     class CContenListtWnd      02_常用语_内容_查看     实现函数  end   wangyu /////
////////
////////     常用语结束
////////////////////////////////////// 
 
//////////////////////////////////////////////////////////////////////
/////////
/////////   class CGRPSDWnd  01_群发     实现函数  start   wangyu //// 
/////////
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGRPSDWnd, CAppWnd) 
	ON_MESSAGE(GRPSD_LISTVIEW,CN_COMMAND,OnLVCommand)////ListViewMessage  
	ON_MESSAGE(GRPSD_NewButton,CN_COMMAND,OnGRPSDNewBtn)
	ON_MESSAGE(GRPSD_ComBox,CN_COMMAND,OnShowMenuBtn) 
	ON_MESSAGE(GRPSD_Delete,CN_COMMAND,OnDelete)///删除  
    ON_MESSAGE(GRPSD_Rename,CN_COMMAND,OnRename)///改名  
    ON_MESSAGE(GRPSD_Taxis,CN_COMMAND,OnTaxis)///排序
END_MESSAGE_MAP

CGRPSDWnd::CGRPSDWnd()
{	/////初始化变量
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
	////////创建ListView控件
//	if(m_pGrpSdListView==NULL) 
//	m_pGrpSdListView = new CListView;
	m_pGrpSdListView.Create ( CRect ( m_sRect.left ,
								m_sRect.top + WND_TITLE_HEIGHT,
								159,159 ),this,GRPSD_LISTVIEW );
	///////创建群组项
	ModifyFlag=TRUE;
	for(int i=0; i<GroupNumber; i++){		 
        m_pGrpSdListView.AppendItem ( g_pGroup[i].szName,ICON_SMMAINLV03 ); 
	}
	//////////创建菜单下拉按钮
	m_GrpSdMB.Create("群发",CRect (m_sRect.left ,
		                           m_sRect.top  , 
								   m_sRect.left + TITLE_MENU_WIDTH - 1, 
								   m_sRect.top + TITLE_BAR_HEIGHT - 1),
    								this,GRPSD_ComBox);
    //////////创建下拉菜单选项	
	if(!m_GrpSd.Create(this))
		return FALSE;
	m_GrpSd.AppendItem("群发改名" ,GRPSD_Rename, 0);
	m_GrpSd.AppendItem("群发删除" ,GRPSD_Delete, 0);
	m_GrpSd.AppendItem("群发排序" , GRPSD_Taxis,0);  
		//////创建新建按钮
	CRect rc ( m_sRect.left + 75,  0,
			   m_sRect.left + 109, SMS_APP_TITLE_HEIGHT );
	m_pGSNewButton.Create ( "新建",rc,this,GRPSD_NewButton ); 
  return TRUE;
}

////////////显示“群发”下拉菜单中的选项
void CGRPSDWnd::OnShowMenuBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{ 
	m_GrpSd.PopupMenu ( m_sRect.left,
				        m_sRect.top + WND_TITLE_HEIGHT );
}           
///////// 创建"类别改名"窗口
void CGRPSDWnd::OnRename( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{   
	if(GroupNumber > 0){
       GrpSDModifyFlag =  0;
	   if(!m_pRenameWnd)
	       m_pRenameWnd = new CGroup_ModWnd;
	       m_pRenameWnd->Create( "改名", 
		     	              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
			                  CRect(0 ,  0, 159 , 159) , HELP_SMS_SEND_GROUP_RENAME);		
	   m_pRenameWnd->ShowWindow ( ); 			
	}
}
////////// 创建"类别删除"窗口
void CGRPSDWnd::OnDelete( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	if(GroupNumber > 0){
       GrpSDModifyFlag =  1;
	   if(!m_pDeleteWnd)
	      m_pDeleteWnd = new CGroup_ModWnd;
	      m_pDeleteWnd->Create( "删除", 
			              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD  , 
			              CRect(0 ,  0, 159 , 159) , HELP_SMS_SEND_GROUP_DEL);	
	   m_pDeleteWnd->ShowWindow ( ); 
	}
}
////////// 创建"类别排序"窗口
void CGRPSDWnd::OnTaxis( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{  
	if(GroupNumber > 0){
	   GrpSDModifyFlag =  2;
	   if(!m_pTaxisWnd)
	      m_pTaxisWnd = new CGroup_ModWnd;
	      m_pTaxisWnd->Create( "排序", 
			              WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX | WS_CHILD| WS_OKBOX, 
			             CRect(0 ,  0, 159 , 159) , 0);	
	   m_pTaxisWnd->ShowWindow ( ); 
	}
}
//////////////新建群组
BOOL CGRPSDWnd::OnGRPSDNewBtn ( UINT32 nMwssage,UINT32 wParam,UINT32 lParam )
{
    char GroupName[11];
	if( GroupNumber == MAX_GROUP_NUMBER)	{
		MessageBox("群组存储空间已满 !","新建群组",MB_OK);
		return FALSE;
	} 
	GroupName[0] = '\0';
	if(::InputBox("输入群组名称", GroupName , 10)== IDOK)
	{
	     if(GroupName[0]== '\0') {
			MessageBox("群组名称不能为空!" , "新建群组",MB_OK|MB_ICONSTOP);
			return FALSE;
		 }	
		 ////////////
         for( short i = 0; i< GroupNumber; i++){
			  
			  if( strcmp(GroupName , g_pGroup[i].szName)==0){
				 MessageBox("群组不能重名!" , "新建群组",MB_OK|MB_ICONSTOP);
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
///////////////////某群组被按下
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

	  g_pGroupSend = g_pGroup[iItem]; ///// 把相应内容给宋英剑
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
/////////    class CGRPSDWnd  01_群发     实现函数  end   wangyu ////

/////////    class CLeaderWnd  01_群发_修改     实现函数  start   wangyu ////
BEGIN_MESSAGE_MAP(CGroup_ModWnd, CAppWnd) 
    ON_MESSAGE ( GRPSDModify_LISTVIEW,CN_COMMAND,OnModifyGroup )  
    ON_MESSAGE ( GRPSD_TaxisUpButton,CN_COMMAND,OnTaxisGroupUp )
	ON_MESSAGE ( GRPSD_TaxisDwButton,CN_COMMAND,OnTaxisGroupDw )
END_MESSAGE_MAP 
////////  控制 群组修改 的选项
UINT32 CGroup_ModWnd::OnModifyGroup(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    switch(GrpSDModifyFlag)	{
	    case 0:  // 改名	         
		      OnRenameGroup(0,0,lParam);
		      break;	
	    case 1:  // 删除		   
		      OnDeleteGroup(0,0,lParam);
		      break;	     	
	    default : 
		        break;	 
	}
}
//////////////////  删除群组  
UINT32 CGroup_ModWnd::OnDeleteGroup(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{
    UINT32 uItem =  lParam; 	 
	
	if(MessageBox("确定删除此群组吗?","删除群组",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		int i;

		for( i=uItem; i<GroupNumber-1 ; i++)	 
			 g_pGroup[i]=g_pGroup[i+1] ;  
		memset ( &g_pGroup[GroupNumber-1],0,sizeof ( GROUP ) );

		GroupNumber--; // 群组个数减1 
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
//////////////////  群组改名 
UINT32 CGroup_ModWnd::OnRenameGroup(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{   
	char SetTemp[10];    
	UINT32 uItem =lParam;  
	
	strcpy ( SetTemp, g_pGroup[uItem].szName );
	if (InputBox ( "请输入内容",SetTemp,10 ) == IDOK )	{		 
        
		/*for( short i = 0; i< GroupNumber; i++){			  
		     if( strcmp(SetTemp , g_pGroup[i].szName)==0){
				 MessageBox("群组不能重名!" , "新建群组",MB_OK|MB_ICONSTOP);
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

//////////////////  群组上移 
UINT32 CGroup_ModWnd::OnTaxisGroupUp(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{ 
	  GROUP *temp;   
	temp = new GROUP;
    INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pGrpModifyListView.GetSelItem();   
 
	if(uItem == -1 )	{ 
	    MessageBox("请先选中要操作的群组!","提示",MB_OK);
	    return FALSE;
	}
    if(uItem==0) {	
		MessageBox("所选中的群组已经是第一项!","提示",MB_OK);
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
///////////////////// 下移群组  
UINT32 CGroup_ModWnd::OnTaxisGroupDw(UINT32 nMessage,UINT32 nSrcid,UINT32 lParam)
{   GROUP *temp;    
	temp = new GROUP;
    INT32   nItemStart, nScreenRows;
	CListItem list1,list2 ;
	INT32 uItem = m_pGrpModifyListView.GetSelItem();   
 
	if(uItem == -1 )	{
		MessageBox("请先选中要操作的常用语!","提示",MB_OK);
	    return FALSE;
	}
    if(uItem==(GroupNumber-1)) {    ////最后一项	
		MessageBox("所选中的常用语已是最后一项!","提示",MB_OK);
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
/////////// 移动OK
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
	case 0:  /////改名	
	case 1:  /////删除
	       m_pGrpModifyListView.Create ( CRect ( m_sRect.left , m_sRect.top + WND_TITLE_HEIGHT,159,159 ), 
			                              this,  GRPSDModify_LISTVIEW );
	       break;		    
	case 2:  /////排序
	       m_pGrpModifyListView.Create ( CRect ( m_sRect.left , m_sRect.top + WND_TITLE_HEIGHT, 159,159 ),
								          this, LS_SMALLICON, GRPSDModify_LISTVIEW );
	       m_pGSUpButton.Create ( "上移",CRect( m_sRect.left + 58, 0, m_sRect.left + 91,  SMS_APP_TITLE_HEIGHT ),
		   						  this,GRPSD_TaxisUpButton );  
		   m_pGSDwButton.Create ( "下移",CRect( m_sRect.left + 92, 0, m_sRect.left + 125, SMS_APP_TITLE_HEIGHT ),
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
 
/////////    class CLeaderWnd  01_群发_修改     实现函数  start   wangyu ////


/////////    class CLeaderWnd  01_群发_领导     实现函数  start   wangyu ////
BEGIN_MESSAGE_MAP(CLeaderWnd, CAppWnd) 
 	ON_MESSAGE ( MODIFYLEADER_EDIT,CN_GRIDSEL,OnModifyLeader )
	ON_MESSAGE ( MODIFYLEADER_SEND,CN_COMMAND,OnSend )
END_MESSAGE_MAP

BOOL CLeaderWnd:: OnCreate()
{
	if ( !CAppWnd::OnCreate ( ) ) 
		return FALSE;
	
	m_SendBtn.Create ( "发送",
		CRect ( m_sRect.left + 78,
		        m_sRect.top,
				m_sRect.left + 110,
				m_sRect.top + SMS_APP_TITLE_HEIGHT ),
				this,MODIFYLEADER_SEND );
	
	// 网格
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
      m_pGridView.SetUnitText(i, 2, g_pGroup[LEADERFLAG].Numbers[j].szName,0);//名	  
	  m_pGridView.SetUnitText(i, 1, 0,ICON_GROUPSEND_NameCard);//图标	  
	  m_pGridView.SetUnitText(i+1,2,g_pGroup[LEADERFLAG].Numbers[j].szPhone,0);//号码

	  j++;
	}

	return TRUE;
}
////直接发送 
void  CLeaderWnd::OnSend ( UINT32 nMessage,UINT32 nSrcid,UINT32 lParam )
{
	
	//////设置窗口的一些参数	  
	int len;
	///////////号码转化
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
		MessageBox ( "号码为空！","错误",MB_OK|MB_ICONSTOP );
		return;
	}

	if ( g_ST.szPhone[len-1] == '#' )
		g_ST.szPhone[len-1] = '\0';

	strcpy(g_ST.szMsg , "<点这里输入短信内容>" );


	if ( m_pModifyLetter == NULL )
		 m_pModifyLetter = new CModifyLetterWnd;
	m_pModifyLetter->SetFlag ( false ); 
	m_pModifyLetter->Create("写短消息", 
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
	{  //名片--- 与郝庆锋接口   
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
///////////// 从名片夹选择 添加姓名、号码
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
///////////// 手动添加姓名
BOOL CLeaderWnd::OnModifyName(short mNameNO)
{
	char SetTemp[32];

    
	strcpy ( SetTemp, g_pGroup[LEADERFLAG].Numbers[mNameNO].szName   );   
    if ( InputBox ( "请输入新名称",SetTemp,10 ) == IDOK ) {
	     strcpy ( g_pGroup[LEADERFLAG].Numbers[mNameNO].szName , 
			 SetTemp );   
	     Repaint();
	}	
    for(short i=0; i<GroupNumber; i++)
	g_pGroup[i].szGroupNumber = GroupNumber;
	SaveGroupSend(g_pGroup);
	return TRUE;
} 
////////// 手动添加号码
BOOL CLeaderWnd::OnModifyPhone(short mPhoneNO)
{
	char szTemp[16];

    
    strcpy ( szTemp ,g_pGroup[LEADERFLAG].Numbers[mPhoneNO].szPhone ); 
    if ( NumInputBox ( "请输入新号码",szTemp,16 ) == IDOK )	{
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
/////////    class CLeaderWnd  01_群发_领导     实现函数  end   wangyu ////

////wangyu1228
///////////////// class CDrawPicWnd   图形管理 实现函数   start wangyu  ////
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
	case 0x01:  // 清屏
	    DrawClear();
        break;
	
	case 0x02:  // 作图定位		
		x0 = g_pMenuBuffer[4];
		x0 = (x0 << 8) | g_pMenuBuffer[3];
		y0 = g_pMenuBuffer[5];
		
		SetOrigin(x0 , y0);
		break;
	
	case 0x03:  // 画点		 
		x0 = g_pMenuBuffer[4];
		x0 = (x0 << 8) | g_pMenuBuffer[3];
		y0 = g_pMenuBuffer[5];
        
		DrawStyle( BLACK, g_pMenuBuffer[3], 1, 0);
		DrawPoint(  x0 + iX0 , y0 + iY0);
		break;
	
	case 0x04:  // 画线
		x0 = g_pMenuBuffer[6];
		x0 = (x0 << 8) | g_pMenuBuffer[5];
		y0 = g_pMenuBuffer[7]; 

		x1 = g_pMenuBuffer[9];
		x1 = (x1 << 8) | g_pMenuBuffer[8];
		y1 = g_pMenuBuffer[10];
		 
        DrawStyle( BLACK, g_pMenuBuffer[3], 1, g_pMenuBuffer[4]);
		Drawline(x0, y0, x1,  y1,m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle);
        break;
	
	case 0x05:  // 画矩形
        x0 = g_pMenuBuffer[6];
		x0 = (x0 << 8) | g_pMenuBuffer[5];
		y0 = g_pMenuBuffer[7]; 

		x1 = g_pMenuBuffer[8] + x0;		 
		y1 = g_pMenuBuffer[9] + y0 ;
		 
        DrawStyle( BLACK, g_pMenuBuffer[3], 1, g_pMenuBuffer[4]);
		DrawRect(x0, y0, x1, y1);
		break;
	
	case 0x06:  // 画K图点
		x0 = g_pMenuBuffer[6];
		x0 = (x0 << 8) | g_pMenuBuffer[5];
		y0 = L3Buffer[7]; 

		x1 = g_pMenuBuffer[11] + x0;
		y1 = 159 - g_pMenuBuffer[10];
 
        DrawStyle( BLACK, g_pMenuBuffer[3], 1, g_pMenuBuffer[4]);
		DrawKPoint(x0, y0, x1, y1);
	    break;
	
	case 0x07:  // 显示模式图
		break;
	
	case 0x08:  // 画顺序点
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
	
	case 0x09:  // 画椭圆
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
	
	case 0x0a:  // 定义图形绘制宏命令
	    break;
	
	case 0x0b:  // 利用图形宏编号作图命令
		break;
	}	
	
} 
//// 属性复位
void CDrawPicWnd::DrawReset()
{   m_iCurX =0; 
    m_iCurY =0; 
	m_iBackColor = WHITE;
    m_iPenColor  = BLACK;
    m_iDrawMode  = DRAWMODE_REP;
    m_iPenWidth  = 1;
    m_iLineStyle = 0;
}
//// 设置背景色、 
void CDrawPicWnd::DrawStyle(UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle)
{
	CDC dc;
  m_iPenColor = iColor;//BLACK 0; GRAY  1; LGRAY 2; WHITE 3
  m_iPenWidth = iSize; // 1
  /// 设置模式
  if(iAttrib == 0x00)
     dc.SetROP2(DRAWMODE_REP);
  else if(iAttrib == 0x01 )
	  dc.SetROP2(DRAWMODE_AND);
  else if(iAttrib == 0x02 )
	  dc.SetROP2(DRAWMODE_OR);
  else if(iAttrib == 0x03 )
	  dc.SetROP2(DRAWMODE_XOR); 
  ///设置画线类型
  if(iLineStyle == 0xFF)
     dc.SetLineStyle(0);   //0 实线 
  else if(iLineStyle == 0x55)
     dc.SetLineStyle(1);  //其他 虚线 
  
} 
//// 设置坐标原点
void CDrawPicWnd:: SetOrigin(INT16 x0, INT16 y0) 
{
	 iX0 = x0;
	 iY0 = y0;
}
//// 清屏
void CDrawPicWnd:: DrawClear()
{
	CDC dc;
	UINT8 iColor = WHITE;
	dc.ClearDevice( iColor );
}
//// 画点
void CDrawPicWnd :: DrawPoint(INT16 x0, INT16 y0) 
{
	 CDC dc;
	 dc.SetPixel( x0 ,  y0 );
}
//// 画线
void CDrawPicWnd :: Drawline(INT16 x0, INT16 y0, INT16 x1, INT16 y1, 
                    UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle)
 {
    CDC dc;
	dc.MoveTo(x0 , y0);
	dc.LineTo(x1 , y1); 
}
//// 画矩形
void CDrawPicWnd :: DrawRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    CDC dc;
	dc.FrameRect(iLeft, iTop,  iRight, iBottom);
}
//// 画K图点	 
void CDrawPicWnd :: DrawKPoint(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    CDC dc;
	dc.FrameRect(iLeft, iTop,  iRight, iBottom);
}
//// 画椭圆
void CDrawPicWnd::DrawEllipse(INT16 x0, INT16 y0, INT16 x1, INT16 y1)
{
	CDC dc;
    dc.Ellipse(x0, y0, x1, y1);
}
//////// class CDrawPicWnd   图形管理  end    wangyu //////

/////////////短消息结束
