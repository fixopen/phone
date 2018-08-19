#pragma once
#include "stdafx.h"
typedef enum ImgFormat
{
	RGB565,
	RGB888,
	YUV420P,
	YUV422P,
	YUV422

};
class Display
{
public:
	Display(void);
	~Display(void);
	
	//void SetPara(unsigned char *Rgb24Data,int Video_Width,int Video_Height,HWND hVideoWnd);
	bool SetPara(HWND hVideoWnd);
	bool ShowPic(unsigned int uiVideo_Width,unsigned int uiVideo_Height,unsigned char *Videoata,bool IsRGB565);
	bool DisplayInit();
	bool DisplayDeInit();
	
private:
	unsigned int uiWinWidth;
	unsigned int uiWinHeight;
	ImgFormat	 ImgFormat;
	CWnd* pWnd;
};
