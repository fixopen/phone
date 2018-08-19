// ContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "ContactDlg.h"
#include "../Data/ContactGroup.h"
#include "../Data/SkinStyle.h"

#include "SIMImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//add by qi 2009_09_21
CString backup = ".\\adv\\mjpg\\k5\\common\\通讯录\\信息条.png" ;//信息条的底图
CString hook = ".\\adv\\mjpg\\k5\\common\\勾选.png";//打勾图片
CString empty(L"");

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

CContactDlg::CContactDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactDlg::IDD, pParent)
	, ContactTotal(10000)
	, ContactGroupTotal(8)
	, PageSize(6)
	,m_iCurrentPage(0)
	,m_iTotalPages(0)
	,m_bSelectAll(false)
	,m_iUpCurrentPage(1)
	,m_sSimName("SIM卡")
	,m_iBindCount(3)
	,m_iSimID(1)
	,m_defGroupID(0)
	,m_iFriendID(3)
	,m_iClassMate(4)
{

	m_sAddtion = "";
	m_bSearch = false ;
	m_iUpAllCount = 0 ;
	
	//{{AFX_DATA_INIT(CContactDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//	m_pSimImportDlg = NULL;
}


void CContactDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactDlg, CDialog)
	//{{AFX_MSG_MAP(CContactDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_LIST, OnClickListList)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEW, OnButtonContactNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEWTYPE, OnButtonContactNewtype)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_EDITTYPE, OnButtonContactEdittype)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_SEARCH, OnButtonContactSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_CLOSE, OnButtonContactClose)
	ON_MESSAGE(WM_SOFTKEY, OnSoftKeyMessage)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE,OnClickMJPG)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CContactDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_LIST_CONTACT_TYPE)
		OnClickListType(NULL, &ret);
	else if(w == IDC_LIST_CONTACT_LIST)
		OnClickListList(NULL, &ret);
}

/////////////////////////////////////////////////////////////////////////////
// CContactDlg message handlers

void CContactDlg::OnClickMJPG_Toggle(WPARAM w, LPARAM l)
{

}

void CContactDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	CString content(L"");
	CString icon ;
	CString hideIcon1 = L".\\adv\\mjpg\\k5\\common\\隐藏_fore.bmp";
	CString hideIcon2 = L".\\adv\\mjpg\\k5\\common\\隐藏_back.bmp";
	std::string s ;
	switch (w)
	{
	case 0://页签1
	case 1://页签2
	case 2://页签3
	case 3://页签4
	case 4:	//页签5
	case 5:	//页签6
		SetUpBtn(w);
		Clear();
		ClearCurrentPage();
		SetContactFilter(w);
		FromContactDataBase();
		ShowGroupInfo();	
		//m_sAddtion = "";
		break;

	case 6://向左翻页
		PageUpSwitch(left_page);
		break;
	case 7://向右翻页
		PageUpSwitch(right_page);
		break;

	case 20://删除确认
		ShowDeleteDlg();
		//DeleteItems();
		break;
	case 21://新建	
		OnButtonContactNew();
		break;
	case 22://编辑类别
		OnButtonContactEdittype();
		break;

	case 23://新类别
		OnButtonContactNewtype();
		break;

	case 24://sim卡备份
		BackupSim();
		break;

	case 25://查找
		if (!m_bSearch)
		{	
			m_MJPGList.SetUnitBitmap(25,hideIcon1,hideIcon2,true);
			m_MJPGLetter_key.ShowWindow(SW_SHOW);
			m_bSearch = true ;
		}
		else
		{	
			m_MJPGList.SetUnitBitmap(25,L"",L"",true);
			m_MJPGLetter_key.ShowWindow(SW_HIDE);
			m_MJPGData_key.ShowWindow(SW_HIDE);
			m_bSearch = false ;
			m_sAddtion = "";
			m_MJPGData_key.SetUnitText(800,empty,true);
			m_MJPGLetter_key.SetUnitText(900,empty,true);
		}
		break;

	case 30:// 当前页的第一行
		ClickedOneItem(w,0);
		break;
	case 31://第一行类型
	case 32://第一行姓名
	case 33://第一行电话1
	case 34://第一行电话2
	case 35://第一行公司
		OnBtnEditContact(0);//编辑行
		break;

	case 40:// 当前页的第二行
		ClickedOneItem(w,1);
		break;
	case 41://第2行类型
	case 42://第2行姓名
	case 43://第2行电话1
	case 44://第2行电话2
	case 45://第2行公司
		OnBtnEditContact(1);//编辑行
		break;

	case 50:// 当前页的第三行
		ClickedOneItem(w,2);
		break;
	case 51://第3行类型
	case 52://第3行姓名
	case 53://第3行电话1
	case 54://第3行电话2
	case 55://第3行公司
		OnBtnEditContact(2);//编辑行
		break;

	case 60:// 当前页的第四行
		ClickedOneItem(w,3);
		break;
	case 61://第4行类型
	case 62://第4行姓名
	case 63://第4行电话1
	case 64://第4行电话2
	case 65://第4行公司
		OnBtnEditContact(3);//编辑行
		break;

	case 70:// 当前页的第五行
		ClickedOneItem(w,4);
		break;
	case 71://第5行类型
	case 72://第5行姓名
	case 73://第5行电话1
	case 74://第5行电话2
	case 75://第5行公司
		OnBtnEditContact(4);//编辑行
		break;

	case 80:// 当前页的第六行
		ClickedOneItem(w,5);
		break;
	case 81://第一行类型
	case 82://第一行姓名
	case 83://第一行电话1
	case 84://第一行电话2
	case 85://第一行公司
		OnBtnEditContact(5);//编辑行
		break;
	case 90:// 全选
		SelectAll();
		break;

	case 110://上翻一页
		PageSwitch(up_page);
		break;
		
	case 111://下翻一页
		PageSwitch(down_page);
		break;

	case 801://切换到字母
		m_MJPGData_key.ShowWindow(SW_HIDE);
		m_MJPGLetter_key.ShowWindow(SW_SHOW);
		break;
	case 802://删除
		content = m_MJPGData_key.GetUnitText(800);
		content.Delete(content.GetLength()-1);
		m_MJPGData_key.SetUnitText(800,content,true);
		Add(empty);
		break;
	case 803://清空
		m_MJPGData_key.SetUnitText(800,empty,true);
		m_sAddtion = "";
		m_MJPGList.SetUnitText(100,L"0/0",TRUE);//设置当前的页数
		Clear();
		ClearCurrentPage();
		break;
	case 804://?号
		 Add(L"?");
		break;
	case 810://9-0
		m_MJPGData_key.SetUnitFont(800,font_30);
		m_MJPGData_key.SetUnitColor(800,font_black,true);
		Add(L"1");
		break;
	case 811:
		Add(L"2");
		break;
	case 812:
		Add(L"3");
		break;
	case 813:
		Add(L"4");
		break;	
	case 814:
		Add(L"5");
		break;	
	case 815:
		Add(L"6");
		break;
	case 816:
		Add(L"7");
		break;	
	case 817:
		Add(L"8");
		break;
	case 818:
		Add(L"9");
		break;
	case 819:
		Add(L"0");
		break;

	case 901: //切换到数字
		m_MJPGLetter_key.ShowWindow(SW_HIDE);
		m_MJPGData_key.ShowWindow(SW_SHOW);
		break;
	case 902://删除
		content = m_MJPGLetter_key.GetUnitText(900);
		content.Delete(content.GetLength()-1);
		m_MJPGLetter_key.SetUnitText(900,content,true);
		Add(empty);
		break;
	case 903://清空
		m_MJPGLetter_key.SetUnitText(900,empty,true);
		m_sAddtion = "";
		m_MJPGList.SetUnitText(100,L"0/0",TRUE);//设置当前的页数
		Clear();
		ClearCurrentPage();
		break;
	case 904://问号
		Add(L"?");
		break;

	case 910://310-335,a-z
		Add(L"a");
		break;	
	case 911:
		Add(L"b");
		break;
	case 912:
		Add(L"c");
		break;
	case 913:
		Add(L"d");
		break;
	case 914:
		Add(L"e");
		break;
	case 915:
		Add(L"f");
		break;
	case 916:
		Add(L"g");
		break;
	case 917:
		Add(L"h");
		break;
	case 918:
		Add(L"i");
		break;
	case 919:
		Add(L"j");
		break;
	case 920:
		Add(L"k");
		break;
	case 921:
		Add(L"l");
		break;
	case 922:
		Add(L"m");
		break;
	case 923:
		Add(L"n");
		break;
	case 924:
		Add(L"o");
		break;
	case 925:
		Add(L"p");
		break;
	case 926:
		Add(L"q");
		break;
	case 927:
		Add(L"r");
		break;
	case 928:
		Add(L"s");
		break;
	case 929:
		Add(L"t");
		break;
	case 930:
		Add(L"u");
		break;
	case 931:
		Add(L"v");
		break;
	case 932:
		Add(L"w");
		break;
	case 933:
		Add(L"x");
		break;
	case 934:
		Add(L"y");
		break;
	case 935:
		Add(L"z");
		break;

	case 1000://返回
		ShowWindow(SW_HIDE);		
		main->PopbackIcon();			
		break;
		
	case 1001://确定
		OnBtnOK();
		break;

	default:
		break;
	}
}

void CContactDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 'U')
	{
		m_lsList.m_scollbar_.DoPageUp();
	}
	else if(nChar == 'D')
	{
		m_lsList.m_scollbar_.DoPageDown();
	}
}

BOOL CContactDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
/*
	m_lsType.m_nLineHeight = 30;
	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(1, 46, 112, 348), this, IDC_LIST_CONTACT_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 110);

	m_pImageList = new CImageList();
	m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 12, 2);   
    
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP5);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP1);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP9);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP10);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP11);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP12);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP13);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP14);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP15);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP16);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP17);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP18);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

//	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);
	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(115, 45, 115+572, 45+303), this, IDC_LIST_CONTACT_LIST, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 120+22+34);
	m_lsList.InsertColumn(1, _T("Company"), LVCFMT_LEFT, 180+60);
//	m_lsList.InsertColumn(2, _T("Department"), LVCFMT_LEFT, 120);
	m_lsList.InsertColumn(2, _T("Duty"), LVCFMT_LEFT, 150-22+26);

	m_pImageList = new CImageList();
	m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 3, 2);   
	bm.LoadBitmap(IDB_BITMAP8);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP7);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP6);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);
*/
	CRect rectKey[29];
	UINT16 uiKey[29];
	int offsetX = 0;
	int offsetY = 0;
	for (int j = 0; j < 26; ++j)
	{	
		if (j > 12)
		{
			offsetX = (j - 13) * 50;
			offsetY = 37;
		}
		else
		{
			offsetX = j * 50;
		}
		rectKey[j] = CRect(150 + offsetX, offsetY, 200 + offsetX, offsetY + 37);
		uiKey[j] = 'a' + j;
	};
	rectKey[26] = CRect(0, 37, 50, 74);
	uiKey[26] = 0x7F;
	rectKey[27] = CRect(50, 37, 100, 74);
	uiKey[27] = 0x00;
	rectKey[28] = CRect(100, 37, 150, 74);
	uiKey[28] = '\?';

	
//	m_sticPinYin.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(70, 348, 148, 380), this, IDC_STATIC_CONTACT_PINYIN);
//	m_sticPinYin.SetColor(RGB(0, 0, 0), RGB(140, 181, 239));
//	m_sticPinYin.SetLeftMargin(10);
//	m_sticPinYin.SetFontSize(26);

//  change by qi 2009_09_17
//	m_skSoftKey.Create(IDB_VOIPPYSEARCH, CPoint(0,0), 29, rectKey, uiKey, CRect(0, 346, 800, 420),this,WS_CHILD|WS_VISIBLE, IDC_SOFTKEY_CONTACT_PYSEARCH);

/*
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDBTN);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_CONTACT_NEW);
	m_btnNewContact.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
	str = strTemp.c_str();
	m_btnNewType.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_CONTACT_NEWTYPE);
	m_btnNewType.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
	str = strTemp.c_str();
	m_btnEditType.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66, 471, 85), this, IDC_BUTTON_CONTACT_EDITTYPE);
	m_btnEditType.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86, 471, 105), this, IDC_BUTTON_CONTACT_SEARCH);
	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
		
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 138, 471, 157), this, IDC_BUTTON_CONTACT_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 157), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);

	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);

	ts[0].txtRect = CRect(0, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).length());
	
	ts[1].txtRect = CRect(112, 0, 170, 20);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).length());
	
	ts[2].txtRect = CRect(170, 0, 266, 20);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).length());

	ts[3].txtRect = CRect(266, 0, 336, 20);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).length());
	
	ts[4].txtRect = CRect(336, 0, 396, 20);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).length());
	
	ts[5].txtRect = CRect(417, 0, 470, 20);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).length());
	
	m_sticBackground.SetTextStruct(ts, 6);
*/

//	change by qi 2009_09_17
// 	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
// 	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\通讯录.xml");
// 	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));

//  add by qi 2009_09_17

	m_pSelectNumberDlg.Create(CSelectNumberDlg::IDD);
	m_pSelectNumberDlg.ShowWindow_(SW_HIDE);
	
	m_MJPGData_key.Create(L"", WS_CHILD, CRect(0, 308, 800, 400), this);
	m_MJPGData_key.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\data_key.xml");
	m_MJPGData_key.SetMJPGRect(CRect(0, 308, 800, 400));
	
	m_MJPGLetter_key.Create(L"", WS_CHILD, CRect(0, 308, 800, 400), this);
	m_MJPGLetter_key.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\letter_key.xml");
	m_MJPGLetter_key.SetMJPGRect(CRect(0, 308, 800, 400));

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\通讯录.xml");
 	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

	m_pContactSearchDlg = new CContactSearchDlg(this);
	m_pContactSearchDlg->Create(CContactSearchDlg::IDD);

//	ShowTypeItems();
	SetPageFont();
	ShowWindow_();


//	m_sListFilter = "";
//	m_sListSearchFilter = "";	
//	ShowItemsInList();

//	add by qi 2009_09_17
	MoveWindow(0,57,800,423);
//	m_lsType.ShowWindow(SW_HIDE);
//	m_lsList.ShowWindow(SW_HIDE);
	
	m_simImportDlg.Create(CSIMImportDlg::IDD);
	m_simImportDlg.ShowWindow(SW_HIDE);

//	m_pSimImportDlg.Create(CSIMImportDlg::IDD);
//	m_pSimImportDlg.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
 	ShowItemsInList();

	*pResult = 0;
}

