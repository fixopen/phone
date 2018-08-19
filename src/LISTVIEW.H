/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             listview.h                                                     */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             列表控件类头文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-01                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-07-22    增加接口函数ModifyItem                           */
/*                           增加接口函数DeleteItem                           */
/*                           增加接口函数DeleteAllItems                       */
/*                           修改AppendItems时内存的分配方案                  */
/*                           增加m_nMemoryItemCount成员                       */
/*                                                                            */
/*             2001-08-21    增加ListView选中风格                             */
/*                           增加设置选中和获取选中单元格接口                 */
/*                                                                            */
/*             2001-09-05    增加接口InsertItem接口                           */
/******************************************************************************/
  
#ifndef __E201_CLISTVIEW_H__
#define __E201_CLISTVIEW_H__
#include "hwfcl.h"
#include "scrollbar.h"

#define LS_LAGEICON           0
#define LS_SMALLICON          1

#define LISTITEM_TEXT_LENGTH   28

#define LISTCOL_WIDTH1          ((160-SB_WIDTH)/3)	// 列固定宽度
#define LISTROW_HEIGHT1         40  // ((LCD_HEIGHT-WND_TITLE_HEIGHT-WND_TOOLBAR_HEIGHT)/3)    //40
#define LISTROW_HEIGHT2         17

////////////////////////////////////////////////////////////////////////////////
//接口说明
//   Create            创建
//   AppendItem        增加
//   SetStatus         设置显示风格
//   GetItem           获取一个列表项的指针
//   ModifyItem        修改一个列表项的内容
//   DeleteItem        删除一个列表项
//   DeleteAllItems    清空列表

/*============================================================================*/
//  CListView的一个列表项结构定义                                             //
/*============================================================================*/
struct CListItem
{
	UINT32       nLageIcon;                         //大图标的ID
	UINT32       nSmallIcon;                        //对应的小图标的ID
	char         strText[LISTITEM_TEXT_LENGTH];     //显示的文字内容
	char         strExtText[LISTITEM_TEXT_LENGTH];  //小图标的显示的文字内容
	UINT32       nExtData;                          //存放数据
};

/*============================================================================*/
//  CListView类定义                                                           //
/*============================================================================*/
class CListView : public CCtrl
{
protected:
	INT32       m_nCurSelItem;       //当前笔选中的列表项
	CRect       m_sSelRect;          //当前反色的矩形
	INT32       m_nDispRow;          //当前显示的行号 
	INT32       m_nItems;            //当前列表项的总项数
	INT32       m_nCols;             //当前列表项的列数
	INT32       m_nScreenRows;       //屏幕显示行数
	UINT8       m_nStatus;           //当前风格，是大图标还是小图标
	INT32       m_nItemWidth;        //每个列表项的宽度
	INT32       m_nItemHeight;       //每一行的高度
	INT32       m_nMemoryItemCount;  //已分配的内存空间可以容纳列表项总数
	CScrollBar  m_oScrollBar;        //列表控件的滚动条
	CListItem  *m_pListItems;        //指向存放列表项的内存空间
    UINT8       m_nAlwaysSel;         //为1时笔抬起仍有选中，为0时笔抬起则没有选中
	INT16       m_iColsNum;           //列数 hqf
protected:
    void          DoPenDown(CPoint &pt);
	void          DoPenMove(CPoint &pt);
	void          DoPenUp();
	void          DoPenMoveOut();
	void          DoScrollTo(INT16 nDispRow);
	INT32         GetPenItem(CPoint &pt, CRect &sItemRect);
	void          ItemPaint(INT32 nItemNum, CRect &r, CDC &dc);
	void          PaintItems(CDC &dc);

public:
	CListView();
	~CListView();
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);
    virtual void Destroy();
	virtual void OnDactive();

public:
	/*============================================================================*/
    //  以下为接口函数                                                            //
    /*============================================================================*/

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         设置列数（必须在Create之前调用） 2或3
	BOOL       SetColumnNum(INT16 iNum);
	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         创建列表控件
	BOOL       Create(CRect &ctrRect, CFrameWnd *pParent, UINT32 nCtrlId=0);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         创建列表控件
	BOOL       Create(CRect &ctrRect, CFrameWnd *pParent, UINT8 nAlwaysSel, UINT32 nCtrlId);

    ////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         添加一个列表项
	BOOL       AppendItem(char *strText, UINT32 nLageIcon, char *strExtText=NULL, UINT32 nSmallIcon=0 , UINT32 nExtData =0);

	BOOL       InsertItem(INT32 nInsertPos, char *strText, UINT32 nLageIcon, char *strExtText=NULL, UINT32 nSmallIcon=0, UINT32 nExtData=0) ;
	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         设置列表控件的显示风格
	void       SetStatus(UINT8 nStatus = LS_LAGEICON, BOOL bRedraw = FALSE);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         获取指定的列表项
	BOOL       GetItem(CListItem *pItemBuf, INT32 nItemNumber);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         修改一个列表项
	BOOL       ModifyItem(CListItem *pNewItem, INT32 nItemNumber , BOOL bRedraw = TRUE);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         删除一个列表项
	BOOL       DeleteItem(INT32 nItemNumber);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         清空列表控件
	void       DeleteAllItems();

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         设置选中的列表项
	void       SetSelItem(INT32 nItemNumber);
   
	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         获取选中的列表项序号
	INT32      GetSelItem();

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         把列表滚动到指定列表项
	void       ScrollTo(INT32 nItemNumber, BOOL bRedraw = FALSE);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	INT32      GetScreenItems(){return m_nScreenRows;}
   
	////////////////////////////////////////////////////////////////////////////////
	//描述:
    INT32      GetStartItem(){return m_nDispRow;}

	CCtrl *    GetScrollBar(){return &m_oScrollBar;}
	////////////////////////////////////////////////////////////////////////////////
	//描述:
    void       SetSelAttrib(UINT8 nAlwaysSel = 0){m_nAlwaysSel = nAlwaysSel;}
};

#endif
////////////////////////////////////////////////////////////////////////////////
// end listview.h

