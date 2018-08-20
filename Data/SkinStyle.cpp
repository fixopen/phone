#include "stdafx.h"
#include "SkinStyle.h"

#include "../resource.h"

namespace Data{
	//记录当前皮肤风格
	SKIN_STYLE g_skinstyle;
	
	//滚动条
	/*
	COLORREF g_scroolbarFrameRGB[] = {RGB(6, 70, 92)};
	COLORREF g_scroolbarMiddleLineRGB[] = {RGB(8, 92, 120)};
	COLORREF g_scroolbarBackRGB[] = {RGB(10, 112, 145)};
	*/
	COLORREF g_scroolbarFrameRGB[] = {RGB(0, 0, 0)};
	COLORREF g_scroolbarMiddleLineRGB[] = {RGB(57, 90, 140)};//{RGB(171, 169, 170)};
	COLORREF g_scroolbarBackRGB[] = {RGB(57, 90, 140)};//{RGB(171, 169, 170)};

	UINT16   g_scroolbarUpArrowBmpID[] = {IDB_BITMAP_UPARROW};
	UINT16   g_scroolbarDownArrowBmpID[] = {IDB_BITMAP_DOWNARROW};
    UINT16   g_scroolbarThickBmpID[] = {IDB_BITMAP_THICK};

	//主界面状态条
	COLORREF g_mainstatusBackRGB[] = {RGB(0, 0, 0)};		//目前无效
	UINT16	g_mainstatusMainBmpID[] = {IDB_BITMAP_MAIN};
	UINT16	g_mainstatusToggleBmpID[] = {IDB_BITMAP_MAINTOGGLE};
	UINT16	g_mainstatusIMEBmpID[] = {IDB_BITMAP_MAININPUT};
	UINT16	g_mainstatusTELBmpID[] = {IDB_BITMAP_MAINTEL};
	UINT16	g_mainstatusDesktopBmpID[] = {IDB_BITMAP_MAINDESKTOP};
	UINT16	g_mainstatusCardBmpID[] = {IDB_BITMAP_MAINCARD};
	UINT16	g_mainstatusCalllistBmpID[] = {IDB_BITMAP_MAINCALLLIST};
	UINT16	g_mainstatusRecordBmpID[] = {IDB_BITMAP_MAINRECORD};
	UINT16	g_mainstatusInfoBmpID[] = {IDB_BITMAP_MAININFO};
	UINT16	g_mainstatusSettingBmpID[] = {IDB_BITMAP_MAINSETTING};

	//全屏窗口背景框
	COLORREF g_allFramBackRGB[] = {RGB(89, 189, 238)};
	COLORREF g_allFramTopLineRGB[5][4] = {{RGB(12, 20, 23)},{RGB(17, 73, 106)}, {RGB(14, 135, 190)}, {RGB(120, 212, 251)}, {RGB(14, 139, 197)}};
	COLORREF g_allFramBottomLineRGB[4][4] = {{RGB(12, 20, 23)},{RGB(17, 73, 106)}, {RGB(14, 135, 190)}, {RGB(14, 139, 19)}};
	COLORREF g_allFramLeftLineRGB[4][4] = {{RGB(12, 20, 23)},{RGB(17, 73, 106)}, {RGB(13, 163, 224)}, {RGB(164, 236, 251)}};
	COLORREF g_allFramRightLineRGB[4][4] = {{RGB(12, 20, 23)},{RGB(17, 73, 106)}, {RGB(17, 128, 181)}, {RGB(14, 154, 213)}};

