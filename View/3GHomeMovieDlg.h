#if !defined(AFX_3GHOMEMOVIEDLG_H__2427BD27_C3A9_45F0_8BB1_928C17397A36__INCLUDED_)
#define AFX_3GHOMEMOVIEDLG_H__2427BD27_C3A9_45F0_8BB1_928C17397A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3GHomeMovieDlg.h : header file
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"

/////////////////////////////////////////////////////////////////////////////
// C3GHomeMovieDlg dialog

class C3GHomeMovieDlg : public CDialog
{
// Construction
public:
	C3GHomeMovieDlg(CWnd* pParent = NULL);   // standard constructor
	CMJPGStatic		m_MJPGList;

	void SetUnitFont();
	void OnClearAll();
	void ShowItemsInList(int type);
	void ShowArrayInList(std::vector<CString> fileName);
	void SetShowStatus(int unitNO, BOOL status);
	int SetPlayList(TCHAR *dir);
	void OnSelectAll();
	void OnButtonCheck(int uintNO);
	void PageUp();
	void PageDown();
	void OnShiftFile();
	void OnClickDelete();
	void DeleteSelectedItem();
	void OnExit_();
	void OnPlayer(int index = 0);
	void FindSelectPage();
	void AddFileMovie();
	void SetInitShow();
	void ShowWindow_(int nCmdShow);

// Dialog Data
	//{{AFX_DATA(C3GHomeMovieDlg)
	enum { IDD = IDD_DLG_MOVIE };
	//}}AFX_DATA
private:
	std::vector<CString> m_fileMovie;
	std::vector<CString> m_fileDir;
	std::vector<int> m_isFileSelect;
	
	unsigned int m_cardType1;
	unsigned int m_cardType2;
	unsigned int m_uiType;
	unsigned int m_fileCount;
	unsigned int m_currentPage;
	unsigned int m_firstSelPage;
	unsigned int m_lastSelPage;
	unsigned int m_pageSize;
	unsigned int m_pageCount;
	unsigned int m_offSet;
	
	TCHAR	m_chDir[128];
	BOOL    m_isSelectAll;
	BOOL m_UnitStatus[5];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GHomeMovieDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(C3GHomeMovieDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnDeviceChange(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3GHOMEMOVIEDLG_H__2427BD27_C3A9_45F0_8BB1_928C17397A36__INCLUDED_)
