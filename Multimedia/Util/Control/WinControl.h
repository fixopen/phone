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
            std::wstring telcode; //对应按键号码
        };

        struct AdvanceItem { //幻灯广告
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
            std::wstring outlineCode; //外线号码
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
            std::wstring backgroundFilename; //背景图片 (正常)
            std::wstring pressedBackgroundFilename; //背景图片 (被按下)
            std::wstring content; //内容
            Graphics::Rect bounds; //区域
            TextFont font; //字体
            TextSize fontSize; //字体大小
            TextWeights fontWeights; //粗体
            bool isUnderline; //下划线
            bool isItalic; //是否斜体
            TextColor fontColor; //字颜色
            TextHeight fontHeight; //字间距
            TextAlign fontAlign; //字对齐方式
            UnitType unitType; //动作类型 (link, click, dial, outdial, video, mp3, pageup, pagedown)
            std::wstring unitContent; //动作内容
            bool isPressed; //是否按下状态显示
            int serialNo; //控件的序号
            bool isShow; //是否显示
            bool isTransparent; //是否支持透明色
            ControlType controlType; //单元类型
            int scroll; //控件是否属于滚动 分组中    0 不滚动  1  page1  2 page2
            //CWnd* unitWindow; //记录 edit radio check commbox Wnd
            HWND unitWindow; //记录 edit radio check commbox Wnd
            bool isDisable; //是否设置点击不响应
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
