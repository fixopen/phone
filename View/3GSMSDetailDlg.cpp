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
	char *title[] = {".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅÔÄ¶Á-ÐÂ½¨±êÌâ.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅÔÄ¶Á-ÔÄ¶ÁÐÅÏ¢±êÌâ.bmp"};
	char *senderTitle[] = {".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅÔÄ¶Á-ÊÕ¼þÈË.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅÔÄ¶Á-·¢¼þÈË.bmp"};
	char *newbuttonTile[][2] = {
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á·¢ËÍ1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á·¢ËÍ2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á´æÖü1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á´æÖü2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á¸´ÖÆ1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á¸´ÖÆ2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶ÁÕ³Ìù1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶ÁÕ³Ìù2.bmp"
	};
	char *readbuttonTile[][2] = {
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á»Ø¸´1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á»Ø¸´2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á»Øµç2.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á»Øµç2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á×ª·¢1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶Á×ª·¢2.bmp",
		".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶ÁÉ¾³ý1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\¶ÌÐÅ-ÔÄ¶ÁÉ¾³ý2.bmp"
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
			else if(m_nSMSType == MMS_NEW)    //Ð´²ÊÐÅ
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
			else if(m_nSMSType == MMS_NEW)    //Ð´²ÊÐÅ
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
	m_cmbType.AddString(L"¶ÌÐÅ");
	m_cmbType.AddString(L"²ÊÐÅ");
	m_cmbType.SetCurSel(0);

	m_senderEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(15, 90, 15+160, 90+205), this, 0xFFFF);
	m_contentEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(180, 60, 180+528, 60+340), this, 0xFFFF);
	m_contentEdit.SetLimitText(140);
	
	m_MMSShow.Create(WS_CHILD, CRect(180, 60, 180+528, 60+340), this, 0, 0);
	
	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\ÖÐÎÄ\\Ð´¶ÌÐÅ.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
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
//	pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSListDlg->DeleteRefreshList();
	ShowWindow(SW_HIDE);
}

int g_iSendSMSId = -1;
void C3GSMSDetailDlg::OnClickMJPG(WPARAM w, LPARAM l)
{

}