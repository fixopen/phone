/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             计算器.cpp                                                     */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技计算器应用、单位换算类的源文件                         */
/*             HanWang Technology E-PhoneII Card Application class header file*/
/*                                                                            */
/*  组    成                                                                  */
/*             计算器应用所有类、单行编辑器类定义                             */
/*                                                                            */
/*  设计说明                                                                  */
/*             参见《计算器应用设计说明书》文档                               */
/*                                                                            */
/*  作    者                                                                  */
/*             郝庆丰                                                         */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-10-17                                                     */
/*                                                                            */
/*  修改记录                                                                  */
/*             2001-10-17                                                     */
/*             修改了单位转换中光标显示的bug                                  */
/*             2001-10-25                                                     */
/*             修改了单位转换中当数字太大时科学计数法显示的bug                */
/*             2001-10-25                                                     */
/*             修改了计算器当数字太小时显示的bug（ConvNumtoStr, HwPutText）   */
/*             2001-10-31                                                     */
/*             修改了CConvWnd::OnCreate函数（修改了控件的尺寸）               */
/*             2001-11-01                                                     */
/*             修改了CCalcWnd::DoMemOp函数                                    */
/*             2002-1-16                                                      */
/*             修改了CCalcWnd::OnPaint(CDC &dc)函数                           */
/******************************************************************************/
#include   "hwfcl.h"
#include   "window.h"
#include   "softkey.h"
#include   "str.h"
#include   "combox.h"

#define  LOWBYTE(a)            ((BYTE)(((WORD)a)&0x00FF))
#define  HIGBYTE(a)            ((BYTE)((((WORD)a)&0xFF00)>>8))

#define             CHAR_BACK               ('\b')  // 回删字符的键值
#define             CHAR_CLEAR              27          // 清除编辑框内容的键值
#define             LEFT_ALIGN              0           // 编辑控件风格-字符靠左排列
#define             RIGHT_ALIGN             1           // 编辑控件风格-字符靠右排列
#define             BLOCK_SIZE              64          // 基本内存块的大小

////////////////////////////////////////////////////////////////////////////
//单行编辑控件的声明
typedef struct tagSinEdit         // 单行编辑控件的存放数据的结构
{
	char  *pEditText; // 编辑框文本值
	int   nCurLen;                 // 文本的字节数
	CPoint CurPos;                 // 存放光标的位置
	CRect  rcArea;                 // 编辑框的区域范围
	short Style;                  // 编辑框字符的对齐风格
	short BlockNum;               // 所用的存放编辑控件文本内存块的个数
	short nMaxNum;                // 已分配的内存块的个数
}SINEDIT;

class CSinEdit  : public CCtrl
{
public:
	CSinEdit();
	virtual ~CSinEdit();
	BOOL  Create(CRect  rc, CWnd *parent ,  int ID ,short style=0 ,short maxnum = 64 , UINT32 wndStyle = WS_VISIBLE);
	void  SetWindowText(char *pText , BOOL bShowcur=TRUE);
	char  *GetWindowText();
	void CloseCur();
	void ShowCur();
	INT16 SetBackColor(INT16 iColor = WHITE);
protected:
	SINEDIT   m_edData;
	INT16     m_iBackColor;
	virtual void OnPaint(CDC &dc);
	virtual void Destroy();
	char *GetPrevChar(char *pText);
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);

    BOOL InitEdit();
	BOOL IsAscChar(short chJge);
	void CreateCursor();
	void SetCurPos(short pX  , short pY);
	void SetCurPos(CPoint pt);
	void EmptyWindow();
	char * GetNextChar(char *pText);

	BOOL AppendChar(short apch);
	void BackChar();
	void DoClearOper();
};

////////////////////////////////////////////////////////////////////////////
//计算器的声明

void ConvAppRun(); //单位换算应用的入口函数
void CalcAppRun(); //计算应用的入口函数

#define         CALC_NUM_WIDTH          11             // 数字显示宽度
#define         CALC_NUM_HEIGHT         15              // 数字显示高度
#define         CALC_SHOW_LEN           10              // 数字显示长度
														//modefied on 2.11
#define            MAX_NUM            99999999.0        //计算的最大范围
#define            DIM                0.0000001         //计算的精度
#define            COM_ERR            0                 //计算溢出
#define            MEM_ERR            1                 //内存数溢出

/*
*	计算器键值定义
*/
#define			CAL_ADD				20
#define			CAL_SUB				21
#define			CAL_MUL				22
#define			CAL_DIV				23

#define			CAL_MC				13
#define			CAL_MR				14
#define			CAL_MADD			15
#define			CAL_MSUB			16

#define			CAL_0				0
#define			CAL_1				1
#define			CAL_2				2
#define			CAL_3				3
#define			CAL_4				4
#define			CAL_5				5
#define			CAL_6				6
#define			CAL_7				7
#define			CAL_8				8
#define			CAL_9				9
#define			CAL_SIGN			10
#define			CAL_POINT			11

#define			CAL_C				30
#define			CAL_CE				31

#define			CAL_EQU				100
#define			CAL_BACKSPACE		101
#define			CAL_SQR				104
#define			CAL_REC				105		// 倒数
#define			CAL_PERCENT			106

/*
*	标题尺寸
*/
#define			CONV_BUTTONWIDTH	70
#define			EXIT_BUTTONWIDTH	18
#define			CAL_TITLE_HEIGHT	18

typedef struct tagCalData  //计算器应用的数据结构定义
{
	  double cdOpNum1;     // 操作数1
	  double cdOpNum2;     // 操作数2
	  double cdscNum;      // 屏幕数
	  double cdmemNum;     // 内存数
	  short cdnLen;        // 屏幕数长度
	  char cdprevOp;       // 前一个点击的操作符
	  char cdscText[25];   // 屏幕字符串
	  char cdOperator;     // 操作符
	  int   cdprevKey;     // 前一个键值

	  char cdCanBack;      // 能否回删
	  char cdIsNum2Null;   // 操作数2是否为空
	  char cdbCon;         // 能否继续运算
	  char cdCanOp;        // 能否继续
	  char cdbCanchg;
	  char cdbHasError;
} CALDATA;

//白亚伟提供的函数 , 显示字符串 ,大小24 * 16
void PutCalcString(char  *buf, UINT16 x, UINT16 y);

//////////////////////////一些全局函数////////////////////////
int   FindChar (char * pStr , char ch);  // 在字符串中查找指定字符, 返回索引值
void ConvNumtoStr(double num , char *pStr , BOOL bScie =FALSE);  // 转换数字成字符串  
double  ConvStrtonum(char *pStr);   // 把字符串转换成数字
char * RightTrim(char *pStr , BOOL bEndHasDot = TRUE);    // 去掉字符串右边的空格

class CCalcSoftKey : public CSoftKey
{
public:
	virtual void OnPaint(CDC &dc)
	{
		return;
	}
};

class CCalcWnd  :public CAppWnd  
{
public:
	CCalcWnd();
	virtual ~CCalcWnd();
   
	MESSAGE_HANDLE  void OnKeyBoard2Click(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE  void OnConvBtnClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	//其他键
	DECLARE_MESSAGE_MAP(CCalcWnd)
	
protected:
	CALDATA m_cdData;	
//	int SoftKey1Draw(CTRL *ctrl);   //软件盘1的自画函数
//	int SoftKey2Draw(CTRL *ctrl);   //软件盘2的自画函数

	virtual  BOOL  OnCreate();  
	virtual  BOOL  OnDestroy();
	virtual  BOOL  PreCreateWindow();
    virtual  void  OnPaint(CDC &dc);  // 窗口的自画函数
	CRect   m_scrc;                 // 窗口的大小
	
	void DoNumHan(short num );  // 处理数字的函数
	void DoBackSpc();    // 处理"backspace" 键
	void DoSymbolOp();   // 处理"+/-"号
	void DoEqual();      // 处理"=" 号
    void DoDbleOp(char opType);  // 处理"+ 、- 、 * 、/"
	void DoClearOp(); // 处理 "CE" 键
	void DoSqrtHan(); // 处理 "sqrt" 键
	void DoClear(UINT32 lParam);	// 处理清除键
	void ResetElem();      // 复位数据的值
	char * GetScText();  // 得到屏幕数的字符串
	void AppendChar(char *pStr ,char ch); //  向字符串后添加一个字符
	double GetScreenNum();  // 得到屏幕数的大小
	void ResetSctext(); // 清空屏幕数字符串
    void HwPutText(char *str);   // 输出字符串
	void ShowErrorMsg( );  // 显示错误信息
	void HanOverFlow( double num , char cType = COM_ERR);  // 处理溢出的情况
	void EmptyEdit();  // 清空显示框
	

    void  DoPerSymbol();  // 百分号的处理函数
	void  DoRecHan();  // 倒数的处理函数
	void  DoMemOp(short opType);  // 内存数的处理函数
	void  EmptyMem();  // 清空内存数
	
	CCalcSoftKey m_softkey2 ;
	CButton   m_btnConv;
};

#define             MAX_TYPES           8               // 单位类型的最大值
#define             MAX_NAMES           11              // 一种单位的最大单位名称
#define             MAX_SIZES           55              // 存放转换系数数组的列数
#define             TYPE_TEMPER         4               // 温度单位类型的索引   

const short g_pUnitType[MAX_TYPES+1]={STR_IDLENGTH ,  STR_IDAREA , STR_IDWEIGHT , STR_IDCAP,
  STR_IDTEMP ,  STR_IDPOWER  , STR_IDENERGY  ,STR_IDPRESS ,-1 }; // 单位类型种类字符串ID数组

const short g_pUnitName[MAX_TYPES][MAX_NAMES+1]={ // 保存单位名字符串ID的数组
		{STR_IDMETER , STR_IDFEET , STR_IDSHICHI , STR_IDDM , STR_IDCM , STR_IDMM ,
              STR_IDYARD ,  STR_IDINCH  ,   STR_IDSM , STR_IDKM , STR_IDMILE ,-1} , 
		
		{STR_IDSQRTMT  , STR_IDSQCHI ,  STR_IDSQKM  ,STR_IDLG    , STR_IDARE ,STR_IDACRE   ,
 	         STR_IDMU , STR_IDSQYARD  ,STR_IDHEKTARE ,STR_IDSQMILE  ,-1} , 
		
		{ STR_IDKG  ,STR_IDJIN , STR_IDLIANG ,  STR_IDPOUND , STR_IDGRAM , STR_IDCARAT ,
		      STR_IDOUNCE , STR_IDTON ,-1 }, 
		
		{ STR_IDLITRE , STR_IDGALAME,  STR_IDGALENG , STR_IDCUBCUN , STR_IDCAPOUNCE , 
		    STR_IDPINT , STR_IDQUART , STR_IDBUSHEL , STR_IDCUBYARD , STR_IDCUBM  ,-1} ,
		
		{STR_IDSHESHI ,  STR_IDHUASHI ,STR_IDKAISHI ,-1  } ,
		
		{ STR_IDW  , STR_IDHPAME  , STR_IDHPENG ,-1 } ,
		
		{  STR_IDQIANK , STR_IDJIAO   , STR_IDKWH ,-1 },
		
		{ STR_IDMH   ,	 STR_IDP ,	 STR_IDSGP  ,-1 } 
};

