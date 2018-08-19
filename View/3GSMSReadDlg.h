#if !defined(G3SMSREADDLG)
#define G3SMS3READDLG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog
#include "../control/MJPGStatic.h"
#include "../Data/Message.h"

class CSMSReadDlg : public CDialog
{
// Construction
public:
//	C3GSMSListDlg   *m_pSMSListDlg;

	CMJPGStatic		m_MJPGList;
	CSMSReadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C3GSMSDlg)
	enum { IDD = IDD_DIALOG_MMS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GSMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

private:
	
	boost::shared_ptr< Data::Message > m_pMessage ;
	SMSBOX_TYPE				m_nBoxType ;
	CCELineEdit				m_contentEdit;//写短信的内容

public:
	void SetSMSInfo(int id,SMSBOX_TYPE type);//
	void PageDown();
	void PageUp();
	void Replay(void);
	void Transit(void);
	void DialBack();//回拨
	void Repeat();//重发
	void SaveDraft(void);//存成草稿
	void NumberExtract();//号码提取
	void CheckChatacter(CString &ct);//号码检查

	void RefreshName(std::string name);  //wangzhenxing20100604
	void CalCulateContent(CString content = L"");

protected:
	// Generated message map functions
	//{{AFX_MSG(C3GSMSDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
