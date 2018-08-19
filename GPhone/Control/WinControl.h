#ifndef __CONTROL_WINCONTROL_H__
#define __CONTROL_WINCONTROL_H__

#include <string>
#include <vector>
#include "../Util/Video/Graphics.h" //for Point Size Rect Color Image LineStyle
#include "../Util/Event/Event.h" //for MessageHandler::Handler MessageArgs
#include "../Util/Text/XMLParser.h"

//#include <afxwin.h> //for CWnd

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

    struct AdvanceItem {
        //�õƹ��
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
        AdvPlayList const & operator=(AdvPlayList const & rhs);
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
        PlayConfigList const & operator=(PlayConfigList const & rhs);
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
        Util::Video::Rect bounds; //����
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
        Util::Video::Rect bounds;
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
        Canvas const & operator=(Canvas const & rhs);
    };

    typedef unsigned int WINDOWMESSAGE;
    class Handler;
    //Window is the WinControl for application
    //not split the Model from the Control
    //It's a thin warp for Win32 Control
    //include create destroy message-capture and send to parent capability
    class Window : public Util::Event::Component {
    public:
        Window(Window* const parent, Xml::Element& element);
        int const GetId() const {
            return id_;
        }
        HWND const Handle() const {
            return handle_;
        }
        static Window const* const FindWindowByHandle(HWND const handle);
    private:
        static ATOM const registerClass_();
        static LRESULT CALLBACK windowMessageProc_(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
        static void fillMessageConvert_();
        LRESULT const dispatcher_(Util::Event::WinMessageArgs const * const args) const;
    private:
        Xml::Element& element_;
        Handler* root_;
        static std::wstring const defaultClassName_;
        static std::map<WINDOWMESSAGE, std::wstring> messageConvert_;
        std::map<std::wstring, std::wstring> messageMap_;
        ATOM atom_;
        HWND parent_;
        HWND handle_;
        int id_;
    };
    //Handler is the MessageHandler for application
    //include message map and dispatcher capability
    class Handler : public Util::Event::Component {
        friend class Window;
    public:
        Handler(Handler* const parent, Xml::Document& document);
        void Show();
        void Hide();
    private:
        std::map<std::wstring, Util::Event::MessageHandler> handlerConvert_;
        Xml::Document& document_;
    };
}

#endif //__CONTROL_WINCONTROL_H__
