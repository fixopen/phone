/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             cedit.cpp                                                      */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             编辑控件类源文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-08-09    修改Paste函数和最后一行不显示文本的问题          */
/*             2001-09-03    修改Paste函数SetWindowTex函数GetWindowTex函数    */
/*             2001-11-01    王飞修改很多地方                                 */
/*             2001-11-09    郝庆丰修改ResetText函数                          */
/*             2001-11-26    余国荣修改Create()和SetWindowText()              */
/*                           增加WS_Disable属性的strTitle风格，可示标题       */
/*             2001-12-06    郝庆丰修改Create()函数                           */
/******************************************************************************/
#include "edit.h"
#include "writepad.h"

/////////////////////////////////////////////////////////////////////
//功能    : 判断是否是回车
//参数    : 字符串指针
//返回值  : 是,真; 否,假
//注意事项: 无
/////////////////////////////////////////////////////////////////////
BOOL IsEntCha(const char * pSrc)
{
	return (pSrc[0] == 0x0d && pSrc[1] == 0x0a);
}

/////////////////////////////////////////////////////////////////////
//功能    : 取下一行指针
//参数    : 字符串指针, 行宽度
//返回值  : 下一行字符串指针
//注意事项: 无
/////////////////////////////////////////////////////////////////////
char * GetNextLine(const char * pSrc, INT32 iWidth)//-
{
	const char  * start;
	const char  * pb;
	INT32		linewidth;

	linewidth = 0;
	start = (const char *)pSrc;

	while(linewidth <= iWidth && !IsEntCha(start) && *start != 0)
	{
		pb = start;
		start = CharNext(start);
		linewidth = linewidth + CHARWIDTH*(start - pb);
	}
	if(linewidth > iWidth)
	{
		start = CharPre(pSrc,start);
	}
	else if(start[0] == 0x0d && start[1] == 0x0a)
	{
		start = start + 2;
	}

	return (char *)start;
}

/////////////////////////////////////////////////////////////////////
//功能    : 取总行数
//参数    : 字符串指针, 行宽度
//返回值  : 总行数
//注意事项: 宽度太小会进入死循环
/////////////////////////////////////////////////////////////////////
INT32 GetTotalLines(const char * pSrc, INT32 iWidth)
{
	const char  * start;
	INT32		lines;

	if(!pSrc)
		return 0;

	start = (const char *)pSrc;
	lines = 0;

	while(*start != 0)
	{
		start = GetNextLine(start, iWidth);
		lines = lines + 1;
	}
	if(*(start -1) == 0x0a && *(start -2) == 0x0d)
		lines = lines + 1;
	
	return lines;
}

/////////////////////////////////////////////////////////////////////
//功能    : 取下一字符指针(汉字算一个)
//参数    : 当前字符串指针
//返回值  : 下一字符指针
//注意事项: 无
/////////////////////////////////////////////////////////////////////
char *CharNext(const char *pCurrent)//-
{
	if((*(const unsigned char *)pCurrent >= 0xa0) && 
		(*(const unsigned char *)(pCurrent+1) != 0x00))
		return (char *)(pCurrent + 2);
	return (char *)(pCurrent + 1);
}

/////////////////////////////////////////////////////////////////////
//功能    : 取上一字符指针(汉字算一个)
//参数    : 顶头字符串指针, 当前字符串指针
//返回值  : 上一字符指针
//注意事项: 无
/////////////////////////////////////////////////////////////////////
char *CharPre(const char *pStart, const char *pCurrent)
{
	const char * pold;
	const char * p = pStart;
	if(!p)
		return NULL;
	while(p < pCurrent)
	{
		pold = p;
		p = CharNext(pold);
	}
	if(p <= pStart + 1)
		return (char *)pStart;
	return (char *)(pold);
}

