#if !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
#define AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// telephonedlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
//#include "NoteDlg.h"
#include "../Data/ContactInfo.h"
#include "../Data/SoundSegment.h"
#include "../inc/SABTD.h"
#include "../Util/OggCodec.h"
//#define WM_CHANGELANGUAGE (WM_USER+300)

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog

class CTelephoneDlg : public CCEDialog/*CDialog*/
{
// Construction
public:
	CTelephoneDlg(CWnd* pParent = NULL);   // standard constructor

		void ShowWindow_(int nCmdShow);
// Dialog Data
	//{{AFX_DATA(CTelephoneDlg)
	enum { IDD = IDD_DIALOG_TELEPHONE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelephoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTelephoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMM_WIM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_DATA(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_CLOSE(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_DONE(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_CLOSE(UINT wParam,LONG lParam);
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnButtonTelephoneHide();
	afx_msg void OnButtonTelephoneNote();
	afx_msg void OnButtonTelephoneHandle();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnButtonTelephoneRecord();

private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticName;
	CCEStatic m_sticDynName;
	CCEStatic m_sticNumber;
	CCEStatic m_sticDynNumber;
	CCEStatic m_sticCompany;
	CCEStatic m_sticDynCompany;
	CCEStatic m_sticDuty;
	CCEStatic m_sticDynDuty;
	CCEStatic m_sticMcaller;
	CCEStatic m_sticDynMcaller;
	CCEStatic m_sticRecordCaption;
	CCEStatic m_sticRecord;
	CCEStatic m_sticDurationCaption;
	CCEStatic m_sticDuration;
	
	CCEBmpButton m_btnHide;
	CCEBmpButton m_btnNote;
	CCEBmpButton m_btnRecord;
	CCEBmpButton m_btnHandle;
	CCEBmpButton m_btnRecordTip;
	CCEFramePartStatic m_sticBackground;

//	CNoteDlg* m_pNoteDlg;
	boost::shared_ptr<Data::ContactInfo> m_spContactInfo;
	boost::shared_ptr<Data::SoundSegment> m_spSoundSegment;
	std::vector<boost::shared_ptr<Data::SoundSegment> >m_vSoundSegment;
	UINT m_uiTelephoneTimer;
	UINT m_uiTelephoneSecond;
	UINT m_uiRecordTimer;
	UINT m_uiRecordSecond;
	std::vector<std::string> m_sDeleteFile;  //待删除的录音文件
	void SaveRecAndContactInfo();

	// 用于打开铃声的定时器.有特色振铃或防火墙的情况下,平时需要关闭铃声.第一声
	// 铃声结束后第一个Ring消息发上来,开启此定时器,以保证第二声铃声到来前打开
	// 振铃.
	UINT m_uiCallIDTimeoutTimer;
	// 定时器m_uiCallIDTimeoutTimer的延迟.目前设为2.5秒(两次铃声中间的间隔为4秒).
	UINT m_uiCallIDTimeoutMS;

	// 连接是否已经建立.该变量在定时器m_uiCallIDTimeoutTimer被设置为FALSE,在
	// 连接建立后被设为TRUE,以防止定时器m_uiCallIDTimeoutTimer在连接建立后错误
	// 的打开铃声.
	BOOL m_bConnected;
	BOOL m_bAddZero;  //wzx 20101229
	UINT m_uiRingCount;
	UINT m_uiTipTimer;
	UINT m_uiInNoCount;
	UINT m_uiRecordCound;
	UINT m_uiToneDetect;
	BOOL m_bHasTDThread;
	BOOL m_bHasCallID;
	BOOL m_bKeyDial;
	// 记录CallID是否已经接收完毕,用于决定新收到的CallID后是否应当挂断当前会话.
	// 将改变量用于除此以外的目的时一定要小心:
	// 判断依据为Ring事件.因为Ring消息在响铃结束后才会发出,故可以保证下板收到的
	// CallID都已经被发上来.而Connect事件产生后由于下板仍可能继续发送之前缓存的
	// 数据,故Cannect事件后不应当设置m_bCallIDFinished.这样,如果第一声Ring之后
	// 才发送的CallID,并且在第二声铃响前接听,就会导致m_bCallIDFinished保持FALSE.
	BOOL m_bCallIDFinished;

	BOOL m_bFirwall;
	UINT m_uiRemainRecordSecond;
	std::string m_sTipFilename;
	std::string m_sRingFilename;
	std::string m_sCity;
	BOOL m_bRingPlayed;
	int m_firwallType;
private:
	volatile BOOL m_bEnding;
	BOOL m_bRecording;
	BOOL m_bPlaying;
	bool m_IsRecordNumber;
	bool m_IsTaking;
	CString m_bRecordNumber;
 	DWORD m_dwDataLength;
	HWAVEIN m_hWaveIn;
	HWAVEOUT m_hWaveOut;
	CFile m_fWav;	
	WAVEFORMATEX m_waveform;
	std::string m_sRecordFileName;
	std::string m_sPlayFileName;

	PBYTE m_pBuffer[WAVE_BUFFER_COUNT];
	PWAVEHDR m_pWaveHdr[WAVE_BUFFER_COUNT];

	bool RecStart(void);
	bool RecStop(void);
	void SaveWavHead(void);
	bool PlayStart(void);
	bool PlayStop(void);

	BOOL m_bHasLeaveWord;  //是否有留言
	BOOL m_bPlayingLeaveWord;
	bool InitData(void);
	OggCodec* m_pOggCodec;

	//设置flashdrv录音文件目录，每500个文件换个目录
	void  SetFlashdrvRecordDir(std::string &path);

public:
	CString record_str,Hang_str;
	CString m_strRecord;
	CString m_strStopRecord;
	CString m_strHangOff;
	CString m_strHangOn;
	void HangOff_(void* param);
	void HangOn_(void* param);
	void Key_(void* param);
	void SubKey_(void* param);
	void Connect_(void* param);
	void Ring_(void* param);
	void CallID_(void* param);
	std::vector<boost::shared_ptr<Data::Contact> > FindCallContact(std::string number);
	
	// 根据号码和查找到的名片更新联系信息.注意:仅仅更新控件内容,不会导致弹框.
	void ShowContact(Data::Contact * pContact, std::string number);
	// 设置窗口标题.
	void SetTitle(DWORD titleID);
	// 弹出窗口.titleID为窗口标题字符串资源,btnHandleTxt为挂断/接听按钮的文字.
	void PopupWindow(DWORD titleID, CString btnHandleTxt);

	void DialContact(Data::TelephoneNumber telephoneNumber, int contactId = 0);
	void HangOnInRecord(void);
	void HangOnToRecord(void);
	void ChangeLanguage();
	UINT GetUnconnectCount(void);
	UINT GetRecordCount(void); 
	void SetModeStr(CString s_);
	std::string StartSearch(FILE* file, const std::string& mobile, int low, int high);
	std::string ParseCode(std::string tel);
	std::string StartMobileSearch(FILE* file, const std::string& mobile, int low, int high);
	std::string ParseMobileCode(std::string mobile);
	bool isMobileNumber(std::string DialNumber);
	std::string ParseDialNumber(std::string DialNumber);
	std::string ParseTelePhoneCode(std::string tel);
	void setRecordNumber(CString number);
	void TakeLine();
	void DialTelePhoneNumber(Data::TelephoneNumber telephoneNumber,int contactId = 0);
	bool getCheckTakingState();
	void setCheckTakingstate(bool flag);

	// 打开铃声
	void OpenRing();
	// 根据指定的名片信息更新防火墙状态,NULL表示没有匹配名片.
	void UpdateFirewallStatus(Data::Contact * pContact);
	// 根据指定的名片信息和号码更新m_spContactInfo.
	void UpdateCurContactInfo(Data::Contact * pContact, std::string telNum);

	// m_spContactInfo(已经存在则直接返回).
	void CreateContactInfo(Data::ContatcInfoType type);
	CString GenerateRecordName(BOOL isHighQuality, BOOL isProtect);

	std::string GetSoundPath(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
