#if !defined(AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_)
#define AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactInfoDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Data/ContactData.h"
#include "../Util/SmartPtr.h"
#include "PlaySoundDlg.h"
#include "ContactInfoSearchDlg.h"
#include "../Data/ContactInfo.h"
//#include "noteDlg.h"
#include "PasswordDlg.h"

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg dialog

class CContactInfoDlg : public CDialog
{
// Construction
public:
	CContactInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactInfoDlg)
	enum { IDD = IDD_DIALOG_CONTACTINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnClickPageDown(int currentPage);
	void OnClickPageUP(int currentPage);
	void OnButtonCheck( int UnitNO);
	void FindSelectPage();
	// Generated message map functions
	//{{AFX_MSG(CContactInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonDial();
	afx_msg void OnButtonSound();
	afx_msg void OnButtonNote();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonDelete();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()

private:

	CMJPGStatic		m_MJPGList;
	CFont			m_NewListFont;	
	CCEListCtrl		m_lsList;
	CCERectEdit		*m_rEditInput;
	CMJPGStatic		m_MJPGData_key;//���ּ���
 	CMJPGStatic		m_MJPGLetter_key;//��ĸ����

	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);

	CPasswordDlg*		m_pPasswordDlg;
	bool				m_bTelephoneInUse;
	const unsigned int  ContactInfoTotal;
	const unsigned int  PageSize;

	
	std::string m_sListFilter;
	std::string m_sListSearchFilter;
	std::vector<boost::shared_ptr<Data::ContactInfo> > m_vCurrentResult;
 	void DeleteSelectedItem(void);

public:
	CPlaySoundDlg* m_pPlaySoundDlg;

	std::string GetPYIndex(CString content);
	void SaveContactInfo(boost::shared_ptr<Data::ContactInfo> ci);
	void Search(std::string filter);
	void ShowUnconnectItems(void);

private:
	enum Action{
			up_page,
			down_page,
		};

	std::vector<int>				m_vClick;//��ϵ���Ƿ񱻹�ѡ
	std::vector<int>				m_vContactID;//������ϵ�˵�ID
	int								m_iCurrentPage;//��ǰ��ҳ
	int								m_iTotalPages;//һ������ҳ
	int								m_uiType ;
	int								m_iTabs;//ҳǩ
	bool							m_bSelectAll;//�Ƿ�ȫѡ
	bool							m_bSearch;//�Ƿ����
	int								m_nType;//
	std::string						m_sAddtion;//��ѯ����
	int								m_operateType;

public:
	void SetUpBtn(int ID);//��������һ�Ű�ť��״̬
	void PageSwitch(Action action);//�·�һҳ
	void ClearCurrentPage();//��յ�ǰҳ
	void ClickedOneItem(int unitID,int item);//���б����֮�������������е���Ϣ
	void SeeOneItem(int item);
	void DeleteAll();
	void DeleteItems();
	void SetPageFont();//���ý�����������ɫ
	void SelectAll();//ȫ����ѡ
	void Clear();//�������·���
	void Add(CString ch);

	void SetUpPages();//���������ҳ��
	void SetPageTurnBtn(int type = 0);//���÷�ҳ��ť��״̬
	void SetUnitStatus();//����unit��״̬
	int  GetVectorPages();//�õ�������ҳ��
	void ShowTypeInfo();//��ʾÿ�����Ϣ
	void ShowTypeCount();//չʾ����������Ϣ��������
	void ResetTypeInfo();//���½�����Ϣ
	void SetContactInfoFilter(int index);//���ò�ѯ����
	void FromContactInfoDataBase();//����ϵ����������
	
	bool OnBtnSMS();//����Ϣ
	std::string GetType(int groupId = -1);//ͨ������������
	void ShowWindow_();
	void ShowRightBtn(bool bshow);
	void SetType(int type);//��������
	void OnBtnOK();//ȷ��
	void Detail();//����
	bool Warning();//����
	void InsertNum2Phone();//��һ��ͨ�����봫���绰
	void Num2Name(boost::shared_ptr<Data::ContactInfo > pcontactInfo,std::vector<CString> &vname,std::vector<CString> &vnumber);//�����־���ʾ���֣�û������ʾ����
	void GetNumName(std::vector<CString> &vname,std::vector<CString> &vnumber);//�õ����ֺͺ���
	std::string GetName(std::string number, int &groupId);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_)
