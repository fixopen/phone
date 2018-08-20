#ifndef __UTIL_CONTROL_IMAGEBUTTON_H__
#define __UTIL_CONTROL_IMAGEBUTTON_H__

#include <string>
#include <afxwin.h> //for CButton
#include "../Util/Video/Graphics.h" //for Size, Color

namespace Util {
    namespace Control {
        // ImageButton

        class ImageButton : public CButton {
            DECLARE_DYNAMIC(ImageButton)

        public:
            static Util::Video::Size const GetDefaultButtonSize() {
                return Util::Video::Size(95, 52);
                //SIZE result;
                ////result.cx = 78;
                ////result.cy = 40;
                //result.cx = 95;
                //result.cy = 52;
                //return result;
            }
            ImageButton(std::wstring const& imageFilename = L"/FlashDrv/debug/button.jpg", Util::Video::Size const& imageSize = GetDefaultButtonSize());
            virtual ~ImageButton();
            //BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, RECT const& bounds, CWnd* pParentWnd, UINT nID);
            void SetText(std::wstring const& text, COLORREF const& textColor = RGB(255, 255, 255), size_t const fontHeight = 20);
            void SetImage(std::wstring const& imageFilename, Util::Video::Size const& imageSize);
            void SetImage(std::wstring const& imageFilename, Util::Video::Size const& imageSize, std::wstring const& maskFilename);
        private:
            virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
            std::wstring imageFilename_;
            Util::Video::Size imageSize_;
            std::wstring maskFilename_;
            std::wstring text_;
            COLORREF textColor_;
            size_t fontHeight_;
            CFont font_;
            //HFONT font_;
        protected:
            virtual void PreSubclassWindow();
            DECLARE_MESSAGE_MAP()
        };
    }
}

#endif //__UTIL_CONTROL_IMAGEBUTTON_H__
