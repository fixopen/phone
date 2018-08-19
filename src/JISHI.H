/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             jishi.h                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII记事应用程序头文件                            */
/*                                                                            */
/*  组    成                                                                  */
/*             记事应用程序头文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-08-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/


#ifndef		_JISHI_H_
#define		_JISHI_H_

#include "listview.h"
#include "combox.h"
#include "messagebox.h"
#include "listview.h"
#include "edit.h"
#include "datadrv.h"
#include "menu.h"

//CMenuButton++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//CMenuButton--------------------------------------------------------

//CStatic+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静态文本控件
#define CS_DOTLINE	0X8000

class CStatic : public CEdit
{
private :
	WORD m_style;
	char m_strTitle[20];
public:
	BOOL         Create(char *strName, const CRect wndRect, 
						CWnd * pParent, UINT32 dwStyle, 
						UINT32 nCtrlId, INT32 uiBuffLen,char *strTitle=NULL);
	void  OnClientEvent(UINT32 nMessage, UINT32 x, UINT32 y);
	virtual void	OnPaint(CDC &dc);

};

//CStatic-----------------------------------------------------------


//CTextWnd++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//文本浏览窗口

//PUBLIC+
int		CompareStrings(const char * pStr1, const char * pSt2);
INT16	IsTitleExist(const char * pTitle, short * pNotIdBuf);
BOOL	SetDefaultTitle(MEMO * pMEMO, short * pNotIdBuf);
BOOL	SaveNote(MEMO * pMEMO, short * pNotIdBuf, 
				 short uiCurrentId, BOOL * bIsAlerted);
//PUBLIC-

class CTextWnd : public CAppWnd
{
public:
	DECLARE_MESSAGE_MAP(CAppWnd)
public:
	CMenu			m_oMenu;
	CMenuButton		m_buCtl;
	CIconButton     m_buCtl1;
	CStatic			m_oEdit,m_oTitle;
	BOOL            m_bIsDeleted;   //当前浏览的记事是否已被删除 hqf
	char            m_strPassWord[MEMO_PASSWORD_LEN +1]; //记录临时密码, hqf
public:
	BOOL			SaveText();
	virtual void	OnPaint(CDC &dc);
	virtual void    PaintTitle(CDC &dc); // hqf
	virtual void	OnOk(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	virtual BOOL	OnDestroy();
	
	virtual			BOOL OnCreate();

    //declare the message handle
	MESSAGE_HANDLE void  OnADVANCEClick(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnCode(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnSave(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnDelete(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnEditTitle(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnEdit(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnCancel(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
};
//CTextWnd-----------------------------------------------------------

//CItermWnd++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//主窗口类

typedef struct
{
	MEMO		 noteBuff;
	short		 noteIdBuff[MEMO_MAX_COUNT];
	short		 currentId;
	INT32		 listId;
	BOOL		 isAlerted;
}NOTEDOCSTRUCT;

class CItermWnd : public CAppWnd
{
public:
	DECLARE_MESSAGE_MAP(CAppWnd)
public:
	CTextWnd	 m_editwnd;
	CMenu		 m_oMenu;
	CMenuButton	 m_buCtl;
	CIconButton  m_buCtl1,m_buCtl2;
	CListView	 m_oGrid;
public:
    BOOL		 Create();                    
	virtual void OnPaint(CDC &dc);
	virtual		 BOOL OnCreate();
	virtual void Destroy();

    //消息处理函数
	MESSAGE_HANDLE void  OnADVANCEClick(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnNew(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnDelete(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
    MESSAGE_HANDLE void  OnGridSel(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
};

//CItermWnd----------------------------------------------------------


#endif
