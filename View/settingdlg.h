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
    afx_msg LRESULT OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l);

    afx_msg void OnBackup();
    afx_msg void OnRestore();
    afx_msg void OnFileOut();
    //速拨设置
    afx_msg void OnButtonFastDialsOk();
    afx_msg void OnButtonFastDialsCancel();

    afx_msg LRESULT OnStaticClick(WPARAM w, LPARAM l);
    afx_msg LRESULT OnComboSelect(WPARAM w, LPARAM l);
    afx_msg void OnPaint();
    afx_msg LRESULT OnCheckPWD(WPARAM w, LPARAM l);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnButtonSetting();
    void SetSettingDefault();
    void SetSettingOK();
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);

    CMJPGStatic		m_MJPGList;
    CFont m_Font;
    unsigned int m_uiType;
    unsigned int m_clickType;
    unsigned int m_iDeleteType;
    CImageList* m_pImageList, *m_pImageList1;
    CCERectEdit m_edtLocalAreaNumber;
    CCERectEdit m_edtOutLineDial1;
    TCHAR	  m_ringDir[128];
    CCEComboBox_ m_cmbRingTimes;
    CCEComboBox_ m_cmbAutoRecoedeTimes;
    //browser
    CCERectEdit m_edtADSLName;
    CCERectEdit m_edtADSLPassword;	
    CCERectEdit m_edtIP[4];
    CCERectEdit m_edtMask[4];
    CCERectEdit m_edtGateway[4];
    CCERectEdit m_edtDNS[4];

    //systom
    CCEComboBox_ m_cmbWaitTime;

    //时间日期设置
    CDateCtrl m_dtTime;
    CDateCtrl m_dtDate;
    SYSTEMTIME m_curtime;

    //blacklight
    CCEComboBox_ m_cmbBlackLightWaitTime;
    CCEComboBox_ m_cmbNightBlackLightStartTime;
    CCEComboBox_ m_cmbNightBlackLightEndTime;

    CCEStatic m_sticPanel;
    CCEStatic m_sticPanel2;
    CCEFrameStatic m_sticBackground;

    CPinSetDlg *m_pPinSetDlg;
    CRingSetDlg *m_pRingSetDlg;
    CTipDlg *m_pTipDlg;
    CPasswordDlg*  m_passwordDlg;
    CCallSetDlg *m_pCallSetDlg;

    void SetButtonDefaultColor(CCEButtonST* button);
    void SetButtonSelectedColor(CCEButtonST* button);
    void ShowConfigItems(void);
    void ShowWindow_(int nCmdShow);
    void IniCtrlData();
    void SetIPConfig(NETWORK_ADPT_INFO& AdptInfo);
    void SetShowTimer();
    void SetBlackLightParam(boost::shared_ptr<Data::Setting> data);
    void SetSyetemParam(boost::shared_ptr<Data::Setting> data);

    afx_msg LRESULT OnStopTryRing(WPARAM w, LPARAM l);
    afx_msg LRESULT OnBtnSTClick(WPARAM w, LPARAM l);
    void StopTryRing();

public:
    bool m_bLogin;
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
    CCEComboBox_ m_cmbTime;
    void OnButtonFireWallOk();
    void SetParameters(int type, int duration);
    int GetFirewallType();
    void BrushWindow(void);
    //铃声
    CCERectEdit m_edtRingNumber[3];
    CCEComboBox_ m_cmbRing;
    CCEComboBox_ m_cmbSMSRing;
    CCEComboBox_ m_cmbSpecRing;
    int m_systemVolume;
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__62383DD8_9B79_4FB2_9194_5181774AF920__INCLUDED_)
