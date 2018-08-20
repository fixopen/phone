#if !defined(_HANDWRITEDLG_)
#define _HANDWRITEDLG__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog
#include "softkey.h"

#define INPUTMATH_H		0x105
#define LEFT			0
#define RIGHT			1	

//#define WM_CHANGELANGUAGE (WM_USER+300)
class CHandWriteDlg : public CDialog
{
// Construction
public:
	CEdit		*m_pOwnerEdit;
	void		SetOwnerEdit(CEdit *pEdit);
public:
	CHandWriteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_DIALOG_HANDWRITE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSoftKey(WPARAM w, LPARAM l);
	afx_msg void OnSoftKeyReg(WPARAM w, LPARAM l);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG


public:

	void SetLimit(BOOL falg = TRUE){m_bIsLimit = falg;}
	
	bool m_bDigital ;
	void SetLimitDiagital(BOOL falg = TRUE)
	{
		m_bDigital = falg;
	}

protected:
	void OnSoftKeyHand(NMHDR_*, LRESULT*);
private:
	void l_PutIcon();			//ˢ�½���
	char m_chSxShowBuff[32];    //��¼ƴ��������дʱ,�ں�ѡ����ʾ�ĺ���


public:
	BOOL MoveTo(int x);
	void hide(BOOL flag = TRUE);
	void show(BOOL flag = TRUE);	
	void hideorshow();

private:
	void CreateCtrl();
	BOOL m_bIsLimit;			//�Ƿ����������ַ�������\n

	int	  m_nRegStyle;		    //��¼ʶ��ķ�Χ 0 ���� 1  Ӣ��  2 ����
	char *m_pPreText;			//��������buff
	char *m_Text;				//�����������������
	int  m_iRecogRange;			//��дʶ��Χ 
	
	int  m_xPoint, m_yPoint;	//���ڵ�λ��
	BOOL m_bIsShow;

	CSoftKey m_oSoftkeyHand;	//��д	
	CFont    m_Font ;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__3E0A0999_75A6_4848_8994_A96B63FF2BD7__INCLUDED_)
