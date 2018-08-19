#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_41__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GDetailDlg dialog
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"
#include "../Data/OTAData.h"
#include "3GDetailReadDlg.h"

struct Menu3G 
{
	int			menuId;				//�˵�ID
	int         parentId;			//���˵�ID
	int         menuLevel;			//�˵������ļ���
	int         orderNo;			//�˵���λ��
	std::string telcode;			//�������ط���
	std::string menuName;			//�˵�����
	std::string jpgName;			//�˵�ͼƬ��
};

/*
struct MenuLink
{
	Menu3G *pCurMenu;           
	MenuLink *pParentMenu;    //��   
	MenuLink *pNext;		  //ͬһ���еĺ�һ��
	MenuLink *pPre;           //ͬһ���е�ǰһ��
	MenuLink *pChild;		  //��
};
*/

class C3GDetailDlg : public CDialog
{
// Construction
public:
	CMJPGStatic		m_MJPGList;
	C3GDetailDlg(CWnd* pParent = NULL);   // standard constructor
	C3GDetailReadDlg *m_p3GDetailReadDlg;

	boost::shared_ptr<Data::MenuManager> m_pMenuManager; //= boost::shared_ptr<Data::MenuManager> (new Data::MenuManager) ;
	std::vector<boost::shared_ptr<Data::MenuManager> > m_pMenuManagerresult;// = Data::MenuManager::GetFromDatabase(""); 

	Menu3G m_Menu3G[9];
	//��ʼ���˵���parentId = 0 ��һ���˵�,��ͥ����ͨ��������
	void initmenu(int parentId);
	//����˵��ĵ��������ж����˵������ʼ�������˵���m_Menu3G
	int clickmenu(int index); 
	void doNext();
	void doPre();
	void OnExit();

	int m_nMenuCount;
	int m_nMenuIndex;
	int m_nMenuLevel;

// 	void InitMenuLink();
// 	void FreeMenuLink();

// Dialog Data
	//{{AFX_DATA(C3GDetailDlg)
	enum { IDD = IDD_DIALOG_CACULATER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3GDetailDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
