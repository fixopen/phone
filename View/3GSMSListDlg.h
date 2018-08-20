#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_40_41__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg dialog
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"
#include "../Data/Message.h"
#include "../Data/MMSData.h"

enum SMSBOX_TYPE{RECV_TYPE = 0, SEND_TYPE, DRAFT_TYPE, HOME_RECORD_TYPE};
enum SMSMMS_TYPE{SMS_TYPE = 0, MMS_TYPE, HOME_TYPE};
class C3GSMSListDlg : public CDialog
{
// Construction
public:
	int m_nSelectItem;
	int m_nListCountTotal;
	int m_nPageSize;
	std::string m_strHomeRecordeTelCode;     //º“Õ•¡Ù—‘Ãÿ∑˛∫≈

	unsigned int m_uiSelectIndex;
	std::string m_sListFilter;
	std::string m_sListSearchFilter;
	std::vector<boost::shared_ptr<Data::Message> > m_vMessageCurrentResult;
	std::vector<boost::shared_ptr<Data::MMSData> > m_vMMSDataCurrentResult;

//	std::vector<boost::shared_ptr<Data::SoundSegment> > m_vSearchResult;

	void ScrollItemsInList(int step, int nPos);
	void ShowArrayInList();
	void DeleteRefreshList();

	CCEListCtrl	m_lsList;
	CImageList* m_pImageList;

	SMSBOX_TYPE     m_nBoxType;
	SMSMMS_TYPE     m_nSMSType;
	void initType(SMSBOX_TYPE box_type, SMSMMS_TYPE sms_type, BOOL reDraw = FALSE);
	void initDateBase();
	
	CMJPGStatic		m_MJPGList;
	C3GSMSListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C3GSMSListDlg)
	enum { IDD = IDD_DIALOG_CONTACT };
		// NOTE: the ClassWizard will add data members here
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GSMSListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3GSMSListDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
