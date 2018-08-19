//------------------------------------------------------------------
//------------------------------------------------------------------
//修改纪录
//2001/12/19 程晓东增加OnDestroy();解决返回时钟时时钟停止问题
//	byw, 2001/12/25, modify HelpAppRun()
//------------------------------------------------------------------
//------------------------------------------------------------------
#include "help.h"
#include "pointrect.h" 
//主窗口指针

static CZiliaoMainWnd *g_pZiliaoApp=NULL;
#define ID_HELPEDIT 4000//显示文本框ID



struct HelpStrStruct  //帮助结构
{
	INT32 nHelpId;//帮助id 
	char * szHelpTitle;//帮助标题
	char * szHelpStr;//帮助内容 
};
BOOL Help_GetHelpInfo(INT32 nHelpID,char **pHelpTitle,char **pHelpStr)
{
	INT32 *pHelpID;
	
	pHelpID=(INT32 *)HELPADDRESS;

	//缺省处理，为缺省帮助
	*pHelpTitle=(char *)(pHelpID[1]+HELPADDRESS);
	*pHelpStr=(char*)(*pHelpTitle+strlen(*pHelpTitle)+1);
	for(int i=0;i<512;i++)//查找该帮助
	{
		if(*pHelpID==nHelpID)
		{
			*pHelpTitle=(char *)(pHelpID[1]+HELPADDRESS);
			*pHelpStr=(char*)(*pHelpTitle+strlen(*pHelpTitle)+1);
			return TRUE;
		}
		pHelpID+=2;
	}
	return FALSE;
}

 
//取得帮助信息
void CHelpWnd::GetHelpInfo(INT32 nHelpID)
{
	Help_GetHelpInfo(nHelpID,&m_pHelpTitle,&m_pHelpStr);
}			
			
