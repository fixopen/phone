#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_40__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog
#include "../control/MJPGStatic.h"
#include "../control/CEStatic.h"
#include "3GSMSDetailDlg.h"
#include "../Data/Message.h"
#include "10PhrasesDlg.h"

#define RECEIVE_BOX_SIZE 1000
#define SEND_BOX_SIZE 1000

class CSMSDlg : public CDialog
{
// Construction
public:
//	C3GSMSListDlg   *m_pSMSListDlg;

	CMJPGStatic		m_MJPGList;
	CSMSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C3GSMSDlg)
	enum { IDD = IDD_DIALOG_MMS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GSMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

private:
	
	CCELineEdit				m_senderEdit1;//������1-5
	CCELineEdit				m_senderEdit2;
	CCELineEdit				m_senderEdit3;
	CCELineEdit				m_senderEdit4;
	CCELineEdit				m_senderEdit5;

	CCELineEdit				m_contentEdit;//д���ŵ�����

	boost::shared_ptr<Data::Message> m_pMessageData;
	boost::shared_ptr<Data::MMSData> m_pMMSData;
	
	std::vector<CString>    m_vTelnum;//�����˵ĵ绰����
	std::map<int,CString>	m_mapTelnum;//������ϵ�˵ĵ绰
	int						m_iCurrentPage;//��ǰ�ڵڼ�ҳ
	int const				pageSize;
	int						m_charNumber;
	int						m_charMax;
	int						m_msgNumber;
	int						m_nInsesrtIndex;//�������ϵ�˵�����
	C10PhrasesDlg			*m_p10PhrasesDlg;//10��ͨ����¼
	enum Action{
		up_page,
		down_page,
		current_page,
		};
public:

	void initDataBase(SMSDETAILTYPE type,  int smsid = -1, BOOL reDraw = FALSE);

	void SetUpBtn(int ID);//��������һ�Ű�ť��״̬
	void PageSwitch(Action action);//��ҳ
	void GetTelnum(Action action);
	bool SenderIsFull();//��ǰ�ķ����˵ĵ绰�Ƿ�д��
	void SendSMS();
	void SaveDraft(bool tip = true);//��ɲݸ�
	void SetSender(std::vector<CString> telnum);//���÷��ź��˺���
	void SetAppend(std::vector<CString> append);

	void InsertPhrases(CString phrases);//���볣�ö���
	void SMSPageUp();//���·�ҳ
	void SMSPageDown();//���Ϸ�ҳ
	void Show10Phrases();//��ʾ10������
	void SetSmsContent(std::string content);
	void SetMessge(boost::shared_ptr<Data::Message> pmessage);
	void ShowWindow_(int nCmdShow );
	void GetSender(std::vector<CString> &telnum);
	void Clear();
	void SetPage();
	
	void OnCharNumberChange();
	void TransferSender(std::vector<CString> Sender);
	void TransferTelnum(std::vector<CString> telnum);//

	void FindNumber(std::string &adr);//������ϵ��
	void OnBtnCancel();

	int  SmsBoxCount(Data::Message::Group group);//���ŷ����������
	bool SaveSmsData(boost::shared_ptr<Data::Message> pmessage,Data::Message::Group group);
	void SaveSmsData(Data::Message *pmessage);
	void SmsDelOldest(Data::Message::Group group);//ɾ�����ϵ�һ��

	static void SingleQuotes(CString &content);

	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Generated message map functions
	//{{AFX_MSG(C3GSMSDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
