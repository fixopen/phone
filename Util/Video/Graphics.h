#ifndef __UTIL_VIDEO_GRAPHICS_H__
#define __UTIL_VIDEO_GRAPHICS_H__

#include <string>
#include <deque>
#include <vector>

namespace Util {
    namespace Video {
        struct Point {
            Point() {
            }
            Point(short const ax, short const ay)
            : x(ax)
            , y(ay) {
            }
            static Point const Parse(std::wstring const& v);
            std::wstring const ToString();
            short x;
            short y;
        };

        struct Size {
            Size() {
            }
            Size(short const w, short const h)
                : width(w)
                , height(h) {
            }
            static Size const Parse(std::wstring const& v);
            std::wstring const ToString();
            short width;
            short height;
        };

        struct Rect {
            Rect() {
            }
            Rect(Point const& aorigin, Size const& asize)
                : origin(aorigin)
                , size(asize) {
            }
            Rect(short const x, short const y, short const w, short const h)
                : origin(Point(x, y))
                , size(Size(w, h)) {
            }
            static Rect const Parse(std::wstring const& v);
            std::wstring const ToString();
            Point origin;
            Size size;
        };

        class Color {
        public:
            union WinColorRef {
                struct RGBColorRef {
                    unsigned char red;
                    unsigned char green;
                    unsigned char blue;
                    unsigned char reserved;
                };

                RGBColorRef rgbParts;
                unsigned int whole;
            };
            static Color const Parse(std::wstring const& v);
            std::wstring const ToString();
        };

        class Image {
        };

        class Bitmap : public Image {
        };

        class Jpeg : public Image {
        };

        class Font {
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
        public:
            size_t const Height() const {
                size_t result = 0;
                return result;
            }
        };

        struct LineStyle {
            enum Style {
                sSolid,
                sDot,
                sDash,
                sDashDot,
                sDashDotDot,
            };
            enum EndpointStyle {
                esCircle,
                esQuadrate,
            };
            Color color;
            size_t width;
            Style style;
            EndpointStyle endpointStyle;
        };

        class CoordsOp {
        public:
            static bool const PointInRect(Point const& point, Rect const& rect);
            static Point const Move(Point const& newOrigin, Point const& point);
            static Point const SymmetryPoint(Point const& point);
        };

        class Graphics {
        public:
            static void DrawPoint(int const x, int const y, Color const& color, size_t const width);
            static void DrawLine(Point const& source, Point const& destination, LineStyle const& lineStyle);
            static void FillRect(Rect const& region, Color const& color);
            static void DrawString(Rect const& region, std::wstring const& text, size_t const rowSpace, size_t const columnSpace, Font const& font, Color const& color);
            static void DrawString(Point const& origin, std::wstring const& text, size_t const columnSpace, Font const& font, Color const& color);
            static std::vector<std::wstring> const SplitString(std::wstring const& text, size_t const width, size_t const columnSpace, Font const& font);
            static void DrawImage(Rect const& region, Image const& image);
        };
    }
}

#endif //__UTIL_VIDEO_GRAPHICS_H__
