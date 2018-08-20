// RecordPhone.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "RecordPhoneDlg.h"
#include "Screen.h"

#include "Util/ImageOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRecordPhoneApp

BEGIN_MESSAGE_MAP(CRecordPhoneApp, CWinApp)
END_MESSAGE_MAP()


// CRecordPhoneApp construction
CRecordPhoneApp::CRecordPhoneApp()
: CWinApp() {
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CRecordPhoneApp object
CRecordPhoneApp theApp;

// CRecordPhoneApp initialization

BOOL CRecordPhoneApp::InitInstance() {

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    //prepare background image
	std::map<std::wstring, SIZE> images;
	images.insert(std::make_pair(L"/FlashDrv/debug/NULL.jpg", Screen::size()));
	//prepare button image
	SIZE bigButtonSize;
	bigButtonSize.cx = 130;
	bigButtonSize.cy = 130;
	images.insert(std::make_pair(L"/FlashDrv/debug/contacts.jpg", bigButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/records.jpg", bigButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/setting.jpg", bigButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/telephoneSetting.jpg", bigButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/sysParameters.jpg", bigButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/sysInfo.jpg", bigButtonSize));
	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	images.insert(std::make_pair(L"/FlashDrv/debug/message.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/call.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/newCategory.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/newContact.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/clear.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/pageup.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/pagedown.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/hidden.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/restore.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/answer.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/hangup.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/find.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/playMessage.jpg", buttonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/import.jpg", buttonSize));
	SIZE navButtonSize;
	navButtonSize.cx = 50;
	navButtonSize.cy = 50;
	images.insert(std::make_pair(L"/FlashDrv/debug/previous.jpg", navButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/next.jpg", navButtonSize));
	SIZE playControlButtonSize;
	playControlButtonSize.cx = 68;
	playControlButtonSize.cy = 68;
	images.insert(std::make_pair(L"/FlashDrv/debug/play.jpg", playControlButtonSize));
	images.insert(std::make_pair(L"/FlashDrv/debug/pause.jpg", playControlButtonSize));
	Util::ImageOp::PrepareImage(images);

	CRecordPhoneDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
