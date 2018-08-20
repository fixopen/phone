#if !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
#define AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CECombobox_.h"
#include "../Control/CERadioButton.h"
#include "../Data/Setting.h"
#include "fastdialsdlg.h"
#include "CallSetDlg.h"
#include "PinSetDlg.h"
#include "RingSetDlg.h"
#include "SpecCodeDlg.h"
#include "passwordDlg.h"
#include "copyfileDlg.h"
#include "fireWallDlg.h"
#include "TipDlg.h"
#include "NewApnTypeDlg.h"

#include "CUPLMNDlg.h"
#include "PSTNNumberDlg.h"
#include "TestATDlg.h"

#include "../control/MJPGStatic.h"
#include "../control/DateCtrl.h"
#include "../control/CEDialog.h"

// CSettingDlg dialog
struct IPInfo
{
	BOOL isDHCP;
	CString ipAddress;
	CString mask;
	CString gw;
	CString dns;
};

typedef struct _NETWORK_ADPT_INFO {
	LPTSTR	szAdapterName;
	BOOL	fUseDHCP;
	BOOL	fSaved;
	wchar_t	IPAddr[20];
	wchar_t	SubnetMask[20];
	wchar_t	Gateway[20];
	wchar_t	DNSAddr[20];
	wchar_t	DNSAltAddr[20];
	wchar_t	WINSAddr[20];
	wchar_t	WINSAltAddr[20];
	wchar_t	szDisplayName[256];
	} NETWORK_ADPT_INFO, *PNETWORK_ADPT_INFO;

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

enum FileOperaterType{RING_TYPE, PHOTO_TYPE, MP3_TYPE, VIDEO_TYPE, RECORDE_TYPE};
#define  SPEED_DIAL_NUM 4

class CSettingDlg : public CDialog
{
// Construction
public:
	IPInfo			m_ip;
	void GetIPInfo();
	IPInfo GetIPInfo(char *AdapterName);
	void SetDiskInFo();
	void SetPageTab(int type);
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_DIALOG_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSettingCancel();
	afx_msg void OnAdjustTouchPanel();
	afx_msg void OnSettingSaveScreenPassword();
	afx_msg void OnSettingAdminPassword();
	
	afx_msg void OnBackup();
	afx_msg void OnRestore();
	afx_msg void OnFileOut();
	//速拨设置
	afx_msg void OnButtonFastDialsOk();
	afx_msg void OnButtonFastDialsCancel();

	afx_msg void OnStaticClick(WPARAM w, LPARAM l);
	afx_msg void OnComboSelect(WPARAM w, LPARAM l);
	afx_msg void OnPaint();
	afx_msg void OnCheckPWD(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonSetting();
	void SetSettingDefault();
	void SetSettingOK();
	bool DeleteStartFlag();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);

	CMJPGStatic		m_MJPGList;
	CFont m_Font;
	unsigned int m_uiType;
	unsigned int m_clickType;
	unsigned int m_iDeleteType;
	CImageList* m_pImageList, *m_pImageList1;
	CCERectEdit m_edtLocalAreaNumber;
	CCERectEdit m_edtOutLineDial1;
	TCHAR	  m_ringDir[128];
	CCEComboBox12_ m_cmbRingTimes;
	CCEComboBox12_ m_cmbAutoRecoedeTimes;
	CCEComboBox12_ m_cmbCallTransfer;

	//browser
	CCERectEdit m_edtADSLName;
	CCERectEdit m_edtADSLPassword;	
	CCERectEdit m_edtIP[4];
	CCERectEdit m_edtMask[4];
	CCERectEdit m_edtGateway[4];
	CCERectEdit m_edtDNS[4];

	//systom
	CCEComboBox12_ m_cmbWaitTime;

	//强制收网
	CCERadioButton m_Radio1;
	CCERadioButton m_Radio2;
	CCERadioButton m_Radio3;
	CCERadioButton m_Radio4;
	CButton  m_SetDDMT;

	//时间日期设置
	CDateCtrl m_dtTime;
	CDateCtrl m_dtDate;
	SYSTEMTIME m_curtime;

