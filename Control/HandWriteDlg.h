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
	void l_PutIcon();			//刷新界面
	char m_chSxShowBuff[32];    //记录拼音或者手写时,在候选区显示的汉字


public:
	BOOL MoveTo(int x);
	void hide(BOOL flag = TRUE);
	void show(BOOL flag = TRUE);	
	void hideorshow();

private:
	void CreateCtrl();
	BOOL m_bIsLimit;			//是否限制特殊字符的输入\n

	int	  m_nRegStyle;		    //记录识别的范围 0 汉字 1  英文  2 数字
	char *m_pPreText;			//保存输入buff
	char *m_Text;				//用来接收输入的内容
	int  m_iRecogRange;			//手写识别范围 
	
	int  m_xPoint, m_yPoint;	//窗口的位置
	BOOL m_bIsShow;

	CSoftKey m_oSoftkeyHand;	//手写	
	CFont    m_Font ;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__3E0A0999_75A6_4848_8994_A96B63FF2BD7__INCLUDED_)
