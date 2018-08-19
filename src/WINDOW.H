/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             window.h                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library            */
/*                                                                            */
/*  组    成                                                                  */
/*             CObject类声明及定义                                            */
/*             CWnd类声明                                                     */
/*             CCtrl类声明及定义                                              */
/*             CFrameWnd类声明及定义                                          */
/*             CButton类声明及定义                                            */
/*             CIconButton类声明及定义                                        */
/*             CRadioButton类声明及定义(郝庆丰)                               */
/*             CDialog类声明及定义                                            */
/*             CAppWnd类声明及定义                                            */
/*                                                                            */
/*  设计说明                                                                  */
/*             参见《E201软件整体设计》文档                                   */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-07-20  增加CDialog类声明及定义                            */
/*                                                                            */
/*             2001-07-25  增加CAppWnd类声明及定义                            */
/*                                                                            */
/*             2001-08-01  增加CFrameWnd窗口消息处理函数对WM_DACTIVE          */
/*                         和WM_ACITVE消息处理的函数OnActive和OnDactive       */
/*                                                                            */
/*             2001-08-08  增加CLineEdit类声明及定义                          */
/*                                                                            */
/*             2001-08-23  增加CCheckButton类声明及定义                       */
/*                         CIconButton增加修改图标接口                        */
/*                                                                            */
/*             2001-08-24  增加CLineButton类声明及定义                        */
/*                                                                            */
/*             2001-09-24  CFrameWnd类增加OnRtc虚函数接口                     */
/*                         修改CWnd的构造函数                                 */
/*                                                                            */
/*             2001-09-26  CAppWnd类增加m_nWndIcon成员                        */
/*                         CAppWnd类增加Create函数                            */
/*                         CAppWnd类增加GetWndIcon和SetWndIcon函数            */
/*                                                                            */
/*             2001-09-28  CWnd类增加Enable和Visible成员函数                  */
/*             2001-11-06  增加CRadioButton类声明及定义（郝庆丰）             */
/*             2001-11-14  增加CAppWnd::OnOk函数（郝庆丰）                    */
/******************************************************************************/

#ifndef __E201_WINDOW_H_
#define __E201_WINDOW_H_

#include "hwfcl.h"
#include "string.h"
#include "dc.h"

#define  IDYES     1
#define  IDNO      2
#define  IDOK      3
#define  IDCANCEL  4

#define WND_TYPE_FRAME   0
#define WND_TYPE_APP     1
#define WND_TYPE_DIALOG  2

#define    SS_NORMAL                             0
#define    SS_LOCKED                             1
#define    SS_UNLOCKING                          2

/*
*	标题尺寸定义
*/
#define		TITLE_MENU_WIDTH		TITLE_WIDTH		 // 菜单宽度
#define		TITLE_BUTTON_WIDTH		BTN_WIDTH		 // 快捷按钮宽度
#define		TITLE_BAR_HEIGHT		WND_TITLE_HEIGHT // 标题高度

/*============================================================================*/
//  CObject类定义                                                             //
/*============================================================================*/
class CObject
{
public:
	CObject(){}
	~CObject(){}
};
	
/*============================================================================*/
//  CWnd类定义                                                                //
/*============================================================================*/
class CWnd : public CObject
{
protected:
	CRect     m_sRect;                          //窗口位置及大小
	char      m_strName[WND_TITLE_LENGTH];      //窗口标题 
    UINT32    m_dwStyle;                        //窗口风格 
    CWnd   *  m_pParent;                        //父窗口指针
	CWnd   *  m_pChild;                         //第一个子窗口指针 
	CWnd   *  m_pNextWnd;                       //指向窗口链表中下一个窗口 
	CWnd   *  m_pPrevWnd;                       //指向窗口链表中上一个窗口 hqf

