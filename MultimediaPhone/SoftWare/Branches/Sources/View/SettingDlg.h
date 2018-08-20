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
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSettingOk();
	afx_msg void OnButtonSettingCancel();
	afx_msg void OnButtonSettingDefault();
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

	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTypeTitle;
	CCEStatic m_sticDetailTitle;
	CCEStatic m_sticOperation;
	CCEStatic m_sticSepLine1;
	CCEStatic m_sticSepLine2;
	CCEListCtrl	m_lsType;
	CImageList* m_pImageList, *m_pImageList1;
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnDefault;

	//tel
	CCEStatic m_sticLocalAreaNumber;
	CCERectEdit m_edtLocalAreaNumber;
	CCEButtonST m_chbEnableOutLine;
	CCERectEdit m_edtOutLine;
	CCEButtonST m_chbEnableIPDial;
	CCERectEdit m_edtIPDial;
	CCEButtonST m_chbEnableRing;
	CCEStatic m_sticDefaultRing;
	CCEComboBox_ m_cmbRing;
	//铃声选择由Combox改成ListCtrl
	CCERectEdit m_ringEdit;
	CCEBmpButton m_ringButton;
	CCEListCtrl m_ringList;
	CCEStatic m_ringStatic;
	TCHAR	  m_ringDir[128];
	void SetRingLst(TCHAR *dir);

	CCEButtonST m_chbEnableAutoLeaveWord;
	CCEStatic m_sticRingTimes;
	CCEComboBox_ m_cmbRingTimes;

	CCEStatic m_sticAutoLeaveWord;
//	CCEComboBox_ m_cmbAutoLeaveWordTip;
	CCEBmpButton m_btnRecardLeaveWordTip;
	CCEBmpButton m_btnPlayLeaveWordTip;
	CCEBmpButton m_btnDefineSpeedButton;
	CCEStatic m_sticDefineSpeedButton;


	//browser
	IPInfo			m_ip;
	CCERadioButton m_rdoConnectAuto;
	CCERadioButton m_rdoConnectADSL;
	CCEStatic m_sticADSLName;
	CCEStatic m_sticADSLPassword;
	CCERectEdit m_edtADSLName;
	CCERectEdit m_edtADSLPassword;	
	CCERadioButton m_rdoGetIPDHCP;
	CCERadioButton m_rdoGetIPStatic;
	CCEStatic m_sticIP;
	CCEStatic m_sticMask;
	CCEStatic m_sticGateway;
	CCEStatic m_sticDNS;
	CCERectEdit m_edtIP[4];
	CCERectEdit m_edtMask[4];
	CCERectEdit m_edtGateway[4];
	CCERectEdit m_edtDNS[4];

	//systom
	CCEButtonST m_chbEnnabeScreenProtect;
	CCEStatic m_sticScreenProtctType;
	CCERadioButton m_rdoTimeScreen;
	CCERadioButton m_rdoImageScreen;
	CCEStatic m_sticWaitTime;
	CCEComboBox_ m_cmbWaitTime;
	CCEButtonST m_chbEnablePassword;
	CCEBmpButton m_btnSetPassword;
	CCEStatic m_sticTouchAdjust;
	CCEBmpButton m_btnTouchAdjust;

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
	CCEStatic m_sticSoftwareVersion1;
	CCEStatic m_sticHardwareVersion1;
	CCEStatic m_sticMemorySize1;
	CCEStatic m_sticStorageSize1;
	CCEStatic m_sticContactSize1;
	CCEStatic m_sticContactInfoSize1;
	CCEStatic m_sticSoundSize1;
	CCEStatic m_sticInformationSize1;
	CCEStatic m_sticVersionRight1;
	
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

	//数据导入导出
	CCEStatic m_stcBackUP;//, m_stcReStore;
	CCEBmpButton m_btnBackUP, m_btnReStore;
	CCEStatic m_stcTitle;
	CCEBmpButton m_btnOut, m_btnDelete;
//	CCEBmpButton 

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
	CPasswordDlg*  m_passwordDlg;
	int m_iDeleteType;

	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	void ShowConfigItems(void);
	void IniCtrlData();
	void SetIPConfig(NETWORK_ADPT_INFO& AdptInfo);
	void GetIPInfo();
public:
	copyfileDlg*	m_copyfileDlg;
	boost::shared_ptr<Data::Setting> m_pSetting;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
