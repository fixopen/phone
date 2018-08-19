#pragma once
#include "Object.h"
using namespace New;

#include <vector>
#include <string>
using namespace std;

namespace New
{
struct RECT
{
	int top;
	int left;
	int height;
	int width;
};

class Wnd :
	public Object
{
public:
	Wnd(void);
	virtual ~Wnd(void);
private:
	Wnd* prev;
	Wnd* next;
	Wnd* parent;
	vector<Wnd*> children;
	RECT rect;
	unsigned int style;
	wstring text;
public:
	Wnd* get_next(void) const;
	RECT get_rect(void) const;
	unsigned int get_style(void) const;
	void set_style(const unsigned int style);
	wstring get_text(void) const;
	void set_text(const wstring style);
	void Enable(bool bEnable = true);
	void Visible(bool bVisible = true);
	void sendMessage(MessageArgs* msg);
    virtual void WindowProcess(const unsigned int nMessage, const unsigned int wParam, const unsigned int lParam);
	virtual void OnPaint(CDC& dc);
};
}