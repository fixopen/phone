#include "ImageStatic.h"
#include "../Image/ImageOp.h"
#include "../Image/BackgroundImage.h"

namespace Util {
    namespace Control {
        // ImageStatic

        IMPLEMENT_DYNAMIC(ImageStatic, CStatic)

        ImageStatic::ImageStatic() {
        }

        ImageStatic::~ImageStatic() {
        }

        void ImageStatic::SetText(std::wstring const& text, COLORREF const& textColor, size_t const fontHeight) {
            text_ = text;
            textColor_ = textColor;
            fontHeight_ = fontHeight;
        }

        void ImageStatic::SetImage(std::wstring const& imageFilename, Size const& imageSize) {
            imageFilename_ = imageFilename;
            imageSize_ = imageSize;
        }

        void ImageStatic::SetImage(std::wstring const& imageFilename, Size const& imageSize, std::wstring const& maskFilename) {
            imageFilename_ = imageFilename;
            imageSize_ = imageSize;
            maskFilename_ = maskFilename;
        }


        BEGIN_MESSAGE_MAP(ImageStatic, CStatic)
            //ON_WM_DRAWITEM()
            //ON_WM_DRAWITEM_REFLECT()
            ON_WM_ERASEBKGND()
            ON_WM_PAINT()
        END_MESSAGE_MAP()

        void ImageStatic::PreSubclassWindow() {
            ModifyStyle(0, SS_OWNERDRAW);
            //DWORD dwStyle = GetStyle();
            //::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW);
            CStatic::PreSubclassWindow();
        }

        afx_msg BOOL ImageStatic::OnEraseBkgnd(CDC* pDC) {
            //Don't call CStatic::OnEraseBkgnd
            return TRUE;
        }

        afx_msg void ImageStatic::OnPaint() {
            CPaintDC dc(this);
            HDC commonButtonDc = Util::Image::ImageOp::GetImage(imageFilename_, imageSize_);
            HDC thisDc = ::GetDC(m_hWnd);

            if (maskFilename_ == L"") {
                ::BitBlt(thisDc/*lpDIS->hDC*/, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCCOPY);
            } else {
                HDC backgroundDc = Util::Image::ImageOp::GetImage(BackgroundImage::BackgroundImageFilename, BackgroundImage::ScreenSize);
                RECT bounds;
                GetWindowRect(&bounds);
                ::BitBlt(thisDc/*lpDIS->hDC*/, 0, 0, imageSize_.width, imageSize_.height, backgroundDc, bounds.left, bounds.top, SRCCOPY);
                HDC maskDc = Util::Image::ImageOp::GetMaskImage(maskFilename_, imageSize_);
                ::BitBlt(thisDc/*lpDIS->hDC*/, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCINVERT);
                ::BitBlt(thisDc/*lpDIS->hDC*/, 0, 0, imageSize_.width, imageSize_.height, maskDc, 0, 0, SRCAND);
                ::BitBlt(thisDc/*lpDIS->hDC*/, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCINVERT);
                //HBITMAP mask = Util::Image::ImageOp::GetMaskImage(maskFilename_, imageSize_);
                //::MaskBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, mask, 0, 0, SRCCOPY);
            }

            ::ReleaseDC(m_hWnd, thisDc);
            //Don't call CStatic::OnPaint
        }

        //LRESULT ImageStatic::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        //  ATLTRACE(_T("CColorStatic::OnEraseBackground()\n"));
        //  return TRUE; // indicates we took care of drawing
        //}

        // ImageStatic message handlers

        void ImageStatic::DrawItem(LPDRAWITEMSTRUCT lpDIS) {
            //draw background
            HDC commonButtonDc = Util::Image::ImageOp::GetImage(imageFilename_, imageSize_);

            if (maskFilename_ == L"") {
                ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCCOPY);
            } else {
                HDC backgroundDc = Util::Image::ImageOp::GetImage(BackgroundImage::BackgroundImageFilename, BackgroundImage::ScreenSize);
                RECT bounds;
                GetWindowRect(&bounds);
                ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, backgroundDc, bounds.left, bounds.top, SRCCOPY);
                HDC maskDc = Util::Image::ImageOp::GetMaskImage(maskFilename_, imageSize_);
                ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCINVERT);
                ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, maskDc, 0, 0, SRCAND);
                ::BitBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, SRCINVERT);
                //HBITMAP mask = Util::Image::ImageOp::GetMaskImage(maskFilename_, imageSize_);
                //::MaskBlt(lpDIS->hDC, 0, 0, imageSize_.width, imageSize_.height, commonButtonDc, 0, 0, mask, 0, 0, SRCCOPY);
            }
        }

#if 0
        m_cItemX.Create(_T("Install MSDE"), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, rcItem, this, 999);

        void CMyStatic::OnNcPaint() {
            CPaintDC dc(this);
            HDC hdc = dc.m_hDC;
            HBRUSH hBkgnd = ::CreateSolidBrush(m_clrBackGround);
            HBRUSH hbrOld = (HBRUSH)::SelectObject(hdc, hBkgnd);
            ::FillRect(hdc, &m_rcStatic, hBkgnd);
            ::DeleteObject(::SelectObject(hdc, hbrOld));
            ::ReleaseDC(m_hWnd, hdc);
            // Do not call CStatic::OnNcPaint() for painting messages
        }

        void CMyStatic::OnPaint() {
            CPaintDC dc(this); // device context for painting
            HDC hdc;
            CRect crcCaption;
            crcCaption.CopyRect(&m_rcStatic);
            DRAWTEXTPARAMS dtexParam;
            HFONT hFont, holdFont;
            COLORREF clrOld;
            hdc = dc.m_hDC;
            ASSERT(hdc != NULL);

            // TODO: Add your message handler code here
            if (m_bHotColor && m_bMouseMove) {
                hFont = m_cFontHot.operator HFONT();
                clrOld = (COLORREF)::SetTextColor(hdc, m_clrForeHot);
            } else {
                hFont = m_cFontNormal.operator HFONT();
                clrOld = (COLORREF)::SetTextColor(hdc, m_clrForeNormal);
            }

            holdFont = (HFONT)::SelectObject(hdc, hFont);
            dtexParam.cbSize = sizeof(DRAWTEXTPARAMS);
            dtexParam.uiLengthDrawn = m_strCaption.GetLength();

            if (!::DrawTextEx(hdc, (CHAR*)&m_strCaption, m_strCaption.GetLength(),
                              crcCaption, DT_CENTER | DT_VCENTER | DT_CALCRECT | DT_SINGLELINE,
                              &dtexParam)) {
                LPVOID lpMsgBuf;
                ::FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    ::GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &lpMsgBuf,
                    0,
                    NULL
                );
                ::MessageBeep(MB_ICONASTERISK);
                ::MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
                // Free the buffer.
                ::LocalFree(lpMsgBuf);
            }

            ::SetTextColor(hdc, clrOld);
            ::DeleteObject(::SelectObject(hdc, holdFont));
            // Do not call CStatic::OnPaint() for painting messages
        }

#endif
    }
}
