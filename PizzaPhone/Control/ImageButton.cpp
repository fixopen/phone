#include "ImageButton.h"
#include "../Util/Video/Screen.h"
#include "../Util/Video/ImageOp.h"

namespace Control {
    // ImageButton

    IMPLEMENT_DYNAMIC(ImageButton, CButton)

    ImageButton::ImageButton(std::wstring const& imageFilename, Util::Video::Size const& imageSize)
    : CButton()
    , imageFilename_(imageFilename)
    , imageSize_(imageSize)
    , textColor_(RGB(255, 255, 255))
    , fontHeight_(20) {
    }

    ImageButton::~ImageButton() {
        //WNDPROC OldProc = (WNDPROC)SetWindowsLong(hWnd, GWL_WNDPROC, (LONG)NewProc);
        //BOOL SubclassWindow(hWnd);
        //HWND UnsubclassWindow()
        //LRESULT CallWindowProc(OldProc, hWnd, msg, wParam, lParam);
    }

    //BOOL ImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, RECT const& bounds, CWnd* pParentWnd, UINT nID) {
    //BOOL ret = CButton::Create(lpszCaption, dwStyle, bounds, pParentWnd, nID);

    //VERIFY(m_font.CreateFont(
    //	m_nFontHeight,             // nHeight
    //	0,                         // nWidth
    //	0,                         // nEscapement
    //	0,                         // nOrientation
    //	FW_NORMAL,                 // nWeight
    //	FALSE,                     // bItalic
    //	FALSE,                     // bUnderline
    //	0,                         // cStrikeOut
    //	ANSI_CHARSET,              // nCharSet
    //	OUT_DEFAULT_PRECIS,        // nOutPrecision
    //	CLIP_DEFAULT_PRECIS,       // nClipPrecision
    //	DEFAULT_QUALITY,           // nQuality
    //	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
    //	_T("ËÎÌו")));              // lpszFacename
    //SetFont(&m_font);

    //return ret;
    //}

    void ImageButton::SetText(std::wstring const& text, COLORREF const& textColor, size_t const fontHeight) {
        text_ = text;
        textColor_ = textColor;
        fontHeight_ = fontHeight;
    }

    void ImageButton::SetImage(std::wstring const& imageFilename, Util::Video::Size const& imageSize) {
        imageFilename_ = imageFilename;
        imageSize_ = imageSize;
    }

    void ImageButton::SetImage(std::wstring const& imageFilename, Util::Video::Size const& imageSize, std::wstring const& maskFilename) {
        imageFilename_ = imageFilename;
        imageSize_ = imageSize;
        maskFilename_ = maskFilename;
    }

    BEGIN_MESSAGE_MAP(ImageButton, CButton)
        ON_WM_DRAWITEM()
    END_MESSAGE_MAP()

    void ImageButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) {
        //draw background
        HDC commonButtonDc = Util::Video::ImageOp::GetImage(imageFilename_, imageSize_);
        if (maskFilename_ == L"") {
            ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCCOPY);
        } else {
            HDC backgroundDc = Util::Video::ImageOp::GetImage(L"/FlashDrv/debug/NULL.jpg", Util::Video::Screen::size());
            RECT bounds;
            GetWindowRect(&bounds);
            ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, backgroundDc, bounds.left, bounds.top, SRCCOPY);
            HDC maskDc = Util::Video::ImageOp::GetMaskImage(maskFilename_, imageSize_);
            ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCINVERT);
            ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, maskDc, 0, 0, SRCAND);
            ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCINVERT);
            //HBITMAP mask = Util::Video::ImageOp::GetMaskImage(maskFilename_, imageSize_);
            //::MaskBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, mask, 0, 0, SRCCOPY);
        }
        //::SetTextColor(lpDIS->hDC, textColor_);
        //::SetBkMode(lpDIS->hDC, TRANSPARENT);
        //font_.CreateFont(fontHeight_, // nHeight
        //	0,                         // nWidth
        //	0,                         // nEscapement
        //	0,                         // nOrientation
        //	FW_NORMAL,                 // nWeight
        //	FALSE,                     // bItalic
        //	FALSE,                     // bUnderline
        //	0,                         // cStrikeOut
        //	ANSI_CHARSET,              // nCharSet
        //	OUT_DEFAULT_PRECIS,        // nOutPrecision
        //	CLIP_DEFAULT_PRECIS,       // nClipPrecision
        //	DEFAULT_QUALITY,           // nQuality
        //	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        //	L"ËÎÌו");                  // lpszFacename
        //::SelectObject(lpDIS->hDC, static_cast<HGDIOBJ>(static_cast<HFONT>(font_)));
        //if (text_ == L"") {
        //	CString text;
        //	GetWindowText(text);
        //	text_ = static_cast<LPCTSTR>(text);
        //}
        //CRect bounds;
        //GetClientRect(&bounds);
        //bounds.top += (bounds.Height() - 16) / 2;
        //::DrawText(lpDIS->hDC, text_.c_str(), text_.length(), bounds, DT_WORDBREAK | DT_CENTER);
    }

    void ImageButton::PreSubclassWindow() {
        DWORD dwStyle = GetStyle();
        ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | BS_OWNERDRAW);
        CButton::PreSubclassWindow();
    }

    // ImageButton message handlers

}
