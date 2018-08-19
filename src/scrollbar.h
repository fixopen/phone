/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             scrollbar.h                                                    */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             滚动条控件类头文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-02                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#ifndef __E201_SCROLLBAR_H_
#define __E201_SCROLLBAR_H_
#include "hwfcl.h" 

////////////////////////////////////////////////////////////////////////////////
//
#define SB_WIDTH           16        //滚动条的宽度
#define SB_SLED_MINHEIGHT  14        //滑块的最小高度
#define MOVE_FILTER_NUM    5        // hqf 2002/1/7 笔移动时，每多少个点采一个
/*============================================================================*/
//  CScrollBar类的定义                                                        //
/*============================================================================*/
class CScrollBar : public CCtrl
{
protected:
    UINT8   m_iActive;              //上次选中的
    CCtrl * m_pBuddyCtrl;           //与滚动条相关的控件
    UINT16  m_iRows;                //总行数
	UINT16  m_iCurRow;              //当前显示的行号
    UINT16  m_iScreenRows;          //相关控件屏幕行数
    CRect   m_sUpRect;              //上箭头按钮区域
    CRect   m_sDownRect;            //下箭头按钮区域
    CRect   m_sSledRect;            //滑块区域
    UINT16  m_iLastPenY;            

////////////////////////////////////////////////////////////////////////////////
// 私有过程
protected:
	void DoPenDown(CPoint &pt);
    void DoPenMove(CPoint &pt);
    void DoPenUp();
	    
    void DoUpActive();
    void DoDownActive();
    void DoOtherUpActive();
    void DoOtherDownActive();
    void DoSledMove(UINT16 iPenY);

    void PaintUpDownBut(CDC &dc, CRect &scrRect, BOOL bUp=TRUE);
    void PaintSled(CDC &dc);

    void CalcSledPos();
    void NotifyBuddy();

////////////////////////////////////////////////////////////////////////////////
// 虚函数
public:
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);

////////////////////////////////////////////////////////////////////////////////
// 接口函数
// 滚动条使用方法说明
//     1.滚动条是一个控件，必须以某个从CFrameWnd派生的类为父窗口
//     2.滚动条创建后，自动完成滚动及显示过程
//     3.滚动条使用CM_SCROOLTO消息通知父窗口或滚动对象
// 使用举例
//     在某从CFrameWnd派生的窗口类中定义成员对象CScrollBar m_oSb;
//     在窗口类的虚函数OnCreate中创建滚动条
//     m_oSb.Create(ptLeftTop, iHeight, this, pBuddyCtrl, ID_SCRBAR);
//     设置相关参数
//     m_oSb.SetPerameter(100, 10, 0);
// 说明
//     滚动条创建后如果要在父窗口中处理滚动消息，在创建时必须把pBuddyCtrl参数指定为NULL
//     在父窗口的消息映射表中增加
//     ON_MESSAGE(id, CN_SCROLLTO, OnScroll)
//     并定义消息处理函数 void OnScroll(UINT32, UINT32, UINT32);
public:
    ////////////////////////////////////////////////////////////////////////////
	//描述
	//      创建滚动条
	//参数
	//       ptLeftTop             滚动条左上角坐标
	//       iHeight               滚动条的高度
	//       pParent               父窗口
	//       pBuddyCtrl            与滚动条相关的控件
	//       iScrollId             控件ID
	//返回
	//       创建是否成功
	//说明
	//       滚动条发送CN_SCROLLTO消息，如果pBuddyCtrl为空，则把此消息发送到父窗口
	//       否则发送到pBuddyCtrl中
    virtual BOOL Create(CPoint &ptLeftTop, UINT16 iHeight, CFrameWnd * pParent, CCtrl *pBuddyCtrl=NULL, UINT32 iScrollId=0);


	////////////////////////////////////////////////////////////////////////////
	//描述
	//       设置滚动条参数
	//参数
	//       iRows                滚动条所能滚动的总的行数
	//       iScreenRows          滚动条滚动的对象屏幕所能显示的行数
	//       iCurRow              滚动条滑块当前位置表示的行号
	//       bReDraw              设完参数后是否重绘滚动条
	//说明
	//       滚动条滚动的对象由这三个参数决定，滚动条滑块的大小及位置也由这三个参数和
	//       滚动条的高度确定，其中滑块的大小为 iScreenRows/iRows * 滚动条的客户区高度
	//       滑块的位置为 (滚动条客户区高度-滑块高度)*iCurRow/iRows
    void SetPerameter(UINT16 iRows, UINT16 iScreenRows, UINT16 iCurRow, BOOL bReDraw = FALSE);

	////////////////////////////////////////////////////////////////////////////
	//描述
	//       获得滚动格相关的参数
	void GetPerameter(UINT16 &iRows, UINT16 &iScreenRows, UINT16 &iCurRow);

	void Moveto(INT16 x, INT16 y);
};
#endif
////////////////////////////////////////////////////////////////////////////////
// end menu.h 