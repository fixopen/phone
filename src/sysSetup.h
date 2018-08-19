
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

class CSystemSetup;  //ϵͳ���������洰��

class CDateTimeSet;  //ʱ������
class CRecordSet;    //��������
class CRingSet;   //����
class CForbidDial; //����
class COutLine;  //����
//class CPswdSet;  //��������
class CAcceptCall; //ֻ�ӵ绰
class CIPPhone;   //IP����
//class CCallID;    //������ʾ
class CAutoShutdown;  //�Զ��ػ�
class CMemoryInfo;    //�ڴ�ʹ��
class CAgentInfo;     //��������Ϣ

//***********������ϵͳ���������洰�ڵ�����******************//
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

	CDateTimeSet *m_pDateTimeWnd; //ʱ������
	CRecordSet   *m_pRecordWnd;
//	CPswdSet     *m_pPswdWnd;     //����
	CRingSet     *m_pRingWnd;     //����
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
//**********������ʱ���������ô����������********************//
class CDateTimeSet : public CAppWnd
{
protected:
	virtual BOOL OnCreate();
	CIconButton m_oBtnTime , m_oBtnDate;
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnTimeInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnDateInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
//**********�����������趨�����������************************//
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
//**********�������������ô����������************************//
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
//**********�������������ô����������************************//
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

//**********�����ǽ������봰���������*************************//
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

//**********���������ߴ����������*************************//
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

//**********������ֻ�ӵ绰�����������*************************//
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

//**********������IP�绰�����������*************************//
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

//**********������������ʾ�����������*************************//
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
//**********�������Զ��ػ������������*************************//
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

//**********�������ڴ�ʹ�ô����������*************************//
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

//**********�����ǵ�����Ϣ�������������*************************//
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

//**********������������ʾ������Ϣ�����������*************************//

// ������ʾ������Ϣ�ṹ

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
