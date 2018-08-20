#include "Graphics.h"
#include "../Text/StringOp.h"
//#include <WinGDI.h> //for GDI
//#include <WinUser.h> //for ::FillRect
#include <Windows.h>

namespace Util {
    namespace Graphics {
        using Text::StringOp;
        Size const Size::Parse(std::wstring const& content) {
            Size result;
            size_t firstB = content.find(L'(', 0);

            if (firstB == std::wstring::npos) {
                return result;
            } else {
                size_t firstComma = content.find(L',', firstB + 1);

                if (firstComma == std::wstring::npos) {
                    return result;
                } else {
                    std::wstring x = content.substr(firstB + 1, firstComma - (firstB + 1));
                    result.width = StringOp::ToInt(x);
                    size_t firstE = content.find(L')', firstComma + 1);

                    if (firstE == std::wstring::npos) {
                        return result;
                    } else {
                        std::wstring y = content.substr(firstComma + 1, firstE - (firstComma + 1));
                        result.height = StringOp::ToInt(y);
                    }
                }
            }

            return result;
        }

        Size::operator SIZE const() const {
            SIZE result;
            result.cx = width;
            result.cy = height;
            return result;
        }

        std::wstring const Size::ToString() const {
            return L"(" + StringOp::FromInt(width) + L"," + StringOp::FromInt(height) + L")";
        }

        Point const Point::Parse(std::wstring const& content) {
            Point result;
            size_t firstB = content.find(L'(', 0);

            if (firstB == std::wstring::npos) {
                return result;
            } else {
                size_t firstComma = content.find(L',', firstB + 1);

                if (firstComma == std::wstring::npos) {
                    return result;
                } else {
                    std::wstring x = content.substr(firstB + 1, firstComma - (firstB + 1));
                    result.x = StringOp::ToInt(x);
                    size_t firstE = content.find(L')', firstComma + 1);

                    if (firstE == std::wstring::npos) {
                        return result;
                    } else {
                        std::wstring y = content.substr(firstComma + 1, firstE - (firstComma + 1));
                        result.y = StringOp::ToInt(y);
                    }
                }
            }

            return result;
        }

        Point::operator POINT const() const {
            POINT result;
            result.x = x;
            result.y = y;
            return result;
        }

        std::wstring const Point::ToString() const {
            return L"(" + StringOp::FromInt(x) + L"," + StringOp::FromInt(y) + L")";
        }

        Point const Point::Move(Size const& size) const {
            return Point(x + size.width, y + size.height);
        }

        Size const Point::operator-(Point const& right) const {
            return Size(x - right.x, y - right.y);
        }

        Point const Point::operator-() const {
            return Point(-x, -y);
        }

        Rect const Rect::Parse(std::wstring const& content) {
            Rect result;
            size_t firstB = content.find(L'(', 0);

            if (firstB == std::wstring::npos) {
                return result;
            } else {
                size_t firstComma = content.find(L',', firstB + 1);

                if (firstComma == std::wstring::npos) {
                    return result;
                } else {
                    std::wstring x0 = content.substr(firstB + 1, firstComma - (firstB + 1));
                    result.origin.x = StringOp::ToInt(x0);
                    size_t firstE = content.find(L')', firstComma + 1);

                    if (firstE == std::wstring::npos) {
                        return result;
                    } else {
                        std::wstring y0 = content.substr(firstComma + 1, firstE - (firstComma + 1));
                        result.origin.y = StringOp::ToInt(y0);
                        size_t secondB = content.find(L'(', firstE + 1);

                        if (secondB == std::wstring::npos) {
                            return result;
                        } else {
                            Point rightBottomPos;
                            size_t secondComma = content.find(L',', secondB);

                            if (secondComma == std::wstring::npos) {
                                return result;
                            } else {
                                std::wstring x1 = content.substr(secondB + 1, secondComma - (secondB + 1));
                                rightBottomPos.x = StringOp::ToInt(x1);
                                //result.size.width = StringOp::ToInt(x1);
                                size_t secondE = content.find(L')', secondComma + 1);

                                if (secondE == std::wstring::npos) {
                                    return result;
                                } else {
                                    std::wstring y1 = content.substr(secondComma + 1, secondE - (secondComma + 1));
                                    rightBottomPos.y = StringOp::ToInt(y1);
                                    //result.size.height = StringOp::ToInt(y1);
                                }
                            }

                            result.size = rightBottomPos - result.origin;
                        }
                    }
                }
            }

            return result;
        }

        Rect::operator RECT const() const {
            RECT result;
            result.left = origin.x;
            result.top = origin.y;
            result.right = origin.x + size.width;
            result.bottom = origin.y + size.height;
            return result;
        }

