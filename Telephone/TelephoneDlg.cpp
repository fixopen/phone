// TelephoneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Telephone.h"
#include "TelephoneDlg.h"
#include "Util/Debugger.h"
#include "Util/Logger.h"
#include "Util/Text/StringOp.h"
#include "Device/USB.h"
#include "Device/SerialPort.h"
#include "Device/SDCard.h"
#include "Device/Dialpad.h"
#include "InputMethod/PinYinInputMethod.h"
#include "InputMethod/HandDrawInputMethod.h"
#include "InputMethod/Legend.h"
#include "Database/Database.h"
#include "HttpClient/Executor.h"
#include "json/reader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTelephoneDlg �Ի���

CTelephoneDlg::CTelephoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTelephoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTelephoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTelephoneDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTelephoneDlg ��Ϣ�������

BOOL CTelephoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//FILE* stream = _wfreopen(L"\\hive\\stdErr.out", L"w", stderr);
	//Util::Debugger::Print("Start!");
	////load setting
	//Util::Properties::Init("\\hive\\setting.dat");

	////Timer for screensaver
	//Util::Timer screenSaverTimer = new Util::Timer(...);

	//Util::Logger::log("Start!");
	//Device::USB::StartMonitor();
	//Device::USB::IsAttachDisk();
	//Device::SerialPort::StartMonitor();
	//Device::SDCard::StartMonitor();
	//Device::Flash::PreparePaths(); //for ring, record
	//Device::Dialpad::StartMonitor();
	//Device::TouchPanel::Calibrate();
	//Device::ScreenController::BackLight::On();
	//Device::PowerController;
	//InputMethod::PinYinInputMethod::Init();
	//InputMethod::HandDrawInputMethod::Init();
	//InputMethod::Legend::Init();
	//Database::Database::Init();
	//View::SoftwareKeyboard::Init();
	//Logical::Telephone::Start();

	////Information, Notify, Warning, Error and Challenge dialog

	//Util::Debugger::Print("Stop!");
	
	////test http
	HttpClient::Request req;
	req.method = L"POST";
	req.hostName = L"192.168.1.190";
	req.port = 8080;
	req.uri = L"/recordPhone/CallTypes";
	req.uri = L"/recordPhone/calls";
	req.headers.insert(std::make_pair(L"Host", req.hostName));
	//req.headers.insert(std::make_pair(L"User-Agent", L"Mozilla/5.0 (Windows NT 5.1; rv:25.0) Gecko/20100101 Firefox/25.0"));
	req.headers.insert(std::make_pair(L"Accept", L"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"));
	req.headers.insert(std::make_pair(L"Accept-Language", L"zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3"));
	req.headers.insert(std::make_pair(L"Accept-Encoding", L"gzip, deflate"));
	//req.headers.insert(std::make_pair(L"Referer", L"http://192.168.1.190:8080/testWorkbench.html"));
	req.headers.insert(std::make_pair(L"Content-Type", L"text/plain; charset=UTF-8"));
	//req.headers.insert(std::make_pair(L"Content-Type", L"application/json; charset=UTF-8"));
	//req.headers.insert(std::make_pair(L"Connection", L"keep-alive"));
	//req.headers.insert(std::make_pair(L"Pragma", L"no-cache"));
	//req.headers.insert(std::make_pair(L"Cache-Control", L"no-cache"));
	using namespace Util::Text::StringOp;
	req.body = ToUTF8(L"{\"callWay\":\"����\",\"homePhone\":\"82781281\",\"telephone\":\"18210417393\",\"productCode\":\"\",\"startTime\":\"2013-11-22 13:49:27\",\"callTypesId\":1,\"recordType\":\"�绰¼��\",\"typeCall\":\"new\"}");
	req.headers.insert(std::make_pair(L"Content-Length", FromInt(req.body.length())));
	HttpClient::Response const r =  HttpClient::WinInetExecutor::Do(req);
	json::Object o;
	json::Reader::Read(o, std::stringstream(r.body));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CTelephoneDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_TELEPHONE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_TELEPHONE_DIALOG));
	}
}
#endif

