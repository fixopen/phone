/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             pswd.h                                                         */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技密码输入窗口类的头文件                                 */
/*             HanWang Technology E-PhoneII Password Inputput Application     */
/*               class  file												  */
/*                                                                            */
/*  组    成                                                                  */
/*             密码输入窗口声明                                               */
/*                                                                            */
/*  设计说明                                                                  */
/*             略                                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             郝庆丰                                                         */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-09-05                                                     */
/*                                                                            */
/******************************************************************************/

//下面是密码输入类的声明
#ifndef  _PSWD_H_
#define  _PSWD_H_

#include "hwfcl.h"
#include "edit.h"
#include "window.h"
#include "softkey.h"

class  CPswdEdit : public CEdit
{
protected:
	BOOL    m_bShowCursor;
public:
	BOOL    Create(char *strName, 
		const CRect wndRect, 
		CWnd * pParent, UINT32 dwStyle, 
		UINT32 nCtrlId, INT32 iBuffLen,char * strTitle=NULL, CWritePad *pConnectedCtrl= NULL);

	virtual void OnClientEvent(UINT32 nMessage , UINT32 x , UINT32 y)
	{
//		CEdit::OnClientEvent(nMessage, x, y);
		if(nMessage != WM_DACTIVE)
			DrawCursor();
		if(nMessage == WM_PENUP)
			SendNotifyMessage(CN_COMMAND, m_nCtrlId);
	}
	virtual void OnPaint(CDC &dc);
	void    SetCursorStyle(BOOL bShowCur = TRUE)
	{
		m_bShowCursor = bShowCur;
	}
};

#define  SHIFT_KEY            12
#define  SET_PSWD             0
#define  INPUT_PSWD           1
void SetKeyRectValues(CRect rectArea, CRect *pRectArray, UINT8 num, UINT8 flag);
class CPsWdInput : public CDialog
{
public:
	CPsWdInput(){};
	virtual ~CPsWdInput(){};
protected:
	CPswdEdit m_oEdit1, m_oEdit2, *m_pCurEdit;
	CIconButton m_oBtnOk;// , m_oBtnCancel;
	CSoftKey m_oLowKey;
	char  m_cFlag;
	
	virtual BOOL OnCreate();
	char m_pPswd1[9] , m_pPswd2[9];
	char m_strOrigPswd[9];
	virtual void OnOk();
public:
	virtual void OnPaint(CDC &dc);
	virtual BOOL Create(char *strName , char *strOrigPswd, UINT32 dwStyle , const CRect &wndRect,
		     UINT32 nHelpId=HELP_INPUT_PASSWORD , char flag =  SET_PSWD);
	virtual void OnActive();
	virtual void OnDactive();

	
	DECLARE_MESSAGE_MAP()
	MESSAGE_HANDLE void OnSoftKey(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnEditClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	
	char m_strPswd1[9] , m_strPswd2[9];
};

UINT32  InputPassword(char *passwordBuf , short nBufLen , char flag, 
					  char *strCaption = NULL//密码输入框的标题，默认是“请输入密码”
					  );
//调用此函数直接打开密码输入对话框，passwordBuf是存放密码的缓存，
//nBufLen是缓存长度，flag 是标记， 为0表示设置密码（输两次),为1表示
//输入密码。返回IDOK时passwordBuf值才有效，否则无效。
#endif