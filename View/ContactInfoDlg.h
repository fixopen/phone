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
	CMJPGStatic		m_MJPGData_key;//数字键盘
 	CMJPGStatic		m_MJPGLetter_key;//字母键盘

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

	std::vector<int>				m_vClick;//联系人是否被勾选
	std::vector<int>				m_vContactID;//保存联系人的ID
	int								m_iCurrentPage;//当前的页
	int								m_iTotalPages;//一个多少页
	int								m_uiType ;
	int								m_iTabs;//页签
	bool							m_bSelectAll;//是否全选
	bool							m_bSearch;//是否查找
	int								m_nType;//
	std::string						m_sAddtion;//查询条件
	int								m_operateType;

public:
	void SetUpBtn(int ID);//设置上面一排按钮的状态
	void PageSwitch(Action action);//下翻一页
	void ClearCurrentPage();//清空当前页
	void ClickedOneItem(int unitID,int item);//这行被点击之后，重新设置这行的信息
	void SeeOneItem(int item);
	void DeleteAll();
	void DeleteItems();
	void SetPageFont();//设置界面的字体和颜色
	void SelectAll();//全部勾选
	void Clear();//数据重新分配
	void Add(CString ch);

	void SetUpPages();//设置上面的页数
	void SetPageTurnBtn(int type = 0);//设置翻页按钮的状态
	void SetUnitStatus();//设置unit的状态
	int  GetVectorPages();//得到容器的页数
	void ShowTypeInfo();//显示每组的信息
	void ShowTypeCount();//展示这种类型信息的中条数
	void ResetTypeInfo();//重新界面信息
	void SetContactInfoFilter(int index);//设置查询条件
	void FromContactInfoDataBase();//从联系人里获得数据
	
	bool OnBtnSMS();//短消息
	std::string GetType(int groupId = -1);//通过号码获得组名
	void ShowWindow_();
	void ShowRightBtn(bool bshow);
	void SetType(int type);//设置类型
	void OnBtnOK();//确定
	void Detail();//详情
	bool Warning();//警告
	void InsertNum2Phone();//把一个通话号码传至电话
	void Num2Name(boost::shared_ptr<Data::ContactInfo > pcontactInfo,std::vector<CString> &vname,std::vector<CString> &vnumber);//有名字就显示名字，没名字显示号码
	void GetNumName(std::vector<CString> &vname,std::vector<CString> &vnumber);//得到名字和号码
	std::string GetName(std::string number, int &groupId);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_)
