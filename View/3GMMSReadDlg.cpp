// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GMMSReadDlg.h"
#include <Winuser.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ExtractNumber(CString content , std::vector<CString> &vnum);

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog

#define  AUTO_MODEL L"�Զ�ģʽ"
#define  MANUAL_MODEL L"�ֶ�ģʽ"

CMMSReadDlg::CMMSReadDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSDlg::IDD, pParent)
{
	m_isReadMid = FALSE;
	m_pageCount = 0;
	m_currentPage = 0;
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
	ON_WM_KEYDOWN()
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMMSReadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_MmsShow.Create(WS_CHILD|WS_VISIBLE, CRect(236, 92, 236+402, 92+270), this, 0, 0);
	m_MmsShow.SetpWnd(this);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\����\\��������.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	m_MJPGList.SetUnitFont(7, font_16);
	m_MJPGList.SetUnitFont(8, font_16);

	m_bAutoPlay = false;

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
		if (m_pMMSData->type == Data::MMSData::tpSend)
		{
			//Repeat();//�ط�  lxz20100828
			main->SetPreADSLDial();
			CString str = L"����Ϣ�Ƿ����·���!";
			main->m_pDeleteTipDlg->SetTitle(str, 10*1000,1);
			main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pDeleteTipDlg->ShowWindow_(TRUE);
		} 
		else if (m_pMMSData->type == Data::MMSData::tpReceive)
		{
			Replay();
		}		
		break;
	
	case  2://�ز�
		DialBack();
		break;
	
	case  3://ת��
		Transit();
		break;
	
	case  4://��ɲݸ�
		SaveDraft();
		break;

	case  5://��ȡ����
		NumberExtract();
		break;
	
	case  10://���Ϸ�������
		PageUp();
		break;

	case  11://���²�������
		PageDown();
		break;

	case  20://����ͼƬ
		SavePicture();
		break;

	case  21://������Ƶ
		SaveAudio();
		break;

	case  22://
		PlayBtn();
// 		m_MmsShow.m_picPathDlg.m_uSaveType = w-20;
// 		m_MmsShow.OnBtnPicture();
		break;

	case 1000:
		if(m_isReadMid)
		{
			main->m_pMMSDlg->StopAudio();
		}
		m_MmsShow.StopVideo();
		m_isReadMid = FALSE;
		m_MmsShow.Clear();
		ShowWindow(SW_HIDE);
		main->PopbackIcon();
		main->IsSendMessage(FALSE);
		main->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		break;

	default:
	    break;

	}
}

