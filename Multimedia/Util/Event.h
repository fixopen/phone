#ifndef __UTIL_EVENT_H__
#define __UTIL_EVENT_H__

#pragma once

#include <WinDef.h> //for HWND, UINT, WPARAM, LPARAM
#include <deque> //for std::deque
#include <string> //for std::wstring
#include "Component.h" //for Component
#include "Thread.h" //for Thread
#include "Time/DateTime.h" //for Time::Timestamp

namespace Util {
    //Bubbling phase Yes : B
    //Bubbling phase No : b
    //Cancelable Yes : C
    //Cancelable No : c
    //Element : E
    //Attr : A
    //Comment : C
    //Text : T
    //Document : D
    //CDATASection : s
    //ProcessingInstruction : i
    //DocumentFragment : f
    //DocumentType : t
    //EntityReference : r
    class Event { //abort[BcE], change[BcE], error[BcE], load[bcDE], reset[BCE], select[BcE], submit[BCE], unload[bcDE]
    public:
        enum Phase {
            pCapturing = 1,
            pAtTarget = 2,
            pBubbling = 3,
        };
        Event();
        virtual ~Event();
    private:
        std::wstring name_;
        Component* source_;
        Component* observer_;
        Component* target_;
        Component* currentTarget_;
        Phase phase_;
        bool isBubbles_;
        bool isCancelable_;
        Time::Timestamp timestamp_;
    };
    class UIEvent : public Event { //blur[bcE], DOMActivate[BCE], DOMFocusIn[BcE], DOMFocusOut[BcE], focus[bcE], focusin[BcE], focusout[BcE], resize[BcDE], scroll[bcDE]
    };
    class MouseEvent : public UIEvent { //click[BCE], dblclick[BCE], mousedown[BCE], mouseenter[bCE], mouseleave[bCE], mousemove[BCE], mouseout[BCE], mouseover[BCE], mouseup[BCE]
    };
    class MouseWheelEvent : public MouseEvent { //mousewheel[BCDE]
    };
    class WheelEvent : public MouseEvent { //wheel[BCDE]
    };
    class TextEvent : public UIEvent { //textInput[BCE]
    };
    class KeyboardEvent : public UIEvent { //keydown[BCE], keypress[BCE], keyup[BCE]
    };
    class CompositionEvent : public UIEvent { //compositionstart[BCE], compositionupdate[BCE], compositionend[BCE]
    };
    class MutationEvent : public Event { //DOMAttrModified[BcE], DOMCharacterDataModified[BcTCsi], DOMNodeInserted[BcEATCstri], DOMNodeInsertedIntoDocument[bcEATCstri], DOMNodeRemoved[BcEATCstri], DOMNodeRemovedFromDocument[bcEATCstri], DOMSubtreeModified[BcDfEA]
    };
    class MutationNameEvent : public MutationEvent { //DOMAttributeNameChanged[BcE], DOMElementNameChanged[BcE]
    };
    typedef Event Message;
    typedef Message MessageArgs;

    class MessageHandler {
    public:
        typedef void (Component::*Handler)(Component const* const sender, MessageArgs const* const args);
        MessageHandler();
        MessageHandler(Component const* const owner, Handler const handler);
        ~MessageHandler();
        void Clear();
        MessageHandler const operator=(MessageHandler const& rhs);
        void operator()(Component const* const sender, MessageArgs const* const args);
        void operator+=(MessageHandler const* const next);
        void operator-=(MessageHandler const* const next);
        bool const operator==(MessageHandler const& rhs);
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

    namespace Control {
        class ContainerControl;
    }
    class Application : public Component, public Thread {
    public:
        void AppendMessage(Component const* const sender, MessageArgs const* const args);
        virtual int const run();
    private:
        void dispatchMessage_();
    private:
        std::deque<std::pair<Component*, MessageArgs*> > messageQueue_;
        Control::ContainerControl* form_;
    };
}

#endif //__UTIL_EVENT_H__