void CContactDlg::ShowTypeItems(void)
{	

	std::vector<boost::shared_ptr<Data::ContactGroup> > result;
	if (Data::ContactGroup::GetDataCount("") > 0)
	{
		result = Data::ContactGroup::GetFromDatabase("");
		for (int i = 0; i < result.size(); ++i)
		{	
			
			std::string type = result[i]->name();//获得组名
		}
	}

}

void CContactDlg::ShowItemsInList(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_lsList.DeleteAllItems();

		m_sListFilter = "";

		std::string filter;
		std::string aliasFilter;
		//std::string alias = GetPYIndex(); change by qi 
		std::string alias ;
		if (alias != "")
		{
			aliasFilter = "alias LIKE '";
			aliasFilter += alias;
			aliasFilter += "%'";
		}

		int type = m_lsType.GetNextSelectedItem(pos);
		if (type == 0)
		{
			filter = aliasFilter;
		}
		else if (type == m_lsType.GetItemCount() - 1)
		{
			if (m_sListSearchFilter == "")
				return;

			filter = m_sListSearchFilter;
			if (aliasFilter != "")
			{
				filter += " AND ";
				filter += aliasFilter;
			}

		}
		else
		{
			m_sListFilter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
			filter = m_sListFilter;
			if (aliasFilter != "")
			{
				filter += " AND ";
				filter += aliasFilter;
			}
		}	

		if (Data::Contact::GetDataCount("") > 0)
		{
			m_vCurrentResult = Data::Contact::GetFromDatabase(filter, Data::dDown, Data::Contact::GetCurrentId() + 1, PageSize);
			int count = Data::Contact::GetDataCount(filter);
			m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
			ShowArrayInList(m_vCurrentResult);		
			m_lsList.SetScrollRagle(TRUE);
		}
	}
}

void CContactDlg::ScrollItemsInList(int step, int nPos)
{
	
	if (m_vCurrentResult.empty())
	{
		return;
	}
	
	std::string filter = m_sListFilter;
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int type = m_lsType.GetNextSelectedItem(pos);
		if (type == m_lsType.GetItemCount() - 1)
		{
			filter = m_sListSearchFilter;
		}
	}

//	std::string alias = GetPYIndex(); change by qi 
	std::string alias ;
	if (alias != "")
	{
		if (filter != "")
		{
			filter += " AND ";
		}
		filter = "alias LIKE '";
		filter += alias;
		filter += "%'";
	}

	m_vCurrentResult.clear();
	m_vCurrentResult = Data::Contact::GetFromDatabaseByTypeOffsetLength(filter, nPos, PageSize);

// 	if (step == SB_LINEUP)
// 	{
// 		currentID = m_vCurrentResult[0]->id();
// 		temp = Data::Contact::GetFromDatabase(filter, Data::dUp, currentID, 1);
// 		if (!temp.empty())
// 		{
// 			m_vCurrentResult.insert(m_vCurrentResult.begin(),temp[0]);
// 			if (m_vCurrentResult.size() > PageSize)
// 			{
// 				m_vCurrentResult.pop_back();
// 			}
// 		}
// 	}

// 	else if (step == SB_LINEDOWN)
// 	{
// 		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
// 		temp = Data::Contact::GetFromDatabase(filter, Data::dDown, currentID, 1);
// 		if (!temp.empty())
// 		{
// 			m_vCurrentResult.push_back(temp[0]);
// 			if (m_vCurrentResult.size() > PageSize)
// 			{
// 				m_vCurrentResult.erase(m_vCurrentResult.begin());
// 			}
// 		}
// 	}
// 	else if (step == SB_PAGEUP)
// 	{
// 		currentID = m_vCurrentResult[0]->id();
// 		temp = Data::Contact::GetFromDatabase(filter, Data::dUp, currentID, PageSize);
// 		if (temp.size() < PageSize)
// 		{
// 			temp.insert(temp.end(), m_vCurrentResult.begin(), m_vCurrentResult.begin() + PageSize - temp.size());
// 		}
// 		m_vCurrentResult = temp;
// 	}
// 	else if(step == SB_PAGEDOWN)
// 	{
// 		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
// 		temp = Data::Contact::GetFromDatabase(filter, Data::dDown, currentID, PageSize);
// 		if (temp.size() < PageSize)
// 		{
// 			m_vCurrentResult.insert(m_vCurrentResult.end(), temp.begin(), temp.end());
// 			if (m_vCurrentResult.size() > PageSize)
// 			{
// 				m_vCurrentResult.erase(m_vCurrentResult.begin(), m_vCurrentResult.begin() + m_vCurrentResult.size() - PageSize);
// 			}
// 		}
// 		else
// 		{
// 			m_vCurrentResult = temp;
// 		}
// 	}
// 	else if(step == SB_THUMBPOSITION)
// 	{
// 		if(m_pContactFilter.size() <= 0)
// 		{
// 			m_pContactFilter = Data::Contact::GetFromDatabase(filter);
// 		}
// 		
// 		m_vCurrentResult.clear();
// 		if (m_pContactFilter.size() > nPos)
// 		{
// 			int nOff = m_pContactFilter.size() - nPos;
// 			if(nOff >= PageSize)
// 			{
// 				m_vCurrentResult.insert(m_vCurrentResult.begin(), m_pContactFilter.begin() + nPos, m_pContactFilter.begin() + nPos + PageSize);
// 			}
// 			else
// 				m_vCurrentResult.insert(m_vCurrentResult.begin(), m_pContactFilter.begin() + m_pContactFilter.size() - PageSize, m_pContactFilter.end());
// 		}
// 		else
// 		{
// 			m_vCurrentResult.insert(m_vCurrentResult.begin(), m_pContactFilter.begin() + m_pContactFilter.size() - PageSize, m_pContactFilter.end());
// 		}
// 
// 	
// 	}
	ShowArrayInList(m_vCurrentResult);		
}

void CContactDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array)
{

}

void CContactDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//由于第一次点击名片列表经常不能显示名片详细信息，启动一个定时器，在定时器中处理.
	SetTimer(1, 50, NULL);
	*pResult = 0;
}

void CContactDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		POSITION pos = m_lsList.GetFirstSelectedItemPosition();
		Dprintf("List click!!!!\n");
		if (pos != NULL)
		{
			int index = m_lsList.GetNextSelectedItem (pos);
			Dprintf("List click %d!!!!\n", index);
			int id = m_lsList.GetItemData(index);
			std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
			if (!result.empty())
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
				::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CContactDlg::OnButtonContactNew() 
{
	// TODO: Add your control notification handler code here
	if (Data::Contact::GetDataCount("") < ContactTotal)
	{
		boost::shared_ptr<Data::Contact> contact(new Data::Contact);
		contact->id(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(true);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->MoveWindow(0,57,800,423);

	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
	}
}

void CContactDlg::OnButtonContactNewtype() 
{
	// TODO: Add your control notification handler code here
	if (Data::ContactGroup::GetDataCount("") < ContactGroupTotal)
	{
		boost::shared_ptr<Data::ContactGroup> contactgroup(new Data::ContactGroup);
		contactgroup->id(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(contactgroup);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow_(SW_SHOW);
	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
	}
}

void CContactDlg::OnButtonContactEdittype() 
{	
	if (m_iCurrentGroupID == m_iSimID)
	{
		return ;
	}

	int id = m_iCurrentGroupID ;
	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
	if (!result.empty())
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(result[0]);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow_(TRUE);
	}
	
}

void CContactDlg::OnButtonContactSearch() 
{
	// TODO: Add your control notification handler code here
	m_pContactSearchDlg->ShowWindow(TRUE);
}

void CContactDlg::OnButtonContactClose()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

LRESULT CContactDlg::OnSoftKeyMessage(WPARAM wParam, LPARAM lParam)
{
	unsigned short key = wParam; 
	CString py(key); 
	switch(char(key))
	{
	case 0x7F://del
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(py.Left(py.GetLength() - 1));
		break;
	case 0x00://clear
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
		break;
// 	case '\?'://?
// 		break';
	default:
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
		if (py.GetLength() >= 6)
		{
			return 0;
		}
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(py + key);
		break;
	}

	ShowItemsInList();
	return 0;
}

std::string CContactDlg::GetPYIndex(CString content)
{
	std::string alias;
	CString py = content;
//	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
	if (!py.IsEmpty())
	{
		alias = Util::StringOp::FromCString(py);
		for (int i = 0; i < alias.size(); ++i)
		{
			if (alias[i] == '\?')
			{
				alias[i] = '_';
			}
		}
	}
	return alias;
}

int CContactDlg::GetTypeListSelected(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsType.GetNextSelectedItem (pos);
		return index;
	}
	return -1;
}

void CContactDlg::SetTypeListSelected(int index)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CContactDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;
	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ShowItemsInList();
}

LRESULT CContactDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
		case WM_SCROLL_EDO:
			ScrollItemsInList(wParam, lParam);
			break;

		case WM_DELETESELITEM:		
			DeleteItems();		
			break;

		case WM_SIM_IMPORT:
			HandleSimImport(wParam);
			break;

		case WM_NUMBER_SELECT:
			HandleNumber(lParam);
			break;

		default:
			break;
	};
	
	return CDialog::WindowProc(message, wParam, lParam);
}

// add by qi 2009_09_17

void CContactDlg::SetUpBtn(int ID)
{
	m_MJPGList.SetUnitIsDownStatus(ID,TRUE);
	for (int i = 0 ; i < 6;i++)
	{	
		if (i != ID)
		{
			m_MJPGList.SetUnitIsDownStatus(i,FALSE);
		}
	}
	m_MJPGList.Invalidate(FALSE);	
}


void CContactDlg::ClearCurrentPage()
{
	int ibegin = 30 ;
	int items = 0 ;
	for (int i = 0 ; i < PageSize ;i++)
	{	
		//m_MJPGList.SetUnitBitmap(ibegin+items+6,empty,"",false);
		//m_MJPGList.SetUnitBitmap(ibegin+items,empty,"",false);

		m_MJPGList.SetUnitIsDownStatus(ibegin+items,false);		
		m_MJPGList.SetUnitIsShow(ibegin+items,false,false);

		//m_MJPGList.SetUnitText(ibegin+items+1,empty,false);//组类别
		m_MJPGList.SetUnitBitmap(ibegin+items+1,empty,L"",false);
		m_MJPGList.SetUnitIsDownStatus(ibegin+items+1,false);
		m_MJPGList.SetUnitIsShow(ibegin+items+1,false,false);

		m_MJPGList.SetUnitText(ibegin+items+2,empty,false);//姓名
		m_MJPGList.SetUnitIsDownStatus(ibegin+items+2,false);
		m_MJPGList.SetUnitIsShow(ibegin+items+2,false,false);

		m_MJPGList.SetUnitText(ibegin+items+3,empty,false);//移动电话
		m_MJPGList.SetUnitIsDownStatus(ibegin+items+3,false);
		m_MJPGList.SetUnitIsShow(ibegin+items+3,false,false);

		m_MJPGList.SetUnitText(ibegin+items+4,empty,false);//工作电话
		m_MJPGList.SetUnitIsDownStatus(ibegin+items+4,false);
		m_MJPGList.SetUnitIsShow(ibegin+items+4,false,false);

		m_MJPGList.SetUnitText(ibegin+items+5,empty,false);//公司名称
		m_MJPGList.SetUnitIsDownStatus(ibegin+items+5,false);
		m_MJPGList.SetUnitIsShow(ibegin+items+5,false,false);

		items = items+10;
	}
	
	m_MJPGList.SetUnitIsShow(90,false,false);
	m_MJPGList.Invalidate();
}

void CContactDlg::PageSwitch(Action action)
{	
		
	if (action == up_page)
	{
		m_iCurrentPage--;
		if (m_iCurrentPage < 1)
		{	
			m_iCurrentPage = 1;
			return ;
		}
	}
	
	if (action == down_page)
	{
 		m_iCurrentPage++;
		if (m_iCurrentPage > m_iTotalPages)
 		{
 			m_iCurrentPage--;
 			return ;
 		}

	}
	
	ClearCurrentPage();
	FromContactDataBase();
	ShowGroupInfo();		
}

void CContactDlg::ClickedOneItem(int unitID,int item)
{	

	if (m_vClick[(m_iCurrentPage-1)*PageSize+item] == 0)//被点击了
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,true);		
		m_vClick[(m_iCurrentPage-1)*PageSize+item] = 1;
	}
	else
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,false);
		m_vClick[(m_iCurrentPage-1)*PageSize+item] = 0;
	}

	m_MJPGList.SetUnitIsShow(unitID,true,true);

}

void CContactDlg::DeleteItems()
{	
	int npos = 0 ;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{	
		if ( 1 == m_vClick[i])
		{				
			if (m_iCurrentGroupID == m_iSimID)//如果是SIM组，则SIM卡里的相应项也删除
			{	
				std::string filter = "id = ";
				filter += Util::StringOp::FromInt(m_vContactID[i]) ; 
				
				std::vector< boost::shared_ptr<Data::Contact> > result ;
				result = Data::Contact::GetFromDatabase(filter);
				if (!result.empty())
				{	
					int id = atoi(CString(result[0]->memo().c_str()));
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pATCommandWarp1->DeleteSim(id);						
				}

			}

			Data::Contact::Remove("id = " + Util::StringOp::FromInt(m_vContactID[i]));
			
			npos++;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(npos);

		}
	}
	
	//删除容器里的数据
	std::vector<int>::iterator it;
	it = m_vClick.begin();	
	std::vector<int>::iterator itID;
	itID = m_vContactID.begin();
	for ( it ; it < m_vClick.end() ; it)
	{	
		if ( 1 == (*it))
		{
			m_vClick.erase(it);
			m_vContactID.erase(itID);
		}
		else
		{
			itID++ ;
			it++;
		}
	}	
	
	//容器的末尾值也去掉，保留为页的整数
	if ( m_vClick.size()%PageSize != 0)
	{
		it	 = m_vClick.end() - m_vClick.size()%PageSize ;
		itID = m_vContactID.end() - m_vContactID.size()%PageSize ;
		for (it ; it < m_vClick.end() ;it)
		{
			m_vClick.erase(it);
			m_vContactID.erase(itID);
		}
	}
	
	m_iTotalPages = Data::Contact::GetDataCount(m_sListFilter)/PageSize;//总页数
	if (Data::Contact::GetDataCount(m_sListFilter)%PageSize != 0)
	{
		m_iTotalPages +=1;
	}
	if (m_iCurrentPage > m_iTotalPages)
	{
		m_iCurrentPage = m_iTotalPages ;	
	}

	if ( 0 == m_iTotalPages)
	{
		m_iCurrentPage = 1;
	}
	
	ClearCurrentPage();
	FromContactDataBase();
	ShowGroupInfo();
	
}