void CMMSReadDlg::SetMMSInfo(int id,SMSBOX_TYPE type)					
{	

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd; 
	//unit
	m_MJPGList.SetUnitIsShow(20,false,true);
	m_MJPGList.SetUnitIsShow(21,false,true);
	m_MJPGList.SetUnitIsShow(22,true,true);
	
	m_MJPGList.SetUnitIsDisable(10,false);
	m_MJPGList.SetUnitIsDisable(11,false);
	m_MJPGList.SetUnitText(22,MANUAL_MODEL,true);

	m_bAutoPlay = false;

	CString recIcon = L"" ;//�ռ���ͼ��
	CString sendIcon = L".\\adv\\mjpg\\k5\\common\\����\\������1.bmp";//������ͼ��

	CString repeatIcon[2] = { 
		L".\\adv\\mjpg\\k5\\common\\����\\repeat_fore.bmp",
		L".\\adv\\mjpg\\k5\\common\\����\\repeat_back.bmp",
	};
	
	CString replayIcon[2] = { 
		L".\\adv\\mjpg\\k5\\common\\����\\replay_fore.bmp",
		L".\\adv\\mjpg\\k5\\common\\����\\replay_back.bmp",
		};

	if ( SEND_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,recIcon,L"",true);
		m_MJPGList.SetUnitBitmap(1,repeatIcon[0],repeatIcon[1],false);
		m_MJPGList.SetUnitIsShow(2,false);

	}
	else if ( RECV_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,sendIcon,L"",true);	
		m_MJPGList.SetUnitBitmap(1,replayIcon[0],replayIcon[1],false);
		m_MJPGList.SetUnitIsShow(2,true);
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
			CString adr = Util::StringOp::ToCString(m_pMMSData->SenderAddress);
			temp += L"(" + adr + L")";	
		}
	
		m_MJPGList.SetUnitText(31,temp,false);//�ռ��˻��߷�����

		if ( SEND_TYPE == type || type == DRAFT_TYPE)
		{	
			std::string name ;
			std::string number = Util::StringOp::FromCString(temp);
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp  = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) == L";")
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
		
		temp.Format(L"%d", m_currentPage);
		m_MJPGList.SetUnitText(7, temp, FALSE);
		if(m_pageCount <= 0)
		{
			m_pageCount = 1;
		}
		temp.Format(L"/ %d", m_pageCount);
		m_MJPGList.SetUnitText(8, temp, FALSE);
		
		m_MJPGList.SetUnitIsDisable(10, TRUE);  //��һ�β鿴�Ϸ�ҳ������
		if(m_pageCount <= 1)
		{
			m_MJPGList.SetUnitIsDownStatus(11, FALSE);
			m_MJPGList.SetUnitIsDisable(11, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(11, FALSE);
		}

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
	if(m_currentPage < m_pageCount)
	{
		m_currentPage++;
		CString str;
		str.Format(L"%d", m_currentPage);
		m_MJPGList.SetUnitText(7, str, TRUE);
	}

	if(m_currentPage >= m_pageCount)
	{
		m_currentPage = m_pageCount;
		m_MJPGList.SetUnitIsDisable(11, TRUE);
	}

	if(m_currentPage > 1)
	{
		m_MJPGList.SetUnitIsDisable(10, FALSE);
	}
}

void CMMSReadDlg::PageUp()
{
	m_MmsShow.OnBtnUp();
	if(m_currentPage > 1)
	{
		m_currentPage--;
		CString str;
		str.Format(L"%d", m_currentPage);
		m_MJPGList.SetUnitText(7, str, TRUE);
	}

	if(m_currentPage <= 1)
	{
		m_currentPage = 1;
		m_MJPGList.SetUnitIsDisable(10, TRUE);
	}

	if(m_currentPage < m_pageCount)
	{
		m_MJPGList.SetUnitIsDisable(11, FALSE);
	}
}

void CMMSReadDlg::Replay(void)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if ( RECV_TYPE == m_nBoxType )//ֻ���ռ���ʱ����ܻظ�
	{	
		std::vector<CString> vtel;
		std::vector<CString> vappend;
		
		if (m_pMMSData)
		{	
			if(m_isReadMid)
			{
				main->m_pMMSDlg->StopAudio();
			}
			std::string sender = m_pMMSData->SenderAddress;
			std::string name = main->GetName(sender);
		//	CString name = m_MJPGList.GetUnitText(31);
			if (!sender.empty())
			{	
				//vtel.push_back(name);
				if (name.empty())
				{
					name = sender;
				}
				vtel.push_back(Util::StringOp::ToCString(name));

				vappend.push_back(Util::StringOp::ToCString(sender));
				main->m_pMMSDlg->Clear();
				main->m_pMMSDlg->SetSender(vtel);
				main->m_pMMSDlg->SetAppend(vappend);

				main->AddIcon(Allicon[1],main->m_pMMSDlg,false);
				main->m_pMMSDlg->ClearParPage();
				main->m_pMMSDlg->SetReplayStatus();
				main->m_pMMSDlg->ShowWindow(true);
				//main->m_pMMSDlg->SetMmsSize(0);

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
		if(m_isReadMid)
		{
			main->m_pMMSDlg->StopAudio();
			m_isReadMid = FALSE;
		}
	
		main->m_pMMSDlg->SetMmsContent(m_pMMSData);

		main->AddIcon(Allicon[1],main->m_pMMSDlg,false);
		main->m_pMMSDlg->ClearParPage();
		main->m_pMMSDlg->ShowWindow(SW_SHOW);
		main->m_pMMSDlg->m_MmsShow.AllFileSize();

	//	ShowWindow(SW_HIDE);
	}
	
}

void CMMSReadDlg::Repeat()
{	
	if (m_pMMSData)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		boost::shared_ptr<Data::MMSData> pmmsdata = boost::shared_ptr<Data::MMSData>(new Data::MMSData) ;
		
		if(m_isReadMid)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMMSDlg->StopAudio();
			m_isReadMid = FALSE;
		}

		m_MmsShow.StopVideo();

		//�жϴ��̿ռ�
		if (main->IsDiskFull())
		{
			return;
		}

		if (pmmsdata)
		{
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			CTime ct = CTime(tm);
			CTimeSpan sp = ct - CTime(1970, 1, 1, 0, 0, 0);
			
			char datetime[24];
			sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
			
			*pmmsdata = *m_pMMSData;
			pmmsdata->type = Data::MMSData::tpReadySend;
			pmmsdata->isRead = false; //��ʾ����ʧ��
			pmmsdata->TransactionId = datetime;
			pmmsdata->DateAndTime = sp.GetTotalSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
			//pmmsdata->Insert();
			main->m_pMMSDlg->SaveMmsData(pmmsdata,Data::MMSData::tpSend);
			main->m_isDialTimeout = 0;

			main->PopIcon(this);
			
		}
	}

}

