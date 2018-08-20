#if !defined(G3MMSREADDLG)
#define G3MMS3READDLG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog
#include "../control/MJPGStatic.h"
#include "../Data/MMSData.h"
#include "../Control/MmsShow.h"

class CMMSReadDlg : public CDialog
{
// Construction
public:
//	C3GSMSListDlg   *m_pSMSListDlg;

	CMJPGStatic		m_MJPGList;
	
	CMJPGStatic		m_MJPGList2;

	CMMSReadDlg(CWnd* pParent = NULL);   // standard constructor

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
	
	CCELineEdit				m_contentEdit;//д���ŵ�����
	SMSBOX_TYPE				m_nBoxType ;
	bool					m_bAutoPlay;

public:
	void SetMMSInfo(int id,SMSBOX_TYPE type);//
	void PageDown();
	void PageUp();
	void Replay(void);
	void Repeat();//�ӷ�
	void Transit(void);
	void DialBack();//�ز�
	void SaveDraft(void);//��ɲݸ�
	void SavePicture(void);//����ͼƬ
	void SaveAudio(void);//������Ƶ
	void PlayBtn();//����ģʽ
	void NumberExtract();//������ȡ 
	void MMSDetails(int id,SMSBOX_TYPE type);
	void MMSPreview(std::wstring path,std::wstring title);//����Ԥ��
	void MMSPageUp();	
	void MMSPageDown();
	void SetReadPage();

	void RefreshName(std::string name);  //wangzhenxing20100604
	
	MmsShow	m_MmsShow  ;//������ʾ�Ŀؼ�
	boost::shared_ptr<Data::MMSData >  m_pMMSData ;  //wangzhenxing20100610

	BOOL m_isReadMid;
	unsigned int m_pageCount;
	unsigned int m_currentPage;
protected:
	// Generated message map functions
	//{{AFX_MSG(C3GSMSDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