/////////////////////////////////////////////////////////////////////
//功能    : 创建编辑框
//参数    : 初始文本, 大小, 父窗口, 类型, ID号, 文本长度
//返回值  : 成功,真;不成功,假
//注意事项: 无
/////////////////////////////////////////////////////////////////////
#include "datadrv.h"
BOOL CEdit::Create(char *strName, const CRect wndRect, 
				   CWnd * pParent, UINT32 dwStyle, 
				   UINT32 nCtrlId, INT32 uiBuffLen,char * strTitle, CWritePad *pConnectedCtrl)//ygr
{
	m_type  =0;			
	m_editBuff = NULL;			
	m_editSelBuff = NULL;	//byw
	m_currentCursor = 0;	
	m_endSelected = 0;	
	m_pConnectedCtrl = pConnectedCtrl ; // hqf
	int iLen;
	if(strTitle == NULL)
		iLen = 0;
	else 
		iLen = strlen(strTitle);
	m_buffLen = uiBuffLen + iLen;  // hqf			
	m_lineIndex = NULL;
	if(wndRect.Width() < CHARHEIGHT*2)
		return 0;

	if(!CScrollWnd::Create(strName, wndRect, pParent, dwStyle|WS_BORDER, nCtrlId, 14, 1, 0))
		return 0;

	if(!Init())
		return 0;
//ygr-----------------------------------------------------
	m_strTitle[0]=0;
	if(strTitle && (dwStyle & WS_DISABLE) )
	{
		char stmp[MEMO_DATA_LEN+28];
		strcpy(m_strTitle,strTitle);
		if(strName)		
			sprintf(stmp,"%s:%s",strTitle ,strName);
		strcpy(m_editBuff,stmp);
	}
	else
//-----------------------------------------------------ygr       // ??
	{
		if(strName)		
			hwstrncpy(m_editBuff, strName, m_buffLen-1);
	}
	ResetText();
	m_type=1;
	if(m_type)
		SetCursorAttr(2, CHARHEIGHT-1);
	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 反初始化
//参数    : 无
//返回值  : 无
//注意事项: 无
/////////////////////////////////////////////////////////////////////
void   CEdit::UnInit()
{										
	if(m_editBuff)
		SysFree(m_editBuff);
	if(this->m_lineIndex)
		SysFree(m_lineIndex);
	
	// byw -----------------------------
	if(m_editSelBuff)
		SysFree(m_editSelBuff);
	// byw -----------------------------

    CloseCursor();
}

/////////////////////////////////////////////////////////////////////
//功能    : 销毁编辑器
//参数    : 无
//返回值  : 无
//注意事项: 无
/////////////////////////////////////////////////////////////////////
void CEdit::Destroy()
{
	UnInit();
    CCtrl::Destroy(); 
}

/////////////////////////////////////////////////////////////////////
//功能    : 取编辑器文本
//参数    : 文本缓存, 文本字节数
//返回值  : 无
//注意事项: 无
/////////////////////////////////////////////////////////////////////
void CEdit::GetWindowText(char *pBuf, UINT16 nMaxCount)
{
	hwstrncpy(pBuf, m_editBuff, nMaxCount-1);
	pBuf[nMaxCount-1] = 0;
}

/////////////////////////////////////////////////////////////////////
//功能    : 社编辑器文本
//参数    : 文本缓存
//返回值  : 成功,真;不成功,假
//注意事项: 无
/////////////////////////////////////////////////////////////////////
BOOL CEdit::SetWindowText(const char * pText,BOOL bRedraw)
{
	CDC dc;

	if(strlen(pText) > m_buffLen-1 || !pText)
		return 0;
	//ygr----------------------------------------------
	char stmp[MEMO_DATA_LEN+28];
	if(m_strTitle[0])
	{
		sprintf(stmp,"%s:%s",m_strTitle ,pText);
		strcpy(m_editBuff,stmp);
	}
	else
		strcpy(m_editBuff,pText);
	//----------------------------------------------ygr

	// byw -----------------------------
	if(m_editSelBuff)
		memset(m_editSelBuff, 0, strlen(m_editBuff) * RECOG_SEL_COUNT);
	// byw -----------------------------

	//strcpy(m_editBuff, pText);
	ResetText();
	if(bRedraw)
		OnPaint(dc);
	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 拷贝选中文本
//参数    : 文本缓存
//返回值  : 成功,真;不成功,假
//注意事项: 无
/////////////////////////////////////////////////////////////////////
BOOL	CEdit::Copy(char * pTextBuff, UINT32 nBufLen)
{
	UINT32 iSelTextLen;
	if(m_currentCursor > m_endSelected)
	{
		iSelTextLen = MIN(m_currentCursor-m_endSelected , nBufLen -1);
		memcpy(pTextBuff, m_editBuff+m_endSelected, 
					iSelTextLen);
		pTextBuff[iSelTextLen] = 0;
	}
	else
	{
		iSelTextLen = MIN(m_endSelected-m_currentCursor , nBufLen -1);
		memcpy(pTextBuff, m_editBuff+m_currentCursor, 
					iSelTextLen);
		pTextBuff[iSelTextLen] = 0;
	}
	
	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 复制选中文本
//参数    : 文本缓存
//返回值  : 成功,真;不成功,假
//注意事项: 无
/////////////////////////////////////////////////////////////////////
BOOL	CEdit::Paste(const char * pText)
{
	char *str1, *str2, *selstr;
	int iBufLeft = m_buffLen -1 -strlen(m_editBuff) + 
		ABS((int)(m_currentCursor-m_endSelected));

	if(iBufLeft  <= 0)
		return FALSE;
	str1 = m_editBuff + MIN(m_currentCursor, m_endSelected);
	
	// byw -----------------------------
	if(m_editSelBuff)
		selstr = m_editSelBuff + MIN(m_currentCursor, m_endSelected) * RECOG_SEL_COUNT;
	// byw -----------------------------

	if( strlen(pText) <= iBufLeft)
	{
		if(GetCurPos() != m_endSelected)
			BackUp();
		
		// byw -----------------------------
		int		templen = strlen(str1);
		// byw -----------------------------

		memmove(str1 + strlen(pText), str1, strlen(str1)+1);
	
		if(m_editSelBuff)
		{
			// byw -----------------------------
			memmove(selstr + strlen(pText) * RECOG_SEL_COUNT, selstr, (templen+1) * RECOG_SEL_COUNT);
			// byw -----------------------------
			
			memcpy(str1, pText, strlen(pText));
			
			// byw -----------------------------
			memset(selstr, 0, strlen(pText) * RECOG_SEL_COUNT);
			// byw -----------------------------
		}

		SetCurPos(GetCurPos() + strlen(pText));


		ResetText();
		BaseCursor();
		CDC dc;
		OnPaint(dc);
	}
	else  // hqf
	{
		char *pTemp = (char *)SysMalloc(iBufLeft + 1);
		if(!pTemp)
			return FALSE;
		strncpy(pTemp ,	pText, iBufLeft);
		pTemp[iBufLeft] = '\0';
		if(iBufLeft <= 1)
			return FALSE;
		char *pPrev = CharPre(pTemp, &pTemp[iBufLeft-1]);
		if((pTemp[iBufLeft - 1] & 0x80 )&& ((pTemp + iBufLeft  - pPrev) & 0x01))
			pTemp[iBufLeft -1] = '\0';
		Paste(pTemp);
		SysFree(pTemp);
		return 1;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 初始化 
//参数    : 
//返回值  : 成功,真;不成功,假
//注意事项: 
/////////////////////////////////////////////////////////////////////
#define		SAFELEN		32		//byw
INT32 CEdit::Init()
{
	char * p = (char *)SysMalloc(this->m_buffLen + SAFELEN);
	if(!p)
		return 0;
	this->m_editBuff = p;
	m_editBuff[m_buffLen-1] = 0;

	// byw -----------------------------
	if(m_pConnectedCtrl)
	{
		p = (char *)SysMalloc(this->m_buffLen * RECOG_SEL_COUNT + SAFELEN);
		if (!p){
			SysFree(this->m_editBuff);
			this->m_editBuff = NULL;
			return 0;
		}
		m_editSelBuff = p;
		memset(m_editSelBuff, 0, this->m_buffLen * RECOG_SEL_COUNT);
	}
	// byw -----------------------------

	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 处理函数
//参数    : 消息类型,  参数一, 参数二
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	CScrollWnd::WindowProcess(nMessage, wParam, lParam);

	switch(nMessage)
	{
		case CM_WRITEPAD_WRITE:
			{
			// hqf
			char strRegbuf[RECOG_SEL_COUNT +1];
			short iPos = GetCurPos();
			for(short i =0;i < RECOG_SEL_COUNT/2 ;i++)
			{
				strRegbuf[i*2] = m_editSelBuff[(iPos- 2) * RECOG_SEL_COUNT + i];
				strRegbuf[i*2 + 1] = m_editSelBuff[(iPos - 1) * RECOG_SEL_COUNT +i];
			}
			strRegbuf[RECOG_SEL_COUNT] = '\0';
			
			BackUp(FALSE);
			InsetChar((short)lParam, strRegbuf, FALSE);
			break;
			}
		case CM_WRITEPAD_RECOG:		//byw
			//BackUp(FALSE);
			InsetChar(MAKEWORD(*((char*)lParam + 1), *(char*)lParam), (char*)lParam, FALSE);
			break;
		case WM_KEY:
			if(m_type == ENABLE_EDIT)
			{
				switch((short)wParam)
				{
				case 0:
					break;
				case CE_CURSOR_UP:
					CursorUp();
					break;
				case CE_CURSOR_DOWN:
					CursorDown();
					break;
				case CE_CURSOR_LEFT:
					CursorLeft();
					break;
				case CE_CURSOR_RIGHT:
					CursorRight();
					break;
				case CE_DELETE_CHAR:
					DeleteChar();
					break;
				case CE_INSERT_ENTER:
					InsertEnter();
					break;
				case CE_BACK_UP:
					BackUp();
					break;
				default: 
					InsetChar((short)wParam, NULL, TRUE);
					break;
				}
			}
			break;
		case WM_DACTIVE:
			if(g_pCurWindow != m_pParent) // hqf
				::CloseCursor();
			break;
		default:
			break;
		
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 客户区消息处理函数
//参数    : 消息类型, x坐标, y坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void  CEdit::OnClientEvent(UINT32 nMessage, UINT32 x, UINT32 y)
{
	CScrollWnd::OnClientEvent(nMessage, x, y);
	if(nMessage == WM_PENDOWN)
		DoMouseDown(x, y);
	else if(nMessage == WM_PENMOVE)
		DoMouseMove(x, y);
}

/////////////////////////////////////////////////////////////////////
//功能    : 绘制函数
//参数    : CDC
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void  CEdit::OnPaint(CDC &dc)
{
	if(m_dwStyle & WS_BORDER)//ygr
		CScrollWnd::OnPaint(dc);
	else
		CScrollWnd::DrawScroll();
	PutLines();
	DrawSelected();
	//Draw cursor
	if(m_type == ENABLE_EDIT)
		DrawCursor();
	//Draw end
}

/////////////////////////////////////////////////////////////////////
//功能    : 取字符所在行好号
//参数    : 字符索引
//返回值  : 字符所在行号
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CEdit::GetLineOfChar(UINT32 nChar)//-
{
	INT32 i;

	if(GetScrollRage(1) == 0)
		return 0;
	for(i=0; i<GetScrollRage(1) && nChar >= m_lineIndex[i]; i++);
	if((i == GetScrollRage(1) - 1) && nChar > m_lineIndex[i])
		return i;
	else
		return i - 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 插入字符
//参数    : 起始字符索引, 结束字符索引
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::InvertChar(INT32 iFrom, INT32 iEnd)
{
	CDC dc;
	INT32 i;
	RECT re;
	if(iEnd <= iFrom)
		return;
	for(i = iFrom; i<iEnd && GetCharRect(i, &re); i++)
	{
		if(IsEntCha(m_editBuff + i))
			i += 1;
		else
			dc.InvertRect(re.left, re.top, re.right, re.bottom);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取字符矩形
//参数    : 字符索引, RECT指针
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CEdit::GetCharRect(UINT32 iChar, RECT * rRect)
{
	INT32				i;
	short lines = (m_sRect.bottom-m_sRect.top-TOPMAGIN)/CHARHEIGHT;
	UINT32 currentline = GetScrollPos(1);

	if(GetScrollRage(1) > 0 && 
		iChar >= m_lineIndex[currentline] && 
		iChar <strlen(m_editBuff))
	{
		for(i=0; 
			i<GetScrollRage(1)-1-currentline && 
				m_lineIndex[currentline+i]<=iChar && i<lines; 
			i++);
		if(m_lineIndex[currentline+i] > iChar)
			i = i - 1;
		else if(GetScrollRage(1)-1 < currentline + lines)
			;
		else
			return 0;
		rRect->left = m_sRect.left+LEFTMAGIN+
						(iChar-m_lineIndex[currentline+i])*CHARWIDTH;
		rRect->right = rRect->left + CHARWIDTH - 1;
		rRect->top = m_sRect.top + TOPMAGIN + i*CHARHEIGHT;
		rRect->bottom = rRect->top + CHARHEIGHT - 1;
		return 1;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////
//功能    : 取得当前光标所在行
//参数    : 
//返回值  : 当前光标所在行号
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CEdit::GetLineOfCursor()//-
{
	GetLineOfChar(GetCurPos());
}

/////////////////////////////////////////////////////////////////////
//功能    : 取鼠标点击的字符位置
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32  CEdit::GetMouseChar(INT32 *iCol, INT32 *iRow)//-
{
	if(GetScrollRage(1) <= 0)
	{
		*iCol = 0;
		*iRow  = 0;
		return -1;
	}
	*iCol = (*iCol-m_sRect.top-TOPMAGIN)/CHARHEIGHT+GetScrollPos(1);
	*iRow  = (*iRow-m_sRect.left-LEFTMAGIN)/CHARWIDTH;

	if(*iCol < GetScrollRage(1)-1 && 
		m_lineIndex[*iCol]+*iRow >= m_lineIndex[*iCol + 1])
		return -1;
	else if(*iCol == GetScrollRage(1) - 1 &&
		*iRow > strlen(m_editBuff + m_lineIndex[*iCol]))
		return -1;
	else if(*iCol > GetScrollRage(1) - 1)
		return -1;

	return m_lineIndex[*iCol] + *iRow;
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标点下处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::DoMouseDown(INT32 x, INT32 y)//-
{
	INT32 line=y, row=x;
	INT32 cursorpos;
	char *str;	
	UINT32  *pline;

	pline = m_lineIndex;
	cursorpos = GetMouseChar(&line, &row);

	if(cursorpos >= 0)
	{
		SetCurPos( cursorpos);
		str = m_editBuff + GetCurPos();
		if(cursorpos > 0 && *str == 0x0a &&	*(str - 1) == 0x0d)
			SetCurPos(GetCurPos() - 1);
	}
	else if(line < GetScrollRage(1) - 1)
	{
		if(m_editBuff[pline[line + 1] - 1] == 0x0a &&
			m_editBuff[pline[line + 1] - 2] == 0x0d)
		{
			SetCurPos( pline[line + 1] - 2);
		}
		else
		{
			SetCurPos( pline[line + 1]);
		}
	}
	else
	{
		SetCurPos( strlen(m_editBuff));
	}

	ResetCursorPos();

	CDC dc;
	OnPaint(dc);
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标移动处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::DoMouseMove(INT32 x, INT32 y)//-
{
	INT32 line=y, row=x;
	INT32 cursorpos;
	char *str;	
	UINT32  *pline;
	UINT32  oldend;

	pline = m_lineIndex;
	cursorpos = GetMouseChar(&line, &row);
	oldend = m_endSelected;

	if(cursorpos >= 0)
	{
		m_endSelected = (cursorpos);
		str = m_editBuff + m_endSelected;
		if(cursorpos > 0 && *str == 0x0a &&	*(str - 1) == 0x0d)
			m_endSelected = (m_endSelected - 1);
	}
	else if(line < GetScrollRage(1) - 1)
	{
		if(m_editBuff[pline[line + 1] - 1] == 0x0a &&
			m_editBuff[pline[line + 1] - 2] == 0x0d)
		{
			m_endSelected = (pline[line + 1] - 2);
		}
		else
		{
			m_endSelected = (pline[line + 1]);
		}
	}
	else
	{
		m_endSelected = (strlen(m_editBuff));
	}
	ResetSelEnd();
	if(oldend != m_endSelected)
	{
		if(oldend > m_endSelected)
			InvertChar(m_endSelected, oldend);
		else
			InvertChar(oldend, m_endSelected);
	}

}

/////////////////////////////////////////////////////////////////////
//功能    : 显示光标
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::DrawCursor( )
{
	INT32		curpos;
	INT32		curY;
	char *  str;

	curpos = GetLineOfCursor();
	str = m_editBuff + GetCurPos();

	if(GetCurPos() > strlen(m_editBuff))
		m_currentCursor = 0;

	if(GetScrollRage(1) > 0)
	{
		curY = (curpos - GetScrollPos(1))*CHARHEIGHT+
				m_sRect.top+TOPMAGIN;
		SetCursorXY((GetCurPos() - m_lineIndex[curpos])*CHARWIDTH + 
						LEFTMAGIN + m_sRect.left, curY);
	}
	else
	{
		curY = m_sRect.top;
		SetCursorXY( LEFTMAGIN + m_sRect.left, curY);
	}

	if(curpos < GetScrollPos(1) || curY > m_sRect.bottom-CHARHEIGHT)
		CloseCursor();
	else
		OpenCursor();
}

/////////////////////////////////////////////////////////////////////
//功能    : 光标移到汉字中的处理(将光标设到汉字两边)
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::ResetCursorPos()
{
	INT32			   lineofcur;
	char		*  str, *strold;
	UINT32  *pline;

	pline = m_lineIndex;

	if(*(m_editBuff + GetCurPos()) == 0)
		return;
	if(GetCurPos() > 0)
	{
		lineofcur = GetLineOfCursor();
		str = m_editBuff + pline[lineofcur];
		while(*str != 0 && str-m_editBuff-pline[lineofcur] <= 
									GetCurPos()-pline[lineofcur])
		{
			strold = str;
			str = CharNext(str);
		}

		str = strold;

		SetCurPos(str - m_editBuff);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 光标移到无文本区的处理(将光标移到行末)
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::ResetSelEnd()
{
	INT32			   lineofcur;
	char		*  str, *strold;
	UINT32  *pline;

	pline = m_lineIndex;

	if(*(m_editBuff + m_endSelected) == 0)
		return;
	if(m_endSelected > 0)
	{
		lineofcur = GetLineOfChar(m_endSelected);
		str = m_editBuff + pline[lineofcur];
		while(*str != 0 && str-m_editBuff-pline[lineofcur] <= 
								m_endSelected-pline[lineofcur])
		{
			strold = str;
			str = CharNext(str);
		}

		str = strold;

		m_endSelected = (str - m_editBuff);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 将光标所在行该为当前行
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::BaseCursor()//-
{
	char		 * str;
	INT32			   lineofcur;

	lineofcur = GetLineOfCursor();
	str = m_editBuff;

	if(lineofcur < GetScrollPos(1))
		SetScrollPos( lineofcur, 1);
	else if((lineofcur - GetScrollPos(1)) >= 
		(m_sRect.bottom-m_sRect.top-TOPMAGIN) /CHARHEIGHT)
	{
		SetScrollPos(lineofcur+1-
			(m_sRect.bottom-m_sRect.top-TOPMAGIN)/CHARHEIGHT, 1);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 社光标所在字符索引
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::SetCurPos(UINT32 iPos, BOOL bRedraw)
{
	m_currentCursor = iPos;
	m_endSelected = iPos;
	if(bRedraw && m_pConnectedCtrl )
	{
		char strRegbuf[RECOG_SEL_COUNT+1] = "\0";
		if(iPos >= 2)
		{
			if(m_editBuff[iPos -1] & 0x80)
			{
				for(short i =0;i < RECOG_SEL_COUNT/2 ;i++)
				{
					strRegbuf[i*2] = m_editSelBuff[(iPos- 2) * RECOG_SEL_COUNT + i];
					strRegbuf[i*2 + 1] = m_editSelBuff[(iPos - 1) * RECOG_SEL_COUNT +i];
				}
				strRegbuf[RECOG_SEL_COUNT] = '\0';
			}	
		}
		m_pConnectedCtrl->SendMessage(CM_WRITEPAD_SHOWREC , m_nCtrlId , (UINT32)strRegbuf);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取光标所在字符索引
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
UINT32 CEdit::GetCurPos()
{
	return m_currentCursor;
}

/////////////////////////////////////////////////////////////////////
//功能    : 光标左移处理
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::CursorLeft( )//-
{
	char *str1, *str2;

	str1 = m_editBuff + GetCurPos();

	if(GetCurPos() > 1)
	{
		if(*(str1 - 1) == 0x0a && *(str1 - 2) == 0x0d)
			SetCurPos(GetCurPos() - 2);
		else
		{
			str2 = CharPre(m_editBuff, str1);
			SetCurPos( GetCurPos() - (str1 - str2));
		}
	}
	else if(GetCurPos() > 0)
		SetCurPos( GetCurPos() - 1);

	BaseCursor();
	CDC dc;
	OnPaint(dc);
}

/////////////////////////////////////////////////////////////////////
//功能    : 光标右移处理
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::CursorRight( )//-
{
	char *str1, *str2;

	str1 = m_editBuff + GetCurPos();

	if(GetCurPos() < strlen(m_editBuff))
	{
		if(*(str1) == 0x0d && *(str1 + 1) == 0x0a)
			SetCurPos( GetCurPos() + 2);
		else
		{
			str2 = CharNext(str1);
			SetCurPos( GetCurPos() + str2 - str1);
		}
	}
	BaseCursor();
	CDC dc;
	OnPaint(dc);
}

/////////////////////////////////////////////////////////////////////
//功能    : 光标上移处理
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::CursorUp( )//-
{
	char         * str;
	INT32			   cursorline;
	UINT32 * pline;

	pline = m_lineIndex;
	cursorline = GetLineOfCursor();
	if(cursorline > 0)
	{
		str = m_editBuff + GetCurPos();
		SetCurPos(pline[cursorline-1]+GetCurPos()-pline[cursorline]);
		if(GetCurPos() > pline[cursorline] - 2)
			SetCurPos( pline[cursorline] - 2);
	}
	ResetCursorPos();
	BaseCursor();
	CDC dc;
	OnPaint(dc);
}

/////////////////////////////////////////////////////////////////////
//功能    : 光标下移处理
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::CursorDown( )//-
{
	char		 * str;
	INT32			   cursorpos;
	UINT32 * pline;

	pline = m_lineIndex;
	cursorpos = GetLineOfCursor();
	str = m_editBuff;

	if(cursorpos < GetScrollRage(1) - 2)
	{
		SetCurPos(GetCurPos()+pline[cursorpos+1]-pline[cursorpos]);
		if(GetCurPos() > pline[cursorpos + 2] - 2)
			SetCurPos( pline[cursorpos + 2] - 2);
	}
	else if(cursorpos < GetScrollRage(1) - 1)
	{
		SetCurPos(GetCurPos()+pline[cursorpos+1]-pline[cursorpos]);
		if(GetCurPos() > 
				strlen(str+pline[cursorpos+1])+pline[cursorpos+1])
			SetCurPos( strlen(str));
	}
	else if(*(str+strlen(str)-1) == 0x0a && 
					*(str+strlen(str)-2) == 0x0d)
		SetCurPos( strlen(str));

	ResetCursorPos();
	BaseCursor();
	CDC dc;
	OnPaint(dc);
}

/////////////////////////////////////////////////////////////////////
//功能    : 在当前光标插入字符
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::InsetChar(short ch, char *sel, BOOL bRedraw)//-
{
	char *str1, *str2, *selstr;

	if(GetCurPos() != m_endSelected)
		DeleteChar(FALSE);

	str1 = m_editBuff + GetCurPos();
	
	//byw ----------------------
	if(m_editSelBuff)
		selstr = m_editSelBuff + GetCurPos() * RECOG_SEL_COUNT;
	//byw ----------------------

	long len = strlen(m_editBuff);
	if(len < m_buffLen -1 && 0x00ff & ch)
	{
		if(0xff00 & ch)
		{
			if(len < m_buffLen -1-1)
			{
				//byw ----------------------
				int		templen = strlen(str1);
				//byw ----------------------
				
				memmove(str1 + 2, str1, strlen(str1)+1);
				*(m_editBuff+GetCurPos()) = HIBYTE(ch);
				*(m_editBuff+GetCurPos() + 1) = LOBYTE(ch);

				//byw ----------------------
				if(m_editSelBuff)
				{
					UINT32	csrpos = GetCurPos();
					memmove(selstr + 2 * RECOG_SEL_COUNT, selstr, (templen+1) * RECOG_SEL_COUNT);
					for (int i = 0; i < RECOG_SEL_COUNT/2; i ++){
						if(sel)
						{
							*(m_editSelBuff + csrpos * RECOG_SEL_COUNT + i) = *(sel + i * 2);
							*(m_editSelBuff + (csrpos + 1) * RECOG_SEL_COUNT + i) = *(sel + i * 2 + 1);
						}
						else
						{
							*(m_editSelBuff + csrpos * RECOG_SEL_COUNT + i) = 0;
							*(m_editSelBuff + (csrpos + 1) * RECOG_SEL_COUNT + i) = 0;
						}
					}

				}
				//byw ----------------------

				SetCurPos(GetCurPos() + 2 , bRedraw);
			}
		}
		else
		{
			//byw ----------------------
			int		templen = strlen(str1);
			//byw ----------------------

			memmove(str1 + 1, str1, strlen(str1)+1);
			*(m_editBuff + GetCurPos()) = ch;

			//byw ----------------------
			if(m_editSelBuff)
			{
				UINT32	csrpos = GetCurPos();
				memmove(selstr + RECOG_SEL_COUNT, selstr, (templen+1) * RECOG_SEL_COUNT);
				memset(m_editSelBuff + csrpos * RECOG_SEL_COUNT , 0 ,RECOG_SEL_COUNT);
			}
			//byw ----------------------

			SetCurPos( GetCurPos() + 1 , bRedraw);
		}
		ResetText();
		BaseCursor();

		CDC dc;
		OnPaint(dc);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 在当前光标前删字符
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::DeleteChar(BOOL bRedraw)//-
{
	char *str1, *str2, *selstr;
	UINT32 min, max;

	if(GetCurPos() == m_endSelected)
	{
		str1 = m_editBuff + GetCurPos();
	
		// byw -----------------------------
        if(m_editSelBuff)
			selstr = m_editSelBuff + GetCurPos() * RECOG_SEL_COUNT;
		// byw -----------------------------

		if(*str1 == 0x0d && *(str1 + 1) == 0x0a)
		{
			// byw -----------------------------
			int		templen = strlen(str1 + 2);
			// byw -----------------------------
			
			memmove(str1, str1 + 2, strlen(str1 + 2)+2);
			

			// byw -----------------------------
			if(m_editSelBuff)
				memmove(selstr, selstr + 2 * RECOG_SEL_COUNT, (templen+2) * RECOG_SEL_COUNT);
			// byw -----------------------------
		}
		else if(GetCurPos() < strlen(m_editBuff))
		{
			str2 = CharNext(str1);

			// byw -----------------------------
			int		templen = strlen(str1+(str2-str1));
			// byw -----------------------------

			memmove(str1, str1+(str2-str1), 
						strlen(str1+(str2-str1))+2);

			// byw -----------------------------
			if(m_editSelBuff)
				memmove(selstr, selstr + (str2-str1) * RECOG_SEL_COUNT,
						(templen+2) * RECOG_SEL_COUNT);
			// byw -----------------------------
		}
		else
			return;
	}
	else
	{
		min = MIN(m_endSelected, GetCurPos());
		max = MAX(m_endSelected, GetCurPos());
		str1 = m_editBuff + min;
		
		// byw -----------------------------
        if(m_editSelBuff)
			selstr = m_editSelBuff + min * RECOG_SEL_COUNT;
		int		templen = strlen(str1 + max - min);
		// byw -----------------------------

		memmove(str1, str1 + max - min, strlen(str1 + max - min)+2);
		
		// byw -----------------------------
		if(m_editSelBuff)
			memmove(selstr, selstr + (max - min) * RECOG_SEL_COUNT, (templen+2) * RECOG_SEL_COUNT);
		// byw -----------------------------

		SetCurPos( min, bRedraw);
	}
	ResetText();
	BaseCursor();
	CDC dc;
	OnPaint(dc);
}

/////////////////////////////////////////////////////////////////////
//功能    : 在当前光标后删字符
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::BackUp(BOOL bRedraw)//-
{
	char *str, *str1, *str2, *selstr1, *selstr2;
	UINT32 min, max;

	str = m_editBuff;
	str1 = str + GetCurPos();

	// byw -----------------------------
	if(m_editSelBuff)
		selstr1 = m_editSelBuff + GetCurPos() * RECOG_SEL_COUNT;
	// byw -----------------------------

	if(GetCurPos() == m_endSelected)
	{
		if(GetCurPos() >= 2 && *(str1-1) == 0x0a && *(str1-2) == 0x0d)
		{
			// byw -----------------------------
			int		templen = strlen(str1);
			// byw -----------------------------
			
			memmove(str1-2, str1, strlen(str1)+2);
			
			// byw -----------------------------
			if(m_editSelBuff)
				memmove(selstr1 - 2 * RECOG_SEL_COUNT, selstr1, (templen+2) * RECOG_SEL_COUNT);
			// byw -----------------------------

			SetCurPos(GetCurPos()-2, bRedraw);
		}
		else if(GetCurPos() > 0)
		{
			str2 = CharPre(str, str1);
			
			// byw -----------------------------
			if(m_editSelBuff)
				selstr2 = m_editSelBuff + (str2 - m_editBuff) * RECOG_SEL_COUNT;
			int		templen = strlen(str1);
			// byw -----------------------------

			memmove(str2, str1, strlen(str1)+2);
			
			// byw -----------------------------
			if(m_editSelBuff)
				memmove(selstr2, selstr1, (templen+2) * RECOG_SEL_COUNT);
			// byw -----------------------------

			SetCurPos(GetCurPos()-(str1-str2), bRedraw);
		}
		else 
			return;
	}
	else
	{
		min = MIN(m_endSelected, GetCurPos());
		max = MAX(m_endSelected, GetCurPos());
		str1 = m_editBuff + min;
		
		// byw -----------------------------
		if(m_editSelBuff)
			selstr1 = m_editSelBuff + min * RECOG_SEL_COUNT;
		int		templen = strlen(str1+max-min);
		// byw -----------------------------

		memmove(str1, str1+max-min, strlen(str1+max-min)+2);

		// byw -----------------------------
		if(m_editSelBuff)
			memmove(selstr1, selstr1 + (max-min) * RECOG_SEL_COUNT, (templen+2) * RECOG_SEL_COUNT);
		// byw -----------------------------

		SetCurPos( min,bRedraw);
	}
	ResetText();
	BaseCursor();
	//modify by wang fei 
	if(bRedraw)
       OnPaint(CDC());
}

/////////////////////////////////////////////////////////////////////
//功能    : 在当前光标插入回车
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::InsertEnter( )//-
{
	if(GetCurPos() != m_endSelected)
		DeleteChar();

	if(strlen(m_editBuff) < m_buffLen -1-1)
	{
		// byw -----------------------------
		int		templen =  strlen(m_editBuff + GetCurPos());
		// byw -----------------------------

		memmove(m_editBuff + GetCurPos()+2, m_editBuff + GetCurPos(), 
			strlen(m_editBuff + GetCurPos())+1);
		*(m_editBuff + GetCurPos()) = 0x0d;
		*(m_editBuff + GetCurPos() + 1) = 0x0a;

		// byw -----------------------------
		if(m_editSelBuff)
		{
			memmove(m_editSelBuff + (GetCurPos()+2) * RECOG_SEL_COUNT, m_editSelBuff +  GetCurPos() * RECOG_SEL_COUNT, 
				(templen+1) * RECOG_SEL_COUNT);
			memset(m_editSelBuff + GetCurPos() * RECOG_SEL_COUNT, 0, RECOG_SEL_COUNT);
			memset(m_editSelBuff + (GetCurPos() + 1)* RECOG_SEL_COUNT, 0, RECOG_SEL_COUNT);
		}
		// byw -----------------------------

		SetCurPos( GetCurPos() + 2);
		ResetText();
		BaseCursor();
		CDC dc;
		OnPaint(dc);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 重设编辑器的数据构成
//参数    : 
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CEdit::ResetText( )//+
{
	INT32			   width;
	INT32			   lines;
	UINT32 * pline;
	char		 * start;
 
	//modify by wangfei
	//width = m_sRect.right-m_sRect.left-(GetBarWid(1)+
	//					LEFTMAGIN)-RIGHTMAGIN;
	width = m_sRect.right-m_sRect.left-(GetBarWid(1)+
						LEFTMAGIN)-RIGHTMAGIN +1;
	lines = GetTotalLines(m_editBuff, width);
	if(m_lineIndex)
	{
		SysFree(m_lineIndex);
		m_lineIndex = NULL;
	}
	if(lines <=0)
		lines =1;
	m_lineIndex = (UINT32 *)SysMalloc(sizeof(UINT32)*lines + SAFELEN);
	m_lineIndex[0] =0;

	SetScrollRage(lines, 1);
	SetRowsPerPage((m_sRect.bottom-m_sRect.top-TOPMAGIN)/CHARHEIGHT, 1);

	pline = m_lineIndex;
	start = m_editBuff;

	if(!pline || !start)
		return 0;

	lines = 0;
	while(*start != 0)
	{
		pline[lines] = start - m_editBuff;
		start = GetNextLine(start, width);
		lines = lines + 1;
	}
	if(*(start -1) == 0x0a && *(start -2) == 0x0d)
		pline[lines] = start - m_editBuff;

	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 显示文本
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::PutLines( )//-
{
	CDC			   dc;
	INT32			   i, len, rage, rows, height, currentpos;
	INT32			   x, y;
	UINT32 * pline;
	char		 * start;
	char		   buff[21];

	if(!m_editBuff)
		return;

	start   = m_editBuff; 
	rage   = GetScrollRage(1)-1;

	if(GetScrollRage(1) >= 1)
	{
		//modify by wangfei
		//height = m_sRect.bottom-m_sRect.top-TOPMAGIN;  
		height = m_sRect.bottom-m_sRect.top-TOPMAGIN+1;  
		currentpos = GetScrollPos(1);
		pline = m_lineIndex;
		for(i=currentpos; (i<rage) && 
						(i<height/CHARHEIGHT + currentpos); i++)
		{
			x = m_sRect.left + LEFTMAGIN;
			y = m_sRect.top + (i - currentpos)*CHARHEIGHT + TOPMAGIN;
			memcpy(buff, start + pline[i], pline[i + 1] -pline[i]);
			buff[pline[i + 1] -pline[i]] = 0;
			if(buff[pline[i+1]-pline[i]-1] == 0x0a && 
						buff[pline[i+1]-pline[i] - 2] == 0x0d)
				buff[pline[i + 1] -pline[i] - 2] = 0;

			dc.EraseRect(x, y,
					m_sRect.right-GetBarWid(1)-RIGHTMAGIN-1, 
					y+CHARHEIGHT-1);
			dc.TextOut(x, y, buff);
		}
		if(rage < height/CHARHEIGHT + currentpos)
		{
			x = m_sRect.left + LEFTMAGIN;
			y = m_sRect.top + (rage-currentpos)*CHARHEIGHT+TOPMAGIN;
			strcpy(buff, start + pline[rage]);
			if(buff[strlen(buff)-1] == 0x0a && 
						buff[strlen(buff)-2] == 0x0d)
				buff[strlen(buff) - 2] = 0;

			dc.EraseRect(x, y, 
					m_sRect.right-GetBarWid(1)-RIGHTMAGIN-1, 
					y+CHARHEIGHT-1);
			dc.TextOut(x, y, buff);
		}
		for(i=rage-currentpos+1; i<height/CHARHEIGHT;i++)
		{
			x = m_sRect.left + LEFTMAGIN;
			y = m_sRect.top + i*CHARHEIGHT + TOPMAGIN;
			dc.EraseRect(x, y, 
					m_sRect.right-GetBarWid(1)-RIGHTMAGIN-1, 
					y+CHARHEIGHT-1);
		}
	}
	else
	{
		x = m_sRect.left + LEFTMAGIN;
		y = m_sRect.top + TOPMAGIN;
		dc.EraseRect(x, y, 
				m_sRect.right-GetBarWid(1)-RIGHTMAGIN-1, 
				y+m_sRect.Height()-2-TOPMAGIN);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 显示选中
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CEdit::DrawSelected()
{
	CDC				dc;
	INT32			cursorpos;
	UINT32			last;
	UINT32 *		pline;
	INT32			i;
	RECT			re;

	cursorpos = GetCurPos();
	pline = m_lineIndex;

	if(cursorpos != m_endSelected)
	{
		last = GetLastCharOfPage();
		if(cursorpos >= pline[GetScrollPos(1)] && cursorpos <= last)
		{
			if(cursorpos < m_endSelected)
			{
				InvertChar(cursorpos, m_endSelected);
			}
			else
			{
				if(m_endSelected >= pline[GetScrollPos(1)])
					i = m_endSelected;
				else
					i = pline[GetScrollPos(1)];
				InvertChar(i, cursorpos);
			}
		}
		else if(m_endSelected >= pline[GetScrollPos(1)] && 
										m_endSelected <= last)
		{
			if(cursorpos > m_endSelected)
				InvertChar(m_endSelected, cursorpos);
			else
				InvertChar(pline[GetScrollPos(1)], m_endSelected);
		}
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取当前页最后一个字符索引
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
UINT32 CEdit::GetLastCharOfPage()
{
	INT32			   i, height, currentpos;

	//modify by wangfei
	//height = m_sRect.bottom -m_sRect.top- TOPMAGIN;
	height = m_sRect.bottom -m_sRect.top- TOPMAGIN+1;
	currentpos = GetScrollPos(1);
	for(i=currentpos; i<GetScrollRage(1)-1 && 
					(i<=height/CHARHEIGHT + currentpos); i++);
	if(i == GetScrollRage(1)-1)
		return strlen(m_editBuff);
	else
		return m_lineIndex[i];
}



INT32 CEdit::GetFindLineNum(char *FindStr )//-
{
	
	UINT32 nTotalLine,nCourentLine;
	INT32 FindLine;
	nTotalLine   = GetScrollRage(1);
	nCourentLine=  GetScrollPos(1);

	FindLine=SearchSuoxieStr(FindStr,nCourentLine,nTotalLine );
	if(FindLine<0)
		FindLine=SearchSuoxieStr(FindStr,0,nCourentLine);
	return FindLine;
}
INT32 CEdit::GetNextFindLineNum(char *FindStr )//-
{
	
	UINT32 nTotalLine,nCourentLine;
	INT32 FindLine;
	nTotalLine   = GetScrollRage(1);
	nCourentLine=  GetScrollPos(1)+1;
	if(nCourentLine>nTotalLine)
		nCourentLine=0;

	FindLine=SearchSuoxieStr(FindStr,nCourentLine,nTotalLine );
	if(FindLine<0)
		FindLine=SearchSuoxieStr(FindStr,0,nCourentLine);
	return FindLine;
}


INT32 CEdit::SearchSuoxieStr(char *FindStr,UINT32 nBeginLine,UINT32 nEndLine )//-
{
	char * Pos,*pLineBegin;
	Pos=m_editBuff+m_lineIndex[nBeginLine];
	pLineBegin=strstr(Pos,FindStr);
	if(pLineBegin)
	{
		for(UINT32 i=nBeginLine;i<nEndLine;i++)
		{
			if((pLineBegin-m_editBuff)==m_lineIndex[i])
				return i;
			if((pLineBegin-m_editBuff)<m_lineIndex[i])
			{	
				if(i)
					return i-1;
			}
		}
	}
	return -1;
}
