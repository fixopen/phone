/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             combox.h                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             组合框控件类头文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-01                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-07-22  增加接口DeleteItem                                 */
/*                         增加接口DeleteAllItems                             */
/*                         修改AppendItem时的内存分配方案                     */
/*                         增加成员变量m_nMemoryItemCont                      */
/*                                                                            */
/*             2001-07-26  增加接口GetItem                                    */
/*                         增加接口SetItem                                    */
/*                                                                            */
/*             2001-08-27  增加接口SetCurSel                                  */
/*                         增加接口GetCurSel                                  */
/******************************************************************************/
#ifndef __E201_COMBOX_H__
#define __E201_COMBOX_H__

#include "hwfcl.h"
#include "scrollbar.h"

#define LIST_ITEM_LENGTH  17
#define LIST_ITEM_HEIGHT  18
#define COMBOX_HEIGHT 20
	
/*============================================================================*/
//  弹出列表每个列表项类定义                                                  //
/*============================================================================*/
class CPopListItem
{
public:
	UINT32 nIconId;
	char   strText[LIST_ITEM_LENGTH];
	UINT32 exData;
};
	
/*============================================================================*/
//  弹出列表类定义                                                            //
/*============================================================================*/
class CCombox;
class CPopUpList:public CCtrl
{
protected:
	friend class CCombox;
	CPopListItem * m_pListItems;
	CScrollBar     m_oScrollBar;
	BOOL           bSbUnCreated;
	ScreenBuf    * m_pScrBuf;
	CRect          m_sBkScrRect;
	CRect          m_sSelRect;
	CCtrl        * m_pBuddy;

    INT32          m_nItems;
	INT32          m_nMemoryItemCount;
	INT32          m_nScreenItems;
	INT32          m_nDispRow;
	INT32          m_nCurSelItem;
	

protected:
	INT32          GetPenItem(CPoint &pt, CRect &sItemRect);
	void           DoPenDown(CPoint &pt);
	void           DoPenUp();
	void           DoDactive(UINT32 wParam);

	void           PaintItems(CDC &dc);
    void           PaintItem(INT32 nItemNumber, CRect &r, CDC &dc);
public:
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);
    virtual void Destroy();
public:
	BOOL Create(CRect &ctrlRect, CFrameWnd *pParent, CCtrl *pBuddyCtrl= NULL, UINT32 nCtrlId=0);
    BOOL AppendItem(char *strText, UINT32 nIconId, UINT32 exData);
	BOOL PopUpList(INT16 x, INT16 y);
	void HideList();
	char *GetListItemText(INT32 nItemNumber);

	BOOL DeleteItem(INT32 nItemNumber);
	void DeleteAllItems();
	CPopListItem *GetItem(INT32 nItemNumber);
	BOOL SetItem(INT32 nItemNumber, CPopListItem * pItemBuf);
	void SetCurSel(INT32 nSelNumber);
};

	
/*============================================================================*/
//  组合框控件类定义                                                          //
/*============================================================================*/
class CCombox : public CCtrl
{
protected:
	CPopUpList  m_oPopList;
	BOOL        m_bHasDropDown;
	BOOL        m_bPressed;
	INT32       m_nCurSel;
public:
	void         SetWindowText(const char * strText, INT16 nAlign = HALIGN_LEFT, BOOL bReDraw = FALSE);
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);
public:
	////////////////////////////////////////////////////////////////////////////
	//接口函数
	BOOL Create(char *strText, CRect &ctrRect, INT16 nDropListHeight, CFrameWnd *pParent, UINT32 nCtrlId=0);
	BOOL AppendItem(char *strText, UINT32 nIconId, UINT32 exData =0);
	void ShowDropDown( BOOL bShowIt = TRUE);
	BOOL DeleteItem(INT32 nItemNumber);
	void DeleteAllItems();
	CPopListItem *GetItem(INT32 nItemNumber);
	BOOL SetItem(INT32 nItemNumber, CPopListItem * pItemBuf);
	void  SetCurSel(INT32 nItemNumber);
	INT32 GetCurSel();
	INT32 GetTotalItems(){return m_oPopList.m_nItems;}
};

#endif
////////////////////////////////////////////////////////////////////////////////
// end combox.h