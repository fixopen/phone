#include "WinControl.h"
#include "../Graphics/Graphics.h" //for Graphics::CoordsOp
#include "../Graphics/Screen.h" //for Graphics::Screen
#include "../Text/StringOp.h" //for Text::StringOp

#include <windows.h>

namespace Util {
    namespace Control {
        std::wstring const Window::defaultClassName_ = L"DW";
        std::map<WINDOWMESSAGE, std::wstring> Window::messageConvert_;

        namespace {
            void childCreate(Xml::Node& parent, Xml::Node& node, void* params) {
                Window* parentWindow = static_cast<Window*>(parent.ext); //dynamic_cast

                if (node.getNodeType() == Xml::Node::ELEMENT_NODE) {
                    Window* selfWindow = new Window();
                    selfWindow->Create(parentWindow, static_cast<Xml::Element&>(node));
                }
            }
        }

        Window::Window()
            : Component()
            , atom_(0)
            , parent_(0)
            , handle_(0)
            , id_(0) {
        }

        void Window::Create(Window* const parent, Xml::Element& element) {
            atom_ = registerClass_();
            DWORD exStyle = 0;
            std::wstring windowName;

            if (element.hasAttr(L"text")) {
                windowName = element[L"@text"].value();
            }

            DWORD style = WS_POPUP;

            if (windowName != L"") {
                style |= WS_CAPTION;
            }

            Graphics::Rect bounds;

            if (element.hasAttr(L"bounds")) {
                bounds = Graphics::Rect::Parse(element[L"@bounds"].value());
            } else {
                bounds = Graphics::Rect(Graphics::Point(0, 0), Graphics::Screen::size());
            }

            parent_ = parent->Handle();
            HMENU menu = 0;
            HINSTANCE instance = ::GetModuleHandle(0);
            void* param = &element;
            handle_ = ::CreateWindowEx(exStyle, defaultClassName_.c_str(), windowName.c_str(), style, bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height, parent_, menu, instance, param);
            id_ = Text::StringOp::ToInt(element[L"@id"].value());
            element.ext = this;

            for (std::map<WINDOWMESSAGE, std::wstring>::iterator i = Window::messageConvert_.begin(); i != Window::messageConvert_.end(); ++i) {
                std::wstring event = i->second;

                if (element.hasAttr(event)) {
                    messageMap_.insert(std::make_pair(event, element[L"@" + event].value()));
                }
            }

            element.ForeachChild(childCreate, 0);
        }

        ATOM const Window::registerClass_() {
            WNDCLASS/*EX*/ windowClass;
            //windowClass.cbSize = sizeof windowClass;
            windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS | CS_NOCLOSE;
            windowClass.lpfnWndProc = windowMessageProc_;
            windowClass.cbClsExtra = 0;
            windowClass.cbWndExtra = 0;
            windowClass.hInstance = ::GetModuleHandle(0);
            //windowClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
            windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
            windowClass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
            windowClass.lpszMenuName = 0;
            windowClass.lpszClassName = defaultClassName_.c_str();
            //windowClass.hIconSm = 0;
            return ::RegisterClass/*Ex*/(&windowClass);
        }

        LRESULT CALLBACK Window::windowMessageProc_(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
            LRESULT result = S_OK;
            WinMessageArgs* args = new WinMessageArgs(window, message, wParam, lParam);
            //return dispatcher_(window, message, wParam, lParam);
            return result;
        }

        void Window::fillMessageConvert_() {
            //std::map<WINDOWMESSAGE, std::wstring> messageConvert_;
            messageConvert_.insert(std::make_pair(WM_KEYDOWN, L"onkeydown"));
            messageConvert_.insert(std::make_pair(WM_KEYUP, L"onkeyup"));
            messageConvert_.insert(std::make_pair(WM_CHAR, L"onchar"));
            messageConvert_.insert(std::make_pair(WM_LBUTTONDOWN, L"onpendown"));
            messageConvert_.insert(std::make_pair(WM_LBUTTONUP, L"onpenup"));
        }

        LRESULT Window::dispatcher_(WinMessageArgs const* const args) {
            LRESULT result = 0;
            std::wstring event = messageConvert_[args->message];

            if (messageMap_.find(event) != messageMap_.end()) { //has this handler
                std::wstring handler = messageMap_[event];

                if (handlerConvert_.find(handler) != handlerConvert_.end()) { //has this function
                    handlerConvert_[handler](this, args);
                } else { //not this function
                    throw "not handler function";
                }
            } else { //not this handler
                ::SendMessage(parent_, args->message, args->wParam, args->lParam);
            }

            result = ::DefWindowProc(args->window, args->message, args->wParam, args->lParam);
            return result;
        }
        //namespace {
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