  /*"平方米",\
	"平方英尺",\
	"平方公里",\
	"坪",\
	"公亩",\
	"英亩",\
	"亩",\
	"平方码",\
	"公顷",\
	"平方英里",\*/
const double g_fConvPro[MAX_TYPES][MAX_SIZES+1]= {  //保存转换系数的数组
		{11 ,3.28084, 3.0,10.0 ,100.0,1000.0,1.09361, 39.37008 , 0.00054, 0.001, 6.2137e-4 ,
			0.9143999 ,3.0479999 ,30.479999 ,304.79999, 0.3333323, 12.0 ,1.6459e-4 ,3.0479e-4 ,1.8939e-4 ,
			3.3333333,33.333333,333.333333, 0.3645366 ,13.12336 ,1.8e-4 ,3.3333e-4 ,2.0712e-4 ,
			   10.0, 100.0, 0.109361 ,3.937008,5.4e-5 , 1.0e-4 ,6.2137e-5,
			   10.0 ,0.0109361, 0.3937008 ,5.4e-6, 1.0e-5, 6.2137e-6, 
			   0.00109361, 0.03937008 , 5.4e-7 , 1.0e-6 ,6.2137e-7,
			   36.000109 , 4.9377e-4 , 9.144e-4, 5.6818e-4 , 
			   1.3715e-5 , 2.5399e-5 , 1.5782e-5,1.8518518 , 1.1506851 , 0.62137 } , //长度
		
		{10 ,10.76391,1.0e-6,0.3025 ,0.01, 2.471e-4, 1.5e-3 ,1.19599 ,1.0e-4 ,3.861e-7,
		  9.2903e-8,0.0281031, 9.2903e-4, 2.2956e-5 , 1.3935e-4,0.1111111, 9.2903e-6 ,3.587e-8 ,
		  3.025e+5 , 10000.0 ,247.1, 1500 , 1.19599e+6 , 100.0 ,0.3861016 ,  //公里转换成其他
		  8.1687e-4, 4.9586e-3 ,4.9586e-3,3.9536859, 3.3057e-4, 1.2763e-6 ,
		  0.0247105, 0.15 , 119.599,0.01 ,3.861e-5,	
		  6.0702841, 4839.9994, 0.4046856, 1.5624e-3 , 
		  797.32666,0.0666666, 2.574e-4,8.3612e-5,3.2283e-7,3.861e-3} ,  // 面积
        
		{8 ,2.0 , 20.0 ,2.2046 , 1000.0 ,5000.0, 35.274 ,0.001 ,
		   10.0 , 1.1023, 500 , 2500 , 17.637, 5.0e-4 , 
		   0.11023 , 50.0 , 250 , 1.7637, 5.0e-5 , 
		   453.59702, 2267.9851, 16.000181, 4.5359702e-4, 
		   5.0 ,0.035274, 1.0e-6, 7.0548e-3, 2.0e-7, 2.797985e-5 },   //重量
		
		{10 ,0.2642, 0.22 , 27.0, 33.815 , 2.1134, 1.0567, 0.0284 , 1.308e-3, 1.0e-3, 
		  0.8327024,102.1953, 127.99015 ,7.9992429,	3.9996214, 0.1074943, 4.9507e-3, 
		   3.785e-3, 122.72727 , 153.70454, 9.6063636, 4.8031818 ,0.1290909, 5.9454e-3,
			4.5454e-3, 1.2524074, 0.078274, 0.039137 , 1.0518e-3, 4.8444e-5, 3.7037e-5, 
			0.0624988 ,0.0312494, 8.3986e-4, 3.8681e-5, 2.9572e-5, 
			0.5, 0.013438, 6.189e-4 , 4.7317e-4, 0.0268761 , 1.2378e-3 ,9.4634e-4,
			0.0460563,0.0352112, 0.7645259},
		
		{3 ,273.15,1.8 , 1.8},    {3 , 1.3605e-3, 1.341e-3 , 0.9856513}, 
		{3, 4186.8 ,1.163e-3, 2.7777e-7},     {3, 133.322, 1.3157e-3,9.8692e-6}
};

//////////////////////////////////////////////////////////////////////
//下面是单位换算的声明
class C3DSinEdit : public CSinEdit
{
public:
	virtual void OnPaint(CDC &dc)
	{
		CRect r(m_sRect);
		dc.SetBackColor(m_iBackColor);
		dc.EraseRect(r);

		dc.FrameRect(r);
		r.DeflateRect(1, 1);
		dc.SetPenColor(GRAY);
		dc.FrameRect(r);
		dc.SetPenColor(WHITE);
		dc.MoveTo(r.left+1, r.bottom);
		dc.LineTo(r.right , r.bottom);
		dc.MoveTo(r.right, r.top);
		dc.LineTo(r.right , r.bottom);

		dc.Reset();
		if(m_edData.Style==0)
		{
			m_edData.CurPos.x = 2;
			m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT -1;
			SetCurPos(m_edData.CurPos);
		}
		else if(m_edData.Style ==1)
		{
			m_edData.CurPos.x = m_edData.rcArea.right-2 ;
			m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT -1;
			SetCurPos(m_edData.CurPos);
		}
	}
};
	
#define  CS_CLOSE                  0
#define  CS_POPUP                  (!CS_CLOSE)
	
typedef  struct  tagUnitConv
{
	short      UnitType; //当前单位类型
    short     FocusEditID; //当前要被转换的源数据的edit 控件标记
	short     FocusListID;  
    short      nSrcSel;  //当前转换的源单位索引
    short      nDstSel;  //当前转换的目的单位索引 
    double     dSrcNum;  //当前转换的源数值
	double     dDstNum;  //当前转换的目的数值
}UNITCONV;
    
class CConvWnd :public CAppWnd
{
public:
	CConvWnd();
	virtual ~CConvWnd();
    DECLARE_MESSAGE_MAP()
protected:
	void DoConv();  //进行转换操作
	void DoTempConv();   // 进行温度转换(因为温度换算公式特殊,单独列出)
	virtual void OnPaint(CDC &dc);  
	MESSAGE_HANDLE  void OnListSel(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE  void OnSoftKeyClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE  void OnSrcEditClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE  void OnDstEditClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE  void OnBtnClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	void   ResetEditData();  // 复位编辑控件的内容
	C3DSinEdit *GetFocusEdit();
	UNITCONV   m_ucData;
	CCombox  comType , comUnitSrc , comUnitDst;  // 单位类型、名称组合框控件
	BOOL     m_bComStatus[3];
	CButton      btnClear , btnDot ,btnConv , btnCalc;      // “清除”、“.”、"换算"按钮
	CSoftKey     skNum;                        // 软键盘,用来输入数字
	C3DSinEdit     seSrc ,seDst;     // 源编辑控件,目的编辑控件
	BOOL OnCreate();               
};

#define  TIP_CHAR_HEIGHT                   7

int   FindChar (char * pStr , char ch)
{
	int len = strlen(pStr);
    int pos = -1;
	for(int i=0 ; i<len ;i++)
	{
		if(pStr[i]==ch)
		{
			pos=i;
			break;
		}
	}
	return pos;
}

char * RightTrim(char *pStr , BOOL bEndHasDot)
{
     int len = strlen(pStr);
	 int i;
	 i = FindChar(pStr , '.');
	 if(i==-1) return pStr;
	 for(i =len-1 ;i>0;i--)
	 {
		 if(pStr[i]!='0'||pStr[i]=='.')
			 break;
		 pStr[i]=0;
	 }
	 if(!bEndHasDot)
	   if(pStr[i]=='.')
	 	    pStr[i]=0;
	 return pStr;
}
/*
      函数说明: 转换字符串pStr为数字, pStr中可以有e、.、1、2、...、9、-
*/
double  ConvStrtonum(char *pStr)
{
	int  len, t,s, i;
	double res;
	len = strlen(pStr);
	int pos = FindChar(pStr , '.');
	int pos2= FindChar(pStr , 'e');
	res=0;
	s=0;
	if(pStr[0]=='-')
        s=1;
    if(pos2==-1)
	{
		if(pos>=0)
		{
			for( i =s ;i<len ;i++)
			{
				t= pStr[i]- 0x30;
				if(i<pos)
				  res+=(t *pow(10, pos-i-1));
				else if(i>pos)
				  res+=(t *pow(10, pos-i));
			}
		}
		else
		{
			for( i =s ;i<len;i++)
			{
				t= pStr[i]- 0x30;
				res+=(t *pow(10, len-i-1));
			}
		}
	}
	else 
	{
		if(pos>=0)
		{
			for( i =s ;i<pos2 ;i++)
			{
				t= pStr[i]- 0x30;
				if(i<pos)
				  res+=(t *pow(10, pos-i-1));
				else if(i>pos)
				  res+=(t *pow(10, pos-i));
			}
		}
		else
		{
			for( i =s ;i<pos2;i++)
			{
				t= pStr[i]- 0x30;
				res+=(t *pow(10, pos2-i));
			}
		}
		t=0;
        for(i= pos2+2;i<len;i++)
		    t+=((pStr[i]-0x30) * pow(10 , len -i-1));
		res *=pow(10 ,t);
	}

    if(s==1)
		res = 0.0- res;
	return res;
		
}

BOOL LeftTrim(char *pStr)  //去掉字符串左边的'0'
{
	if(!pStr)
		return FALSE;
	char pTemp[25];
	int i ,len;
	len = strlen(pStr);
	for(i=0 ; i<len; i++)
	{
		if(pStr[i]!='0')
	        break;
	}
	memcpy(pTemp , pStr + i , len - i);
	memcpy(pStr, pTemp, len - i );
	pStr[len - i] = 0;
    return TRUE;
}
/*
   函数说明： 把数字转换成字符串
   参数：num 是要转换的数字， bScie决定是否需要科学计数法
*/
void ConvNumtoStr(double num  ,  char *pStr, BOOL bScie ) 
{
	char pTemp[25];
	char pTemp2[25];
	int len, pos;
	double tem , tem2;
	if(!bScie)
	{
		sprintf(pStr, "%8f",num); // hqf 2-27
		pos = FindChar(pStr , '.');// 
		if(pos!=-1)
		{
			memcpy(pTemp , pStr , pos);
			pTemp[pos] = '\0';
			tem = ConvStrtonum(pTemp);
			tem2 = num - tem;
            if(fabs(tem2) < DIM)
				num = tem;
		}
		pStr[0] = '\0';
		sprintf(pStr, "%8.7f",num);
		pStr[24] = '\0';
	    RightTrim(pStr);
	}
	else 
	{
		if(num==0.0)
		{
			sprintf(pStr, "%f" , num);
			RightTrim(pStr ,FALSE);
		}
		else if(fabs(num)>MAX_NUM || fabs(num)<0.01)
		  sprintf(pStr , "%8.4e" , num);
		else
		{
		  sprintf(pStr, "%10.6f", num);
		  RightTrim(pStr ,FALSE);
		}
	}
	pos =FindChar(pStr , 'e');
	len = strlen(pStr);
	if(pos!=-1)
	{
		memcpy(pTemp , pStr , pos);
		if(len > 9)
		  pTemp[pos-1]=0;
		else 
		  pTemp[pos]=0;
        RightTrim(pTemp, FALSE);
        memcpy(pTemp2, pStr+pos+2 , len -pos -2);
		pTemp2[len-pos-2]=0;
		LeftTrim(pTemp2);
		len =strlen(pTemp);
		memcpy(pStr, pTemp,len );
		memmove(pStr+len, pStr+pos, 2);
		pos = strlen(pTemp2);
		memcpy(pStr+len+2, pTemp2, pos);
		pStr[len+2+pos]=0;
	}
	else
	    RightTrim(pStr,FALSE);
}


/*
*	控件 ID 定义
*/
#define             ID_SOFTKEY1					1201
#define             ID_SOFTKEY2					1202
#define             ID_CALCADV					1203
#define             ID_ADVANCE					1204
#define				ID_CONVBUTTON				1205

BEGIN_MESSAGE_MAP(CCalcWnd, CAppWnd)
   //{{
    //add your message hander here
	 ON_MESSAGE(ID_CONVBUTTON,  CN_COMMAND , OnConvBtnClick)
	 ON_MESSAGE(ID_SOFTKEY2,  WM_KEY , OnKeyBoard2Click)
   //}}
END_MESSAGE_MAP


CCalcWnd::CCalcWnd()
{
	ResetElem();
	m_cdData.cdmemNum =0.0;
	m_cdData.cdbHasError= FALSE;
}

CCalcWnd::~CCalcWnd()
{

}

BOOL CCalcWnd::PreCreateWindow()
{
	return TRUE;
}

/*
   函数说明： 创建计算器需要的控件
   参数：无
   返回值：TRUE表示创建成功， FALSE 表示失败
*/
BOOL CalcuRectValue(CRect *pRectArray , int iRows , int iCols , CRect rectTotal , 
					short xInterval , short yInterval)
{
	if(rectTotal.Width() <= xInterval || rectTotal.Height() <= yInterval)
		return FALSE;
	short sBlockHeight , sBlockWidth , i ,j;

	sBlockHeight = (rectTotal.Height() - (iRows -1) * yInterval)/ iRows;
	sBlockWidth = (rectTotal.Width() -(iCols -1) *xInterval)/iCols;
	if(sBlockHeight <=0 || sBlockWidth <=0)
		return FALSE;
	for(i = 0; i< iRows ; i++)
	    for(j = 0;j< iCols;j++)
			pRectArray[i * iCols + j].SetRect(rectTotal.left + j *(sBlockWidth + xInterval) , 
				rectTotal.top + i * (sBlockHeight + yInterval) , rectTotal.left + j *(sBlockWidth + xInterval)
				+ sBlockWidth -1 , rectTotal.top + i * (sBlockHeight + yInterval) + sBlockHeight -1);
	return TRUE;
}

BOOL CCalcWnd::OnCreate()
{
	CAppWnd::OnCreate();
/*	char *pTemp1[]={"CE" , "C"};
	char *pTemp2[]={"MR/C" ,"M-" ,"M+","back","sqrt","1/x","%","/","7","8" ,"9","*","4","5","6","-",
		"1", "2", "3","+","0","+/-",".","="};*/
	unsigned short pKeyVa2[]={CAL_MR, CAL_7, CAL_8, CAL_9, CAL_DIV, \
							  CAL_MC, CAL_4, CAL_5, CAL_6, CAL_MUL, \
							  CAL_MADD, CAL_1, CAL_2, CAL_3, CAL_SUB, \
							  CAL_REC, CAL_0, CAL_POINT, CAL_SIGN, CAL_ADD, \
							  CAL_SQR, CAL_BACKSPACE, CAL_CE, CAL_C, CAL_EQU};
	CRect rectArray2[25];
	CalcuRectValue(rectArray2, 5, 5, CRect(3, 46 , 155, 159), 3, 2);
		
	m_softkey2.Create(0, 25, rectArray2 ,pKeyVa2, this, ID_SOFTKEY2);
	m_btnConv.Create("单位换算",
		CRect(m_sRect.left + TITLE_WIDTH , m_sRect.top, m_sRect.right - WND_TITLE_HEIGHT, m_sRect.top + CAL_TITLE_HEIGHT - 1), this, ID_CONVBUTTON);
	m_scrc.SetRect(6, 25, 153, 39);
	/*ResetElem();
	m_cdData.cdmemNum =0.0;
	m_cdData.cdbHasError= FALSE;*/
	//btnAdv.Create("高级" , CRect(60 ,24 ,94 , 42),GetWindowStruct(),1103);
	
//	wnd->exitProc = (int (*)(WINDOW))Delete;
	return TRUE;

}

/*
   函数说明：窗口的自画函数， 显示一幅位图，输出文本
   参数：pWnd是窗口指针
*/
void CCalcWnd::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.PutIcon(0 ,20 ,ICON_CALCULATOR);
	char strText[25];
//	HwPutText(m_cdData.cdscText);  
	::ConvNumtoStr(m_cdData.cdscNum , strText); 
	HwPutText(strText);  
	if(!m_cdData.cdCanOp)
	{
		CDC dc;
		dc.PutIcon(m_scrc.left +2, m_scrc.bottom - TIP_CHAR_HEIGHT , 
			ICON_CHAR_E);
	}
	if(m_cdData.cdmemNum!=0 )
		dc.PutIcon(m_scrc.left + 2, m_scrc.top-1 ,ICON_CHAR_M); 
}

BOOL CCalcWnd::OnDestroy()
{
	::SetCurrentApp();
	return CAppWnd::OnDestroy();
}

//void CCalcWnd::OnKeyBoard1Click(UINT32 nMessage , UINT32 wParam , UINT32 lParam)  // 软键盘1的点击事件处理函数 
void CCalcWnd::DoClear(UINT32 lParam)  // 清除按键操作
{
	 CDC  dc;
     switch(lParam)
	 {
	 case CAL_CE:
		 DoClearOp();
		 break;
     case CAL_C:
		 if(m_cdData.cdbHasError)
		 {
			 dc.EraseRect(m_scrc.left +2, m_scrc.bottom - TIP_CHAR_HEIGHT-1 , m_scrc.left + 11 , 
				 m_scrc.bottom -1   );
			 m_cdData.cdCanOp = m_cdData.cdbCanchg =TRUE;
			 //m_cdData.cdbCon= TRUE;
			 ResetSctext();
			 m_cdData.cdbHasError =FALSE;
			 break;          
		 }
		 ResetElem();
		 break;
	 }
	 m_cdData.cdprevKey =lParam;
}

void CCalcWnd::OnKeyBoard2Click(UINT32 nMessage , UINT32 wParam , UINT32 lParam)// 软键盘2的点击事件处理函数 
{
	if(lParam>=0 &&lParam<=11)
	{
		DoNumHan(lParam);
		m_cdData.cdprevKey=lParam;
	}
	else if(lParam>=20&&lParam<=23)
	{
	//	cData->cdbReset=FALSE;
		if(lParam==CAL_DIV)
			DoDbleOp('/');
		else if(lParam==CAL_MUL)
			DoDbleOp('*');
		else if(lParam==CAL_SUB)
			DoDbleOp('-');
		else 
			DoDbleOp('+');
		m_cdData.cdprevKey=lParam;
	}
	else if(lParam==CAL_EQU)
	{
		DoEqual();
		m_cdData.cdprevKey=lParam;
	}
	else if(lParam==CAL_SQR)
	{
		DoSqrtHan();
		m_cdData.cdprevKey=lParam;
	}
	else if(lParam==CAL_SIGN)
	{
		DoPerSymbol();
		m_cdData.cdprevKey = lParam;
	}
	else if(lParam==CAL_REC)
	{
		DoRecHan();
		m_cdData.cdprevKey = lParam;
	}
	else if(lParam==CAL_BACKSPACE)
	{
		DoBackSpc();
	    m_cdData.cdprevKey = lParam;
	}
	else if(lParam>=13 &&lParam<=16)
	{
		DoMemOp(lParam);
		m_cdData.cdprevKey = lParam;
	}
	else if(lParam >= 30 && lParam <= 31)
	{
		DoClear(lParam);
		m_cdData.cdprevKey = lParam;
	}

}

/*
   函数说明：回删字符操作
   参数：cData是存放计算数据的结构的指针 
*/
void CCalcWnd::DoBackSpc() 
{
     if(!m_cdData.cdCanBack)
		 return;
	 char *pStr=GetScText();
	 int len= strlen(pStr);
	 if(len==1|| (len ==2 &&pStr[0]=='-'))   
	 {
		 EmptyEdit();
		 m_cdData.cdnLen = 0;
		 return;
	 }
	 int pos= FindChar(pStr, '.');
	 if(pos==len-1)
	 {
	    pStr[len-2] ='.';
		pStr[len-1] = 0;
		m_cdData.cdscText[len-2]=0;
	 }
	 else 
	 {
        pStr[len-1]=0;
		m_cdData.cdscText[len-1]=0;
	 }
     m_cdData.cdnLen--;
	 HwPutText(pStr);
}

/*
   函数说明：内存数的操作
   参数：cData是存放计算数据的结构的指针 ，opType 是操作类型，13表示调出内存数或将它清成0。
   14表示从内存数中减去屏幕数 ， 15表示加上屏幕数
*/

void CCalcWnd::DoMemOp(short opType)
{
	CDC dc;
	double  temp;
	if(!m_cdData.cdCanOp)
		return;
	switch(opType)
	{
	case 13: // MC
	/*	if(m_cdData.cdprevKey!=13)
		{
			m_cdData.cdscNum=m_cdData.cdmemNum;
			char pStr[25];
			ConvNumtoStr(m_cdData.cdmemNum , pStr); 
			HwPutText(pStr);
           // mcFree(pStr);
		}
		else */
		{
			m_cdData.cdmemNum=0.0;
			//m_cdData.cdscNum
//	        EmptyEdit();
			EmptyMem();
		}
		break;
	case 14: //MR
		m_cdData.cdscNum=m_cdData.cdmemNum;
		char pStr[25];
		ConvNumtoStr(m_cdData.cdmemNum , pStr); 
		HwPutText(pStr);
		break;
	
	case 15: //M+
		temp = m_cdData.cdmemNum + m_cdData.cdscNum;
        if(fabs(temp)> MAX_NUM)
		{
			HanOverFlow(temp, MEM_ERR);
			return;
		}
		m_cdData.cdmemNum = temp;
		if(m_cdData.cdmemNum!=0 )
		    dc.PutIcon(m_scrc.left + 2, m_scrc.top-1 ,ICON_CHAR_M); //modified just for immediate use
		else
		{
		    EmptyMem();
		}
	
	//	m_cdData.cdedit2.SetWindowText("M"); //此处单独做成一个函数
		break;
	case 16: //M-
		temp = m_cdData.cdmemNum - m_cdData.cdscNum;
        if(fabs(temp)> MAX_NUM)
		{
			HanOverFlow(temp, MEM_ERR);
			return;
		}
		m_cdData.cdmemNum = temp;
		if(m_cdData.cdmemNum!=0 )
		    dc.PutIcon(m_scrc.left + 2, m_scrc.top  ,ICON_CHAR_M);
		else
		{
			EmptyMem();
		}
		
		break;
	}
   ResetSctext();
}

/*
   函数说明：清空内存数操作
   参数：cData是存放计算数据的结构的指针 
*/
void CCalcWnd::EmptyMem()
//此处需注意
{
    //HwPutText("0.");
	CDC  dc;
	dc.EraseRect(m_scrc.left +2, m_scrc.top -1 , m_scrc.left + 11 , m_scrc.top + TIP_CHAR_HEIGHT );
//	ResetSctext(cData);
}

/*
   函数说明：“+、-、*、/”的操作
   参数：cData是存放计算数据的结构的指针，opType是操作符
*/
void CCalcWnd::DoDbleOp(char opType) 
{
	if(!m_cdData.cdCanOp)
		return;
	m_cdData.cdCanBack =FALSE;
	
    if(m_cdData.cdOperator!=-1&&m_cdData.cdprevOp!=-1&&(m_cdData.cdprevKey!=20 
		&&m_cdData.cdprevKey!=21&&m_cdData.cdprevKey!=22&& m_cdData.cdprevKey!=23))
		DoEqual();

//	m_cdData.cdscNum=GetScreenNum();
	m_cdData.cdOpNum1=m_cdData.cdscNum;
	m_cdData.cdprevOp=m_cdData.cdOperator= opType;
	m_cdData.cdIsNum2Null= TRUE;
	m_cdData.cdbCanchg=FALSE;
	ResetSctext();
  
}

/*
   函数说明：数据溢出的处理
   参数：cData是存放计算数据的结构的指针 ， num是溢出后的数据
*/
void CCalcWnd::HanOverFlow(double num , char cType)
{
	char pStr[25];
	if(cType == COM_ERR)
	{
		do{ num/=(MAX_NUM+1) ;}while (num > MAX_NUM); // hqf 2-27
		m_cdData.cdOpNum1= m_cdData.cdscNum = num;
		ConvNumtoStr(m_cdData.cdscNum , pStr); 
		HwPutText(pStr);
	}
	ShowErrorMsg();
}

/*
   函数说明：运算的操作
   参数：cData是存放计算数据的结构的指针 
*/
void CCalcWnd::DoSqrtHan()
{
	//double num = GetScreenNum(CALDATA * cData);
	if(!m_cdData.cdCanOp)
		return;
	if(m_cdData.cdscNum<0.0)
	{
		ShowErrorMsg();
		return;
	}
	m_cdData.cdscNum =m_cdData.cdOpNum2 = sqrt(m_cdData.cdscNum);
    char pStr[25];
	ConvNumtoStr(m_cdData.cdscNum , pStr );
	HwPutText(pStr);
	ResetSctext();
	m_cdData.cdbCanchg=TRUE;
	m_cdData.cdCanBack = FALSE;
}

/*
   函数说明：复位计算器窗口结构数据的操作
   参数：wnd是窗口的指针
*/
void CCalcWnd::ResetElem()
{
   m_cdData.cdOpNum1=m_cdData.cdOpNum2=m_cdData.cdscNum=0.0;
   m_cdData.cdprevOp= m_cdData.cdOperator = -1;
   ResetSctext();
   HwPutText("0");
   m_cdData.cdnLen =0;
   m_cdData.cdIsNum2Null= TRUE;
 //  m_cdData.cdbReset=TRUE;
   m_cdData.cdCanOp=TRUE;
   m_cdData.cdCanBack=FALSE;
   m_cdData.cdbCanchg = FALSE;
   m_cdData.cdprevKey = -1;
  // m_cdData.cdmemNum=0.0;
   CDC  dc;
   dc.Reset();
   dc.EraseRect(m_scrc.left +2, m_scrc.bottom - TIP_CHAR_HEIGHT , m_scrc.left + 11 ,
	   m_scrc.bottom  );//???
}

/*
   函数说明：设定屏幕文本为空的操作
   参数：cData是存放计算数据的结构的指针 
*/
void CCalcWnd::ResetSctext()
{
   m_cdData.cdscText[0]='0';
   m_cdData.cdscText[1]=0;
   m_cdData.cdnLen=0;

}

/*
   函数说明：得到屏幕数字符串的操作
   参数：cData是存放计算数据的结构的指针 
*/
char * CCalcWnd::GetScText()
//此函数待检查!!!
{
    return m_cdData.cdscText;
}

/*void CCalcWnd::OnTest3Click(WINDOW w ,mwEvent *ev , int nCommand)
{
	CloseWindow(w);
}*/

/*
   函数说明：向字符串中添加一个字符
   参数：pStr 是待添加的字符串，ch是待添加的字符
*/
void CCalcWnd::AppendChar(char *pStr, char ch)//mcMalloc
{
    int len = strlen(pStr);
	pStr[len]=ch;
	pStr[len+1]=0;
}

/*
   函数说明：按下数字键的操作
   参数：cData是存放计算器数据的结构的指针  ， chnum是按键的索引值
*/
void CCalcWnd::DoNumHan(short  chnum)
{
	int len ,pos;
	if(!m_cdData.cdCanOp) 
		return;
	m_cdData.cdCanBack=TRUE;
	if(strcmp(m_cdData.cdscText ,"0")==0)
	{
		if(chnum<=9&&chnum>=0)
		{
			m_cdData.cdscNum = chnum;
			m_cdData.cdscText[0]=chnum +0x30;
			m_cdData.cdscText[1]=0;
			m_cdData.cdscNum=m_cdData.cdscText[0] -0x30 ;
			if(chnum!=0)
			   m_cdData.cdbCanchg=TRUE;
			HwPutText(m_cdData.cdscText);
			m_cdData.cdnLen++;
		}
		else if(chnum==11)
		{
			m_cdData.cdscText[1]='.';
			m_cdData.cdscText[2]=0;
			HwPutText(m_cdData.cdscText);
			m_cdData.cdbCanchg=TRUE;
		}
		else 
		{
			DoSymbolOp();
		}
	}
	else 
	{
		len = strlen(m_cdData.cdscText);
		if(chnum<=9&&chnum>=0)
		{
			
			if(m_cdData.cdnLen>=8) return;
			
			m_cdData.cdscText[len] = chnum - 0+0x30;
			m_cdData.cdscText[len +1]=0;
			m_cdData.cdscNum = ConvStrtonum(m_cdData.cdscText);
			m_cdData.cdbCanchg=TRUE;  
			HwPutText(m_cdData.cdscText);
			m_cdData.cdnLen++;
		}
		else if(chnum==11)
		{
			if(m_cdData.cdnLen>=8) return;
			if(FindChar(m_cdData.cdscText , '.')>=0)
			   return;
            m_cdData.cdscText[len]='.';
			m_cdData.cdscText[len +1]=0;
			HwPutText(m_cdData.cdscText);
			m_cdData.cdbCanchg=TRUE;
		}
		else 
		{
			DoSymbolOp();
		}
	}

}

/*
   函数说明：按下数字键的操作
   参数：cData同上 ， chnum是按键的索引值
*/
void CCalcWnd::ShowErrorMsg()
{
    //HwPutText(pMsg);
    m_cdData.cdCanOp=FALSE;
	m_cdData.cdCanBack=FALSE;
	m_cdData.cdbHasError = TRUE ;
	CDC dc;
	dc.PutIcon(m_scrc.left +2, m_scrc.bottom - TIP_CHAR_HEIGHT , 
		ICON_CHAR_E);
}

/*
   函数说明：“%”的操作
   参数：cData是存放计算器数据的结构的指针 
*/
void CCalcWnd::DoPerSymbol()
{
	//m_scNum = GetScreenNum();
	if(!m_cdData.cdCanOp)
		return;
	m_cdData.cdOpNum2=m_cdData.cdOpNum1 * m_cdData.cdscNum/100.0;
	m_cdData.cdscNum= m_cdData.cdOpNum2;
    if(fabs(m_cdData.cdOpNum2)>MAX_NUM)
	    HanOverFlow(m_cdData.cdOpNum2);
	else
	{
		char pStr[25];
		ConvNumtoStr(m_cdData.cdOpNum2 , pStr);
		HwPutText(pStr);
		ResetSctext();
	}
	m_cdData.cdbCanchg=TRUE;
	m_cdData.cdCanBack = FALSE;
}

/*
   函数说明：倒数的操作
   参数：同上
*/
void CCalcWnd::DoRecHan()
{
  //  double num = GetScreenNum();
	if(!m_cdData.cdCanOp)
		return;
	if(m_cdData.cdscNum==0.0)
	{
		ShowErrorMsg();
		return;
	}
    m_cdData.cdscNum =m_cdData.cdOpNum2 = 1/m_cdData.cdscNum;
	if(fabs(m_cdData.cdOpNum2)>MAX_NUM)
	    HanOverFlow(m_cdData.cdOpNum2);
	else
	{
		char pStr[25];
		ConvNumtoStr(m_cdData.cdscNum, pStr);
		HwPutText(pStr);
		ResetSctext();
		
	}
	m_cdData.cdbCanchg=TRUE;
	m_cdData.cdCanBack = FALSE;

}

/*
   函数说明：执行双目运算符的操作
   参数：同上
*/
void CCalcWnd::DoEqual()
{
    if(!m_cdData.cdCanOp)
	   return;
	if(m_cdData.cdOperator==-1)
	{
		m_cdData.cdCanBack=FALSE;
	    return;
	}
	
	m_cdData.cdCanBack = FALSE;
	m_cdData.cdbCanchg= TRUE;
	if(m_cdData.cdIsNum2Null==TRUE)
	{
  		m_cdData.cdOpNum2=m_cdData.cdscNum;//=GetScreenNum(CALDATA * cData);
       	m_cdData.cdIsNum2Null=FALSE;
	}
	switch(m_cdData.cdOperator)
	{
	case '+':
	   m_cdData.cdOpNum1+=m_cdData.cdOpNum2;
	  
	   break;
	case '-':
       m_cdData.cdOpNum1-=m_cdData.cdOpNum2;
	  
	   break;
	case '*':
	   m_cdData.cdOpNum1*=m_cdData.cdOpNum2;
	  
	   break;
	case '/':
	   if(m_cdData.cdOpNum2==0.0)
	   {
	      ShowErrorMsg();
	      return;
	   }
	   m_cdData.cdOpNum1/=m_cdData.cdOpNum2;
	   break;
	}
	m_cdData.cdprevOp=-1;
	m_cdData.cdscNum=m_cdData.cdOpNum1;
	if(fabs(m_cdData.cdOpNum1)>MAX_NUM)
	    HanOverFlow(m_cdData.cdOpNum1);
	else 
	{
	   char pStr[25];
	   ConvNumtoStr(m_cdData.cdOpNum1 , pStr);
	   if(strcmp(pStr , "0")==0)
		   m_cdData.cdscNum = 0.0; // hqf 2-27

	   HwPutText(pStr);
	   ResetSctext();
	}
}

/*
    函数功能:改变屏幕数符号
	参数：同上
*/
void CCalcWnd::DoSymbolOp()
{
	int len ;
	if(!m_cdData.cdbCanchg)
	{
		DoClearOp();
		return;
	}

	//问题！！！
	if(m_cdData.cdscNum== 0.0)
		return;
	
	if(m_cdData.cdscText[0]=='0')
	{
	   char pStr[25];
	   ConvNumtoStr(m_cdData.cdscNum , pStr);
	   len = strlen(pStr);
	   if(pStr[0]!='-') //modefied 
	   {
		  for(int i =len; i>0;i--)
			  pStr[i]=pStr[i-1];
		  pStr[0]='-';
		  pStr[len+1]=0;
		  	  
		}
		else 
		{
			for(int i=0 ;i<len;i++)
			   pStr[i]=pStr[i+1];
		}
		HwPutText(pStr);
	}
    else
	{
		len =strlen(m_cdData.cdscText);
		if(m_cdData.cdscText[0]!='-') //modefied 
		{
		  for(int i =len; i>0;i--)
			  m_cdData.cdscText[i]=m_cdData.cdscText[i-1];
		  m_cdData.cdscText[0]='-';
		  m_cdData.cdscText[len+1]=0;
		 
		}
		else 
		{
			for(int i=0 ;i<len;i++)
			   m_cdData.cdscText[i]=m_cdData.cdscText[i+1];
			//m_cdData.cdscNum = ConvStrtonum(m_cdData.cdscText);
			
		}
		HwPutText(m_cdData.cdscText);
	}
	m_cdData.cdscNum = -m_cdData.cdscNum;

}

void CCalcWnd::DoClearOp()
{
	if(!m_cdData.cdCanOp)
		return;
	EmptyEdit();	

}

void CCalcWnd::EmptyEdit()
{
	ResetSctext();
	HwPutText(m_cdData.cdscText);
	//strcpy(m_cdData.cdscText,"0");
	m_cdData.cdscNum= 0.0;
    
}
/*
    函数说明：点击单位换算按钮的消息处理函数
	参数：无
*/
void CCalcWnd::OnConvBtnClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	ConvAppRun();
}

