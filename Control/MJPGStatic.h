#if !defined(AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_)
#define AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MJPGStatic.h : header file
//

#include "../Util/config.h"
#include "../Util/xmlParser.h"

namespace Control {
    using namespace Util;
    /////////////////////////////////////////////////////////////////////////////
    // CMJPGStatic window
    struct MULTILINEINFO
    {
        int nStart;
        int nCount;
    };

    class CMJPGStatic : public CStatic
    {
    public:
        int m_nIsCrollIndex;			//在item中的序号
        int m_nIsCrollCount;			//可以滚动的个数
        void ScrollUnit(int upordown, int unitcount);

        CFont m_Font;
        int m_nIndexSelectUnit;
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

#endif // !defined(AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_)
