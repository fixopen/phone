#ifndef __STRUCTURE_CONFIG_H__
#define __STRUCTURE_CONFIG_H__

#pragma warning(disable: 4786)

//#define _WIN32_WCE_NO_WININET
//#include <wtypes.h> //type define
//#define AFXAPI __stdcall
#include <string>
#include <map>
#include <vector>

namespace Util {
    enum UNITESELECTSTATUS {
        no_select,
        in_select,
        select_end
    };

    enum SHOWTYPE {
        no_show,
        lefttoringt_show,
        showtype_end
    };

    enum TEXTFONT{
        font_song,
        font_hei,
        textfont_end
    };
    enum TEXTSIZE{
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
    enum TEXTWEIGHTS{
        font_normal,
        font_bold,
        textweights_end
    };
    enum TEXTCOLOR{
        font_black,
        font_white,
        font_red,
        font_blue,
        font_green,
        font_purple,
        textcolor_end
    };
    enum TEXTHEIGHT{
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
    enum TEXTALIGN{
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
    enum UNIT_TYPE{
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
    enum ADV_TYPE{
        adv_jpg,
        adv_video,
        advtype_end
    };
    enum CTRL_TYPE{
        ctrl_unit,
        ctrl_edit,
        ctrl_chechbutton,
        ctrl_radiobutton,
        ctrl_commbox,
        ctrtype_end
    };

    /*
    ������ö�      left_top
    ��������		left_center
    ������õ�		left_bottom
    ���Ҿ����ö�	middle_top
    �������Ҿ���	middle_center	
    ���Ҿ����õ�	middle_bottom
    �Ҷ����ö�	right_top
    �Ҷ������	right_center
    �Ҷ����õ�	right_bottom
    */

    struct UNIT{
        CString				m_bgFilename;		//ǰ����ͼƬ (����)
        CString				m_bgFilename_down;	//�󱳾�ͼƬ (������)
        CString				m_sContent;			//����
        CRect				m_Rect;				//����
        TEXTFONT			m_Font;				//����
        TEXTSIZE			m_FontSize;			//�����С
        TEXTWEIGHTS			m_FontWeights;		//����
        BOOL				m_bFontUnLine;		//�»���
        BOOL				m_bFontItalic;		//�Ƿ�б��
        TEXTCOLOR			m_FontColor;		//����ɫ
        TEXTHEIGHT			m_FontHeight;       //�ּ��
        TEXTALIGN			m_FontAlign;		//�ֶ��뷽ʽ
        UNIT_TYPE			m_UnitType;			//�������� (link, click, dial, outdial, video, mp3, pageup, pagedown)		
        CString				m_UnitContent;		//��������
        BOOL				m_bIsDownStatus;	//�Ƿ���״̬��ʾ
        int					m_nSiererNO;		//�ؼ������
        BOOL				m_bIsShow;			//�Ƿ���ʾ
        BOOL				m_bIsTranslate;		//�Ƿ�֧��͸��ɫ
        CTRL_TYPE			m_Ctrltype;			//��Ԫ����
        int 				m_nIsScroll;		//�ؼ��Ƿ����ڹ��� ������    0 ������  1  page1  2 page2
        CWnd				*m_pUnitWnd;		//��¼ edit radio check commbox Wnd
        BOOL				m_bIsDisable;		//�Ƿ����õ������Ӧ.
    };

    enum PLAYCONFIGITEMTYPE{type_ADVItem, type_SoundItem, type_KeyItem, type_MainFile, type_HungonFile};

    struct Rectangle
    {
        unsigned short x0;
        unsigned short y0;
        unsigned short x1;
        unsigned short y1;
    };


    //PLAY config
    struct VIDEOFILEItem
    {
        CString filename;
        UINT	timeout;
    };
    struct SOUNDFILEItem
    {
        CString filename;
    };	
    struct KEYFILEItem
    {
        CString filename;
        CString telcode;		//��Ӧ��������
    };

    struct ADVItem
    {
        CString	advFilename;
        ADV_TYPE advType;
        CString advRefre;
        int		advTimeOut;
    };   //�õƹ��

    struct AdvPlayList
    {
        std::vector<ADVItem*> ADVItems;
        ~AdvPlayList()
        {
            for (std::size_t i = 0; i < ADVItems.size(); ++i)
                delete ADVItems[i];
        }
    private:
        AdvPlayList const& operator=(AdvPlayList const& rhs);
    };

    struct PLAYConfigList
    {
        CString text;
        CString mainFile;
        CString outcode;		//���ߺ���
        std::vector<ADVItem*> ADVItems;
        std::vector<SOUNDFILEItem*> Sounditems;
        std::vector<KEYFILEItem*> Keyitems;
        ~PLAYConfigList()
        {
            for (std::size_t i = 0; i < ADVItems.size(); ++i)
                delete ADVItems[i];
            for (std::size_t i = 0; i < Sounditems.size(); ++i)
                delete Sounditems[i];
            for (std::size_t i = 0; i < Keyitems.size(); ++i)
                delete Keyitems[i];
        }
    private:
        PLAYConfigList const& operator=(PLAYConfigList const& rhs);
    };

    //mJPG
    struct MJPGItem
    {
        UNIT unitparam;
    };

    struct MJPGList
    {
        CString bgfilename;
        CString bgfilename_down;
        SHOWTYPE showtype;
        CRect    m_Rect;

        std::vector<MJPGItem*> items;
        ~MJPGList()
        {
            for (std::size_t i = 0; i < items.size(); ++i)
            {
                if (items[i]->unitparam.m_pUnitWnd)
                    delete items[i]->unitparam.m_pUnitWnd;
                delete items[i];
            }
        }
    private:
        MJPGList const& operator=(MJPGList const& rhs);
    };
}

//using Structure::Environment;
//extern Structure::Environment* env;

//extern Structure::Environment* env;

#endif //__STRUCTURE_CONFIG_H__