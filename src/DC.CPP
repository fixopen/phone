////////////////////////////////////////////
//    2001/12/06  hqf 修改HwPutString()    
//
//	byw, 2001/12/23
//		修改 CDC::DrawText() 函数
//
//////////////////////////////////////////// 
#include "dc.h"
#include "lzw.h"
#include "datadrv.h"
 
    /////////////////////////////////////////////////////////////////////
    //                                                                 //
    //           第一部分，底层做图函数                                //
    //                                                                 //
    //                                                                 //
    /////////////////////////////////////////////////////////////////////
  
 
/////////////////////////////////////////////////////////////////////
//
//  下面的底层函数需要用到的宏定义
//

#ifndef ISCC
#define  ISCC(ch)    ((ch)> 0xa0)
#endif

#ifndef SWAP
#define  SWAP(a, b)  {(a) = (a)^(b), (b)=(b)^(a), (a)=(a)^(b);}
#endif
 
//
#ifdef CIRCUMGYRATE
	//旋转情况下定义
	#define LCD_PHYWIDTH  LCD_HEIGHT
	#define LCD_PHYHEIGHT LCD_WIDTH

	#define ASC_PHY_WIDTH   16                //ASC字符的显示宽度
	#define ASC_PHY_HEIGHT  8

#else

	#define LCD_PHYWIDTH  LCD_WIDTH
	#define LCD_PHYHEIGHT LCD_HEIGHT

	#define ASC_PHY_WIDTH   8                //ASC字符的显示宽度
	#define ASC_PHY_HEIGHT  16

#endif 

void LTOP(INT16 &x, INT16 &y)
{
#ifdef CIRCUMGYRATE
  //逆时针旋转270度
  INT16 tmp = x;
  x = y;
  y = LCD_WIDTH - tmp -1;

  //逆时针旋转90度
  // tmp = x;
  // x = LCD_HEIGHT - y -1;
  // y = tmp;
  
#endif
}

void LTOP(INT16 &x0, INT16 &y0, INT16 &x1, INT16 &y1)
{ 
#ifdef CIRCUMGYRATE
	//逆时针旋转270度
	INT16 tmp =x0;
	x0 = y0;  
	y0 = LCD_WIDTH - tmp -1;
   
	tmp = x1;
	x1 = y1;
	y1 = LCD_WIDTH - tmp -1;

#endif
}
/////////////////////////////////////////////////////////////////////
//
//  下面的底层函数需要用到的全局量
// 
static UINT8  * g_pLcdBuf[LCD_HEIGHT];
const  UINT16   g_Pen16[]={0xFFFF, 0xAAAA, 0x5555, 0x0000};
const  UINT32   g_Pen32[]={0xFFFFFFFF, 0xAAAAAAAA, 0x55555555, 0x0000};
const  UINT8    g_Pen8[] = {0x0, 0x55, 0xAA, 0xFF};
const  UINT8    HeadMask[4] = {0x00, 0xC0, 0xF0, 0xFC};
const  UINT8    TailMask[4] = {0x3F, 0x0F, 0x03, 0x00};

//static UINT8    g_pVirtualScreenBuf[LCD_HEIGHT*LCD_WIDTH/4];

extern const  UINT8 g_iTwoBit2OneBitHeight[256];
extern const  UINT8 g_iTwoBit2OneBitLow[256];
extern const  UINT16 g_iOneBit2TwoBitMap[256];


/////////////////////////////////////////////////////////////////////
//
//下面所有的绘图函数中除注明入口为物理坐标系统的外其余全部为逻辑坐标系统
//
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//  void *  pSrcAddr;               // 源地址
//  UINT16  iSrcFrmWidthBytes;      // 源框架宽度（字节单位）
//  UINT16  x0;                     // 源位图横坐标（象素单位）
//  UINT16  y0;                     // 源位图纵坐标（象素单位）
//  UINT16  iWidth;                 // 位图宽度（象素单位）
//  UINT16  iHeight;                // 位图高度（象素单位）
    
