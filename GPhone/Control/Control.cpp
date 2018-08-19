#include "Control.h"
#include "../Util/Video/Graphics.h" //for CoordsOp

#include <windows.h>
//#include <WinDef.h>
//#include <WinBase.h>
//#include <WinUser.h>

namespace Control {
    Control::Control(ContainerControl* const parent, Util::Video::Point const & position, Util::Video::Size const & size)
    : parent_(parent)
    , position_(position)
    , size_(size) {
        if (parent_) {
            parent_->addChild_(this);
        }
    }

    Control::~Control() {
    }

    Util::Video::Rect const Control::GetWorldBounds() const {
        Util::Video::Point origin = position_;
        Control* p = parent_;
        while (p) {
            origin = Util::Video::CoordsOp::Move(Util::Video::CoordsOp::SymmetryPoint(p->position_), origin);
            p = p->parent_;
        }
        return Util::Video::Rect(origin, size_);
    }

    void Control::drawBackground_() {
        switch (background_.mode) {
        case Background::mOpaque:
            switch (background_.type) {
            case Background::tColor:
                Util::Video::Graphics::FillRect(GetWorldBounds(), background_.color);
                break;
            case Background::tImage:
                Util::Video::Graphics::DrawImage(GetWorldBounds(), *background_.image);
                break;
            default:
                break;
            }
            break;
        case Background::mTransparent:
            break;
        default:
            break;
        }
    }

    void Control::drawBorder_() {
        Util::Video::Graphics::DrawLine(position_, Util::Video::Point(position_.x, position_.y + size_.height), border_[0]);
        Util::Video::Graphics::DrawLine(position_, Util::Video::Point(position_.x + size_.width, position_.y), border_[1]);
        Util::Video::Graphics::DrawLine(Util::Video::Point(position_.x + size_.width, position_.y), Util::Video::Point(position_.x + size_.width, position_.y + size_.height), border_[2]);
        Util::Video::Graphics::DrawLine(Util::Video::Point(position_.x, position_.y + size_.height), Util::Video::Point(position_.x + size_.width, position_.y + size_.height), border_[3]);
    }

    void Control::drawContent_() {
        Util::Video::Graphics::DrawString(GetBounds(), text_, rowSpace_, columnSpace_, font_, textColor_);
    }

