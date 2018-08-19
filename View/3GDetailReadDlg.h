#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_4100__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GDetailReadDlg dialog
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../control/CELineEdit.h"
#include "../control/CEComboBox_.h"
#include "../Data/LanguageResource.h"
#include "../Data/OTAData.h"
#include "../control/MmsShow.h"

class C3GDetailReadDlg : public CDialog
{
// Construction
public:

	CCELineEdit m_onlyEdit;
	MmsShow     m_MMSShow;
	CMJPGStatic		m_MJPGList;
	C3GDetailReadDlg(CWnd* pParent = NULL);   // standard constructor

	boost::shared_ptr<Data::MultimediaDownload> m_pMediaList; //= boost::shared_ptr<Data::MenuManager> (new Data::MenuManager) ;
	std::vector<boost::shared_ptr<Data::MultimediaDownload> > m_pMediaFileresult;// = Data::MenuManager::GetFromDatabase(""); 

	//天气相关
	std::vector<boost::shared_ptr<Data::Weather> > m_pWeatherdateresult;
	std::vector<boost::shared_ptr<Data::Weather> > m_pWeatherresult;
	std::vector<boost::shared_ptr<Data::WeatherPicture> > m_pWeatherPicture;

	void inittitle(char *titleTxt, char *titleIconFilename);
	void ShowDetailRead(int i, int flag);

	//读特服号相关的文件显示，spCode 特服号， nOffset 从数据库中的偏移
	void initmenu( int menutype, std::string spCode, int nOffset);
	//处理菜单的点击，显示相关的内容
	int clickmenu(int index); 
	void doNext();
	void doPre();
	void doDelete();

	void OnExit();

	std::string m_filter;
	int m_nMenuType;       //0 天气预报
	int m_nMenuCount;
	int m_nMenuIndex;
	int m_nOffset;		   //偏移
	int m_nClickIndex;		//点击的是第几个

// 	void InitMenuLink();
// 	void FreeMenuLink();

// Dialog Data
	//{{AFX_DATA(C3GDetailReadDlg)
	enum { IDD = IDD_DIALOG_CONTACTINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GDetailReadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3GDetailReadDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnDeleteItem(WPARAM w, LPARAM l);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