	///////////////////////////////////////////////////////////////////////////
	//友元函数  
	friend void AddToWindowList(CFrameWnd * pWnd);
    friend BOOL DeleteWindowList(CFrameWnd *pWnd);
	friend BOOL DeleteWindow(CFrameWnd *pWnd, BOOL bRedrawBackWnd);

protected:
	///////////////////////////////////////////////////////////////////////////
	//描述:
	//          绘窗口
	//说明:
	//          窗口收到WM_PAINT消息时调用此函数
	void        Paint();

public:

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           构造及析构函数
	CWnd();
	virtual ~CWnd();

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           纯虚函数接口//调用基类纯虚函数时使用子类同名函数PAN 
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam){ return;}
	virtual void OnPaint(CDC &dc){ return;}

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           消息接口
    void         SendMessage(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           设置及获得窗口标题
	void         SetWindowText(const char * strText);
	void         GetWindowText(char * pTextBuf, UINT16 nMaxCount) const;

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           设置及获得窗口属性
	UINT32       GetWindowStyle() const;
	void         SetWindowStyle(const UINT32 dwStyle);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           获得窗口大小及位置
    CRect        GetWindowRect() const;

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//           获得窗口链表中下一个窗口
	CWnd *       GetNextWnd() const;

    ///////////////////////////////////////////////////////////////////////////
	//描述:
	void         Enable(BOOL bEnable = TRUE)
	{
		if(bEnable)
           m_dwStyle &= ~WS_DISABLE;
		else
		   m_dwStyle |= WS_DISABLE;
	}

	void         Visible(BOOL bVisible = TRUE)
	{
		if(bVisible)
			m_dwStyle |= WS_VISIBLE;
		else
			m_dwStyle &= ~WS_VISIBLE;
	}
};

/*============================================================================*/
//  CCtrl类定义                                                               //
/*============================================================================*/
class CCtrl : public CWnd
{
protected:
	UINT32  m_nCtrlId;         //控件的ID
public:

	///////////////////////////////////////////////////////////////////////////
	//描述: 
    //         创建控件
	virtual BOOL Create(char *strName, const CRect &wndRect, CWnd * pParent, UINT32 dwStyle, UINT32 nCtrlId=0);

	///////////////////////////////////////////////////////////////////////////
	//描述: 
	//         销毁控件
	virtual void Destroy();

	///////////////////////////////////////////////////////////////////////////
	//描述: 
	//         消息处理
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam){} 

	///////////////////////////////////////////////////////////////////////////
	//描述: 
	//          向父窗口发送通知消息
	//参数: 
	//          nMessage     通知消息类型
	//          lParam       消息参数
	//说明:
	//          父窗口接到此通知消息时,本控件的ID被包含在消息的第一个
	//          参数wParam里
	void    SendNotifyMessage(UINT32 nMessage, UINT32 lParam);

};

/*============================================================================*/
//  CFrameWnd类定义                                                           //
/*============================================================================*/
class CFrameWnd : public CWnd
{
protected:
	UINT32       m_nWndType;
	UINT32       m_nHelpId;                   //窗口的帮助ID信息
	CCtrl      * m_pCurCtrl;                  //当前Active的控件

protected:
	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         处理控件发送的通知消息
	//参数:
	//         nMessage         消息类型
	//         wParam           消息的第一个参数,为发送此消息的控件ID
	//         lParam           消息参数
	void         ProcessMessageMap(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

    ///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         绘标题栏
	virtual void       PaintTitle(CDC &dc);

    ///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         绘控制按钮
	void       PaintControlBox(CDC &dc);

    ///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         绘所有的控件
	void         PaintChildWnd();

    ///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         处理笔按下消息
    void         DoPenDown(CPoint &pt);

    ///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         处理笔移动消息
	void         DoPenMove(CPoint &pt);

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         处理笔抬起消息
	void         DoPenUp();

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         获得pt表示的点所在的控件
	CCtrl      * GetPenCtrl(CPoint &pt);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         依次销毁所有的子窗口
	void         DestroyChildWnd();
public:
	CFrameWnd();
	~CFrameWnd(); 

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         判断当前窗口是否在窗口链表中
	BOOL    IsLegalWnd();
	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         消息处理函数
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         创建窗口
	virtual BOOL Create(char *strName, UINT32 dwStyle, const CRect &wndRect, UINT32 nHelpId);
    
	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         派生类的创建接口
	virtual BOOL OnCreate(){return  TRUE;}

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         窗口销毁
	virtual void Destroy();

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         派生类的销毁接口
	virtual BOOL OnDestroy();

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         窗口重绘接口
	virtual void OnPaint(CDC &dc);

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         窗口获得焦点事件
	virtual void OnActive(){}

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         RTC消息的处理接口 nRtcType参见datetime.h
	virtual void OnRtc(UINT16 nRtcType){}

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         窗口失去焦点事件
	virtual void OnDactive();

	///////////////////////////////////////////////////////////////////////////
	//描述:  
	//         显示或隐藏窗口
	virtual void ShowWindow(UINT32 nCmdShow = SW_SHOW);
	
public:    
	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         在子窗口链表中增加一个成员
	void         AddChild(CCtrl *pChild);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         在子窗口链表中删除pChild指向的成员
	BOOL         DeleteChild(CCtrl *pChild);
    UINT32      GetWndType()
	{
		return m_nWndType;
	}
	UINT32      GetHelpID()
	{
		return m_nHelpId;
	}

public:
	///////////////////////////////////////////////////////////////////////////
	// 消息映射表声明
	DECLARE_MESSAGE_MAP(CFrameWnd)
};


/*============================================================================*/
//  CButton类定义                                                             //
/*============================================================================*/
class CButton : public CCtrl
{
protected:
	BOOL    m_bPressed;            //记录按钮当前是否被按下状态
public:
	BOOL      Create(char *strCaption, CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId=0);
	
