#pragma once
#include "Wnd.h"
#include "Ctrl.h"
using namespace New;
#pragma once

namespace New
{
class Help
{
};

class FrameWnd :
	public Wnd
{
public:
	FrameWnd(void);
	virtual ~FrameWnd(void);
private:
	unsigned int windowType;
	Help* help;
	Ctrl* pCurrentCtrl;
protected:
	void ProcessMessageMap(Message msg);
	virtual void PaintTitle(CDC& rDC);
	void PaintControlBox(CDC& rDC);
	void PaintChildren(void);
	void DoPenDown(Point pt);
	void DoPenMove(Point pt);
	void DoPenUp(Point pt);
public:
	Control* GetPenControl(Point pt);
	void DestroyChildren(void);
	bool IsLegalWindow(void);
	virtual void WindowProcess(const unsigned int nMessage, const unsigned int wParam, const unsigned int lParam);
	virtual BOOL Create(string name, const unsigned int style, const RECT& rect, Help* pHelp);
	virtual BOOL OnCreate();
	virtual void Destroy();
	virtual BOOL OnDestroy();
	virtual void OnPaint(CDC& dc);
	virtual void OnActive();
	virtual void OnRtc(const unsigned int nRtcType);
	virtual void OnDactive();
	virtual void ShowWindow(const unsigned int nCmdShow = SW_SHOW);
protected:
	void AddChild(Control* pChild);
	bool DeleteChild(Control* pChild);
	unsigned int GetWndType();
	unsigned int GetHelpID();
public:
	DECLARE_MESSAGE_MAP(FrameWnd)
};
}