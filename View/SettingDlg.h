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
#include "passwordDlg.h"
#include "copyfileDlg.h"
#include "..\Util\OggCodec.h"
#include "regrecordsound.h"


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
    CSettingDlg(CWnd* pParent =NULL);
	                    // standard constructor
    
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
	//afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnButtonSettingOk();
	afx_msg void OnButtonSettingLogin();
	afx_msg void OnButtonSettingCancel();
	afx_msg void OnButtonSettingDefault();
	afx_msg void OnButtonSettingPlayRing();
	afx_msg void OnButtonSettingListen ();
	afx_msg void OnButtonSettingDefineFastDial();
	afx_msg void OnAdjustTouchPanel();
	afx_msg void OnSetPlayPassword();
	afx_msg void OnSetAdminPassword();
	afx_msg void OnSettingSaveScreenPassword();
	afx_msg void OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l);
	afx_msg void OnSettingClearContact();
	afx_msg void OnSettingClearContactInfo();
	afx_msg void OnSettingClearSound();
	afx_msg void OnSettingClearInfo();
	afx_msg void OnSettingFormatSD();
	afx_msg void OnSettingLanguageChange();
	afx_msg void OnBackup();
	afx_msg void OnRestore();
	afx_msg void OnFileOut();
	afx_msg void OnButtonSysInfo();
    afx_msg void OnRegRecordSound();
 	afx_msg void OnStaticClick(WPARAM w, LPARAM l);
	afx_msg void OnSettingClearRing();
	afx_msg void OnSettingDeleteRing();
	afx_msg void OnSettingUSBModal();
	afx_msg void OnButtonMissCallClear();
	DECLARE_MESSAGE_MAP()
private:
	void SendMsg(CWnd *pDlg);
	CRegRecordSound * m_pRegRecordSound;