    ///////////////////////////////////////////////////////////////////////////
	//描述:
	//         按钮消息处理函数
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         按钮重绘函数
	virtual void OnPaint(CDC &dc);
};

/*============================================================================*/
//  CIconButton类定义                                                         //
/*============================================================================*/
class CIconButton : public CButton
{
protected:
	UINT32    m_nIconId;            //记录图标ID
public:

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         创建图标按钮
	BOOL      Create(char *strCaption, UINT32 nIconId, CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId=0);

	///////////////////////////////////////////////////////////////////////////
	//描述:
	//         按钮重绘函数
	virtual void OnPaint(CDC &dc);
     
	void     SetIcon(UINT32 nIconId);
};

/*============================================================================*/
//  CLineEdit类定义                                                           //
/*============================================================================*/
class CLineEdit : public CCtrl
{
protected:
	char * m_pEditBuf;
	INT32  m_nMaxCount;
	INT32  m_nCharCount;
	INT32  m_nDispChars;
	INT32  m_nCursorPos;

public:
	BOOL Create(char * strInitText, INT32 nMaxCount, CRect &ctrRect, CFrameWnd *pParent, UINT32 nCtrlId=0);
    virtual void OnPaint(CDC &dc);
	void    ShowCursor();
	virtual void Destroy();
	virtual void OnActive();
	virtual void OnDactive();
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);

public:
    BOOL AppendChar(char ch);
    void BackSpace();
	void Clear();
	void GetInputText(char * pBuf, INT32 nBufLen);
};
/*============================================================================*/
//  CCheckButton类定义                                                        //
/*============================================================================*/
class CCheckButton : public CButton
{
protected:
    BOOL   m_bChecked;
public:
	virtual void OnPaint(CDC &dc);
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	BOOL Create(char *strText, CRect &ctrRect, CFrameWnd *pParent, BOOL bInitValue=FALSE, UINT32 nCtrlId=0);
public:
	BOOL GetValue();
	void SetCheck(BOOL bChecked = TRUE);
};

/*============================================================================*/
//  CRadioButton类定义                                                        //
/*============================================================================*/

#define    HORIZONTAL             0 //水平排列RadioButton
#define    VIRTICAL               1 //垂直排列RadioButton
#define    MAX_ITEM_LENGTH        32 //RadioButton文本的最大长度
#define    MAX_HOR_ITEMS          3
#define    RADITEM_HEIGHT        18
//#define    RADIO_ITEM_HEIGHT      16

class CRadioButton;
class CRadioItem 
{
public:
	virtual void SetSel(BOOL bSel = TRUE);
	virtual void Paint(CDC &dc);
	CRect m_itemRect;
	char  m_itemName[MAX_ITEM_LENGTH +1];
protected:
	friend class CRadioButton;
	void PaintRadio(CDC &dc,BOOL bIsSel);
	void PaintText(CDC &dc);
	void Reset();
	BOOL m_bIsSel;
	CRadioItem *m_pNextItem;
};

