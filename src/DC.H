 

#ifndef _G301_DC_H
#define _G301_DC_H
#define      CIRCUMGYRATE 

// 位图装入地址
//#define G301_ICON_ADDR          (0x010D0000)	// for ROM
//#define G301_ICON_ADDR          (0x300000)		// for RAM

///////////////////////////////////////////////////////////////////////
//  位图操作宏
///////////////////////////////////////////////////////////////////////
#define gICON_ADDR(curIcon)	    ((*((UINT32*)(E201_ICON_ADDR + (curIcon-1) * 4))) + E201_ICON_ADDR)

#ifndef  CIRCUMGYRATE

#define gICON_WIDTH(curIcon)    (*((short *)(gICON_ADDR(curIcon <= BMPTOTAL ? curIcon : BMPTOTAL))))          
#define gICON_HEIGHT(curIcon)   (*((short *)(gICON_ADDR(curIcon <= BMPTOTAL ? curIcon : BMPTOTAL) + 2)))

#else

#define gICON_WIDTH(curIcon)    (*((short *)(gICON_ADDR(curIcon) + 2)))
#define gICON_HEIGHT(curIcon)   (*((short *)(gICON_ADDR(curIcon))))

#endif
 
//////////////////////////////////////////////////////////////////////
// 2001-08-23日           CDC类增加函数WriteScreen
//
#include "hwfcl.h"

#define LCD_WIDTH  160
#define LCD_HEIGHT 160

#define HZ_WIDTH     16                //汉字的显示宽度
#define ASC_WIDTH     8                //ASC字符的显示宽度
#define CHAR_HEIGHT  16

//#define ASC_FONT_ADDR  0x1100000  //长265792字节
//#define HZ_FONT_ADDR   (ASC_FONT_ADDR + 0x1000)

enum COMPRESS
{
	COMPRESS_NO,	// 不压缩
	COMPRESS_LZW
};

///////////////////////////////////////////////////////////////////////
//  四种颜色
///////////////////////////////////////////////////////////////////////
#ifndef BLACK
 #define BLACK 0
 #define GRAY  1
 #define LGRAY 2
 #define WHITE 3
#endif

///////////////////////////////////////////////////////////////////////
//CDC类做图属性
#define DRAWMODE_REP  0
#define DRAWMODE_AND  1
#define DRAWMODE_OR   2
#define DRAWMODE_XOR  3

///////////////////////////////////////////////////////////////////////
//汉字显示时的对齐方式
//垂直对齐方式
#define VALIGN_TOP    0     
#define VALIGN_MID    1
#define VALIGN_BOTTOM 2
//水平对齐方式
#define HALIGN_LEFT   0
#define HALIGN_CENTER 1
#define HALIGN_RIGHT  2

///////////////////////////////////////////////////////////////////////
// 描述:  图形系统初始化函数
//
// 参数:  无
//
// 返回:  无
//
// 说明:  使用CDC类之前需用此函数对图形系统初始化,此初始化函数必须在
//        调用Nucleus的图形初始化函数之后进行
//
extern "C" void HwInitGraphicsSystem();



///////////////////////////////////////////////////////////////////////
//使用CDC类保存屏幕的结构类型
struct  ScreenBuf
{
    INT16 x0, y0;
    INT16 x1, y1;
    UINT8 * pBuffer;
};


///////////////////////////////////////////////////////////////////////
//  CDC类
///////////////////////////////////////////////////////////////////////
class CDC
{
private:
//  static UINT8 m_bOutToVirtualScreen; 
    UINT8  m_iPenColor;         //笔颜色
    UINT8  m_iBackColor;        //背景色
    UINT8  m_iDrawMode;         //做图属性
    UINT8  m_iPenWidth;         //笔宽度
    INT16  m_iCurX, m_iCurY;    //当前位置
    UINT8  m_iValign;           //垂直对齐方式
    UINT8  m_iHalign;           //水平对齐方式 
    UINT8  m_iLineStyle;        //画线类型 
public:
    CDC();

public:
    /////////////////////////////////////////////////////////////////
    //CDC类的私有成员接口函数
    
    //设置当前点
    void MoveTo(INT16 iX, INT16 iY)
    {
        m_iCurX = iX;
        m_iCurY = iY;
    }

    //获得当前点
    void GetCurPos(INT16 &iX, INT16 &iY) const
    {
        iX = m_iCurX;
        iY = m_iCurY;
    }

