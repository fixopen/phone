#include "Graphics.h"
#include "../Text/StringOp.h"

namespace Util {
    namespace Video {
        using Text::StringOp;
                        Size const Size::Parse(std::wstring const & content) {
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

        Size::operator SIZE() const {
            SIZE result;
            result.cx = width;
            result.cy = height;
            return result;
        }

        std::wstring const Size::ToString() const {
            return L"(" + StringOp::FromInt(width) + L"," + StringOp::FromInt(height) + L")";
        }

        Point const Point::Parse(std::wstring const & content) {
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

        Point::operator POINT() const {
            POINT result;
            result.x = x;
            result.y = y;
            return result;
        }

        std::wstring const Point::ToString() const {
            return L"(" + StringOp::FromInt(x) + L"," + StringOp::FromInt(y) + L")";
        }

        Point const Point::Move(Size const & size) const {
            return Point(x + size.width, y + size.height);
        }

        Size const Point::operator-(Point const & right) const {
            return Size(x - right.x, y - right.y);
        }

        Point const Point::operator-() const {
            return Point(-x, -y);
        }

        Rect const Rect::Parse(std::wstring const & content) {
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

        Rect::operator RECT() const {
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

        bool const CoordsOp::PointInRect(Point const & point, Rect const & bounds) {
            bool result = false;
            if (point.x >= bounds.origin.x && point.y >= bounds.origin.y && point.x < bounds.origin.x + bounds.size.width && point.y < bounds.origin.y + bounds.size.height) {
                result = true;
            }
            return result;
        }

        Point const CoordsOp::Move(Point const & newOrigin, Point const & point) {
            return point - newOrigin;
        }

        Point const CoordsOp::SymmetryPoint(Point const & point) {
            return -point;
        }

        void Graphics::DrawPoint(int const x, int const y, Color const & color, size_t const width) {
        }

        void Graphics::DrawLine(Point const & source, Point const & destination, LineStyle const & lineStyle) {
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

        void Graphics::FillRect(Rect const & region, Color const & color) {
        }

        void Graphics::DrawString(Rect const & region, std::wstring const & text, size_t const rowSpace, size_t const columnSpace, Font const & font, Color const & color) {
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

        void Graphics::DrawString(Point const & origin, std::wstring const & text, size_t const columnSpace, Font const & font, Color const & color) {
        }

        std::vector<std::wstring> const Graphics::SplitString(std::wstring const & text, size_t const width, size_t const columnSpace, Font const & font) {
            std::vector<std::wstring> result;
            return result;
        }

        void Graphics::DrawImage(Rect const & region, Image const & image) {
        }
    }
}
