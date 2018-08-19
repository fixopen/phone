#pragma once

#include <string>
// ImageViewer view

class ImageViewer : public CView {
    DECLARE_DYNCREATE(ImageViewer)

protected:
    ImageViewer();           // protected constructor used by dynamic creation
    virtual ~ImageViewer();

public:
    enum SizeStyle {
        ssFit,
        ssOrigin,
    };
    enum RenderStyle {
        rsBit,      //Ω•œ÷
        rsShutters, //∞Ÿ“∂¥∞
        rsZoom,     //Àı∑≈
    };
    enum Type {
        tUnknown,
        tBmp,
        tGif,
        tJpeg,
        tPng,
        tTiff,
        tJpeg2000,
    };
    void SetImage(std::wstring const & filename, Type const type = tUnknown);
    void SetImageRegion(CPoint const & origin, CSize const & size);
    void SetEnterMode(RenderStyle const renderMode);
    void SetLightness(unsigned int const lightness);
    void SetContract(unsigned int const contract);
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
    virtual void AssertValid() const;
#ifndef _WIN32_WCE
    virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
    DECLARE_MESSAGE_MAP()
};


