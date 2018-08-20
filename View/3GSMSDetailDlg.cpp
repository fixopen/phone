// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSMSDetailDlg.h"
#include "sip.h"
#include "../Pblmember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDetailDlg dialog


C3GSMSDetailDlg::C3GSMSDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GSMSDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GSMSDetailDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sReverse = "";
}


void C3GSMSDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDetailDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GSMSDetailDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDetailDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_DELETESELITEM, OnDeleteItem)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnComboSelect)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDetailDlg message handlers
/*
void C3GSMSDetailDlg::OnTimer(UINT nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == 1)
		{
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}
*/
void  C3GSMSDetailDlg::SetSMSDetail(CString Sender, CString Content)
{
	m_senderEdit.SetWindowText(Sender);
	m_contentEdit.SetWindowText(Content);
}

void C3GSMSDetailDlg::OnComboSelect(WPARAM w, LPARAM l)
{
	int i = m_cmbType.GetCurSel();
	if(i == 0)
	{
		if(m_nSMSType != SMS_NEW)
		{
			initDataBase(SMS_NEW, -1, TRUE);
		}
	}
	else
	{
		if(m_nSMSType != MMS_NEW)
		{
			initDataBase(MMS_NEW, -1, TRUE);
		}
	}
}	
void  C3GSMSDetailDlg::initDataBase(SMSDETAILTYPE type,  int smsid, BOOL reDraw )
{
	char *title[] = {".\\adv\\mjpg\\k1\\common\\3g\\短信阅读-新建标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信阅读-阅读信息标题.bmp"};
	char *senderTitle[] = {".\\adv\\mjpg\\k1\\common\\3g\\短信阅读-收件人.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信阅读-发件人.bmp"};
	char *newbuttonTile[][2] = {
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读发送1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读发送2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读存贮1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读存贮2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读复制1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读复制2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读粘贴1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读粘贴2.bmp"
	};
	char *readbuttonTile[][2] = {
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读回复1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读回复2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读回电2.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读回电2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读转发1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读转发2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读删除1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-阅读删除2.bmp"
	};
	m_nSMSType = type;			
	m_nSMSId = smsid;

	m_MMSShow.ShowWindow(SW_HIDE);
	m_contentEdit.ShowWindow(SW_HIDE);
	
	if(m_nSMSType == SMS_READ || m_nSMSType == MMS_READ)
	{
		m_MJPGList.SetUnitBitmap(1, readbuttonTile[0][0],readbuttonTile[0][1], reDraw);
		m_MJPGList.SetUnitBitmap(2, readbuttonTile[1][0],readbuttonTile[1][1], reDraw);
		m_MJPGList.SetUnitBitmap(3, readbuttonTile[2][0],readbuttonTile[2][1], reDraw);
		m_MJPGList.SetUnitBitmap(4, readbuttonTile[3][0],readbuttonTile[3][1], reDraw);
		
		m_MJPGList.SetUnitBitmap(10, title[1], "", reDraw);
		m_MJPGList.SetUnitBitmap(11, senderTitle[1], "", reDraw);
		m_senderEdit.SetReadOnly(TRUE);
		m_contentEdit.SetReadOnly(TRUE);

		//todo::
		if(m_nSMSType == SMS_READ)
		{
			m_cmbType.SetCurSel(0);
			m_cmbType.EnableWindow(FALSE);

			m_pMessageData = Data::Message::GetDataById(m_nSMSId);
			CString s; 
			s = Util::StringOp::ToCString(m_pMessageData->unicodeData);
			m_contentEdit.SetWindowText(s);
			s = Util::StringOp::ToCString(m_pMessageData->remote.address);
			m_senderEdit.SetWindowText(s);
			s = Util::StringOp::ToCString(m_pMessageData->timestamp.ToStringFormat_());
			m_MJPGList.SetUnitText(12, s, reDraw);
			m_contentEdit.ShowWindow(SW_SHOW);
		
			if(m_pMessageData->state != Data::Message::sReaded)
			{
				m_pMessageData->state = Data::Message::sReaded;
				m_pMessageData->Update();
			}			
		}
		else if(m_nSMSType == MMS_READ)
		{
			m_cmbType.SetCurSel(1);
			m_cmbType.EnableWindow(FALSE);
			
			m_pMMSData = Data::MMSData::GetDataById(m_nSMSId);
			
			CString s = m_pMMSData->SavePath.c_str();
			
			CString filename = "";
			CString path = "";
			extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
			FindFileEx(s, path, filename);
			if(path != "")
			{
				m_MMSShow.FindFileSmil(path.GetBuffer(256));
				m_MMSShow.SetTitle(MMS::MMSWarp::GetMMSWarp()->ToUnicode((m_pMMSData->Subject)));
				m_MMSShow.ShowWindow(SW_SHOW);
			}
			s = Util::StringOp::ToCString(m_pMMSData->SenderAddress);
			m_senderEdit.SetWindowText(s);
			CTime tm = CTime(1970, 1, 1, 0, 0, 0);
			tm += CTimeSpan(0, 0, 0, m_pMMSData->DateAndTime);
			s = Util::StringOp::FromTimestampFormat(tm).c_str();
			m_MJPGList.SetUnitText(12, s, reDraw);
		}
	}
	else
	{
		m_MJPGList.SetUnitBitmap(1, newbuttonTile[0][0],newbuttonTile[0][1], reDraw);
		m_MJPGList.SetUnitBitmap(2, newbuttonTile[1][0],newbuttonTile[1][1], reDraw);
		m_MJPGList.SetUnitBitmap(3, newbuttonTile[2][0],newbuttonTile[2][1], reDraw);
		m_MJPGList.SetUnitBitmap(4, newbuttonTile[3][0],newbuttonTile[3][1], reDraw);
		

		m_MJPGList.SetUnitBitmap(10, title[0], "", reDraw);
		m_MJPGList.SetUnitBitmap(11, senderTitle[0], "", reDraw);
		m_senderEdit.SetReadOnly(FALSE);
		m_contentEdit.SetReadOnly(FALSE);
		m_senderEdit.SetWindowText(L"");
		m_contentEdit.SetWindowText(L"");
		
		if(m_nSMSId == -1)
		{
			m_MJPGList.SetUnitText(12, "", reDraw);
			if(m_nSMSType == SMS_NEW)
			{
				m_cmbType.SetCurSel(0);
				m_cmbType.EnableWindow(TRUE);
				
				m_contentEdit.ShowWindow(SW_SHOW);
			}
			else if(m_nSMSType == MMS_NEW)    //写彩信
			{
				m_cmbType.SetCurSel(1);
				m_cmbType.EnableWindow(TRUE);
				m_contentEdit.ShowWindow(SW_HIDE);
				m_MMSShow.SetMmsEdit();
				m_MMSShow.ShowWindow(SW_SHOW);
			}
		}
		else
		{
			if(m_nSMSType == SMS_NEW)
			{
				m_cmbType.SetCurSel(0);
				m_cmbType.EnableWindow(TRUE);
				
				m_pMessageData = Data::Message::GetDataById(m_nSMSId);
				CString s; 
				s = Util::StringOp::ToCString(m_pMessageData->unicodeData);
				m_contentEdit.SetWindowText(s);
				s = Util::StringOp::ToCString(m_pMessageData->remote.address);
				m_senderEdit.SetWindowText(s);
				m_MJPGList.SetUnitText(12, "", reDraw);
				m_contentEdit.ShowWindow(SW_SHOW);
			}
			else if(m_nSMSType == MMS_NEW)    //写彩信
			{
				m_cmbType.SetCurSel(1);
				m_cmbType.EnableWindow(TRUE);
				m_MMSShow.SetMmsEdit();
				m_MMSShow.ShowWindow(SW_SHOW);
			}
		}
	}
}