    //设置笔的颜色,返回原来笔的颜色
    UINT8 SetPenColor(UINT8 iPenColor = BLACK)
    {
        UINT8 old = m_iPenColor;
        m_iPenColor = iPenColor & 3;
        return old;
    }

    //获取笔的颜色
    UINT8 GetPenColor() const
    {
        return m_iPenColor;
    }

    //设置背景色
    UINT8 SetBackColor(UINT8 iBackColor = WHITE)
    {
        UINT8 old = m_iBackColor;
        m_iBackColor = iBackColor &3;
        return old;
    }

    //获取背景色
    UINT8 GetBackColor() const
    {
        return m_iBackColor;
    }

    //设置做图时笔的宽度
    UINT8 SetPenWidth(UINT8 iWidth = 1)
    {
        UINT8 old = m_iPenWidth;
        m_iPenWidth = iWidth;
        return old;
    }

    //获得笔的宽度
    UINT8 GetPenWidth() const
    {
        return m_iPenWidth;
    }

    //设置做图属性，返回原来的做图属性
    UINT8 SetROP2(UINT8 iDrawMode = DRAWMODE_REP)
    {
        UINT8 old = m_iDrawMode;
        m_iDrawMode = iDrawMode;
        return old;
    }
     
    //返回做图属性
    UINT8 GetROP2() const
    {
        return m_iDrawMode;
    }

    
    //设置字符显显示时的对齐方式，返回原来对齐方式，低8位为垂直对齐方式，高8位为水平
    UINT16 SetAlign(UINT8 iHalign = HALIGN_LEFT, UINT8 iValign = VALIGN_TOP )
    {
        UINT16 old = MAKEWORD(m_iValign, m_iHalign);
        m_iValign = iValign;
        m_iHalign = iHalign;
        return old;
    }

    //获得对齐方式
    void GetAlign(UINT8 &iHalign, UINT8 &iValign) const
    {
        iValign = m_iValign;
        iHalign = m_iHalign;
    }
    
    //设置画线类型 0 -实线， 其它:每隔iStyle点空一点的虚线
    UINT8 SetLineStyle(const UINT8 iStyle=0)
    {
        UINT8 old = m_iLineStyle;
        m_iLineStyle = iStyle;
        return old;
    }

    UINT8 GetLineStyle() const
    {
        return m_iLineStyle;
    }

public:
    /////////////////////////////////////////////////////////////////
    //CDC类的公有成员函数，用当前的颜色、属性做图


    //从当前点画线到指定的ix, iy处
    //使用当前颜色，当前宽度，当前做图模式
    //笔宽度为0时画出虚线
    void LineTo(INT16 iX, INT16 iY);

    //在ix, iy处用当前的参数画一个点
    //使用当前颜色，当前宽度，当前做图模式
    void SetPixel(INT16 iX, INT16 iY);

    //用当前笔的颜色画一个填充矩形
    //使用当前颜色，其它参数无效
    void PaintRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
    //用当前笔的颜色画一个填充矩形
    //使用当前颜色，其它参数无效
    void PaintRect(CRect &srRec);

    //用当前背景色画一个填充矩形
    //使用当前背景颜色，其它参数无效 
    void EraseRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
    //用当前背景色画一个填充矩形
    //使用当前背景颜色，其它参数无效 
    void EraseRect(CRect &srRec);

    //用当前笔颜画一个矩形
    //使用当前颜色，当前宽度，当前做图模式
    void FrameRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
    //用当前笔颜画一个矩形
    //使用当前颜色，当前宽度，当前做图模式
    void FrameRect(CRect &srRec);

    //恢复所有参数的缺省值
    void Reset();

    //输出字符串,iX, iY为第一个字符的左上角坐标
    //使用当前背景颜色，当前笔颜色，当前宽度，当前做图模式
    void TextOut(INT16 iX, INT16 iY, char *pText, INT16 iLen=0);

    //输出字符串对齐方式由dc的成员决定
    //使用当前背景颜色，当前笔颜色, 当前宽度，当前做图模式,当前对齐方式
    void DrawText(INT16 iX, INT16 iY, char *pText);

    //在一个矩形类输出字符串,并自动换行
    void DrawText(CRect &srRect, char *pText);

    //用当前做图模式在x,y处显示一个图标
    void PutIcon(INT16 x, INT16 y, UINT32 iIcon);

