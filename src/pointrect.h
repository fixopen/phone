/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             PointRect.h                                                    */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             CPoint类的声明及定义                                           */
/*             CRect类的声明及定义                                            */
/*                                                                            */
/*  设计说明                                                                  */
/*             重载一些操作符方便点和矩形的使用                               */ 
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#ifndef __E201_POINTRECT_H_
#define __E201_POINTRECT_H_
#include "hwfcl.h"


/*============================================================================*/
//  CRect类定义                                                               //
/*============================================================================*/
struct CRect
{

   INT16 left,top,right,bottom;
   //构造函数
   CRect(){}
   
   CRect(INT16 x1, INT16 y1, INT16 x2, INT16 y2)
   {
       left   = x1;
       top    = y1;
       right  = x2;
       bottom = y2;
   }

   //操作符
   BOOL operator==(const INT16 x) const
   {
       return (left == x && right == x && top == x && bottom == x);
   }

   BOOL operator!=(const INT16 x) const
   {
       return !(*this == x);
   }

   BOOL operator==(const CRect &srcRect) const
   {
       return (left == srcRect.left && top == srcRect.top && 
                       right == srcRect.right && bottom == srcRect.bottom); 
   }

   BOOL operator!=(const CRect &srcRect) const
   {
       return  !(*this == srcRect);
   }
   
   CRect operator=(const INT16 x)
   {
       left = right = top = bottom = x;
	   return *this;
   }

   CRect operator=(const CRect & srcRect)
   {
       left   = srcRect.left; 
       top    = srcRect.top;
       right  = srcRect.right; 
       bottom = srcRect.bottom;  
	   return *this;
   }

  void SetRect(INT16 nleft, INT16 ntop, INT16 nright, INT16 nbottom)
  {
     left   = nleft;
	 top    = ntop;
	 right  = nright;
	 bottom = nbottom;
  }
  
  void MoveTo(INT16 nleft, INT16 ntop)
  {
	  INT16 width = Width();
	  INT16 height = Height();
      left = nleft;	  top  = ntop;
	  right  = left + width  - 1;
	  bottom = top  + height - 1;
  }

  void   InflateRect(INT16 x, INT16 y)
  {
	  left -= x; right += x; top -= y; bottom += y;
  }

  
  void   DeflateRect(INT16 x, INT16 y)
  {
	  InflateRect(-x, -y); 
  }
    
  INT16 Width()  const
  {
       return (right - left + 1);
  }

  INT16 Height() const
  {
       return (bottom - top + 1);
  }
  INT16 HCenter() const
  {
	  return (left + right)>>1;
  }
  INT16 VCenter() const
  {
	  return (top + bottom)>>1;
  }
};

/*============================================================================*/
//  CPoint类定义                                                              //
/*============================================================================*/
struct CPoint
{
  INT16 x, y;

  CPoint(){}

  CPoint(INT16 iX, INT16 iY)
  {
      x = iX; y = iY;
  }

  //操作符
  BOOL operator ==(const CPoint &sPoint) const
  {
	  return (x == sPoint.x && y == sPoint.y);
  }

  BOOL operator !=(const CPoint &sPoint) const
  {
	  return !((*this) == sPoint);
  }

  CPoint operator=(const CPoint & srcPoint)
  {
       x = srcPoint.x;
       y = srcPoint.y; 
	   return *this;
  }

  BOOL InRect(const CRect sRect) const
  {
	  return x >= sRect.left && x <= sRect.right && y>= sRect.top && y<= sRect.bottom;
  }
 
  BOOL InRect(INT16 left, INT16 top, INT16 right, INT16 bottom)const
  {
	  return x >= left && x <= right && y >= top && y<= bottom;
  }
};


#endif
// end PointRect.h
////////////////////////////////////////////////////////////////////////////////