#if !defined(AFX_SOFTKEY_H__91DDED4E_D3EE_46B3_9D52_4474B1A696E5__INCLUDED_)
#define AFX_SOFTKEY_H__91DDED4E_D3EE_46B3_9D52_4474B1A696E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoftKey.h : header file
//
///////////////////////////////////
/*��д�Ǳ��
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
    //�ӿ�˵��:
    //    Create        ����
    //    SetParameter  ����ʱ�ı����ӵĽӿ�
    //    �򸸴��ڷ���  WM_KEY��Ϣ

    struct KEYSTRUCT
    {
        CRect    sKeyRect;
        UINT16   nKeyValue;
        short	 nTextId;			//��¼�����ַ�����ID ȱʡΪ0 ����û�б���
    };

    class CSoftKey : public CStatic
    {
    public:
        CFont       m_font;
        KEYSTRUCT * m_pKeys;
        UINT32      m_nKeyCount;
        INT32       m_nCurSel;
        int			m_nLocal;   //��¼��ǰ��key
        CPoint		m_pPoint;	//��¼ͼ���λ��

        UINT32		m_BitmapID;

    private:
        BOOL		m_bHaveHandWrite;	//�Ƿ�����д����
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

        BOOL Create(int			iconNO,       //����̵�ͼ��ID
            CPoint		point,			//ͼ���λ��
            UINT32      nKeyCount,      //����̰�������
            CRect     * pKeyRectArray,  //ÿ�������ľ���
            UINT16    * pKeyValueArray, //ÿ�������ļ�ֵ 
            CRect		&rect,
            CWnd* pParentWnd,          //������ָ��
            int style,					//���
            UINT32      nCtrlId);       //����̵Ŀؼ�ID
        void SetParameters(int iconNO,                //ͼ�� ID
            UINT16 *pKeyValueArray = NULL,  //ÿ�������ļ�ֵ
            BOOL bRedraw = FALSE);          //�Ƿ��ػ�
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
        UINT16	 nTextId;			//��¼�����ַ�����ID ȱʡΪ0 ����û�б���
        UINT16	 nBitmapId;
        UINT8	 nAlign;	// ���nBitmapId��Ϊ0����DT_LEFT ͼ���������  DT_RIGHT ͼ�������ұ� DT_TOP ͼ�������ϱ� DT_BOTTOM ͼ�������±ߡ�������С� 
        BOOL	 bToggle;
        BOOL	 bEnable;
    };

#define IMAGE_LEFT		1
#define IMAGE_RIGHT		2
#define IMAGE_TOP		3
#define IMAGE_BOTTOM	4
    //����״̬���������������е�Ӧ�ó���
    class CCEIconSoftKey : public CStatic
    {
    public:
        ICONKEYSTRUCT * m_pKeys;
        UINT8			m_nKeyCount;
        char			m_nCurSel;
        UINT8			m_nOldCurSel;
        int				m_nToggle;   //��¼��ǰToggle
        CPoint			m_pPoint;	//��¼ͼ���λ��
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

        BOOL Create(int	iconNO,       //����̵�ͼ��ID
            int			toggleIconNo, //toggleͼ��
            int			nToggleKey,   //�ڼ�����toggle
            CPoint		point,			//ͼ���λ��
            UINT32      nKeyCount,      //����̰�������
            ICONKEYSTRUCT	*softkeyval,	//softkey ����
            CRect		&rect,				//�����С
            CWnd* pParentWnd,          //������ָ��
            int style,					//���
            UINT32      nCtrlId);       //����̵Ŀؼ�ID

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