void CContactDlg::SetPageFont()
{
	int items = 0 ;
	int ibegin = 30;
	for (int i = 0 ;i < PageSize;i++)
	{   
		for (int j = 1 ;j < 6;j++)
		{
			m_MJPGList.SetUnitFont(ibegin+items+j,font_18);
			m_MJPGList.SetUnitColor(ibegin+items+j,font_white,false);
		}		
		items +=10; 
	}
	
	for (i = 0; i < 6;i++)
	{
		m_MJPGList.SetUnitFont(i,font_24);
		m_MJPGList.SetUnitColor(i,font_black,false);
	}

	m_MJPGList.SetUnitFont(100,font_18);
	m_MJPGList.SetUnitColor(100,font_white,false);

	m_MJPGData_key.SetUnitFont(800,font_30);
	m_MJPGData_key.SetUnitColor(800,font_black,true);

	m_MJPGLetter_key.SetUnitFont(900,font_30);
	m_MJPGLetter_key.SetUnitColor(900,font_black,true);


}

void CContactDlg::SelectAll()
{	
	int unitID = 30 ;
	if (!m_bSelectAll)
	{	
		m_bSelectAll = true ;
		for (int i = 0 ; i < m_vCurrentResult.size() ; i++)
		{					   			
			m_MJPGList.SetUnitIsDownStatus(90,true);
			m_MJPGList.SetUnitIsDownStatus(unitID,true);		
								
			unitID +=10;
		}
	}
	else
	{	
		m_bSelectAll = false ;
		for (int i = 0 ; i < m_vCurrentResult.size() ; i++)
		{											
			m_MJPGList.SetUnitIsDownStatus(90,false);
			m_MJPGList.SetUnitIsDownStatus(unitID,false);		
					
			unitID +=10;
		}
	}
	
	//所有项全部全选
	for (int i = 0 ; i < m_vClick.size();i++)
	{
		if (m_bSelectAll)
		{
			m_vClick[i] = 1;		
		}
		else
		{
			m_vClick[i] = 0;
		}
	}

	m_MJPGList.Invalidate();

}

void CContactDlg::InsertInfo(std::string name , std::string phone)
{
	boost::shared_ptr<Data::Contact> pContact;
	pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
	pContact->name(name);
	pContact->alias(GetAlias(pContact->name()));
	pContact->mobilesTelephone(Data::TelephoneNumber(phone));	
	
	pContact->worksTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
	pContact->mobilesTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
	pContact->worksTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
	pContact->email(Util::StringOp::FromCString(L""));
	pContact->homesite(Util::StringOp::FromCString(L""));
	pContact->company(Util::StringOp::FromCString(L""));
	pContact->companyAddress(Util::StringOp::FromCString(L""));
	pContact->companyPostCode(Util::StringOp::FromCString(L""));
	pContact->department(Util::StringOp::FromCString(L""));
	pContact->duty(Util::StringOp::FromCString(L""));
	pContact->memo(Util::StringOp::FromCString(L""));
	pContact->type(Data::ctNormal);
	pContact->groupId(0);
	pContact->Insert();	

	Clear();//清数据
	ClearCurrentPage();//清页面内容

}

void CContactDlg::Clear()
{
	if (m_vClick.size() >0)// 该项是否被点击
	{
		m_vClick.clear() ;
	}
	
	if (m_vContactID.size() > 0)
	{
		m_vContactID.clear() ;	
	}

	if (m_vCurrentResult.size() > 0)
	{
		m_vCurrentResult.clear();
	}
	m_bSelectAll = false ;

	m_iCurrentPage = 1 ;
	m_iTotalPages  = 0 ;

//	m_MJPGList.SetUnitBitmap(90,L"","",true);
	m_MJPGList.SetUnitIsDownStatus(90,false);

}
void CContactDlg::ClearUp()
{
	m_iUpCurrentPage = 1;
} 
void CContactDlg::ClearUpGroup()
{
	//清下界面,所有按钮为不可点击
	for (int i = 0; i < 6;i++)
	{
		m_MJPGList.SetUnitText(i,L"",false);
		m_MJPGList.SetUnitIsDisable(i,TRUE);
		m_MJPGList.SetUnitBitmap(i,L"",L"",false);
		m_MJPGList.SetUnitIsShow(i,false);
	}
	m_MJPGList.Invalidate();
} 

void CContactDlg::Add(CString ch)
{
	CString content ;
	int index ;
	if (m_MJPGData_key.IsWindowVisible())
	{	
		index = 800 ;
		content = m_MJPGData_key.GetUnitText(800);
		content = content + ch ;
		m_MJPGData_key.SetUnitText(800,content,true);

	}	
	else if (m_MJPGLetter_key.IsWindowVisible())
	{   
		index = 900 ;
		content = m_MJPGLetter_key.GetUnitText(900);
		content = content + ch ;
		m_MJPGLetter_key.SetUnitText(900,content,true);

	}
	

	if (content.IsEmpty())//没有内容直接返回
	{	
		CString page ;
		page.Format(L"%d/%d",0,0);
		m_MJPGList.SetUnitText(100,page,TRUE);//设置当前的页数

		Clear();
		ClearCurrentPage();
		return ;	
	}

	//查询
	std::string tempFilter;
	std::string filter = GetPYIndex(content);
	if (filter != "")
	{	
		if (800 == index)//电话号码查找
		{
			tempFilter = "(mobilesTelephone Like '";
			tempFilter += filter;
			tempFilter += "%'";
			tempFilter += " OR worksTelephone Like '";
			tempFilter += filter;
			tempFilter += "%')";
			m_sAddtion = tempFilter ;
		}
		
		if (900 == index)//按字母查找
		{
			tempFilter = "alias LIKE '";
			tempFilter += filter;
			tempFilter += "%'";
			m_sAddtion = tempFilter;
		}

	}
	
	m_sListFilter = m_sAddtion;
	Clear();
	ClearCurrentPage();
	FromContactDataBase();
	ShowGroupInfo();
	ShowUpGroupInfo();
	SetUpBtn(0);

}

