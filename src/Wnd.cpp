#include "wnd.h"
using namespace New;

Wnd::Wnd(void)
: prev(NULL)
, next(NULL)
, parent(NULL)
, children(NULL)
, rect(NULL)
, style(0)
{
}

Wnd::~Wnd(void)
{
}

Wnd* New::Wnd::get_next(void) const
{
	return NULL;
}

RECT New::Wnd::get_rect(void) const
{
	return RECT();
}

unsigned int New::Wnd::get_style(void) const
{
	return 0;
}

void Wnd::set_style(const unsigned int style)
{
}

wstring Wnd::get_text(void) const
{
	return wstring();
}

void Wnd::set_text(const wstring style)
{
}

void Enable(bool bEnable = true)
{
	if(bEnable)
        style &= ~WS_DISABLE;
	else
		style |= WS_DISABLE;
}

void Visible(bool bVisible = true)
{
	if(bVisible)
		style |= WS_VISIBLE;
	else
		style &= ~WS_VISIBLE;
}

void New::Wnd::sendMessage(MessageArgs* msg)
{
}

void New::Wnd::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
}

void New::Wnd::OnPaint(CDC &dc)
{
}
