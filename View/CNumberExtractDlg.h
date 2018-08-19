#if !defined(NUMBEREXTRACT)
#define NUMBEREXTRACT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// deletetipdlg.h : header file
//

#include "../Control/CEDialog.h"
#include <string>
#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog

class CNumberExtractDlg : public CCEDialog
{
// Construction
public:
	CNumberExtractDlg(CWnd* pParent = NULL);   // standard constructor
	int       type;

// Dialog Data
	//{{AFX_DATA(CDeleteTipDlg)
	enum { IDD = IDD_DIALOG_DELETETIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeleteTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeleteTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
private:
    const int				m_nPagesize;
	int						m_iCurrentPage;
	int						m_iTotalPage;
	int						m_nItem;
	int						m_ncontact;
	CMJPGStatic				m_MJPGList;
	CWnd*					m_pwnd;
	std::vector<CString>    m_vNumber;
public:
	void SetMainWnd(CWnd* pwnd);
	void ClickNumber(int item);
	void SelectContact(int item);
	void PageUp();
	void PageDown();
	void SetNumber(std::vector<CString> vNumber);
	void ShowNumber();
	int	 GetItems();
	void OnBtnOk();

	void NewContact();
	void OldContact();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETETIPDLG_H__F5EF9874_51FB_4A20_AE47_D27787F7717B__INCLUDED_)