//  void *  pDesAddr;               // 目的地址
//  UINT16  iDesFrmWidthBytes;      // 目的框架宽度（字节单位）
//  UINT16  x1;                     // 目的位图横坐标（象素单位）
//  UINT16  y1;                     // 目的位图纵坐标（象素单位）
//  UINT8   iDraw                   // 写数据的模式
//入口参数坐标系统为物理坐标系统
//  已知BUG:   当宽度小于4时会有问题
extern "C" void HwBitBlt(void * pSrcAddr, UINT16 iSrcFrmWidthBytes, UINT16 x0, UINT16 y0, UINT16 iWidth, UINT16 iHeight,\
                 void * pDesAddr, UINT16 iDesFrmWidthBytes, UINT16 x1, UINT16 y1, UINT8 iDrawMode)
{
        
    UINT8 * from = (UINT8*)(pSrcAddr + (iSrcFrmWidthBytes * y0) + (x0 >> 2));
    UINT8 * to = (UINT8*)(pDesAddr + (iDesFrmWidthBytes * y1) + (x1 >> 2));
    UINT8   bit0 = x0&3, bit1 = x1&3;
    UINT16  bw1  = ((x1 + iWidth - 1) >> 2) - (x1 >> 2) + 1;
    UINT8   hmask0 = HeadMask[bit1];
    UINT8   hmask1 = ~hmask0;
    UINT8   tmask0 = TailMask[(x1 + iWidth - 1) &3];
    UINT8   tmask1 = ~tmask0;
    INT8    dir = bit0 - bit1;                          // 移位方向,0 不移,> 0 左移,< 0 右移
    UINT8   shift0 = (((dir >= 0) ? dir : dir+4) << 1); // 移位次数
    UINT8   shift1 = 8 - shift0;                        // 反向移位次数
   switch(iDrawMode)
   {
   case DRAWMODE_REP:
    while(iHeight--)
    {
        UINT8 *from1 = from;
        UINT8 *to1 = to;
        UINT8 *to2 = to + bw1 - 1;
        UINT8 head = *to1;
        UINT8 tail = *to2;
        if (!dir)
        {
            memcpy(to1, from1, bw1);
        }
        else
        {
            UINT8 op0 = *from1;
            if (dir > 0)
                from1 ++;
            for(UINT16 i = 0; i < bw1; i ++)
            {
                UINT8 op1 = *from1;
                *to1 = (op0 << shift0) | (op1 >> shift1);
                op0 = op1;
                from1 ++;
                to1 ++;
            }
        }
        *to = (head & hmask0) | (*to & hmask1);
        *to2 = (tail & tmask0) | (*to2 & tmask1);
        from += iSrcFrmWidthBytes;
        to += iDesFrmWidthBytes;
    } // end while(iHeight--)
    break;
   case DRAWMODE_AND:
        while(iHeight--)
    {
        UINT8 *from1 = from;
        UINT8 *to1 = to;
        UINT8 *to2 = to + bw1 - 1;
        UINT8 head = *to1;
        UINT8 tail = *to2;
        if (!dir)
        {
            for(INT16 i=0; i<bw1; i++, to1++, from1++)
                *to1 &= *from1;
        }
        else
        {
            UINT8 op0 = *from1;
            if (dir > 0)
                from1 ++;
            for(UINT16 i = 0; i < bw1; i ++, from1++, to1++)
            {
                UINT8 op1 = *from1;
                *to1 &= ((op0 << shift0) | (op1 >> shift1));
                op0 = op1;
            }
        }
        *to  = head & (*to|hmask0); 
        *to2 = tail & (*to2|tmask0);
        from += iSrcFrmWidthBytes;
        to += iDesFrmWidthBytes;
    } // end while(iHeight--)
       break;
   case DRAWMODE_OR:
        while(iHeight--)
        {
            UINT8 *from1 = from;
            UINT8 *to1 = to;
            UINT8 *to2 = to + bw1 - 1;
            UINT8 head = *to1;
            UINT8 tail = *to2;
            if (!dir)
            {
                for(INT16 i=0; i<bw1; i++, to1++, from1++)
                    *to1 |= *from1;
            }
            else
            {
                UINT8 op0 = *from1;
                if (dir > 0)
                    from1 ++;
                for(UINT16 i = 0; i < bw1; i ++, from1++, to1++)
                {
                    UINT8 op1 = *from1;
                    *to1 |= ((op0 << shift0) | (op1 >> shift1));
                    op0 = op1;
                }
            }
            *to  = head | (*to&hmask1); 
            *to2 = tail | (*to2&tmask1);
            from += iSrcFrmWidthBytes;
            to += iDesFrmWidthBytes;
        } // end while(iHeight--)
       break;
   case DRAWMODE_XOR:
       while(iHeight--)
        {
            UINT8 *from1 = from;
            UINT8 *to1 = to;
            UINT8 *to2 = to + bw1 - 1;
            UINT8 head = *to1;
            UINT8 tail = *to2;
            if (!dir)
            {
                for(INT16 i=0; i<bw1; i++, to1++, from1++)
                    *to1 ^= *from1;
            }
            else
            {
                UINT8 op0 = *from1;
                if (dir > 0)
                    from1 ++;
                for(UINT16 i = 0; i < bw1; i ++, from1++, to1++)
                {
                    UINT8 op1 = *from1;
                    *to1 |= ((op0 << shift0) | (op1 >> shift1));
                    op0 = op1;
                }
            }
            *to  = ((head^*to) &hmask1)  | (*to&hmask0); 
            *to2 = ((tail^*to2)&tmask1) | (*to2&tmask0);
            from += iSrcFrmWidthBytes;
            to += iDesFrmWidthBytes;
        }
       break;
   }
}   
    


//清除屏幕
extern "C" void HwClearDevice(UINT8 iColor)
{
   memset(g_pLcdBuf[0], g_Pen8[iColor&3], LCD_WIDTH/4*LCD_HEIGHT);
}
 
//初始化图形系统
extern "C" void HwInitGraphicsSystem()
{
   *(UINT32 *)0xFFFFFA00 = LCD_SCREEN_BUF_ADDR;
   *(UINT8 *)0xFFFFFA05 = LCD_WIDTH/8;
   *(UINT16 *)0xFFFFFA08 = LCD_WIDTH;
   *(UINT16 *)0xFFFFFA0A = LCD_HEIGHT-1;
   *((UINT16 *)0xFFFFFA18) = 0x8000;
   *((UINT16 *)0xFFFFFA1A) = 0x0000;
   *(UINT8 *)0xFFFFFA20  = 9;
   *(UINT8 *)0xFFFFFA21  = 1;
   *(UINT8 *)0xFFFFFA23  = 0;
   *(UINT8 *)0xFFFFFA25 |= 0x0A;
   UINT8 * addr = (UINT8 *)(*((UINT32 *)0xFFFFFA00)); 
   for(INT16 i=0; i< LCD_HEIGHT; i++, addr += LCD_WIDTH/4)
        g_pLcdBuf[i] = addr;
   HwClearDevice(WHITE);
}

extern "C" INT8 HwGetPixel(INT16 x, INT16 y)
{
	LTOP(x, y);
    if(x<0 || y<0 || x>=LCD_WIDTH || y>=LCD_HEIGHT)
        return -1;
    UINT8 bit = 6-((x&3)<<1);
    INT8 byte = *((INT8 *)g_pLcdBuf[y] + (x>>2));
    return (byte>>bit)&3;
}

//在指定位置用指定颜色、宽度、作图模式、写一个点
extern "C" void HwSetPixel(INT16 iX, INT16 iY, UINT8 iColor, UINT8 iDrawMode, UINT8 iSize)
{
	LTOP(iX, iY);
    if(iX+iSize >LCD_WIDTH || iY+iSize >LCD_HEIGHT || iX <0 || iY < 0)
        return;
    UINT8 mask[]={0x3F, 0xCF, 0xF3, 0xFC};
    INT8 off = iX>>2;
    INT8 bit1= iX&3;
    UINT8 * addr = g_pLcdBuf[iY] + off;
    switch(iDrawMode)
    {
        
        case DRAWMODE_REP: 
             for(UINT8 i=0; i< iSize; i++,iY++)
              {
                 UINT8 * addr = g_pLcdBuf[iY] + off;
                 UINT8   bit = bit1;
                 for(UINT8 j=0; j<iSize; j++)
                 {
                  *addr = (*addr & mask[bit]) | (iColor<<(6-(bit<<1)));
                  if(++bit>3)
                     addr++, bit=0;
                 }
             } 
            break;
        case DRAWMODE_AND:
              for(UINT8 i=0; i< iSize; i++,iY++)
              {
                 UINT8 * addr = g_pLcdBuf[iY] + off;
                 UINT8   bit = bit1;
                 for(UINT8 j=0; j<iSize; j++)
                 {
                  *addr &= ((iColor<<(6-(bit<<1))) | mask[bit]);
                  if(++bit>3)
                     addr++, bit=0;
                 }
              }
             break;
        case DRAWMODE_OR:
             for(UINT8 i=0; i< iSize; i++,iY++)
              {
                 UINT8 * addr = g_pLcdBuf[iY] + off;
                 UINT8   bit = bit1;
                 for(UINT8 j=0; j<iSize; j++)
                 {
                  *addr |= (iColor<<(6-(bit<<1)));
                  if(++bit>3)
                     addr++, bit=0;
                 }
              }
            break;

       case DRAWMODE_XOR:
            for(UINT8 i=0; i< iSize; i++,iY++)
              {
                 UINT8 * addr = g_pLcdBuf[iY] + off;
                 UINT8   bit = bit1;
                 for(UINT8 j=0; j<iSize; j++)
                 {
                  *addr ^= (iColor<<(6-(bit<<1)));
                  if(++bit>3)
                     addr++, bit=0;
                 }
              }
            break;

    }
}

