// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSMSListDlg.h"
#include "3GSMSDlg.h"
#include "3GSMSDetailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg dialog

#define PAGE_COUNT  8
C3GSMSListDlg::C3GSMSListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GSMSListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GSMSListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nListCountTotal = 0;
	m_nPageSize = PAGE_COUNT;
	m_strHomeRecordeTelCode = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->speCode3_.c_str();
	m_nBoxType = RECV_TYPE;
	m_nSMSType = SMS_TYPE;
	m_bChangeMain = FALSE;
}


void C3GSMSListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GSMSListDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSListDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_LISTCTRL_CLICKSUBITEM, OnListCltrlClick)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg message handlers
/*
void C3GSMSListDlg::OnTimer(UINT nIDEvent)
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

void C3GSMSListDlg::initType(SMSBOX_TYPE box_type, SMSMMS_TYPE sms_type, BOOL reDraw)
{
	m_nBoxType = box_type;
	m_nSMSType = sms_type;

	char *boxTitle[] = {".\\adv\\mjpg\\k1\\common\\3g\\短信-收件箱标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-发件箱标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-草稿箱标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-留言箱标题.bmp"};
	char *listTitle[] = {".\\adv\\mjpg\\k1\\common\\3g\\短信-发件人标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-收件人标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-收件人标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-留言人标题.bmp"};
    char *pageTitle[][2] = {".\\adv\\mjpg\\k1\\common\\3g\\短信-短信标题1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-短信标题2.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-彩信标题1.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-彩信标题2.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-留言标题1.bmp", ""}; 
	if(m_nBoxType == HOME_RECORD_TYPE)
	{
		m_MJPGList.SetUnitIsShow(2, FALSE, TRUE);
		m_MJPGList.SetUnitIsDownStatus(1, FALSE);
		m_MJPGList.SetUnitIsDownStatus(1, TRUE);
		m_MJPGList.SetUnitBitmap(1, pageTitle[(int)2][1], pageTitle[(int)2][0], reDraw);	
	}
	else
	{
		m_MJPGList.SetUnitIsShow(2, TRUE, TRUE);
		m_MJPGList.SetUnitIsDownStatus(1, FALSE);
		m_MJPGList.SetUnitIsDownStatus(2, FALSE);
		if(m_nSMSType == SMS_TYPE)
		{
			m_MJPGList.SetUnitIsDownStatus(1, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(2, TRUE);
		}
		m_MJPGList.SetUnitBitmap(1, pageTitle[(int)0][1], pageTitle[(int)0][0], reDraw);
		m_MJPGList.SetUnitBitmap(2, pageTitle[(int)1][1], pageTitle[(int)1][0], reDraw);
		
	}

	m_MJPGList.SetUnitBitmap(10, boxTitle[(int)box_type], boxTitle[(int)box_type], reDraw);
	m_MJPGList.SetUnitBitmap(11, listTitle[(int)box_type],  listTitle[(int)box_type], reDraw);
	initDateBase();
}

void C3GSMSListDlg::DoDeleteSMSAll()
{
	if(m_nBoxType == HOME_RECORD_TYPE)   //家庭留言   短信中心特殊号
	{
		m_sListFilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gReMoteSMS); //"smscAddress = '" + m_strHomeRecordeTelCode + "'";
	}
	else
	{
		if(m_nSMSType == MMS_TYPE)
			m_sListFilter = "[type] = " + Util::StringOp::FromInt(m_nBoxType);
		else
			m_sListFilter = "[group] = " + Util::StringOp::FromInt(m_nBoxType);
	}
	
	if(m_nSMSType == MMS_TYPE)			//彩信
	{
		if (Data::MMSData::GetDataCount("") > 0)
		{
			Data::MMSData::Remove(m_sListFilter);
		}	
	}
	else			//短信及家庭留言
	{
		if (Data::Message::GetDataCount("") > 0)
		{
			 Data::Message::Remove(m_sListFilter);
		}
	}
}

BOOL C3GSMSListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(12, 120, 788, 398), this, IDC_LIST_SOUND_LIST, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("1"), LVCFMT_LEFT, 182);
	m_lsList.InsertColumn(1, _T("2"), LVCFMT_LEFT, 190);
	m_lsList.InsertColumn(2, _T("3"), LVCFMT_LEFT, 130);
	m_lsList.InsertColumn(3, _T("4"), LVCFMT_LEFT, 48);
	m_lsList.InsertColumn(4, _T("5"), LVCFMT_LEFT, 48);
	m_lsList.InsertColumn(5, _T("6"), LVCFMT_LEFT, 48);
	m_lsList.InsertColumn(6, _T("7"), LVCFMT_LEFT, 48);
	m_lsList.InsertColumn(7, _T("8"), LVCFMT_LEFT, 48);
	
	
	m_pImageList = new CImageList();
	CBitmap bm;
	m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 2, 2);   
	bm.LoadBitmap(IDB_BITMAP23);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP26);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	
	
	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_短信箱.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));
//	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GSMSListDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:			//短信
		if(m_nSMSType != SMS_TYPE)
		{
			initType(m_nBoxType, SMS_TYPE, TRUE);
			Invalidate();
		}
		break;
	case 2:			//彩信
		if(m_nSMSType != MMS_TYPE)
		{
			initType(m_nBoxType, MMS_TYPE, TRUE);
			Invalidate();
		}	
		break;
	case 3:			//全删
		{
			m_nSelectItem = -1;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			std::string strTemp = ".\\adv\\mjpg\\k1\\common\\确定删除吗.bmp";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		DoDeleteSMSAll();	
		break;
	case 4:
	
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
	case 100:
		{
			if(m_bChangeMain)
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
			else
				ShowWindow(SW_HIDE);
		}
	default:
		break;
	}
}


void C3GSMSListDlg::initDateBase()
{
	if(m_nBoxType == HOME_RECORD_TYPE)   //家庭留言   短信中心特殊号
	{
		m_sListFilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gReMoteSMS); //"smscAddress = '" + m_strHomeRecordeTelCode + "'";
	}
	else
	{
		if(m_nSMSType == MMS_TYPE)
			m_sListFilter = "[type] = " + Util::StringOp::FromInt(m_nBoxType);
		else
			m_sListFilter = "[group] = " + Util::StringOp::FromInt(m_nBoxType);
	}

	m_lsList.DeleteAllItems();
		
	if(m_nSMSType == MMS_TYPE)			//彩信
	{
		if (Data::MMSData::GetDataCount("") > 0)
		{
			m_vMMSDataCurrentResult = Data::MMSData::GetFromDatabase(m_sListFilter, Data::dDown, Data::MMSData::GetCurrentId() + 1, m_nPageSize);
			m_nListCountTotal = Data::MMSData::GetDataCount(m_sListFilter);
		}
		else
		{
			m_nListCountTotal = 0;
			m_vMMSDataCurrentResult.clear();
		}
	}
	else			//短信及家庭留言
	{
		if (Data::Message::GetDataCount("") > 0)
		{
			m_vMessageCurrentResult = Data::Message::GetFromDatabase(m_sListFilter, Data::dDown, Data::Message::GetCurrentId() + 1, m_nPageSize);
			m_nListCountTotal = Data::Message::GetDataCount(m_sListFilter);
		}
		else
		{
			m_vMessageCurrentResult.clear();
			m_nListCountTotal = 0;
		}
	}
	m_lsList.m_scollbar_.SetRangle(0, m_nListCountTotal, m_nPageSize);
	ShowArrayInList();	
	m_lsList.SetScrollRagle(TRUE);
	
}

void C3GSMSListDlg::DeleteRefreshList()
{
	
	int nPos = m_lsList.m_scollbar_.GetPos();
	int nMax = m_lsList.m_scollbar_.GetMax();
	if(nPos+m_nPageSize < nMax)
		nPos++;
	if(m_nSMSType == MMS_TYPE)			//彩信
	{
		m_vMMSDataCurrentResult = Data::MMSData::GetFromDatabaseByTypeOffsetLength(m_sListFilter, nPos, m_nPageSize);
		m_nListCountTotal = Data::MMSData::GetDataCount(m_sListFilter);
	}
	else
	{
		m_vMessageCurrentResult = Data::Message::GetFromDatabaseByTypeOffsetLength(m_sListFilter, nPos, m_nPageSize);
		m_nListCountTotal = Data::Message::GetDataCount(m_sListFilter);
	}
	m_lsList.m_scollbar_.SetRangle(0, m_nListCountTotal, m_nPageSize);
	ShowArrayInList();
//	m_lsList.SetScrollRagle(TRUE);
}

void C3GSMSListDlg::ScrollItemsInList(int step, int nPos)
{
	if(m_nSMSType == MMS_TYPE)			//彩信
	{
		if (m_vMMSDataCurrentResult.empty())
		{
			return;
		}
		m_vMMSDataCurrentResult.clear();
		m_vMMSDataCurrentResult = Data::MMSData::GetFromDatabaseByTypeOffsetLength(m_sListFilter, nPos, m_nPageSize);
			
	}
	else			//短信及家庭留言
	{
		if(m_vMessageCurrentResult.empty())
			return;
		m_vMessageCurrentResult.clear();
		m_vMessageCurrentResult = Data::Message::GetFromDatabaseByTypeOffsetLength(m_sListFilter, nPos, m_nPageSize);	
	}

	if (step == SB_LINEUP)
	{
		if(m_nSMSType == MMS_TYPE)
		{
			if (m_uiSelectIndex < m_vMMSDataCurrentResult.size() - 1)
			{
				++m_uiSelectIndex;
			}
		}
		else
		{
			if (m_uiSelectIndex < m_vMessageCurrentResult.size() - 1)
			{
				++m_uiSelectIndex;
			}
		}
	}
	else if (step == SB_LINEDOWN)
	{
		if (m_uiSelectIndex > 0)
		{
			--m_uiSelectIndex;
		}		
	}
	else if (step == SB_PAGEUP)
	{
		m_uiSelectIndex = 0;
	}
	else if(step == SB_PAGEDOWN)
	{
		m_uiSelectIndex = 0;
	}
	else if(step == SB_THUMBPOSITION)
	{
		m_uiSelectIndex = 0;
	}
	ShowArrayInList();		
}

void C3GSMSListDlg::ShowArrayInList()
{
	m_lsList.DeleteAllItems();
	WCHAR *title[] = {L"回电", L"转发", L"回复", L"删除", L"详情"};
	CString temp;
	if(m_nSMSType == MMS_TYPE)			//彩信
	{
		for (int i = 0; i < m_vMMSDataCurrentResult.size(); ++i)
		{	
			if(m_nBoxType == SEND_TYPE || m_nBoxType == DRAFT_TYPE)
			{
				temp = Util::StringOp::ToCString(m_vMMSDataCurrentResult[i]->RecipientAddress);
			}
			else
				temp = Util::StringOp::ToCString(m_vMMSDataCurrentResult[i]->SenderAddress);
			if (!m_vMMSDataCurrentResult[i]->isRead)
			{
				m_lsList.InsertItem(i, temp, 0);
			}
			else
			{
				//  已读不标记
				m_lsList.InsertItem(i, temp, 1);
			}
			temp = Util::StringOp::ToCString(m_vMMSDataCurrentResult[i]->Subject);
			m_lsList.SetItemText(i, 1, temp);
			CTime tm = CTime(1970, 1, 1, 0, 0, 0);
			tm += CTimeSpan(0, 0, 0, m_vMMSDataCurrentResult[i]->DateAndTime);
			temp = Util::StringOp::FromTimestamp(tm).c_str();

			m_lsList.SetItemText(i, 2, temp);

			for(int j = 3; j < 8; j++)
			{
				m_lsList.SetItemText(i, j, title[j-3]);
			}
		}
		if (!m_vMMSDataCurrentResult.empty())
		{
			POSITION pos = m_lsList.GetFirstSelectedItemPosition();   
			while (pos != NULL)
			{   
				int iSel = m_lsList.GetNextSelectedItem(pos);   
				m_lsList.SetItemState(iSel, 0, LVIS_SELECTED);   
			}
			m_lsList.SetItemState(m_uiSelectIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	else			//短信及家庭留言
	{
		for (int i = 0; i < m_vMessageCurrentResult.size(); ++i)
		{	
			temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->remote.address);
			if (m_vMessageCurrentResult[i]->state == Data::Message::sNoRead)
			{
				m_lsList.InsertItem(i, temp, 0);
			}
			else
			{
				//  已读不标记
				m_lsList.InsertItem(i, temp, 1);
			}
			temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->unicodeData);
			m_lsList.SetItemText(i, 1, temp);
			temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->timestamp.ToString_());
			m_lsList.SetItemText(i, 2, temp);
			
			for(int j = 3; j < 8; j++)
			{
				if(j == 5 && ((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE)))  //发送
				{
					m_lsList.SetItemText(i, j, L"发送");
				}
				else
					m_lsList.SetItemText(i, j, title[j-3]);
			}
		}
		if (!m_vMessageCurrentResult.empty())
		{
			POSITION pos = m_lsList.GetFirstSelectedItemPosition();   
			while (pos != NULL)
			{   
				int iSel = m_lsList.GetNextSelectedItem(pos);   
				m_lsList.SetItemState(iSel, 0, LVIS_SELECTED);   
			}
			m_lsList.SetItemState(m_uiSelectIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

LRESULT C3GSMSListDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
		{
			if(m_nSelectItem == -1)
			{
				DoDeleteSMSAll();
			}
			else
			{
				if(m_nSMSType == MMS_TYPE)
				{
					m_vMMSDataCurrentResult[m_nSelectItem]->Remove();
					CString s = m_vMMSDataCurrentResult[m_nSelectItem]->SavePath.c_str();
					
					CString filename = "";
					CString path = "";
					extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
					FindFileEx(s, path, filename);
					extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
					DeleteDirectory(path);
				}
				else
				{
				//	nId = m_vMessageCurrentResult[0]->id();
					m_vMessageCurrentResult[m_nSelectItem]->Remove();
				}
			}
			
	//		initDateBase();
	//		ScrollItemsInList(SB_PAGEDOWN, m_lsList.m_scollbar_.GetPos());
			DeleteRefreshList();
			::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
		}
		break;
	case WM_SCROLL_EDO:
		ScrollItemsInList(wParam, lParam);
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void C3GSMSListDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	if(l == 7)    //详情
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE))
			{
				if(m_nSMSType == MMS_TYPE)
				{
					pWnd_->initDataBase(MMS_READ, m_vMMSDataCurrentResult[w]->id(), FALSE);
					pWnd_->ShowWindow(SW_SHOW);
				}
				else
				{
					pWnd_->initDataBase(SMS_READ, m_vMessageCurrentResult[w]->id(), FALSE);
					pWnd_->ShowWindow(SW_SHOW);
				}

			}
			else
			{
				if(m_nSMSType == MMS_TYPE)
				{
					pWnd_->initDataBase(MMS_READ, m_vMMSDataCurrentResult[w]->id(), FALSE);
					m_vMMSDataCurrentResult[w]->isRead = TRUE;
					m_vMMSDataCurrentResult[w]->Update();
					pWnd_->ShowWindow(SW_SHOW);
					ShowArrayInList();
				}
				else
				{
					pWnd_->initDataBase(SMS_READ, m_vMessageCurrentResult[w]->id(), FALSE);
					m_vMessageCurrentResult[w]->state = Data::Message::sReaded;
					m_vMessageCurrentResult[w]->Update();
					pWnd_->ShowWindow(SW_SHOW);
					ShowArrayInList();
				}
			}
		}
	}
	else if(l == 6)	//删除
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			m_nSelectItem = w;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			std::string strTemp = ".\\adv\\mjpg\\k1\\common\\确定删除吗.bmp";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
	}
	else if(l == 5)	//回复  发送
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			CString sTelcode = "";
			CString sContent = "";
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE))  //发送
			{
				if(m_nSMSType == MMS_TYPE)
				{
					//todo:彩信
				}
				else
				{
					sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
					sContent = m_vMessageCurrentResult[w]->unicodeData.c_str();

					pWnd_->initDataBase(SMS_NEW, -1, FALSE);
					pWnd_->SetSMSDetail(sTelcode, sContent);
					pWnd_->ShowWindow(SW_SHOW);
				}		
			}
			else
			{
				if(m_nSMSType == MMS_TYPE)
				{
					sTelcode = m_vMMSDataCurrentResult[w]->SenderAddress.c_str();
				}
				else
				{
					sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
				}	
				
				pWnd_->initDataBase(SMS_NEW, -1, FALSE);
				pWnd_->SetSMSDetail(sTelcode, sContent);
				pWnd_->ShowWindow(SW_SHOW);
			}
		}
	}
	else if(l == 4)	//转发
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			CString sTelcode = "";
			CString sContent = "";
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if(m_nSMSType == MMS_TYPE)
			{
			//	sTelcode = m_vMMSDataCurrentResult[w]->unicodeData.c_str();
			}
			else
			{
				sContent = m_vMessageCurrentResult[w]->unicodeData.c_str();
				pWnd_->initDataBase(SMS_NEW, -1, FALSE);
				pWnd_->SetSMSDetail(sTelcode, sContent);
				pWnd_->ShowWindow(SW_SHOW);
			}	
		}
	}
	else if(l == 3)	//回电
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			CString sTelcode = "";
			if(m_nSMSType == MMS_TYPE)
			{
				sTelcode = m_vMMSDataCurrentResult[w]->SenderAddress.c_str();
			}
			else
			{
				sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
			}
			if(sTelcode != "")
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(sTelcode), -1);
			}
		}
	}
}
