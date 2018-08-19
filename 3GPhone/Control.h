#pragma once

#include <string>
#include <vector>

struct Point {
    int x;
    int y;
};

struct Size {
    int width;
    int height;
};

class ImageViewer {
public:
    enum RenderMode {
        mP, //渐现
        mN, //百叶窗。。。
    };
    ImageViewer(CWnd* parent);
    void SetImageFilename(std::wstring const& filename);
    void SetImageRegion(Point const& origin, Size const& size);
    void SetEnterMode(RenderMode const renderMode);
    void SetLightness(unsigned int const lightness);
    void SetContract(unsigned int const contract);
};

class VideoPlayer {
public:
    enum ScaleMode {
        mFit, //缩放至控件大小
        m, //保持纵横比
    };
    VideoPlayer(CWnd* parent);
    void SetVideoFilename(std::wstring const& filename);
    void SetScaleMode(ScaleMode const renderMode);
    void SetLightness(unsigned int const lightness);
    void SetContract(unsigned int const contract);
    void SetVolume(unsigned int const volume);
    void Pause();
    void Resume();
};

class AudioPlayer {
public:
    AudioPlayer(CWnd* parent);
    void SetAudioFilename(std::wstring const& filename);
    void SetVolume(unsigned int const volume);
    void Pause();
    void Resume();
};

class TextViewer {
public:
    TextViewer(CWnd* parent);
    void SetTextContent(std::wstring const& content);
    void SetFont(CFont const& font);
};

class TextEditor {
public:
    TextEditor(CWnd* parent);
    std::wstring const GetTextContent() const;
    void SetFont(CFont const& font);
};

namespace Control {
    struct Point {
        int x;
        int y;
    };
    
    struct Size {
        int width;
        int height;
    };

    namespace EventHandlerType {
        class None {
        };

        enum MouseButton {
            bLeft,
            bMiddle,
            bRight,
        };

        enum ControlKey {
            kControl,
            kShift,
            kAlt,
        };

        class Selectable {
            void SelectAction(int x, int y, MouseButton mouseButton, ControlKey controlKey);
        };

        class CharacterInputable {
        };
    }

    namespace ContentType {
        class None {
        };

        class Drawable {
        public:
            void ExecCommand(std::wstring command);
        private:
            //CDC* dc_;
            HDC dc_;
        };

        class Imageable {
        public:
            enum RenderMode {
                m
            };

            std::wstring filename;
            Point origin;
            Size size;
            void SetEnterMode(RenderMode renderMode);
            void SetLightness(unsigned int lightness);
            void SetContract(unsigned int contract);
            void Draw();
        };

        class Textable {
        public:
            std::wstring content;
            CFont* font;
            void Draw();
        };

        class StreamMedia {
        public:
            void Pause();
            void Resume();
            void SetLightness(unsigned int lightness);
            void SetContract(unsigned int contract);
            void SetVolume(unsigned int volume);
        };
    }

    namespace PositionType {
        class Dockable {
        };

        class Moveable {
        };
    }

    namespace Container {
        class Layout {
        };

        class BoxLayout : public Layout {
        };

        class BorderLayout : public Layout {
        };

        class FlowLayout : public Layout {
        };

        class GridLayout : public Layout {
        };

        class CanvasLayout : public Layout {
        };

        class Container {
        public:
            //std::vector<Control*> children;
            Layout* layout;
        };
    }

    //// Control
    //template<typename EventHandlerType, typename ContentType, typename PositionType, typename Container>
    //class Control : public CWnd, public EventHandlerType, public ContentType, public PositionType, public Container {
    //    DECLARE_DYNAMIC(Control)

    //public:
    //    Control(Control* const parent);
    //    ~Control();
    //    void Create();
    //    void Show();
    //    void Hide();
    //    void Destroy();

    //protected:
    //    DECLARE_MESSAGE_MAP()

    //private:
    //    Control* parent_;
    //    Point position_;
    //    Size size_;
    //    Point contentPosition_;
    //    Size contentSize_;
    //};
}