//从x0,y0到x1,y1画一条直线,颜色、模式、宽度由iColor,iAttrib,iSize决定
extern "C" void HwLine(INT16 x0, INT16 y0, INT16 x1, INT16 y1, 
                      UINT8 iColor, UINT8 iAttrib, UINT8 iSize, UINT8 iLineStyle)
{
    
    INT16  dx = ABS(x1-x0), dy = ABS(y1-y0);
    INT16  x=x0, y=y0;
    INT16  e= (dx>=dy) ? -dx : -dy;
    INT16  stepx = (x1>x0)?1:-1, stepy = (y1>y0)?1:-1;
    UINT16 dot = 0;
    UINT16 swidth = iLineStyle + iSize;
    if(dx>=dy)
      for(INT16 i=0; i<=dx; i++)
      {
         if(!iLineStyle)
            HwSetPixel(x, y, iColor, iAttrib, iSize);
         else if(dot++ < iLineStyle)
            HwSetPixel(x, y, iColor, iAttrib, iSize);
         else if(dot++ > swidth) 
             dot =0;

         x += stepx;
         e=e+(dy<<1); 
         if(e>=0)
         {
             y += stepy;
             e=e-(dx<<1);
         }  
      }
    else
      for(INT16 i=0; i<=dy; i++)
      {
         if(!iLineStyle)
            HwSetPixel(x, y, iColor, iAttrib, iSize);
         else if(dot++ < iLineStyle)
            HwSetPixel(x, y, iColor, iAttrib, iSize);
         else if(dot++ > swidth) 
             dot =0;

         y += stepy;
         e=e+(dx<<1); 
         if(e>=0)
         {
           x += stepx;
           e=e-(dy<<1);
         } 
      }
    
}

//画一个椭圆
extern "C" void HwEllipse(INT16 left, INT16 top, INT16 right, INT16 bottom, UINT8 iColor, UINT8 iDrawMode, UINT8 iPenWidth)
{
    INT16 x0 = (right+left)>>1;
    INT16 y0 = (bottom+top)>>1;
    INT16 x1 = ABS(right-left)>>1;
    INT16 y1 = ABS(bottom-top)>>1;

    INT16 x=0, y =y1;

    INT32 a2 = (x1)*(x1);
    INT32 b2 = y1*y1;

    INT32 _2a2 = a2<<1;
    INT32 _2b2 = b2<<1;
    INT32 a2b2 = a2*b2;

    INT32 _x2 = b2*x*x;
    INT32 _x  = b2*x*2;
    
    INT32 _y2 = a2*y*y;
    INT32 _y  = a2*y*2;

    INT32 ox = x;

    while(x <= x1)
    {
        if(_x2 + _y2 > a2b2)
        {
            _y2 = _y2 - _y + a2;
            _y = _y - _2a2;
            y -= 1;
            if(x - ox >= 1)
            {
                HwSetPixel(x0+x, y0+y, iColor, iDrawMode, iPenWidth);
                HwSetPixel(x0-x, y0+y, iColor, iDrawMode, iPenWidth);
                HwSetPixel(x0+x, y0-y, iColor, iDrawMode, iPenWidth);
                HwSetPixel(x0-x, y0-y, iColor, iDrawMode, iPenWidth);
            }
        }
        else
        {
            HwSetPixel(x0+x, y0+y, iColor, iDrawMode, iPenWidth);
            HwSetPixel(x0-x, y0+y, iColor, iDrawMode, iPenWidth);
            HwSetPixel(x0+x, y0-y, iColor, iDrawMode, iPenWidth);
            HwSetPixel(x0-x, y0-y, iColor, iDrawMode, iPenWidth);
            _x2 = _x2 + _x + b2;
            _x = _x + _2b2;
            ox = x;
            x += 1;
        }
    }
}
/*
extern "C" void HwSetDot(INT16 iX, INT16 iY, UINT8 iColor)
{
    LTOP(iX, iY);

    if(iX >=LCD_WIDTH || iY>=LCD_HEIGHT || iX <0 || iY < 0)
        return;
    UINT8 mask[]={0x3F, 0xCF, 0xF3, 0xFC};
    INT8 off = iX>>2;
    INT8 bit= iX&3;
    INT8 bit1 = 6-(bit<<1);
    UINT8 * addr = g_pLcdBuf[iY] + off;
    *addr = (*addr & mask[bit]) | (iColor<<bit1);

}
//用指定颜色填充一个封闭区域 速度太慢,凑合用
//内存使用太多，用循环队列可解决
extern "C" BOOL HwFloodFill(INT16 x , INT16 y, UINT8 iColor)
{
    INT8  curcolor = HwGetPixel(x,y);
    if(curcolor == iColor || curcolor<0)
        return FALSE;
    UINT16 *xqueue = (UINT16 *)mcMalloc(LCD_WIDTH*LCD_HEIGHT);
    if(!xqueue)
        return FALSE;
    UINT16 *yqueue = (UINT16 *)mcMalloc(LCD_WIDTH*LCD_HEIGHT);
    
    if(!yqueue)
    {
        mcFree(xqueue);
        return FALSE;
    }
    UINT16 head=0,real=0;
    xqueue[real] = x;
    yqueue[real++] =y;
    HwSetDot(x, y, iColor);
    INT8 dx[] ={0, 0, -1, 1};
    INT8 dy[] ={-1, 1, 0, 0};
    INT16 cx, cy;
    INT16 maxdot=1;
    while(head<real)
    {
        x = xqueue[head];
        y = yqueue[head++];
        for(UINT8 i=0; i<4; i++)
        {
            cx = x+dx[i];
            cy = y+dy[i];
            if(HwGetPixel(cx, cy) == curcolor)
            {
              xqueue[real] = cx;
              yqueue[real++] = cy;
              HwSetDot(cx, cy, iColor);
            }
            if(real-head>maxdot)
                maxdot = real-head;
        }
    }

    mcFree(xqueue);
    mcFree(yqueue);
    return TRUE;
}
*/
//用指定颜色画一个填充矩形
extern "C" void HwPaintRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 iColor)
{
	LTOP(x0, y0, x1, y1);
    if(x0>x1)
        SWAP(x0,x1);
    if(y0>y1)
        SWAP(y0,y1);
    if(x1>=LCD_WIDTH || y1>=LCD_HEIGHT || x0<0 || y0<0)
        return;
    UINT8 fmask[] ={0x00, 0xC0, 0xF0, 0xFC};
    UINT8 rmask[]={0x3F, 0x0F, 0x03, 0x00};

    INT16 off = x0>>2;
    UINT8 bit0 = x0&3, bit1 = x1&3;
    INT8  size = (x1>>2)- off -1;
    UINT8 bit2 = bit0<<1;
    for(INT16 i=y0; i<=y1; i++)
    {
        UINT8 * addr =  g_pLcdBuf[i] + off;
        *addr = (*addr & fmask[bit0]) | (g_Pen8[iColor]>>bit2);
        addr ++;
        if(size >0)
         memset(addr, g_Pen8[iColor], size);
        addr += size;
        *addr = (*addr & rmask[bit1]) | (g_Pen8[iColor] &~rmask[bit1]);
    }
}