        std::wstring const Rect::ToString() const {
            return L"[" + origin.ToString() + origin.Move(size).ToString() + L"]";
        }

        bool const CoordsOp::PointInRect(Point const& point, Rect const& bounds) {
            bool result = false;

            if (point.x >= bounds.origin.x
                    && point.y >= bounds.origin.y
                    && point.x < bounds.origin.x + bounds.size.width
                    && point.y < bounds.origin.y + bounds.size.height) {
                result = true;
            }

            return result;
        }

        Color::Color() : value_(RGB(0, 0, 0)) {
        }

        Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : value_(RGB(red, green, blue)) {
        }

        Color::Color(COLORREF const cr) : value_(cr) {
        }

        Color::operator COLORREF const() const {
            //COLORREF result;
            //return result;
            return value_;
        }

        Color const Color::Parse(std::wstring const& v) {
            Color result;
            return result;
        }

        std::wstring const Color::ToString() {
            std::wstring result;
            return result;
        }

        Font::Font()
            : handle_(0)
            , point_(0) {
        }

        Font::Font(std::wstring const& fontFacename, size_t const point, std::wstring const& fontFilename)
            : handle_(0)
            , point_(point)
            , fontFilename_(fontFilename)
            , refCount_(1) {
            if (fontFilename_ != L"") {
                ::AddFontResource(fontFilename_.c_str());
            }

            handle_ = ::CreateFont(point, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, fontFacename.c_str());
            //HFONT CreateFont(
            //  __in  int nHeight,
            //  __in  int nWidth,
            //  __in  int nEscapement,
            //  __in  int nOrientation,
            //  __in  int fnWeight,
            //  __in  DWORD fdwItalic,
            //  __in  DWORD fdwUnderline,
            //  __in  DWORD fdwStrikeOut,
            //  __in  DWORD fdwCharSet,
            //  __in  DWORD fdwOutputPrecision,
            //  __in  DWORD fdwClipPrecision,
            //  __in  DWORD fdwQuality,
            //  __in  DWORD fdwPitchAndFamily,
            //  __in  LPCTSTR lpszFace
            //);
            OUTLINETEXTMETRIC outlineTextMetric;
            memset(&outlineTextMetric, 0, sizeof outlineTextMetric);
            //::GetOutlineTextMetrics(handle_, cjCopy, &outlineTextMetric);
            //typeface, style, size
            //style: weight, slant
            //weight: Thin, Extralight, Light, Normal, Medium, Semibold, Bold, Extrabold, Heavy
            //slant: roman, oblique, and italic
            //.013837 inch [1/72 inch] is point
            //font family
            /*
            Font family Description
            Decorative Specifies a novelty font. An example is Old English.
            Dontcare Specifies a generic family name. This name is used when information about a font does not exist or does not matter. The default font is used.
            Modern Specifies a monospace font with or without serifs. Monospace fonts are usually modern; examples include Pica, Elite, and Courier New.
            Roman Specifies a proportional font with serifs. An example is Times New Roman.
            Script Specifies a font that is designed to look like handwriting; examples include Script and Cursive.
            Swiss Specifies a proportional font without serifs. An example is Arial.
            FF_DECORATIVE, FF_DONTCARE, FF_MODERN, FF_ROMAN, FF_SCRIPT, and FF_SWISS
            */
            //telenologies: Raster, Vector, TrueType, Microsoft OpenType == TrueType + PostScript
            //charset: Windows, Unicode, OEM (original equipment manufacturer), Symbol, Vendor-specific
            //EnumFontFamilies, ChooseFont, AddFontResource, AddFontResourceEx, CreateScalableFontResource, RemoveFontResource, RemoveFontResourceEx, WM_FONTCHANGE, AddFontMemResourceEx, RemoveFontMemResourceEx
            //LOGFONT, CreateFont, CreateFontIndirect, CreateFontIndirectEx, SelectObject, SetMapperFlags, EnumFonts, EnumFontFamilies, EnumFontFamiliesEx
            //GetOutlineTextMetrics, OUTLINETEXTMETRIC, GetFontData, TTEmbedFont, TTEmbedFontEx
            //format:
            //The formatting functions can be divided into three categories:
            //those that retrieve or set the text-formatting attributes for a device context,
            //SetBkColor, SetBkMode, SetTextAlign, SetTextCharacterExtra, SetTextColor, and SetTextJustification
            //GetBkColor, GetBkMode, GetTextAlign, GetTextCharacterExtra, GetTextColor, and GetTextExtentPoint32
            //those that retrieve character widths,
            //GetCharWidth32 and GetCharWidthFloat, GetCharABCWidths and GetCharABCWidthsFloat
            //and those that retrieve string widths and heights.
            //GetTextExtentPoint32, and GetTabbedTextExtent,  GetTextExtentExPoint
            //GetTextMetrics, TEXTMETRIC, OUTLINETEXTMETRIC, NEWTEXTMETRIC, GetDeviceCaps
            //draw:
            //DrawText, DrawTextEx, ExtTextOut, PolyTextOut, TabbedTextOut, TextOut
        }

