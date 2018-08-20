#include "Graphics.h"
#include "../Text/StringOp.h"

namespace Util {
    namespace Video {
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

        Point const CoordsOp::Move(Point const& newOrigin, Point const& point) {
            Point result;
            result.x = point.x - newOrigin.x;
            result.y = point.y - newOrigin.y;
            return result;
        }

        Point const CoordsOp::SymmetryPoint(Point const& point) {
            Point result;
            result.x = -point.x;
            result.y = -point.y;
            return result;
        }

        void Graphics::DrawPoint(int const x, int const y, Color const& color, size_t const width) {
        }

        void Graphics::DrawLine(Point const& source, Point const& destination, LineStyle const& lineStyle) {
            if (lineStyle.width > 0) {
                switch (lineStyle.style) {
                case LineStyle::sSolid:
                    break;
                case LineStyle::sDash:
                    break;
                case LineStyle::sDashDot:
                    break;
                case LineStyle::sDashDotDot:
                    break;
                case LineStyle::sDot:
                    break;
                default:
                    break;
                }
            }
        }

        void Graphics::FillRect(Rect const& region, Color const& color) {
        }

        void Graphics::DrawString(Rect const& region, std::wstring const& text, size_t const rowSpace, size_t const columnSpace, Font const& font, Color const& color) {
            if (!text.empty()) {
                std::vector<std::wstring> lines;
                std::vector<std::wstring> paragraphs = Util::Text::StringOp::Split(text, L"\r\n"); //or L"\r" or L"\n"
                for (std::vector<std::wstring>::iterator i = paragraphs.begin(); i != paragraphs.end(); ++i) {
                    std::vector<std::wstring> pl = SplitString(*i, region.size.width, columnSpace, font);
                    lines.insert(lines.end(), pl.begin(), pl.end());
                }
                Point origin = region.origin;
                for (std::vector<std::wstring>::iterator i = lines.begin(); i != lines.end(); ++i) {
                    DrawString(origin, *i, columnSpace, font, color);
                    origin.y += font.Height() + rowSpace;
                }
            }
        }

        void Graphics::DrawString(Point const& origin, std::wstring const& text, size_t const columnSpace, Font const& font, Color const& color) {
        }

        std::vector<std::wstring> const Graphics::SplitString(std::wstring const& text, size_t const width, size_t const columnSpace, Font const& font) {
            std::vector<std::wstring> result;
            return result;
        }

        void Graphics::DrawImage(Rect const& region, Image const& image) {
        }
    }
}