    //用当前做图模式在x,y处显示一个字
    //如果是汉字，iChar中存放汉字的内码，如果是ASC字符，iChar的低字节为ASC码
    void PutChar(INT16 x, INT16 y, UINT16 iChar);

    //画椭圆
    void Ellipse(INT16 x0, INT16 y0, INT16 x1, INT16 y1);

    void Ellipse(CRect &srRect);

    //BOOL FloodFill(INT16 x, INT16 y);
    /////////////////////////////////////////////////////////////////
    //CDC类的静态公有函数，不用实例子化对象时也可调用
              
    //bScreen TRUE虚屏，FALSE实屏
//  static void SetOutScreen(UINT8 bOutToVirtualScreen = TRUE);

//  static void Invalidate(INT16 x0, INT16 y0, INT16 x1, INT16 y1);
//  static void Invalidate(rect *srRect);

    //计算字符串在当前显示情况下占屏幕的宽度
    static INT16 GetTextExtent(char *pText);
   
    //许算字符串在iLeft, iRight的宽度内显示的话，要用的高度是多少
    //如果字符串内含回车'\n'，也计算在内
    static INT16 GetTextHeight(INT16 iLeft, INT16  iRight, char *pStr);

    //用iColor颜色清除全部屏幕
    static void ClearDevice(UINT8 iColor = WHITE);

    //计算存储一个矩形区域所需的字节数
    static UINT16 ImageSize(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
    //计算存储一个矩形区域所需的字节数
    static UINT16 ImageSize(CRect &srRec);
    
    //在把pImgBuf中的内容写到屏幕上的矩形内
    static void PutImage(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom, 
                                      UINT8 * pImgBuf, UINT8 iPutAttr = DRAWMODE_REP);
    //在把pImgBuf中的内容写到屏幕上的矩形内
    static void PutImage(CRect &srRec, UINT8 * pImgBuf, UINT8 iPutAttr = DRAWMODE_REP);

    //读出屏幕上矩形的内容存放pImgBuf中
    static void GetImage(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom, UINT8 * pImagBuf);
    //读出屏幕上矩形的内容存放pImgBuf中
    static void GetImage(CRect &srRec, UINT8 *pImgBuf);
    
    //画一个三维的矩形,左边上边边框的颜色由cTopLeft指出，右边下边的颜色由cRightBottom指出
    static void Draw3dRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 cTopLeft = WHITE, UINT8 cBottomRight = GRAY);
    //画一个三维的矩形,左边上边边框的颜色由cTopLeft指出，右边下边的颜色由cRightBottom指出
    static void Draw3dRect(CRect &srRec, UINT8 cTopLeft = WHITE, UINT8 cBottomRight = GRAY);

    //保存屏幕一个矩形区域，并分配所需的内存，返回一个存储屏幕的结构指针
    static ScreenBuf * SaveScreen(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
    //保存屏幕一个矩形区域，并分配所需的内存，返回一个存储屏幕的结构指针
    static ScreenBuf * SaveScreen(CRect &srRec);
    
    //恢复屏幕，并释放保存时分配的内存
    static void PutScreen(ScreenBuf * pScrBuf, UINT8 iPutAttr = DRAWMODE_REP);
	//恢复屏幕，但不释放保存时分配的内存
	static void WriteScreen(ScreenBuf *pScrBuf, UINT8 iPutAttr = DRAWMODE_REP);

    //反向显示一个矩形
    static void InvertRect(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom);
    //反向显示一个矩形
    static void InvertRect(CRect &srRec);
    
    //画一个3D按钮
    static void Draw3dButton(INT16 iLeft, INT16 iTop, INT16 iRight, INT16 iBottom, char *pCaption);
    //画一个3D按钮
    static void Draw3dButton(CRect &srRec, char *pCaption);
};

extern "C" void OpenCursor();

extern "C" void CloseCursor();

extern "C" void SetCursorXY(short  iX,  short iY);
//iBlink为0时静止
extern "C" void SetCursorAttr(unsigned char iWidth, unsigned char iHeight);
extern "C" BOOL GetCursorStatus();
extern "C" void GetCursorXY(short * iX, short * iY);
extern "C" void HwBitBlt(void * pSrcAddr, UINT16 iSrcFrmWidthBytes, UINT16 x0, UINT16 y0, UINT16 iWidth, UINT16 iHeight,\
                 void * pDesAddr, UINT16 iDesFrmWidthBytes, UINT16 x1, UINT16 y1, UINT8 iDrawMode);
#endif  //cdc.h 
