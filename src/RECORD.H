#ifndef E201_RECORD_H
#define E201_RECORD_H

#include "hwfcl.h"
#include "datadrv.h"
#include "menu.h"
#include "listview.h"
#include "grid.h"
#include <string.h>
#include <stdio.h>

#include "keydrv.h"

//extern void RepeatAppRun(void);
typedef int WAVETYPE;

//////////////////////////////////////////////////////////////////////////
//
//    查询当前录音状态：
//		返回：
//			真为当前正在录音
//		    假为当前没有录音。设为否可停止录音。
//
extern  bool GetRecordStatus();	

//////////////////////////////////////////////////////////////////////////
//
//    查询当前回放录音状态：
//		返回：
//			真为当前正在回放录音
//		    假为当前没有回放录音。设为否可停止回放录音。
//
extern  bool GetReplayStatus();	

//////////////////////////////////////////////////////////////////////////
//
//    初始化录音驱动，只能运行一次！
//			
//		    
//
extern  void initRecorder(void);

//////////////////////////////////////////////////////////////////////////
//
//    录音驱动接口
//			Type  录音类型，祥见datadrv.h 
//		    RecTime 录音时长 ，单位为秒
//     返回：
//			真为录音正常结束
//			假为不能正常录音
//
extern "C" bool Record(WAVETYPE Type, INT32 RecTime=-1)  ;
//exterm bool RecordPrompt(INT32 RecTime=20);//停止，重录。回放

//////////////////////////////////////////////////////////////////////////
//
//    回放录音接口
//			Wave		放音ID，祥见datadrv.h 
//		    BreakMod	放音模式，如果当前正在放音，设真则打断当前放音
//			PlayTime	回放次数
//     返回：
//			真为放音正常结束
//			假为不能正常放音
//
extern "C" bool RePlay( WAVEHEAD* Wave,bool BreakMod=true,int PlayTime=1);

//////////////////////////////////////////////////////////////////////////
//
//    停止回放录音
//			
//			
#define CONTINUE_PLAY_MODE		true
#define SINGL_PLAY_MODE			false
#define SENDMSG_BY_HISR			true
#define SENDMSG_BY_TASK			false
extern "C" void StopPlay(bool bContinueNext=CONTINUE_PLAY_MODE,bool bActiveHisr=SENDMSG_BY_TASK);

//////////////////////////////////////////////////////////////////////////
//
//    停止录音
//			
//			
#define MSG_BY_HISR		false
#define MSG_BY_TASK		true
extern "C" void StopRec(bool bSendMsgHisr=MSG_BY_TASK);

void itos(char * dst,int num);
#define REC_COL 2 

class CRecord: public CFrameWnd
{
private:
	int * m_pBusyValue;
	char m_Title[20];										//标题
	char m_txt[20];											//内容
	char m_sBeginTime[20];									//录音开始时间
	bool m_bRec;											//开始录音标志
	bool m_bRecInScreen;									//屏保下录音
	bool m_bPrompt;											//问候语
	CDC  m_dc;
//	CButton m_butOK;		
	int  m_bRecordKey;										//录音键按下
public :
	int m_iCurSecond;										//当前记录录音时间
	int m_iType;											//录音类型
	int m_iTime;											//录音的限时时长，
	void RecordOK(UINT32 iType,UINT32 iEndStatus);			//录音结束
	BOOL RunRecord(WAVETYPE iType,int iTotalTime=-1);		//录音
	void DoPen(CPoint &pt);								
	void ResetRecord();										//录音准备
	void SetRecordEndFace(UINT32 iType,UINT32 iEndStatus);									//
	virtual void ShowWindow(UINT32 nCmdShow = SW_SHOW);
	virtual BOOL Create(char * strName,UINT32 dwStyle, const CRect &wndRect,UINT32 nHelpId);
	virtual void OnPaint(CDC &dc);
	virtual void OnDactive();
	virtual void Destroy();	
	virtual void OnRtc(UINT16 nRtcType);
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
/*public: 
	DECLARE_MESSAGE_MAP(CRecord);
	MESSAGE_HANDLE void OnBut(); */
};

#define CS_BIAOCHI		0x01
#define CS_BIAOTI		0x02
#define CS_PENMOVE		0x04
#define CS_FILECOPY		0x08

