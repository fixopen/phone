// GPhoneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPhone.h"
#include "GPhoneDlg.h"

#include "Util/log.h"
#include "Util/ATCommandWarp.h"
#include "Telephone/Logical/TelephoneWarp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
END_MESSAGE_MAP()


// CGPhoneDlg 消息处理程序

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

    //启动3G模块并初始化
    Util::ATCommand::Instance()->Start();

    //拨打/*视频*/电话
    //Telephone::Logical::TelephoneWarp::Instance()->Dial("13693607335");
    Telephone::Logical::TelephoneWarp::Instance()->VideoDial("10086");
    //::Sleep(100 * 1000);

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
    Util::ATCommand::Instance()->Stop();

    //撤销log设施
    Util::Log::RemoveProcess(logIds_[0]);
    Util::Log::RemoveProcess(logIds_[1]);
}