void CContactDlg::HandleSimImport(WPARAM wp)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector< Util::ATCommandWarp::SIM_FORMAT > vsim ;
	std::vector< boost::shared_ptr<Data::Contact> > result;
	
	std::vector< int> vID;//在SIM卡中的位置
	int pos = 0;
	vID.resize(300);
	for (int j = 0 ;j < 300;j++)
	{
		vID[j] = 0;		
	}
	
	//SIM卡的容量
	int capacity = main->m_pATCommandWarp1->GetSimCapacity();
	if (  0 == capacity)//没有SIM返回
	{	
		main->m_pWarningNoFlashDlg->SetTitle(L"SIM卡不存在!");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	} 

	std::string filter = "[groupId] = " + Util::StringOp::FromInt(m_iSimID);//sim卡
	std::vector< boost::shared_ptr<Data::Contact> > simresult;
	simresult = Data::Contact::GetFromDatabase(filter);
	if (!simresult.empty())//获得SIM中的序号
	{
		for (int i = 0 ; i < simresult.size() ;i++)
		{
			CString memo = simresult[i]->memo().c_str();
			int id = atoi(memo);
			vID[id -1] = id ;
		}
	}
	else //SIM卡里没有数据
	{
		vID[0] = 1 ; 
	}

	for (int i = 0 ; i < m_vClick.size() ; i++)//获得姓名 和 电话
	{
		if ( 1 == m_vClick[i])
		{	
			std::string filter = "id =" + Util::StringOp::FromInt(m_vContactID[i]);
			result = Data::Contact::GetFromDatabase(filter);
			if ( !result.empty())
			{
				Util::ATCommandWarp::SIM_FORMAT sim ;
				std::string tel[4];
				bool bEmpty = false  ;
				
				sim.name	= result[0]->name();//名片里一定有名字
				sim.id		= 1;
				
				tel[0]		= result[0]->mobilesTelephone().number();
				tel[1]		= result[0]->mobilesTelephone2().number();
				tel[2]		= result[0]->worksTelephone().number();
				tel[3]		= result[0]->worksTelephone2().number();
						
				for (int i = 0 ; i < 4 ;i++)
				{
					if (!tel[i].empty())
					{	
						if ( wp == LocaltoSIM )
						{
							for (pos;pos < vID.size() ;pos++)
							{
								if ( 0 == vID[pos])//有空的
								{
									vID[pos] = pos+1 ;
									break;	
								}
							}
							sim.id = Util::StringOp::FromInt(pos+1);
												
							if (pos >= capacity)
							{	
								main->m_pWarningNoFlashDlg->SetTitle(L"超过SIM卡容量，不能导入");
								main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
								return ;
							}
							pos++;

						}
	
						sim.telnum = tel[i];
						vsim.push_back(sim);
						bEmpty = true ;
					}	

				}

				if (!bEmpty)//电话是空的
				{
					if ( wp == LocaltoSIM )
					{
						for (pos;pos < vID.size() ;pos++)
						{
							if ( 0 == vID[pos])
							{
								vID[pos] = pos+1 ;
								break;	
							}
						}
						sim.id = Util::StringOp::FromInt(pos+1);

						if (pos >= capacity)
						{	
							main->m_pWarningNoFlashDlg->SetTitle(L"超过SIM卡容量，不能导入");
							main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
							return ;
						}
						pos++;

					}		
					vsim.push_back(sim);

				}
				
			}
		}
	}
	
	if ( wp == LocaltoSIM )//
	{	
		boost::shared_ptr<Data::Contact> pContact;//插入SIM卡名片
		pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);

		m_simImportDlg.SetProcessMax(vsim.size());
		for (int i = 0 ; i < vsim.size();i++)
		{	
			//插入到SIM卡中
			std::vector< Util::ATCommandWarp::SIM_FORMAT > tempSim ;
			tempSim.resize(1);
			tempSim[0] = vsim[i];
			main->m_pATCommandWarp1->Save2Sim(tempSim);//数据放到SIM里
			
			//插入到联系人中
			pContact->name(vsim[i].name);
			pContact->mobilesTelephone(Data::TelephoneNumber(vsim[i].telnum));
			pContact->memo(vsim[i].id);
			pContact->groupId(m_iSimID);
			pContact->Insert();

			m_simImportDlg.SetProcessPos(i+1);
		}	
		
	}

	if ( wp == SIMtoLocal )//把SIM卡里勾选的内容放到默认组里
	{	
		m_simImportDlg.SetProcessMax(vsim.size());
		for (int i = 0 ; i < vsim.size();i++)
		{	
			std::vector< Util::ATCommandWarp::SIM_FORMAT > tempSim ;
			tempSim.resize(1);
			tempSim[0] = vsim[i];
			main->SaveDataToContact(tempSim);	
			
			m_simImportDlg.SetProcessPos(i+1);
		}

	}


}

std::string CContactDlg::GetAlias(std::string name)
{
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex == NULL)
	{
		return "";
	}
	
	std::string alias;
	for (int i = 0; i < name.size(); ++i)
	{
		if ((unsigned char)name[i] > (unsigned char)127)
		{
			unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i+1] - 0xa1);
			
			if(index >= 0 && index < 6768)
			{
				alias += ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex[index];	
			}
			else
			{
			}
			++i;
		}
		else
		{
			if (name[i] >= 'A' && name[i] <= 'Z')
			{
				alias += char(name[i] + 32);
			}
			else
			{
				alias += name[i];
			}
		}
	}
	return alias;
}

void CContactDlg::ShowDeleteDlg()
{	
	bool enble = false ;
	int  count = 0 ;
	
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			enble = true ;
			count++;
		}	
	}
	
	if (enble)
	{	
		CString title = "确认删除已选的内容吗?";
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(title,0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessMax(count);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
}

void CContactDlg::OnBtnEditContact(int item)
{	
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(m_vCurrentResult[item]->id()));
	if (!result.empty())
	{	
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);		
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->MoveWindow(0,57,800,423);		

	}
}

void CContactDlg::ShowNewContact(int groupID)
{	
	int column  = 0 ;

	if ( 0 == groupID)//全部
	{
		m_iUpCurrentPage = 1 ;
		column = 0 ;
	}
	else if(m_iSimID == groupID)
	{
		m_iUpCurrentPage = 1;
		column = 1;
	}
	else
	{
		for (int i = 0 ; i < m_vGroupResult.size();i++)
		{
			if (groupID == m_vGroupResult[i]->id())
			{	
				break ;
			}
		}

		m_iUpCurrentPage = (m_iBindCount + i+1)/PageSize ;
		if ( (m_iBindCount + i+1) % PageSize != 0)
		{
			m_iUpCurrentPage += 1;	
		}

		column	= (m_iBindCount + i)%PageSize ;
	}

	ClearUpGroup();
	ShowUpGroupInfo();
	
	Clear();
	SetUpBtn(column);
	SetContactFilter(column);
	ClearCurrentPage();
	FromContactDataBase();
	ShowGroupInfo();

}

void CContactDlg::FromGroupDataBase()
{
	// 查询条件
	std::string filter = "[name] <> 'SIM卡'";

	if (Data::ContactGroup::GetDataCount("") > 0)
	{	
		m_vGroupResult = Data::ContactGroup::GetFromDatabase(filter);
		m_iUpAllCount = Data::ContactGroup::GetDataCount(filter);
	}	
}

void CContactDlg::ShowRightBtn(bool bshow)
{	
	for (int i = 0 ; i < 5;i++)
	{
		m_MJPGList.SetUnitIsShow(i+20,bshow,false);
	}
}

