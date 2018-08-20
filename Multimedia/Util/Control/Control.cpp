#include "Control.h"
#include "../Graphics/Graphics.h" //for CoordsOp

#include <windows.h>
//#include <WinDef.h>
//#include <WinBase.h>
//#include <WinUser.h>

namespace Util {
    namespace Control {
        Control::Control(ContainerControl* const parent, Graphics::Point const& position, Graphics::Size const& size)
            : parent_(parent)
            , position_(position)
            , size_(size) {
            if (parent_) {
                textAttributes_ = parent_->textAttributes_;
                parent_->addChild_(this);
            }
        }

        Control::~Control() {
        }

        Graphics::Rect const Control::GetWorldBounds() const {
            Graphics::Point origin = position_;
            Control* p = parent_;

            while (p) {
                origin = Graphics::CoordsOp::Move(Graphics::CoordsOp::SymmetryPoint(p->position_), origin);
                p = p->parent_;
            }

            return Graphics::Rect(origin, size_);
        }

        void Control::drawBackground_(Graphics::Context const& g) {
            switch (background_.mode) {
                case Background::mOpaque:

                    switch (background_.type) {
                        case Background::tColor:
                            g.FillRectangle(GetWorldBounds(), background_.color);
                            break;
                        case Background::tImage:
                            g.DrawImage(GetWorldBounds(), *background_.image);
                            break;
                        default:
                            break;
                    }

                    break;
                case Background::mTransparent:
                    //do nothing
                    break;
                default:
                    break;
            }
        }

        void Control::drawBorder_(Graphics::Context const& g) {
            g.DrawLine(position_, Graphics::Point(position_.x, position_.y + size_.height), border_[0]);
            g.DrawLine(position_, Graphics::Point(position_.x + size_.width, position_.y), border_[1]);
            g.DrawLine(Graphics::Point(position_.x + size_.width, position_.y), Graphics::Point(position_.x + size_.width, position_.y + size_.height), border_[2]);
            g.DrawLine(Graphics::Point(position_.x, position_.y + size_.height), Graphics::Point(position_.x + size_.width, position_.y + size_.height), border_[3]);
        }

        void Control::drawContent_(Graphics::Context const& g) {
            g.DrawString(GetBounds(), text_, textAttributes_);
        }

        ScrollableControl::ScrollableControl(ContainerControl* const parent, Graphics::Point const& position, Graphics::Size const& size)
            : Control(parent, position, size)
            , reallyBounds_(GetBounds())
            , viewportOrigin_(Graphics::Point(0, 0)) {
            Scroll = MessageHandler(this, static_cast<MessageHandler::Handler>(&ScrollableControl::onScroll_));
        }

        ScrollableControl::~ScrollableControl() {
        }

        void ScrollableControl::SetReallyBounds(Graphics::Rect const& reallyBounds, Graphics::Point const& viewportOrigin) {
            reallyBounds_ = reallyBounds;
            viewportOrigin_ = viewportOrigin;
        }

        void ScrollableControl::drawContent_() {
            //step1: draw to virtual surface
            //Graphics::Context::DrawString(GetBounds(), text_, font_, textColor_);
            //step2: copy to control surface
            //Graphics::Context::CopyImage(...);
        }

        void ScrollableControl::onScroll_(Component const* const sender, MessageArgs const* const args) {
            //ScrollMessageArgs* const newArgs = const_cast<ScrollMessageArgs* const>(dynamic_cast<ScrollMessageArgs const* const>(args));
            ScrollMessageArgs* const newArgs = const_cast<ScrollMessageArgs * const>(static_cast<ScrollMessageArgs const * const>(args));
            viewportOrigin_.x += newArgs->cx;
            viewportOrigin_.y += newArgs->cy;
        }

        ContainerControl::ContainerControl(HWND const self)
            : ScrollableControl(0, Graphics::Point(), Graphics::Size())
            , handle_(self) {
            ::RECT bounds;
            ::GetClientRect(handle_, &bounds);
            SetBounds(bounds);
            initDelegates_();
        }

        ContainerControl::ContainerControl(ContainerControl* const parent, Graphics::Point const& position, Graphics::Size const& size)
            : ScrollableControl(parent, position, size)
            , currentChild_(0) {
            initDelegates_();
        }

        ContainerControl::~ContainerControl() {
            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                delete *i;
            }
        }

