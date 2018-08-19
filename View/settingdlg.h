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
class CSettingDlg : public CDialog
{
// Construction
public:
	IPInfo			m_ip;
	void GetIPInfo();
	IPInfo GetIPInfo(char *AdapterName);
	void SetDiskInFo();
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
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSettingOk();
	afx_msg void OnButtonSettingCancel();
	afx_msg void OnButtonSettingRecord();
	afx_msg void OnButtonSettingDefineFastDial();
	afx_msg void OnAdjustTouchPanel();
	afx_msg void OnSettingSaveScreenPassword();
	afx_msg void OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l);
	afx_msg void OnSettingClearContact();
	afx_msg void OnSettingClearContactInfo();
	afx_msg void OnSettingClearSound();
	afx_msg void OnSettingClearInfo();
	
	afx_msg void OnBackup();
	afx_msg void OnRestore();
	afx_msg void OnFileOut();

	afx_msg void OnRingLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStaticClick(WPARAM w, LPARAM l);
	afx_msg void OnRingSelect();
	afx_msg void OnPaint();

	/*
	afx_msg void OnDeleteFile();
	afx_msg void OnAllSelect();
	afx_msg void OnClearDelect();
	afx_msg void OnFileIn();
	afx_msg void OnFileOut();
	afx_msg void OnFileSelect(WPARAM w, LPARAM l);
	afx_msg void OnLocalLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUSBLst(NMHDR* pNMHDR, LRESULT* pResult);
	*/
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnDeleteTipOk(WPARAM w, LPARAM l);
	afx_msg void OnCheckPWD(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonSettingDefault();
	void SetSettingDefault();

	CMJPGStatic		m_MJPGList;
	CFont m_Font;
	unsigned int m_uiType;

// 	CCEStatic m_sticTypeTitle;
// 	CCEStatic m_sticDetailTitle;
// 	CCEStatic m_sticOperation;
// 	CCEStatic m_sticSepLine1;
// 	CCEStatic m_sticSepLine2;
	//CCEListCtrl	m_lsType;
	CImageList* m_pImageList, *m_pImageList1;
// 	CCEBmpButton m_btnOk;
// 	CCEBmpButton m_btnCancel;
// 	CCEBmpButton m_btnDefault;

	//tel
//	CCEStatic m_sticLocalAreaNumber;
	CCERectEdit m_edtLocalAreaNumber;
//	CCEButtonST m_chbEnableOutLine;			//3g
//	CCERectEdit m_edtOutLine;				//3g
	CCEButtonST m_chbEnableIPDial;
	CCERectEdit m_edtIPDial;
//	CCEButtonST m_chbEnableRing;			//3g
//	CCEStatic m_sticDefaultRing;
//	CCEComboBox_ m_cmbRing;					//3g
	//铃声选择由Combox改成ListCtrl
	/*
	CCERectEdit m_ringEdit;
	CCEBmpButton m_ringButton;
	CCEListCtrl m_ringList;
 	CCEStatic m_ringStatic;
	*/
	TCHAR	  m_ringDir[128];
	void SetRingLst(TCHAR *dir);

	CCEButtonST m_chbEnableAutoLeaveWord;
//	CCEStatic m_sticRingTimes;
	CCEComboBox_ m_cmbRingTimes;
	CCEComboBox_ m_cmbAutoRecoedeTimes;

//	CCEStatic m_sticAutoLeaveWord;

//	CCEComboBox_ m_cmbAutoLeaveWordTip;

// 	CCEBmpButton m_btnRecardLeaveWordTip;
// 	CCEBmpButton m_btnPlayLeaveWordTip;
// 	CCEBmpButton m_btnDefineSpeedButton;
// 	CCEStatic m_sticDefineSpeedButton;


	//browser
	CCERadioButton m_rdoConnectAuto;
	CCERadioButton m_rdoConnectADSL;
// 	CCEStatic m_sticADSLName;
// 	CCEStatic m_sticADSLPassword;
	CCERectEdit m_edtADSLName;
	CCERectEdit m_edtADSLPassword;	
	CCERadioButton m_rdoGetIPDHCP;
	CCERadioButton m_rdoGetIPStatic;
// 	CCEStatic m_sticIP;
// 	CCEStatic m_sticMask;
// 	CCEStatic m_sticGateway;
// 	CCEStatic m_sticDNS;
	CCERectEdit m_edtIP[4];
	CCERectEdit m_edtMask[4];
	CCERectEdit m_edtGateway[4];
	CCERectEdit m_edtDNS[4];

	//systom
	CCEButtonST m_chbEnnabeScreenProtect;
//	CCEStatic m_sticScreenProtctType;
	CCERadioButton m_rdoTimeScreen;
	CCERadioButton m_rdoImageScreen;

//	CCEStatic m_sticWaitTime;
	CCEComboBox_ m_cmbWaitTime;
	CCEButtonST m_chbEnablePassword;
//	CCEBmpButton m_btnSetPassword;
//	CCEStatic m_sticTouchAdjust;
//	CCEBmpButton m_btnTouchAdjust;

	//录音文件存储位置
//	CCEStatic m_sticSoundSavePath;
	CCEComboBox_ m_cmbSoundSavePath;

	//时间日期设置
//	CCEStatic     m_stcTime;
	CDateCtrl m_dtTime;
	CDateCtrl m_dtDate;
	SYSTEMTIME m_curtime;

	//info
//	CCEStatic m_sticSystomTitle;

//	CCEComboBox_ m_cmbRingVolume;		//3g
	CCEComboBox_ m_cmbSystemVolume;

// 	CCEStatic m_sticSoftwareVersion1;
// 	CCEStatic m_sticHardwareVersion1;
// 	CCEStatic m_sticMemorySize1;
// 	CCEStatic m_sticStorageSize1;
// 	CCEStatic m_sticContactSize1;
// 	CCEStatic m_sticContactInfoSize1;
// 	CCEStatic m_sticSoundSize1;
// 	CCEStatic m_sticInformationSize1;
// 	CCEStatic m_sticVersionRight1;
	
	CCEStatic m_sticSoftwareVersion;
	CEdit	  m_editVersion;
//	CCEStatic m_sticHardwareVersion;
	CCEStatic m_sticMemorySize;
	CCEStatic m_sticStorageSize;
	CCEStatic m_sticContactSize;
	CCEStatic m_sticContactInfoSize;
	CCEStatic m_sticSoundSize;
	CCEStatic m_sticInformationSize;
	CCEStatic m_sticVersionRight;

	//blacklight
	CCEButtonST m_chbContrlBlackLight;
	CCEComboBox_ m_cmbContrlBlackLightWaitTime;
	CCEButtonST m_chbNightContrlBlackLight;
	CDateCtrl m_dtNightContrlBlackLightStartTime;
	CDateCtrl m_dtNightContrlBlackLightEndTime;

	//录音设置
//	CCEButtonST m_chbSettingPlayPassWord;		//放音密码
//	CCEButtonST m_chbSettingSupperPassWord;		//超级密码
	CCERadioButton m_rdoAutoRecord;				//自动录音
	CCERadioButton m_rdoManulRecord;			//手动录音
//	CCERectEdit m_editPlayPassWord;				//显示放音密码	
//	CCERectEdit m_editSupperPassWord;			//显示超级密码
	CCERadioButton m_recordMiddle;			//录音质量标准
	CCERadioButton m_recordHight;			//手动录音


//	CCEBmpButton m_btnClearContact;
//	CCEBmpButton m_btnClearContactInfo;
//	CCEBmpButton m_btnClearSound;
//	CCEBmpButton m_btnClearInformation;

	//数据导入导出
// 	CCEStatic m_stcBackUP;//, m_stcReStore;
// 	CCEBmpButton m_btnBackUP, m_btnReStore;
// 	CCEStatic m_stcTitle;
// 	CCEBmpButton m_btnOut, m_btnDelete;


	/*
	CCEComboBox_ m_cmbTitle;
	CCEListCtrl m_lstLocal, m_lstUSB;
	CCEBmpButton m_btnIn, m_btnOut, m_btnDelete, m_btnAllSelect, m_btnClearSelect;
	CCEStatic m_stcLocalTitle, m_stcUSBTitle;
	CCEStatic m_stcLocal, m_stcUSB;
	CCEFrameStatic m_sticBackground;
	TCHAR		   m_usbDir[128];
	TCHAR		   m_localDir[128];
	int			   m_nListSelect;
	FileOperaterType	   m_nFileOperaterType;			//振铃音乐 0 电子相册 1 MP3音乐 2 视频文件 3 录音文件 4
	void SetLocalLst(TCHAR *dir);
	void SetLocalLst();
	*/

	CCEStatic m_sticPanel;
	CCEStatic m_sticPanel2;
	CCEFrameStatic m_sticBackground;

	CFastDialsDlg* m_pFastDialsDlg;
	CCallSetDlg *m_pCallSetDlg;
	CPinSetDlg *m_pPinSetDlg;
	CRingSetDlg *m_pRingSetDlg;
	CSpecCodeDlg *m_pSpecCodeDlg;

	CPasswordDlg*  m_passwordDlg;
	int m_iDeleteType;

	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	void ShowConfigItems(void);
	void IniCtrlData();
	void SetIPConfig(NETWORK_ADPT_INFO& AdptInfo);
	void SetShowTimer();
	
	afx_msg void OnStopTryRing(WPARAM w, LPARAM l);
	afx_msg void OnBtnSTClick(WPARAM w, LPARAM l);
	void StopTryRing();

public:
	bool m_bLogin;
	void UpdateTips(void);

	copyfileDlg*	m_copyfileDlg;
//	CFireWallDlg*   m_firWallDlg;
	boost::shared_ptr<Data::Setting> m_pSetting;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