void CContactDlg::ShowUpGroupInfo()
{	
	CString groupFore = L".\\adv\\mjpg\\k5\\common\\通讯录\\上面按钮_fore.png" ;//组显示的小图标
	CString groupBack = L".\\adv\\mjpg\\k5\\common\\通讯录\\上面按钮_back.png";//组显示的大图标
	CString name;

	for (int i = 0 ; i < GetUpGroupNum(); i++)
	{	
		if ( 1 == m_iUpCurrentPage)
		{
			if ( 0 == i )
			{
				m_MJPGList.SetUnitBitmap(0,groupFore,groupBack,false);
				m_MJPGList.SetUnitText(0,L"全部",false);
				m_MJPGList.SetUnitIsDisable(0,false);
				m_MJPGList.SetUnitIsShow(0,true);
				
			}
			else if ( 1 == i)
			{
				name = Util::StringOp::ToCString(m_sSimName);
				m_MJPGList.SetUnitBitmap(1,groupFore,groupBack,false);
				m_MJPGList.SetUnitText(1,name,false);
				m_MJPGList.SetUnitIsDisable(1,false);
				m_MJPGList.SetUnitIsShow(1,true);
			}
			else if ( 2 == i)
			{
				m_MJPGList.SetUnitBitmap(2,groupFore,groupBack,false);
				m_MJPGList.SetUnitText(2,L"默认组",false);
				m_MJPGList.SetUnitIsDisable(2,false);
				m_MJPGList.SetUnitIsShow(2,true);
			}
			else
			{
				name = Util::StringOp::ToCString(m_vGroupResult[i- m_iBindCount]->name());
				m_MJPGList.SetUnitBitmap(i,groupFore,groupBack,false);
				m_MJPGList.SetUnitText(i,name,false);
				m_MJPGList.SetUnitIsDisable(i,false);
				m_MJPGList.SetUnitIsShow(i,true);
			}
		}
		else
		{
			name = Util::StringOp::ToCString(m_vGroupResult[(m_iUpCurrentPage-2)*PageSize + PageSize-m_iBindCount+ i]->name());
			m_MJPGList.SetUnitBitmap(i,groupFore,groupBack,false);
			m_MJPGList.SetUnitText(i,name,false);
			m_MJPGList.SetUnitIsDisable(i,false);
			m_MJPGList.SetUnitIsShow(i,true);
		}
	}	

}

void CContactDlg::PageUpSwitch(Action action)
{	
	int allpage;
	int count = m_iBindCount ;

	count += m_iUpAllCount;
	allpage = count/6;//总页数
	if ( 0 != count%6)
	{
		allpage += 1;
	}

	if ( left_page == action )
	{
		m_iUpCurrentPage--;
		if (m_iUpCurrentPage < 1)
		{
			m_iUpCurrentPage = 1 ;
			return ;
		}
	
	}
	else if (right_page == action)
	{
		m_iUpCurrentPage++;
		if (m_iUpCurrentPage > allpage)
		{
			m_iUpCurrentPage--;
			return ;
		}			
	}

	ClearUpGroup();
	ShowUpGroupInfo();
	SetUpBtn(0);
	
	Clear();
	ClearCurrentPage();
	SetContactFilter(0);
	FromContactDataBase();
	ShowGroupInfo();

}

int CContactDlg::GetUpGroupNum()
{
	int icurrent ;
	
	if ( 1 == m_iUpCurrentPage )
	{	
		icurrent = m_iBindCount;
		icurrent += m_vGroupResult.size() - (m_iUpCurrentPage-1)* PageSize ;
	}

	if ( m_iUpCurrentPage > 1 )
	{	
		icurrent = m_vGroupResult.size() - (m_iUpCurrentPage-2)* PageSize - (PageSize - m_iBindCount) ;
	}

	if (icurrent > PageSize)
	{
		icurrent = PageSize ;
	}

	return  icurrent ;
}

void CContactDlg::SetContactFilter(int index)
{
	if ( 1 == m_iUpCurrentPage)
	{	
		if ( 0 == index)
		{
			m_sListFilter = "";	
			m_iCurrentGroupID = 0;
		}
		else if ( 1 == index)
		{
			m_sListFilter = "groupId = " + Util::StringOp::FromInt(m_iSimID);
			m_iCurrentGroupID = m_iSimID;
		}
		else if ( 2 == index)
		{
			m_sListFilter = "groupId = " + Util::StringOp::FromInt(m_defGroupID);
			m_iCurrentGroupID = 0;
		}
		else
		{
			m_sListFilter = "groupId = " 
				+ Util::StringOp::FromInt(m_vGroupResult[index - m_iBindCount]->id());
			m_iCurrentGroupID =m_vGroupResult[index - m_iBindCount]->id();

		}

	}
	
	if ( m_iUpCurrentPage > 1)
	{
		m_sListFilter = "groupId = " 
			+ Util::StringOp::FromInt(m_vGroupResult[PageSize - m_iBindCount 
			+(m_iUpCurrentPage -2)*PageSize + index]->id());
		
			m_iCurrentGroupID = m_vGroupResult[PageSize - m_iBindCount 
							+(m_iUpCurrentPage -2)*PageSize + index]->id();

	}

}
void CContactDlg::ShowGroupInfo()
{
	CString temp ;
	int items	= 0  ;
	int ibegin  = 30 ;  
	
	SetUnitStatus();
	SetUpPages();
	for (int i = 0; i < m_vCurrentResult.size(); ++i)
	{					
		//	temp = Util::StringOp::ToCString(array[i]->type());
		boost::shared_ptr<Data::Contact> result = 
			m_vCurrentResult[i];

		if ( Data::ctNormal == result->type())
		{
			temp =	L".\\adv\\mjpg\\k5\\common\\通讯录\\普通.png" ;
		}
		if ( Data::ctVip  == result->type())
		{
			temp =	L".\\adv\\mjpg\\k5\\common\\通讯录\\VIP.png" ;
		}
		if ( Data::ctBlacklist == result->type())
		{
			temp =	L".\\adv\\mjpg\\k5\\common\\通讯录\\黑名单.png" ;
		}
		
		//	m_MJPGList.SetUnitText(ibegin+items+1,temp,false);//组类别
		m_MJPGList.SetUnitBitmap(ibegin+items+1,temp,L"",false);//组类别
		
		temp = Util::StringOp::ToCString(result->name());
		m_MJPGList.SetUnitText(ibegin+items+2,temp,false);//姓名
		
		temp = Util::StringOp::ToCString(result->mobilesTelephone().number());
		m_MJPGList.SetUnitText(ibegin+items+3,temp,false);//移动电话
		
		temp = Util::StringOp::ToCString(result->worksTelephone().number());
		m_MJPGList.SetUnitText(ibegin+items+4,temp,false);//工作电话
		
		temp = Util::StringOp::ToCString(result->company());
		m_MJPGList.SetUnitText(ibegin+items+5,temp,false);//公司名称
		items = items+10;
		
	}

}
void CContactDlg::SetUnitStatus()
{
	int ibegin = 30 ;
	int item  = 0;	
	for(int i = 0; i < m_vCurrentResult.size(); ++i)
	{
		if ( 1 == m_vClick[(m_iCurrentPage-1)*PageSize + i])
		{
			m_MJPGList.SetUnitIsDownStatus(ibegin+item,true);
		}

		for (int j = 0 ; j < 6 ;j++)
		{
			m_MJPGList.SetUnitIsShow(ibegin+item+j,true,false);
		}

		item += 10 ;
	}

	if (m_vCurrentResult.size() > 0)
	{
		m_MJPGList.SetUnitIsShow(90,true,false);
	}
}

int CContactDlg::GetVectorPages()
{
	int pages ;
	pages = m_vClick.size()/PageSize ;
	if ( m_vClick.size()%PageSize != 0)
	{
		pages += 1;
	}
	return pages ;
}

