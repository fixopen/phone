#include "..\Control\CEDialog.h"
#include "..\Resource.h"
#include "..\control\CeBtnST.h"
#include "..\control\CEStatic.h"
#include "..\control\CEListCtrl.h"
#include "../Data/SoundSegment.h"
#include <string>
#include <vector>
#if !defined(AFX_DETAILTIP_H__65BEEC51_DECE_489C_9617_BE3A84FF9ABE__INCLUDED_)
#define AFX_DETAILTIP_H__65BEEC51_DECE_489C_9617_BE3A84FF9ABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetailTip dialog

class CDetailTip : public CCEDialog
{
// Construction
public:
	CDetailTip(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailTip)
	enum { IDD = IDD_DIALOG_DETALTIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailTip)
	virtual BOOL OnInitDialog();
	 afx_msg void OnExit();
	 afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCEBmpButton m_btnExit;
	CCEBmpButton m_btnOk;
	CCEStatic m_sticName;
	CCEStatic m_sticNumber;
	CCEStatic m_sticDynNumber;
	CCEStatic m_sticDynName;
	CCEStatic m_sticStartime;
	CCEStatic m_sticDynStartime;
	CCEStatic m_sticDuration;
	CCEStatic m_sitcDynDuration;
	CCEStatic m_sticRecordCaption;
	CCEStatic m_sticDynRecordCaption;
	CCEFramePartStatic m_sticBackground;
	HWND m_handle;
	bool m_tip;
	CString result;
	int index;
	int Typeindex;
	std::vector<boost::shared_ptr<Data::SoundSegment> >  m_vCurrentResult;

public:
	void setDetialContent(CString Name,CString Number,CString StartTime,CString Duration,CString RecordCaption,bool flag);
	void ChangeLanguage();
	void setHwnd(HWND handle);
	void setTIp(bool flag);
	void setm_vCurrentResult(std::vector<boost::shared_ptr<Data::SoundSegment> > &tempm_vCurrentResult);
	void setTypeIndex(int index);
	void setTipIndex(int nindex);
	void setCurrentTIp(std::string currentTip);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILTIP_H__65BEEC51_DECE_489C_9617_BE3A84FF9ABE__INCLUDED_)
