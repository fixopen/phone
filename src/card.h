#ifndef _CARD_H_
#define _CARD_H_

#include "hwfcl.h"
#include "menu.h"
#include "listview.h"
#include "edit.h"
#include "softkey.h"
#include "grid.h"
#include "datadrv.h"
#include "pendrv.h"
#include "record.h"

#define  MAX_CARD_FOLDERS        CARDCASE_MAX_COUNT
#define  MAX_CARDS               CARD_MAX_COUNT 

#define  WM_CARD_DELETE          WM_USER + 20

/*
*	名片浏览第一列宽度
*/
#define		CARD_BROWSE_WIDTH		44

//名片其他字段暂定为手机、办公电话1、办公电话2、内线、宅电、传真、呼机、其他1、其他2、其他3、公司名称、职位、电子邮件、公司地址、备注。

//**********下面是名片窗口类的声明*************************//
class CCardFolderView; //名片夹列表
class CCardList;       //名片列表
class CCardEditWnd;    //名片编辑
class CCardDetailWnd;  //名片详细浏览
class CCardFind;       //名片查找
class CFindResult;     //查找结果列表
class CWizardDialog;   //向导窗口
class CCardCaseProperty; //名片夹属性
class CCardCaseSet;    //名片夹设定（删除）
class CCardSel;        //名片选择（复制、移动、删除）
class CSelTelWnd;      //电话类型选择窗口

short SelectCase();
class CCardList : public CAppWnd
{
protected:
    friend void CardProc(UNSIGNED , void *);
	CGrid   m_oCardGrid;
	CMenuButton m_oAdv;
	CMenu m_oAdvMenu, m_oMenuPopup1;// , m_oMenuPopup2;
	CButton m_oBtnNew,m_oBtnFind;// , m_oBtnAttr, m_oBtnHelp;
	CCardEditWnd *m_pEdit;
	CCardDetailWnd *m_pDetail;
	CFindResult *m_pResult;
	CProcessBar  m_oProgBar;

	short m_pCardBuf[MAX_CARDS];
	short m_nCards;
	short m_nCardCaseID;
	BOOL  m_bShowProg;

    virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnPaint(CDC &dc);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnAdvBtn(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnNewCard(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnFindCard(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnCopyCard(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnCutCard(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE BOOL OnDeleteAll(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
//	MESSAGE_HANDLE BOOL OnDeleteFolder(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
//	MESSAGE_HANDLE void OnSetAttrib(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnCardDetail(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
public:
	CCardList();
	~CCardList();
	void SetShowProg(BOOL bShowProg = FALSE){ m_bShowProg = bShowProg;}
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect, UINT32 nHelpId , short nCardFolderID);
	
};
  
//**********下面是名片夹列表窗口类的声明*********************//

class   CCardFolderView : public CAppWnd
{
public:
	CCardFolderView();
	~CCardFolderView();
protected:
	friend class CCardList;
	CButton m_oBtnNew, m_oBtnFind;
	CMenuButton  m_oAdv;
	CMenu   m_oAdvMenu, m_oNewMenu;
	CListView m_oFolderList;
	CCardList *m_pCardList;
	BOOL    m_bFirst;

	CFindResult  *m_pResult;
	CCardEditWnd *m_pNewCard;

	BOOL	OnCreate();
	virtual void OnPaint(CDC &dc);
	virtual BOOL OnDestroy();
//	BOOL    DeleteFolder(short nFolderID , BOOL bRedraw= FALSE);
public:
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnAdvBtn(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnNew(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
    MESSAGE_HANDLE void OnNewCardFolder(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnNewCard(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnSortCardFolder(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnSetCardFolder(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnFindCardFolder(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnStatusCardFolder(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnFolderListClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect , UINT32 nHelpId = 0);
};

//下面是查找、排序窗口类的声明
#define        CARD_FIND_ALL              0//在所有名片夹中查找
#define        CARD_FIND_THIS             1//在当前名片夹或当前查找结果中查找
#define        CARD_SORT				  2//给名片夹排序
//#define        CARD_FIND_THIS             2//在当前查找结果中查找    

class CCardFind : public CDialog//查找、排序窗口类
{
public:
	CCardFind();
	~CCardFind();
	virtual BOOL Create(char *strName , UINT32 dwStyle , const CRect &wndRect,	
		UINT32 wndType = CARD_FIND_ALL , short *pCardIDBuf = NULL,
		short nCards = 0,UINT32 nHelpID = 0);
	virtual void OnPaint(CDC &dc);
	short  m_pCardIDBuf[MAX_CARDS];
	short  m_nFindNum;
protected:
//	CFindResult *m_pResult;
//	CPswdEdit m_oEdit;
	CButton m_oBtnPrev, m_oBtnNext;
	CIconButton m_oBtnFind;
//	CRadioItem m_oRadName , m_oRadAll;
//	short m_nCurSel;
    CListView  m_oFolderList;
	char m_strInfo[13];

//	char   m_pSelFlag[MAX_CARD_FOLDERS];
//	short  m_nTotalCards;
	short  m_nWndFlag;
	virtual BOOL OnCreate();
	short  FilterCards(short *pCardID , short nNum);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnEditClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnFind(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnName(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnListClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
};

class CFindResult : public CAppWnd
{
protected:
	friend void CardProc(UNSIGNED , void *);
	CButton m_oBtnFind;// , m_oBtnDelete;
	CMenuButton m_oAdv;
	CMenu m_oAdvMenu;
	CGrid m_oGridRet;
	CProcessBar  m_oProgBar;

	virtual BOOL OnCreate();
	virtual void OnPaint(CDC &dc);
	virtual BOOL OnDestroy();
	short *m_pCardBuf;
	short  m_nCards;
	CCardDetailWnd  *m_pDetail;
	
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnAdvBtn(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnFind(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnAllCopy(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnAllCut(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnAllDelete(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
public:
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect , short *pCardBuf , short num ,UINT32 nHelpID= 0);
	virtual void WindowProcess(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	CFindResult();
	~CFindResult();
};


//下面是名片详细浏览窗口类声明 
//名片其他字段暂定为手机、办公电话、宅电、传真、呼机、公司名称、职位、电子邮件、公司地址、备注。
class CCardDetailWnd : public CAppWnd
{
protected:
    CGrid m_oGridCard;
	CMenuButton m_oAdv;
	CMenu m_oAdvMenu ;//,m_menuPopup2;
	CButton m_oBtnEdit;
	CPages m_oPageIP;
	BOOL   m_bIPDial;
	short  m_nCardID;
	short  m_iListenRow;
	BOOL   m_bListening;

	CARD   *m_pCard;
	CCardEditWnd *m_pEdit;

	char   m_pField1[CARD_CONTENT_SEGMENT+2][9];//名片第一列的字段
	char   m_pFieldNotEmpty[CARD_CONTENT_SEGMENT+2];
    char   m_cFields;
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnPaint(CDC &dc);
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnAdvBtn(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnEdit(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnCopy(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnCut(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnDelete(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnIP(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
public:
	CCardDetailWnd();
	~CCardDetailWnd();
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect , 
		UINT32 nHelpId , short nCardID, CFrameWnd *pParentWnd = NULL);

};

//下面是名片编辑窗口类的声明
#define       WF_NEW_CARD               0
#define       WF_EDIT_CARD              1

class CCardEditWnd : public CAppWnd
{
protected:
	CGrid m_oGridCard;
	CMenuButton m_oAdv;
	CMenu m_oAdvMenu;
	CButton m_oBtnSave;
	CButton m_oBtnEmpty;
	CPopUpList m_oRing;
//	CIconButton m_oBtnOk;
	UINT32 m_nWndFlag;
	short  m_nCardID;
	short  m_nCardCaseID;
	CARD   *m_pCard;
    BOOL   m_bModified;
	char   m_pField1[CARD_CONTENT_SEGMENT+2][9];//名片第一列的字段
//	CButton  m_oBtnListen;

	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnCancel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	virtual void OnOk(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	DECLARE_MESSAGE_MAP()
    
	MESSAGE_HANDLE void OnClone(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnEmpty(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnAdvBtnClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridDropDown(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	
public:
	CCardEditWnd();
	~CCardEditWnd();
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect , 
			CARD *pCard ,short nCardCaseID,  UINT32 nHelpId = 0);
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	//新建名片时应调用此函数
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect , 
			short nCardID ,	UINT32 nHelpId = 0);
	//编辑名片时应调用此函数
	void    SetModifyFlag(BOOL bModified){m_bModified = bModified;}
	BOOL    SaveCard();
};

//下面开始名片夹、名片、电话选择的向导窗口类的声明
#define   CARDCASE_SEL             0 
#define   CARD_SEL                 1
#define   TEL_SEL                  2

#define   WIZARD_SELECT            0
#define   WIZARD_REPLACE           1
//#define   WIZARD_SELTELONLY        2
class CWizardDialog : public CDialog
{
protected:
	CGrid   m_oGrid;
	CListView m_oCaseList;
	CIconButton m_oBtnOk;
	UINT32  m_nEndType;
    UINT32  m_nCurType;
	UINT8   m_cWizardType;
	short   m_nTotalCards;
//    short   m_pCaseIDBuf[MAX_CARD_FOLDERS];
	short   m_pCardIDBuf[MAX_CARDS];
	short   m_pTelTypeID[CARD_CONTENT_SEGMENT];

 	virtual BOOL OnCreate();
	virtual void OnOk();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnGridClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
public:
	short   m_nSelCaseID;
	short   m_nSelCardID;
	char    m_strTel[PHONE_NUMBER_INPUT_LEN + 1];
	char    m_cTelType;
	void    SetTelNumber(char *strNumber);
	virtual BOOL Create(char *title , UINT32 dwStyle , const CRect &wndRect , 
		UINT32 nSelType = CARDCASE_SEL , UINT8 cWiardType= WIZARD_SELECT ,UINT32 nHelpID = 0);
};

class CCardCaseProperty : public CDialog
{
protected:
	CGrid m_oGridAttr;
	CIconButton m_oBtnOk;// , m_oBtnCancel;
	CIconButton m_oBtnPlayStop;
	CPopUpList m_oRing;
	short  m_nCaseID;
	char   m_cTelSel[2];
	CARDCASE *m_pCase;
	BOOL   m_bIsListening;
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnOk();
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnGridTextClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
//	MESSAGE_HANDLE void OnGridComboxClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
//	MESSAGE_HANDLE void OnListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnRingPlayStop(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	
public:
	virtual BOOL Create(UINT32 dwStyle , const CRect &wndRect , short nCardCaseID , UINT32 nHelpID =0);
};

////////下面是名片夹设定/删除窗口类的声明////////////////////////////////
#define  WF_CASESET             0
#define  WF_CASEDELETE          1
class CCardCaseSet : public CWizardDialog
{
public:
	virtual BOOL Create(char *strName , UINT32 dwStyle , const CRect &wndRect, 
		      UINT32 dwFlag = WF_CASESET,UINT32 nHelpId = 0);
protected:
//	CButton m_oBtnDelSet;
	UINT32  m_nWndFlag;
	virtual BOOL OnCreate();
//	virtual void OnOk();
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnGridClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	
};

////////下面是选择名片窗口类的声明//////////////////////////////////////////////
#define         CS_COPY          0
#define         CS_CUT           1
#define         CS_DELETE        2

class CCardSel: public CDialog
{
public:
	CCardSel();
	~CCardSel();
	virtual BOOL Create(char *strName , UINT32 dwStyle , const CRect &wndRect , 
				short *pCardID, short nCards, UINT32 nHelpId= 0 , UINT32 nSelType = CS_COPY);
	short *m_pCardID;
	short  m_nSelCards;
	UINT32 m_nSelType;
protected:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	CGrid  m_oGridCard;
	CMenuButton m_oAdvBtn;
	CMenu  m_oAdvMenu;
	CButton m_oBtnOk;
	CProcessBar  m_oProgBar;
	
	short m_nCards;
	char  *m_pSelFlag;
	virtual void OnOk();
	virtual void OnPaint(CDC &dc);
	MESSAGE_HANDLE void OnAdvBtn(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnSelectAll(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnClearAll(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	DECLARE_MESSAGE_MAP()
};

////////下面是选择电话类型窗口类的声明//////////////////////////////////////////////
class CSelTelWnd : public CDialog
{
protected:
	CCombox      m_oComTelType;
	CIconButton  m_oBtnOk;
	virtual   BOOL OnCreate();
public:
	CARDFIELD m_enTelType;
	virtual void OnOk();
	virtual void PaintTitle(CDC &dc);
	virtual void OnPaint(CDC &dc);
};

//下面是名片应用的入口函数
void CardAppRun();

//下面是选择电话的接口函数
short SelectTelNum(char *strTel , short nBufLen , CARDFIELD &nCardField);
//返回所选名片的ID号，为－1表示未选择。
//cTelType是电话类型，值如上宏定义

//下面是新建名片地接口函数
void NewCard(CARD *pCard);
//nCardCaseID 是名片所属名片夹的ID ,pCard是名片的初始数据*/

#endif