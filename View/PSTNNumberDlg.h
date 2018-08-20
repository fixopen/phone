//#if !defined(AFX_PSTNNUMBERDLG_H__6B99FBA3_E73D_445B_A328_C9467A872F33__INCLUDED_)
//#define AFX_PSTNNUMBERDLG_H__6B99FBA3_E73D_445B_A328_C9467A872F33__INCLUDED_

#if !defined(PSTNNUMBERDLG_)
#define PSTNNUMBERDLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PSTNNumberDlg.h : header file
//

#include "../control/MJPGStatic.h"
#include "../Control/CEDialog.h"
#include "../Control/CEComboBox_.h"
#include "../Control/CELineEdit.h"
#include "../resource.h"
#include "../Util/ATCommandWarp.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CPSTNNumberDlg dialog

class CPSTNNumberDlg : public CCEDialog
{
// Construction
public:
	CPSTNNumberDlg(CWnd* pParent = NULL);   // standard constructor

	CMJPGStatic		m_MJPGList;
	CCEComboBox12_    m_cmbPSTNList;
	CCERectEdit		m_edtName;
	CCERectEdit		m_edtNumber;

// Dialog Data
	//{{AFX_DATA(CPSTNNumberDlg)
	enum { IDD = IDD_DIALOG_RENAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	int  m_nOper;   //0 无   1  修改  2  新增
	int  m_nContactNum;
	int  m_nIndex;
	void DeleteAll();
	void Change();
	void Delete();
	void Add();
	void SetPSTNList(int index);
	void ReadOnly(bool b);
	void OnbuttonOK();
	void ParseCPBR(char const * data);

	std::vector<Util::ATCommandWarp::SIM_FORMAT> vFDNList;
	void ShowPSTNList();//出现PSTN

private:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSTNNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPSTNNumberDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnSelectPSTN(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSTNNUMBERDLG_H__6B99FBA3_E73D_445B_A328_C9467A872F33__INCLUDED_)