	//blacklight
	CCEComboBox12_ m_cmbBlackLightWaitTime;
	CCEComboBox12_ m_cmbNightBlackLightStartTime;
	CCEComboBox12_ m_cmbNightBlackLightEndTime;

	CCEStatic m_sticPanel;
	CCEStatic m_sticPanel2;
	CCEFrameStatic m_sticBackground;

	CPinSetDlg *m_pPinSetDlg;
	CRingSetDlg *m_pRingSetDlg;
	CTipDlg *m_pTipDlg;
	CPasswordDlg*  m_passwordDlg;
	CCallSetDlg *m_pCallSetDlg;

	//mms 设置 add by qi 20100515
	CCERectEdit m_mmsC;
	CCERectEdit m_apnGW[4];//彩信的IP
	CCERectEdit	m_apnPort;//彩信的端口
	CCERectEdit	m_apnDot;//拨号电话
	CCERectEdit	m_apnUser;//用户
	CCERectEdit m_apnPwd;//密码
	CCEComboBox12_ m_mmsType;//彩信的链接方式
	CCEComboBox12_ m_apnType;//链接方式

	CNewApnTypeDlg *m_pNewApnTypeDlg;//输入APN的名字
	CUPLMNDlg *m_pUPLNDlg;//SIM卡的网络运营商列表
	CPSTNNumberDlg *m_pPstnNumberDlg;//固话列表
	
	void SetApn(int type);
	void GetApn();
	//

	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	void ShowConfigItems(void);
	void ShowWindow_(int nCmdShow);
	void IniCtrlData();
	void SetIPConfig(NETWORK_ADPT_INFO& AdptInfo);
	void SetShowTimer();
	void SetBlackLightParam(boost::shared_ptr<Data::Setting> data);
	void SetSyetemParam(boost::shared_ptr<Data::Setting> data);
	
	afx_msg void OnStopTryRing(WPARAM w, LPARAM l);
	afx_msg void OnBtnSTClick(WPARAM w, LPARAM l);
	afx_msg void OnBtnSetDDTM(WPARAM w, LPARAM l);

//	afx_msg void OnSelectApn(WPARAM w, LPARAM l);

	void StopTryRing();

public:
	bool m_bLogin;
	bool m_bPasswordModify;
	void UpdateTips(void);
	//速拨 特服号
	void SetFastDialParam(boost::shared_ptr<Data::Setting> data);
	CCERectEdit m_edtFastDialNumber[12];
	CCERectEdit m_edtFastDialName[12];
	HWND m_curEditWnd;
	int  m_editFocusIndex;
	int m_edtShowNumber;
	void OnButtonSpacodeOk();
	void SetSpecCodeParam(boost::shared_ptr<Data::Setting> data);
	void DeleteFastDialNum(boost::shared_ptr<Data::Setting> data);
	void OnBtnDelFastDial();

	//防火墙
	CCEComboBox12_ m_cmbTime;
	void OnButtonFireWallOk();
	void SetFirewallParam(bool enable, int type, int duration);
	int  GetFirewallType();
	void BrushWindow(void);
	//铃声
	CCERectEdit m_edtRingNumber[3];
	CCEComboBox12_ m_cmbRing;
	CCEComboBox12_ m_cmbSMSRing;
	CCEComboBox12_ m_cmbSpecRing;

	//延时拨号
	CCEComboBox12_ m_cmbPhoneDialTimes;//电话延时拨打时间表
	void PhoneDialTimes();//延时拨号

	int m_nphoneVolume;
	int m_nsmsVolume;
	int m_nspecialVolume;
	void OnButtonRingOk();
	void SetRingSetParam(boost::shared_ptr<Data::Setting> data);
	void SetRingLst(TCHAR *dir);
	void ChangeVolume(int w, const int type);
	//呼叫设置
	void OnButtonCallOk();
	void SetCallSetParam(boost::shared_ptr<Data::Setting> data);

	copyfileDlg*	m_copyfileDlg;
	boost::shared_ptr<Data::Setting> m_pSetting;
	boost::shared_ptr<Data::Setting> m_pTempSetting;
	
	bool m_bManualSearchNet;
	uchar m_nFDstatus;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
