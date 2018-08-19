#pragma once

#include <string>
#include "Screen.h"

// CBackgroundDialog dialog

class CBackgroundDialog : public CDialog {
	DECLARE_DYNAMIC(CBackgroundDialog)

public:
	CBackgroundDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBackgroundDialog();
    void SetBackgroundDC(HDC const backDC) {
        backgroundDc_ = backDC;
    }
protected:
    void drawBackground_(std::wstring const& imageFilename = L"/FlashDrv/debug/NULL.jpg", SIZE const& imageSize = Screen::size());
    HBRUSH const controlColor_(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    HDC backgroundDc_;
    std::wstring backgroundImageFilename_;
    SIZE backgroundImageSize_;
    HDC thisDc_;

	DECLARE_MESSAGE_MAP()
};
