#include "Creator.h"

#include "../Util/StringOp.h"
#include "../Util/ImageOperator.h"

namespace UIEngineer {
    namespace Logical {
        //CWnd* const Creator::Create(CWnd* const parent, Xml::Element const& element) {
        //    CWnd* control = 0;
        //    CRect bounds = Util::StringOp::ToRectangle(element[L"@bounds"].value());
        //    if (element.name() == L"button") {
        //        control = new CButton();
        //        (dynamic_cast<CButton*>(control))->Create(element[L"@text"].value().c_str(),
        //            WS_CHILD | WS_VISIBLE,
        //            bounds,
        //            parent,
        //            Util::StringOp::ToInt(element[L"@id"].value()));
        //    } else if (element.name() == L"edit") {
        //        control = new CEdit();
        //    } else if (element.name() == L"label") {
        //        control = new CStatic();
        //    }

        //    CDC* dc = control->GetDC();
        //    std::wstring imageFilename;
        //    std::wstring newImageFilename = element[L"background"][L"@image"].value();
        //    if (newImageFilename != L".") {
        //        imageFilename = newImageFilename;
        //    }
        //    std::wstring boundsString = element[L"background"][L"@bounds"].value();
        //    if (boundsString != L".") {
        //        bounds = Util::StringOp::ToRectangle(boundsString);
        //    }
        //    Util::ImageOperator::DrawImage(dc->m_hDC, imageFilename.c_str(), bounds);

        //    CFont font;
        //    font.CreateFont(
        //        Util::StringOp::ToInt(element[L"font"][L"@size"].value()),      // nHeight
        //        0,                         // nWidth
        //        0,                         // nEscapement
        //        0,                         // nOrientation
        //        Util::StringOp::ToInt(element[L"font"][L"@weight"].value()),    // FW_NORMAL
        //        Util::StringOp::ToInt(element[L"font"][L"@italic"].value()),    // bItalic
        //        Util::StringOp::ToInt(element[L"font"][L"@underline"].value()), // bUnderline
        //        0,                         // cStrikeOut
        //        DEFAULT_CHARSET,           // DEFAULT_CHARSET,
        //        OUT_DEFAULT_PRECIS,        // nOutPrecision
        //        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        //        DEFAULT_QUALITY,           // nQuality
        //        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        //        element[L"font"][L"@name"].value().c_str());     // lpszFacename

        //    CFont* pFont = dc->SelectObject(&font);
        //    dc->SetTextColor(Util::StringOp::ToColor(element[L"@color"].value()));
        //    dc->DrawText(element[L"@text"].value().c_str(), element[L"@text"].value().length(), bounds, DT_NOCLIP | DT_WORDBREAK);
        //    dc->SelectObject(pFont);
        //    font.DeleteObject();

        //    return control;
        //}
    }
}