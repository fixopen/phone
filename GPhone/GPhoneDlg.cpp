// GPhoneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPhone.h"
#include "GPhoneDlg.h"

#include <ras.h>

#include "Util/Text/StringOp.h"
#include "Util/Log.h"
#include "Util/AT/Register.h"
#include "Util/AT/NetworkDial.h"
#include "Telephone/Logical/AtTelephone.h"
#include "Telephone/View/CallDialog.h"
#include "Sms/Logical/SmsManager.h"
#include "Protocol/KeySession.h"
#include "Mms/Data/MmsMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGPhoneDlg* const mainForm() {
    static CGPhoneDlg* result = 0;
    if (!result) {
        result = new CGPhoneDlg(0);
        result->Create(CGPhoneDlg::IDD, 0);
    }
    return result;
}

// CGPhoneDlg 对话框

CGPhoneDlg::CGPhoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGPhoneDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGPhoneDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGPhoneDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON1, &CGPhoneDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CGPhoneDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CGPhoneDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CGPhoneDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CGPhoneDlg::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON6, &CGPhoneDlg::OnBnClickedButton6)
    ON_BN_CLICKED(IDC_BUTTON7, &CGPhoneDlg::OnBnClickedButton7)
    ON_BN_CLICKED(IDC_BUTTON8, &CGPhoneDlg::OnBnClickedButton8)
    ON_BN_CLICKED(IDC_BUTTON9, &CGPhoneDlg::OnBnClickedButton9)
    ON_BN_CLICKED(IDC_BUTTON10, &CGPhoneDlg::OnBnClickedButton10)
    ON_BN_CLICKED(IDC_BUTTON11, &CGPhoneDlg::OnBnClickedButton11)
END_MESSAGE_MAP()


// CGPhoneDlg 消息处理程序

namespace {
    void echo(unsigned char const* const data, size_t const length) {
        Util::Log::Log(Util::Log::Entry(reinterpret_cast<char const* const>(data)));
    }
}

BOOL CGPhoneDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//设置log设施
    Util::Log::Sink* fileSink = new Util::Log::Sink();
    Util::Log::Destination* fileDst = new Util::Log::FileDestination("log.log"); //normal file
    fileSink->SetDestination(fileDst);
    Util::Log::Formatter* fileFmt = new Util::Log::Formatter();
    fileSink->SetFormatter(fileFmt);
    int fileSinkId = Util::Log::AddProcess(fileSink, -1);
    logIds_.push_back(fileSinkId);

    Util::Log::Sink* consoleSink = new Util::Log::Sink();
    Util::Log::Destination* consoleDst = new Util::Log::FileDestination(); //console stdout
    consoleSink->SetDestination(consoleDst);
    Util::Log::Formatter* consoleFmt = new Util::Log::Formatter();
    consoleSink->SetFormatter(consoleFmt);
    int consoleSinkId = Util::Log::AddProcess(consoleSink, fileSinkId);
    logIds_.push_back(consoleSinkId);

    //Util::Log::Sink* traceSink = new Util::Log::Sink();
    //Util::Log::Destination* traceDst = new Util::Log::TraceDestination(); //TRACE
    //traceSink->SetDestination(traceDst);
    //Util::Log::Formatter* traceFmt = new Util::Log::Formatter();
    //traceSink->SetFormatter(traceFmt);
    //int traceSinkId = Util::Log::AddProcess(traceSink, consoleSinkId);
    //logIds_.push_back(traceSinkId);

    //Util::Log::Sink* uiSink = new Util::Log::Sink();
    //Util::Log::Destination* uiDst = new Util::Log::UIDestination(); //MessageBox
    //uiSink->SetDestination(uiDst);
    //Util::Log::Formatter* uiFmt = new Util::Log::Formatter();
    //uiSink->SetFormatter(uiFmt);
    //int uiSinkId = Util::Log::AddProcess(uiSink, traceSinkId);
    //logIds_.push_back(uiSinkId);

    //redirect to call form
    //OnBnClickedButton3();

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CGPhoneDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/) {
	if (AfxIsDRAEnabled()) {
		DRA::RelayoutDialog(AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_GPHONE_DIALOG_WIDE) : MAKEINTRESOURCE(IDD_GPHONE_DIALOG));
	}
}
#endif