    ScrollableControl::ScrollableControl(ContainerControl* const parent, Util::Video::Point const & position, Util::Video::Size const & size)
    : Control(parent, position, size)
    , reallyBounds_(GetBounds())
    , viewportOrigin_(Util::Video::Point(0, 0)) {
        Scroll = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ScrollableControl::onScroll_));
    }

    ScrollableControl::~ScrollableControl() {
    }

    void ScrollableControl::SetReallyBounds(Util::Video::Rect const & reallyBounds, Util::Video::Point const & viewportOrigin) {
        reallyBounds_ = reallyBounds;
        viewportOrigin_ = viewportOrigin;
    }

    void ScrollableControl::drawContent_() {
        //step1: draw to virtual surface
        //Util::Graphics::DrawString(GetBounds(), text_, font_, textColor_);
        //step2: copy to control surface
        //Util::Graphics::CopyImage(...);
    }

    void ScrollableControl::onScroll_(Component const * const sender, Util::Event::MessageArgs const * const args) {
        //ScrollMessageArgs* const newArgs = const_cast<ScrollMessageArgs* const>(dynamic_cast<ScrollMessageArgs const* const>(args));
        ScrollMessageArgs* const newArgs = const_cast<ScrollMessageArgs* const>(static_cast<ScrollMessageArgs const * const>(args));
        viewportOrigin_.x += newArgs->cx;
        viewportOrigin_.y += newArgs->cy;
    }

    ContainerControl::ContainerControl(ContainerControl* const parent, Util::Video::Point const & position, Util::Video::Size const & size)
    : ScrollableControl(parent, position, size)
    , currentChild_(0) {
        PenDown = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onPenDown_));
        PenUp = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onPenUp_));
        PenMove = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onPenMove_));
        Click = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onClick_));
        KeyDown = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onKeyDown_));
        KeyUp = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onKeyUp_));
        Key = Util::Event::MessageHandler(this, static_cast<Util::Event::MessageHandler::Handler>(&ContainerControl::onKey_));
    }

    ContainerControl::~ContainerControl() {
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            delete * i;
        }
    }

    void ContainerControl::addChild_(Control const * const child) {
        children_.push_back(const_cast<Control*>(child));
    }

    void ContainerControl::removeChild_(Control const * const child) {
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            if (*i == child) {
                delete * i;
                children_.erase(i);
                break;
            }
        }
    }

    void ContainerControl::drawContent_() {
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            (*i)->DrawSelf();
        }
        Util::Video::Graphics::DrawString(GetBounds(), text_, rowSpace_, columnSpace_, font_, textColor_);
    }

    void ContainerControl::onPenDown_(Util::Event::Component const * const sender, Util::Event::MessageArgs const * const args) {
        //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
        PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(static_cast<PenMessageArgs const * const>(args));
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            if (Util::Video::CoordsOp::PointInRect(Util::Video::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                Util::Video::Point p = Util::Video::CoordsOp::Move((*i)->GetPosition(), Util::Video::Point(newArgs->x, newArgs->y));
                newArgs->x = p.x;
                newArgs->y = p.y;
                (*i)->PenDown(sender, newArgs);
            }
        }
    }

    void ContainerControl::onPenUp_(Util::Event::Component const * const sender, Util::Event::MessageArgs const * const args) {
        //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
        PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(static_cast<PenMessageArgs const * const>(args));
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            if (Util::Video::CoordsOp::PointInRect(Util::Video::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                Util::Video::Point p = Util::Video::CoordsOp::Move((*i)->GetPosition(), Util::Video::Point(newArgs->x, newArgs->y));
                newArgs->x = p.x;
                newArgs->y = p.y;
                (*i)->PenUp(sender, newArgs);
            }
        }
    }

    void ContainerControl::onPenMove_(Util::Event::Component const * const sender, Util::Event::MessageArgs const * const args) {
        //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
        PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(static_cast<PenMessageArgs const * const>(args));
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            if (Util::Video::CoordsOp::PointInRect(Util::Video::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                Util::Video::Point p = Util::Video::CoordsOp::Move((*i)->GetPosition(), Util::Video::Point(newArgs->x, newArgs->y));
                newArgs->x = p.x;
                newArgs->y = p.y;
                (*i)->PenMove(sender, newArgs);
            }
        }
    }

    void ContainerControl::onClick_(Util::Event::Component const * const sender, Util::Event::MessageArgs const * const args) {
        //PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(dynamic_cast<PenMessageArgs const* const>(args));
        PenMessageArgs* const newArgs = const_cast<PenMessageArgs* const>(static_cast<PenMessageArgs const * const>(args));
        for (std::vector<Control*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            if (Util::Video::CoordsOp::PointInRect(Util::Video::Point(newArgs->x, newArgs->y), (*i)->GetBounds())) {
                Util::Video::Point p = Util::Video::CoordsOp::Move((*i)->GetPosition(), Util::Video::Point(newArgs->x, newArgs->y));
                newArgs->x = p.x;
                newArgs->y = p.y;
                (*i)->Click(sender, newArgs);
            }
        }
    }

    void ContainerControl::onKeyDown_(Component const * const sender, Util::Event::MessageArgs const * const args) {
        //currentChild_->KeyDown();
    }

    void ContainerControl::onKeyUp_(Component const * const sender, Util::Event::MessageArgs const * const args) {
    }

    void ContainerControl::onKey_(Component const * const sender, Util::Event::MessageArgs const * const args) {
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
