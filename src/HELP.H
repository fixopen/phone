/******************************************************************************/


#ifndef		_HELP_H_
#define		_HELP_H_


#include "listview.h"
#include "combox.h"
#include "messagebox.h"
#include "listview.h"
#include "edit.h"
#include "datadrv.h"
#include "menu.h"
#include "jishi.h"
#include "softkey.h"
class CHelpWnd : public CDialog
{
public:
	
	char *m_pHelpStr;//帮助内容
	char *m_pHelpTitle;//帮助标题
	CStatic m_HelpEdit;//显示文本框
    //取得帮助信息
	void CHelpWnd::GetHelpInfo(INT32 nHelpID);
public:
   
	BOOL		 Create(INT32 nHelpID);                    
	virtual		 BOOL OnCreate();
	
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//显示资料窗口
//-------------------------------------------------------------------
//-------------------------------------------------------------------
class CZiliaoWnd : public CAppWnd
{
private:
	CButton FindButton;
	CButton NextButton;
	char FindStr[20];
	char *m_pZiliaoTitle,*m_pZiliaoStr;
	CStatic m_ZiliaoText;//显示资料文本框
	void SetToFindStrLine();//滚动到当前查找的行

public:
	DECLARE_MESSAGE_MAP(CZiliaoWnd)
public:
	MESSAGE_HANDLE void  OnFind(UINT32 nMessage,UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void  OnNext(UINT32 nMessage,UINT32 nSrcId, UINT32 lParam);
	BOOL		 Create(UINT32 nZiliaoID);
	virtual void OnPaint(CDC &dc);
	virtual		 BOOL OnCreate();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//资料主窗口
//-------------------------------------------------------------------
//-------------------------------------------------------------------

class CZiliaoMainWnd : public CAppWnd
{
private:
	CButton m_YouBianButton;//邮编
	CButton m_QuHaoButton;//区号
	CButton m_WorldQuHaoButton;//世界区号
	CButton m_PhoneButton;//电话

	CButton m_XingZuoButton;//星座
	CButton m_XueXingButton;//血型
	CButton m_ShuXiangButton;//属相
	CButton m_HistoryButton;//历史年表
	CButton m_HolidayButton;//中外节日
	CButton m_TimeButton;//世界时差
	CZiliaoWnd *DispWnd;
public:
	DECLARE_MESSAGE_MAP(CZiliaoMainWnd)
public:
    MESSAGE_HANDLE void  OnDocSelect(UINT32 nMessage,UINT32 nDocButtonID, UINT32 lParam);
	BOOL		 Create();                    
	virtual		 BOOL OnCreate();
	virtual BOOL OnDestroy();
};

#endif