class CRadioButton :public CCtrl
{
public:
	CRadioButton();
	~CRadioButton();
	virtual BOOL Create(CRect &wndRect , UINT32 dwStyle , CWnd *pParent, UINT32 nCtrlID=0);
    virtual void OnPaint(CDC &dc);  
	BOOL AddButton(char *itemName);
	void SetSel(INT16 nItem, BOOL bRedraw = FALSE);
	BOOL SetButtonText(INT16 nItem , char *strNewText , BOOL bRedraw = FALSE);
	INT16 GetSel()
	{
		return m_nSelItem;
	}
	void SetAlign(INT16 nAlign = HORIZONTAL){ m_nAlign = nAlign;}
	virtual void WindowProcess(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	virtual void Destroy();
	CRadioItem *GetItem(INT16 nItem);
protected:
	virtual void DoPenDown(CPoint &pt);
	virtual void PaintItem(CRect r, CRadioItem *pItem);
	INT16 m_nItems;
	INT16 m_nSelItem;
	INT16 m_nAlign;
	CRadioItem *m_pFirstItem , *m_pLastItem;
};

/*============================================================================*/
//  CMenuButton类定义                                                         //
/*============================================================================*/
class CMenuButton : public CButton
{
public:
	virtual void OnPaint(CDC &dc);
};
/*============================================================================*/
//  CLineButton类定义                                                         //
/*============================================================================*/
class CLineButton : public CButton
{
public :
	virtual void OnPaint(CDC &dc);
};
/*============================================================================*/
//  CPages类定义                                                         //
/*============================================================================*/
class CPages : public CButton
{
private:
	BOOL         m_bSelected;
public:
	virtual BOOL Create(char *strCaption, CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId=0);
	virtual void OnPaint(CDC &dc);
	void         SetSelected(BOOL bSel)
	{
	     m_bSelected = bSel;
	}
	BOOL        GetSelected()
	{
		return m_bSelected;
	}
};
/*============================================================================*/
//  CDialog类定义                                                             //
/*============================================================================*/
class CDialog : public CFrameWnd
{
protected:
    struct  ScreenBuf  * m_pBkScrBuf;
	CIconButton  m_butCancel;
    UINT32       m_bExit;
	UINT32       m_nReturnCode;
	BOOL         m_bPutScreen;
public:
	CDialog();
	~CDialog();
	virtual BOOL Create(char *strName , UINT32 dwStyle ,const CRect &wndRect ,
		            BOOL bPutScreen = TRUE, UINT32 nHelpId= 0);
	//added by hqf . bPutScreen 表示在窗口关闭后要否恢复以前保存的部分
    virtual BOOL OnCreate();
    virtual void Destroy();
	virtual void OnOk();
	virtual void OnCancel();
	virtual void OnYes();
	virtual void OnNo();
public:
	UINT32 DoModal();
	DECLARE_MESSAGE_MAP(CDialog)
};

/*============================================================================*/
//  CAppWnd类定义                                                             //
/*============================================================================*/
extern  BOOL ReturnToDeskTop(BOOL bPaintDesktop);
class CAppWnd: public CFrameWnd
{
protected:
   friend      BOOL ReturnToDeskTop(BOOL bPaintDesktop);
   UINT32      m_nWndIcon;
   BOOL        m_bForceToClose; // 强制关闭窗口,若此标志为TRUE，则在Destory()或 OnDestroy()中必须被关闭
   CIconButton m_obutCancel;
   CIconButton m_oBtnOk;
   UINT32	   m_nReturnCode;
protected:
   virtual void       PaintTitle(CDC &dc);
public: 
   CAppWnd(){m_nWndIcon;}
   BOOL  Create(char *strName, UINT32 dwStyle, const CRect &wndRect, UINT32 nHelpId=0, UINT32 nWndIcon=0);
   //创建时WS_OKBOX风格表示有"√"按钮
   virtual BOOL OnCreate();
   virtual void OnPaint(CDC &dc);
   virtual void OnCancel(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
   virtual void OnOk(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
   virtual void Destroy();
   DECLARE_MESSAGE_MAP(CAppWnd)
public: 
	UINT32      GetWndIcon(){return m_nWndIcon;}
	void        SetWndIcon(UINT32 nNewIcon){m_nWndIcon = nNewIcon;}
};
extern CFrameWnd * g_pCurWindow;
#endif 
//  end window.h
////////////////////////////////////////////////////////////////////////////////