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
#include "fastdialsdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

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
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSoundOk();
	afx_msg void OnButtonSoundCancel();
	afx_msg void OnButtonSoundDefault();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTypeTitle;
	CCEStatic m_sticDetailTitle;
	CCEStatic m_sticOperation;
	CCEStatic m_sticSepLine1;
	CCEStatic m_sticSepLine2;
	CCEListCtrl	m_lsType;
	CImageList* m_pImageList;
	CCEButtonST m_btnOk;
	CCEButtonST m_btnCancel;
	CCEButtonST m_btnDefault;

	//tel
	CCEButtonST m_chbEnableRing;
	CCEStatic m_sticDefaultRing;
	CCEComboBox_ m_cmbRing;
	CCEButtonST m_chbEnableAutoLeaveWord;
	CCEStatic m_sticRingTimes;
	CCEComboBox_ m_cmbRingTimes;
	CCEStatic m_sticAutoLeaveWord;
	CCEComboBox_ m_cmbAutoLeaveWordTip;
	CCEButtonST m_btnRecardLeaveWordTip;
	CCEButtonST m_btnDefineSpeedButton;
	CCEStatic m_sticDefineSpeedButton;
	//browser
	CCEButtonST m_rdoConnectAuto;
	CCEButtonST m_rdoConnectManual;
	CCEStatic m_sticHttpAgent;
	CCEStatic m_sticScoksAgent;
	CCELineEdit m_edtHttp;
	CCELineEdit m_edtScoks;
	CCEStatic m_sticHttpPort;
	CCEStatic m_sticScoksPort;
	CCELineEdit m_edtHttpPort;
	CCELineEdit m_edtScoksPort;
	CCEStatic m_sticHomeSite;
	CCELineEdit m_edtHomeSite;
	//systom
	CCEButtonST m_chbEnnabeScreenProtect;
	CCEStatic m_sticScreenProtctType;
	CCEButtonST m_rdoTimeScreen;
	CCEButtonST m_rdoImageScreen;
	CCEStatic m_sticWaitTime;
	CCEComboBox_ m_cmbWaitTime;
	CCEButtonST m_chbEnablePassword;
	CCEButtonST m_btnSetPassword;
	CCEStatic m_sticTouchAdjust;
	CCEButtonST m_btnTouchAdjust;
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
	CCEStatic m_sticHardwareVersion;
	CCEStatic m_sticMemorySize;
	CCEStatic m_sticStorageSize;
	CCEStatic m_sticContactSize;
	CCEStatic m_sticContactInfoSize;
	CCEStatic m_sticSoundSize;
	CCEStatic m_sticInformationSize;
	CCEStatic m_sticVersionRight;

	CCEButtonST m_btnClearContact;
	CCEButtonST m_btnClearContactInfo;
	CCEButtonST m_btnClearSound;
	CCEButtonST m_btnClearInformation;

	CFastDialsDlg* m_pFastDialsDlg;

	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	void ShowConfigItems(void);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
