#if !defined(CUPLMNDLG_)
#define CUPLMNDLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenameDlg.h : header file
//

#include "../control/MJPGStatic.h"
#include "../Control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog

class CUPLMNDlg : public CCEDialog
{
public:
	CMJPGStatic		m_MJPGList;
	CCEComboBox12_    m_cUPLMNList;
	CCERectEdit		m_cEditOper;
	CCERectEdit		m_cEditGsm_Act;
	CCERectEdit		m_cEditGsm_Compact;
	CCERectEdit		m_cEditUtran_Act;

	struct UPLMN  
	{
		uchar index;
		uchar format;
		std::string oper;
		uchar gsm_act;
		uchar gsm_compact;
		uchar utran_act;
	};

	enum operate
	{
		search,
		add,
		change,
	};
	
	std::vector<UPLMN> m_vUplmnList;
	operate m_operate;
	//
	void Add();
	void Change();
	void Delete();
	void ParseUPLMN(char *u);
	void ShowUplmn();
	void SetUplmn(int index);
	void OnbuttonOK();
	void DelList(int index);
	void ReadOnly(bool b);

public:
	CUPLMNDlg(CWnd* pParent = NULL);   // standard constructor

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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnSelectUPLMN(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENAMEDLG_H__0A17544A_9F8E_4C81_ADA6_0A95FF826E02__INCLUDED_)