void CMMSReadDlg::DialBack()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	if ( RECV_TYPE == m_nBoxType )//ֻ���ռ���ʱ����ܻظ�
	{
		if (m_pMMSData)
		{
			main->m_pMMSDlg->StopAudio();
			std::string telnum ;
			telnum = m_pMMSData->SenderAddress;
			
			CString contact = Util::StringOp::ToCString(telnum);
			main->m_pSMSListDlg->m_pSmsReadDlg->CheckChatacter(contact);
			if ( 0 == contact.Mid(0,2).Compare(L"86"))
			{
				contact = contact.Mid(2);	
			}
			
			main->m_pTelephoneDlg->Dialback(Util::StringOp::FromCString(contact));
			main->m_pTelephoneDlg->ShowWindow(SW_SHOW);
		}
	}

}

void CMMSReadDlg::SaveDraft()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	//if (!m_pMMSData)
	if (m_pMMSData)  //wangzhenxing20100605
	{
		boost::shared_ptr<Data::MMSData> pmessge = boost::shared_ptr<Data::MMSData>(new Data::MMSData);
		*pmessge = *m_pMMSData;
		
		//add 20100919
		std::string address = pmessge->RecipientAddress;
		if (!address.empty() && 
			address.substr(address.size()-1,1).compare(";") != 0)
		{
			pmessge->RecipientAddress = address + ";";
		}
		//

		pmessge->type = Data::MMSData::tpUnSend ;
		bool bt = main->m_pMMSDlg->SaveMmsData(pmessge,Data::MMSData::tpUnSend);

		CString title = L"����ɲݸ�ɹ�!";
		if (!bt)
		{
			title = L"����ɲݸ�ʧ��!";
		}	
		main->m_pDeleteTipDlg->SetTitle(title,3000);
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

}

void CMMSReadDlg::SaveAudio()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CString AudioPath ;
	CString sourcePath ;
	CString filename ;
	CString destPath;
	int index ;
	AudioPath = m_MmsShow.GetAudio();
	
	if ((index = AudioPath.ReverseFind('/')) != -1)
	{
		filename = AudioPath.Mid(index+1);
	}
	
	//add by qi 20100828 �����Ƶ�ļ�����

	CreateDirectoryW(L"FlashDrv/MY_MUSIC/", 0);
	destPath = L"FlashDrv/MY_MUSIC/" + filename ;
			
	if ( 0 != CopyFile(AudioPath,destPath,FALSE))
	{			
		main->m_pTipDlg->SetTitle(filename + L"�ѳɹ����浽�ҵ�����!",3000);
		main->m_pTipDlg->ShowWindow_(SW_SHOW);
	}
	else 
	{
		main->m_pDeleteTipDlg->SetTitle(L"����ʧ��!",2000);
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

}