	COLORREF g_allFramAngleTopLineRGB[5][4] = {{RGB(12, 20, 23)},{RGB(17, 73, 106)}, {RGB(31, 48, 126)}, {RGB(71, 91, 190)}, {RGB(45, 67, 167)}};
	COLORREF g_allFramAngleRightLineRGB[5][4] = {{RGB(12, 20, 23)},{RGB(17, 73, 106)}, {RGB(31, 48, 126)}, {RGB(71, 91, 190)}, {RGB(39, 60, 151)}};
	COLORREF g_allFramAngleXiexianLineRGB[2][4] = {{RGB(1, 48, 66)}, {RGB(58, 121, 190)}};
	COLORREF g_allFramAngleHerLineRGB[2][4] = {{RGB(54, 87, 217)}, {RGB(23, 37, 110)}};
	COLORREF g_allFramAngleBackLineRGB[4] = {RGB(45, 67, 169)};
	
	COLORREF g_allFramInRectBackRGB[4] = {RGB(189, 210, 241)};
	COLORREF g_allFramInRectTopLineRGB[3][4] = {{RGB(4, 59, 80)}, {RGB(26, 116, 151)}, {RGB(32, 137, 182)}};
	COLORREF g_allFramInRectBottomLineRGB[3][4] = {{RGB(7, 119, 157)}, {RGB(31, 138, 182)}, {RGB(9, 160, 215)}};
	COLORREF g_allFramInRectLeftLineRGB[3][4] = {{RGB(4, 59, 80)}, {RGB(32, 137, 182)}, {RGB(9, 160, 215)}};
	COLORREF g_allFramInRectRightLineRGB[3][4] = {{RGB(9, 112, 145)}, {RGB(31, 138, 182)}, {RGB(9, 160, 215)}};
	COLORREF g_allFramInRectAngleTopLineRGB[3][4] = {{RGB(15, 30, 87)}, {RGB(24, 38, 109)}, {RGB(34, 54, 140)}};
	COLORREF g_allFramInRectAngleRightLineRGB[3][4] = {{RGB(17, 30, 85)}, {RGB(37, 54, 142)}, {RGB(34, 54, 140)}};

	COLORREF g_allInRectLine1RGB[2][4] = {{RGB(0, 107, 150)}, {RGB(140, 214, 247)}};
	COLORREF g_allInRectLine2RGB[2][4] = {{RGB(8, 74, 90)}, {RGB(8, 74, 90)}};
	COLORREF g_allInRectLine3RGB[2][4] = {{RGB(8, 148, 197)}, {RGB(8, 123, 165)}};
	COLORREF g_allInRectLine4RGB[2][4] = {{RGB(16, 165, 222)}, {RGB(8, 123, 165)}};
	COLORREF g_allInRectBackRGB[4] = {{RGB(8, 148, 197)}};

		//用于名片 通话记录 系统设置主窗口
	COLORREF g_allFrameInFrameLine1RGB[3][4] = {{RGB(4, 59, 80)}, {RGB(26, 116, 151)}, {RGB(32, 137, 182)}};
	COLORREF g_allFrameInFrameLine2RGB[3][4] = {{RGB(4, 59, 80)}, {RGB(26, 116, 151)}, {RGB(32, 137, 182)}};

	//非全屏窗口背景框
	COLORREF g_partFrameTitle1TopRGB[4][4] = {{RGB(2, 48, 64)}, {RGB(116, 119, 200)}, {RGB(123, 129, 213)}, {RGB(155, 161, 235)}};
	COLORREF g_partFrameTitle1BottomRGB[4][4] = {{RGB(1, 48, 66)}, {RGB(88, 92, 166)}, {RGB(99, 109, 196)}, {RGB(118, 123, 205)}};
	COLORREF g_partFrameTitle1LeftRGB[4][4] = {{RGB(2, 48, 63)}, {RGB(116, 119, 198)}, {RGB(123, 129, 215)}, {RGB(156, 160, 234)}};
	COLORREF g_partFrameTitle1RightRGB[4][4] = {{RGB(1, 48, 66)}, {RGB(98, 92, 166)}, {RGB(102, 108, 194)}, {RGB(119, 123, 210)}};
	COLORREF g_partFrameTitle1BackRGB[4] = {RGB(132, 136, 223)};

