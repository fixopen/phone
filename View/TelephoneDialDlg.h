#if !defined(AFX_TELEPHONEDIALDLG_H__2F2778C5_ECA9_4873_8F2E_91B0E7A1D97F__INCLUDED_)
#define AFX_TELEPHONEDIALDLG_H__2F2778C5_ECA9_4873_8F2E_91B0E7A1D97F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TelephoneDialDlg.h : header file
//

//#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"
//#include "../Data/ContactInfo.h"
#include "10ContactInfoDlg.h"

#define IDT_AUTO_DIAL	1

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDialDlg dialog

class CTelephoneDialDlg :public CDialog/*: public CCEDialog*/
{
// Construction
public:
	CTelephoneDialDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTelephoneDialDlg)
	enum { IDD = IDD_DIALOG_TELEPHONE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelephoneDialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	virtual BOOL OnInitDialog();

	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPG1(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	//boost::shared_ptr<Data::ContactInfo>	m_spContactInfo[2];
	std::string								m_sTelephoneNumber;
	bool									m_bFrom10ContactInfo;//电话是否来自10

public:
	CMJPGStatic				m_MJPGList;
	C10ContactInfoDlg      *m_p10ContactInfoDlg;
	bool					m_bMayReturn;
	bool					m_bSoftware ;//是否软件拨号
	bool					m_bsubDial;//是否二次拨号
	bool					m_bSetGainLevel;//是否设置增益

	void	Key_(void* param);
	void	HangOff_();
	void    SetTel(CString tel);
	void	DeleteNum();
	void    ShowWindow_(int nCmdShow);
	void    SubDialShow();//二次拨号
	void    ExitWindow();//退出窗口
	void    Dial();//拨号
	void    SetDialTimer();

	afx_msg void OKMessage(WPARAM w, LPARAM l);//OK

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDIALDLG_H__2F2778C5_ECA9_4873_8F2E_91B0E7A1D97F__INCLUDED_)