/*
    函数说明：计算器文本输出函数 ，由于计算器精度只有八位，而
	计算结果可能超出这个范围，所以在显示时应该截取掉多余的数字。
	参数：str是待显示的文本
*/
void CCalcWnd::HwPutText(char *str)
{
  int pos , len;
  char str2[25];
   strcpy(str2 , str);
   pos =FindChar(str2 , '.');
   len = strlen(str2);
  if(pos==-1)
  {
	  if(str2[0]=='-')
	  {
		  if(len>9)
	        str2[9]=0;
		  else if(len ==2 && str2[1]== '0')
		  {
			  str2[0]= '0' , str2[1] = 0;
		  }

	  }
	  else 
	  {
		  if(len>8)
	        str2[8]=0;
	  }
     AppendChar(str2 ,'.');
  }
  else 
  {
	 if(str2[0]=='-')
	  {
		  if(len>10)
	        str2[10]=0;
	  }
	  else 
	  {
		  if(len>9)
	        str2[9]=0;
	  }
  }  
   PutCalcString(str2 , m_scrc.right-1 , m_scrc.bottom - CALC_NUM_HEIGHT);
}

/*
*  函数
*               计算器数字显示子函数
*  说明
*               由于 8 x 16 的数字太小，所以特制大号数字，专供计算器显示用。
*  参数
*               buf             显示字符串
*                               有效字符为 '0', '1', '2', '3', '4', '5', '6',
*               '7', '8', '9', '.', '-', 其他字符显示为空。
*                               能显示的最大长度为 10（例如 "-0.1234567")，超过此长度的部分不显示。
*               (x, y)  显示坐标
*                               每个字符宽度为 16，高度为 24，显示区域大小为 160 x 24，
*               采用右对齐方式显示，(x, y) 是显示区域的右上角坐标。
*  备注
*               调用显示之前不用清除区域，显示函数负责清除。
*
*/
//void PutCalcString(INT8 *buf, UINT16 x, UINT16 y);

