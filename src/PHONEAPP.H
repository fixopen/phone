/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             phoneapp.h                                                     */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII应用程序头文件                                */
/*                                                                            */
/*  组    成                                                                  */
/*             电话状态窗口类及相关控件                                       */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-08-23                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-11-25  郝庆丰添加CMDFastDial，CSortFastDial类             */         
/******************************************************************************/
#ifndef __E201_PHONEAPP_H_
#define __E201_PHONEAPP_H_

#include "hwfcl.h"
#include "combox.h"
#include "datadrv.h"
#include "menu.h"
#include "grid.h"
#define WM_NEWHISTORY  (WM_USER + 1)
#define WM_ONEHIS_DELETE    (WM_USER +2)
#define DIAL_DELAY_SECONDS    5        //硬件拨号后廷时DIAL_DELAY_SECONDS秒后进入通话界面

//#define FASTDIAL_MAX_COUNT		64				// 
/*#define FASTDIAL_NAME_LEN     10
#define FASTDIAL_NUMBER_LEN   40*/

#define FASTDIAL_MAX_BUTTONS        8        //速拨按钮的个数
#define CALL_RECORDLIST_COUNT 10        //下拉列表中通话记录的个数

////////////////////////////////////////////////////////////////////////////////
//描述
//       根据电话系统设置进行拨号，包括弹出规则选择对话框，选择拨号规则等内容
//       存入通话记录等内容
//参数
//       strDial         指向以'\0'结束的电话号码字符串
//       bOutLineHeader  前面是否加拨外线字头
//       bIPHeader       前面是否加拨IP帐号头（在系统设置中设定）
//返回
//       如下所示
#define      DIAL_SUCCESS                   0
#define      DIAL_INVALIDCHAR               -1
#define      DIAL_OPENMICFAIL               -2
#define      DIAL_NUMFORBIDDEN              -3
#define		 DIAL_INVALIDIP					-4

struct SOFTDIAL
{
	char	name[HISTORY_NAME_LEN + 1];
	char	number[HISTORY_NUMBER_LEN + 1];
	char	areacode[HISTORY_AREACODE_LEN + 1];
	short	type;
	short	ip;
};

//INT16 AppDialOut(char *strDial, char *strName, short nNumberType = CARD_NONE, short nIPHeader = -1);
INT16 AppDialOut(SOFTDIAL &sdToDial);

//拨号规则
#define DIALFLAG_DIALOUT    0xFFFF    //直接拨号
#define DIALFLAG_USERSEL    0xFFFE    //由用户选择拨号规则
#define FASTDIAL_MAX_PAGES  8
//#define DIALRULE_COUNT      8         //拨号规则条数


