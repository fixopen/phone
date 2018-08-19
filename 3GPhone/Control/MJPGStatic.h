#ifndef __CONTROL_MJPGSTATIC_H__
#define __CONTROL_MJPGSTATIC_H__

//#include "../Util/config.h"
//#include "../Util/xmlParser.h"
#include <string>
#include <vector>

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CMJPGStatic window
    struct MULTILINEINFO
    {
        int nStart;
        int nCount;
    };

    enum Alignment {
        left_top,
        left_center,
        left_bottom,
        middle_top,
        middle_center,
        middle_bottom,
        right_top,
        right_center,
        right_bottom,
    };

    enum UnitType {
        unit_link,
        unit_click,
        unit_
    };

    struct UNIT {
        bool m_bIsShow;
        CWnd* m_pUnitWnd;
        bool m_bIsDownStatus;
        CString m_bgFilename;
        CString m_bgFilename_down;
        bool m_bIsTranslate;
        CRect m_Rect;
        int m_FontWeights;
        bool m_bFontItalic;
        bool m_bFontUnLine;
        //CFont m_Font;
        //CString m_Font;
        int m_Font;
        COLORREF m_FontColor;
        int m_FontHeight;
        CString m_sContent;
        int m_FontSize;
        Alignment m_FontAlign;
        int m_nIsScroll;
        bool m_bIsDisable;
        int m_UnitType;
        CString m_UnitContent;
        int m_nSiererNO;
    };

    struct MJPGItem {
        UNIT unitparam;
    };

    struct MJPGList {
        CString bgfilename_down;
        CString bgfilename;
        CRect m_Rect;
        std::vector<MJPGItem*> items;
    };

    struct TEXTCOLOR {
    };

    class CMJPGStatic : public CStatic
    {
    public:
        int m_nIsCrollIndex;			//在item中的序号
        int m_nIsCrollCount;			//可以滚动的个数
        void ScrollUnit(int upordown, int unitcount);

        CFont m_Font;
        int m_nIndexSelectUnit;
        CString bgfilename_down;
        CRect m_Rect;
        std::vector<MJPGItem*> items;
        MJPGList* m_currentMJPGList;
        MJPGList* m_oldMJPGList;
        CString m_oldLinkFile;
        CString ToFileDir(CString filename);

        CDC* m_pMemDC;
        CBitmap* m_pBmp;
        CBitmap* m_pOldbmp;
        void CreateMemDC();
        void ReleaseMemDC();

        int GetCStringMuliteLine(CString s, int height, CRect rt, MULTILINEINFO *pCount, int LineCount);
        void DrawMultiLine(CDC *pdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
        void DrawUnit(UNIT *pUnit, CDC *pdc);
        void DrawMJPGPage(CString sFile);
        int FindUnitIndex(CPoint pt);
        void InvertRect(CRect rt);
        void DrawUnitStatus(UNIT *pUnit, int nUintStatus);

        CRect m_rtMJPG;
        CRect m_ClientRect;
        void SetMJPGRect(CRect rt) {
            m_rtMJPG = rt;
            m_ClientRect = CRect(0, 0, rt.Width(), rt.Height());
        }
        CString m_sCurrentLinkFile;
        void SetCurrentLinkFile(CString s);
        void GetCurrentLinkFile(CString &s) {
            s = m_sCurrentLinkFile;
        }

        BOOL SetUnitIsDownStatus(int UnitNO, BOOL isDown);
        BOOL SetUnitIsShow(int UnitNO, BOOL isShow, BOOL isInvalidate = FALSE);

        BOOL SetUnitBitmap(int UnitNO, CString filename_up, CString filename_down, BOOL isDraw, BOOL isInvalidate = FALSE);
        BOOL SetUnitText(int UnitNO, CString text, BOOL isDraw, BOOL isInvalidate = FALSE);
        BOOL SetUnitColor(int UnitNO, TEXTCOLOR cl, BOOL isDraw, BOOL isInvalidate = FALSE);
        BOOL SetUnitIsTranslate(int UnitNO, BOOL isTrans);
        BOOL SetUnitIsDisable(int UnitNO, BOOL isDisable);
        BOOL GetUnitIsShow(int UnitNO);
        BOOL GetUnitIsDownStatus(int UnitNO);
        // Construction
    public:
        CMJPGStatic();
        virtual ~CMJPGStatic();

        // Generated message map functions
    protected:
        //{{AFX_MSG(CMJPGStatic)
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnPaint();
        virtual void PreSubclassWindow();
        afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        //}}AFX_MSG
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif //__CONTROL_MJPGSTATIC_H__
