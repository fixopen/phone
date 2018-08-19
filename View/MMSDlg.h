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
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"
//#include "3GSMSListDlg.h"
//#include "3GSMSDetailDlg.h"

#include "../control/MmsShow.h"

#define MMS_RECIVE_BOX 500
#define MMS_SEND_BOX 500
#define MMS_DRAFT_BOX 500

class CMMSDlg : public CDialog
{
// Construction
public:
	MmsShow			m_MmsShow;	//彩信图片显示控件
	CMJPGStatic		m_MJPGList;
	CMMSDlg(CWnd* pParent = NULL);   // standard constructor

	double	m_MmsSize;  //wangzhenxing20100609
	int m_picCount;
	int m_mp3Count;
	BOOL m_isMid;
	int m_nSelVideo;
	
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

	enum ImageAction{
		insertImage,
		deleteImage,
		saveImage,
	};
	enum AudioAction{
		insertAudio,
		deleteAudio,
		saveAudio,
	};

	enum AudioOperate{
		play,
		stop,
	};

	enum Action{
		up_page,
		down_page,
		current_page,
	};

	CCELineEdit				m_senderEdit1;//发信人1-5
	CCELineEdit				m_senderEdit2;
	CCELineEdit				m_senderEdit3;
	CCELineEdit				m_senderEdit4;
	CCELineEdit				m_senderEdit5;

	CCELineEdit				m_contentEdit;//写短信的内容

	boost::shared_ptr<Data::Message> m_pMessageData;
	boost::shared_ptr<Data::MMSData> m_pMMSData;
	
	std::vector<CString>    m_vTelnum;//发信人的电话号码
	std::map<int,CString>	m_mapTelnum;//保存联系人的电话
	int						m_iCurrentPage;//当前在第几页
	int						m_nInsesrtIndex;//
	int const				pageSize;
	ImageAction				m_nImageAction;//图片动作类型
	AudioAction				m_nAudioAction;//音频文件
	AudioOperate			m_nAudioOperate;
	int n ;
	int m_pageCount;
	int m_currentPage;

public:

	void initDataBase(SMSDETAILTYPE type,  int smsid = -1, BOOL reDraw = FALSE);

	void SetUpBtn(int ID);//设置上面一排按钮的状态
	void PageSwitch(Action action);//翻页
	void GetTelnum(Action action);
	bool SenderIsFull();//当前的发信人的电话是否写满
	void SendMMS();
	void SetSender(std::vector<CString> telnum);//设置发信好人号码
	void SetAppend(std::vector<CString> append);//设置跟名字绑定的号码
	void SaveDraft(bool tip = true);
	void CopyData(CString srcpath,CString despath);//把一个目录下的所有文件拷贝到另一个目录
	void SetMmsContent(boost::shared_ptr<Data::MMSData> pmmsdata ,bool baddname = false);
	void Preview();//预览
	void InsertPicture(CString path);
	void OnBtnPicture();
	void InsertAudio(CString path);//插入音频
	void OnBtnAudio();//图片处理
	void OnBtnPlayAudio();//播放音频
	void StopAudio();
	void GetSender(std::vector<CString> &telnum);
	void SetPage();//
	void Clear();
	void ShowWindow_(int nCmdShow);
	int  GetMMSMaxSize();
	void SetMmsSize(double size);
	void TransferSender(std::vector<CString> Sender);//把短息里的发件人插入到mms
	void TransferTelnum(std::vector<CString> telnum);//
	void FindNumber(std::string &adr);
	void OnBtnCancel();
	void ClearParPage();

	//Mms 
	int  MmsBoxCount(Data::MMSData::Type type);//彩信的数目
	bool SaveMmsData(boost::shared_ptr<Data::MMSData> pmmsdata,Data::MMSData::Type type);
	void SaveMmsData(Data::MMSData *pmmsdata);
	void MmsDelOldest(Data::MMSData::Type type);//删除彩信里最早插入的

	//彩信回复
	void SetReplayStatus();//把界面设置成回复的状态

	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	
protected:
	// Generated message map functions
	//{{AFX_MSG(C3GSMSDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
