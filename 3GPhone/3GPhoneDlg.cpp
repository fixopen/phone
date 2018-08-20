// 3GPhoneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3GPhone.h"
#include "3GPhoneDlg.h"

#include "Util/log.h"
#include "Util/ATCommandWarp.h"

//#include "Util/xmlLoad.h"
//#include "Util/DocSource.h"

#include "Telephone/Logical/TelephoneWarp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy3GPhoneDlg �Ի���

CMy3GPhoneDlg::CMy3GPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy3GPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3GPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy3GPhoneDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMy3GPhoneDlg ��Ϣ�������

BOOL CMy3GPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    //Util::shared_ptr<Util::DocSource> source(new Util::DocSource("/temp/test.xml"));
    //Util::xmlLoad loader;
    //Util::shared_ptr<Xml::Document> doc = loader.parse(source);
    //Xml::Node const& bs = (*doc)[L"bindings"];
    //std::wstring i = bs[L"binding"][L"@id"].value();
    //std::wstring x = (*doc)[L"bindings"][L"binding"][L"content"][L"svg:svg"][L"svg:g"](L"svg:circle", 1)[L"@fill"].value();

    Util::Log::Sink* processor = new Util::Log::Sink();
    Util::Log::Destination* dst = new Util::Log::FileDestination("log.log");
    processor->SetDestination(dst);
    Util::Log::Formatter* fmt = new Util::Log::Formatter();
    processor->SetFormatter(fmt);
    Util::Log::AddProcess(processor, -1);

    Util::ATCommand* at = new Util::ATCommand(8);
    at->Write("AT");
    std::string data = at->Read();
    Util::Log::Log(data);
    at->Write("AT");
    data = at->Read();
    Util::Log::Log(data);
    at->Write("AT");
    data = at->Read();
    Util::Log::Log(data);
    //at->Init(8);
    //at->Init3GModule();
    //at->Startup(); //same as at->RegisterNetwork();
    //Telephone::Logical::TelephoneWarp* telephone = new Telephone::Logical::TelephoneWarp(at);
    //telephone->Dial("13693607335");
    //::Sleep(100 * 1000);
    //at->ShutDown(); //same as at->UnregisterNetwork();
    //at->Finally();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CMy3GPhoneDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_MY3GPHONE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_MY3GPHONE_DIALOG));
	}
}
#endif

