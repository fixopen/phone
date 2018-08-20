#if !defined(AFX_SOFTKEY_H__91DDED4E_D3EE_46B3_9D52_4474B1A696E5__INCLUDED_)
#define AFX_SOFTKEY_H__91DDED4E_D3EE_46B3_9D52_4474B1A696E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoftKey.h : header file
//
///////////////////////////////////
/*手写是别库
*/

#include "../inc/HWAPI.H"
namespace Control {
#define WRITE_LINE_BUFFMAX		10
#define WRITE_LINE_POINTMAX		1024*4
    struct WriteLineBuff
    {
        THWPoint pt[WRITE_LINE_POINTMAX];
    };

    /////////////////////////////////////////////////////////////////////////////
    // CSoftKey window
#define WM_SOFTKEY		(WM_USER+200)
#define WM_SOFTKEY_REG  (WM_SOFTKEY+1)

#define MAX(x, y) (x>y?x:y)
#define MIN(x, y) (x<y?x:y)

    typedef unsigned short UINT16;
    typedef unsigned char UINT8;

    struct NMHDR_ 
    {
        CWnd			*pWndFrom; 
        int				code; 
        unsigned long	wParam;
        unsigned long	lParam;
    }; 

    ///////////////////////////////////////////////////////////////////////////////
    //接口说明:
    //    Create        创建
    //    SetParameter  运行时改变样子的接口
    //    向父窗口发送  WM_KEY消息

    struct KEYSTRUCT
    {
        CRect    sKeyRect;
        UINT16   nKeyValue;
        short	 nTextId;			//记录标题字符串的ID 缺省为0 表明没有标题
    };

    class CSoftKey : public CStatic
    {
    public:
        CFont       m_font;
        KEYSTRUCT * m_pKeys;
        UINT32      m_nKeyCount;
        INT32       m_nCurSel;
        int			m_nLocal;   //记录当前的key
        CPoint		m_pPoint;	//记录图标的位置

        UINT32		m_BitmapID;

    private:
        BOOL		m_bHaveHandWrite;	//是否有手写功能
        int			m_nHWPenDown;
        int			m_nOldHWPenDown;
        CRect		m_HWRect[2];
        CPoint		m_OldPoint;
        CPoint      m_PrePoint[3];
        int			m_RegTimer;

        CPoint		DoWithWrite(CPoint pt);
        int			FindHWRect_(CPoint pt);

        // Construction
    public:
        CSoftKey();

        // Attributes
    public:

        BOOL Create(int			iconNO,       //软键盘的图标ID
            CPoint		point,			//图标的位置
            UINT32      nKeyCount,      //软键盘按键个数
            CRect     * pKeyRectArray,  //每个按键的矩形
            UINT16    * pKeyValueArray, //每个按键的键值 
            CRect		&rect,
            CWnd* pParentWnd,          //父窗口指针
            int style,					//风格
            UINT32      nCtrlId);       //软键盘的控件ID
        void SetParameters(int iconNO,                //图标 ID
            UINT16 *pKeyValueArray = NULL,  //每个按键的键值
            BOOL bRedraw = FALSE);          //是否重绘
        void SetSoftKeyRect(CRect *pKeyRectArray, int x, int y);

        INT32   GetPenKey(CPoint &pt);
        void	SetHandWriteRect(CRect rect1, CRect rect2);
        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSoftKey)
    protected:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CSoftKey();

        // Generated message map functions
    protected:
        //{{AFX_MSG(CSoftKey)
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnPaint();
        afx_msg void OnTimer(UINT nIDEvent);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
    };


    struct ICONKEYSTRUCT
    {
        CRect    sKeyRect;
        UINT16   nKeyValue;
        UINT16	 nTextId;			//记录标题字符串的ID 缺省为0 表明没有标题
        UINT16	 nBitmapId;
        UINT8	 nAlign;	// 如果nBitmapId不为0，则DT_LEFT 图标在最左边  DT_RIGHT 图标在最右边 DT_TOP 图标在最上边 DT_BOTTOM 图标在最下边。否则居中。 
        BOOL	 bToggle;
        BOOL	 bEnable;
    };

#define IMAGE_LEFT		1
#define IMAGE_RIGHT		2
#define IMAGE_TOP		3
#define IMAGE_BOTTOM	4
    //用于状态任务栏，主界面中的应用程序
    class CCEIconSoftKey : public CStatic
    {
    public:
        ICONKEYSTRUCT * m_pKeys;
        UINT8			m_nKeyCount;
        char			m_nCurSel;
        UINT8			m_nOldCurSel;
        int				m_nToggle;   //记录当前Toggle
        CPoint			m_pPoint;	//记录图标的位置
        UINT32		m_BitmapToggleID;
        UINT32		m_BitmapID;
        COLORREF	m_clrBack;
        void SetBackRGB(COLORREF rgb){m_clrBack = rgb;}
        void SetKeyText(CString s, int nkey, COLORREF cr);
        void    SetLeftTimer();
        void SetEnableKey(int nKey, BOOL flag){m_pKeys[nKey].bEnable = flag;};

    private:
        int			FindHWRect_(CPoint pt);

        // Construction
    public:
        CCEIconSoftKey();

        // Attributes
    public:

        BOOL Create(int	iconNO,       //软键盘的图标ID
            int			toggleIconNo, //toggle图标
            int			nToggleKey,   //第几个被toggle
            CPoint		point,			//图标的位置
            UINT32      nKeyCount,      //软键盘按键个数
            ICONKEYSTRUCT	*softkeyval,	//softkey 参数
            CRect		&rect,				//区域大小
            CWnd* pParentWnd,          //父窗口指针
            int style,					//风格
            UINT32      nCtrlId);       //软键盘的控件ID

        INT32   GetPenKey(CPoint &pt);
        INT8	GetToggleKey();
        void    SetToggleKey(int nIndex){m_nToggle = nIndex; Invalidate();}

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSoftKey)
    protected:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CCEIconSoftKey();

    protected:
        //{{AFX_MSG(CCEIconSoftKey)
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnPaint();
        afx_msg void OnTimer(UINT nIDEvent);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_SOFTKEY_H__91DDED4E_D3EE_46B3_9D52_4474B1A696E5__INCLUDED_)