BOOL C3GSMSDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cmbType.Create(WS_CHILD|WS_VISIBLE, CRect(175, 3, 175+120, 160), this, 0xFFFF);
	m_cmbType.AddString(L"短信");
	m_cmbType.AddString(L"彩信");
	m_cmbType.SetCurSel(0);

	m_senderEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(15, 90, 15+160, 90+205), this, 0xFFFF);
	m_contentEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(180, 60, 180+528, 60+340), this, 0xFFFF);
	m_contentEdit.SetLimitText(140);
	
	m_MMSShow.Create(WS_CHILD, CRect(180, 60, 180+528, 60+340), this, 0, 0);
	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_阅读短信.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));
//	SetTimer(1, 1000, NULL);

	m_dlgCardSelect.Create(IDD_DIALOG_TELEPHONE,this);
	m_dlgCardSelect.MoveWindow(10,120,790, 340);
	m_dlgCardSelect.CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GSMSDetailDlg:: OnDeleteItem(WPARAM w, LPARAM l)
{
	if(m_nSMSType == MMS_READ)
	{
		m_pMMSData->Remove();
		CString s =m_pMMSData->SavePath.c_str();
		
		CString filename = "";
		CString path = "";
		extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
		FindFileEx(s, path, filename);
		extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
		DeleteDirectory(path);
	}
	else if(m_nSMSType == SMS_READ)
	{
		m_pMessageData->Remove();
	}
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSListDlg->DeleteRefreshList();
	ShowWindow(SW_HIDE);
}

