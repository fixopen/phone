#if !defined(CSMSDETAILDLG)
#define CSMSDETAILDLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenameDlg.h : header file
//

#include "../control/MJPGStatic.h"
#include "../Control/CEDialog.h"
#include "../Data/MMSData.h"
#include "../Data/Message.h"

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog

class CSmsDetailDlg : public CCEDialog
{
public:
	CMJPGStatic		m_MJPGList;
	void ShowSmsDetail(boost::shared_ptr< Data::Message > pMessage );
	void ShowMmsDetail(boost::shared_ptr<Data::MMSData > pMmsData );
	void ShowContactinfoDetail(boost::shared_ptr<Data::ContactInfo > pContactInfo );//通话信息详情
	void Clear();
public:
	CSmsDetailDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRenameDlg)
	enum { IDD = IDD_DIALOG_RENAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENAMEDLG_H__0A17544A_9F8E_4C81_ADA6_0A95FF826E02__INCLUDED_)
