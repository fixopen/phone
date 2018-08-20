#ifndef __UTIL_CONTROL_CONTROL_H__
#define __UTIL_CONTROL_CONTROL_H__

#include <WinDef.h> //for HDC
#include <string> //for std::wstring
#include <vector> //for std::vector
#include "../Graphics/Graphics.h" //for Graphics::Point, Graphics::Size, Graphics::Rect, Graphics::Color, Graphics::LineStyle
#include "../Image/Image.h" //for Image::Image
#include "../Component.h" //for Component
#include "../Event.h" //for MessageArgs MessageHandle

namespace Util {
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
                , color(Graphics::Color())
                , image(0) {
            }
            Background(Graphics::Color const clr)
                : mode(mOpaque)
                , type(tColor)
                , color(clr)
                , image(0) {
            }
            Background(Image::Image* const img)
                : mode(mOpaque)
                , type(tImage)
                , color(Graphics::Color())
                , image(img) {
            }
            Mode mode;
            Type type;
            Graphics::Color color;
            Image::Image* image;
        };

        struct PointMessageArgs : MessageArgs {
            short x;
            short y;
        };

        struct KeyMessageArgs : MessageArgs {
            char key;
        };

        class ContainerControl;
        class Control : public Component {
            //event process
            //buble to parent
        public:
            Control(ContainerControl* const parent, Graphics::Point const& position, Graphics::Size const& size);
            ~Control();
            void DrawSelf(Graphics::Context const& g) {
                drawBackground_(g);
                drawBorder_(g);
                drawContent_(g);
                afterDraw_(g);
            }
            void SetBounds(Graphics::Rect const& bounds) {
                position_ = bounds.origin;
                size_ = bounds.size;
            }
            void SetBackground(Background const& background) {
                background_ = background;
            }
            void SetText(std::wstring const& text) {
                text_ = text;
            }
            void SetTextAttributes(Graphics::TextAttributes const& textAttr) {
                textAttributes_ = textAttr;
            }
            Graphics::Rect const GetWorldBounds() const;
            Graphics::Rect const GetBounds() const {
                return Graphics::Rect(position_, size_);
            }
            Graphics::Point const GetPosition() const {
                return position_;
            }
            MessageHandler PointDown;
            MessageHandler PointUp;
            MessageHandler PointMove;
            MessageHandler Click;
        private:
            void drawBackground_(Graphics::Context const& g);
            void drawBorder_(Graphics::Context const& g);
            virtual void drawContent_(Graphics::Context const& g);
            virtual void afterDraw_(Graphics::Context const& g) {}
        protected:
            Graphics::Point position_;
            Graphics::Size size_;
            Background background_;
            Graphics::LineStyle border_[4]; //left top right bottom
            std::wstring text_;
            Graphics::TextAttributes textAttributes_;
            size_t spaces_[4]; //left top right bottom
            size_t margins_[4]; //left top right bottom
            size_t paddings_[4]; //left top right bottom
            ContainerControl* parent_;
        };

        struct ScrollMessageArgs : MessageArgs {
            //Util::Rect originViewport;
            //Util::Rect currentViewport;
            short cx;
            short cy;
        };

        class ScrollableControl : public Control {
        public:
            ScrollableControl(ContainerControl* const parent, Graphics::Point const& position, Graphics::Size const& size);
            ~ScrollableControl();
            void SetReallyBounds(Graphics::Rect const& reallyBounds, Graphics::Point const& viewportOrigin = Graphics::Point(0, 0));
            MessageHandler Scroll;
        private:
            virtual void drawContent_();
        private:
            void onScroll_(Component const* const sender, MessageArgs const* const args);
        private:
            Graphics::Rect reallyBounds_;
            Graphics::Point viewportOrigin_;
        };

        class ContainerControl : public ScrollableControl {
            friend class Control;
            //capture event
            //resend to child
        public:
            ContainerControl(HWND const self);
            ContainerControl(ContainerControl* const parent, Graphics::Point const& position, Graphics::Size const& size);
            ~ContainerControl();
            MessageHandler KeyDown;
            MessageHandler KeyUp;
            MessageHandler Key;
        private:
            void initDelegates_();
            void addChild_(Control const* const child);
            void removeChild_(Control const* const child);
            virtual void drawContent_(Graphics::Context const& g);
        private:
            void onPointDown_(Component const* const sender, MessageArgs const* const args);
            void onPointUp_(Component const* const sender, MessageArgs const* const args);
            void onPointMove_(Component const* const sender, MessageArgs const* const args);
            void onClick_(Component const* const sender, MessageArgs const* const args);
            void onKeyDown_(Component const* const sender, MessageArgs const* const args);
            void onKeyUp_(Component const* const sender, MessageArgs const* const args);
            void onKey_(Component const* const sender, MessageArgs const* const args);
        private:
            std::vector<Control*> children_;
            Control* currentChild_;
            HWND handle_;
        };

        struct TextInputMessageArgs : MessageArgs {
            std::wstring text;
        };

        class Edit : public Control {
        public:
            Edit();
            ~Edit();
            MessageHandler KeyDown;
            MessageHandler KeyUp;
            MessageHandler Key;
            MessageHandler TextInput;
        private:
            virtual void afterDraw_();
        };
    }
}

#endif //__UTIL_CONTROL_CONTROL_H__