	COLORREF g_partFrameTitle2TopRGB[4][4] = {{RGB(2, 47, 66)}, {RGB(30, 48, 130)}, {RGB(42, 65, 161)}, {RGB(69, 91, 190)}};
	COLORREF g_partFrameTitle2BottomRGB[4][4] = {{RGB(1, 48, 66)}, {RGB(31, 48, 130)}, {RGB(39, 60, 153)}, {RGB(44, 64, 161)}};
	COLORREF g_partFrameTitle2RightRGB[4][4] = {{RGB(1, 48, 64)}, {RGB(31, 47, 132)}, {RGB(39, 60, 151)}, {RGB(46, 68, 169)}};
	COLORREF g_partFrameTitle2BackRGB[4] = {RGB(45, 67, 168)};
	COLORREF g_partFrameTitle2LineRGB[2][4] = {{RGB(54, 82, 215)}, {RGB(23, 37, 110)}};

	COLORREF g_partFrameTitle1XiexianRGB[4][4] = {{RGB(103, 107, 194)}, {RGB(98, 92, 166)}, {RGB(0, 49, 60)}, {RGB(195, 201, 233)}};
	COLORREF g_partFrameTitle2XiexianRGB[4][4] = {{RGB(117, 123, 207)}, {RGB(103, 107, 194)}, {RGB(0, 48, 68)}, {RGB(39, 56, 138)}};

	COLORREF g_partFrameMainLeftRGB[4][4] = {{RGB(2, 47, 66)}, {RGB(173, 181, 230)}, {RGB(195, 200, 232)}, {RGB(221, 221, 247)}};
	COLORREF g_partFrameMainBottomRGB[5][4] = {{RGB(2, 47, 66)}, {RGB(142, 150, 197)}, {RGB(195, 200, 232)}, {RGB(201, 206, 235)}, {RGB(220, 221, 249)}};
	COLORREF g_partFrameMainRightRGB[5][4] = {{RGB(2, 48, 64)}, {RGB(171, 181, 230)}, {RGB(195, 199, 234)}, {RGB(201, 206, 236)}, {RGB(217, 221, 246)}};
	COLORREF g_partFrameMainBackRGB[4] = {RGB(211, 211, 244)};

	COLORREF g_partFrameInRectBackRGB[4] = {{RGB(178, 180, 219)}};
	COLORREF g_partFrameInRectTopRGB[4][4] = {{RGB(142, 150, 197)}, {RGB(173, 181, 230)}, {RGB(194, 200, 234)}, {RGB(254, 255, 255)}};
	COLORREF g_partFrameInRectBottomRGB[3][4] = {{RGB(142, 150, 179)}, {RGB(173, 181, 230)}, {RGB(194, 200, 234)}};
	COLORREF g_partFrameInRectLeftRGB[4][4] = {{RGB(142, 150, 197)}, {RGB(173, 181, 230)}, {RGB(194, 200, 234)}, {RGB(232, 233, 251)}};
	COLORREF g_partFrameInRectRightRGB[7][4] = {{RGB(142, 150, 197)}, {RGB(173, 181, 230)}, {RGB(194, 200, 234)}, {RGB(210, 210, 244)}, {RGB(142, 150, 197)}, {RGB(173, 181, 230)}, {RGB(194, 200, 234)}};

