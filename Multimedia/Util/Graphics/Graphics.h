#ifndef __UTIL_GRAPHICS_GRAPHICS_H__
#define __UTIL_GRAPHICS_GRAPHICS_H__

#include <WinDef.h> //for SIZE, POINT, RECT, COLORREF, HFONT, HPEN, HBRUSH

#include <string> //for std::wstring
#include <deque> //for std::deque
#include <vector> //for std::vector

#include "../Image/Image.h" //for Image::Image

namespace Util {
    namespace Graphics {
        struct Size {
            Size()
                : width(0)
                , height(0) {
            }
            Size(short const w, short const h)
                : width(w)
                , height(h) {
            }
            Size(SIZE const& sz)
                : width(static_cast<short>(sz.cx))
                , height(static_cast<short>(sz.cy)) {
            }
            static Size const Parse(std::wstring const& v);
            operator SIZE const() const;
            std::wstring const ToString() const;
            short width;
            short height;
        };

        struct Point {
            Point()
                : x(0)
                , y(0) {
            }
            Point(short const ax, short const ay)
                : x(ax)
                , y(ay) {
            }
            Point(Size const& size)
                : x(size.width)
                , y(size.height) {
            }
            Point(POINT const& pt)
                : x(static_cast<short>(pt.x))
                , y(static_cast<short>(pt.y)) {
            }
            static Point const Parse(std::wstring const& v);
            operator POINT const() const;
            std::wstring const ToString() const;
            Point const Move(Size const& size) const;
            Size const operator-(Point const& right) const;
            Point const operator-() const;
            short x;
            short y;
        };

        struct Rect {
            Rect()
                : origin(Point())
                , size(Size()) {
            }
            Rect(Point const& org, Size const& sz)
                : origin(org)
                , size(sz) {
            }
            Rect(short const x, short const y, short const w, short const h)
                : origin(Point(x, y))
                , size(Size(w, h)) {
            }
            Rect(RECT const& rc)
                : origin(Point(static_cast<short>(rc.left), static_cast<short>(rc.top)))
                , size(Size(static_cast<short>(rc.right - rc.left), static_cast<short>(rc.bottom - rc.top))) {
            }
            static Rect const Parse(std::wstring const& v);
            operator RECT const() const;
            std::wstring const ToString() const;
            Point origin;
            Size size;
        };

        class Color {
        public:
            Color();
            Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
            Color(COLORREF const cr);
            operator COLORREF const() const;
            static Color const Parse(std::wstring const& v);
            std::wstring const ToString();
        private:
            COLORREF value_;
        };

        class Font {
        public:
            Font();
            Font(std::wstring const& fontFacename, size_t const point, std::wstring const& fontFilename = L"");
            Font(Font const& right);
            ~Font();
            operator HFONT const() const {
                return handle_;
            }
            size_t const Height() const {
                return point_;
            }
            //Load&Unload
            //Enumeration Family
            //Enumeration Face
        private:
            HFONT handle_;
            size_t point_;
            std::wstring fontFilename_;
            int volatile refCount_;
        };

        class Brush {
        public:
            Brush(Color const color);
            ~Brush();
            operator HBRUSH const() const {
                return handle_;
            }
        private:
            HBRUSH handle_;
        };

        class Pen {
        public:
            Pen(DWORD const style, size_t const width, Color const color);
            ~Pen();
            operator HPEN const() const {
                return handle_;
            }
        private:
            HPEN handle_;
        };

        struct LineStyle {
            enum Style {
                sSolid,
                sDash,
                sDot,
                sDashDot,
                sDashDotDot,
                sNull,
                sInsideFrame,
                sUserStyle,
                sAlternate,
                sMask = 0xF,
            };
            enum EndpointStyle {
                esRound,
                esSquare = 0x100,
                esFlat = 0x200,
                esMask = 0xF00,
            };
            enum JoinStyle {
                jsRound,
                jsBevel = 0x1000,
                jsMiter = 0x2000,
                jsMask = 0xF000,
            };
            enum TypeStyle {
                tsCosmetic,
                tsGeometric = 0x10000,
                tsMask = 0xF0000,
            };
            LineStyle()
                : color(Color())
                , width(0)
                , style(sSolid)
                , endpointStyle(esRound) {
            }
            Color color;
            size_t width;
            Style style;
            EndpointStyle endpointStyle;
        };

        struct TextAttributes {
            TextAttributes()
                : rowSpace(0)
                , columnSpace(0)
                , horizontalAlign(haCenter)
                , verticalAlign(vaCenter)
                , ellipsis(eYes)
                , prefix(pYes) {
            }
            size_t rowSpace;
            size_t columnSpace;
            Font font;
            Color color;
            enum HorizontalAlignment {
                haLeft,
                haCenter,
                haRight,
            };
            HorizontalAlignment horizontalAlign;
            enum VerticalAlignment {
                vaTop,
                vaCenter = 0x4,
                vaBottom = 0x8,
            };
            VerticalAlignment verticalAlign;
            enum Ellipsis {
                eNo,
                eMaybe,
                eYes,
            };
            Ellipsis ellipsis;
            enum Prefix {
                pNo,
                pMaybe,
                pYes,
            };
            Prefix prefix;
        };

        class CoordsOp {
        public:
            static bool const PointInRect(Point const& point, Rect const& rect);
            static Point const Move(Point const& newOrigin, Point const& point);
            static Point const SymmetryPoint(Point const& point);
        };

        class Context {
        public:
            Context(HDC const dc) : handle_(dc) {}
            void DrawPoint(int const x, int const y, Color const& color, size_t const width) const;
            void DrawLine(Point const& source, Point const& destination, LineStyle const& lineStyle) const;
            void FillRectangle(Rect const& region, Color const& color) const;
            void DrawString(Rect const& region, std::wstring const& text, TextAttributes const& textAttr) const;
            void DrawString(Rect const& region, std::wstring const& text, size_t const rowSpace, size_t const columnSpace, Font const& font, Color const& color, UINT const format) const;
            void DrawString(Point const& origin, std::wstring const& text, size_t const columnSpace, Font const& font, Color const& color) const;
            std::vector<std::wstring> const SplitString(std::wstring const& text, size_t const width, size_t const columnSpace, Font const& font) const;
            void DrawImage(Rect const& region, Image::Image const& image) const;
        private:
            Context(Context const& right);
        private:
            HDC handle_;
        };
    }
}

#endif //__UTIL_GRAPHICS_GRAPHICS_H__