//void PutCalcString(INT8 *buf, UINT16 x, UINT16 y)
void PutCalcString(char  *buf, UINT16 x, UINT16 y)
{
        INT8    ch;
        INT16   cx, cy;
        INT16   len, i, j;
        CDC     dc;
        
        UINT32  index[] = {ICON_CHAR_0 , ICON_CHAR_1 , ICON_CHAR_2 , ICON_CHAR_3 , 
							ICON_CHAR_4 , ICON_CHAR_5 , ICON_CHAR_6 , ICON_CHAR_7 , 
							ICON_CHAR_8 , ICON_CHAR_9 , ICON_POINT , ICON_NEG , 
							ICON_NONE , ICON_POSITIVE, ICON_CHAR_E, ICON_COLON};
							
			/*{CALC_0, CALC_1, CALC_2, CALC_3, CALC_4, CALC_5, CALC_6, CALC_7, \
                             CALC_8, CALC_9, CALC_POINT, CALC_NEG, CALC_NONE , CALC_POSI , CHAR_E}; */

        // 统计字符串长度
        for (len = 0; len < CALC_SHOW_LEN; len ++){
                if (*(buf + len) == 0)
                        break;
        }
        
        cx = x - CALC_NUM_WIDTH * CALC_SHOW_LEN + 1;
        cy = y;

        for (i = 0; i < CALC_SHOW_LEN; i ++){
                j = i + len - CALC_SHOW_LEN;
                if (j < 0){
                        dc.PutIcon(cx, cy, ICON_NONE);  // 显示开头空字符
                }
                else{
                        ch = *(buf + j);
                        if ((ch >= '0') && (ch <= '9'))
                                j = ch - '0';           // 数字
                        else{
                                if (ch == '.')
                                        j = 10;         // 小数点
                                else if (ch == '-')
                                        j = 11;         // 负号
								else if (ch == '+')
									    j = 13;
								else if (ch == 'e')
									    j = 14;
								else if (ch == ':')
										j = 15;
                                else
										j = 12;         // 非法字符，显示为空
                        }
                        dc.PutIcon(cx, cy, index[j]);   // 显示字符
                }
                cx += CALC_NUM_WIDTH;
        }
}