        void ContainerControl::initDelegates_() {
            PointDown = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onPointDown_));
            PointUp = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onPointUp_));
            PointMove = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onPointMove_));
            Click = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onClick_));
            KeyDown = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onKeyDown_));
            KeyUp = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onKeyUp_));
            Key = MessageHandler(this, static_cast<MessageHandler::Handler>(&ContainerControl::onKey_));
        }

        void ContainerControl::addChild_(Control const* const child) {
            children_.push_back(const_cast<Control*>(child));
        }

        void ContainerControl::removeChild_(Control const* const child) {
            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                if (*i == child) {
                    delete *i;
                    children_.erase(i);
                    break;
                }
            }
        }

        void ContainerControl::drawContent_(Graphics::Context const& g) {
            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                (*i)->DrawSelf(g);
            }

            g.DrawString(GetBounds(), text_, textAttributes_);
        }

        void ContainerControl::onPointDown_(Component const* const sender, MessageArgs const* const args) {
            PointMessageArgs* const newArgs = const_cast<PointMessageArgs * const>(static_cast<PointMessageArgs const * const>(args));

            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                if (Graphics::CoordsOp::PointInRect(Graphics::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                    Graphics::Point p = Graphics::CoordsOp::Move((*i)->GetPosition(), Graphics::Point(newArgs->x, newArgs->y));
                    newArgs->x = p.x;
                    newArgs->y = p.y;
                    (*i)->PointDown(sender, newArgs);
                }
            }
        }

        void ContainerControl::onPointUp_(Component const* const sender, MessageArgs const* const args) {
            //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
            PointMessageArgs* const newArgs = const_cast<PointMessageArgs * const>(static_cast<PointMessageArgs const * const>(args));

            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                if (Graphics::CoordsOp::PointInRect(Graphics::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                    Graphics::Point p = Graphics::CoordsOp::Move((*i)->GetPosition(), Graphics::Point(newArgs->x, newArgs->y));
                    newArgs->x = p.x;
                    newArgs->y = p.y;
                    (*i)->PointUp(sender, newArgs);
                }
            }
        }

        void ContainerControl::onPointMove_(Component const* const sender, MessageArgs const* const args) {
            //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
            PointMessageArgs* const newArgs = const_cast<PointMessageArgs * const>(static_cast<PointMessageArgs const * const>(args));

            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                if (Graphics::CoordsOp::PointInRect(Graphics::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                    Graphics::Point p = Graphics::CoordsOp::Move((*i)->GetPosition(), Graphics::Point(newArgs->x, newArgs->y));
                    newArgs->x = p.x;
                    newArgs->y = p.y;
                    (*i)->PointMove(sender, newArgs);
                }
            }
        }

        void ContainerControl::onClick_(Component const* const sender, MessageArgs const* const args) {
            //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
            PointMessageArgs* const newArgs = const_cast<PointMessageArgs * const>(static_cast<PointMessageArgs const * const>(args));

            for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
                if (Graphics::CoordsOp::PointInRect(Graphics::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                    Graphics::Point p = Graphics::CoordsOp::Move((*i)->GetPosition(), Graphics::Point(newArgs->x, newArgs->y));
                    newArgs->x = p.x;
                    newArgs->y = p.y;
                    (*i)->Click(sender, newArgs);
                }
            }
        }

        void ContainerControl::onKeyDown_(Component const* const sender, MessageArgs const* const args) {
            //currentChild_->KeyDown();
        }

        void ContainerControl::onKeyUp_(Component const* const sender, MessageArgs const* const args) {
        }

        void ContainerControl::onKey_(Component const* const sender, MessageArgs const* const args) {
        }

        //namespace {
        //    std::wstring const defaultMainFormClassName = L"MainForm";
        //    LRESULT CALLBACK windowMessageProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
        //        LRESULT result = 0;
        //        result = ::DefWindowProc(window, message, wParam, lParam);
        //        return result;
        //    }
        //    void RegisterFormClass() {
        //        WNDCLASSEX windowClass;
        //        windowClass.cbSize = sizeof windowClass;
        //        windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS | CS_NOCLOSE;
        //        windowClass.lpfnWndProc = windowMessageProc;
        //        windowClass.cbClsExtra = 0;
        //        windowClass.cbWndExtra = 0;
        //        windowClass.hInstance = ::GetModuleHandle(0);
        //        windowClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
        //        windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        //        windowClass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
        //        windowClass.lpszMenuName = 0;
        //        windowClass.lpszClassName = defaultMainFormClassName.c_str();
        //        windowClass.hIconSm = 0;
        //        ATOM atom = ::RegisterClassEx(&windowClass);
        //    }
        //    struct ElementCreator {
        //        struct CreationInfo {
        //            int id;
        //            HWND handle;
        //            std::map<std::string, void*> eventHandlers;
        //        };
        //        void operator()(Xml::Node* const node) {
        //            if (node->getNodeType() == Xml::Node::ELEMENT_NODE) {
        //                Xml::Element& element = static_cast<Xml::Element&>(*node);
        //                if (element.name() == L"form") {
        //                    createForm_(element);
        //                } else if (element.name() == L"background") {
        //                    CreationInfo* parentCreationInfo = static_cast<CreationInfo*>(element.Parent()->ext);
        //                    ::SetWindowText(parentCreationInfo->handle, L"background setting!");
        //                } else if (element.name() == L"button") {
        //                    createControl_(element, L"BUTTON");
        //                } else if (element.name() == L"list") {
        //                    createControl_(element, L"LISTBOX");
        //                } else if (element.name() == L"combobox") {
        //                    createControl_(element, L"COMBOBOX");
        //                } else if (element.name() == L"label") {
        //                    createControl_(element, L"STATIC");
        //                } else if (element.name() == L"edit") {
        //                    createControl_(element, L"EDIT");
        //                } else if (element.name() == L"richedit") {
        //                    createControl_(element, L"RICHEDIT_CLASS"); //L"RichEdit"
        //                } else if (element.name() == L"scrollbar") {
        //                    createControl_(element, L"SCROLLBAR");
        //                } else if (element.name() == L"imageViewer") {
        //                    createControl_(element, L"DImagePlayer");
        //                } else if (element.name() == L"audioPlayer") {
        //                    createControl_(element, L"DAudioPlayer");
        //                } else if (element.name() == L"videoPlayer") {
        //                    createControl_(element, L"DVideoPlayer");
        //                } else if (element.name() == L"smilPlayer") {
        //                    createControl_(element, L"DSmilPlayer");
        //                } else if (element.name() == L"composite") {
        //                    //create the panel[or static or form window]
        //                    createControl_(element, L"DPanel");
        //                } else {
        //                    //not support this type element
        //                    //MDICLIENT
        //                }
        //            }
        //        }
        //    private:
        //        void createForm_(Xml::Element& element, std::wstring const& className = defaultMainFormClassName) {
        //            RegisterFormClass();
        //            CreationInfo* creationInfo = new CreationInfo();
        //            creationInfo->id = StringOp::ToInt(element[L"@id"].value());
        //            DWORD exStyle = 0;
        //            std::wstring windowName;
        //            if (element.hasAttr(L"text")) {
        //                windowName = element[L"@text"].value();
        //            }
        //            DWORD style = WS_POPUP;
        //            if (windowName != L"") {
        //                style |= WS_CAPTION;
        //            }
        //            RECT bounds;
        //            if (element.hasAttr(L"bounds")) {
        //                bounds = StringOp::ToRectangle(element[L"@bounds"].value());
        //            } else {
        //                //bounds = Screen::size();
        //                bounds.left = 0;
        //                bounds.right = 1280;
        //                bounds.top = 0;
        //                bounds.bottom = 1024;
        //            }
        //            HWND parent = 0;
        //            HMENU menu = 0;
        //            HINSTANCE instance = ::GetModuleHandle(0);
        //            void* param = &element;
        //            //HWND window = ::CreateWindowEx(exStyle, L"BUTTON", L"OK", WS_POPUP | WS_VISIBLE, 0, 0, 100, 30, 0, 0, ::GetModuleHandle(0), 0);
        //            //std::wstring myClassName = L"BUTTON";
        //            //creationInfo->handle = ::CreateWindowEx(exStyle, myClassName.c_str(), windowName.c_str(), style, bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top, parent, menu, instance, param);
        //            creationInfo->handle = ::CreateWindowEx(exStyle, className.c_str(), windowName.c_str(), style, bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top, parent, menu, instance, param);
        //            if (!creationInfo->handle) {
        //                DWORD errorCode = ::GetLastError();
        //                DWORD ec = errorCode;
        //            }
        //            creationInfo->eventHandlers;
        //            element.ext = creationInfo;
        //        }
        //        void createControl_(Xml::Element& element, std::wstring const& className) {
        //            CreationInfo* creationInfo = new CreationInfo();
        //            CreationInfo* parentInfo = static_cast<CreationInfo*>(element.Parent()->ext);
        //            creationInfo->id = StringOp::ToInt(element[L"@id"].value());
        //            DWORD exStyle = 0;
        //            std::wstring windowName;
        //            if (element.hasAttr(L"text")) {
        //                windowName = element[L"@text"].value();
        //            }
        //            DWORD style = WS_CHILD | WS_VISIBLE;
        //            RECT bounds;
        //            if (element.hasAttr(L"bounds")) {
        //                bounds = StringOp::ToRectangle(element[L"@bounds"].value());
        //            } else {
        //                ::GetWindowRect(parentInfo->handle, &bounds);
        //            }
        //            HWND parent = parentInfo->handle;
        //            HMENU menu = reinterpret_cast<HMENU>(creationInfo->id);
        //            HINSTANCE instance = ::GetModuleHandle(0);
        //            void* param = &element;
        //            creationInfo->handle = ::CreateWindowEx(exStyle, className.c_str(), windowName.c_str(), style, bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top, parent, menu, instance, param);
        //            creationInfo->eventHandlers;
        //            element.ext = creationInfo;
        //        }
        //    };
        //}

        //void RenderForm(std::string const& descriptionFilename) {
        //    Util::DocSource* source = new Util::DocSource(descriptionFilename);
        //    Util::XMLParser parser;
        //    Util::shared_ptr<Xml::Document> doc = parser.Parse(source);
        //    delete source;

        //    if (doc->hasChild(L"view")) {
        //        Xml::Node& view = const_cast<Xml::Node&>((*doc)[L"view"]);
        //        if (view.hasChild(L"form")) {
        //            ElementCreator()(const_cast<Xml::Node*>(&view[L"form"]));
        //            view.ForeachChild(ElementCreator());
        //            ::ShowWindow(static_cast<ElementCreator::CreationInfo*>(view.ext)->handle, SW_SHOW);
        //        } else {
        //            throw std::exception("file format is incorrect");
        //        }
        //    } else {
        //        throw std::exception("not an ui description file");
        //    }
        //}
    }
}