public:
	CCEStatic m_sticTypeTitle;
	CCEStatic m_sticDetailTitle;
	CCEStatic m_sticOperation;
	CCEStatic m_sticSepLine1;
	CCEStatic m_sticSepLine2;
	CCEListCtrl	m_lsType;
	CImageList* m_pImageList, *m_pImageList1;
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnLogin;
	CCEBmpButton m_btnDefault;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnSystemInfo;

	//tel
	CCEStatic m_sticLocalAreaNumber;
	CCERectEdit m_edtLocalAreaNumber;
	CCEStatic m_sticLocalNumber;
	CCERectEdit m_edtLocalNumber;
	CCEButtonST m_chbEnableOutLine;
	CCERectEdit m_edtOutLine;
	CCEButtonST m_chbEnableIPDial;
	CCERectEdit m_edtIPDial;
	CCEButtonST m_chbEnableRing;
	CCEStatic m_sticDefaultRing;
	CCEStatic m_sticVolume;
	CCEComboBox_ m_cmbVolume;
	CCEComboBox_ m_cmbTip;
	CCEComboBox_ m_cmbRing;
	CCEButtonST m_chbEnableAutoLeaveWord;
	CCEButtonST m_IsSpeakerOut;
	CCEStatic m_sticRingTimes;
	CCEComboBox_ m_cmbRingTimes;
	CCEBmpButton m_btnPlayRing;
	CCEBmpButton m_btnPlayLeaveWordTip;
	CCEBmpButton m_btnDefineSpeedButton;
	CCEStatic m_sticDefineSpeedButton;

	//supper
	CCEButtonST m_chbSetAdminPassword;
	CCEBmpButton m_btnSetAdminPassword;
	CCEBmpButton m_btnSetPlayPassword;
	CCEButtonST m_chbEnabeRecord;
	CCEButtonST m_chbEnabePlayProtect;
	CCEButtonST m_chbEnabeDeleteProtect;

	//add by yy 20111103
	//一键清除未接电话
	CCEBmpButton m_btnSetOneKeyClear;
	CCEStatic m_sticUconnectNum;
	CCEStatic m_sticUconnectNum1;
	//

	CCEStatic m_sticRecordLimit;
	CCEComboBox_ m_cmbRecordLimit;
	//systom
	CCEButtonST m_chbEnnabeScreenProtect;
	CCEStatic m_sticWaitTime;
	CCEComboBox_ m_cmbWaitTime;
	CCEButtonST m_chbEnablePassword;
	CCEBmpButton m_btnSetPassword;
	CCEStatic m_sticTouchAdjust;
	CCEBmpButton m_btnTouchAdjust;

	CCEStatic m_sticBacklight;
	CCEComboBox_ m_cmbBacklight;

	//录音文件存储位置
	CCEStatic m_sticSoundSavePath;
	CCEComboBox_ m_cmbSoundSavePath;

	//时间日期设置
	CCEStatic     m_stcTime;
	CDateTimeCtrl m_dtTime;
	CDateTimeCtrl m_dtDate;
	SYSTEMTIME m_curtime;

	//info
	CCEStatic m_sticSystomTitle;
	CCEStatic m_sticSystomSerialNo;
	CCEStatic m_sticSystomSerialNo1;
	CCEStatic m_sticSoftwareVersion1;
	CCEStatic m_sticHardwareVersion1;
	CCEStatic m_sticMemorySize1;
	CCEStatic m_sticStorageSize1;
	CCEStatic m_sticContactSize1;
	CCEStatic m_sticContactInfoSize1;
	CCEStatic m_sticSoundSize1;
	CCEStatic m_sticInformationSize1;
	CCEStatic m_sticVersionRight1;
	CCEStatic m_versionChange;
	CCEStatic m_sticSoftwareVersion;
	CEdit	  m_editVersion;
	CCEStatic m_sticHardwareVersion;
	CCEStatic m_sticMemorySize;
	CCEStatic m_sticStorageSize;
	CCEStatic m_sticContactSize;
	CCEStatic m_sticContactInfoSize;
	CCEStatic m_sticSoundSize;
	CCEStatic m_sticInformationSize;
	CCEStatic m_sticVersionRight;

	CCEBmpButton m_btnClearContact;
	CCEBmpButton m_btnClearContactInfo;
	CCEBmpButton m_btnClearSound;
	CCEBmpButton m_btnClearInformation;
	CCEBmpButton m_btnFormatSD;
    CCEBmpButton m_btnLangChange;

	//RingInfo
	CCEStatic m_sticRingInfo;
	CCEStatic m_sticRingSize;
	CCEStatic m_sticRingList;
	CCEBmpButton m_btnClearRing;
	CCEBmpButton m_btnDeleteRing;
	CCEComboBox_ m_cmbDelRingList;
	CCEBmpButton m_btnUSBModal;
	CCEStatic m_sticUSBPort;

	//数据导入导出
	CCEStatic m_stcBackUP;//, m_stcReStore;
	CCEBmpButton m_btnBackUP, m_btnReStore;
	CCEStatic m_stcTitle;
	CCEBmpButton m_btnOut, m_btnDelete;

	CCEStatic m_sticPanel;
	CCEStatic m_sticPanel2;
	CCEFrameStatic m_sticBackground;
	CFastDialsDlg* m_pFastDialsDlg;
	CPasswordDlg*  m_pPasswordDlg;
	int m_iDeleteType;
	bool m_bTelephoneInUse;
	bool m_bPlaying;
    bool m_bPlayPause;
	int m_iTotalSeconds;
	int m_iCurrentSecond;
	OggCodec* m_pOggCodec;
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	void ShowConfigItems(void);
	void SetIPConfig(NETWORK_ADPT_INFO& AdptInfo);
	void GetIPInfo();
	BOOL FormatSD(void);
	void GetSystemInfo();
	char m_sSerialNo[64];
	char m_sHardwareNo[64];
	BOOL m_bUSBModal;
public:
	copyfileDlg*	m_copyfileDlg;
	boost::shared_ptr<Data::Setting> m_pSetting;

	///作为只弹出一次对话框的判断条件
	bool m_bLogin;
	bool m_iSBell;

	std::vector<CString> m_vTipsDir;//提示语的目录 

	int m_iPasswordType;
	void UpdateTips();
	void IniCtrlData();
	void ChangeLanguage();
	void bTntext(CString s);
	void SENDMSG();
	void UpdateDataok();
	void MoveControl();//移动控件位置
	void ChangeTips();
	void ChangeReg();//修改系统语言
	void setButtoName();
	void SettingDefault();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