///////////////////下面是单行编辑器的定义部分////////////////////
CSinEdit::CSinEdit()
{

}

CSinEdit::~CSinEdit()
{

}

/*
*  函数说明  单行编辑器的创建函数
   参数 :rc是窗口的位置, parent 是父窗口的指针 , style 指文本左(右)对齐 , maxnum 指内存块的最大个数
*/

BOOL CSinEdit::Create(CRect rc , CWnd *parent , int ID ,short style , short maxnum , UINT32 wndStyle /*=64*/)
{
	m_edData.Style = style;
	m_edData.rcArea = rc;
	m_edData.nMaxNum = maxnum;
    InitEdit();
	return CCtrl::Create(NULL , rc , parent , wndStyle, ID);
}

INT16 CSinEdit::SetBackColor(INT16 iColor)
{
	INT16 prev = m_iBackColor;
	m_iBackColor = iColor;
	return prev;
}
/*
*  函数说明  单行编辑器的自画函数
   参数 :ctrl 是编辑控件的指针
*/
void  CSinEdit::OnPaint(CDC &dc)
{
//	SINEDIT  *seData = (SINEDIT *)ctrl->data;
    CRect r(m_sRect);
	dc.EraseRect(r);
	dc.FrameRect(r);
	r.DeflateRect(1, 1);
	dc.SetPenColor(LGRAY);
	dc.FrameRect(r); 
	if(m_edData.Style==0)
	{
		m_edData.CurPos.x = 2;
		m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT -1;
	    SetCurPos(m_edData.CurPos);
	}
	else if(m_edData.Style ==1)
	{
		m_edData.CurPos.x = m_edData.rcArea.right-2 ;
		m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT -1;
	    SetCurPos(m_edData.CurPos);
	}
}

/*
*  函数说明  单行编辑器的初始化函数
   参数 :    ctrl是编辑控件的指针
*/
BOOL CSinEdit::InitEdit()
{
    m_edData.pEditText =(char *)SysMalloc(BLOCK_SIZE);
	memset(m_edData.pEditText , 0 , BLOCK_SIZE);
	m_edData.BlockNum = 1;
	m_edData.CurPos.x = m_edData.rcArea.left;
	m_edData.CurPos.y = m_edData.rcArea.top;
	m_edData.nCurLen =0;
	m_iBackColor = WHITE;
//	CreateCursor(ctrl);
}

/*
*  函数说明  判断是否是英文字符 
   参数 :ctrl 是编辑控件的指针
   返回值: 1 ,是; 0 , 不是
*/
BOOL CSinEdit::IsAscChar(short chJge)
{
	BYTE ch = LOWBYTE(chJge) ;
	return ch<=0xa0 ? 1 :0 ;
}

/*
*  函数说明  显示光标
   参数 :ctrl 是编辑控件的指针
   返回值: 无
*/
void   CSinEdit::CreateCursor()
{
	::CloseCursor();
	::OpenCursor();
	::SetCursorAttr(2, CHAR_HEIGHT );
}

/*
*  函数说明  设定光标位置
   参数 :pX 是光标的x 坐标, pY 是y 坐标
   返回值: 无
*/

void  CSinEdit::SetCurPos(short pX  , short pY)
{
	::SetCursorXY(pX , pY);
}
/*
*  函数说明  关闭光标
   参数 :  ctrl 是编辑控件的指针
   返回值: 无
*/

void  CSinEdit::CloseCur()
{
	::CloseCursor();
}

