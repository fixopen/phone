
/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             cwritepad.cpp                                                  */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             手写板控件类源文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-09-27   修改RecognnizePrc2和RecognnizePrc1                */
/*                          当控件不可见时不发手写消息                        */
/*             2001-11-1  王飞修改很多地方                                    */
/*                                                                            */
/*				byw, 2001/12/23                                               */
/*					修改：CWritePad::RecognnizePrc1()函数                     */
/*					修改：CWritePad::RecognnizePrc2()函数                     */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

#include "writepad.h"
#include "edit.h" 
#include "pendrv.h"
#include "datadrv.h"
//CWritePad++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/////////////////////////////////////////////////////////////////////
//功能    : 处理函数
//参数    : 消息类型,  参数一, 参数二
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void	CWritePad::WindowProcess(UINT32 nMessage, 
								 UINT32 wParam, UINT32 lParam)
{
	switch(nMessage)
	{
		case WM_TIMER:
			OnTimer((NU_TIMER *)wParam);
			break;
		case WM_PENDOWN:
		case WM_PENMOVEIN:
			DoMouseDown(HIWORD(wParam), LOWORD(wParam));
			break;
		case WM_PENMOVE:
			DoMouseMove(HIWORD(wParam), LOWORD(wParam));
			break;
		case CM_WRITEPAD_SHOWREC:
			{
			CDC dc;
			strcpy(m_szResult , (char *)lParam);
			if (m_dwStyle & WS_VISIBLE){
				if((char *)lParam == NULL||(lParam && *(char *)lParam =='\0'))
					dc.EraseRect(CRect(m_sRect.left+1, m_sRect.top+1, m_sRect.left +RECOG_SEL_COUNT * ASC_WIDTH ,m_sRect.top + CHAR_HEIGHT));
				else
					dc.TextOut(m_sRect.left+1, m_sRect.top+1, m_szResult, RECOG_SEL_COUNT);    
				break;
			}
			}
		default:
			DoPenOther(nMessage, wParam, lParam);
			break;
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 绘制函数
//参数    : CDC
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void	CWritePad::OnPaint(CDC &dc)
{
	INT16 width3 = m_sRect.Width()/2-1;

	dc.EraseRect(m_sRect.left, m_sRect.top, 
		m_sRect.right, m_sRect.bottom);
	dc.FrameRect(m_sRect.left, m_sRect.top, 
		m_sRect.right, m_sRect.bottom);
	dc.MoveTo(m_sRect.left, m_sRect.top+TOP_HEIGHT);
	dc.LineTo(m_sRect.right, m_sRect.top+TOP_HEIGHT);

	dc.MoveTo(m_sRect.left+width3, m_sRect.top+TOP_HEIGHT);
	dc.LineTo(m_sRect.left+width3, m_sRect.bottom);
    dc.TextOut(m_sRect.left+1, m_sRect.top+1, m_szResult, RECOG_SEL_COUNT);    
} 

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标点下处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::DoMouseDown(int x, int y)
{
	CDC		dc; 

	INT16 width3 = m_sRect.Width()/2-1;

	if(!(x<m_sRect.left || y<m_sRect.top+TOP_HEIGHT || 
			x>m_sRect.right-width3-2 || y>m_sRect.bottom))
	{
		if(m_isMouseUp2)
			OnTimer(&m_Timer2);

		if(m_oldisDoMouseDown1)
			return;

		if(m_currentPoint1 - m_pointBuff1 >= MAX_POINT_NUM - 2)// hqf 防止死机
			return;
//		NU_Terminate_Task(&m_RecognnizeTask1);
//		NU_Delete_Task(&m_RecognnizeTask1);

		NU_Control_Timer(&(m_Timer1), NU_DISABLE_TIMER);
		((UINT8 *)m_currentPoint1)[0] = x;// - m_sRect.left;
		((UINT8 *)m_currentPoint1)[1] = y;// - (m_sRect.top+TOP_HEIGHT);
		m_currentPoint1++;
		m_isDoMouseDown = 1;
	}
	else if(!(x<(m_sRect.right-width3) || 
				y<m_sRect.top+TOP_HEIGHT || 
				x>m_sRect.right-2 || y>m_sRect.bottom))
	{
		if(m_isMouseUp1)
			OnTimer(&m_Timer1);

		if(m_oldisDoMouseDown2)
			return;

		if(m_currentPoint2 - m_pointBuff2 >= MAX_POINT_NUM - 2)// hqf 防止死机
			return;
//		NU_Terminate_Task(&m_RecognnizeTask2);
//		NU_Delete_Task(&m_RecognnizeTask2);

		NU_Control_Timer(&(m_Timer2), NU_DISABLE_TIMER);
		((UINT8 *)m_currentPoint2)[0] = x;// - (m_sRect.right-width3);
		((UINT8 *)m_currentPoint2)[1] = y;// - (m_sRect.top+TOP_HEIGHT);
		m_currentPoint2++;
		m_isDoMouseDown = 2;
	} 
	else//处理非手写板区域事件
	{
		// modify by wangfei
        int xt = (x - m_sRect.left)/WRITECHARHEIGHT;
		int yt = (y - m_sRect.top)/WRITECHARHEIGHT; 
		INT16 left = m_sRect.left+xt*WRITECHARHEIGHT+1;
		CDC::InvertRect(left, m_sRect.top+1, left+WRITECHARHEIGHT, m_sRect.top+WRITECHARHEIGHT);
		m_bPressed = 1;
		m_isDoMouseDown = 12+xt;

		/*
		
		if(xt == 0 && yt == 0)
		{
			CDC::InvertRect(m_sRect.left+40, m_sRect.top+2, 
							m_sRect.left+40+WRITECHARHEIGHT, 
							m_sRect.top+2+WRITECHARHEIGHT);
			m_bPressed = 1;

			m_isDoMouseDown = 12;
		}
		else if(xt == 1 && yt == 0)
		{
			CDC::InvertRect(m_sRect.left+40+WRITECHARHEIGHT, 
							m_sRect.top+2, 
							m_sRect.left+40+2*WRITECHARHEIGHT, 
							m_sRect.top+2+WRITECHARHEIGHT);
			m_bPressed = 1;

			m_isDoMouseDown = 13;
		}
		else if(xt == 2 && yt == 0)
		{
			CDC::InvertRect(m_sRect.left+40+2*WRITECHARHEIGHT, 
							m_sRect.top+2, 
							m_sRect.left+40+3*WRITECHARHEIGHT, 
							m_sRect.top+2+WRITECHARHEIGHT);
			m_bPressed = 1;

			m_isDoMouseDown = 14;
		}
		else if(xt == 3 && yt == 0)
		{
			CDC::InvertRect(m_sRect.left+40+3*WRITECHARHEIGHT, 
							m_sRect.top+2, 
							m_sRect.left+40+4*WRITECHARHEIGHT, 
							m_sRect.top+2+WRITECHARHEIGHT);
			m_bPressed = 1;

			m_isDoMouseDown = 15;
		}
		*/
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标移动处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::DoMouseMove(int x, int y)
{
	CDC		dc;
	CPoint  pt;

	pt.x = x;
	pt.y = y;

	INT16 width3 = m_sRect.Width()/2-1;

	if(!m_isDoMouseDown)
		return;

	if(m_isDoMouseDown == 1)
	{
		if(m_currentPoint1 - m_pointBuff1 >= MAX_POINT_NUM - 2 ||
			m_currentPoint1 == m_pointBuff1)
			return ;

		if(x<m_sRect.left || y<m_sRect.top+TOP_HEIGHT || 
			x>m_sRect.right-width3-2 || y>m_sRect.bottom
			||( x == ((UINT8 *)(m_currentPoint1-1))[0] && 
				y== ((UINT8 *)(m_currentPoint1-1))[1])) // hqf 2002/1/7 
			return; 

		//dc.SetPenWidth(2);
		dc.MoveTo(((UINT8 *)(m_currentPoint1-1))[0], 
					((UINT8 *)(m_currentPoint1-1))[1]);
		((UINT8 *)m_currentPoint1)[0] = x;
		((UINT8 *)m_currentPoint1)[1] = y;
		m_currentPoint1++;
		dc.SetPenWidth(2);
		dc.LineTo(x, y);
	}
	else if(m_isDoMouseDown == 2)
	{
		if(m_currentPoint2 - m_pointBuff2 >= MAX_POINT_NUM - 2 ||
			m_currentPoint2 == m_pointBuff2)
			return ;

		if(x<(m_sRect.right-width3) || y<m_sRect.top+TOP_HEIGHT || 
			x>m_sRect.right-2 || y>m_sRect.bottom)
			return; 

		//dc.SetPenWidth(2);
		dc.MoveTo(((UINT8 *)(m_currentPoint2-1))[0], 
					((UINT8 *)(m_currentPoint2-1))[1]);
		((UINT8 *)m_currentPoint2)[0] = x;
		((UINT8 *)m_currentPoint2)[1] = y;
		m_currentPoint2++;
		dc.SetPenWidth(2);
		dc.LineTo(x, y);
	}
	//处理非手写板区域事件
	// modify by wangfei
	else if(m_isDoMouseDown >= 12 && m_isDoMouseDown <= 18)
	{
		 INT16 left = m_sRect.left+(m_isDoMouseDown-12)*WRITECHARHEIGHT+1;
         if(pt.InRect(left, m_sRect.top+1, 
					  left + WRITECHARHEIGHT, 
					  m_sRect.top+WRITECHARHEIGHT))
		 {
			 if(!m_bPressed)
			{
				CDC::InvertRect (left, m_sRect.top+1, 
								 left+WRITECHARHEIGHT, 
								m_sRect.top+WRITECHARHEIGHT);
				m_bPressed = 1;
			}
		 }
		 else
		 {
			 if(m_bPressed)
			{
				CDC::InvertRect (left, m_sRect.top+1, 
								 left+WRITECHARHEIGHT, 
								m_sRect.top+WRITECHARHEIGHT);
				m_bPressed = 0;
			}
 
		 }
		
	}

}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标其他事件处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::DoPenOther(UINT32 nMessage, 
						   UINT32 wParam, UINT32 lParam)
{
	if(!m_isDoMouseDown)
		return;
	if(m_isDoMouseDown == 1)
	{
		if(m_currentPoint1 - m_pointBuff1 >= MAX_POINT_NUM - 2)
		{
			NU_Reset_Timer(&(m_Timer1), (void(*)(UINT32))TimerProc1, 
							   IDENTIFY_TIME,0, NU_ENABLE_TIMER);
		}
		else
		{
			((UINT8 *)m_currentPoint1)[0] = PEN_END_SOMBOY;
			((UINT8 *)m_currentPoint1)[1] = PEN_STOP_SOMBOY;
			m_currentPoint1++;
			NU_Reset_Timer(&(m_Timer1), (void(*)(UINT32))TimerProc1, 
							   IDENTIFY_TIME,0, NU_ENABLE_TIMER);
		}
		m_isMouseUp1 = 1;
	}
	else if(m_isDoMouseDown == 2)
	{
		if(m_currentPoint2 - m_pointBuff2 >= MAX_POINT_NUM - 2)
		{
			NU_Reset_Timer(&(m_Timer2), (void(*)(UINT32))TimerProc2, 
							   IDENTIFY_TIME,0, NU_ENABLE_TIMER);
		}
		else
		{
			((UINT8 *)m_currentPoint2)[0] = PEN_END_SOMBOY;
			((UINT8 *)m_currentPoint2)[1] = PEN_STOP_SOMBOY;
			m_currentPoint2++;
			NU_Reset_Timer(&(m_Timer2), (void(*)(UINT32))TimerProc2, 
							   IDENTIFY_TIME,0, NU_ENABLE_TIMER);
		}
		m_isMouseUp2 = 1;
	}
	else if(m_isDoMouseDown>=12 && m_isDoMouseDown<=18 && m_bPressed ) 
	{
		INT16 left = m_sRect.left + (m_isDoMouseDown-12)*WRITECHARHEIGHT+1;
		CDC::InvertRect(left, m_sRect.top+1, left+WRITECHARHEIGHT, m_sRect.top+WRITECHARHEIGHT);
		m_bPressed = 0;
		if(m_CennectedCtrl && (*m_szResult))
				m_CennectedCtrl->SendMessage(CM_WRITEPAD_WRITE, 
						m_nCtrlId, 
						MAKEWORD((UINT8)(m_szResult[(m_isDoMouseDown-12)*2+1]), 
						(UINT8)(m_szResult[(m_isDoMouseDown-12)*2])));
	}
	//处理非手写板区域事件
	//todo //by wangfei
	m_isDoMouseDown = 0;
}

/////////////////////////////////////////////////////////////////////
//功能    : 记时器1响应函数 
//参数    : 手写板控件指针
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::TimerProc1(CWritePad *pCtrl)
{
	SYSTEM_MESSAGE	pmsg;

	NU_Control_Timer(&(pCtrl->m_Timer1), NU_DISABLE_TIMER);
	pmsg.message = WM_TIMER;
	pmsg.handle = (UINT32)(pCtrl);
	pmsg.wparam = (UINT32)(&(pCtrl->m_Timer1)) ;
	PostMessageToQueue(&pmsg);
}

/////////////////////////////////////////////////////////////////////
//功能    : 记时器2响应函数 
//参数    : 手写板控件指针
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::TimerProc2(CWritePad *pCtrl)
{
	SYSTEM_MESSAGE	pmsg;

	NU_Control_Timer(&(pCtrl->m_Timer2), NU_DISABLE_TIMER);
	pmsg.message = WM_TIMER;
	pmsg.handle = (UINT32)(pCtrl);
	pmsg.wparam = (UINT32)(&(pCtrl->m_Timer2)) ;
	PostMessageToQueue(&pmsg);
}

/////////////////////////////////////////////////////////////////////
//功能    : 识别任务1函数 
//参数    : 第一参数, 手写板控件指针
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
extern CFrameWnd *g_pCurWindow;
void   CWritePad::RecognnizePrc1(UNSIGNED argc, CWritePad *pCtrl)
{
	CDC dc;
	SYSTEM_MESSAGE	pmsg;

	INT16 width3 = pCtrl->m_sRect.Width()/2-1;

	//等待识别任务2识别结束
	while(pCtrl->m_oldisDoMouseDown2);
	if(!(pCtrl->m_dwStyle & WS_VISIBLE))
	{
		pCtrl->m_oldisDoMouseDown1 = 0;
		return;
	}

	//NU_Sleep(5);	//byw
	memset(pCtrl->m_szResult, 40, 0);
	int len=(pCtrl->m_currentPoint1 - pCtrl->m_pointBuff1);
	HWRecognize((unsigned char *)(pCtrl->m_pointBuff1), 
				len ,
				pCtrl->m_szResult, 16, 
				ALC_CHINESE_COMMON | ALC_CHINESE_RARE );	

	pCtrl->m_currentPoint1 = pCtrl->m_pointBuff1;       // hqf  2002-1-28


	if(!(pCtrl->m_dwStyle & WS_VISIBLE))// hqf 
	{
		pCtrl->m_oldisDoMouseDown1 = 0;
		return;
	}
	if(g_pCurWindow != ((CFrameWnd *)pCtrl->m_pParent))
	{
		pCtrl->m_oldisDoMouseDown1 = 0;
		return;
	}
// byw, debug	
	if(pCtrl->m_pParent)
		pCtrl->m_pParent->SendMessage(WM_WRITEREG_OVER , 0 , 0);	

	dc.EraseRect((pCtrl->m_sRect.left+1), 
		pCtrl->m_sRect.top+TOP_HEIGHT+1, 
		(pCtrl->m_sRect.left+width3)-1, 
		pCtrl->m_sRect.bottom-1);
	dc.TextOut(pCtrl->m_sRect.left+1, 
		pCtrl->m_sRect.top+1, pCtrl->m_szResult, RECOG_SEL_COUNT);
/////////////////// hqf ///////////////////////////////
/*   	pmsg.message = WM_PENDOWN;
	pmsg.handle  =  0;
	pmsg.wparam  =  MAKELONG(1, 1);
	PostMessageToQueue(&pmsg);
	pmsg.message = WM_PENMOVE;
	PostMessageToQueue(&pmsg);
	pmsg.message = WM_PENUP;
	PostMessageToQueue(&pmsg);*/

	//pmsg.message = WM_KEY;
	pmsg.handle = (UINT32)(pCtrl->m_CennectedCtrl);

//byw -----------------------
	/* 
	pmsg.wparam = MAKEWORD((UINT8)(pCtrl->m_szResult[1]), 
							(UINT8)(pCtrl->m_szResult[0])); 
	*/
	pmsg.message = CM_WRITEPAD_RECOG;
	pmsg.wparam = pCtrl->m_nCtrlId;
	pmsg.lparam = (UINT32)pCtrl->m_szResult;
//byw -----------------------

	if(pCtrl->m_szResult[0] != '\0')
		PostMessageToQueue(&pmsg);
	pCtrl->m_oldisDoMouseDown1 = 0;
}

/////////////////////////////////////////////////////////////////////
//功能    : 识别任务2函数 
//参数    : 第一参数, 手写板控件指针
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void    CWritePad::RecognnizePrc2(UNSIGNED argc, CWritePad *pCtrl)
{
	CDC dc;
	SYSTEM_MESSAGE	pmsg;

	INT16 width3 = pCtrl->m_sRect.Width()/2-1;

	//等待识别任务1识别结束
	while(pCtrl->m_oldisDoMouseDown1);
	if(!(pCtrl->m_dwStyle & WS_VISIBLE))
	{
		pCtrl->m_oldisDoMouseDown2 = 0;
		return;
	}

	//NU_Sleep(5);	//byw
	memset(pCtrl->m_szResult, 40, 0);
	int len=(pCtrl->m_currentPoint2 - pCtrl->m_pointBuff2);
	HWRecognize((unsigned char *)(pCtrl->m_pointBuff2), len, 
		pCtrl->m_szResult, 16, 
		ALC_CHINESE_COMMON | ALC_CHINESE_RARE );	
	pCtrl->m_currentPoint2 = pCtrl->m_pointBuff2;       // hqf  2002-1-28
	///////////// hqf //////////////////
	if(!(pCtrl->m_dwStyle & WS_VISIBLE))
	{
		pCtrl->m_oldisDoMouseDown2 = 0;
		return;
	}

/*	pmsg.message = WM_PENDOWN;
	pmsg.handle  =  0;
	pmsg.wparam  =  MAKELONG(1, 1);
	PostMessageToQueue(&pmsg);
	pmsg.message = WM_PENMOVE;
	PostMessageToQueue(&pmsg);
	pmsg.message = WM_PENUP;
	PostMessageToQueue(&pmsg);*/
		if(g_pCurWindow != ((CFrameWnd *)pCtrl->m_pParent))
	//如果有窗口覆盖在父窗口上,则不发消息
	{
		pCtrl->m_oldisDoMouseDown2 = 0;
		return;
	}
// byw, debug
	if(pCtrl->m_pParent)
		pCtrl->m_pParent->SendMessage(WM_WRITEREG_OVER , 0 , 0);
	///////////// hqf //////////////////

	dc.EraseRect(pCtrl->m_sRect.left+width3+1, 
		pCtrl->m_sRect.top+TOP_HEIGHT+1, 
		(pCtrl->m_sRect.right-1), 
		pCtrl->m_sRect.bottom-1);
	dc.TextOut(pCtrl->m_sRect.left+1, 
		pCtrl->m_sRect.top+1, pCtrl->m_szResult, RECOG_SEL_COUNT);

	//pmsg.message = WM_KEY;
	pmsg.handle = (UINT32)(pCtrl->m_CennectedCtrl);

//byw -----------------------
	/* 
	pmsg.wparam = MAKEWORD((UINT8)(pCtrl->m_szResult[1]), 
							(UINT8)(pCtrl->m_szResult[0])); 
	*/
	pmsg.message = CM_WRITEPAD_RECOG;
	pmsg.wparam = pCtrl->m_nCtrlId;
	pmsg.lparam = (UINT32)pCtrl->m_szResult;
//byw -----------------------

	if(pCtrl->m_szResult[0] != '\0')
		PostMessageToQueue(&pmsg);

	pCtrl->m_oldisDoMouseDown2 = 0;
}

/////////////////////////////////////////////////////////////////////
//功能    : 反初始化函数
//参数    : 
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CWritePad::UnInit()
{
	NU_Terminate_Task(&m_RecognnizeTask1); // hqf
	NU_Delete_Task(&m_RecognnizeTask1);
	NU_Terminate_Task(&m_RecognnizeTask2);
	NU_Delete_Task(&m_RecognnizeTask2);

    if(NU_Control_Timer(&(m_Timer1), NU_DISABLE_TIMER) != NU_SUCCESS)
		return 0;
	if(NU_Delete_Timer(&m_Timer1) != NU_SUCCESS)
		return 0;
    if(NU_Control_Timer(&(m_Timer2), NU_DISABLE_TIMER) != NU_SUCCESS)
		return 0;
	if(NU_Delete_Timer(&m_Timer2) != NU_SUCCESS)
		return 0;
/*
	NU_Terminate_Task(&m_RecognnizeTask1);
	NU_Delete_Task(&m_RecognnizeTask1);
	NU_Terminate_Task(&m_RecognnizeTask2);
	NU_Delete_Task(&m_RecognnizeTask2);
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 反初始化函数
//参数    : 
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CWritePad::Init()
{
	m_pointBuff1[MAX_POINT_NUM-1] = PEN_END_SOMBOY;
	m_pointBuff1[MAX_POINT_NUM-2] = PEN_END_SOMBOY;
	m_pointBuff2[MAX_POINT_NUM-1] = PEN_END_SOMBOY;
	m_pointBuff2[MAX_POINT_NUM-2] = PEN_END_SOMBOY;
	m_isMouseUp1 = 0;
	m_isMouseUp2 = 0;
	m_currentPoint1 = m_pointBuff1;
	m_currentPoint2 = m_pointBuff2;
	m_isDoMouseDown = 0;
	m_oldisDoMouseDown2 = 0;
	m_oldisDoMouseDown1 = 0;
	m_bPressed = 0;
	m_CennectedCtrl = 0;
	//modify by wangfei
    *m_szResult = 0;
	memset(&m_Timer1, 0, sizeof(NU_TIMER));
	if(NU_Create_Timer(&m_Timer1, 
		            NULL, (void(*)(UINT32))TimerProc1,
					UINT32(this), IDENTIFY_TIME, 0, 
					NU_DISABLE_TIMER) != NU_SUCCESS)
					return FALSE;
	memset(&(m_RecognnizeTask1), 0 , sizeof(NU_TASK));
	if(NU_Create_Task(&(m_RecognnizeTask1), "TASK 1",    // hqf 
		(void(*)(unsigned long, void *))(RecognnizePrc1),
		0, this, m_uiTaskStack1, HAND_STACK_SIZE, 
		10, 2, NU_PREEMPT, NU_START) != NU_SUCCESS)
		return FALSE;
	NU_Terminate_Task(&m_RecognnizeTask1);

	memset(&(m_RecognnizeTask2), 0 , sizeof(NU_TASK));
	if(NU_Create_Task(&(m_RecognnizeTask2), "TASK 2", 
		(void(*)(unsigned long, void *))(RecognnizePrc2),
		0, this, m_uiTaskStack2, HAND_STACK_SIZE, 
		10, 2, NU_PREEMPT, NU_START) != NU_SUCCESS)
		return FALSE;
	NU_Terminate_Task(&m_RecognnizeTask2);


	memset(&m_Timer2, 0, sizeof(NU_TIMER));
	if(NU_Create_Timer(&m_Timer2, 
		            NULL, (void(*)(UINT32))TimerProc2,
					UINT32(this), IDENTIFY_TIME, 0, 
					NU_DISABLE_TIMER) != NU_SUCCESS)
					return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 创建手写板
//参数    : 标题(不显示), 大小, 父窗口, 类型, id号
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CWritePad::Create(char *strName, const CRect wndRect, 
				   CWnd * pParent, UINT32 dwStyle, UINT32 nCtrlId)
{
	if(!CCtrl::Create(strName, wndRect, pParent, dwStyle, nCtrlId))	
		return FALSE;
    m_bInit = Init();
	if(!m_bInit)
	{
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 销毁函数  
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::Destroy()
{
	if(m_bInit)
		UnInit();
    CCtrl::Destroy(); 
}

/////////////////////////////////////////////////////////////////////
//功能    : 关联一个控件
//参数    : 控件指针
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CWritePad::CennectCtrl(CCtrl *pCtrl)
{
	if(pCtrl)
		m_CennectedCtrl = pCtrl;
	else
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 记时器事件处理函数
//参数    : 记时器指针
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CWritePad::OnTimer(NU_TIMER * pTimer)
{
	CDC dc;
	STATUS status;
	if(pTimer == &m_Timer1)
	{
		if(!m_isMouseUp1)
			return;
		m_isMouseUp1 = 0;

		m_oldisDoMouseDown1 = 1;

		((UINT8 *)(m_currentPoint1))[0] = PEN_END_SOMBOY;
		((UINT8 *)(m_currentPoint1))[1] = PEN_END_SOMBOY;

//		m_currentPoint1 = m_pointBuff1;
	
/*		if(NU_Create_Task(&(m_RecognnizeTask1), "TASK 1", 
					(void(*)(unsigned long, void *))(RecognnizePrc1),
					0, this, m_uiTaskStack1, HAND_STACK_SIZE, 
					10, 2, NU_PREEMPT, NU_START) != NU_SUCCESS)

*/
		status = NU_Reset_Task(&m_RecognnizeTask1 , 0, this);
		status = NU_Resume_Task(&m_RecognnizeTask1);
		if(status != NU_SUCCESS)
		{
			m_oldisDoMouseDown1 = 0;
			this->OnPaint(dc);
		}
		//*/
	}
	else if(pTimer == &m_Timer2)
	{
		if(!m_isMouseUp2)
			return;
		m_isMouseUp2 = 0;

		m_oldisDoMouseDown2 = 1;

		((UINT8 *)(m_currentPoint2))[0] = PEN_END_SOMBOY;
		((UINT8 *)(m_currentPoint2))[1] = PEN_END_SOMBOY;

//		m_currentPoint2 = m_pointBuff2;
		
		/*if(NU_Create_Task(&(m_RecognnizeTask2), "TASK 2", 
					(void(*)(unsigned long, void *))(RecognnizePrc2),
					0, this, m_uiTaskStack2, HAND_STACK_SIZE, 
					10, 2, NU_PREEMPT, NU_START) != NU_SUCCESS)
		*/
		status = NU_Reset_Task(&m_RecognnizeTask2 , 0, this);
		status = NU_Resume_Task(&m_RecognnizeTask2);
		if(status != NU_SUCCESS)
		{
			m_oldisDoMouseDown2 = 0;
			this->OnPaint(dc);
		}
		//*/
	}
}

//CWritePad--------------------------------------------------------------


UINT8		g_uiHandUseBuff[16*1024];			//识别函数用的缓存
/////////////////////////////////////////////////////////////////////
//功能    : 初始化识别字典
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void InitHwDictionary()
{
	HWSetDictionary((UINT32)g_uiHandUseBuff, (UINT32)HW_DIC_ADDRESS);
}
