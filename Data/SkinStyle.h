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
#define		SKIN_STYLE_MAX		4
namespace Data
{
	//记住皮肤的风格 
	//color define
	enum SKIN_STYLE{
		skin_blue, 
			skin_red, 
			skin_yellow, 
			skin_white
	};

	extern SKIN_STYLE g_skinstyle;
	
	//滚动条
	extern COLORREF g_scroolbarFrameRGB[];
	extern COLORREF g_scroolbarMiddleLineRGB[];
	extern COLORREF g_scroolbarBackRGB[];
	extern UINT16   g_scroolbarUpArrowBmpID[]; 
	extern UINT16   g_scroolbarDownArrowBmpID[];
	extern UINT16   g_scroolbarThickBmpID[];

	//主界面状态条
	extern COLORREF g_mainstatusBackRGB[];
	extern UINT16	g_mainstatusMainBmpID[];
	extern UINT16	g_mainstatusToggleBmpID[];
	extern UINT16	g_mainstatusIMEBmpID[];
	extern UINT16	g_mainstatusTELBmpID[];
	extern UINT16	g_mainstatusDesktopBmpID[];
	extern UINT16	g_mainstatusCardBmpID[];
	extern UINT16	g_mainstatusCalllistBmpID[];
	extern UINT16	g_mainstatusRecordBmpID[];
	extern UINT16	g_mainstatusInfoBmpID[];
	extern UINT16	g_mainstatusSettingBmpID[];

	//全屏窗口背景框
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

	//非全屏窗口背景框
	extern COLORREF g_partFrameTitle1TopRGB[4][4];
	extern COLORREF g_partFrameTitle1BottomRGB[4][4];
	extern COLORREF g_partFrameTitle1LeftRGB[4][4];
	extern COLORREF g_partFrameTitle1RightRGB[4][4];
	extern COLORREF g_partFrameTitle1BackRGB[4];
	
	extern COLORREF g_partFrameTitle2TopRGB[4][4];
	extern COLORREF g_partFrameTitle2BottomRGB[4][4];
	extern COLORREF g_partFrameTitle2RightRGB[4][4];
	extern COLORREF g_partFrameTitle2BackRGB[4];
	extern COLORREF	g_partFrameTitle2LineRGB[2][4];
	
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

	//按钮图标
	extern UINT16 g_buttonArcBMPID[2][4];// = {{}, {}};
	extern UINT16 g_buttonPlayBMPID[2][4];
	extern UINT16 g_buttonSoundBMPID[2][4];

	//进度条
	extern UINT16 g_progressPlayBMPID[2][4];
	extern UINT16 g_progressSoundBMPID[2][4];
	extern UINT16 g_progressBarBMPID[4];
	extern COLORREF g_progressBackRGB[4];
	extern COLORREF g_progressLeftRGB[4];
	extern COLORREF g_progressRightRGB[4];

	//主界面时间显示框
	extern UINT16	g_mainTimeBmpID[4];
	extern COLORREF g_mainTimeFrameRGB[2][4];
	extern COLORREF g_mainTimeInFrameRGB[3][4];

	//主界面文字背景图
	extern UINT16 g_mainTxtBmpID[4];
	extern COLORREF g_mainTxtColor[3][4];

	//static border 颜色
	extern COLORREF g_staticLine1RGB[2][4];
	extern COLORREF g_staticLine2RGB[2][4];
	extern COLORREF g_staticLine3RGB[2][4];
	
	class SkinResource
	{
	
		static void InitSkinStyle();
		static void SetSlinStyle(SKIN_STYLE style);

	};
}
#endif