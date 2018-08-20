#ifndef __VIEW_SOFTKEY_H__
#define __VIEW_SOFTKEY_H__

#pragma warning(disable: 4786)

#include <map>

inline bool operator<(CRect const& lhs, CRect const& rhs)
{
	if (lhs.left < rhs.left || lhs.top < rhs.top)
		return true;
	else
		return false;
}

namespace View
{
    enum Position
    {
        pUp,
        pDown,
    };

    //panel type is panel-id and image-id and message-id
    enum PanelType
    {
        ptSymbol,
        ptEnglish,
        ptPinyin,
        ptHandDraw,
    };

	class SoftKey : public CDialog
	{
	// Construction
	public:
		typedef unsigned int DataType;
		typedef void (*Proc)(DataType);
        typedef std::map<std::pair<CRect, DataType>, Proc> RegionMap;
        typedef std::map<PanelType, RegionMap> PanelMap;
		SoftKey(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(SoftKey)
		enum { IDD = IDD_DIALOG_SOFTKEY };
			// NOTE: the ClassWizard will add data members here
		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(SoftKey)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Function
	public:
        static shared_ptr<SoftKey> Instance()
        {
            static shared_ptr<SoftKey> result(new SoftKey);
            return result;
        }
		void RegisterProc(CRect bounds, DataType code, Proc proc);
		void SetDestination(CWnd* destination)
		{
			destination_ = destination;
		}
        void Show(Position position);
	// Implementation
	private:
        bool isHandDraw_;
        CRect handDrawArea_[2];

        bool isChineseInput; //for pinyin or handdraw
        CRect chineseArea_;
        CRect previousPageArea_;
        CRect nextPageArea_;
        CRect legendArea_;

        CRect switchArea_[4];
        CRect upOrDownArea_;
        CRect hideArea_;

        RegionMap currentRegionMap_;
        RegionMap::iterator currentRegion_;
        static PanelMap panelMap_;
		CWnd* destination_;
		bool PointInRect_(CRect const& bounds, CPoint const& point) const;
		void InvertRect_(CRect const& bounds);
        static void Hide_(DataType);
        static Position currentPosition_;
		static void UpDown_(DataType position);
        static void SwitchPanel_(DataType panelId);
        static void DrawTrack_(DataType points);
        static void DrawChinese_(DataType chinese);
	protected:
		// Generated message map functions
		//{{AFX_MSG(SoftKey)
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};
}

#endif //__VIEW_SOFTKEY_H__
