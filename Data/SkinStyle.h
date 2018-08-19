#ifndef __DATA_SKINSTYLE_H__
#define __DATA_SKINSTYLE_H__

/*
SKIN_STYLE g_skinstyle = skin_blue;


COLORREF g_scroolbarFrameRGB[] = {RGB(6, 70, 92)};
COLORREF g_scroolbarMiddleLineRGB[] = {RGB(8, 92, 120)};
COLORREF g_scroolbarBackRGB[] = {RGB(10, 112, 145)};
int      g_scroolbarUpArrowBmpID[] = {IDB_BITMAP_UPARROW};
int      g_scroolbarDownArrowBmpID[] = {IDB_BITMAP_DOWNARROW};
int      g_scroolbarThickBmpID[] = {IDB_BITMAP_THICK};
*/
#define     SKIN_STYLE_MAX      4
namespace Data
{
    //��סƤ���ķ�� 
    //color define
    enum SKIN_STYLE {
        skin_blue,
        skin_red,
        skin_yellow,
        skin_white
    };

    extern SKIN_STYLE g_skinstyle;

    //������
    extern COLORREF g_scroolbarFrameRGB[];
    extern COLORREF g_scroolbarMiddleLineRGB[];
    extern COLORREF g_scroolbarBackRGB[];
    extern UINT16 g_scroolbarUpArrowBmpID[]; 
    extern UINT16 g_scroolbarDownArrowBmpID[];
    extern UINT16 g_scroolbarThickBmpID[];

    //������״̬��
    extern COLORREF g_mainstatusBackRGB[];
    extern UINT16 g_mainstatusMainBmpID[];
    extern UINT16 g_mainstatusToggleBmpID[];
    extern UINT16 g_mainstatusIMEBmpID[];
    extern UINT16 g_mainstatusTELBmpID[];
    extern UINT16 g_mainstatusDesktopBmpID[];
    extern UINT16 g_mainstatusCardBmpID[];
    extern UINT16 g_mainstatusCalllistBmpID[];
    extern UINT16 g_mainstatusRecordBmpID[];
    extern UINT16 g_mainstatusInfoBmpID[];
    extern UINT16 g_mainstatusSettingBmpID[];

    //ȫ�����ڱ�����
    extern COLORREF g_allFramBackRGB[];
    extern COLORREF g_allFramTopLineRGB[5][4];
    extern COLORREF g_allFramBottomLineRGB[4][4];
    extern COLORREF g_allFramLeftLineRGB[4][4];
    extern COLORREF g_allFramRightLineRGB[4][4];

    extern COLORREF g_allFramAngleTopLineRGB[5][4];
    extern COLORREF g_allFramAngleRightLineRGB[5][4];
    extern COLORREF g_allFramAngleXiexianLineRGB[2][4];
    extern COLORREF g_allFramAngleHerLineRGB[2][4];
    extern COLORREF g_allFramAngleBackLineRGB[4];

    extern COLORREF g_allFramInRectBackRGB[4];
    extern COLORREF g_allFramInRectTopLineRGB[3][4];
    extern COLORREF g_allFramInRectBottomLineRGB[3][4];
    extern COLORREF g_allFramInRectLeftLineRGB[3][4];
    extern COLORREF g_allFramInRectRightLineRGB[3][4];
    extern COLORREF g_allFramInRectAngleTopLineRGB[3][4];
    extern COLORREF g_allFramInRectAngleRightLineRGB[3][4];

    extern COLORREF g_allInRectLine1RGB[2][4];
    extern COLORREF g_allInRectLine2RGB[2][4];
    extern COLORREF g_allInRectLine3RGB[2][4];
    extern COLORREF g_allInRectLine4RGB[2][4];

    extern COLORREF g_allInRectBackRGB[4];
    //������Ƭ ͨ����¼ ϵͳ����������
    extern COLORREF g_allFrameInFrameLine1RGB[3][4];
    extern COLORREF g_allFrameInFrameLine2RGB[3][4];


    //��ȫ�����ڱ�����
    extern COLORREF g_partFrameTitle1TopRGB[4][4];
    extern COLORREF g_partFrameTitle1BottomRGB[4][4];
    extern COLORREF g_partFrameTitle1LeftRGB[4][4];
    extern COLORREF g_partFrameTitle1RightRGB[4][4];
    extern COLORREF g_partFrameTitle1BackRGB[4];

    extern COLORREF g_partFrameTitle2TopRGB[4][4];
    extern COLORREF g_partFrameTitle2BottomRGB[4][4];
    extern COLORREF g_partFrameTitle2RightRGB[4][4];
    extern COLORREF g_partFrameTitle2BackRGB[4];
    extern COLORREF g_partFrameTitle2LineRGB[2][4];