void CMMSReadDlg::SavePicture()
{	
CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CString sourcePath;
	CString VideoPath ;
	CString filename ;
	CString destPath;
	int index ;
	m_MmsShow.GetImage(sourcePath);
	VideoPath = m_MmsShow.GetVideo();
	
	//add by qi 20100828 �����Ƶ�ļ�����
	if(!sourcePath.IsEmpty())
	{
		if ((index = sourcePath.ReverseFind('/')) != -1)
		{
			filename = sourcePath.Mid(index+1);
		}
		CreateDirectoryW(L"FlashDrv/MY_PHOTO/", 0);
		destPath = L"FlashDrv/MY_PHOTO/" + filename ;
		if ( 0 != CopyFile(sourcePath,destPath,FALSE))
		{
			main->m_pTipDlg->SetTitle(filename + L"�ѳɹ����浽�ҵ�ͼƬ!",3000);
			main->m_pTipDlg->ShowWindow_(SW_SHOW);
			
		}
		else 
		{
			main->m_pDeleteTipDlg->SetTitle(L"����ʧ��!",2000);
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
	}
	else if(!VideoPath.IsEmpty())
	{
		if ((index = VideoPath.ReverseFind('/')) != -1)
		{
			filename = VideoPath.Mid(index+1);
		}
		CreateDirectoryW(L"FlashDrv/MY_VIDEO/", 0);
		destPath = L"FlashDrv/MY_VIDEO/" + filename ;
		if ( 0 != CopyFile(VideoPath,destPath,FALSE))
		{
			main->m_pTipDlg->SetTitle(filename + L"�ѳɹ����浽�ҵ���Ƶ!",3000);
			main->m_pTipDlg->ShowWindow_(SW_SHOW);
			
		}
		else 
		{
			main->m_pDeleteTipDlg->SetTitle(L"����ʧ��!",2000);
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
	}

}

void CMMSReadDlg::MMSPreview(std::wstring path,std::wstring title)
{
	m_MmsShow.FindFileSmil(path);
	m_MmsShow.SetTitle(title);
	m_MmsShow.ShowWindow(SW_SHOW);

	m_MJPGList.SetUnitIsShow(22,true,true);
	m_MJPGList.SetUnitText(22,MANUAL_MODEL,true);
	
	m_bAutoPlay = false;

	CString temp ;
	temp.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(7, temp, FALSE);
	if(m_pageCount <= 0)
	{
		m_pageCount = 1;
	}
	temp.Format(L"/ %d", m_pageCount);
	m_MJPGList.SetUnitText(8, temp, FALSE);
	
	m_MJPGList.SetUnitIsDisable(10, TRUE);  //��һ�β鿴�Ϸ�ҳ������
	if(m_pageCount <= 1)
	{
		m_MJPGList.SetUnitIsDownStatus(11, FALSE);
		m_MJPGList.SetUnitIsDisable(11, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(11, FALSE);
	}

	//add 20100919
		m_MJPGList.SetUnitText(31,L"",false);
		m_MJPGList.SetUnitText(32,(CString)title.c_str(),false);
		m_MJPGList.SetUnitText(33,L"",false);
	//
	
	m_MJPGList.Invalidate();
}

void CMMSReadDlg::NumberExtract()
{	
	CMultimediaPhoneDlg*main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector<CString> vNumber ;	
	CString contact ; 
	CString content ;
	CString title;
	
	if (m_pMMSData)
	{	
		if ( SEND_TYPE == m_nBoxType)
		{	
			contact   = Util::StringOp::ToCString(m_pMMSData->RecipientAddress);
		}
		else
		{	
			contact   = Util::StringOp::ToCString(m_pMMSData->SenderAddress);
		}

		main->m_pSMSListDlg->m_pSmsReadDlg->CheckChatacter(contact);
		//wangzhenxing20100728
		ExtractNumber(contact, vNumber);//��ȡ������
				
		title = Util::StringOp::ToCString(m_pMMSData->Subject);
		ExtractNumber(title,vNumber);//��ȡ����
		
		main->m_pSMSListDlg->GetPathTxt(Util::StringOp::ToCString(m_pMMSData->SavePath),content);
		if (!content.IsEmpty())//��ȡ����
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
	
}

void CMMSReadDlg::PlayBtn()
{
	if (!m_bAutoPlay)
	{	
		m_MmsShow.AutoPlay(true);
		m_bAutoPlay = true ;
		m_MJPGList.SetUnitIsDisable(10,true);
		m_MJPGList.SetUnitIsDisable(11,true);
		m_MJPGList.SetUnitText(22, AUTO_MODEL,true);
	}
	else
	{
		m_MmsShow.AutoPlay(false);
		m_bAutoPlay = false;
		m_MJPGList.SetUnitIsDisable(10,false);
		m_MJPGList.SetUnitIsDisable(11,false);
		m_MJPGList.SetUnitText(22, MANUAL_MODEL,true);
	}
}

void CMMSReadDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 'U')
	{
		MMSPageUp();
	}
	else if(nChar == 'D')
	{
		MMSPageDown();
	}
}

void CMMSReadDlg::MMSPageUp()
{
	m_MmsShow.TextScrollUp();
}

void CMMSReadDlg::MMSPageDown()
{	
	m_MmsShow.TextScrollDown();
}

void CMMSReadDlg::SetReadPage()
{
	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(7, str, TRUE);
}

LRESULT CMMSReadDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (message)
	{
		case WM_IMAGE:
			{	
				CString image[2] = {
					L".\\adv\\mjpg\\k5\\common\\����\\����ͼƬ_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\����\\����ͼƬ_back.bmp",
				};

				if ( 2 == wParam)//����ͼƬ
				{
					m_MJPGList.SetUnitBitmap(20,image[0],image[1],true);
					m_MJPGList.SetUnitIsShow(20,true,true);
				}
				else if (3 == wParam)//���ذ�ť
				{
					m_MJPGList.SetUnitBitmap(20,L"",L"",true);
					m_MJPGList.SetUnitIsShow(20,false,true);
				}

			}
			break;

		case WM_AUDIO:
			{	
				CString audio[2][2] = {
					{
						L".\\adv\\mjpg\\k5\\common\\����\\������Ƶ_fore.bmp",
						L".\\adv\\mjpg\\k5\\common\\����\\������Ƶ_back.bmp",
					},
					{
						L".\\adv\\mjpg\\k5\\common\\����\\����_fore.bmp",
						L".\\adv\\mjpg\\k5\\common\\����\\����_back.bmp",
					}
				};

				if ( 2 == wParam)//������Ƶ
				{	
					m_MJPGList.SetUnitBitmap(21,audio[0][0],audio[0][1],true);
					m_MJPGList.SetUnitIsShow(21,true,true);
				}
				else if (3 == wParam)//������Ƶ
				{
					
				}
				else if (4 == wParam)//���水ť��ʧ
				{
					m_MJPGList.SetUnitIsShow(21,false,true);	
				}
				else if (5 == wParam)//������ť��ʧ
				{
				//	m_MJPGList.SetUnitIsShow(22,false,true);	
				}
				m_MJPGList.Invalidate();
			}
			break;
		case WM_SEND_MMS:
			if(1 == wParam)
			{
				if(0 == lParam)    //lxz 20100828
				{
					Repeat();
				}
				else			//lxz 20100828
				{
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->UnSetPreADSLDial();
				}
			}
			break;
		default:
			break;
	}
	return CDialog::WindowProc(message, wParam, lParam);

}

void CMMSReadDlg::RefreshName(std::string name)
{
	//m_MJPGList.SetUnitText(31, Util::StringOp::ToCString(name), TRUE);
	//m_MJPGList.Invalidate();
}

