#if !defined(DRAFT)
#define DRAFT

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


class CDraftDlg : public CDialog
{
// Construction
public:
	int m_nSelectItem;
	int m_nListCountTotal;
	int m_nPageSize;
	std::string m_strHomeRecordeTelCode; //��ͥ�����ط���

	unsigned int m_uiSelectIndex;
	std::string m_sListFilter;
	std::string m_sListSearchFilter;

	std::vector<boost::shared_ptr<Data::Message> > m_vMessageCurrentResult;
	std::vector<boost::shared_ptr<Data::MMSData> > m_vMMSDataCurrentResult;

//	CCEListCtrl	m_lsList;
	CImageList* m_pImageList;

	SMSBOX_TYPE     m_nBoxType;
	SMSMMS_TYPE     m_nSMSType;
	
	CMJPGStatic		m_MJPGList;
	CDraftDlg(CWnd* pParent = NULL);   // standard constructor


private:
	std::vector<int>				m_vClick;//��ϵ���Ƿ񱻹�ѡ
	std::vector<int>				m_vID;//
	int								m_iCurrentPage;//��ǰ��ҳ
	int								m_iCurrentItems;//��ǰ�м���
	int								m_iTurnOverPages;//��������ҳ 
	int								m_iTotalPages;//һ������ҳ
	bool							m_bSelectAll;//�Ƿ�ȫѡ
	enum Action{
			up_page,
			down_page,
			current_page,
		};
public:
	void FromDataBase();//ȡ�����ݿ�����Ϣ
	void ShowArrayInList();//�����ݿ�����Ϣչ�����б���	
	
	void SetUpBtn(int ID);
	int  GetCurrentItems();//��ǰҳ������
	void PageSwitch(Action action);//ҳ��ת��
	void ClearCurrentPage();//��յ�ǰҳ�ϵ�����
	void SelectAll();//ѡ������
	void DeleteItems();//ɾ����
	void Clear();//������
	void ClickedOneItem(int unitID,int item);//�����һ��
	void ReadOneItem(int item);//�鿴ÿһ�е�����
	
	void ShowWindow_(SMSBOX_TYPE type,SMSMMS_TYPE smsType);//������ʾ��װ
	void ShowSMS(SMSMMS_TYPE smsType);//չʾ����Ϣ���߲���Ϣ
	void SetUnitStatus();//���õ�Ԫ��״̬
	void ShowDeleteDlg();//����ȷ��ɾ���Ļ���
	void SetUpPages();//���������ҳ��
	void SetPagefont();
	int  GetVectorPages();

	void Edit(void);//�༭
	void Transit(void);//ת��
	void Details(void);//����
	void Repeat();//�ط�
	bool GetFirstClickID(int &index,bool bcurrent = false);

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

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
