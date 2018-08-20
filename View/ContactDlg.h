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
	
	CMJPGStatic		m_MJPGData_key;//数字键盘. add by qi 2009_09_22
	CMJPGStatic		m_MJPGLetter_key;//字母键盘. add by qi 2009_09_22

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

	const std::string				m_sSimName;//sim卡的组的名字
	std::vector<int>				m_vClick;//联系人是否被勾选
	std::vector<int>				m_vContactID;//保存联系人的ID
	int								m_iCurrentPage;//当前的页
	int								m_iTotalPages;//一个多少页
	bool							m_bSelectAll;//是否全选
	bool							m_bSearch;//是否查找
	int								m_iUpCurrentPage;//左右翻页
	int								m_iUpAllCount;//上面的总数
	std::string						m_sAddtion;//查询条件
	std::string						m_sNumber;
	int								m_index;
	std::vector<CString>			m_vNumber;//保存号码
	std::vector<CString>            m_v4Number;//保存号码
	std::vector<CString>			m_vName;//保存名字
	CString							m_sName;//联系人的名字
	
	int								m_nSimCapacity;//SIM卡容量
	int								m_iSelectNum;//选择的号码数
	int								m_ntype;//类型
	int								m_iCurrentGroupID;//当前的组ID
	int								m_iSimID;//SIM组在数据库的ID
	const int						m_iBindCount;//绑定的数目
	const int						m_defGroupID;//默认的组ID
	const int						m_iFriendID;//朋友组在数据库的ID
	const int						m_iClassMate;//同学组在数据库的ID
	CSIMImportDlg					m_simImportDlg;//SIM导入
	CSelectNumberDlg				m_pSelectNumberDlg;//选择号码

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

	void SetUpBtn(int ID);//设置上面一排按钮的状态
	void PageSwitch(Action action);//下翻一页
	void ClearCurrentPage();
	void ClickedOneItem(int unitID,int item);//这行被点击之后，重新设置这行的信息
	void DeleteItems();
	void DeleteAll();
	void SetPageFont();//设置界面的字体和颜色
	void SelectAll();//全部勾选
	void InsertInfo(std::string name , std::string phone);//插入信息
	void Clear();//数据重新分配
	void Add(CString ch);//按数字处理 
	void HandleSimImport(WPARAM wp);//处理SIM 和 LOCAL之间的互倒
	std::string GetAlias(std::string name);//别名
	void ShowDeleteDlg();//弹出确认删除的画面
	void OnBtnEditContact(int item);//编辑联系人
	void ShowNewContact(int groupID);//查找该组ID在组中的第几页，第几列。
	void SetVector();//初始化ID容器和 是否被点击容器 

	void SetUpPages();//设置上面的页数
	void SetUnitStatus();//设置unit的状态
	int  GetVectorPages();//得到容器的页数
	void ShowGroupInfo();//显示每组的信息
	void SetContactFilter(int index);//设置查询条件
	void FromContactDataBase();//从联系人里获得数据

	void ClearUp();//清空上面组的信息
	void ClearUpGroup();//清除上面的组的Unit
	void SetUpGroup();//设置上面的组的Unit
	void FromGroupDataBase();//取得组全部内容
	void ShowUpGroupInfo();//显示上面组的内容
	void PageUpSwitch(Action action);//上面的左右翻页
	int	 GetUpGroupNum();//得到上面组的当前页的数目
	void ShowWindow_();
	void ShowRightBtn(bool bshow);//是否显示右边的按钮
	void Contact2Sms();//把联系人的插入到SIM名片中
	void SetType(int type);//设置类型
	void SetNumber(std::string num,int index);
	void AddNumbertoContact();//
	void OnBtnOK();
	void HandleNumber(LPARAM lParam);//处理选择的号码
	void SetSimID(int id);
	int	 GetSimID();
	void BackupSim();
	int  GetSimCapacity();
	std::string GetContactName(std::string const number);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
