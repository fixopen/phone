#if !defined(AFX_CWARNINGDLG_H__D6EA15E4_421E_4333_936A_E63892516BB7__INCLUDED_)
#define AFX_CWARNINGDLG_H__D6EA15E4_421E_4333_936A_E63892516BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CWarningDlg.h : header file
//

#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEDialog.h"
#include <string>

/////////////////////////////////////////////////////////////////////////////
// CCWarningDlg dialog

class CCWarningDlg : public CCEDialog
{
// Construction
public:
	CCWarningDlg(CWnd* pParent = NULL);   // standard constructor
	void ChangeLanguage();

// Dialog Data
	//{{AFX_DATA(CCWarningDlg)
	enum { IDD = IDD_DIALOG_WARNING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCWarningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCWarningDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCEBmpButton m_btnOk;
	CCEFramePartStatic m_sticBackground;

public:
	void SetTitle(std::string title);
	void OnButtonWarningOK();
	void SetDelTip(CString tips);

private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticTip;
	CCEStatic m_sticPassword;
	CCERectEdit m_edtPassword;

	CCEBmpButton m_btnCancel;

	HWND m_handle;
	bool m_bIsPasswordModel;
	bool m_bIsNewContact;
	CString m_sPassword;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWARNINGDLG_H__D6EA15E4_421E_4333_936A_E63892516BB7__INCLUDED_)