extern "C" void HwWriteImage(INT16 x0, INT16 y0, INT16 x1, 
                         INT16 y1, UINT8 iDrawMode, UINT8 *pImageBuf)
{
    LTOP(x0, y0, x1, y1);
    if(x0>x1)
        SWAP(x0,x1);
    if(y0>y1)
        SWAP(y0,y1);

    if(x0<0 || x1>=LCD_WIDTH || y0<0 || y1>=LCD_HEIGHT)
         return ;

    INT16 width = x1-x0+1;
    INT16 height = y1-y0+1;
    HwBitBlt(pImageBuf, (width+3)>>2, 0,0,width, 
                 height, g_pLcdBuf[0], LCD_WIDTH/4, x0, y0, iDrawMode);
    
} 

extern "C" void HwInvertRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1)
{
	LTOP(x0, y0, x1, y1);
    if(x0>x1)
        SWAP(x0,x1);
    if(y0>y1)
        SWAP(y0,y1);
   if(x1>=LCD_WIDTH || y1>= LCD_HEIGHT || x0<0 || y0<0)
       return;
    UINT8 mask[]= {0x00, 0xC0, 0xF0, 0xFC, 0xFF};
    UINT8 mask1[]={0xFF, 0x3F, 0x0F, 0x03, 0x00};

    UINT8 off = x0>>2;
    UINT8 bit0 = x0&3, bit1 = (x1&3)+1;
    INT8 size = (x1>>2)- off-1;
    if(size>=0)
        for(INT16 i=y0; i<=y1; i++)
        {
            UINT8 *addr = g_pLcdBuf[i] + off;
            *addr = *addr & mask[bit0] | ~(*addr) &mask1[bit0];
            addr++;
            for(INT16 j=0; j<size; j++,addr++)
                *addr = ~(*addr);
            *addr  = *addr & mask1[bit1] | (~(*addr) & mask[bit1]);
        }
    else
      for(INT16 i=y0; i<=y1; i++)
      {
            UINT8 *addr = g_pLcdBuf[i] + off;
            UINT8 cmask = mask[bit1-bit0]>>bit0;
            UINT8 tmp =*addr & ~cmask;
            *addr = ~(*addr) & cmask | tmp;
      }
}

extern "C" UINT16 HwImageSize(INT16 x0, INT16 y0, INT16 x1, INT16 y1)
{
	LTOP(x0,y0,x1,y1);
    if(x0>x1)
        SWAP(x0,x1);
    if(y0>y1)
        SWAP(y0,y1);
    UINT16 bytes = (x1-x0+4)>>2;// + ((x1-x0+1)%4? 1:0);
    return bytes * (y1-y0+1);
}

extern "C" void HwReadImage(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 *pBuff)
{
	LTOP(x0, y0, x1, y1);
    if(x0>x1)
        SWAP(x0,x1);
    if(y0>y1)
        SWAP(y0,y1);
    
    if(x0<0 || x1>=LCD_WIDTH || y0<0 || y1>=LCD_HEIGHT)
         return ;
    UINT16 bytes = (x1-x0+4)>>2;
    HwBitBlt(g_pLcdBuf[0], LCD_WIDTH/4, x0, y0, x1-x0+1, y1-y0+1, pBuff, bytes,0,0, DRAWMODE_REP);
    
}

extern "C" void HwPutAsc(INT16 x, INT16 y, UINT8 iBackColor, UINT8 iForColor, UINT8 iDrawMode, UINT8 iChar)
{

   UINT32   bitmap[CHAR_HEIGHT * ASC_WIDTH / 16];
   UINT32 * fontaddr = (UINT32 *)(ASC_FONT_ADDR + (iChar<<5));
   UINT32 * paddr = fontaddr;
   UINT32   mask = g_Pen32[iBackColor];
   for(UINT8 i=0; i<CHAR_HEIGHT * ASC_WIDTH / 16; i++)
      bitmap[i] = ~(*(paddr++)| mask);
   if(iForColor)
   {
       paddr = fontaddr;
       mask = g_Pen32[3-iForColor];
       for(UINT8 i=0; i<CHAR_HEIGHT * ASC_WIDTH / 16; i++)
          bitmap[i] |= *(paddr++) & mask;
   }
   
   ::HwWriteImage(x, y, x+ASC_WIDTH-1, y+CHAR_HEIGHT-1, iDrawMode, (UINT8 *)bitmap);
 }

extern "C" void HwPutHz(INT16 x, INT16 y, UINT8 iBackColor, UINT8 iForColor, UINT8 iDrawMode, UINT16 iHz)
{
    UINT16 bitmap[CHAR_HEIGHT*2];
    UINT8 *fontaddr = (UINT8 *)HZ_FONT_ADDR + ((((UINT32)HIBYTE(iHz)-161)*94L+LOBYTE(iHz)-161) << 5);
    UINT8 *paddr = fontaddr;
    UINT16 mask = g_Pen16[iBackColor];
    for(UINT8 i=0; i<CHAR_HEIGHT*2; i++)
        bitmap[i] = ~(g_iOneBit2TwoBitMap[*(paddr++)] | mask);//
    if(iForColor)
    {
       paddr = fontaddr;
       mask = g_Pen16[3-iForColor];
       for(UINT8 i=0; i<CHAR_HEIGHT*2; i++)
          bitmap[i] |= g_iOneBit2TwoBitMap[*(paddr++)] & mask;
    }
    ::HwWriteImage(x, y, x+HZ_WIDTH-1, y+CHAR_HEIGHT-1, iDrawMode, (UINT8 *)bitmap);
}

