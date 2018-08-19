#ifndef __UTIL_VIDEO_SOFTKEYPAD_H__
#define __UTIL_VIDEO_SOFTKEYPAD_H__

//#include "Observer.h"
#include "Graphics.h"
//#include "../Control/Control.h"
namespace Control {
    class Control;
}

namespace Util {
    namespace Video {
        //switch button
        //move keypad button
        //hide keypad button
        //candidatelist panel
        //handdraw panel
        class SoftKeypad/* : public Observable*/ {
        public:
            struct KeyStruct {
                KeyStruct(Rect const & abounds, unsigned short const acode)
                : bounds(abounds)
                , code(acode) {
                }
                Rect bounds;
                unsigned short code;
            };
            static SoftKeypad* const Instance() {
                static SoftKeypad* result = new SoftKeypad();
                return result;
            }
            void SetKey(Rect const & bounds, unsigned short const code) {
                layout_.push_back(KeyStruct(bounds, code));
            }
            void SetOwner(Control::Control* owner) {
                owner_ = owner;
            }
        protected:
            unsigned short const getCode_(Point const & point);
        private:
            std::vector<KeyStruct> layout_;
            Control::Control* owner_;
        };
    }
}

#endif //__UTIL_VIDEO_SOFTKEYPAD_H__
