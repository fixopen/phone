/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             menu.h                                                         */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             菜单控件类头文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-01                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-07-25   增加接口EnableItem                                */
/*                          增加接口DisableItem                               */
/******************************************************************************/

#ifndef __E201_MENU_H_
#define __E201_MENU_H_
#include "hwfcl.h" 

#define MENU_ITEM_TEXT_LONG  17       //菜单字符长度
#define MENU_ITEM_HEIGHT     17       //每一菜单项的高度

#define MENU_LEFT_SPACE       1
#define MENU_MID_SPACE        1

#define MENU_BACK_COLOR      LGRAY    //菜单背景色
#define MENU_FORE_COLOR      BLACK    //菜单前景色 
#define MENU_ICON_WIDTH      16       //菜单图标大小 

///////////////////////////////////////////////////////////////////////////////
//  菜单项的风格
#define MS_NORMAL            0x00     //正常
#define MS_POPUP             0x01     //带有子菜单 
#define MS_DISABLE           0x02     //不响应消息

/*============================================================================*/
//  CMenuItem类定义                                                           //
/*============================================================================*/
class CMenuItem 
{ 
private:
	CMenuItem  * pNextItem;
    CRect        sRect;
public:
	UINT8        nStyle;
	char         strText[MENU_ITEM_TEXT_LONG];
	UINT32       nIconId;
	UINT32       nItemId;
    CMenu      * pPopupMenu;
	
public:
	void      PaintItem(CDC &dc);
	void      InvertItem();
	void      Active();
	void      Dactive();
};

/*============================================================================*/
//  CMenu类定义                                                               //
/*============================================================================*/
class CMenu : public CCtrl
{
protected:
	CMenuItem * m_pFirstItem;
    CMenuItem * m_pLastItem;
	CMenuItem * m_pCurItem;
	CMenu     * m_pParentMenu;
	ScreenBuf * m_pBackScreenBuf;
	

protected:
    void         DoPenDown(CPoint &pt);
	void         DoPenMove(CPoint &pt);
	void         DoPenUp();
	void         DoDactive(UINT32 wParam);
	CMenuItem *  GetPenItem(CPoint &pt);
	void         HideMenu();
public:
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);
    virtual void Destroy();
public:
	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         创建一个菜单控件
	BOOL Create(CFrameWnd * pParent);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         添加一个菜单项
	BOOL AppendItem(char *strText,  UINT32 nItemID, UINT32 nIconId , UINT8 nStyle, CMenu * pPopUpMenu=NULL);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         添加一个菜单项
	BOOL AppendItem(char *strText,  UINT32 nItemID = 0, UINT32 nIconId =0);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         在指定位置弹出菜单
	BOOL PopupMenu(INT16 x, INT16 y);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         修改某一指定菜单项的内容
	//         nItemID  指定菜单项的ID，即AppendItem时的第二个参数
	void SetItem(UINT32 nItemID, CMenuItem *pNewItem);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         获取一个菜单项
	//         nItemID  指定菜单项的ID，即AppendItem时的第二个参数
	void GetItem(UINT32 nItemID, CMenuItem *pItemBuf);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         修改某一指定菜单项的内容
	//         nItemNumber 指定菜单项的位置，从0开始
	void ModifyItem(UINT16 nItemNumber, CMenuItem *pNewItem);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         获取一个菜单项
	//         nItemNumber 指定菜单项的位置，从0开始
	void ObtainItem(UINT32 nItemNumber, CMenuItem *pItemBuf);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         充许或变灰一个菜单项下面两个函数功能一样
	//         一个是以菜单项位置为关键字一个是以菜单项的ID为关键字
	void EnableItem(UINT32 nItemNumber, BOOL bEnable  = TRUE);
	void DisableItem(UINT32 nItemID,    BOOL bDisable = TRUE);

	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         CHECK或UNCHECK一个菜单项
	void CheckItem(UINT32 nItemID, BOOL bChecked = TRUE);
    
	////////////////////////////////////////////////////////////////////////////////
	//描述:
	//         删除全部菜单项
	void DeleteAllItem();
};
#endif
////////////////////////////////////////////////////////////////////////////////
// end menu.h 