extern "C" void HwPutString(INT16 x, INT16 y, UINT8 iBackColor, UINT8 iForColor, UINT8 iDrawMode, UINT8 *pText, INT16 iLen)
{
#define MAX_STRING_LEN 255
    if(!iLen) iLen = MAX_STRING_LEN;
    for(INT16 i=0; i<iLen && *pText && pText;)
      {
         if(ISCC(*pText))
         {
             ::HwPutHz(x, y, iBackColor, iForColor, iDrawMode, MAKEWORD(*(pText+1), *pText));
             x+= HZ_WIDTH;
             pText +=2;
             i += 2;
         }
         else
         {
			 if(!((*pText) < 0x20 || (*pText) > 0x7F)){
				 ::HwPutAsc(x, y, iBackColor, iForColor, iDrawMode, *pText);
				 x+= ASC_WIDTH;
			 }
			 else 
				 return;// hqf
             pText++;
             i++;
         }
      }
#undef MAX_STRING_LEN
}
extern "C" void HwPutIcon(INT16 x, INT16 y, UINT8 iDrawMode, UINT32 iIcon)
{
//    ::HwWriteImage(x, y, x+gICON_WIDTH(iIcon)-1,  y+gICON_HEIGHT(iIcon)-1, iDrawMode, (UINT8*)(iIcon + 12));
}

extern "C" INT16 HwGetTextExtern(char *pText)
{
    return strlen(pText)*ASC_WIDTH;
}

extern "C" void HwDrawText(INT16 x, INT16 y, UINT8 iBkColor, UINT8 iForColor, UINT8 iDrawMode, UINT8 iHalign, UINT8 iValign, char *pText)
{
  INT16 width = HwGetTextExtern(pText); 
    switch(iHalign)
    {
    case HALIGN_CENTER:
        x -= width/2;
        break;
    case HALIGN_RIGHT:
        x -= width;
        break;
    }
    switch(iValign)
    {
    case VALIGN_MID:
         y -= CHAR_HEIGHT/2;
         break;
    case VALIGN_BOTTOM:
         y -= CHAR_HEIGHT;
         break;
    }
    ::HwPutString(x, y, iBkColor, iForColor, iDrawMode, (UINT8 *)pText, 0); 

}

    /////////////////////////////////////////////////////////////////////
    //                                                                 //
    //           第二部分，硬件光标接口                                //
    //                                                                 //
    //                                                                 //
    /////////////////////////////////////////////////////////////////////

extern "C" void OpenCursor()
{
    *((unsigned short *)0xFFFFFA18) |= 0x8000;	//YGR
    *((unsigned char *)0xFFFFFA1F) |= 0x9F;
} 
extern "C" BOOL GetCursorStatus()
{
	return *((unsigned short *)0xFFFFFA18) & 0xC000;
}
extern "C" void GetCursorXY(short * iX, short * iY)
{
	*iX = *((unsigned short *)0xFFFFFA18) & 0x01FF;
    *iY = *((unsigned short *)0xFFFFFA1A) & 0x00FF;
#ifdef CIRCUMGYRATE
  INT16 tmp = *iY;
  *iY = *iX;
  *iX = LCD_WIDTH-tmp-1;
#endif
}

extern "C" void CloseCursor()
{
    *((unsigned short *)0xFFFFFA18) &= 0x1FF;
} 
 
extern "C" void SetCursorXY(short  iX,  short iY)
{
	LTOP(iX, iY);
    *((unsigned short *)0xFFFFFA18) = (iX & 0x01FF|0x8000);	//YGR
    *((unsigned short *)0xFFFFFA1A) = (iY & 0x00FF);
}
//iBlink为0时静止
extern "C" void SetCursorAttr(unsigned char iWidth, unsigned char iHeight)
{
#ifdef CIRCUMGYRATE
	SWAP(iWidth, iHeight);
#endif
    *((unsigned short *)0xFFFFFA1C) = (((iWidth&0x1F)<<8)|iHeight&0x1F);
    //频率
    *((unsigned char *)0xFFFFFA1F) = 0x9F;
} 

extern "C" void StrTrim(char *szText, char cTrimChar)
{
    char *pstr = szText;
    while(*pstr == cTrimChar && *pstr)
        pstr++;

    if(pstr != szText)
     memmove(szText, pstr, strlen(szText)+1);

    pstr = szText + strlen(szText)-1;
    while(*pstr == cTrimChar)
        pstr--;
    *(pstr+1)=0;
}
    /////////////////////////////////////////////////////////////////////
    //                                                                 //
    //           第三部分，CDC类函数定义                               //
    //                                                                 //
    //                                                                 //
    /////////////////////////////////////////////////////////////////////

/*
UINT8 CDC:: m_bOutToVirtualScreen = FALSE; 

void CDC::SetOutScreen(UINT8 bOutToVirtualScreen)
{
    //if(m_bOutToVirtualScreen == bOutToVirtualScreen)
    //  return;
    UINT8 * addr= bOutToVirtualScreen ? 
                     g_pVirtualScreenBuf : (UINT8 *)(*((UINT32 *)0xFFFFFA00));
    
    m_bOutToVirtualScreen = bOutToVirtualScreen; 
    
    for(INT16 i=0; i< LCD_HEIGHT; i++, addr += LCD_WIDTH/4)
        g_pLcdBuf[i] = addr;
}

void CDC::Invalidate(INT16 x0, INT16 y0, INT16 x1, INT16 y1)
{
    if(m_bOutToVirtualScreen)
    {
        LTOP(x0, y0, x1, y1);
        if(x0 > x1)
          SWAP(x0,x1);
        if(y0 > y1)
          SWAP(y0, y1);
        UINT8 *addr = (UINT8 *)(*((UINT32 *)0xFFFFFA00)); 
        ::HwBitBlt(g_pVirtualScreenBuf, LCD_WIDTH/4, x0, y0, x1-x0+1,
                   y1-y0+1, addr, LCD_WIDTH/4, x0, y0, DRAWMODE_REP);   
    }
}

void CDC::Invalidate(CRect *srRect)
{
    Invalidate(srRect->Xmin, srRect->Ymin, srRect->Xmax, srRect->Ymax);    
}
*/
CDC::CDC()
{
  Reset();
}

void CDC::Reset()
{
    m_iBackColor = WHITE;
    m_iPenColor  = BLACK;
    m_iDrawMode  = DRAWMODE_REP;
    m_iValign    = VALIGN_TOP;
    m_iHalign    = HALIGN_LEFT;
    m_iCurX =0; 
    m_iCurY =0;
    m_iPenWidth  = 1;
    m_iLineStyle = 0;

}

void CDC::LineTo(INT16 iX, INT16 iY)
{
    HwLine(m_iCurX, m_iCurY, iX, iY,
               m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle);
    m_iCurX = iX;
    m_iCurY = iY;
}

