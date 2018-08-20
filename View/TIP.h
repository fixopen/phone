

#if !defined(AFX_TIP_H__D59BF2B9_5D0A_4890_8E54_FC4D1B0AA29D__INCLUDED_)
#define AFX_TIP_H__D59BF2B9_5D0A_4890_8E54_FC4D1B0AA29D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TIP.h : header file
//
#include "../Control/CEDialog.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Resource.h"
#include "../Control/CEComboBox_.h"
#include <string>
#include <vector>
#include "../Util/SmartPtr.h"
#include "../Data/SoundSegment.h"
#include "../control/CELineEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CTIP dialog

class CTIP : public CCEDialog
{
// Construction
public:
	CTIP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTIP)
	enum { IDD = IDD_DIALOG_TIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTIP)
		// NOTE: the ClassWizard will add member functions here
    virtual BOOL OnInitDialog();
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTip;
	CCERectEdit  m_cmbTip;
	//CCEButtonST m_isSetTip;
	//CCEComboBox_ m_cmbTip;
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEFramePartStatic m_sticBackground;
    CString result;
	int index;
	int Typeindex;
	std::vector<boost::shared_ptr<Data::SoundSegment> >  m_vCurrentResult;
public:
	void  OnButtonTipOk();
	void OnButtonCancel();
	void ChangeLanguage();
	void setCurrentTIp(std::string currentTip);
	//void AddTIpItems();
	void setTipIndex(int nindex);
	void setm_vCurrentResult(std::vector<boost::shared_ptr<Data::SoundSegment> > &tempm_vCurrentResult);
	void setTypeIndex(int index);
	std::string ReplaceSingleQuotation(std::string& field);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIP_H__D59BF2B9_5D0A_4890_8E54_FC4D1B0AA29D__INCLUDED_)
