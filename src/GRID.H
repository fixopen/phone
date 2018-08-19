/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             grid.h                                                         */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技网格控件类的头文件                                     */
/*             HanWang Technology E-PhoneII grid control class header file    */
/*                                                                            */
/*  组    成                                                                  */
/*             CGrid类声明                                                    */
/*                                                                            */
/*  设计说明                                                                  */
/*             参见《网格控件设计说明书》文档                                 */
/*                                                                            */
/*  作    者                                                                  */
/*             郝庆丰                                                          */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-24                                                     */
/*                                                                            */
/*  修改记录                                                                  */
/*             2001-07-26                                                     */
/*             改正了一个bug, 现象是单元格折行显示时，                        */
/*             当选中后行与行之间会出现白色线条                               */
/*             2001-08-04                                                     */
/*             增强了单元格的功能，即有图标的单元也可换行，但图标必须为16X16  */            
/*             2001-08-06                                                     */
/*             增加GetRectFromClick函数,即可以得到笔点中的单元的大小和位置    */
/*             2001-08-08                                                     */
/*             改正了SetUnitText中的bug                                       */
/*             2001-08-09                                                     */
/*             增加GetTotalRows函数                                           */
/*			   2001-12-26                                         */
/*             add ConverTest() ,modify SetStartRow()  余国荣                 */	
/******************************************************************************/

#ifndef  _GRID_H_
#define  _GRID_H_

#include "hwfcl.h"
#include "combox.h"
#include "scrollbar.h"

#define TBS_SPACELINE     0x100     //表格风格，单元是否有间隔线
#define TBS_HAVFIXROW     0x200     //表格风格 有固定行
#define TBS_ROWSELECT     0x400     //表格风格，行选中
#define TBS_NOVERLINE     0x800     //表格风格，是否没有竖线

#define UNS_TEXT    0x00          //表格单元风格，是否只是显示一行文本
#define UNS_COMBOX  0x01          //表格单元风格，是否是下拉控件

#define RS_NORMAL                 0x00    
#define RS_DISABLE                0x08    //行的风格，是否允许显示
#define RS_CANNOTSELECT           0x10	  //行的风格，不允许被选中


#define TABLE_ROW_HEIGHT   17     //表格单行高度
#define TABLE_MAX_COLS     4      //表格最大列数
 
#define INVALID_ICON_ID    0      //如果单元格没有图标，图标ID应设为此值

#define GRID_IDSCROLLBAR   0x0FFFFE    //滚动条ID
#define GRID_BACK_COLOR    WHITE       
#define GRID_FORE_COLOR    BLACK
#define LINE_COLOR         GRAY

#define MAX_SUB_LINES      64    //
#define MAX_UNITTEXT_LEN   256   //一个单元格允许的最大字符个数，不包括'\0'
#define GRID_ICON_WIDTH    16
#define RD_DRAWALL                0xFFFF  

typedef struct
{
  UINT32 nIconIndex;  //图标的索引值，为0表示没有
  char  style;        //风格，是否包含组合框
  char  *pText;       //单元格文本
}GridColData;  //网格单元格数据的结构

typedef struct GRIDROWDATA
{
  GridColData  pColData[TABLE_MAX_COLS];
  GRIDROWDATA *pPrev;
  GRIDROWDATA *pNext;
  BYTE         cChilds;//折行的个数
  char         style;  //风格
}GridRowData; //每一行数据的结构


//网格控件说明：第一行、第一列索引为（1，1），例如要在第一行前插入一行，
//InsertRow函数中的iBeforeRow 应该是1，其他亦然；另如果插入的行某单元格
//无图标，相应的图标索引值应设为0。  
 