void CDC::SetPixel(INT16 iX, INT16 iY)
{
    ::HwSetPixel(iX, iY, m_iPenColor, m_iDrawMode, m_iPenWidth);
}
void CDC::PaintRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    ::HwPaintRect(iLeft, iTop, iRight, iBottom, m_iPenColor);
}
void CDC::EraseRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    ::HwPaintRect(iLeft, iTop, iRight, iBottom, m_iBackColor);
}
void CDC::FrameRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    ::HwLine(iLeft, iTop, iRight, iTop, m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle); 
    ::HwLine(iLeft, iTop, iLeft, iBottom, m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle); 
    ::HwLine(iLeft, iBottom, iRight, iBottom, m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle); 
    ::HwLine(iRight, iBottom, iRight, iTop, m_iPenColor, m_iDrawMode, m_iPenWidth, m_iLineStyle); 
}
void CDC::PutImage(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom, UINT8 * pImgBuf, UINT8 iPutAttr)
{
    ::HwWriteImage(iLeft, iTop, iRight, iBottom, iPutAttr, pImgBuf); 
}
void CDC::PaintRect(CRect &src)
{
    ::HwPaintRect(src.left, src.top, src.right, src.bottom, m_iPenColor);
}
void CDC::EraseRect(CRect &src)
{
    ::HwPaintRect(src.left, src.top, src.right, src.bottom, m_iBackColor);
}
void CDC::FrameRect(CRect &src)
{
    FrameRect(src.left, src.top, src.right, src.bottom);
}
void CDC::Draw3dRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 cTopLeft, UINT8 cBottomRight)
{
    ::HwLine(x0, y0, x1, y0, cTopLeft, DRAWMODE_REP, 1, 0);
    ::HwLine(x0, y0, x0, y1, cTopLeft, DRAWMODE_REP, 1, 0);
    ::HwLine(x1, y1, x1, y0, cBottomRight, DRAWMODE_REP, 1, 0);
    ::HwLine(x1, y1, x0, y1, cBottomRight, DRAWMODE_REP, 1, 0);
}
void CDC::Draw3dRect(CRect &srRec, UINT8 cTopLeft, UINT8 cBottomRight)
{
    Draw3dRect(srRec.left, srRec.top, srRec.right, srRec.bottom, cTopLeft, cBottomRight);
}
void CDC::ClearDevice(UINT8 iColor)
{
    ::HwClearDevice(iColor);
}

ScreenBuf * CDC::SaveScreen(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    ScreenBuf *pBuf = (ScreenBuf *)SysMalloc(sizeof(ScreenBuf));
    if(!pBuf)
        return NULL;
    UINT16 size = HwImageSize(iLeft, iTop, iRight, iBottom);
    pBuf->pBuffer = (UINT8 *)SysMalloc(size);
    if(!pBuf->pBuffer)
    {
        SysFree(pBuf); 
        return NULL;
    }
    memset(pBuf->pBuffer, 0, size);
    pBuf->x0 = iLeft;
    pBuf->y0 = iTop;
    pBuf->x1 = iRight;
    pBuf->y1 = iBottom;
    ::HwReadImage(iLeft, iTop, iRight, iBottom, pBuf->pBuffer);
    return pBuf;

}
 
ScreenBuf * CDC::SaveScreen(CRect &srRec)
{
    return SaveScreen(srRec.left, srRec.top, srRec.right, srRec.bottom);
}

void CDC::WriteScreen(ScreenBuf *pScrBuf, UINT8 iPutAttr)
{
    ::HwWriteImage(pScrBuf->x0, pScrBuf->y0, 
                   pScrBuf->x1, pScrBuf->y1, 
                   iPutAttr, pScrBuf->pBuffer);
}

void CDC::PutScreen(ScreenBuf * pScrBuf, UINT8 iPutAttr)
{
    ::HwWriteImage(pScrBuf->x0, pScrBuf->y0, 
                   pScrBuf->x1, pScrBuf->y1, 
                   iPutAttr, pScrBuf->pBuffer);
    SysFree(pScrBuf->pBuffer);
    SysFree(pScrBuf);
	pScrBuf = NULL;
}
void CDC::InvertRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    ::HwInvertRect(iLeft, iTop, iRight, iBottom);
} 
void CDC::InvertRect(CRect &srRec)
{
    ::HwInvertRect(srRec.left, srRec.top, srRec.right, srRec.bottom);
}
void CDC::TextOut(INT16 iX, INT16 iY, char *pText, INT16 iLen)
{
    ::HwPutString(iX, iY, m_iBackColor, m_iPenColor, m_iDrawMode, (UINT8 *)pText, iLen); 
}

void CDC::DrawText(CRect &srRect, char *pStr)
{
	INT16	x, y;
    UINT8 *pText = (UINT8 *)pStr;

    if (m_iHalign == HALIGN_CENTER)
		x = (srRect.left + srRect.right) / 2 - GetTextExtent(pStr) / 2;
	else if (m_iHalign == HALIGN_RIGHT)
		x = srRect.right - GetTextExtent(pStr);
	else
		x = srRect.left;

	if (m_iValign == VALIGN_MID)
		y = (srRect.top + srRect.bottom) / 2 - CHAR_HEIGHT / 2;
	else if (m_iValign == VALIGN_BOTTOM)
		y = srRect.bottom - CHAR_HEIGHT;
	else
		y = srRect.top;

    while(*pText && y <= srRect.bottom - CHAR_HEIGHT+1 && pStr)
    {
        if(ISCC(*pText))
         {
			if(x < srRect.left)
			{
				x+= HZ_WIDTH;
				pText +=2;
			}
            else if(x + HZ_WIDTH-1 <= srRect.right)
            {
              ::HwPutHz(x, y, m_iBackColor, m_iPenColor, m_iDrawMode, MAKEWORD(*(pText+1), *pText));
              x+= HZ_WIDTH;
              pText +=2;
            }
            else
            {
                x = srRect.left;
                y += CHAR_HEIGHT;
            }
         }
         else
         {
			 if(x < srRect.left)
			 {
				x+= ASC_WIDTH;
				pText++;
			 }
             else if(x + ASC_WIDTH-1 < srRect.right && *pText != 0x0D )// hqf
             {
              ::HwPutAsc(x, y, m_iBackColor, m_iPenColor, m_iDrawMode, *pText);
              x+= ASC_WIDTH;
              pText++;
             }
             else
             {
               x = srRect.left;
               y += CHAR_HEIGHT;
               if(*pText == 0x0D)  // hqf
                   pText +=2;
             }
         }
    }
}

void CDC::DrawText(INT16 iX, INT16 iY, char *pText)
{
 ::HwDrawText(iX, iY, m_iBackColor, m_iPenColor, m_iDrawMode, m_iHalign, m_iValign, pText);
}

