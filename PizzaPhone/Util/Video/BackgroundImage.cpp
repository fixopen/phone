#include "BackgroundImage.h"
#include "ImageOp.h"

namespace Util {
    namespace Video {
        BOOL const BackgroundImage::drawBackground(HWND window, std::wstring const & imageFilename, Size const & imageSize) {
            HDC backgroundDc = ImageOp::GetImage(imageFilename, imageSize);
            HDC dc = ::GetDC(window);
            RECT bounds;
            ::GetWindowRect(window, &bounds);
            BOOL result = ::BitBlt(dc, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top, backgroundDc, 0, 0, SRCCOPY);
            ::ReleaseDC(window, dc);
            return result;
        }

        HBRUSH const BackgroundImage::setStaticProperties(HDC const dc, Rect const & bounds, HDC const backgroundDc, HFONT const font, COLORREF const textColor) {
            ::SetTextColor(dc, textColor);
            ::SetBkMode(dc, TRANSPARENT);
            setFont(dc, font);
            //::SelectFont(dc, font);
            BOOL r = ::BitBlt(dc, 0, 0, bounds.size.width, bounds.size.height, backgroundDc, bounds.origin.x, bounds.origin.y, SRCCOPY);
            if (!r) {
                DWORD errorCode = GetLastError();
                DWORD ec = errorCode;
            }
            return (HBRUSH)::GetStockObject(NULL_BRUSH); //HOLLOW_BRUSH
        }

        void BackgroundImage::setFont(HDC const dc, HFONT const & font) {
            ::SelectObject(dc, static_cast<HGDIOBJ>(font));
        }
    }
}