int g_iSendSMSId = -1;
void C3GSMSDetailDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:			//发送
		{
			if(m_nSMSType == SMS_NEW)
			{
				CString s;
				m_contentEdit.GetWindowText(s);
				CString s_;
				m_senderEdit.GetWindowText(s_);
				
				//add by qi
				std::vector<Util::ATCommandWarp::SIM_FORMAT> vcontact; 
				std::vector<Util::ATCommandWarp::SIM_FORMAT>::iterator it; 
				m_dlgCardSelect.GetSendContact(vcontact);
				for (it = vcontact.begin(); it < vcontact.end();it++)
				{
					std::string name  = (*it).name ;
					std::string phone = (*it).telnum ;
				}

				if(s != "" && s_ != "")
				{
					SipShowIM(SIPF_OFF);

					m_pMessageData = boost::shared_ptr<Data::Message> (new Data::Message);
					m_pMessageData->unicodeData = Util::StringOp::FromCString(s);
					CString s_;
					m_senderEdit.GetWindowText(s_);
					m_pMessageData->remote.address = Util::StringOp::FromCString(s_);
					SYSTEMTIME tm;
					GetLocalTime(&tm);
					m_pMessageData->timestamp.year = tm.wYear%100;
					m_pMessageData->timestamp.month = tm.wMonth;
					m_pMessageData->timestamp.day = tm.wDay;
					m_pMessageData->timestamp.hour = tm.wHour;
					m_pMessageData->timestamp.minite = tm.wMinute;
					m_pMessageData->timestamp.second = tm.wSecond;
					m_pMessageData->group = Data::Message::gSend;
					m_pMessageData->Insert();
					g_iSendSMSId = m_pMessageData->id();

					pMainDlg->m_pSMSWarp->Send(m_pMessageData->remote.address, m_pMessageData->unicodeData);
					
					/*
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"正在发送短信...", FALSE);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
					*/

					ShowWindow(SW_HIDE);
				}
			}
		
			else if(m_nSMSType == MMS_NEW)		//发送
			{
				CString s_;
				m_senderEdit.GetWindowText(s_);
			//	std::wstring sImage, sText, sAudio, sVedio;
			//	m_MMSShow.GetAllFileInfo(sImage, sText, sAudio, sVedio);
			//	void EncodeSmil(vector<MMS_PAR> vpars, MMS_LAYOUT layout, std::string path);

				MMS::MMSWarp::MMS_LAYOUT layout;
				std::vector<MMS::MMSWarp::MMS_PAR> pars;
				std::wstring sTitle;
				BOOL ret = m_MMSShow.GetParInfo(pars, layout, sTitle);//获得par里的内容

				
				if(s_ != "" && ret)
				{
					SipShowIM(SIPF_OFF);
					
					SYSTEMTIME tm;
					GetLocalTime(&tm);
					CTime ct = CTime(tm);
					CTimeSpan sp = ct - CTime(1970, 1, 1, 8, 0, 0);
				//	CTimeSpan sp = 
					char datetime[24];
					char sDir[64];
					sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
					sprintf(sDir, "/flashdrv/3g/mms/%s/", datetime);
					CString sDir_ = sDir;
					CreateDirectory((LPTSTR)(LPCTSTR)sDir_, NULL);
					
					MMS::MMSWarp::GetMMSWarp()->EncodeSmil(pars, layout, sDir);

					/*       
					CString sI = sImage.c_str();
					CString sT = sText.c_str();
					CString sA = sAudio.c_str();
					CString sV = sVedio.c_str();
					extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
					CString path; 
					CString filename;
					if(sT != "")
					{
						extern std::string const ToUTF8(std::wstring const& unicode);
						std::string stext = ToUTF8((LPTSTR)(LPCTSTR)sT);
						char fn[64];
						strcpy(fn, sDir);
						strcat(fn, "t001.txt");
						FILE *fl;
						fl = fopen(fn, "w+b");
						if(fl)
						{
							fwrite(stext.c_str(), sizeof(char), stext.length(), fl);
							fclose(fl);
						}
					}
					
					if(sI != "")
					{
						filename = "";
						FindFileEx(sI, path, filename);
						if(filename != "")
						{
							CString sNew = sDir_;
							sNew += filename;
							CopyFile(sI, sNew, FALSE);
						}
					}
					if(sA != "")
					{
						filename = "";
						FindFileEx(sA, path, filename);
						if(filename != "")
						{
							CString sNew = sDir_;
							sNew += filename;
							CopyFile(sA, sNew, FALSE);
						}
					}
					if(sV != "")
					{
						filename = "";
						FindFileEx(sV, path, filename);
						if(filename != "")
						{
							CString sNew = sDir_;
							sNew += filename;
							CopyFile(sV, sNew, FALSE);
						}
					}
					*/
					
				//	CopyFile()
					m_pMMSData = boost::shared_ptr<Data::MMSData> (new Data::MMSData);
					m_pMMSData->type = Data::MMSData::tpReadySend;
					m_pMMSData->TransactionId = datetime;
					m_pMMSData->RecipientAddress = Util::StringOp::FromCString(s_); //To-value = Encoded-string-value
					m_pMMSData->SenderAddress = pMainDlg->m_sMobileNumber; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
					m_pMMSData->Subject = MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle);   //存进去应该是 GB2312
					//m_pMMSData->Subject = Util::StringOp::ToUTF8(MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle));
					m_pMMSData->DateAndTime = sp.GetSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
					m_pMMSData->SavePath = sDir;
					m_pMMSData->isRead = TRUE;
					m_pMMSData->Insert();

					ShowWindow(SW_HIDE);	
				}
			}
			else								//回复
			{
				CString sTelcode = "";
				CString sContent = "";
				C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
				if(m_nSMSType == MMS_READ)
				{
					sTelcode = m_pMMSData->SenderAddress.c_str();
				}
				else
				{
					sTelcode = m_pMessageData->remote.address.c_str();
				}	
				pWnd_->initDataBase(SMS_NEW, -1, TRUE);
				pWnd_->SetSMSDetail(sTelcode, sContent);
			//	pWnd_->ShowWindow(SW_SHOW);
			}
		}
		break;
	case 2:
		if(m_nSMSType == SMS_NEW)			//保存
		{
			CString s;
			m_contentEdit.GetWindowText(s);
			if(s != "")
			{
				SipShowIM(SIPF_OFF);

				m_pMessageData = boost::shared_ptr<Data::Message> (new Data::Message);
				m_pMessageData->unicodeData = Util::StringOp::FromCString(s);
				CString s_;
				m_senderEdit.GetWindowText(s_);
				m_pMessageData->remote.address = Util::StringOp::FromCString(s_);
				SYSTEMTIME tm;
				GetLocalTime(&tm);
				m_pMessageData->timestamp.year = tm.wYear%100;
				m_pMessageData->timestamp.month = tm.wMonth;
				m_pMessageData->timestamp.day = tm.wDay;
				m_pMessageData->timestamp.hour = tm.wHour;
				m_pMessageData->timestamp.minite = tm.wMinute;
				m_pMessageData->timestamp.second = tm.wSecond;
				m_pMessageData->group = Data::Message::gUnSend;
				m_pMessageData->Insert();
				ShowWindow(SW_HIDE);				
			}
		}
		else if(m_nSMSType == MMS_NEW)		//保存
		{
			
		}
		else								//回电
		{
			CString sTelcode = "";
			CString sContent = "";
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if(m_nSMSType == MMS_READ)
			{
				sTelcode = m_pMMSData->SenderAddress.c_str();
			}
			else
			{
				sTelcode = m_pMessageData->remote.address.c_str();
			}	
			if(sTelcode != "")
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(sTelcode), -1);
			}
		}
		break;
	case 3:				//复制
		if(m_nSMSType == SMS_NEW)
		{
			int n, n1;
			m_contentEdit.GetSel(n, n1);
			if(n != n1)
			{
				CString s;
				m_contentEdit.GetWindowText(s);
				m_sReverse = s.Mid(n, n1-n);
			}
		}
		else if(m_nSMSType == MMS_NEW)
		{
	
		}
		else		//转发
		{
			CString sTelcode = "";
			CString sContent = "";
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if(m_nSMSType == MMS_READ)
			{
				
			}
			else
			{
				sContent = m_pMessageData->unicodeData.c_str();
				pWnd_->initDataBase(SMS_NEW, -1, TRUE);
				pWnd_->SetSMSDetail(sTelcode, sContent);
			//	pWnd_->ShowWindow(SW_SHOW);
			}	
		}

		break;
	case 4:				//粘贴
		if(m_nSMSType == SMS_NEW)
		{
			if(m_sReverse != "")
				m_contentEdit.ReplaceSel(m_sReverse);
		}
		else if(m_nSMSType == MMS_NEW)
		{

		}
		else			//删除
		{		
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			std::string strTemp = ".\\adv\\mjpg\\k1\\common\\确定删除吗.bmp";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}

		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 11: //add by qi 2009_08_05
		m_dlgCardSelect.ShowWindow(true);
		m_dlgCardSelect.ShowCard();
		break;

	case 20:
		if(m_contentEdit.IsWindowVisible())
		{
			m_contentEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
			m_contentEdit.Invalidate();
		}
		break;

	case 21:
		if(m_contentEdit.IsWindowVisible())
		{
			m_contentEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
			m_contentEdit.Invalidate();
		}
		break;
	case 100:
		::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
		SipShowIM(SIPF_OFF);
		ShowWindow(SW_HIDE);
	default:
		break;
	}
}