INT16 CDC::GetTextExtent(char *pText)
{
    return HwGetTextExtern(pText);
}

INT16 CDC::GetTextHeight(INT16 iLeft, INT16  iRight, char *pStr)
{
 
    INT16 x = iLeft;
    INT16 y = 0; 
    UINT8 *pText = (UINT8 *)pStr;
    while(*pText && pStr)
    {
        if(ISCC(*pText))
         {
            if(x + HZ_WIDTH-1 <= iRight)
            {
              x+= HZ_WIDTH;
              pText +=2;
            }
            else
            {
                x = iLeft;
                y += CHAR_HEIGHT;
            }
         }
         else
         {
             if(x + ASC_WIDTH-1 < iRight && *pText != '\n')
             {
              x+= ASC_WIDTH;
              pText++;
             }
             else 
             {
               x = iLeft;
               y += CHAR_HEIGHT;
               if(*pText == '\n')
                   pText++;
             }
         }
    }
    return y + CHAR_HEIGHT;
}

void CDC::Draw3dButton(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom, char *pCaption)
{
	::HwPaintRect(iLeft, iTop, iRight, iBottom, LGRAY);
    ::HwDrawText((iLeft + iRight)/2 +1, (iTop + iBottom)/2+1, 
                 LGRAY, BLACK, DRAWMODE_REP, HALIGN_CENTER, 
                 VALIGN_MID, pCaption);
    Draw3dRect(iLeft, iTop, iRight, iBottom);
	//Draw3dRect(iLeft+1, iTop+1, iRight-1, iBottom-1);
}

void CDC::Draw3dButton(CRect &srRec, char *pCaption)
{
  Draw3dButton(srRec.left, srRec.top, srRec.right, srRec.bottom, pCaption);
}

void CDC::PutIcon(INT16 x, INT16 y, UINT32 iIcon)
{
	UINT16 *temp;

	if (iIcon > BMPTOTAL  || iIcon == 0)
		return;
	temp = (UINT16 *)gICON_ADDR(iIcon);
	if(*(temp + 3)== COMPRESS_LZW)
	{ 
		BYTE *pImage;
		CLZWDecode lzwDec;

		lzwDec.Decode((BYTE *)(temp + 6), &pImage);
	    ::HwWriteImage(x, y, x+gICON_WIDTH(iIcon)-1, y+gICON_HEIGHT(iIcon)-1, m_iDrawMode, (UINT8*)(pImage));
		lzwDec.ReleaseBuf(pImage);
	}
	if(*(temp + 3)== COMPRESS_NO)
	{
	   ::HwWriteImage(x, y, x+gICON_WIDTH(iIcon)-1, y+gICON_HEIGHT(iIcon)-1, m_iDrawMode, (UINT8*)(gICON_ADDR(iIcon) + 12));
	}

//    if(iIcon && iIcon <= BMPTOTAL)
//     ::HwWriteImage(x, y, x+gICON_WIDTH(iIcon)-1,  y+gICON_HEIGHT(iIcon)-1, m_iDrawMode, (UINT8*)(gICON_ADDR(iIcon)+12));
}

void CDC::GetImage(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom, UINT8 * pImagBuf)
{
    ::HwReadImage(iLeft, iTop, iRight, iBottom, pImagBuf); 
}

UINT16 CDC::ImageSize(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom)
{
    return ::HwImageSize(iLeft,iTop, iRight, iBottom); 
}

UINT16 CDC::ImageSize(CRect &srRec)
{
    return ::HwImageSize(srRec.left, srRec.top, srRec.right, srRec.bottom);
}

void CDC::PutImage(CRect &srRec, UINT8 * pImgBuf, UINT8 iPutAttr)
{ 
    PutImage(srRec.left, srRec.top, srRec.right, srRec.bottom, pImgBuf, iPutAttr);
}
   
void CDC::GetImage(CRect &srRec, UINT8 *pImgBuf)
{
    GetImage(srRec.left, srRec.top, srRec.right, srRec.bottom, pImgBuf);
}

void CDC::PutChar(INT16 x, INT16 y, UINT16 iChar)
{
    if(iChar)
    {
        if(ISCC(HIBYTE(iChar)))
            ::HwPutHz(x, y, m_iBackColor, m_iPenColor, m_iDrawMode, iChar);
        else
            ::HwPutAsc(x, y, m_iBackColor, m_iPenColor, m_iDrawMode, LOBYTE(iChar));
    }
}

//画椭圆
void CDC::Ellipse(INT16 x0, INT16 y0, INT16 x1, INT16 y1)
{
    ::HwEllipse(x0, y0, x1, y1, m_iPenColor, m_iDrawMode, m_iPenWidth);
}

void CDC::Ellipse(CRect &srRec)
{
    Ellipse(srRec.left, srRec.top, srRec.right, srRec.bottom);
}
/*
BOOL CDC::FloodFill(INT16 x, INT16 y)
{
    return HwFloodFill(x, y, m_iPenColor);
}
*/
//end of CDC

