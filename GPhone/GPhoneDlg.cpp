// GPhoneDlg.cpp : ʵ���ļ�
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

// CGPhoneDlg �Ի���

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


// CGPhoneDlg ��Ϣ�������

BOOL CGPhoneDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//����log��ʩ
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

    //����3Gģ�鲢��ʼ��
    Util::ATCommand::Instance()->Start();

    //����/*��Ƶ*/�绰
    //Telephone::Logical::TelephoneWarp::Instance()->Dial("13693607335");
    Telephone::Logical::TelephoneWarp::Instance()->VideoDial("10086");
    //::Sleep(100 * 1000);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

    //ֹͣ3Gģ��
    Util::ATCommand::Instance()->Stop();

    //����log��ʩ
    Util::Log::RemoveProcess(logIds_[0]);
    Util::Log::RemoveProcess(logIds_[1]);
}
