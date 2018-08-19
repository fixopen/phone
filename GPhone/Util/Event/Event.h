#ifndef __UTIL_EVENT_EVENT_H__
#define __UTIL_EVENT_EVENT_H__

//#include <string>
//#include <vector>
#include <deque>

#include "../Thread.h"
//#include "../../Control/Control.h"
namespace Control {
    class ContainerControl;
}

namespace Util {
    namespace Event {
        class Component {
        public:
            virtual ~Component() = 0 {
            }
        };

        struct MessageArgs {
            enum Type {
                tUI,
            };
            virtual ~MessageArgs() {
            }
        };

        class MessageHandler {
        public:
            typedef void (Component::*Handler)(Component const * const sender,
                                               MessageArgs const * const args);
            MessageHandler();
            MessageHandler(Component const * const owner, Handler const handler);
            ~MessageHandler();
            void Clear();
            MessageHandler const operator=(MessageHandler const & rhs);
            void operator()(Component const * const sender, MessageArgs const * const args);
            void operator+=(MessageHandler const * const next);
            void operator-=(MessageHandler const * const next);
            bool const operator==(MessageHandler const & rhs);
            bool IsValid();
        private:
            bool isValid_;
            Component* owner_;
            Handler handler_;
            MessageHandler* nexts_;
        };

        struct WinMessageArgs : MessageArgs {
            WinMessageArgs(HWND awindow, UINT amessage, WPARAM awParam, LPARAM alParam)
            : window(awindow)
            , message(amessage)
            , wParam(awParam)
            , lParam(alParam) {
            }
            HWND window;
            UINT message;
            WPARAM wParam;
            LPARAM lParam;
        };

        class Application : public Component, public Thread {
        public:
            void AppendMessage(Component const * const sender, MessageArgs const * const args);
            virtual int const run();
        private:
            void dispatchMessage_();
        private:
            std::deque<std::pair<Component*, MessageArgs*> > messageQueue_;
            Control::ContainerControl* form_;
        };
    }
}

#endif //__UTIL_EVENT_EVENT_H__
