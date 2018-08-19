// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GMMSReadDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


CMMSReadDlg::CMMSReadDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSDlg::IDD, pParent)
{

}

void CMMSReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMMSReadDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMMSReadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_MmsShow.Create(WS_CHILD|WS_VISIBLE, CRect(236, 92, 236+419, 92+239), this, 0, 0);
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\����\\��������.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	MoveWindow(0,57,800,423);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CMMSReadDlg::PreTranslateMessage(MSG* pMsg)
{

	return CDialog::PreTranslateMessage(pMsg);

}
void CMMSReadDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon ;
	switch(w)
	{
	case  1://�ظ�
	//	Replay();
		break;
	
	case  2://�ز�
	//	DialBack();
		break;
	
	case  3://ת��
		Transit();
		break;
	
	case  4://��ɲݸ�
	//	SaveDraft();
		break;

	case  5://��ȡ����
	//	NumberExtract();
		break;
	
	case  20://���Ϸ�������
		PageUp();
		break;

	case  21://���²�������
		PageDown();
		break;

	case 1000:
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//�ָ���Ƶ
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		ShowWindow(SW_HIDE);

		break;

	default:
	    break;

	}
}

void CMMSReadDlg::SetMMSInfo(int id,SMSBOX_TYPE type)					
{	

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd; 
	CString recIcon = L"" ;//�ռ���ͼ��
	CString sendIcon = L".\\adv\\mjpg\\k5\\common\\����\\������1.bmp";//������ͼ��
	if ( SEND_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,recIcon,L"",true);
	}
	else if ( RECV_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,sendIcon,L"",true);		
	}
	m_nBoxType = type ;

	//������ʾ
	m_pMMSData = Data::MMSData::GetDataById(id);
	if (m_pMMSData)
	{		
		CString temp ;
		std::string  filter;
		if ( SEND_TYPE == type)
		{	
			temp   = Util::StringOp::ToCString(m_pMMSData->RecipientAddress);
			filter = m_pMMSData->RecipientAddress;
		}
		else
		{	
			temp   = Util::StringOp::ToCString(m_pMMSData->SenderAddress);
			filter = m_pMMSData->SenderAddress;
		}
		std::string name = main->GetName(filter);
		if (!name.empty())
		{
			temp = Util::StringOp::ToCString(name);	
		}
	
		m_MJPGList.SetUnitText(31,temp,false);//�ռ��˻��߷�����

		if ( SEND_TYPE == type || type == DRAFT_TYPE)
		{	
			std::string name ;
			std::string number = Util::StringOp::FromCString(temp);;
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp  = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) = L";")
			{
				temp = temp.Mid(0,temp.GetLength()-1);
			}
			m_MJPGList.SetUnitText(31,temp,false);
		}

		temp = Util::StringOp::ToCString(m_pMMSData->Subject);
		m_MJPGList.SetUnitText(32,temp,false);//����

		CString s = m_pMMSData->SavePath.c_str();//��ʾ����		
		CString filename = "";
		CString path = "";
		extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
		FindFileEx(s, path, filename);
		if(path != "")
		{
			m_MmsShow.FindFileSmil(path.GetBuffer(256));
			m_MmsShow.SetTitle(MMS::MMSWarp::GetMMSWarp()->ToUnicode((m_pMMSData->Subject)));
			m_MmsShow.ShowWindow(SW_SHOW);
		}
		
		CTime tm = CTime(1970, 1, 1, 0, 0, 0);
		tm += CTimeSpan(0, 0, 0, m_pMMSData->DateAndTime);
		temp = Util::StringOp::FromTimestampFormat(tm).c_str();
		temp = temp.Mid(2);
		m_MJPGList.SetUnitText(33, temp, false);//ʱ��
		
		m_MJPGList.Invalidate();
	}
	
	if ( RECV_TYPE == m_nBoxType )
	{
		if (!(m_pMMSData->isRead))
		{
			m_pMMSData->isRead = true;
			m_pMMSData->Update();		
			::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
		}
	}

}

void CMMSReadDlg::PageDown()
{
	m_MmsShow.OnBtnNext();
}

void CMMSReadDlg::PageUp()
{
	m_MmsShow.OnBtnUp();
}

void CMMSReadDlg::Replay(void)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if ( RECV_TYPE == m_nBoxType )//ֻ���ռ���ʱ����ܻظ�
	{	
		std::vector<CString> vtel;
		std::vector<CString> vappend;
		
		if (!m_pMMSData)
		{	
			std::string sender = m_pMMSData->SenderAddress;
			CString name = m_MJPGList.GetUnitText(31);
			if (!sender.empty())
			{	
				vtel.push_back(name);
				vappend.push_back(Util::StringOp::ToCString(sender));
				main->m_pMMSDlg->SetSender(vtel);
				main->m_pMMSDlg->SetAppend(vappend);
				main->m_pMMSDlg->ShowWindow(true);
			}

		}			
	}

}

void CMMSReadDlg::Transit()//ת��
{
	//ֻ��ѡ��һ��
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	
	if (m_pMMSData)
	{
		main->m_pMMSDlg->SetMmsContent(m_pMMSData);
		main->m_pMMSDlg->ShowWindow(SW_SHOW);
	}
	
}

void CMMSReadDlg::DialBack()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	if (!m_pMMSData)
	{
		std::string telnum ;
		telnum = m_pMMSData->SenderAddress;
		main->m_pTelephoneDlg->Dialback(telnum);
		main->m_pTelephoneDlg->ShowWindow(SW_SHOW);
	}
}

void CMMSReadDlg::SaveDraft()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!m_pMMSData)
	{
		boost::shared_ptr<Data::MMSData> pmessge ;
		pmessge = m_pMMSData;
		pmessge->type = Data::MMSData::tpUnSend ;
		bool bt = pmessge->Insert();

		CString title = L"����ɲݸ�ɹ�!";
		if (!bt)
		{
			title = L"����ɲݸ�ʧ��!";
		}	
		main->m_pDeleteTipDlg->SetTitle(title,3000);
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

}
void CMMSReadDlg::MMSPreview(std::wstring path,std::wstring title)
{
	m_MmsShow.FindFileSmil(path);
	m_MmsShow.SetTitle(title);
	m_MmsShow.ShowWindow(SW_SHOW);
}

void CMMSReadDlg::NumberExtract()
{	
	CMultimediaPhoneDlg*main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector<CString> vNumber ;
	CString content ;
	content = m_MJPGList.GetUnitText(31);
	vNumber.push_back(content);
	
	m_contentEdit.GetWindowText(content);
	void ExtractNumber(CString content , std::vector<CString> &vnum);
	if (!content.IsEmpty())
	{
		ExtractNumber(content,vNumber);
	}
	
	if (vNumber.size())
	{	
		main->m_pSMSListDlg->m_pNumberExtractDlg->SetNumber(vNumber);
		main->m_pSMSListDlg->m_pNumberExtractDlg->ShowNumber();
		main->m_pSMSListDlg->m_pNumberExtractDlg->ShowWindow_(SW_SHOW);
	}

}