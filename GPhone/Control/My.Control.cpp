namespace Controls {
    namespace Data {
        class Alignment {
        };

        class Color {
        };

        class Rectangle {
        };

        class Point {
        };

        class Size {
        };

        class Font {
        };

        class Image {
        };

        class Background {
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
            , color(Color())
            , image(0) {
            }
            Mode mode;
            Type type;
            Color color;
            Image* image;
        };
    }

    class Control {
    };

    class Menu : public Control {
    };

    class Timestamp : public Control {
    };

    class Duration : public Control {
    };

    class Progress : public Control {
    };

    class Slider : public Control {
    };

    class Editor : public Control {
    };

    class RichTextEditor : public Editor {
    };

    class Container : public Control {
    };

    class List : public Container {
    };

    class MediaRender : public Container {
    };

    class TextViewer : public MediaRender {
    };

    class PDFViewer : public MediaRender {
    };

    class XHTMLViewer : public MediaRender {
    };

    class ImageViewer : public MediaRender {
    };

    class AudioPlayer : public MediaRender {
    };

    class VideoPlayer : public MediaRender {
    };
}
