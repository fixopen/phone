
#ifndef		__E201_SETUP_H_
#define		__E201_SETUP_H_

#include "hwfcl.h"
#include "window.h"
//#include "options.h"
#include "combox.h"
#include "datadrv.h"
#include "messagebox.h"
#include "grid.h"
#include "record.h"
#include "jishi.h"
#include "phonedrv.h"

class CSystemSetup;  //系统设置主界面窗口

class CDateTimeSet;  //时间日期
class CRecordSet;    //留言设置
class CRingSet;   //振铃
class CForbidDial; //禁拨
class COutLine;  //外线
//class CPswdSet;  //屏保密码
class CAcceptCall; //只接电话
class CIPPhone;   //IP设置
//class CCallID;    //来电显示
class CAutoShutdown;  //自动关机
class CMemoryInfo;    //内存使用
class CAgentInfo;     //代理商信息

//***********下面是系统设置主界面窗口的声明******************//
class CSystemSetup : public CAppWnd
{
public:
	CSystemSetup();
	~CSystemSetup();
	static void  ResetSystemOption();

	void    SetSystemTime();
	void    SetSystemDate();
	void    SetRecord();
	void    SetDefaultRing();
	void    SetScreenPswd();
	void    SetForbidDial();
	void    SetOutLine();
	void    SetOnlyAccept();
	void    SetIPPhone();
	//void    SetCallIDMode();
	void    SetAutoShutTime();
	void    ShowMemInfo();
	void    ShowAgentInfo();
protected:
	friend class CRingSet;
	
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	
	CButton m_oBtnDate , m_oBtnTime;
	CButton m_oBtnRing , m_oBtnForbidDial;
	CButton m_oBtnOutLine , m_oBtnPswd;
	CButton m_oBtnNotBother , m_oBtnIP;
	CButton m_oBtnAgent , m_oBtnAutoShut;
	CButton m_oBtnMemory , m_oBtnAdjust;

