#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_40_41__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

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


enum SMSBOX_TYPE{RECV_TYPE = 0, SEND_TYPE, DRAFT_TYPE, HOME_RECORD_TYPE};
enum SMSMMS_TYPE{SMS_TYPE = 0, MMS_TYPE, HOME_TYPE};

#include "3GSMSReadDlg.h"
#include "3GMMSReadDlg.h"
#include "CNumberExtractDlg.h"

class C3GSMSListDlg : public CDialog
{
// Construction
public:
	int m_nSelectItem;
	int m_nListCountTotal;
	int m_nPageSize;
	std::string m_strHomeRecordeTelCode; //家庭留言特服号

	unsigned int m_uiSelectIndex;
	std::string m_sListFilter;
	std::string m_sListSearchFilter;

	std::vector<boost::shared_ptr<Data::Message> > m_vMessageCurrentResult;
	std::vector<boost::shared_ptr<Data::MMSData> > m_vMMSDataCurrentResult;

	void ScrollItemsInList(int step, int nPos);

	void DeleteRefreshList();

	CCEListCtrl	m_lsList;
	CImageList* m_pImageList;

	SMSBOX_TYPE     m_nBoxType;
	SMSMMS_TYPE     m_nSMSType;

	void initType(SMSBOX_TYPE box_type, SMSMMS_TYPE sms_type, BOOL reDraw = FALSE);
	
	CMJPGStatic		m_MJPGList;
	C3GSMSListDlg(CWnd* pParent = NULL);   // standard constructor

	void DoDeleteSMSAll();

private:
	std::vector<int>				m_vClick;//联系人是否被勾选
	std::vector<int>				m_vID;//存储ID
	int								m_iCurrentPage;//当前的页
	int								m_iCurrentItems;//当前有几行
	int								m_iTurnOverPages;//翻过多少页
	int								m_iTotalPages;//总页数
	bool							m_bSelectAll;//是否全选
	enum Action{
			up_page,
			down_page,
			current_page,
		};
public:
	CSMSReadDlg						*m_pSmsReadDlg ;
	CMMSReadDlg						*m_pMmsReadDlg ;
	CNumberExtractDlg				*m_pNumberExtractDlg ;

	void FromDataBase();//取得数据库中信息
	void ShowArrayInList();//把数据库中信息展现在列表中	
	
	void SetUpBtn(int ID);
	void SetHooks();//设置打不打勾
	int  GetCurrentItems();//当前页的行数
	void PageSwitch(Action action);//页面转换
	void ClearCurrentPage();//清空当前页上的数据
	void SelectAll();//选择所有
	void DeleteItems();//删除行
	void DeleteALL();//删除所有
	void Clear();//清数据
	void ClickedOneItem(int unitID,int item);//点击了一行
	void ReadOneItem(int item);//查看每一行的内容

	void ShowWindow_(SMSBOX_TYPE type,SMSMMS_TYPE smsType);//窗口显示封装
	void ShowSMS(SMSMMS_TYPE smsType);//展示短消息或者彩消息
	void SetUnitStatus();//设置单元的状态
	int  GetVectorPages();//
	void SetUpPages();//设置左上角的页
	void ShowDeleteDlg();//展示删除的按钮
	void SetPagefont();
	bool GetFirstClickID(int &index,bool bcurrent = false);
	
	void Replay();
	void Transit();//转发
	void Details();//详情
	void NumberExtract();//号码提取
	void AnalyseSender(std::string number,std::string &name);//
	std::string GetContactName(std::string const number) ;
	
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
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
