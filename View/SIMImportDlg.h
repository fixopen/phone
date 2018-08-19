#if !defined(AFX_SIMIMPORTDLG_H__D34B822E_F946_480E_98CA_6451685DC58F__INCLUDED_)
#define AFX_SIMIMPORTDLG_H__D34B822E_F946_480E_98CA_6451685DC58F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIMImportDlg.h : header file
//

#include "../Control/CEDialog.h"
#include "../control/MJPGStatic.h"
#include "../control/CEComboBox_.h"
#include "../control/CEProcessBar.h"

#include "../Data/Contact.h"
/////////////////////////////////////////////////////////////////////////////
// CSIMImportDlg dialog

enum Destination{
	SIMtoLocal,
	LocaltoSIM,
	};
class CSIMImportDlg : public CCEDialog
{
private:
	int				m_iMaxPos;
	Destination		m_enumDes;
	HWND			m_handle;
	CMJPGStatic		m_MJPGList;
	CCEProcessBar	m_procbar;
	void WriteSIMTelPhoneBOOK(CString name, CString telcode);

	boost::shared_ptr<Data::Contact> m_pContact;

public:

	CSIMImportDlg(CWnd* pParent = NULL);   // standard constructor
	
	void SetHWnd(HWND handle);
	void InitSIM();
	void SendCMDSIM(int nIndex);
	void OnBtnOK();	
	void SetUnit(Destination des);

	void SetProcessMax(int max );
	void SetProcessPos(int npos);

// Dialog Data
	//{{AFX_DATA(CSIMImportDlg)
	enum { IDD = IDD_DIALOG_SIMIMPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSIMImportDlg)
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMIMPORTDLG_H__D34B822E_F946_480E_98CA_6451685DC58F__INCLUDED_)