//下面的数据用于1 bit到2 bit字节转换时查表以加快转换速度
const UINT16 g_iOneBit2TwoBitMap[256] =
{
    0x0000, 0x0003, 0x000C, 0x000F, 0x0030, 0x0033, 0x003C, 0x003F, 
    0x00C0, 0x00C3, 0x00CC, 0x00CF, 0x00F0, 0x00F3, 0x00FC, 0x00FF, 
    0x0300, 0x0303, 0x030C, 0x030F, 0x0330, 0x0333, 0x033C, 0x033F, 
    0x03C0, 0x03C3, 0x03CC, 0x03CF, 0x03F0, 0x03F3, 0x03FC, 0x03FF, 
    0x0C00, 0x0C03, 0x0C0C, 0x0C0F, 0x0C30, 0x0C33, 0x0C3C, 0x0C3F, 
    0x0CC0, 0x0CC3, 0x0CCC, 0x0CCF, 0x0CF0, 0x0CF3, 0x0CFC, 0x0CFF, 
    0x0F00, 0x0F03, 0x0F0C, 0x0F0F, 0x0F30, 0x0F33, 0x0F3C, 0x0F3F, 
    0x0FC0, 0x0FC3, 0x0FCC, 0x0FCF, 0x0FF0, 0x0FF3, 0x0FFC, 0x0FFF, 
    0x3000, 0x3003, 0x300C, 0x300F, 0x3030, 0x3033, 0x303C, 0x303F, 
    0x30C0, 0x30C3, 0x30CC, 0x30CF, 0x30F0, 0x30F3, 0x30FC, 0x30FF, 
    0x3300, 0x3303, 0x330C, 0x330F, 0x3330, 0x3333, 0x333C, 0x333F, 
    0x33C0, 0x33C3, 0x33CC, 0x33CF, 0x33F0, 0x33F3, 0x33FC, 0x33FF, 
    0x3C00, 0x3C03, 0x3C0C, 0x3C0F, 0x3C30, 0x3C33, 0x3C3C, 0x3C3F, 
    0x3CC0, 0x3CC3, 0x3CCC, 0x3CCF, 0x3CF0, 0x3CF3, 0x3CFC, 0x3CFF, 
    0x3F00, 0x3F03, 0x3F0C, 0x3F0F, 0x3F30, 0x3F33, 0x3F3C, 0x3F3F, 
    0x3FC0, 0x3FC3, 0x3FCC, 0x3FCF, 0x3FF0, 0x3FF3, 0x3FFC, 0x3FFF, 
    0xC000, 0xC003, 0xC00C, 0xC00F, 0xC030, 0xC033, 0xC03C, 0xC03F, 
    0xC0C0, 0xC0C3, 0xC0CC, 0xC0CF, 0xC0F0, 0xC0F3, 0xC0FC, 0xC0FF, 
    0xC300, 0xC303, 0xC30C, 0xC30F, 0xC330, 0xC333, 0xC33C, 0xC33F, 
    0xC3C0, 0xC3C3, 0xC3CC, 0xC3CF, 0xC3F0, 0xC3F3, 0xC3FC, 0xC3FF, 
    0xCC00, 0xCC03, 0xCC0C, 0xCC0F, 0xCC30, 0xCC33, 0xCC3C, 0xCC3F, 
    0xCCC0, 0xCCC3, 0xCCCC, 0xCCCF, 0xCCF0, 0xCCF3, 0xCCFC, 0xCCFF, 
    0xCF00, 0xCF03, 0xCF0C, 0xCF0F, 0xCF30, 0xCF33, 0xCF3C, 0xCF3F, 
    0xCFC0, 0xCFC3, 0xCFCC, 0xCFCF, 0xCFF0, 0xCFF3, 0xCFFC, 0xCFFF, 
    0xF000, 0xF003, 0xF00C, 0xF00F, 0xF030, 0xF033, 0xF03C, 0xF03F, 
    0xF0C0, 0xF0C3, 0xF0CC, 0xF0CF, 0xF0F0, 0xF0F3, 0xF0FC, 0xF0FF, 
    0xF300, 0xF303, 0xF30C, 0xF30F, 0xF330, 0xF333, 0xF33C, 0xF33F, 
    0xF3C0, 0xF3C3, 0xF3CC, 0xF3CF, 0xF3F0, 0xF3F3, 0xF3FC, 0xF3FF, 
    0xFC00, 0xFC03, 0xFC0C, 0xFC0F, 0xFC30, 0xFC33, 0xFC3C, 0xFC3F, 
    0xFCC0, 0xFCC3, 0xFCCC, 0xFCCF, 0xFCF0, 0xFCF3, 0xFCFC, 0xFCFF, 
    0xFF00, 0xFF03, 0xFF0C, 0xFF0F, 0xFF30, 0xFF33, 0xFF3C, 0xFF3F, 
    0xFFC0, 0xFFC3, 0xFFCC, 0xFFCF, 0xFFF0, 0xFFF3, 0xFFFC, 0xFFFF
} ;

//下面的数据用于2 bit到1 bit字节转换时查表以加快转换速度
//把一个两bit字节转到一个bit中的高四位
const  UINT8 g_iTwoBit2OneBitHeight[256] =
{
    0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
    0x20, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30, 0x30, 
    0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
    0x20, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30, 0x30, 
    0x40, 0x40, 0x50, 0x50, 0x40, 0x40, 0x50, 0x50, 
    0x60, 0x60, 0x70, 0x70, 0x60, 0x60, 0x70, 0x70, 
    0x40, 0x40, 0x50, 0x50, 0x40, 0x40, 0x50, 0x50, 
    0x60, 0x60, 0x70, 0x70, 0x60, 0x60, 0x70, 0x70, 
    0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
    0x20, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30, 0x30, 
    0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
    0x20, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30, 0x30, 
    0x40, 0x40, 0x50, 0x50, 0x40, 0x40, 0x50, 0x50, 
    0x60, 0x60, 0x70, 0x70, 0x60, 0x60, 0x70, 0x70, 
    0x40, 0x40, 0x50, 0x50, 0x40, 0x40, 0x50, 0x50, 
    0x60, 0x60, 0x70, 0x70, 0x60, 0x60, 0x70, 0x70, 
    0x80, 0x80, 0x90, 0x90, 0x80, 0x80, 0x90, 0x90, 
    0xA0, 0xA0, 0xB0, 0xB0, 0xA0, 0xA0, 0xB0, 0xB0, 
    0x80, 0x80, 0x90, 0x90, 0x80, 0x80, 0x90, 0x90, 
    0xA0, 0xA0, 0xB0, 0xB0, 0xA0, 0xA0, 0xB0, 0xB0, 
    0xC0, 0xC0, 0xD0, 0xD0, 0xC0, 0xC0, 0xD0, 0xD0, 
    0xE0, 0xE0, 0xF0, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0, 
    0xC0, 0xC0, 0xD0, 0xD0, 0xC0, 0xC0, 0xD0, 0xD0, 
    0xE0, 0xE0, 0xF0, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0, 
    0x80, 0x80, 0x90, 0x90, 0x80, 0x80, 0x90, 0x90, 
    0xA0, 0xA0, 0xB0, 0xB0, 0xA0, 0xA0, 0xB0, 0xB0, 
    0x80, 0x80, 0x90, 0x90, 0x80, 0x80, 0x90, 0x90, 
    0xA0, 0xA0, 0xB0, 0xB0, 0xA0, 0xA0, 0xB0, 0xB0, 
    0xC0, 0xC0, 0xD0, 0xD0, 0xC0, 0xC0, 0xD0, 0xD0, 
    0xE0, 0xE0, 0xF0, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0, 
    0xC0, 0xC0, 0xD0, 0xD0, 0xC0, 0xC0, 0xD0, 0xD0, 
    0xE0, 0xE0, 0xF0, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0 
}; 
//把一个两bit字节转到一个bit中的低四位
const  UINT8 g_iTwoBit2OneBitLow[256] =
{
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03, 
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03, 
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05, 
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07, 
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05, 
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07, 
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03, 
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03, 
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05, 
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07, 
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05, 
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07, 
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09, 
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B, 
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09, 
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B, 
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D, 
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F, 
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D, 
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F, 
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09, 
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B, 
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09, 
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B, 
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D, 
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F, 
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D, 
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F
};
// dc.cpp end
