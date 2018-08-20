#ifndef __CONTROL_IMAGEBUTTON_H__
#define __CONTROL_IMAGEBUTTON_H__

#include <string>
#include <afxwin.h>

namespace Control {
    // ImageButton

    class ImageButton : public CButton {
        DECLARE_DYNAMIC(ImageButton)

    public:
        static SIZE const GetDefaultButtonSize() {
            SIZE result;
            //result.cx = 78;
            //result.cy = 40;
            result.cx = 95;
            result.cy = 52;
            return result;
        }
        ImageButton(std::wstring const& imageFilename = L"/FlashDrv/debug/button.jpg", SIZE const& imageSize = GetDefaultButtonSize());
        virtual ~ImageButton();
        //BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, RECT const& bounds, CWnd* pParentWnd, UINT nID);
        void SetText(std::wstring const& text, COLORREF const& textColor = RGB(255, 255, 255), size_t const fontHeight = 20);
        void SetImage(std::wstring const& imageFilename, SIZE const& imageSize);
        void SetImage(std::wstring const& imageFilename, SIZE const& imageSize, std::wstring const& maskFilename);
    private:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
        std::wstring imageFilename_;
        SIZE imageSize_;
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

#endif //__CONTROL_IMAGEBUTTON_H__