/*
*  函数说明  在光标原来位置处打开光标
   参数 :ctrl 是编辑控件的指针
   返回值: 无
*/
void  CSinEdit::ShowCur()
{
	SetCurPos(m_edData.CurPos);
}

/*
*  函数说明  清空编辑窗口的内容
   参数 :ctrl 是编辑控件的指针
   返回值: 无
*/
void  CSinEdit::EmptyWindow()
{
	CloseCur();
	CRect rc ;
	rc.SetRect(m_edData.rcArea.left+2 , m_edData.rcArea.top +2 , m_edData.rcArea.right -2,m_edData.rcArea.bottom-2);
	CDC dc;
	dc.SetBackColor(m_iBackColor);
	dc.EraseRect(rc);
    if(m_edData.Style == LEFT_ALIGN)
	{
		m_edData.CurPos.x = 2 ;
		m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT-1;
	    SetCurPos(m_edData.CurPos);
	}
	else 
	{
		m_edData.CurPos.x = m_edData.rcArea.right -3 ;
		m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT-1;
		SetCurPos(m_edData.CurPos);
	}
}

/*
*  函数说明: 得到下一个字符开始的字符串的指针
   参数 :    ctrl 是编辑控件的指针
   返回值: 无
*/
char *CSinEdit::GetNextChar(char *pText)
{
	if(pText[0] ==0)
		return pText;
	unsigned char tm =(unsigned char )pText[0];
	return tm>0xa0 ? pText+2 : pText+1 ;
}

void CSinEdit::SetCurPos(CPoint pt)
{
	SetCurPos(pt.x , pt.y);
}

/*
*  函数说明: 得到上一个字符开始的字符串的指针
   参数 :    ctrl 是编辑控件的指针
   返回值: 无
*/
char *CSinEdit::GetPrevChar(char *pText)
{
	short len;
	if(pText[0]==0)
		return pText;
	len = strlen(pText);
	unsigned char tm;
	if(len>=1)
	{
		tm = pText[len-1];
		if(tm<= 0xa0)
		    return pText-1;
	}
	if(len>1)
	{
		tm= pText[len-1];
		if(tm >0xa0)
		    return pText-2;
	}
}

/*
*  函数说明: 设定编辑框的内容
   参数 :    ctrl 是编辑控件的指针
   返回值: 无
*/

void  CSinEdit::SetWindowText(char *pText , BOOL bShowcur)
{
	if(pText==NULL)
	{
		EmptyWindow();
		m_edData.pEditText[0] = 0;
		if(!bShowcur)
		   CloseCur();
		return;
	}

	char *pTemp= (char *)SysMalloc(m_edData.nMaxNum +1);
	if(!pTemp)
			return;
	short  len ;
	len = strlen(pText);
	if(len > m_edData.nMaxNum)
	{
		len = m_edData.nMaxNum;
  	   	memcpy(pTemp , pText , m_edData.nMaxNum);
		pTemp[m_edData.nMaxNum] =0;
	}
    else 
	    strcpy(pTemp , pText);
    strcpy(m_edData.pEditText, pTemp);
	EmptyWindow();
	CloseCur();
	char *str1 , str2[2];
  	short phlen , posX;
	CDC    dc;
    
	phlen =len *ASC_WIDTH; 
	dc.SetBackColor(m_iBackColor);
	if(phlen<= m_edData.rcArea.right - m_edData.rcArea.left -4 )
	{

	   if(m_edData.Style == LEFT_ALIGN)
	   {
          dc.SetAlign(HALIGN_LEFT, VALIGN_TOP);
		  dc.TextOut(m_edData.rcArea.left +2, m_edData.rcArea.bottom - CHAR_HEIGHT-1 , pTemp);
		  m_edData.CurPos.x = m_edData.rcArea.left + phlen+2 ;
		  m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT -1;
          CreateCursor();
		  if(bShowcur)
			 SetCurPos(m_edData.CurPos);
	   }
	   else 
	   {
			dc.SetAlign(HALIGN_LEFT, VALIGN_TOP);
			dc.TextOut(m_edData.rcArea.right -phlen- 2 , m_edData.rcArea.bottom- CHAR_HEIGHT- 1 , pTemp);
			m_edData.CurPos.x = m_edData.rcArea.right- 3;
			m_edData.CurPos.y = m_edData.rcArea.bottom- CHAR_HEIGHT- 1;
			CreateCursor();
			if(bShowcur)
			   SetCurPos(m_edData.CurPos);
	   }
	}	
	else
	{
    	posX=m_edData.rcArea.left;
		pText = pTemp;
		while(posX < m_edData.rcArea.right)// modified??? - m_edData.rcArea.left)
		{
			str1= GetNextChar(pText);
			if(str1-pText>0)
			{
				if(posX +ASC_WIDTH * (str1-pText) > m_edData.rcArea.right)
				  break;
				memcpy(str2 , pText , str1-pText);
				str2[str1-pText]=0;
				dc.SetAlign(HALIGN_LEFT, VALIGN_TOP);
				dc.TextOut(posX +2, m_edData.rcArea.bottom - CHAR_HEIGHT-1 , str2);
				posX +=ASC_WIDTH * (str1-pText);
			}
			else 
				break;
			pText=str1;
		}
		m_edData.CurPos.x = m_edData.rcArea.right -3;
		m_edData.CurPos.y = m_edData.rcArea.bottom - CHAR_HEIGHT-1;
		CreateCursor();
		if(bShowcur)
			SetCurPos(m_edData.CurPos );
	}
	SysFree(pTemp);
}

/*
*  函数说明: 得到编辑控件的文本
   参数 :    ctrl 是编辑控件的指针
   返回值: 无
*/
char *CSinEdit::GetWindowText()
{
	return  m_edData.pEditText;
}

/*
*  函数说明: 向已有文本添加一字符
   参数 :    ctrl 是编辑控件的指针
   返回值: 无
*/
BOOL CSinEdit::AppendChar(short apch)
{
	int  Len= strlen(m_edData.pEditText);
	if(Len >= m_edData.nMaxNum)
	   return FALSE;
	if(apch==0 ) 
		return FALSE;
	if(IsAscChar(apch))
	{
		if((Len+1) *ASC_WIDTH >(m_edData.rcArea.right - m_edData.rcArea.left))
			return 0;
		if(Len+1 >= (m_edData.BlockNum <<6) )
		{
			m_edData.BlockNum++;
			m_edData.pEditText = (char *)SysRealloc(m_edData.pEditText , (m_edData.BlockNum -1)<<6 , m_edData.BlockNum <<6);
			if(m_edData.pEditText==NULL)
				return FALSE;
		}
		m_edData.pEditText[Len]= LOWBYTE(apch);
		m_edData.pEditText[Len+1]=0;
		m_edData.nCurLen++;
	}
	else 
	{
		if((Len+2) *ASC_WIDTH >(m_edData.rcArea.right - m_edData.rcArea.left))
			return 0;
		if(Len+2 >= (m_edData.BlockNum <<6) )
		{
			m_edData.BlockNum++;
			m_edData.pEditText = (char *)SysRealloc(m_edData.pEditText , (m_edData.BlockNum -1)<<6 , m_edData.BlockNum <<6);
			if(m_edData.pEditText==NULL)
				return FALSE;
		}
		m_edData.pEditText[Len]=HIGBYTE(apch);
		m_edData.pEditText[Len+1]=LOWBYTE(apch);
		m_edData.pEditText[Len+2]=0;
		m_edData.nCurLen+=2;
	}
	return TRUE;
}
    
/*
*  函数说明: 回删一个字符
   参数 :    ctrl 是编辑控件的指针
   返回值: 无
*/
void CSinEdit::BackChar()
{
	char *str= m_edData.pEditText;
	short len = strlen(str);
	if(len==0)
	{
		EmptyWindow();
		return;
	}
	if(len < 2)
	{
	    SetWindowText(NULL);
		return;
	}
	
    char *str2= GetPrevChar(str);
	if(str-str2==1)
	{
		str[len-1]=0;
		SetWindowText(str);
	}
	else if(str-str2==2)
	{
		str[len-2]=0;
		SetWindowText(str);
	}
}
	
//清空编辑框内容		
void CSinEdit::DoClearOper()
{
   SetWindowText(NULL);
   
}

// 控件的事件处理函数 , pCtrl 是编辑控件的指针
void CSinEdit::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
    int x = m_edData.CurPos.x;
	char pStr[5];
	switch(nMessage)
	{
		case WM_KEY:
			if(lParam !=CHAR_BACK && lParam!=CHAR_CLEAR)
			{
				AppendChar( lParam);
				SetWindowText( m_edData.pEditText); 
			}
			else if(lParam==CHAR_CLEAR)
				DoClearOper();
				
			else if(lParam==CHAR_BACK)
				BackChar();
			break;
		case WM_DACTIVE:
//			CloseCur();
			break;
/*		case CM_WRITEPAD_SEL:       //TODO:手写板的选字消息？
			BackChar();
			AppendChar(lParam);
			SetWindowText(m_edData.pEditText);
			break;//*/
		case CM_WRITEPAD_WRITE:
            AppendChar(lParam);
			SetWindowText( m_edData.pEditText);
			break;
		case WM_PENDOWN:
			SendNotifyMessage(CN_COMMAND, 0);
			break;
	}
}

/*
*  函数说明: 控件的退出处理函数
   参数 :    ctrl 是编辑控件的指针
   返回值: 0
*/

void CSinEdit::Destroy()
{
	//单行编辑控件内存的释放
	SysFree(m_edData.pEditText);
	CloseCur();
	CCtrl::Destroy();
}

//////////////////////////下面是单位换算的定义部分//////////////////////

#define   ID_LISTTYPE              1003
#define   ID_LISTSRC               1004
#define   ID_LISTDST               1005
#define   ID_CONV_SFK              1006
#define   ID_CONV_CLEAR            1007
#define   ID_CONV_DOT              1008
#define   ID_CONV_OPER             1009
#define   ID_SRCEDIT               1010
#define   ID_DSTEDIT               1011
#define   ID_CONVADV               1103

#ifndef SWAP
#define  SWAP(a, b)  {(a) = (a)^(b), (b)=(b)^(a), (a)=(a)^(b);}
#endif

BEGIN_MESSAGE_MAP(CConvWnd,CAppWnd)
   //{{
	 ON_MESSAGE(ID_LISTTYPE,  CN_COMMAND , OnListSel)
     ON_MESSAGE(ID_LISTSRC,  CN_COMMAND , OnListSel)
	 ON_MESSAGE(ID_LISTDST,  CN_COMMAND , OnListSel)
	 ON_MESSAGE(ID_CONV_SFK,  WM_KEY, OnSoftKeyClick)
     ON_MESSAGE(ID_SRCEDIT,   CN_COMMAND , OnSrcEditClick)
	 ON_MESSAGE(ID_DSTEDIT,   CN_COMMAND , OnDstEditClick)
	 ON_MESSAGE(ID_CONV_CLEAR,   CN_COMMAND , OnBtnClick)
	 ON_MESSAGE(ID_CONV_OPER,   CN_COMMAND , OnBtnClick)

  //}}