    extern COLORREF g_partFrameTitle1XiexianRGB[4][4];
    extern COLORREF g_partFrameTitle2XiexianRGB[4][4];

    extern COLORREF g_partFrameMainLeftRGB[4][4];
    extern COLORREF g_partFrameMainBottomRGB[5][4];
    extern COLORREF g_partFrameMainRightRGB[5][4]; 
    extern COLORREF g_partFrameMainBackRGB[4]; 

    extern COLORREF g_partFrameInRectBackRGB[4];
    extern COLORREF g_partFrameInRectTopRGB[4][4];
    extern COLORREF g_partFrameInRectBottomRGB[3][4];
    extern COLORREF g_partFrameInRectLeftRGB[4][4];
    extern COLORREF g_partFrameInRectRightRGB[7][4];

    //��ťͼ��
    extern UINT16 g_buttonArcBMPID[2][4];// = {{}, {}};
    extern UINT16 g_buttonArcBMPPARTDILOAGID[2][4];
    extern UINT16 g_buttonArcBMPALLDIALOGID[2][4];
    extern UINT16 g_buttonArcBMPSETTINGID[2][4];
    extern UINT16 g_buttonPlayBMPID[2][4];
    extern UINT16 g_buttonSoundBMPID[2][4];
    extern UINT16 g_buttonALLSelectBMPID[2][4];
    extern UINT16 g_buttonDialBMPID[2][4];
    extern UINT16 g_buttonRectBMPID[2][4];
    extern UINT16 g_buttonExitBMPID[2][4];
    extern UINT16 g_buttonExitBMP1ID[2][4];

    //������
    extern UINT16 g_progressPlayBMPID[2][4];
    extern UINT16 g_progressSoundBMPID[2][4];
    extern UINT16 g_progressBarBMPID[4];
    extern COLORREF g_progressBackRGB[4];
    extern COLORREF g_progressLeftRGB[4];
    extern COLORREF g_progressRightRGB[4];

    //������ʱ����ʾ��
    extern UINT16 g_mainTimeBmpID[4];
    extern COLORREF g_mainTimeFrameRGB[2][4];
    extern COLORREF g_mainTimeInFrameRGB[3][4];

    //���������ֱ���ͼ
    extern UINT16 g_mainTxtBmpID[4];
    extern COLORREF g_mainTxtColor[3][4];

    //������App ICON
    extern UINT16 g_mainAppVideoBmpID[4];
    extern UINT16 g_mainAppPhotoBmpID[4];
    extern UINT16 g_mainAppMp3BmpID[4];
    extern UINT16 g_mainAppCandBmpID[4];
    extern UINT16 g_mainAppCaluBmpID[4];
    extern UINT16 g_mainAppSaveBmpID[4];

    //static border ��ɫ
    extern COLORREF g_staticLine1RGB[2][4];
    extern COLORREF g_staticLine2RGB[2][4];
    extern COLORREF g_staticLine3RGB[2][4];
    extern COLORREF g_staticAppTileBackRGB[4];

    //listctrl ����ɫ
    extern COLORREF g_listctrlBackRGB1[4];
    extern COLORREF g_listctrlBackRGB2[4];
    extern UINT16 g_listctrlSelectBMPID[6][4]; 

    //edit ����ɫ
    extern COLORREF g_editBackRGB[4];
    extern COLORREF g_editLineRGB[4];

    //combox ����ɫ
    extern COLORREF g_comboxBackRGB[4];
    extern UINT16 g_comboxBMPID[2][4];

    //���뷨����ͼ
    extern UINT16 g_inputEngBMPID[4];
    extern UINT16 g_inputEng1BMPID[4];
    extern UINT16 g_inputPinyinBMPID[4];
    extern UINT16 g_inputWriteBMPID[4];
    extern UINT16 g_inputSysmolBMPID[4];

    //������softkey
    extern UINT16 g_caluSoftKeyBMP[4];

    //�����ؼ�
    extern COLORREF g_lunarderTitleRGB[4];
    extern COLORREF g_lunarderLineRGB[4];
    extern COLORREF g_lunarderBackRGB[4];
    extern COLORREF g_lunarderSecDayRGB[4];
    extern COLORREF g_lunarderNoteDayRGB[4];

    //����+����
    extern UINT16 g_lunarderNoteLeftBMPID[2][4];
    extern UINT16 g_lunarderNoteRightBMPID[2][4];
    extern UINT16 g_lunarderNoteUpBMPID[2][4];
    extern UINT16 g_lunarderNoteDownBMPID[2][4];

    class SkinResource {
        static void InitSkinStyle();
        static void SetSlinStyle(SKIN_STYLE style);
    };
}
#endif