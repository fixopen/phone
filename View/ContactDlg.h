#if !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
#define AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/SoftKey.h"
#include "../Data/Contact.h"
#include "ContactSearchDlg.h"

#include "SIMImportDlg.h"

#include "../control/MJPGStatic.h"
#include "../control/CEDialog.h"
#include "CSelectNumberDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

class CContactDlg : public CDialog
{
// Construction
public:
	CContactDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactDlg)
	enum { IDD = IDD_DIALOG_CONTACT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonContactNew();
	afx_msg void OnButtonContactNewtype();
	afx_msg void OnButtonContactEdittype();
	afx_msg void OnButtonContactSearch();
	afx_msg void OnButtonContactClose();
	afx_msg LRESULT OnSoftKeyMessage(WPARAM wParam, LPARAM lParam);   
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPG_Toggle(WPARAM w, LPARAM l);

	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
private:
	
	CMJPGStatic		m_MJPGData_key;//���ּ���. add by qi 2009_09_22
	CMJPGStatic		m_MJPGLetter_key;//��ĸ����. add by qi 2009_09_22

	CCEStatic	m_sticPinYin;
	CCEListCtrl	m_lsType;
	CCEListCtrl	m_lsList;
	CSoftKey    m_skSoftKey;
	CImageList* m_pImageList;
/*
	CCEBmpButton m_btnNewContact;
	CCEBmpButton m_btnNewType;
	CCEBmpButton m_btnEditType;
	CCEBmpButton m_btnSearch;
	CCEBmpButton m_btnClose;
*/	

	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
public:
	
//	CSIMImportDlg     m_pSimImportDlg;

	int GetContactTotal() { return ContactTotal; };

public:
	CMJPGStatic		m_MJPGList;

	const int		ContactTotal;
	const int		ContactGroupTotal;
	const int		PageSize;
private:
	std::string m_sListFilter;
	std::string m_sListSearchFilter;
	
	std::vector<boost::shared_ptr<Data::Contact> >		m_vCurrentResult;
	std::vector<boost::shared_ptr<Data::ContactGroup> > m_vGroupResult;

	void ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array);
	
	std::string GetPYIndex(CString content);// change by qi 2009_09_22

	CContactSearchDlg* m_pContactSearchDlg;
	void ScrollItemsInList(int step, int nPos);

public:
	void ShowItemsInList(void);
	void ShowTypeItems(void);
	int	 GetTypeListSelected(void);
	void Search(std::string filter);

//add by qi 2009_09_17
private:

	const std::string				m_sSimName;//sim�����������
	std::vector<int>				m_vClick;//��ϵ���Ƿ񱻹�ѡ
	std::vector<int>				m_vContactID;//������ϵ�˵�ID
	int								m_iCurrentPage;//��ǰ��ҳ
	int								m_iTotalPages;//һ������ҳ
	bool							m_bSelectAll;//�Ƿ�ȫѡ
	bool							m_bSearch;//�Ƿ����
	int								m_iUpCurrentPage;//���ҷ�ҳ
	int								m_iUpAllCount;//���������
	std::string						m_sAddtion;//��ѯ����
	std::string						m_sNumber;
	int								m_index;
	std::vector<CString>			m_vNumber;//�������
	std::vector<CString>            m_v4Number;//�������
	std::vector<CString>			m_vName;//��������
	CString							m_sName;//��ϵ�˵�����
	
	int								m_nSimCapacity;//SIM������
	int								m_iSelectNum;//ѡ��ĺ�����
	int								m_ntype;//����
	int								m_iCurrentGroupID;//��ǰ����ID
	int								m_iSimID;//SIM�������ݿ��ID
	const int						m_iBindCount;//�󶨵���Ŀ
	const int						m_defGroupID;//Ĭ�ϵ���ID
	const int						m_iFriendID;//�����������ݿ��ID
	const int						m_iClassMate;//ͬѧ�������ݿ��ID
	CSIMImportDlg					m_simImportDlg;//SIM����
	CSelectNumberDlg				m_pSelectNumberDlg;//ѡ�����

public:
	enum Action{
			up_page,
			down_page,
			current_page,
			left_page,
			right_page,
			up_item,
			down_item,
		};

	void SetUpBtn(int ID);//��������һ�Ű�ť��״̬
	void PageSwitch(Action action);//�·�һҳ
	void ClearCurrentPage();
	void ClickedOneItem(int unitID,int item);//���б����֮�������������е���Ϣ
	void DeleteItems();
	void DeleteAll();
	void SetPageFont();//���ý�����������ɫ
	void SelectAll();//ȫ����ѡ
	void InsertInfo(std::string name , std::string phone);//������Ϣ
	void Clear();//�������·���
	void Add(CString ch);//�����ִ��� 
	void HandleSimImport(WPARAM wp);//����SIM �� LOCAL֮��Ļ���
	std::string GetAlias(std::string name);//����
	void ShowDeleteDlg();//����ȷ��ɾ���Ļ���
	void OnBtnEditContact(int item);//�༭��ϵ��
	void ShowNewContact(int groupID);//���Ҹ���ID�����еĵڼ�ҳ���ڼ��С�
	void SetVector();//��ʼ��ID������ �Ƿ񱻵������ 

	void SetUpPages();//���������ҳ��
	void SetUnitStatus();//����unit��״̬
	int  GetVectorPages();//�õ�������ҳ��
	void ShowGroupInfo();//��ʾÿ�����Ϣ
	void SetContactFilter(int index);//���ò�ѯ����
	void FromContactDataBase();//����ϵ����������

	void ClearUp();//������������Ϣ
	void ClearUpGroup();//�����������Unit
	void SetUpGroup();//������������Unit
	void FromGroupDataBase();//ȡ����ȫ������
	void ShowUpGroupInfo();//��ʾ�����������
	void PageUpSwitch(Action action);//��������ҷ�ҳ
	int	 GetUpGroupNum();//�õ�������ĵ�ǰҳ����Ŀ
	void ShowWindow_();
	void ShowRightBtn(bool bshow);//�Ƿ���ʾ�ұߵİ�ť
	void Contact2Sms();//����ϵ�˵Ĳ��뵽SIM��Ƭ��
	void SetType(int type);//��������
	void SetNumber(std::string num,int index);
	void AddNumbertoContact();//
	void OnBtnOK();
	void HandleNumber(LPARAM lParam);//����ѡ��ĺ���
	void SetSimID(int id);
	int	 GetSimID();
	void BackupSim();
	int  GetSimCapacity();
	std::string GetContactName(std::string const number);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