END_MESSAGE_MAP

CConvWnd::CConvWnd()
{
    m_bComStatus[0] = m_bComStatus[1] = m_bComStatus[2] = CS_CLOSE;
}

CConvWnd::~CConvWnd()
{

}

#define   DROP_HEIGHT                      74	
extern BOOL CalcuRectValue(CRect *pRectArray , int iRows , int iCols , CRect rectTotal , 
					short xInterval , short yInterval);

BOOL CConvWnd::OnCreate()
{
	CAppWnd::OnCreate();
	CRect rc;

	int i;
    m_ucData.UnitType = 0;
	m_ucData.FocusEditID =ID_SRCEDIT ;
	m_ucData.FocusListID =-1 ;
	m_ucData.nSrcSel = 0;
	m_ucData.nDstSel = 1;
//	m_pWnd->scrollDistance = (int)ucData;
	rc.SetRect(m_sRect.left + 83 , m_sRect.top + WND_TITLE_HEIGHT+ 3 , m_sRect.left + 156 , m_sRect.top + WND_TITLE_HEIGHT + 22);
	comType.Create(NULL, rc, DROP_HEIGHT , this , ID_LISTTYPE);
	for(i=0;i< MAX_TYPES ;i++)
	   comType.AppendItem(LOADSTRING(g_pUnitType[i]) , 0);
	comType.SetCurSel(0);

	rc.top = rc.bottom + 3, rc.bottom = rc.top + 19;
	comUnitSrc.Create(NULL , rc , DROP_HEIGHT, this,ID_LISTSRC);
	i = 0;
	while(g_pUnitName[m_ucData.UnitType][i]!=-1)
	{
	   comUnitSrc.AppendItem(LOADSTRING(g_pUnitName[m_ucData.UnitType][i]), 0);
	   i++;
	}
	comUnitSrc.SetCurSel(0);

	rc.top = rc.bottom + 3, rc.bottom = rc.top + 19;
    comUnitDst.Create(NULL, rc, DROP_HEIGHT, this, ID_LISTDST);
	i = 0;
	while(g_pUnitName[m_ucData.UnitType][i]!=-1)
	{
	   comUnitDst.AppendItem(LOADSTRING(g_pUnitName[m_ucData.UnitType][i]), 0);
	   i++;
	}
	comUnitDst.SetCurSel(1);

	rc.right = rc.left - 3, rc.left = m_sRect.left +3;
	seDst.Create(rc , this, ID_DSTEDIT,RIGHT_ALIGN);
    seDst.SetBackColor(WHITE);

	rc.bottom = rc.top -3 , rc.top = rc.bottom - 19;
	seSrc.Create(rc , this , ID_SRCEDIT,RIGHT_ALIGN);
	seSrc.SetBackColor(WHITE);

	unsigned short pKeyValue[]={ 0x30 , 0x31 ,0x32 , 0x33 ,
		0x34 , 0x35 ,0x36 , 0x37 ,0x38 , 0x39 ,(unsigned short)'.'};
	CRect pRectArray[11];

    //modify by wangfei
	//rc.SetRect(m_sRect.left +3, m_sRect.top +WND_TITLE_HEIGHT + 83 , m_sRect.right -4, m_sRect.top +WND_TITLE_HEIGHT +95 );
	rc.SetRect(m_sRect.left+2, m_sRect.top +WND_TITLE_HEIGHT + 80 , m_sRect.right-3, m_sRect.top +WND_TITLE_HEIGHT+99);

	CalcuRectValue(pRectArray, 1, 11, rc, 1, 0); 
	skNum.Create(ICON_UNITCONV_NUMKEY, 11 , pRectArray,pKeyValue, this, ID_CONV_SFK);
//	char *pTemp[] ={"0" ,"1" , "2" ,"3" ,"4","5", "6","7" , "8", "9" };
 //   btnDot.Create("." , CRect(1,212, 19, 235), m_pWnd, ID_CONV_DOT);
	//modify by wangfei
	rc.left = m_sRect.HCenter()-55;
	rc.top = rc.bottom + 16,rc.bottom = rc.top + 18;
	rc.right = rc.left + 48;
	btnClear.Create("清空" , rc, this,ID_CONV_CLEAR);
	rc.left = m_sRect.HCenter() + 7, rc.right = rc.left + 48;
	btnConv.Create("换算" , rc , this,ID_CONV_OPER);
	rc.SetRect(m_sRect.left + TITLE_WIDTH , m_sRect.top, m_sRect.right - WND_TITLE_HEIGHT, m_sRect.top + CAL_TITLE_HEIGHT - 1);
	btnCalc.Create("计算器" , rc , this , IDCANCEL);
	return TRUE;
}

#undef   DROP_HEIGHT
/*
    函数说明：单位转换窗口复位数据的函数
	参数：窗口结构的指针
*/
void CConvWnd::ResetEditData()
{
//	CDC  dc;
//	dc.TextOut(20, 79 ,LOADSTRING(STR_CONVUNIT));
	seSrc.SetWindowText("0", FALSE);
	seDst.SetWindowText("0", FALSE);
	m_ucData.FocusEditID= -1;
	m_ucData.FocusListID= -1;
	m_ucData.dSrcNum= m_ucData.dDstNum =0.0;
}

/*
    函数说明：单位转换窗口自画的函数
	参数：窗口结构的指针
*/
void CConvWnd::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	ResetEditData();
	CloseCursor();
}
/*
    函数说明：单位转换窗口单位类型列表框、单位名称列表框的选择函数
	参数：w是窗口结构的指针, ev是存放事件结构的指针 , nCommand是附加参数
	返回值:无
*/
void CConvWnd::OnListSel(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	 short  nIndex;
	 int i;
	 switch(wParam)
	 {
	 case ID_LISTTYPE:
		 {
		 if(lParam != m_ucData.UnitType)
		 {
			m_ucData.UnitType= lParam;
			comUnitSrc.DeleteAllItems();
			i = 0;
			if(lParam !=1)
			{
				while(g_pUnitName[m_ucData.UnitType][i]!=-1)
				{
				   comUnitSrc.AppendItem(LOADSTRING(g_pUnitName[m_ucData.UnitType][i]), 0);
				   i++;
				}
			}
			else
			{
				while(g_pUnitName[m_ucData.UnitType][i]!=-1)
				{
					if(i == 1)
						comUnitSrc.AppendItem(NULL , ICON_SQUAREFEET);
					else if(i == 2)
						comUnitSrc.AppendItem(NULL , ICON_SQUAREKM);
					else if(i == 9)
						comUnitSrc.AppendItem(NULL , ICON_SQUAREMILE);
					else
						comUnitSrc.AppendItem(LOADSTRING(g_pUnitName[m_ucData.UnitType][i]), 0);
				   i++;
				}
			}
            comUnitSrc.SetCurSel(0);
			comUnitSrc.OnPaint(CDC());

			comUnitDst.DeleteAllItems();
			i = 0;
			if(lParam != 1)
			{
				while(g_pUnitName[m_ucData.UnitType][i]!=-1)
				{
				   comUnitDst.AppendItem(LOADSTRING(g_pUnitName[m_ucData.UnitType][i]), 0);
				   i++;
				}
			}
			else
			{
				while(g_pUnitName[m_ucData.UnitType][i]!=-1)
				{
					if(i == 1)
						comUnitDst.AppendItem(NULL , ICON_SQUAREFEET);
					else if(i == 2)
						comUnitDst.AppendItem(NULL , ICON_SQUAREKM);
					else if(i == 9)
						comUnitDst.AppendItem(NULL , ICON_SQUAREMILE);
					else
						comUnitDst.AppendItem(LOADSTRING(g_pUnitName[m_ucData.UnitType][i]), 0);
				   i++;
				}
			}
			comUnitDst.SetCurSel(1);
			comUnitDst.OnPaint(CDC());
			
			if(m_ucData.FocusEditID==-1)
			{
                  ResetEditData();
				  CloseCursor();
			}
			if(m_ucData.UnitType ==4)
				m_ucData.FocusEditID = ID_SRCEDIT;
			m_ucData.nSrcSel = 0;
			m_ucData.nDstSel = 1;
		 }
		}
		break;
	 case ID_LISTSRC:

/*		 ctrl = FindControl(w , ID_COMUNITSRC);
		 CCtrl::SetCtrlText(ctrl ,LOADSTRING(g_pUnitName[m_ucData.UnitType][ev->eventUser[1]]),TRUE);*/
		 m_ucData.nSrcSel = lParam;
         if(m_ucData.FocusEditID==-1)
		 {
			 m_ucData.FocusEditID=ID_DSTEDIT;
			 DoConv();
		 }
		 break;
	 case ID_LISTDST:
/*		 ctrl = FindControl(w , ID_COMUNITDST);
		 CCtrl::SetCtrlText(ctrl ,LOADSTRING(g_pUnitName[m_ucData.UnitType][ev->eventUser[1]]),TRUE);*/
    	 m_ucData.nDstSel = lParam;
		 if(m_ucData.FocusEditID==-1)
		 {
			 m_ucData.FocusEditID=ID_SRCEDIT;
			 DoConv();
		 }
		 break;
	 }
}

C3DSinEdit * CConvWnd::GetFocusEdit()
{
	if(m_ucData.FocusEditID == -1)
		return NULL;
	if(m_ucData.FocusEditID == ID_SRCEDIT)
		return &seSrc;
	return &seDst;
}
/*
    函数说明：源单位名称组合框的点击处理
	参数：w 窗口指针 ，ev是窗口的事件， nCommand附加参数
*/
void CConvWnd::OnSoftKeyClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
//	UNITCONV  *ucData = (UNITCONV *)w->scrollDistance;
	if(m_ucData.FocusEditID ==-1)
	   return ;
//	CTRL *ctrl= FindControl(w , m_ucData.FocusEditID);  //焦点编辑控件
	C3DSinEdit *pFocusEdit = GetFocusEdit();
	char *pText = pFocusEdit->GetWindowText();
	int len = strlen(pText);
	char str[4];
	str[0]= lParam;
	str[1]=0;
	if(len==1 &&pText[0]=='0' && lParam != '.')
	{
	   pFocusEdit->SetWindowText(str);
	   return;
	}
	if(lParam == '.')
	{
		int pos = FindChar(pText , '.');
		if(pos==-1)
		{
			if((*pText)!=0)
				pFocusEdit->SendMessage(nMessage, wParam , lParam);
			else 
			   pFocusEdit->SetWindowText("0.");
		}
		return;
	}
