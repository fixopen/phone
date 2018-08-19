#include "FrameWnd.h"

FrameWnd::FrameWnd(void)
: windowType(0)
, help(NULL)
, pCurrentCtrl(NULL)
{
}

FrameWnd::~FrameWnd(void)
{
}

void New::FrameWnd::ProcessMessageMap(Message msg)
{
}

void New::FrameWnd::PaintTitle(CDC& rDC)
{
}

void New::FrameWnd::PaintControlBox(CDC& rDC)
{
}

void New::FrameWnd::PaintChildren(void)
{
}

void New::FrameWnd::DoPenDown(Point pt)
{
}

void New::FrameWnd::DoPenMove(Point pt)
{
}

void New::FrameWnd::DoPenUp(Point pt)
{
}

Control* New::FrameWnd::GetPenControl(Point pt)
{
	return NULL;
}

void New::FrameWnd::DestroyChildren(void)
{
}

bool New::FrameWnd::IsLegalWindow(void)
{
	return false;
}

void New::FrameWnd::WindowProcess(const unsigned int nMessage, const unsigned int wParam, const unsigned int lParam)
{
}

bool New::FrameWnd::Create(string name, const unsigned int style, const RECT& rect, Help* pHelp)
{
}

bool New::FrameWnd::OnCreate()
{
	return  TRUE;
}
void New::FrameWnd::Destroy()
{
}

bool New::FrameWnd::OnDestroy()
{
}

void New::FrameWnd::OnPaint(CDC& dc)
{
}

void New::FrameWnd::OnActive()
{
}

void New::FrameWnd::OnRtc(const unsigned int nRtcType)
{
}

void New::FrameWnd::OnDactive()
{
}

void New::FrameWnd::ShowWindow(const unsigned int nCmdShow = SW_SHOW)
{
}

void AddChild(Control* pChild)
{
}

bool DeleteChild(Control* pChild)
{
}

unsigned int GetWndType()
{
	return windowType;
}
unsigned int GetHelpID()
{
	return help;
}
