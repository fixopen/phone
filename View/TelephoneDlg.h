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
	std::vector<std::string> m_sDeleteFile;  //��ɾ����¼���ļ�
	void SaveRecAndContactInfo();

	// ���ڴ������Ķ�ʱ��.����ɫ��������ǽ�������,ƽʱ��Ҫ�ر�����.��һ��
	// �����������һ��Ring��Ϣ������,�����˶�ʱ��,�Ա�֤�ڶ�����������ǰ��
	// ����.
	UINT m_uiCallIDTimeoutTimer;
	// ��ʱ��m_uiCallIDTimeoutTimer���ӳ�.Ŀǰ��Ϊ2.5��(���������м�ļ��Ϊ4��).
	UINT m_uiCallIDTimeoutMS;

	// �����Ƿ��Ѿ�����.�ñ����ڶ�ʱ��m_uiCallIDTimeoutTimer������ΪFALSE,��
	// ���ӽ�������ΪTRUE,�Է�ֹ��ʱ��m_uiCallIDTimeoutTimer�����ӽ��������
	// �Ĵ�����.
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
	// ��¼CallID�Ƿ��Ѿ��������,���ھ������յ���CallID���Ƿ�Ӧ���Ҷϵ�ǰ�Ự.
	// ���ı������ڳ��������Ŀ��ʱһ��ҪС��:
	// �ж�����ΪRing�¼�.��ΪRing��Ϣ�����������Żᷢ��,�ʿ��Ա�֤�°��յ���
	// CallID���Ѿ���������.��Connect�¼������������°��Կ��ܼ�������֮ǰ�����
	// ����,��Cannect�¼���Ӧ������m_bCallIDFinished.����,�����һ��Ring֮��
	// �ŷ��͵�CallID,�����ڵڶ�������ǰ����,�ͻᵼ��m_bCallIDFinished����FALSE.
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

	BOOL m_bHasLeaveWord;  //�Ƿ�������
	BOOL m_bPlayingLeaveWord;
	bool InitData(void);
	OggCodec* m_pOggCodec;

	//����flashdrv¼���ļ�Ŀ¼��ÿ500���ļ�����Ŀ¼
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
	
	// ���ݺ���Ͳ��ҵ�����Ƭ������ϵ��Ϣ.ע��:�������¿ؼ�����,���ᵼ�µ���.
	void ShowContact(Data::Contact * pContact, std::string number);
	// ���ô��ڱ���.
	void SetTitle(DWORD titleID);
	// ��������.titleIDΪ���ڱ����ַ�����Դ,btnHandleTxtΪ�Ҷ�/������ť������.
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

	// ������
	void OpenRing();
	// ����ָ������Ƭ��Ϣ���·���ǽ״̬,NULL��ʾû��ƥ����Ƭ.
	void UpdateFirewallStatus(Data::Contact * pContact);
	// ����ָ������Ƭ��Ϣ�ͺ������m_spContactInfo.
	void UpdateCurContactInfo(Data::Contact * pContact, std::string telNum);

	// m_spContactInfo(�Ѿ�������ֱ�ӷ���).
	void CreateContactInfo(Data::ContatcInfoType type);
	CString GenerateRecordName(BOOL isHighQuality, BOOL isProtect);

	std::string GetSoundPath(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
