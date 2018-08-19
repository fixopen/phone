
/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             cedit.h                                                        */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             编辑控件类头文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-11-01   王飞修改很多地方                                  */
/******************************************************************************/
#ifndef __E401_EDIT_H_
#define __E401_EDIT_H_

#include "hwfcl.h"
#include "string.h"
#include "dc.h"    
#include "scrollwnd.h"

#define  CE_CURSOR_UP		24
#define  CE_CURSOR_DOWN		25
#define  CE_CURSOR_RIGHT	26
#define  CE_CURSOR_LEFT		27
#define  CE_DELETE_CHAR		16
#define  CE_BACK_UP			('\b')	// TODO: 值? 
#define  CE_INSERT_ENTER	('\n')	// TODO: 值?

#define  ENABLE_EDIT		1
#define  DISABLE_EDIT		0

#define  LEFTMAGIN			0		 
#define  TOPMAGIN			0        
#define  RIGHTMAGIN			0

#define  CHARHEIGHT			16
#define  CHARWIDTH			8

struct RECT
{
	int left;
	int top;
	int right;
	int bottom;
};

class CEdit : public CScrollWnd
{
private:
	unsigned char	m_type;								//类型,是否可编辑
	char		*	m_editBuff;							//文本buffer
	char		*	m_editSelBuff;						//候选字 buffer
	UINT32			m_currentCursor;					//当前光标位置
	UINT32			m_endSelected;						//当前选中结束字符
	UINT32		*	m_lineIndex;						//行索引
	INT32			m_buffLen;						
	char			m_strTitle[28];

	CWritePad    *m_pConnectedCtrl;                     //相关的手写控件 hqf

public:
		BOOL         Create(char *strName, 
							const CRect wndRect, 
							CWnd * pParent, UINT32 dwStyle, 
							UINT32 nCtrlId, INT32 iBuffLen,char * strTitle=NULL, CWritePad *pConnectedCtrl= NULL);
	virtual void Destroy();
public:
	virtual void	WindowProcess(UINT32 nMessage, 
						UINT32 wParam, UINT32 lParam);	//处理函数
	virtual void	OnClientEvent(UINT32 nMessage,		//点击客户区处理函数（由用户重载）
						UINT32 x, UINT32 y);		
	virtual void	OnPaint(CDC &dc	);					//重画函数
	BOOL			SetWindowText(const char * pText,BOOL bRedraw = TRUE);	//设置文本
	void            GetWindowText(char *pBuf, 
									UINT16 nMaxCount);	//取得文本
	BOOL			Copy(char * pTextBuff, UINT32 nBufLEn);	//拷贝选中文本
	BOOL			Paste(const char * pText);			//粘贴选中文本
	void			DrawCursor();						//画光标
	void			CursorLeft();						//光标左移处理函数
	void			CursorRight();						//光标右移处理函数
	void			CursorUp();							//光标上移处理函数
	void			CursorDown();						//光标下移处理函数
	INT32			GetFindLineNum(char *FindStr )	;	//-cxd add
	INT32			GetNextFindLineNum(char *FindStr );  //-cxd add
private:
	INT32  Init();										//初始化
	void   UnInit();									//反初始化
	INT32  ResetText( );
	INT32  GetLineOfChar(UINT32 nChar);					//取得当前字符所在行
	INT32  GetLineOfCursor();							//取得当前光标所在行
	INT32  GetMouseChar(INT32 *iLine,INT32 *iRow);		//取得鼠标点击字符偏移
	void   DoMouseDown(INT32 x, INT32 y);				//鼠标点击处理函数
	void   DoMouseMove(INT32 x, INT32 y);				//鼠标移动处理函数

	//以下函数处理完毕，光标位置改变，
	//如果要对汉字处理，可以判断光标是
	//否在汉字中间，再重新定位。
	void	ResetCursorPos();

	void	BaseCursor();								//将当前光标所在行设为当前显示行
	void	SetCurPos(UINT32 iPos,	BOOL bRedraw = TRUE);//设置光标位置
	UINT32  GetCurPos();								//取得光标位置

	//byw-------------------
	void	InsetChar(short ch, char *sel = NULL , BOOL bRedraw= TRUE);				//插入字符处理函数
	//byw-------------------

	//modify by wangfei
	//void	BackUp();									//退格处理函数
	void BackUp(BOOL bRedraw = TRUE);
	
	void	InsertEnter();								//插入回车处理函数
	
	void	DeleteChar(BOOL bRedraw = TRUE);					//删除处理函数


	void			 PutLines();						//输出文本函数

	//以下函数控制选中操作
	void			 DrawSelected();					//反向选中部分
	UINT32			 GetLastCharOfPage();				//取得当前页最后一个字符位置
	INT32			 GetCharRect(UINT32 iChar, 
										RECT * rRect);	//取得字符矩形
	void			 ResetSelEnd();						//类似ResetCursorPos
	void			 InvertChar(INT32 iFrom, 
									INT32 iEnd);		//反向一个字符
	//INT32 SearchShuziStr(char *FindStr,UINT32 nBeginLine,UINT32 nEndLine );//cxd add;
	INT32 SearchSuoxieStr(char *FindStr,UINT32 nBeginLine,UINT32 nEndLine );//-cxd add
};

//+PUBLIC
char *CharNext(const char *pCurrent);					//取得下一个字符地址，可辨认汉字
char *CharPre(const char *pStart,						//取得上一个字符地址，可辨认汉字
			  const char *pCurrent);

char *GetNextLine(const char * pSrc,					//取得下一行的地址
				  int iWidth);
int	  GetTotalLines(const char * pSrc,					//取得文本总行数
					int iWidth);
//-PUBLIC


#endif