	//按钮图标
	UINT16 g_buttonArcBMPID[2][4] = {{IDB_BITMAP_ARC_TOP1}, {IDB_BITMAP_ARC_BOTTOM1}};
	UINT16 g_buttonArcBMPPARTDILOAGID[2][4] = {{IDB_BITMAP_ARC_TOP2}, {IDB_BITMAP_ARC_BOTTOM2}};
	UINT16 g_buttonArcBMPALLDIALOGID[2][4] = {{IDB_BITMAP_ARC_TOP3}, {IDB_BITMAP_ARC_BOTTOM3}};
	UINT16 g_buttonArcBMPSETTINGID[2][4] = {{IDB_BITMAP_ARC_TOP4}, {IDB_BITMAP_ARC_BOTTOM4}};
	UINT16 g_buttonPlayBMPID[2][4] = {{IDB_BITMAP_PLAY_TOP1}, {IDB_BITMAP_PLAY_BOTTOM1}};
	UINT16 g_buttonSoundBMPID[2][4] = {{IDB_BITMAP_MP3SOUND_TOP1}, {IDB_BITMAP_MP3SOUND_BOTTOM1}};
	UINT16 g_buttonALLSelectBMPID[2][4] = {{IDB_BITMAP_MP3_SELECT_UP1}, {IDB_BITMAP_MP3_SELECT_DOWN1}};
	UINT16 g_buttonDialBMPID[2][4] = {{IDB_BITMAP_DIAL_UP1}, {IDB_BITMAP_DIAL_DOWN1}};
	UINT16 g_buttonRectBMPID[2][4] = {{IDB_BITMAP_RECT_UP1}, {IDB_BITMAP_RECT_DOWN1}};
	UINT16 g_buttonExitBMPID[2][4] = {{IDB_BITMAP_EXIT_UP1}, {IDB_BITMAP_EXIT_DOWN1}};
	UINT16 g_buttonExitBMP1ID[2][4] = {{IDB_BITMAP_EXIT1_UP1}, {IDB_BITMAP_EXIT1_DOWN1}};
	
	//进度条
	UINT16 g_progressPlayBMPID[2][4] = {{IDB_BITMAP_MP3PROGRESS_TOP1}, {IDB_BITMAP_MP3PROGRESS_BOTTOM1}};
	UINT16 g_progressSoundBMPID[2][4] = {{IDB_BITMAP_MP3VPROGRESS_TOP1}, {IDB_BITMAP_MP3VPROGRESS_BOTTOM1}};
	UINT16 g_progressBarBMPID[4] = {IDB_PROGRESS_BAR};
	COLORREF g_progressBackRGB[4] = {RGB(192, 192, 192)};
	COLORREF g_progressLeftRGB[4] = {RGB(0,  0, 0xFF)};
	COLORREF g_progressRightRGB[4] = {RGB(0xFF, 0xFF, 0xFF)};

	//主界面时间显示框
	UINT16   g_mainTimeBmpID[4] = {IDB_BITMAP_MAINTIME};
	COLORREF g_mainTimeFrameRGB[2][4] = {{RGB(33, 33, 33)}, {RGB(255, 255, 255)}};
	COLORREF g_mainTimeInFrameRGB[3][4] = {{RGB(90, 90, 90)}, {RGB(74, 74, 74)}, {RGB(107, 107, 115)}};

	//主界面文字背景图
	UINT16 g_mainTxtBmpID[4] = {IDB_BITMAP_MAIN_TXTBAR1};
	COLORREF g_mainTxtColor[3][4] = {{RGB(157, 23, 22)}, {RGB(16, 91, 6)}, {RGB(167, 1, 165)}};

	//主界面App ICON
	UINT16 g_mainAppVideoBmpID[4] = {IDB_BITMAP_MAINVIDEO};
	UINT16 g_mainAppPhotoBmpID[4] = {IDB_BITMAP_MAINPHOTO};
	UINT16 g_mainAppMp3BmpID[4] = {IDB_BITMAP_MAINMP3};
	UINT16 g_mainAppCandBmpID[4] = {IDB_BITMAP_MAINCAND};
	UINT16 g_mainAppCaluBmpID[4] = {IDB_BITMAP_MAINCALU};
	UINT16 g_mainAppSaveBmpID[4] = {IDB_BITMAP_MAINSAVE};

	//static border 颜色
	COLORREF g_staticLine1RGB[2][4] = {{RGB(8, 74, 90)}, {RGB(8, 74, 90)}};
	COLORREF g_staticLine2RGB[2][4] = {{RGB(16, 132, 173)}, {RGB(8, 123, 165)}};
	COLORREF g_staticLine3RGB[2][4] = {{RGB(16, 165, 222)}, {RGB(16, 132, 173)}};
	COLORREF g_staticAppTileBackRGB[4] = {{RGB(8, 148, 197)}};