class CProcessBar: public CCtrl
{
protected:
	char m_sCaption[20];			//标题
	int m_iRange;					//范围
	int m_iMin;						//最小值
	int m_iMax;						//最大值
	int m_iStep;					//步长
	int m_iXInterval;				//单步长的X值
	int m_iCurValue;				//当前位置
	int m_iBarLeft;					//条的左X值
	int m_iStyle;					//风格
	int m_iXPlusTimes;				//几次加一（超长时）
	int m_iXCurPlus;				//当前加了几次
	ScreenBuf *  m_pbarback;		
public:
	int		GetCurValue(void);				//获取当前滑块位置
	void	SetCurValue(int iValue,bool bRedraw=false);		//设置当前滑块位置
	int		GetCurRange(void);				//获取当前进度条范围
	void	SetCurRange(int iMin,int iMax,int iStep=1);	//设置当前进度条范围
	int		GetCurStep(void);					//获取当前进度步长
	int 	GetMax(void){return m_iMax;};		//获取进度的上限
	int 	GetMin(void){return m_iMin;};		//获取进度的下限
	void	SetCtrlStyle(int iStyle);			//设置控件风格
	int		GetCtrlStyle(void);					//获取控件风格
	void	SetStep(int iStep);					//设置进度步长
	void operator ++();							//滑块右移
	void operator --();							//滑块左移
	void operator =(const int iValue);			//设置当前滑块位置
	void	MoveBar();						//重画滑块
	void DoPen(CPoint &pt)	;
public:
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);
	virtual void Destroy();
	BOOL Create(int iMin,int iMax,int iStep,CRect &ctrlRect, CFrameWnd * pParent,int iStyle, UINT32 nCtrlId);
};

static bool bCPlayOnTop=false;
class CPlay: public CFrameWnd
{
private:
	bool m_bOnEndDestroy;			//
	bool m_bRTC;					//允许计数
	long m_lRecId;					//当前选中的留言ID
	bool m_bMessage;
	CWnd * m_parent;		
	CButton m_butOK;					
	CProcessBar	m_oSlider;
public :
	char m_stitle[20];				//标题
	char m_strName[20];				//留言名字
	void plus();					//进度条右移
	bool Process(bool bOnEndDestroy);	//开始放音
	virtual void OnRtc(UINT16 nRtcType);
	virtual BOOL Create(long RecId,CWnd * parent);
	virtual void Destroy();
	virtual void OnPaint(CDC &dc);
	virtual void OnDactive();
	virtual void WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam);
public: 
	DECLARE_MESSAGE_MAP(CPlay);
	MESSAGE_HANDLE void OnBut(); //UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};

class CPromptWnd :public CFrameWnd
{
private :
	CButton m_butplay,m_butdel,m_butexit;		
	bool m_bmsgbox;					//上面窗口为消息窗口
	long m_RecId;					//当前选中的留言ID
	char m_stitle[20];				//标题
	char m_sdate[20];				//留言日期
	char m_stime[20];				//留言时间
	char m_slength[14];				//留言长度
public :
	int	 m_istatus;					//返回参数
	void DoPen(UINT32 nMessage,UINT32 wParam,UINT32 lParam);
	virtual void OnPaint(CDC &dc);
	virtual BOOL Create(char * strName, const CRect &wndRect,UINT32 nHelpId,long WaveID,WAVETYPE type=WAVE_ALL);
	virtual void WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam);
public :
	DECLARE_MESSAGE_MAP(CPromptWnd);
	MESSAGE_HANDLE void OnOK(); 
	MESSAGE_HANDLE void OnPlay();
	MESSAGE_HANDLE void OnDel(); 
};

class CRePlay: public CAppWnd
{
protected:
	int m_iRecTotal;			//当前查看录音类型总数
	int m_iCurRec;				//当前录音位置
	bool m_bDelete;				//要求删除当前留言???
	int m_iType;				//查看类型
	bool m_bCreate;				//新建
	CMenuButton m_obMenu;		
	CButton m_obDelete;
	CGrid m_grid;
	CPromptWnd  pPrompt;
	long WaveList[WAVE_MAX_COUNT];	//临时的留言ID列表
	void AppendNewLine(WAVETYPE iType,char * txt,DateTime &time) ;// int timelong)//char * strDate)
	//void AppendNewLine(WAVETYPE iType,char * txt,int timelong);//char * strDate);  //在最前添加一行
	void PlayNewRecord();
public:
	virtual BOOL Create(char * strName, WAVETYPE type=WAVE_ALL);
	void CheckStatus(int istatus);	//弹出窗口返回值
	void ReFill(int Type=WAVE_ALL,bool bCheckStatus=false);	//列表
	void DeleteSigleWave(int NumOfWave);	//删除一条录音
 	virtual void OnPaint( CDC &dc);
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void ShowWindow(UINT32 nCmdShow = SW_SHOW);
	virtual void WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam);
	DECLARE_MESSAGE_MAP(CRePlay);
	MESSAGE_HANDLE void OnGrid(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnDeleteAllRecord(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};

///
class CGreeting: public CAppWnd
{
private:
	WAVEHEAD	* m_pGreeting;
	bool m_bDirty;
	char m_iCurTime;
	char m_strStatus[60];
	BOOL m_bSliderMove;
	CProcessBar	m_oSlider;
	CButton		m_oPlay,m_oRecord;
public:
	bool FlashSaveGreeting(void);
 	virtual void OnPaint( CDC &dc);
	virtual void OnOk(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	virtual void OnCancel(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	virtual void OnRtc(UINT16 nRtcType);
	virtual BOOL OnDestroy();
	virtual BOOL OnCreate();
	DECLARE_MESSAGE_MAP(CGreeting);
	MESSAGE_HANDLE void OnPlay();
	MESSAGE_HANDLE void OnRecord();
};

#endif 