class CGrid : public CCtrl
{
public:
	CGrid();
	~CGrid();
	BOOL   Create(CFrameWnd * pParent , CRect &rectPos , UINT32 dwStyle, 
		     int iRows , int iCols , UINT32 nID =0 , BOOL bCanSel = FALSE);
	//创建控件
	void   InsertRow(UINT16 iBeforeRow ,char *pText[] , char *pStyle , 
		     UINT32 *pIconID, BOOL bRedraw =FALSE);
	//插入一行，iBeforeRow是待插的行号，如 ＝ 1 则在第一行前插入
	void   AddRow(char *pText[], char *pUnitStyle , UINT32 *pIconID , BOOL bRedraw = FALSE);
	//添加一行
	void   DeleteRow(UINT16 iRow , BOOL bRedraw = FALSE); 
	//删除一行
	void   RemoveAll();
	//删除所有行
	void   SetUnitText(UINT16 iRow , UINT16 iCol , char *pText , UINT32 nIconID = 0, BOOL bRedraw = FALSE) ;
	//设定单元格的内容（包括图标）
	UINT16 GetUnitText(UINT16 iRow , UINT16 iCol , char *pBuf, UINT16 nBufLen);
	//得到单元格的文本，pBuf 为存放文本的缓冲，返回字符个数
	void   SetUnitIcon(UINT16 iRow , UINT16 iCol , UINT32 nIconID , BOOL bRedraw = FALSE);
	//设定单元格图标
	INT32 GetUnitIcon(UINT16 iRow , UINT16 iCol);
	//得到单元格图标ID
	void   SetColsWidth(UINT16 *pWidth , BOOL bRedraw = FALSE);
	//设定列宽
	INT16 GetColsWidth(UINT16 col);
	//得到列宽
	UINT32 GetSelectedRowCol(){return MAKELONG(LOWORD(m_iSelectedRow), m_iSelectedCol);}
	//得到当前选中的行号和列号
    void   Destroy(); 
	//销毁控件
	BOOL   GetRectFromClick(int iRow , int iCol, CRect &rcUnit);
	//得到单元的大小和位置,如果点击的地方没有单元格，那么rcUnit为空。
	UINT16 GetTotalRows();
	//得到共有多少行
	void   SetUnSel();
	//设置当前选中的项为空
	void   SetSelRow(INT16 iRow, BOOL bRedraw = FALSE);
	//设置当前选中的项(注意：当选中项在屏幕外时，会自动滚屏显示。)
	void   SetStartRow(INT16 iRow , BOOL bRedraw = FALSE);
	//设置显示的起始行
	INT16  GetScreenRows(){return m_iScreenRows;}//得到一屏的行数
	CCtrl  *GetScrollBar(){return m_pScrollBar;}
	virtual void OnPaint(CDC &dc);
protected:
    virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
    
	virtual void    PaintGrid();
	virtual void    PaintRows();
	virtual void    PaintRow(UINT32 iRow);
//	void    PaintSingleRow(INT16 iRow);
	void    ScrollTo(UINT32 iRow);
	void    PaintEmptyRows();
	void    MoveDisplayRowPointer(INT16 iNum);
//	void    ResetRowComboxes();
	void    DoPenDown(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	void    DoPenMove(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	void    DoPenUp(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	void    GetUnitFromPt(INT16 x , INT16 y, int& iRow , int& iCol);
	BOOL    GetRectFromUnit(int iRow , int iCol , CRect &rcUnit);
	GridRowData*  Convert(UINT32 appeRow , UINT32 &pcrow);
	GridRowData*  GetRowPointer(UINT16 iRow);	
	UINT32  UnConvert(UINT32 iRow);
	void    FreeRow(GridRowData *pRow);
	UINT16  GetSubStringAddr(char *pText , UINT16 nUnitLen, UINT32 *pSubAddr);
	void    DeleteScrollbar();
private:
	CScrollBar *m_pScrollBar;
	int    m_iActualRows , m_iScreenRows , 
		   m_iCurRow , m_iAppearRows , m_iSelectedRow;
	//m_iStartRow的低16位表示初始的大行号，高16位表示初始的小行号
	int m_iTotalCols , m_iCurCol , m_iSelectedCol;
	UINT32 m_iStartRow, m_iEndRow;
	char m_cUnitStyle; // m_cStyle ,
	GridRowData *m_pRowHead , *m_pDisplayRow;
	UINT16 m_iColsWidth[TABLE_MAX_COLS];
	BOOL   m_bCanSel;
};

#endif