	//listctrl 背景色
	COLORREF g_listctrlBackRGB1[4] = {{RGB(189, 206, 239)}};
	COLORREF g_listctrlBackRGB2[4] = {{RGB(214, 222, 247)}};
	UINT16   g_listctrlSelectBMPID[6][4] = {{IDB_BITMAP_SELECT1}, {IDB_BITMAP_UNSELECT1}, {IDB_BITMAP_LISTDIR}, {IDB_BITMAP_LISTCIPAN}, {IDB_BITMAP_LISTTOBOTTOM}, {IDB_BITMAP3_MUSIC}};

	//edit 背景色
	COLORREF g_editBackRGB[4] = {RGB(200, 206, 238)};
	COLORREF g_editLineRGB[4] = {RGB(165, 173, 222)};

	//combox 背景色
	COLORREF g_comboxBackRGB[4] = {{RGB(200, 206, 238)}};
	UINT16   g_comboxBMPID[2][4] = {{IDB_BITMAP_COMBOX_UP1}, {IDB_BITMAP_COMBOX_DOWN1}};

	//输入法背景图
	UINT16 g_inputEngBMPID[4] = {IDB_VOIPINPUTENG};
	UINT16 g_inputEng1BMPID[4] = {IDB_VOIPINPUTENG1};
	UINT16 g_inputPinyinBMPID[4] = {IDB_VOIPINPUTPINYIN};
	UINT16 g_inputWriteBMPID[4] = {IDB_VOIPINPUTH};
	UINT16 g_inputSysmolBMPID[4] = {IDB_VOIPINPUTFUHAO};

	//计算器softkey
	UINT16 g_caluSoftKeyBMP[4] = {IDB_BITMAP_CALCULATOR};

	//日历控件
	COLORREF g_lunarderTitleRGB[4] = {RGB(8, 197, 230)};
	COLORREF g_lunarderLineRGB[4] = {RGB(165, 173, 222)};
	COLORREF g_lunarderBackRGB[4] = {RGB(189, 206, 239)};
	COLORREF g_lunarderSecDayRGB[4] = {RGB(173, 239, 49)};
	COLORREF g_lunarderNoteDayRGB[4] = {RGB(255, 0, 0)};

	//日历+记事
	UINT16 g_lunarderNoteLeftBMPID[2][4] = {{IDB_BITMAP_LEFT_UP1}, {IDB_BITMAP_LEFT_DOWN1}};
	UINT16 g_lunarderNoteRightBMPID[2][4] = {{IDB_BITMAP_RIGHT_UP1}, {IDB_BITMAP_RIGHT_DOWN1}};
	UINT16 g_lunarderNoteUpBMPID[2][4] = {{IDB_BITMAP_UP_UP1}, {IDB_BITMAP_UP_DOWN1}};
	UINT16 g_lunarderNoteDownBMPID[2][4] = {{IDB_BITMAP_DOWN_UP1}, {IDB_BITMAP_DOWN_DOWN1}};

	void SkinResource::InitSkinStyle()
	{
		//滚动条
		/*
		g_scroolbarFrameRGB[0] = RGB(6, 70, 92);
		g_scroolbarMiddleLineRGB[0] = RGB(8, 92, 120);
		g_scroolbarBackRGB[0] = RGB(10, 112, 145);
		g_scroolbarUpArrowBmpID[0] = IDB_BITMAP_UPARROW;
		g_scroolbarDownArrowBmpID[0] = IDB_BITMAP_DOWNARROW;
        g_scroolbarThickBmpID[0] = IDB_BITMAP_THICK;
		*/
	}
	void SetSlinStyle(SKIN_STYLE style)
	{
		g_skinstyle = style;
	}
}