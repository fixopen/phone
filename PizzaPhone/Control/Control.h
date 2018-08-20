#ifndef __CONTROL_CONTROL_H__
#define __CONTROL_CONTROL_H__

#include <string>
#include <vector>
#include "../Util/Video/Graphics.h" //for Point Size Rect Color Image LineStyle
#include "../Util/Event/Event.h" //for MessageArgs MessageHandle

//#include <afxwin.h> //for CWnd

namespace Control {
    struct Background {
        enum Mode {
            mTransparent,
            mOpaque,
        };
        enum Type {
            tColor,
            tImage,
        };
        Background()
        : mode(mTransparent)
        , type(tColor)
        , color(Util::Video::Color())
        , image(0) {
        }
        Mode mode;
        Type type;
        Util::Video::Color color;
        Util::Video::Image* image;
    };

    struct PenMessageArgs : Util::Event::MessageArgs {
        short x;
        short y;
    };

    struct KeyMessageArgs : Util::Event::MessageArgs {
        char key;
    };

    class ContainerControl;
    class Control : public Util::Event::Component {
    public:
        Control(ContainerControl* const parent, Util::Video::Point const & position, Util::Video::Size const & size);
        ~Control();
        void DrawSelf() {
            drawBackground_();
            drawBorder_();
            drawContent_();
            afterDraw_();
        }
        Util::Video::Rect const GetWorldBounds() const;
        Util::Video::Rect const GetBounds() const {
            return Util::Video::Rect(position_, size_);
        }
        Util::Video::Point const GetPosition() const {
            return position_;
        }
        Util::Event::MessageHandler PenDown;
        Util::Event::MessageHandler PenUp;
        Util::Event::MessageHandler PenMove;
        Util::Event::MessageHandler Click;
    private:
        void drawBackground_();
        void drawBorder_();
        virtual void drawContent_();
        virtual void afterDraw_() {
        }
    protected:
        Util::Video::Point position_;
        Util::Video::Size size_;
        Background background_;
        Util::Video::LineStyle border_[4]; //left top right bottom
        std::wstring text_;
        size_t rowSpace_;
        size_t columnSpace_;
        Util::Video::Font font_;
        Util::Video::Color textColor_;
        size_t spaces_[4]; //left top right bottom
        size_t margins_[4]; //left top right bottom
        size_t paddings_[4]; //left top right bottom
        ContainerControl* parent_;
    };

    struct ScrollMessageArgs : Util::Event::MessageArgs {
        //Util::Rect originViewport;
        //Util::Rect currentViewport;
        short cx;
        short cy;
    };

    class ScrollableControl : public Control {
    public:
        ScrollableControl(ContainerControl* const parent, Util::Video::Point const & position, Util::Video::Size const & size);
        ~ScrollableControl();
        void SetReallyBounds(Util::Video::Rect const & reallyBounds, Util::Video::Point const & viewportOrigin = Util::Video::Point(0, 0));
        Util::Event::MessageHandler Scroll;
    private:
        virtual void drawContent_();
    private:
        //void onScroll_(Component const* const sender, ScrollMessageArgs const* const args);
        void onScroll_(Component const * const sender, Util::Event::MessageArgs const * const args);
    private:
        Util::Video::Rect reallyBounds_;
        Util::Video::Point viewportOrigin_;
    };

    class ContainerControl : public ScrollableControl {
        friend class Control;
    public:
        ContainerControl(ContainerControl* const parent, Util::Video::Point const & position, Util::Video::Size const & size);
        ~ContainerControl();
        Util::Event::MessageHandler KeyDown;
        Util::Event::MessageHandler KeyUp;
        Util::Event::MessageHandler Key;
    private:
        void addChild_(Control const * const child);
        void removeChild_(Control const * const child);
        virtual void drawContent_();
    private:
        //void onPenDown_(Component const* const sender, PenMessageArgs const* const args);
        void onPenDown_(Component const * const sender, Util::Event::MessageArgs const * const args);
        //void onPenUp_(Component const* const sender, PenMessageArgs const* const args);
        void onPenUp_(Component const * const sender, Util::Event::MessageArgs const * const args);
        //void onPenMove_(Component const* const sender, PenMessageArgs const* const args);
        void onPenMove_(Component const * const sender, Util::Event::MessageArgs const * const args);
        //void onClick_(Component const* const sender, PenMessageArgs const* const args);
        void onClick_(Component const * const sender, Util::Event::MessageArgs const * const args);
        //void onKeyDown_(Component const* const sender, KeyMessageArgs const* const args);
        void onKeyDown_(Component const * const sender, Util::Event::MessageArgs const * const args);
        //void onKeyUp_(Component const* const sender, KeyMessageArgs const* const args);
        void onKeyUp_(Component const * const sender, Util::Event::MessageArgs const * const args);
        //void onKey_(Component const* const sender, KeyMessageArgs const* const args);
        void onKey_(Component const * const sender, Util::Event::MessageArgs const * const args);
    private:
        std::vector<Control*> children_;
        Control* currentChild_;
    };

    struct TextInputMessageArgs : Util::Event::MessageArgs {
        std::wstring text;
    };

    class Edit : public Control {
    public:
        Edit();
        ~Edit();
        Util::Event::MessageHandler KeyDown;
        Util::Event::MessageHandler KeyUp;
        Util::Event::MessageHandler Key;
        Util::Event::MessageHandler TextInput;
    private:
        virtual void afterDraw_();
    };
}

#endif //__CONTROL_CONTROL_H__