	CDateTimeSet *m_pDateTimeWnd; //时间日期
	CRecordSet   *m_pRecordWnd;
//	CPswdSet     *m_pPswdWnd;     //密码
	CRingSet     *m_pRingWnd;     //振铃
	CForbidDial  *m_pForbidWnd;
	COutLine     *m_pOutLineWnd;
	CAcceptCall  *m_pOnlyAccept;
	CIPPhone     *m_pIPWnd;
	CAgentInfo   *m_pAgentInfo;
	//CCallID      *m_pCallId;
	CAutoShutdown *m_pAutoShut;
	CMemoryInfo   *m_pMemInfo;
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnBtnCmd(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
//**********下面是时间日期设置窗口类的声明********************//
class CDateTimeSet : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	CIconButton m_oBtnTime , m_oBtnDate;
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnTimeInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnDateInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
//**********下面是密码设定窗口类的声明************************//
/*class CPswdSet : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	virtual void OnPaint(CDC &dc);
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	CLineButton  m_oBtnPswd;
	char         m_strPswd[9];
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnPswdInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};*/
//**********下面是留言设置窗口类的声明************************//
class CRecordSet : public CAppWnd
{
protected:
	CRecordSet();
	~CRecordSet();
	virtual BOOL OnCreate();
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	CCombox      m_oRingCount;
	CButton      m_oBtnGreet;
	CCheckButton m_oBtnIsUse;
	CGreeting    *m_pGreetSet;

	virtual void OnPaint(CDC &dc);
	virtual BOOL OnDestroy();
	void    PaintOther(CDC &dc);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnRecordGreet(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnAutoRec(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
//**********下面是振铃设置窗口类的声明************************//
class CRingSet : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnPaint(CDC &dc);
	CButton		m_oBtnListenTest,m_oBtnDel;
//	CRadioButton  m_oRadVolume;
	CCombox		m_oComRing;
	BOOL        m_bIsListening;
//	CIconButton m_oBtnOk;
	BOOL OnDelRing();
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE  void OnListenTest(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE  void OnRingSel(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};

//**********下面是禁拨号码窗口类的声明*************************//
class CForbidDial : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	virtual void OnPaint(CDC &dc);
	CGrid   m_oGridNum;
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnGridClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};

//**********下面是外线窗口类的声明*************************//
class COutLine : public CAppWnd
{
protected:
	CCombox			m_oComHeader;
	//CRadioButton  m_oRadOutNum;
	char			m_strHeader[OUTLINE_HEAD_LEN + 1];
	char			m_strAreaCode[8];
    CLineButton     m_oBtnLocalAreaCode;
	virtual BOOL OnCreate();
	virtual void OnPaint(CDC &dc);
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnComSel(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnAreaCodeInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	
};

//**********下面是只接电话窗口类的声明*************************//
class CAcceptCall :public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	CGrid   m_oGridTel;
	BOOL    m_bChecked;
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnGridClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void   OnPaint(CDC &dc);
};

//**********下面是IP电话窗口类的声明*************************//
class CIPPhone : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	virtual void OnPaint(CDC &dc);
	void    LoadIPOption(INT16 iIndex);
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	CPages  m_oPageip[5];
	CButton m_oBtnAdv;
	CLineButton   m_oBtnInfo[3];
    CLineButton   m_oBtnDelay[4];
	CRadioButton  m_oRadDef;
	CMenu   m_oAdvMenu;
//	char    m_strDialRule[5][64];
	IP_INFO   m_ipInfo[DIALRULE_COUNT];
	INT16     m_nDefIP , m_nCurIP;
    DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE  void OnPagesClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE  void OnInfosClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE  void OnRadClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};

//**********下面是来电显示窗口类的声明*************************//
/*
class CCallID : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	CRadioButton   m_oRadMode;
public:
	virtual void OnPaint(CDC &dc);
};
*/
//**********下面是自动关机窗口类的声明*************************//
class CAutoShutdown : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
//	virtual void OnPaint(CDC &dc);
	void    PaintOther(CDC &dc);
	virtual void OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	CProcessBar    m_oTimeBar;
	CButton        m_oBtnAdd , m_oBtnDec;
	CCheckButton   m_oBtnUse;
	BOOL           m_bUsed;

	CLineButton  m_oBtnPswd;
	char         m_strPswd[9];
	MESSAGE_HANDLE void OnPswdInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnTimeAdd(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnTimeDec(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnUseSwitch(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnProcessMove(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
public:
	virtual void OnPaint(CDC &dc);
};

//**********下面是内存使用窗口类的声明*************************//
class CMemoryInfo : public CAppWnd
{
protected:
	CStatic m_oStaticInfo;
	virtual void OnPaint(CDC &dc);
	virtual BOOL OnCreate();
};

class CAgentInfo : public CAppWnd
{
protected:
	virtual void OnPaint(CDC &dc);
};

//**********下面是调试信息主窗口类的声明*************************//
class CCallIDInfo;
class CDebugMainWnd : public CAppWnd
{
protected:
	CButton m_oBtnCallID, m_oBtnBurn;
	CCallIDInfo  *m_pCallIDWnd;
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
public:
	virtual void OnPaint(CDC &dc);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnCallID(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnBurn(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};

//**********下面是来电显示调试信息窗口类的声明*************************//

// 来电显示调试信息结构

struct CallIDInfo
{
	DateTime dt;
	char	 strCallID[PHONE_CALLIDBUF_SEZE*3];
};

class CCallIDInfo : public CAppWnd
{
protected:
	CButton		m_oBtnSpecifyItem;
	CIconButton   m_oBtnPrevItem, m_oBtnNextItem;
	int     m_iItems, m_iCurItem;
	CStatic     m_oTxtCallID;
	virtual BOOL OnCreate();
public:
	virtual void OnPaint(CDC &dc);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnSpecifyItem(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnPrevItem(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnNextItem(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};

#endif
