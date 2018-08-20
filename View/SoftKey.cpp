#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SoftKey.h"

namespace View
{
	SoftKey::SoftKey(CWnd* pParent /*=NULL*/)
		: CDialog(SoftKey::IDD, pParent)
	{
		//{{AFX_DATA_INIT(SoftKey)
			// NOTE: the ClassWizard will add member initialization here
		//}}AFX_DATA_INIT
	}

    void SoftKey::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(SoftKey)
			// NOTE: the ClassWizard will add DDX and DDV calls here
		//}}AFX_DATA_MAP
	}

	void SoftKey::RegisterProc(CRect bounds, DataType code, Proc proc)
	{
		regionMap_[std::make_pair(bounds, code)] = proc;
	}

    void SoftKey::Show(Position position)
    {
        int x = 0, y = 0, cx = 0, cy = 0, flags = 0;
        switch (position)
        {
        case dUp:
            y = 0;
            break;
        case dDown:
            y = 0;
            break;
        }
        ::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, cx, xy, flags);
        ShowWindow(SW_SHOW);
    }

	bool SoftKey::PointInRect_(CRect const& bounds, CPoint const& point) const
	{
        return ::PtInRect(&bounds, point);
        /*
		return (bounds.left < point.x && point.x < bounds.right)
			&& (bounds.top < point.y && point.y < bounds.bottom);
        */
	}

	void SoftKey::InvertRect_(CRect const& bounds)
	{
		//invert rectangle
		//CDC dc(this);
		//dc.InvertRect(&bounds);
	}

    void SoftKey::Hide_(DataType)
    {
        ShowWindow(SW_HIDE);
    }

    void SoftKey::UpDown_(DataType position)
    {
        if (currentPosition_ != position)
        {
            currentPosition_ = position;
            Hide_(0);
            SoftKey::Instance()->Show(direction);
        }
    }

    void SoftKey::SwitchPanel_(DataType panelId)
    {
        if (currentRegionMap != panelMap_[panelId])
        {
            currentRegionMap = panelMap_[panelId];
            HDC memDC = ::CreateCompatibleDC(pdc);
            HBITMAP bitmap = ::LoadBitmap(::AfxGetResourceHandle(), MAKEINTRESOURCE(panelId));
            CRect bounds;
            ::GetClientRect(SoftKey::Instance()->m_hWnd, bounds);
            HBITMAP oldBitmap = (HBITMAP)::SelectObject(memDC, bitmap);
            ::BitBlt(pdc,  x,  y,  bounds.Width (), bounds.Height(),  memDC, bounds.left, bounds.top, SRCCOPY);
            ::SelectObject (memDC, oldBitmap);
            //::DeleteBitmap(bitmap);
        }
    }

    void SoftKey::DrawTrack_(DataType points)
    {
        //draw pen move track
    }

    void SoftKey::DrawChinese_(DataType chinese)
    {
        //draw chinese to chinese tips area
    }

	void SoftKey::OnLButtonDown(UINT nFlags, CPoint point) 
	{
		// TODO: Add your message handler code here and/or call default
        unsigned int const messageId = 0; //SOFTKEY_PRESS;
		for (std::map<std::pair<CRect, DataType>, Proc>::iterator i = regionMap_.begin(); i != regionMap_.end(); ++i)
		{
			if (PointInRect_(i->first.first, point))
			{
                currentRegion_ = i;
				InvertRect_(i->first.first);
				break;
			}
		}
		CDialog::OnLButtonDown(nFlags, point);
	}

	void SoftKey::OnLButtonUp(UINT nFlags, CPoint point) 
    {
        if (currentRegion_)
        {
            if (destination_)
            {
                //send message
                ::PostMessage(destination_->m_hWnd, messageId, currentRegion_->first.second, 0); //wParam, lParam
                InvertRect_(currentRegion_->first.first);
                //post process
                currentRegion_->second(currentRegion_->first.second);
                destination_->SetFocus();
                destination_->Invalidate();
            }
            currentRegion_ = 0;
        }
    }

	void SoftKey::OnMouseMove(UINT nFlags, CPoint point)
    {
        if (isHandDraw_)
        {
            if (mouseLButtonDown)
            {
                if (inHandDrawArea_(point))
                {
                    drawTrack_();
                }
            }
        }
    }

    int SoftKey::OnCreate(LPCREATESTRUCT lpCreateStruct) 
    {
    	if (CDialog::OnCreate(lpCreateStruct) == -1)
    		return -1;
    	// TODO: Add your specialized creation code here
        destination_ = 0;
        //for any panel
        RegisterProc(bounds, panelId, switchPanel_);
        RegisterProc(bounds, panelId, switchPanel_);
        RegisterProc(bounds, panelId, switchPanel_);
        RegisterProc(bounds, panelId, switchPanel_);

        RegisterProc(bounds, direction, UpDown_);
        RegisterProc(bounds, 0, Hide_);
        RegisterProc(bounds, chinese, DrawChinese_);
        RegisterProc(bounds, points, DrawTrack_);
    	return 0;
    }

	BEGIN_MESSAGE_MAP(SoftKey, CDialog)
		//{{AFX_MSG_MAP(SoftKey)
		ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_MOUSEMOVE()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()
}