void CGPhoneDlg::OnDestroy() {
    CDialog::OnDestroy();

    //停止3G模块
    Util::AT::Register::Instance()->Stop();

    //撤销log设施
    for (size_t i = 0; i < logIds_.size(); ++i) {
        Util::Log::RemoveProcess(logIds_[i]);
    }
}

LRESULT CGPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	std::pair<int, std::wstring> result;
	switch (message) {
		case WM_RASDIALEVENT: //0xCCCD
			//std::pair<int, std::wstring> result = Util::ATCommand::OnRasStateChanged(wParam, lParam);
            Util::Log::Log(Util::AT::Dialer::Instance()->OnRasStateChanged(wParam, lParam).second);
			break;
        default:
            break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CGPhoneDlg::OnBnClickedButton1() {
    Telephone::Logical::AtTelephone::Instance()->VideoAnswer();
}

void CGPhoneDlg::OnBnClickedButton2() {
    Telephone::Logical::AtTelephone::Instance()->VideoHangup();
}

void CGPhoneDlg::OnBnClickedButton3() {
    //Set display window handle
    //if (false) {
    //    FILE* file = 0;
    //    fopen_s(&file, "/FlashDrv/test/number.txt", "w");
    //    if (file) {
    //        fprintf(file, "%s", "18811038175");
    //        fclose(file);
    //    }
    //}
    //Telephone::Logical::AtTelephone::Instance()->SetVideoChannel(6);
    //HWND local = Telephone::View::callForm()->GetLocalDisplay();
    //HWND remote = Telephone::View::callForm()->GetRemoteDisplay();
    //Telephone::Logical::AtTelephone::Instance()->SetDisplayWindow(local, remote);
    //Telephone::Logical::AtTelephone::Instance()->InitCamera();
    //FILE* file = 0;
    //fopen_s(&file, "/FlashDrv/test/number.txt", "r");
    //if (file) {
    //    char number[256] = {0};
    //    fscanf(file, "%s", number);
    //    fclose(file);
    //    Telephone::Logical::AtTelephone::Instance()->VideoDial(number);
    //}
    //Telephone::Logical::AtTelephone::Instance()->Dial("13693607335");
    //Telephone::Logical::AtTelephone::Instance()->VideoDial("10086");
    Telephone::Logical::AtTelephone::Instance()->VideoDial("18811038175");
}

void CGPhoneDlg::OnBnClickedButton4() {
    std::string datas[] = {
        //"+CMT: \"\",100\xD\xA" "0891683108100005F0040D91683126219370F9000801109081148223508001516CFF0C003A002D002983CA56ED4E0B8F66540EFF0C523062115E9796448FD176846C34679C5E974E704E9B82F9679C5427FF0C5C0F5BB64F1960F35403FF0C62117ED95FD88BB04E704E863002",
        "+CMT: \"\",155\xD\xA" "0891683108100005F0600C91015618451020000401109091404423880605040B8423F0BA06246170706C69636174696F6E2F766E642E7761702E6D6D732D6D65737361676500B487AF848C82984A544573784F794561774C45008D9083687474703A2F2F3232312E3137392E3138352E3233312F4A544573784F794561774C45008805810302A2FF89148031303635383030302F545950453D504C4D4E008A808E02B074",
        "+CMT: \"\",155\xD\xA" "0891683108100005F0640C91015618451020000401101190308123880605040B8423F02706246170706C69636174696F6E2F766E642E7761702E6D6D732D6D65737361676500B487AF848C82984C4A44525A72502B52484145008D9083687474703A2F2F3232312E3137392E3138352E3233312F4C4A44525A72502B52484145008805810302A30089148031303635383030302F545950453D504C4D4E008A808E0289F0",
        "+CMT: \"\",155\xD\xA" "0891683108100005F0600C91015618451020000401100101506323880605040B8423F00306246170706C69636174696F6E2F766E642E7761702E6D6D732D6D65737361676500B487AF848C82984B4B466B68384F455A476744008D9083687474703A2F2F3232312E3137392E3138352E3233312F4B4B466B68384F455A476744008805810302A30089148031303635383030302F545950453D504C4D4E008A808E02B059",
        "+CMT: \"\",155\xD\xA" "0891683108100005F0640C91015618451020000401100181955123880605040B8423F02206246170706C69636174696F6E2F766E642E7761702E6D6D732D6D65737361676500B487AF848C82984B5337507836575743516143008D9083687474703A2F2F3232312E3137392E3138352E3233312F4B5337507836575743516143008805810302A30089148031303635383030302F545950453D504C4D4E008A808E02A9F6",
        "+CMT: \"\",153\xD\xA" "0891683108100005F064089101561845000490013290300223880605040B8423F0DD06246170706C69636174696F6E2F766E642E7761702E6D6D732D6D65737361676500B487AF848C8298584A4454686A427337416741008D9083687474703A2F2F3232312E3137392E3138352E3233312F584A4454686A427337416741008805810302A2FF89148031303635383030302F545950453D504C4D4E008A808E0281C1",
        "+CMT: \"\",155\xD\xA" "0891683108100005F0640C91015618451020000401105190119423880605040B8423F0BA06246170706C69636174696F6E2F766E642E7761702E6D6D732D6D65737361676500B487AF848C8298504A4C785A59335879593041008D9083687474703A2F2F3232312E3137392E3138352E3233312F504A4C785A59335879593041008805810302A30089148031303635383030302F545950453D504C4D4E008A808E02A999",

    };
    for (size_t i = 0; i < sizeof datas / sizeof datas[0]; ++i) {
        Sms::Logical::SmsManager::Instance()->Update(datas[i]);
    }
}

void CGPhoneDlg::OnBnClickedButton5() {
    //Protocol::KeySession key;
    //key.Process();

    //::Sleep(100 * 1000);
    //std::string v = "842c618d45297e18f0799c674d7915d146b2869ad3d2d22eca8279a243e7e7e8268e3b76a07dd37ec8b83cba1d9d8e4d4d4be3725dcdb8dd57b3c160de7c137b89238984ca87ef2ded51423bd03be513280903117bc554c93ad5ccdc1b80d580baf481d5346683b2d09d7ca937e1fb4242aadaae0f3b4485820251fbbf13aef7";
    //std::string a = Util::StringOp::Base16ToArray(v);
    //Util::Log::Log(a);

    //unsigned char a[1024] = {
    //    0x84, 0x2c, 0x61, 0x8d, 0x45, 0x29, 0x7e, 0x18, 0xf0, 0x79, 0x9c, 0x67,
    //    0x4d, 0x79, 0x15, 0xd1, 0x46, 0xb2, 0x86, 0x9a, 0xd3, 0xd2, 0xd2, 0x2e,
    //    0xca, 0x82, 0x79, 0xa2, 0x43, 0xe7, 0xe7, 0xe8, 0x26, 0x8e, 0x3b, 0x76,
    //    0xa0, 0x7d, 0xd3, 0x7e, 0xc8, 0xb8, 0x3c, 0xba, 0x1d, 0x9d, 0x8e, 0x4d,
    //    0x4d, 0x4b, 0xe3, 0x72, 0x5d, 0xcd, 0xb8, 0xdd, 0x57, 0xb3, 0xc1, 0x60,
    //    0xde, 0x7c, 0x13, 0x7b, 0x89, 0x23, 0x89, 0x84, 0xca, 0x87, 0xef, 0x2d,
    //    0xed, 0x51, 0x42, 0x3b, 0xd0, 0x3b, 0xe5, 0x13, 0x28, 0x09, 0x03, 0x11,
    //    0x7b, 0xc5, 0x54, 0xc9, 0x3a, 0xd5, 0xcc, 0xdc, 0x1b, 0x80, 0xd5, 0x80,
    //    0xba, 0xf4, 0x81, 0xd5, 0x34, 0x66, 0x83, 0xb2, 0xd0, 0x9d, 0x7c, 0xa9,
    //    0x37, 0xe1, 0xfb, 0x42, 0x42, 0xaa, 0xda, 0xae, 0x0f, 0x3b, 0x44, 0x85,
    //    0x82, 0x02, 0x51, 0xfb, 0xbf, 0x13, 0xae, 0xf7
    //};
}

void CGPhoneDlg::OnBnClickedButton6() {
    int const comPorts[] = {
        3,
        2,
        1,
        4,
        5,
        6,
        7,
        8,
        9,
    };

    for (size_t i = 0; i < sizeof comPorts / sizeof comPorts[0]; ++i) {
        Util::RS232* port = new Util::RS232();
        port->Open(comPorts[i], 0);
        port->SetReadFunc(echo);
        std::string command = "AT\xD";
        port->Write(command);
        Util::Log::Log(command);
        ::Sleep(1000);
        port->Close();
        delete port;
    }
}

void CGPhoneDlg::OnBnClickedButton7() {
    //启动3G模块并初始化
    Util::AT::Register::Instance()->Start();

    while (true) {
        if (Util::AT::Register::Instance()->IsRegistered()) {
            Telephone::Logical::AtTelephone::Instance()->SetVideoChannel(6);
            break;
        } else {
            ::Sleep(100);
        }
    }
}

void CGPhoneDlg::OnBnClickedButton8() {
    HWND local = GetDlgItem(IDC_STATIC_LOCAL)->m_hWnd;
    HWND remote = GetDlgItem(IDC_STATIC_REMOTE)->m_hWnd;
    Telephone::Logical::AtTelephone::Instance()->SetDisplayWindow(local, remote);
    Telephone::Logical::AtTelephone::Instance()->InitCamera();
}

void CGPhoneDlg::OnBnClickedButton9() {
    int const bytes[] = {
        31771,
        3493,
        28110,
        5725,
        7783,
        2213,
        4343,
        10363,
        8653,
        23577,
        8817,
        4499,
        2367,
        13659,
        4229,
        11653,
        3388,
        6045,
    };
    int const ticks[] = {
        152346,
        153177,
        158501,
        159715,
        161145,
        161555,
        162419,
        164790,
        166387,
        170914,
        172608,
        173480,
        173936,
        176496,
        177240,
        179472,
        180078,
        181003,
    };
    int totalBytes = 0;
    //for (size_t i = 0; i < sizeof bytes / sizeof bytes[0]; ++i) {
    //    totalBytes += bytes[i];
    //}
    //size_t ticksCount = sizeof ticks / sizeof ticks[0];
    size_t ticksCount = Telephone::Logical::AtTelephone::Instance()->receiveTicks_.size();

    for (size_t i = 0; i < ticksCount; ++i) {
        totalBytes += Telephone::Logical::AtTelephone::Instance()->receiveDataSize_[i];
    }

    //int totalTicks = ticks[ticksCount - 1] - ticks[0];
    int totalTicks = Telephone::Logical::AtTelephone::Instance()->receiveTicks_[ticksCount - 1] - Telephone::Logical::AtTelephone::Instance()->receiveTicks_[0];

    int speed = totalBytes / totalTicks;
    int tickInterval = totalTicks / ticksCount;
    Util::Log::Log(Util::Text::StringOp::FromInt(totalBytes));
    Util::Log::Log(Util::Text::StringOp::FromInt(totalTicks));
    Util::Log::Log(Util::Text::StringOp::FromInt(speed));
    Util::Log::Log(Util::Text::StringOp::FromInt(tickInterval));
}

void CGPhoneDlg::OnBnClickedButton10() {
    char* filename = "/FlashDrv/test/jpg.txt";
    FILE* file = 0;
    fopen_s(&file, filename, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        size_t length = ftell(file);
        fseek(file, 0, SEEK_SET);
        unsigned char* data = (unsigned char*)malloc(length);
        fread(data, 1, length, file);
        fclose(file);
        size_t pos = 155;
        Mms::Data::MmsMessage m = Mms::Data::MmsMessage::Parse(data, length, pos);
        free(data);
    }
}

void CGPhoneDlg::OnBnClickedButton11() {
    //@@OnOk();
}