/*===========================================================================*/
//  电话状态窗口中用到的控件                                                 //
/*===========================================================================*/
class CPropertiyPage : public CCtrl
{
protected:
	char    m_strText[2][10];
    UINT8   m_nCurSel;
	CRect   m_sLeftRect, m_sRightRect;
protected:
	void   DoPenDown(CPoint &pt);
public:
	BOOL   Create(char *str1, char *str2, UINT8 nSel, CRect &ctrRect, CFrameWnd *pParent, UINT32 nCtrlId);
	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         消息处理函数
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         重绘函数
	virtual void OnPaint(CDC &dc);
};
/*===========================================================================*/
//  速拨设置窗口类                                                           //
/*===========================================================================*/
class CFastDialSet : public CDialog
{
protected:
	CIconButton     m_oButOk;
	CLineButton     m_oName;
	CLineButton     m_oNumber;
	CCombox         m_oTypeSel;
	CButton         m_oSelCard;
	FASTDIAL  m_sData;
public:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnOk();
	virtual void OnPaint(CDC &dc);

public:
    DECLARE_MESSAGE_MAP(CFastDialSet)		// ?? bbj
public:
	void GetInput(FASTDIAL * pUnitBuf);
	void SetInitData(FASTDIAL * pUnit);
	MESSAGE_HANDLE void OnNameClick(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
    MESSAGE_HANDLE void OnNumberClick(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnTypeSel(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnSelCard(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};

/*===========================================================================*/
//  电话状态窗口类                                                           //
/*===========================================================================*/
#define PHONE_FACE_FASTDIAL   0
#define PHONE_FACE_DIAL       1
#define PHONE_FACE_TALK       2
#define PHONE_FACE_RING       3

class CMDFastDial;
class CSortFastDial;
class CPhoneWnd : public CFrameWnd
{
protected:
    ///////////////////////////////////////////////////////////////////////////
	//三种界面下的公用成员变量
	UINT8               m_nUserFace;                       //当前界面状态 0 速拨 1拨号 2通话 3振铃
	ScreenBuf         * m_pBackScrBuf;                     //保存屏幕背景的指针
    HISTORY             m_sCurHistory;                     //当前的通话记录
	INT32               m_nTalkStart;                      //通话记时的开始时间
//	INT16               m_iCurPage;                        //当前页
	BOOL                m_bIsDialing;                    //能否清除当前的历史记录的号码

	///////////////////////////////////////////////////////////////////////////
	// 速拨界面中的成员变量
	CCombox             m_oDialStrDisp;
	CIconButton			m_oBtnDial;
//	CPropertiyPage      m_oPageSwatch;                       //常用，速拨切换按钮
	CIconButton         m_oBtnPrevPage;                      //上页
	CIconButton         m_oBtnNextPage;                      //下页
	CButton             m_oBtnPageTip;                       //当前页号
	CIconButton         m_oBtnDialSet;                       //速拨设置
	CMenu               m_oMenuSet;                          //速拨菜单
	CPages              m_oPageIP;
	UINT16              m_curPage;                           //速拨界面页号当前是常用还是速拨0
	INT16               m_iTotalPages;                     //最大页数
	CIconButton         m_obutFastDialUnit[FASTDIAL_MAX_BUTTONS]; //8个速拨按钮 
//	CCheckButton        m_obutSetup;                         //设置选框  
    
	BOOL                m_bIfCloseRtc;                       //挂机时是否关闭RTC

	BOOL                m_bPenDownInWnd;
    BOOL                m_bMult;
    BOOL                m_bTalkStartEnable;

	BOOL                m_bNewMsgInScrProt;
	BOOL                m_bIsSoftDial;     //电话是否正在拨号，1表示正在拨号，0相反
    CMDFastDial        *m_pMDWnd;     
//	CFrameWnd          *m_pAreaTipWnd;
	BOOL                m_bInAreaTip;
	BOOL                m_bInAddTip;
	BOOL                m_bInMDTip;
	BOOL                m_bInSortTip;
	CARD				*m_pCard;		// 查名片结果
	CARDCASE			*m_pCase;		// 查名片结果

	short				m_nNewHistoryBuf[CALL_RECORDLIST_COUNT];
	int					m_nNewHistoryCount;
protected:
	void DoPickUp();
	void DoPickDown();
	void DoRing(char *strCallId);
	void DoRingEnd();
	void DoDial(char num);
	void DoSoftDial(SOFTDIAL *todial);
	//void DoReDial();		// bbj
	void DoMuteStart();
	void DoMuteEnd();
	void DoFlash();
	void DoPenDown(CPoint &pt);
	void DoPenUp();
	void DoPenMove(CPoint &pt);
	void DoKey(UINT16 nKeyValue);
	void PlayAni(BOOL nShow = TRUE);
	BOOL SetFastDialButData();
    void SetCallRecordList();
	void ChangeUserFace(UINT8 nNewFace);
	void PaintRingFace(CDC &dc); 
	void PaintDialFace(CDC &dc);
	void PaintTalkFace(CDC &dc);
	void PaintIconName(CDC &dc, BOOL bPaintDetail = TRUE);
public:
	virtual void ShowWindow(UINT32 nCmdShow = SW_SHOW);
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual BOOL Create();
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnPaint(CDC &dc);
	virtual void OnRtc(UINT16 nRtcType);
    void         TalkStart();
	//void         GetCurrentNumber(HISTORY &histroy);
	INT16        GetUserFace(){return m_nUserFace;}
public:
	DECLARE_MESSAGE_MAP(CPhoneWnd)

public:
	MESSAGE_HANDLE void OnIPDialClick(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnPageChange(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnFastDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnDialSet(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnAddDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnDelDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnModifyDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnSortDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnComboxDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnComboxSel(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};


#define         WF_MODIFY                   0
#define         WF_DELETE                   1
class CMDFastDial : public CAppWnd
{
public:
	virtual BOOL Create(char *strName , UINT32 dwStyle , CRect &wndRect, UINT8 wndFlag =0 , UINT32 nHelpID=0);
protected:
	UINT8   m_cWndFlag;
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	CGrid   m_oGridData;
//	CButton m_oBtnModDel;
	short	m_idbuf[FASTDIAL_MAX_COUNT];
	int		m_count;
//	BOOL    m_bModified;
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnModDelDial(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};

class CSortFastDial : public CDialog
{
protected:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnOk();
	
	short	m_idbuf[FASTDIAL_MAX_COUNT];
	int		m_count;
	CButton m_oBtnPrev, m_oBtnNext;
	CIconButton	m_oButOk;
	CGrid   m_oGridData;
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnMoveItem(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};

void DrawNumString(INT16 x, INT16 y, char *strNum);
void DrawSecondString(INT16 x, INT16 y, char *strSec);

//////////////////////////////////////////////////////////
//函数说明：得到当前电话窗口的状态
/*返回值：-1 电话窗口未创建
          0 速拨 1拨号 2通话 3振铃
 */        
INT16 GetPhoneWndStatus();

/*
*	分析来电号码
*/
CARDFIELD AnalyseCallID(char * pSourceNumber, char *pAreaCode, char *pNumber);


////////////////////////////////////////////////////////////////////////////////
//end phoneapp.h

#endif