/*	ev->eventType = CM_SOFTKEY;
	ctrl->ev =*ev;*/
	pFocusEdit->SendMessage(nMessage, wParam , lParam);
}

/*
    函数说明：“换算”、“转换”按钮的点击处理函数
	参数：w 窗口指针 ，ev是窗口的事件， nCommand附加参数
*/
void CConvWnd::OnBtnClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
//	mwEvent event = w->ev;
//	UNITCONV  *ucData = (UNITCONV *)w->scrollDistance;
//	CTRL  *ctrl;
	char *pText;
	switch(wParam)
	{
	case ID_CONV_CLEAR:
		{
		 if(m_ucData.FocusEditID==-1)
		 {
			 seSrc.SetWindowText("0" ,FALSE);
             seDst.SetWindowText("0" ,FALSE);
			 m_ucData.dSrcNum = m_ucData.dDstNum=0.0;
			 CloseCursor();
		 }
		 else
		 {
			 GetFocusEdit()->SetWindowText(NULL);
			 m_ucData.FocusEditID==ID_SRCEDIT ? m_ucData.dSrcNum==0.0 : m_ucData.dDstNum==0.0;
		 }
		 break;
		}
	case ID_CONV_OPER:
		{
		if(m_ucData.FocusEditID==-1)
	         return;
//		ctrl = FindControl(w , m_ucData.FocusEditID);
	    pText = GetFocusEdit()->GetWindowText();
	    double res = ConvStrtonum(pText);
		(m_ucData.FocusEditID==ID_SRCEDIT)? m_ucData.dSrcNum=res : m_ucData.dDstNum=res;
		DoConv();
		break;
		}
	}
}

/*
    函数说明：源单位数值编辑框的点击处理
	参数：w 窗口指针 ，ev是窗口的事件， nCommand附加参数
*/
void CConvWnd::OnSrcEditClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
//	UNITCONV  *ucData = (UNITCONV *)w->scrollDistance;
//	CTRL * ctrl;
	if(m_ucData.FocusEditID !=ID_SRCEDIT)
	{
		CloseCursor();
//		ctrl = FindControl(w , ID_DSTEDIT);
		seDst.SetWindowText("0" ,FALSE);
//		ctrl = FindControl(w , ID_SRCEDIT);
		seSrc.SetWindowText(NULL, FALSE);
//      SINEDIT  *seData= (SINEDIT *)ctrl->data;
		m_ucData.FocusEditID = ID_SRCEDIT;
		m_ucData.FocusListID = ID_LISTSRC;
	/*	char *pText;
		ctrl = FindControl(w , ID_DSTEDIT);
		pText = CSinEdit::GetWindowText(ctrl);
		double res = ConvStrtonum(pText);*/
		m_ucData.dSrcNum= m_ucData.dDstNum= 0.0;
		seSrc.ShowCur();
	}
	return;
}


/*
    函数说明：目的单位数值编辑框的点击处理
	参数：w 窗口指针 ，ev是窗口的事件， nCommand附加参数
*/
void CConvWnd::OnDstEditClick(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
//	UNITCONV  *ucData = (UNITCONV *)w->scrollDistance;
//	CTRL * ctrl;
	if(m_ucData.FocusEditID !=ID_DSTEDIT)
	{
		CloseCursor();
//		ctrl = FindControl(w , ID_SRCEDIT);
		seSrc.SetWindowText("0" ,FALSE);
//		ctrl = FindControl(w , ID_DSTEDIT);
		seDst.SetWindowText(NULL );
//        SINEDIT  *seData= (SINEDIT *)ctrl->data;
                     
		m_ucData.FocusEditID = ID_DSTEDIT;
		m_ucData.FocusListID = ID_LISTSRC;
	/*	char *pText;
		ctrl = FindControl(w , ID_DSTEDIT);
		pText = CSinEdit::GetWindowText(ctrl);
		double res = ConvStrtonum(pText);*/
		m_ucData.dSrcNum= m_ucData.dDstNum= 0.0;
		seDst.ShowCur();
	}
	return;
}
/*
    函数说明：温度单位的换算处理函数
	参数：w 窗口指针
*/
void CConvWnd::DoTempConv()
{
//    UNITCONV  *ucData = (UNITCONV *)w->scrollDistance;
    int i = m_ucData.nSrcSel;
    int j = m_ucData.nDstSel; 
	double res;
	char  pRes[25];
//	CTRL  *ctrl;
    if(i==j)
	{
		if(m_ucData.FocusEditID==ID_SRCEDIT)
		{
			m_ucData.dDstNum= res =m_ucData.dSrcNum;
			ConvNumtoStr(res , pRes,TRUE);
//	 		ctrl = FindControl(w , ID_DSTEDIT);
			seDst.SetWindowText(pRes, FALSE);
		}
		else
		{
			m_ucData.dSrcNum= res =m_ucData.dDstNum;
			ConvNumtoStr(res , pRes ,TRUE);
//	 		ctrl = FindControl(w , ID_SRCEDIT);
			seSrc.SetWindowText(pRes, FALSE);
		}
	}
	else 
	{
		if(m_ucData.FocusEditID==ID_SRCEDIT)
		{
			if(i==0 &&j==1)
				res = m_ucData.dSrcNum * 1.8 +32.0;
			else if(i==0 && j==2)
				res = m_ucData.dSrcNum + 273.15;
			else if(i==1&& j==2)
				res = m_ucData.dSrcNum *1.8 -459.67;
			else if(i==1 &&j==0)
				res = (m_ucData.dSrcNum -32.0)/1.8;
			else if(i==2 &&j==0)
				res = m_ucData.dSrcNum- 273.15;
			else if(i==2&& j==1)
				res = (m_ucData.dSrcNum +459.67)/1.8;
			m_ucData.dDstNum = res;
		    ConvNumtoStr(res , pRes, TRUE);
//	 		ctrl = FindControl(w , ID_DSTEDIT);
			seDst.SetWindowText(pRes, FALSE);
			if(m_ucData.dSrcNum == 0.0)
				seSrc.SetWindowText("0", FALSE);
		}
		else 
		{
			if(i==0 &&j==1)
				res = (m_ucData.dDstNum -32.0)/1.8 ;
			else if(i==0 && j==2)
				res = m_ucData.dDstNum - 273.15;
			else if(i==1&& j==2)
				res = (m_ucData.dDstNum +459.67)/1.8;
			else if(i==1 &&j==0)
				res = m_ucData.dDstNum *1.8 + 32.0;
			else if(i==2 &&j==0)
				res = m_ucData.dDstNum+ 273.15;
			else if(i==2&& j==1)
				res = m_ucData.dDstNum *1.8 - 459.67;
			m_ucData.dSrcNum = res;
		    ConvNumtoStr(res ,pRes, TRUE);
//	 		ctrl = FindControl(w , ID_SRCEDIT);
			seSrc.SetWindowText( pRes, FALSE);
			if(m_ucData.dDstNum == 0.0)
				seDst.SetWindowText("0", FALSE);
		}
	}
	m_ucData.FocusEditID =-1;
	CloseCursor();
}

/*
    函数说明：单位转换处理函数
	参数：w 窗口指针
*/
void CConvWnd::DoConv()
{
//	UNITCONV  *ucData = (UNITCONV *)w->scrollDistance;
//    CTRL *ctrl ;
   char *pText ;
   double res  ;
   char pRes[25];
   double pro;
   
   if(m_ucData.UnitType == TYPE_TEMPER)
   {
        DoTempConv();
		return;
   }
   int  t =  g_fConvPro[m_ucData.UnitType][0];
   int i = m_ucData.nSrcSel;
   int j = m_ucData.nDstSel; 
   int  index ;

   if(m_ucData.FocusEditID==ID_SRCEDIT)
   {
		if(i == j)
		{
		   pro=1.0;
		   res = m_ucData.dSrcNum ;
		}
		else if(i<j)
		{
		  index = (2*t*i - 3 * i  - i*i)/2 + j;
		  res = m_ucData.dSrcNum *g_fConvPro[m_ucData.UnitType][index];
		}
		else 
		{
		  SWAP(i , j)
		  index = (2*t*i - 3 * i  - i*i)/2 + j;
		  res = m_ucData.dSrcNum /g_fConvPro[m_ucData.UnitType][index];
		}
	//	res =m_ucData.dSrcNum * pro;
		ConvNumtoStr(res ,pRes, TRUE);
//	 	ctrl = FindControl(w , ID_DSTEDIT);
	    seDst.SetWindowText(pRes, FALSE);
		m_ucData.dDstNum = res;
		if(m_ucData.dSrcNum == 0.0)
		{
//	        ctrl = FindControl(w , m_ucData.FocusEditID);
	        GetFocusEdit()->SetWindowText("0" , FALSE);
		}
   }
   else 
   {
	   if(i == j)
	   {
		   pro=1.0;
		   res = m_ucData.dDstNum ;
	   }
		else if(i<j)
		{
		  index = (2*t*i - 3 * i  - i*i)/2 + j;
		  res = m_ucData.dDstNum /g_fConvPro[m_ucData.UnitType][index];
		}
		else 
		{
		  SWAP(i , j)
		  index = (2*t*i - 3 * i  - i*i)/2 + j;
    	  res = m_ucData.dDstNum *g_fConvPro[m_ucData.UnitType][index];
		}
		//res = m_ucData.dDstNum  *pro;
		ConvNumtoStr(res ,pRes, TRUE);
//	 	ctrl = FindControl(w , ID_SRCEDIT);
	    seSrc.SetWindowText(pRes, FALSE);
		m_ucData.dSrcNum= res;
		if(m_ucData.dDstNum == 0.0)
		{
//	        ctrl = FindControl(w , m_ucData.FocusEditID);
	        GetFocusEdit()->SetWindowText("0" , FALSE);
		}
   }
   m_ucData.FocusEditID = m_ucData.FocusListID =-1;
   CloseCursor();
}

/*
    函数说明：单位转换窗口创建的调用函数
	参数：无
*/
static CConvWnd *g_pConvWnd = NULL;
void ConvAppRun()
{
	if(!g_pConvWnd)
		g_pConvWnd = new CConvWnd ;
	g_pConvWnd->Create("单位换算", WS_VISIBLE|WS_CAPTION, 
		DESK_TOP_RECT,HELP_UNIT );
    g_pConvWnd->ShowWindow();
}       


/*
    函数说明：计算器窗口创建的调用函数
	参数：无
*/
static CCalcWnd  *g_pCalcWnd = NULL;
void CalcAppRun()
{
	if(!g_pCalcWnd)
		g_pCalcWnd= new CCalcWnd;
	::SetCurrentApp(APPID_CALCULATOR);
    g_pCalcWnd->Create("计算器" , WS_VISIBLE | WS_CAPTION , DESK_TOP_RECT, HELP_CALCULATOR);
    g_pCalcWnd->ShowWindow();
}                



