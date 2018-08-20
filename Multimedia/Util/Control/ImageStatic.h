#ifndef __UTIL_CONTROL_IMAGESTATIC_H__
#define __UTIL_CONTROL_IMAGESTATIC_H__

#pragma once

#include <WinDef.h> //for COLORREF
#include <WinGDI.h> //for RGB
#include <WinUser.h> //for LPDRAWITEMSTRUCT
#include <AfxWin.h> //for CFont
#include <string> //for std::wstring
#include "../Graphics/Graphics.h" //for Graphics::Size

namespace Util {
    namespace Control {
        // ImageStatic

        class ImageStatic : public CStatic {
            DECLARE_DYNAMIC(ImageStatic)

        public:
            ImageStatic();
            virtual ~ImageStatic();
            void SetText(std::wstring const& text, COLORREF const& textColor = RGB(255, 255, 255), size_t const fontHeight = 20);
            void SetImage(std::wstring const& imageFilename, Size const& imageSize);
            void SetImage(std::wstring const& imageFilename, Size const& imageSize, std::wstring const& maskFilename);
        private:
            virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
            std::wstring imageFilename_;
            Size imageSize_;
            std::wstring maskFilename_;
            std::wstring text_;
            COLORREF textColor_;
            size_t fontHeight_;
            CFont font_;
            //HFONT font_;
        protected:
            afx_msg BOOL OnEraseBkgnd(CDC* pDC);
            afx_msg void OnPaint();
            //LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            virtual void PreSubclassWindow();
        protected:
            DECLARE_MESSAGE_MAP()
        };
    }
}

#endif //__UTIL_CONTROL_IMAGESTATIC_H__
