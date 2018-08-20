#ifndef __UTIL_CONTROL_WINCONTROL_H__
#define __UTIL_CONTROL_WINCONTROL_H__

#include <WinDef.h> //for HWND
#include <string> //for std::wstring
#include <vector> //for std::vector
#include <map> //for std::map
#include "../Graphics/Graphics.h" //for Graphics::Point, Graphics::Size, Graphics::Rect, Graphics::Color, Graphics::Font, Graphics::LineStyle
#include "../Event.h" //for MessageHandler::Handler MessageArgs
#include "../Text/XMLParser.h" //for Xml::*

//#include <afxwin.h> //for CWnd

namespace Util {
    namespace Control {
        enum UnitSelectStatus {
            no_select,
            in_select,
            select_end
        };

        enum PlayConfigItemType {
            type_ADVItem,
            type_SoundItem,
            type_KeyItem,
            type_MainFile,
            type_HungonFile
        };

        enum AdvanceType {
            adv_jpg,
            adv_video,
            advtype_end
        };

        struct VideoFileItem {
            std::wstring filename;
            unsigned int timeout;
        };

        struct SoundFileItem {
            std::wstring filename;
        };

        struct KeyFileItem {
            std::wstring filename;
            std::wstring telcode; //��Ӧ��������
        };

        struct AdvanceItem { //�õƹ��
            std::wstring advFilename;
            AdvanceType advType;
            std::wstring advRefre;
            int advTimeOut;
        };

        struct AdvPlayList {
            std::vector<AdvanceItem*> advanceItems;
            ~AdvPlayList() {
                for (std::size_t i = 0; i < advanceItems.size(); ++i) {
                    delete advanceItems[i];
                }
            }
        private:
            AdvPlayList const& operator=(AdvPlayList const& rhs);
        };

        struct PlayConfigList {
            std::wstring text;
            std::wstring mainFile;
            std::wstring outlineCode; //���ߺ���
            std::vector<AdvanceItem*> advanceItems;
            std::vector<SoundFileItem*> soundItems;
            std::vector<KeyFileItem*> keyItems;
            ~PlayConfigList() {
                for (std::size_t i = 0; i < advanceItems.size(); ++i) {
                    delete advanceItems[i];
                }

                for (std::size_t i = 0; i < soundItems.size(); ++i) {
                    delete soundItems[i];
                }

                for (std::size_t i = 0; i < keyItems.size(); ++i) {
                    delete keyItems[i];
                }
            }
        private:
            PlayConfigList const& operator=(PlayConfigList const& rhs);
        };

        enum TextFont {
            font_song,
            font_hei,
            textfont_end
        };

        enum TextSize {
            font_10,
            font_12,
            font_14,
            font_16,
            font_18,
            font_20,
            font_22,
            font_24,
            font_26,
            font_28,
            font_30,
            textsize_end
        };

        enum TextWeights {
            font_normal,
            font_bold,
            textweights_end
        };

        enum TextColor {
            font_black,
            font_white,
            font_red,
            font_blue,
            font_green,
            font_purple,
            textcolor_end
        };

        enum TextHeight {
            font_h_1,
            font_h_2,
            font_h_4,
            font_h_6,
            font_h_8,
            font_h_10,
            font_h_12,
            font_h_14,
            font_h_16,
            font_h_18,
            font_h_20,
            font_h_22,
            font_h_24,
            font_h_26,
            font_h_28,
            font_h_30,
            font_h_end
        };

        enum TextAlign {
            left_top,
            left_center,
            left_bottom,
            middle_top,
            middle_center,
            middle_bottom,
            right_top,
            right_center,
            right_bottom,
            textalign_end
        };

        enum UnitType {
            unit_no,
            unit_link,
            unit_click,
            unit_dial,
            unit_outdial,
            uint_pagedown,
            unit_pageup,
            unit_audio,
            unit_video,
            unittype_end
        };

        enum ControlType {
            ctrl_unit,
            ctrl_edit,
            ctrl_chechbutton,
            ctrl_radiobutton,
            ctrl_commbox,
            ctrtype_end
        };

        struct Unit {
            std::wstring backgroundFilename; //����ͼƬ (����)
            std::wstring pressedBackgroundFilename; //����ͼƬ (������)
            std::wstring content; //����
            Graphics::Rect bounds; //����
            TextFont font; //����
            TextSize fontSize; //�����С
            TextWeights fontWeights; //����
            bool isUnderline; //�»���
            bool isItalic; //�Ƿ�б��
            TextColor fontColor; //����ɫ
            TextHeight fontHeight; //�ּ��
            TextAlign fontAlign; //�ֶ��뷽ʽ
            UnitType unitType; //�������� (link, click, dial, outdial, video, mp3, pageup, pagedown)
            std::wstring unitContent; //��������
            bool isPressed; //�Ƿ���״̬��ʾ
            int serialNo; //�ؼ������
            bool isShow; //�Ƿ���ʾ
            bool isTransparent; //�Ƿ�֧��͸��ɫ
            ControlType controlType; //��Ԫ����
            int scroll; //�ؼ��Ƿ����ڹ��� ������    0 ������  1  page1  2 page2
            //CWnd* unitWindow; //��¼ edit radio check commbox Wnd
            HWND unitWindow; //��¼ edit radio check commbox Wnd
            bool isDisable; //�Ƿ����õ������Ӧ
        };

        enum ShowType {
            no_show,
            lefttoringt_show,
            showtype_end
        };

        struct Canvas {
            std::wstring backgroundFilename;
            std::wstring pressedBackgroundFilename;
            ShowType showType;
            Graphics::Rect bounds;
            std::wstring tab;

            std::vector<Unit> items;
            ~Canvas() {
                //for (int i = 0; i < items.size(); ++i) {
                //    if (items[i].unitWindow) {
                //        delete items[i].unitWindow;
                //    }
                //}
            }
        private:
            Canvas const& operator=(Canvas const& rhs);
        };

        typedef unsigned int WINDOWMESSAGE;
        class Window : public Component {
        public:
            Window();
            void Create(Window* const parent, Xml::Element& element);
            int const GetId() const {
                return id_;
            }
            HWND const Handle() const {
                return handle_;
            }
        private:
            static ATOM const registerClass_();
            static LRESULT CALLBACK windowMessageProc_(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
            static void fillMessageConvert_();
            LRESULT dispatcher_(WinMessageArgs const* const args);
        private:
            static std::wstring const defaultClassName_;
            static std::map<WINDOWMESSAGE, std::wstring> messageConvert_;
            std::map<std::wstring, MessageHandler> handlerConvert_;
            std::map<std::wstring, std::wstring> messageMap_;
            ATOM atom_;
            HWND parent_;
            HWND handle_;
            int id_;
        };
    }
}

#endif //__UTIL_CONTROL_WINCONTROL_H__