void CContactDlg::FromContactDataBase()
{	
	if (m_vCurrentResult.size() > 0)
	{
		m_vCurrentResult.clear();
	}

	if (Data::Contact::GetDataCount("") > 0)
	{
		m_iTotalPages = Data::Contact::GetDataCount(m_sListFilter)/PageSize;//总页数
		if (Data::Contact::GetDataCount(m_sListFilter)%PageSize != 0)
		{
			m_iTotalPages +=1;
		}
		
		m_vCurrentResult = Data::Contact::GetFromDatabaseByTypeOffsetLength(m_sListFilter, (m_iCurrentPage-1)*PageSize+1, PageSize);
		
		int a = m_vCurrentResult.size() ;

		//是否往容器里添加数据
		if (m_iCurrentPage > GetVectorPages())
		{
			for (int i = 0 ; i < m_vCurrentResult.size() ;i++)
			{
				if(m_bSelectAll)
				{
					m_vClick.push_back(1);
				}
				else
				{
					m_vClick.push_back(0);
				}
				m_vContactID.push_back(m_vCurrentResult[i]->id());
			}	
		}
	}
}

void CContactDlg::SetUpPages()
{	
	CString page ;
	page.Format(L"%d/%d",m_iCurrentPage,m_iTotalPages);
	if (  0 == m_iTotalPages)
	{
		page.Format(L"%d/%d",0,m_iTotalPages);
	}
	m_MJPGList.SetUnitText(100,page,TRUE);//设置当前的页
}
void CContactDlg::OnBtnOK()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ( m_ntype == 1 || m_ntype == 2)
	{
		Contact2Sms();
		if (m_ntype > 0)
		{
			ShowWindow(SW_HIDE);			
			main->PopbackIcon();
		}
	}
	else if ( m_ntype == 3)
	{
		AddNumbertoContact();
		if (m_ntype > 0)
		{
			ShowWindow(SW_HIDE);			
			main->PopbackIcon();
			m_ntype = 0;
		}

	}

}

void CContactDlg::SetType(int type)
{	
	if (m_vNumber.size())
	{
		m_vNumber.clear();
	}
	
	if (m_vName.size())
	{
		m_vName.clear();
	}
	m_iSelectNum = 0;
	m_ntype = type ;

}

void CContactDlg::SetNumber(std::string num,int index)
{
	m_sNumber =  num	;
	m_index   =  index  ;
}

void CContactDlg::AddNumbertoContact()
{	
	bool enble = false ;
	int  count = 0 ;
	
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			enble = true ;
			count++;
			break;
		}	
	}
	if (enble)
	{
		std::vector<boost::shared_ptr<Data::Contact> > result;
		result= Data::Contact::GetFromDatabase("id = " +Util::StringOp
			::FromInt(m_vContactID[i]));
		if (!result.empty())
		{	
			if ( 1 == m_index )
			{	
				result[0]->mobilesTelephone(m_sNumber);
			}
			else if( 2 == m_index )
			{
				result[0]->mobilesTelephone2(m_sNumber);	
			}
			if ( 3 == m_index )
			{
				result[0]->worksTelephone(m_sNumber);	
			}
			else if (4 == m_index )
			{
				result[0]->worksTelephone2(m_sNumber);	
			}

			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
			::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);		
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->MoveWindow(0,57,800,423);
			
		}
	}

}
void CContactDlg::Contact2Sms()//把联系人插入到SIM
{	
	for (m_iSelectNum; m_iSelectNum < m_vClick.size(); m_iSelectNum++)
	{
		if ( 1 == m_vClick[m_iSelectNum])
		{	
			if (m_v4Number.size() > 0)
			{
				m_v4Number.clear();
			}

			std::string filter;
			filter  = "id =";
			filter += Util::StringOp::FromInt(m_vContactID[m_iSelectNum]);
		
			std::string tel[4];
			std::vector<boost::shared_ptr<Data::Contact> > result ;
			result = Data::Contact::GetFromDatabase(filter);
			
			if ( result.size())
			{   
				m_sName = Util::StringOp::ToCString(result[0]->name());

				tel[0]	= result[0]->mobilesTelephone().number();
				tel[1]	= result[0]->mobilesTelephone2().number();
				tel[2]	= result[0]->worksTelephone().number();
				tel[3]	= result[0]->worksTelephone2().number();
				
				for (int j = 0 ; j < 4 ; j++ )
				{
					if (!tel[j].empty())
					{
						m_v4Number.push_back(Util::StringOp::ToCString(tel[j]));
					}
				}

				if (m_v4Number.size() > 1)//勾选框
				{	
					m_pSelectNumberDlg.SetHWnd(GetSafeHwnd());
					m_pSelectNumberDlg.SetNumber(m_v4Number);
					m_pSelectNumberDlg.ShowWindow_(SW_SHOW);
					return;
				}
				else if ( 1 == m_v4Number.size() )
				{
					m_vNumber.push_back(m_v4Number[0]);
					if(m_sName.IsEmpty())
					{
						m_vName.push_back(m_v4Number[0]);
					}
					else
					{
						m_vName.push_back(m_sName);
					}
					HandleNumber(-1);
					return;	

				}				
			}
		}
	}

	if (m_iSelectNum == m_vClick.size())
	{
		HandleNumber(-1);
	}
		
}

void CContactDlg::HandleNumber(LPARAM lParam)
{
	if ((int)lParam > -1)
	{	
		m_vNumber.push_back(m_v4Number[(int)lParam/10]);
		m_vName.push_back(m_sName);
	}
	
	m_iSelectNum++;
	if ( m_iSelectNum >= m_vClick.size())
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if (1 == m_ntype)//来至于写短信
		{
			main->m_pMainDlg->m_p3GSMSDlg->SetSender(m_vName);
			main->m_pMainDlg->m_p3GSMSDlg->SetAppend(m_vNumber);
			main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
		}
		else if ( 2 == m_ntype)//来至于写彩信
		{
			main->m_pMMSDlg->SetSender(m_vName);
			main->m_pMMSDlg->SetAppend(m_vNumber);
			main->m_pMMSDlg->ShowWindow(SW_SHOW);
		}
		m_pSelectNumberDlg.ShowWindow_(SW_HIDE);
		m_ntype = 0;
	}
	else
	{
		Contact2Sms();
	}
}
void CContactDlg::SetSimID(int id)
{
	m_iSimID = id ;
}

int CContactDlg::GetSimID()
{
	return m_iSimID ;
}

void CContactDlg::BackupSim()
{	
	int able = false  ;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if ( m_vClick[i] == 1)
		{
			able = true ;
			break;
		}
	}

	if (able)
	{
		if (m_iCurrentGroupID == m_iSimID)
		{	
			m_simImportDlg.SetHWnd(GetSafeHwnd());
			m_simImportDlg.SetUnit(SIMtoLocal);
			m_simImportDlg.ShowWindow_(SW_SHOW);
		}
		else if (m_iCurrentGroupID > 0)
		{	
			m_simImportDlg.SetHWnd(GetSafeHwnd());
			m_simImportDlg.SetUnit(LocaltoSIM);
			m_simImportDlg.ShowWindow_(SW_SHOW);
		}
	}

}

void CContactDlg::ShowWindow_()
{		
	FromGroupDataBase();
	ShowUpGroupInfo();

	SetUpBtn(0);
	Clear();
	ClearCurrentPage();
	SetContactFilter(0);
	FromContactDataBase();
	ShowGroupInfo();

	ShowWindow(SW_SHOW);

}