/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CHelpWnd::Create(INT32 nHelpID)
{
	GetHelpInfo(nHelpID);

	if(!CDialog::Create(m_pHelpTitle, WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, TRUE,0))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CHelpWnd::OnCreate()
{
	if(!CDialog::OnCreate())
		return FALSE;
	//创建浏览器
	m_HelpEdit.Create( m_pHelpStr, 
					   CRect( m_sRect.left+1, 
						      m_sRect.top+WND_TITLE_HEIGHT+5, 
					          m_sRect.right, m_sRect.bottom
							 ), 
					   this, WS_VISIBLE, ID_HELPEDIT, 
					   strlen(m_pHelpStr)+1
					 );
	return TRUE;
}


/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
//                                                                 //
//                以下为帮助程序的定义和实现                 //
//                                                                 //
//                                                                 //
/////////////////////////////////////////////////////////////////////
//CHelpApp-------------------------------------------------------
void ReferenceAppRun();
/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void HelpAppRun(INT32 nHelpID)
{
	/*
	*	Illegal help id
	*/	
	if (nHelpID < DOC_ID_START || nHelpID > DOC_ID_END)
		return;
	CHelpWnd   HelpApp;
	HelpApp.Create(nHelpID);
	HelpApp.DoModal();
	
}

//
#define ID_ZILIAOTEXT    5000
#define IDB_ZILIAOVIEW_FIND  5001
#define IDB_ZILIAOVIEW_NEXT  5002
/*

#define ID_ZILIAO_PHONE  5003 //电话
#define ID_ZILIAO_YOUBIAN 5004//邮编
#define ID_ZILIAO_QUHAO 5005 //区号
#define ID_ZILIAO_XUEXING 5005 //血型
#define ID_ZILIAO_SHUXIANG 5005 //属相
#define ID_ZILIAO_HISITROY 5005 //历史年表
#define ID_ZILIAO_HOLIDAY 5005 //中外节日
#define ID_ZILIAO_XINGZUO 5005 //星座
*/
/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CZiliaoWnd::Create(UINT32 nZiliaoID)
{
	m_pZiliaoTitle=NULL;
	m_pZiliaoStr=NULL;
	memset(FindStr,0x00,20);
	if(Help_GetHelpInfo(nZiliaoID,&m_pZiliaoTitle,&m_pZiliaoStr)==FALSE)
		return FALSE;
	if(!CAppWnd::Create(m_pZiliaoTitle, WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, HELP_DOCUMENT))
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CZiliaoWnd::OnCreate()
{
	if(CAppWnd::OnCreate()==FALSE)
		return FALSE;
	m_ZiliaoText.Create( m_pZiliaoStr, 
					   CRect( m_sRect.left+1, 
						      m_sRect.top+WND_TITLE_HEIGHT+5, 
					          m_sRect.right, m_sRect.bottom
							 ),
					   this, WS_VISIBLE, ID_ZILIAOTEXT, 
					   strlen(m_pZiliaoStr)+1
					 );
	FindButton.Create("查找",CRect(  m_sRect.left+57, 
							    m_sRect.top, 
							    m_sRect.left+90, 
							    m_sRect.top+WND_TITLE_HEIGHT-1
							),
						this,IDB_ZILIAOVIEW_FIND);
	NextButton.Create("下一个",CRect(  m_sRect.left+91, 
							    m_sRect.top, 
							    m_sRect.left+141, 
							    m_sRect.top+WND_TITLE_HEIGHT-1
							),
						this,IDB_ZILIAOVIEW_NEXT);
	m_nHelpId = HELP_DOCUMENT_DETAIL; // hqf
	return TRUE;
}
/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CZiliaoWnd::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
}

BEGIN_MESSAGE_MAP(CZiliaoWnd,CAppWnd)
ON_MESSAGE(IDB_ZILIAOVIEW_FIND, CN_COMMAND , OnFind)
ON_MESSAGE(IDB_ZILIAOVIEW_NEXT, CN_COMMAND , OnNext)
END_MESSAGE_MAP 


//滚动到当前查找的行
void CZiliaoWnd::SetToFindStrLine()
{
	INT32 nFindLine;
	nFindLine=m_ZiliaoText.GetFindLineNum(FindStr);
	if(nFindLine>=0)
	{
		m_ZiliaoText.SetScrollPos(nFindLine, 1);
		m_ZiliaoText.OnPaint(CDC());
	}
	else
		::MessageBox("没有符合条件的内容！","查找", MB_OK|MB_ICONINFORMATION);
}
void  CZiliaoWnd::OnFind(UINT32 nMessage,UINT32 nSrcId, UINT32 lParam)
{
	char szFindStr[20];
	memset(szFindStr,0x00,20);
	if(InputBox((char *)"查找", (char *)szFindStr, 10, INIT_WRITE ,TRUE)==IDOK)
	{
		
		strcpy(FindStr,szFindStr);
		if(strlen(FindStr)==0)
		{
			::MessageBox("查找内容不能为空！","查找", MB_OK|MB_ICONSTOP);
			return;
		}
		//查找定位
		SetToFindStrLine();
	}
	
}
void  CZiliaoWnd::OnNext(UINT32 nMessage,UINT32 nSrcId, UINT32 lParam)
{
	//查找定位
	INT32 nFindLine;
	if(strlen(FindStr)==0)
	{
		::MessageBox("查找内容不能为空！","查找",MB_OK|MB_ICONSTOP);
		return;
	}
	nFindLine=m_ZiliaoText.GetNextFindLineNum(FindStr);
	if(nFindLine>=0)
	{
		m_ZiliaoText.SetScrollPos(nFindLine, 1);
		m_ZiliaoText.OnPaint(CDC());
	}
	else
		::MessageBox("没有符合条件的内容！","查找",MB_OK|MB_ICONINFORMATION);
}
/////////////////////////////////////////////////////////////////////
//
//资料主窗口
//
/////////////////////////////////////////////////////////////////////

BOOL CZiliaoMainWnd::Create()
{
	DispWnd=NULL;
	if(!CAppWnd::Create("资料", WS_CAPTION |WS_VISIBLE, 
		DESK_TOP_RECT, HELP_DOCUMENT))
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CZiliaoMainWnd::OnCreate()
{
	
	if(CAppWnd::OnCreate()==FALSE)
		return FALSE;
	m_HolidayButton.Create((char*)"节日",CRect(8,30,75,50),this,ID_DOC_HOLIDAY);
	m_ShuXiangButton.Create((char*)"属相",CRect(8,55,75,75),this,ID_DOC_SHUXIANG);
	m_PhoneButton.Create((char*)"特服号码",CRect(8,80,75,100),this,ID_DOC_PHONE);
	m_QuHaoButton.Create((char*)"国内区号",CRect(8,105,75,125),this,ID_DOC_QUHAO);

	m_XingZuoButton.Create((char*)"星座",CRect(85,30,152,50),this,ID_DOC_XINGZUO);
	m_XueXingButton.Create((char*)"血型",CRect(85,55,152,75),this,ID_DOC_XUEXING);
	m_YouBianButton.Create((char*)"邮政编码",CRect(85,80,152,100),this,ID_DOC_YOUBIAN);
	m_WorldQuHaoButton.Create((char*)"国际区号",CRect(85,105,152,125),this,ID_DOC_WORLDAREA);	
	
	m_HistoryButton.Create((char*)"历史年表",CRect(8,130,75,150),this,ID_DOC_HISTORY);
	m_TimeButton.Create((char*)"世界时差",CRect(85,130,152,150),this,ID_DOC_WORLDTIME);
	return TRUE;
	
}

BEGIN_MESSAGE_MAP(CZiliaoMainWnd,CAppWnd)
ON_MESSAGE_RANGE(ID_DOC_YOUBIAN,ID_DOC_WORLDTIME, CN_COMMAND, OnDocSelect)
END_MESSAGE_MAP 

void  CZiliaoMainWnd::OnDocSelect(UINT32 nMessage,UINT32 nDocButtonID, UINT32 lParam)
{
	if(nDocButtonID>=ID_DOC_YOUBIAN&&nDocButtonID<=ID_DOC_WORLDTIME)
	{
		if (nDocButtonID == ID_DOC_YOUBIAN){
			CDC dc;
			CRect r2(30,74,112,94);
			dc.EraseRect(r2);
			dc.FrameRect(r2);
			dc.TextOut(32,76,"载入中...");
		}			
		if(DispWnd==NULL)
			DispWnd= new CZiliaoWnd;
		if(DispWnd->Create(nDocButtonID)==TRUE)
			DispWnd->ShowWindow();
	}
}

BOOL CZiliaoMainWnd::OnDestroy()
{
	if(DispWnd)
	{
		delete DispWnd;
		DispWnd=NULL;
	}
	CAppWnd::OnDestroy();
	::SetCurrentApp();
	return TRUE;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//运行资料
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void ReferenceAppRun()
{
	if(!g_pZiliaoApp)
		g_pZiliaoApp = new CZiliaoMainWnd;
	::SetCurrentApp(APPID_DOCUMENT);
	g_pZiliaoApp->Create();
	g_pZiliaoApp->ShowWindow();
}