        Font::Font(Font const& right)
            : handle_(right.handle_)
            , point_(right.point_)
            , fontFilename_(right.fontFilename_)
            , refCount_(right.refCount_ + 1) {
        }

        Font::~Font() {
            --refCount_;

            if (!refCount_) {
                ::DeleteObject(handle_);

                if (fontFilename_ != L"") {
                    ::RemoveFontResource(fontFilename_.c_str());
                }
            }
        }

        Brush::Brush(Color const color) {
            handle_ = ::CreateSolidBrush(color);
        }

        Brush::~Brush() {
            ::DeleteObject(handle_);
        }

        Pen::Pen(DWORD const style, size_t const width, Color const color) {
            handle_ = ::CreatePen(style, width, color);
        }

        Pen::~Pen() {
            ::DeleteObject(handle_);
        }

        Point const CoordsOp::Move(Point const& newOrigin, Point const& point) {
            return point - newOrigin;
        }

        Point const CoordsOp::SymmetryPoint(Point const& point) {
            return -point;
        }

        void Context::DrawPoint(int const x, int const y, Color const& color, size_t const width) const {
            int radius = width / 2;
            int left = x - radius;
            int top = y - radius;
            int right = x + radius;
            int bottom = y - radius;
            HGDIOBJ old = ::SelectObject(handle_, Brush(color));
            ::Ellipse(handle_, left, top, right, bottom);
            ::SelectObject(handle_, old);
        }

        void Context::DrawLine(Point const& source, Point const& destination, LineStyle const& lineStyle) const {
            HGDIOBJ old = ::SelectObject(handle_, Pen(lineStyle.style | lineStyle.endpointStyle, lineStyle.width, lineStyle.color));
            ::MoveToEx(handle_, source.x, source.y, 0);
            ::LineTo(handle_, destination.x, destination.y);
            ::SelectObject(handle_, old);
        }

        void Context::FillRectangle(Rect const& region, Color const& color) const {
            ::RECT r = region;
            ::FillRect(handle_, &r, Brush(color));
        }

        void Context::DrawString(Rect const& region, std::wstring const& text, TextAttributes const& textAttr) const {
            DrawString(region, text, textAttr.rowSpace, textAttr.columnSpace, textAttr.font, textAttr.color, textAttr.horizontalAlign | textAttr.verticalAlign);
        }

        void Context::DrawString(Rect const& region, std::wstring const& text, size_t const rowSpace, size_t const columnSpace, Font const& font, Color const& color, UINT const format) const {
            int oldBackMode = ::SetBkMode(handle_, TRANSPARENT);
            Color oldColor = ::SetTextColor(handle_, color);
            //HGDIOBJ old = ::SelectObject(handle_, font);
            RECT r = region;
            ::DrawTextW(handle_, text.c_str(), text.length(), &r, format);
            //if (!text.empty()) {
            //    std::vector<std::wstring> lines;
            //    std::vector<std::wstring> paragraphs = Util::Text::StringOp::Split(text, L"\r\n"); //or L"\r" or L"\n"
            //    for (std::vector<std::wstring>::iterator i = paragraphs.begin(); i != paragraphs.end(); ++i) {
            //        std::vector<std::wstring> pl = SplitString(*i, region.size.width, columnSpace, font);
            //        lines.insert(lines.end(), pl.begin(), pl.end());
            //    }
            //    Point origin = region.origin;
            //    for (std::vector<std::wstring>::iterator i = lines.begin(); i != lines.end(); ++i) {
            //        DrawString(origin, *i, columnSpace, font, color);
            //        origin.y += font.Height() + rowSpace;
            //    }
            //}
            //::SelectObject(handle_, old);
            ::SetTextColor(handle_, oldColor);
            ::SetBkMode(handle_, oldBackMode);
        }

        void Context::DrawString(Point const& origin, std::wstring const& text, size_t const columnSpace, Font const& font, Color const& color) const {
        }

        std::vector<std::wstring> const Context::SplitString(std::wstring const& text, size_t const width, size_t const columnSpace, Font const& font) const {
            std::vector<std::wstring> result;
            return result;
        }

        void Context::DrawImage(Rect const& region, Image::Image const& image) const {
        }
    }
}
