/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             huaban.h                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII画板应用程序头文件                            */
/*                                                                            */
/*  组    成                                                                  */
/*             画板应用程序头文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-08-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/

#ifndef		_HUABAN_H_
#define		_HUABAN_H_

#include "messagebox.h"
#include "datadrv.h"
#include "scrollwnd.h"
#include "jishi.h"

//CTab+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/////////////////////////////////////////////////////////////////////
//                     tab控件使用说明
//
//  创建声明控件标签个数，不能为零，否则创建失败；
//
//  点击控件会产生一个COMMAND消息，LOWORD(lParam)是上次选中标签，
//  HIWORD(lParam)是当前选中标签；
//
//  用户可重载DrawSelected和DrawOther以获得不同的标签绘画效果。
//
/////////////////////////////////////////////////////////////////////

class CTab : public CCtrl
{
private:
	INT16		m_uiCount;
	INT16		m_uiSelected;
public:
	CTab()		
	{
		m_uiCount = 0;
		m_uiSelected = 0;
	};
public:
	BOOL			Create(CRect &rRect, CFrameWnd *pParent, 
							INT32 uiTabCount,
							UINT32 nCtrlId);
	virtual void	WindowProcess(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	
	virtual void	OnPaint(CDC &dc);
	virtual void	OnSelectedChanged(){};
	void			DrawOnes();
	virtual void	DrawSelected(UINT32 uiIndex);
	virtual void	DrawOther(UINT32 uiIndex);

	//取得选中
	virtual INT16	GetSelected()
	{
		return m_uiSelected;
	};
	//取得标签数
	virtual INT16	GetTabCount()
	{
		return m_uiCount;
	};

};

//CTab---------------------------------------------------------------

//CBanTab++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//选择画板标签类

class CBanTab : public CTab
{
public:
	virtual void DrawSelected(UINT32 uiIndex)
	{
		CRect re;
		char buff[10];
		CDC  dc;
		short cell = (m_sRect.Width()/GetTabCount());
		CTab::DrawSelected(uiIndex);
		sprintf(buff, "%d", uiIndex);
		dc.TextOut(cell*uiIndex+2, m_sRect.top+2, buff);
		int l;
		if(uiIndex == GetTabCount()-1)
			l = 1;
		else
			l = 0;
		re.left = (m_sRect.left+uiIndex*cell+2);
		re.right = (m_sRect.left+uiIndex*cell+cell-2-l);
		re.top = (m_sRect.top+2);
		re.bottom = (m_sRect.bottom-2);
		dc.InvertRect(re);
	}
	virtual void DrawOther(UINT32 uiIndex)
	{
		char buff[10];
		CDC  dc;
		short cell = (m_sRect.Width()/GetTabCount());
		CTab::DrawOther(uiIndex);
		sprintf(buff, "%d", uiIndex);
		dc.TextOut(cell*uiIndex+2, m_sRect.top+3, buff);
	}

};

//CBanTab------------------------------------------------------------

//CPenTab++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//选择笔模式类

class CPenTab : public CTab
{
public:
	virtual void DrawSelected(UINT32 uiIndex)
	{
		CDC dc;
		short cell = (m_sRect.Width()/GetTabCount());
		dc.EraseRect(uiIndex*cell, m_sRect.top, 
					 uiIndex*cell+cell, m_sRect.bottom);
		dc.FrameRect(uiIndex*cell, m_sRect.top, 
					 uiIndex*cell+cell, m_sRect.bottom);
		dc.PutIcon(uiIndex*cell+1, m_sRect.top+2, ICON_PEN+uiIndex);
		dc.InvertRect(uiIndex*cell+2, m_sRect.top+2, 
					 uiIndex*cell+cell-2, m_sRect.bottom-2);
	}
	virtual void DrawOther(UINT32 uiIndex)
	{
		CDC dc;
		short cell = (m_sRect.Width()/GetTabCount());
		dc.EraseRect(uiIndex*cell, m_sRect.top, 
					 uiIndex*cell+cell, m_sRect.bottom);
		dc.PutIcon(uiIndex*cell+1, m_sRect.top+2, ICON_PEN+uiIndex);
		dc.FrameRect(uiIndex*cell, m_sRect.top, 
					 uiIndex*cell+cell, m_sRect.bottom);
	}

};

//CPenTab--------------------------------------------------------------

//CPicture--------------------------------------------------------------
//画板控件类

#define		BOTTOM_WIDTH	16
#define		RUBER_WIDTH		19

#define		PEN_MODE		0
#define		RUBER_MODE		1
#define		LINE_MODE		2
#define		RECT_MODE		3
#define		CIRCLE_MODE		4

#define		PEN_WIDTH_NUM	3

#define		HUABAN_CUNT		10

class CPicture  : public CScrollWnd
{
private:
	UINT8	m_lineMode;
	UINT8	m_selectedMode;
	int		m_startX;
	int		m_startY;
	int		m_oldX;
	int		m_oldY;

public:
	BOOL	m_isAlerted;
	UINT8 * m_uiPicSrc;
	CPicture()
	{
		m_uiPicSrc = NULL;
		m_selectedMode = 0;
		m_lineMode = 1;
	};
public:
	BOOL			Create(const CRect wndRect, 
						CWnd * pParent, UINT32 dwStyle, 
						UINT32 nCtrlId);
	void			LoadPicture(UINT8 * pPicture);
	virtual void	OnPaint(CDC &dc);
	//下翻一行处理函数
	virtual void	ScrollDown(INT32 iSelect);				
	//上翻一行处理函数
	virtual void	ScrollUp(INT32 iSelect);				
	//客户区处理函数
	virtual void	OnClientEvent(UINT32 nMessage, 
									UINT32 x, UINT32 y);
	void			DoClientDown(UINT32 nMessage, 
									UINT32 x, UINT32 y);
	void			DoClientMove(UINT32 nMessage, 
									UINT32 x, UINT32 y);
	void			DoClientOther(UINT32 nMessage, 
									UINT32 x, UINT32 y);
public:
	//选择笔模式
	void			SetSeleMode(UINT32 uiMode)
	{
		m_selectedMode = uiMode%(CIRCLE_MODE+1);
	}
	//设置线模式
	void			SetLineMode(UINT32 uiMode)
	{
		m_lineMode = (uiMode%(PEN_WIDTH_NUM))+1;
	}
	//获得线模式
	UINT32			GetLineMode()
	{
		return m_lineMode ;
	}
	
};

//CPicture++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//CHuabanWnd++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//主窗口类

class CHuabanWnd : public CAppWnd
{
public:
	DECLARE_MESSAGE_MAP(CAppWnd)
public:
	CBanTab		 m_oTab1;
	CPenTab		 m_oTab2;
	CPicture	 m_oHuaban;
	CIconButton  m_oButton1;
	CButton		 m_oButton2;
public:
    BOOL		 Create();                    
	virtual void Destroy();
	virtual	BOOL OnCreate();

    //消息处理函数
	MESSAGE_HANDLE void  OnClickTab1(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnClickTab2(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnButton1(UINT32 nMessage, 
									  UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnButton2(UINT32 nMessage, 
									  UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnCancel(UINT32 nMessage, 
									UINT32 nSrcId, UINT32 lParam);
};

//CHuabanWnd----------------------------------------------------------


#endif