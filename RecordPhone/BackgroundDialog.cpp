// BackgroundDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "BackgroundDialog.h"

#include "Util/ImageOp.h"

#include <afxstr.h>
#include <atlimage.h>

// CBackgroundDialog dialog

IMPLEMENT_DYNAMIC(CBackgroundDialog, CDialog)

CBackgroundDialog::CBackgroundDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
, backgroundDc_(0)
, thisDc_(0) {
}

CBackgroundDialog::~CBackgroundDialog() {
}

void CBackgroundDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBackgroundDialog, CDialog)
END_MESSAGE_MAP()


// CBackgroundDialog message handlers
void CBackgroundDialog::drawBackground_(std::wstring const& imageFilename, SIZE const& imageSize) {
    backgroundImageFilename_ = imageFilename;
    backgroundImageSize_ = imageSize;
    backgroundDc_ = Util::ImageOp::GetImage(imageFilename, imageSize);
    if (!thisDc_) {
        thisDc_ = ::GetDC(m_hWnd);
    }
	HDC thisDc = ::GetDC(m_hWnd);
	if (!thisDc) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
    RECT bounds;
    GetWindowRect(&bounds);
	//::LineTo(thisDc, 32, 200);
	::Sleep(1000);
    BOOL r = ::BitBlt(thisDc, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top, backgroundDc_, 0, 0, SRCCOPY);
    if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
    }
    //r = ::BitBlt(thisDc_, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top, backgroundDc_, 0, 0, SRCCOPY);
    //if (!r) {
    //    DWORD errorCode = GetLastError();
    //    DWORD ec = errorCode;
    //}
    ::ReleaseDC(m_hWnd, thisDc);
}

HBRUSH const CBackgroundDialog::controlColor_(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    HBRUSH result = 0;
    if (backgroundDc_) {
        result = (HBRUSH)GetStockObject(NULL_BRUSH); //disable control draw background
        RECT bounds;
        //HDC windowDC = ::GetDC(m_hWnd);
        HDC windowDC = Util::ImageOp::GetImage(backgroundImageFilename_, backgroundImageSize_); //backgroundDc_;
        BOOL r = TRUE;
        switch (nCtlColor) {
            case CTLCOLOR_BTN:
                break;
            case CTLCOLOR_STATIC:
                pDC->SetTextColor(RGB(255, 255, 255));
                pDC->SetBkMode(TRANSPARENT);
                //pWnd->ModifyStyle(0, 1);
                pWnd->GetClientRect(&bounds);
                //pWnd->GetWindowRect(&bounds);
                //fill control background by parent background
                r = BitBlt(pDC->m_hDC, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top, windowDC, bounds.left, bounds.top, SRCCOPY);
                if (!r) {
                    DWORD errorCode = GetLastError();
                    DWORD ec = errorCode;
                }
                break;
            case CTLCOLOR_EDIT:
                break;
            //case CTLCOLOR_SCROLLBAR:
            //    break;
            case CTLCOLOR_LISTBOX:
                break;
            case CTLCOLOR_MSGBOX:
                break;
            //case CTLCOLOR_DLG:
            //    break;
            default:
                break;
        }
        ::ReleaseDC(m_hWnd, windowDC);
    }
    return result;
}
