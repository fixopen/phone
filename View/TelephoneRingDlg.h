#if !defined(TELEPHONERING_)
#define TELEPHONERING_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TelephoneDialDlg.h : header file
//

#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"
#include "../Data/Contact.h"

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDialDlg dialog

class CTelephoneRingDlg : public CCEDialog
{
// Construction
public:
	CTelephoneRingDlg(CWnd* pParent = NULL);   // standard constructor

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
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	std::string		m_sTelephoneNumber;

public:
	CMJPGStatic		m_MJPGList;

	void	HangOff_();
	void	ShowContact(boost::shared_ptr<Data::Contact> contact, std::string number, int unitNo = 100);
	void	HandleOn();
	void	Switch();
	void	HideContact(int uintNo,bool bshow = true );
	void    InitData();
	void	ClearData(int unitNo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDIALDLG_H__2F2778C5_ECA9_4873_8F2E_91B0E7A1D97F__INCLUDED_)
