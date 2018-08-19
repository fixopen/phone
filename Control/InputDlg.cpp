// InputDlg.cpp : implementation file
//
#include "stdafx.h"
#include "../resource.h"
#include "../Data/SkinStyle.h"
#include "InputDlg.h"

#define INPUTSOFTKEYRECT	CRect(0, 0, VOIPINPUT_FRMAE_WIDTH-1, VOIPINPUT_FRMAE_HEIGHT-1)
#define INPUTSOFTKEYRECT1	CRect(0, 28, VOIPINPUT_FRMAE_WIDTH-1, VOIPINPUT_FRMAE_HEIGHT-1)

#define VOIPINPUT_ENG_IDC			0xFC02
#define VOIPINPUT_FUHAO_IDC			0xFC03
#define VOIPINPUT_PINYIN_IDC		0xFC04
#define VOIPINPUT_HAND_IDC			0xFC05

#define VOIPINPUT_DELETE_VALUE		VK_BACK		//0xFF00
#define VOIPINPUT_COPY_VALUE		0xFF01
#define VOIPINPUT_PASTE_VALUE		0xFF02
#define VOIPINPUT_SPACE_VALUE		VK_SPACE	//0xFF03
#define VOIPINPUT_ENTER_VALUE		VK_RETURN	//0xFF04
#define VOIPINPUT_MOVE_VALUE        0xFF03
#define VOIPINPUT_HIDE_VALUE		0xFF05
#define VOIPINPUT_WRITE_VALUE		0xFF06
#define VOIPINPUT_PINYIN_VALUE		0xFF07
#define VOIPINPUT_ENG_VALUE			0xFF08
#define VOIPINPUT_ZIFU_VALUE		0xFF09
#define VOIPINPUT_COPSLOCK_VALUE	0xFF0A
#define HOUXUAN_LOCAL				0xFF0B
#define ARROW_LEFT					(HOUXUAN_LOCAL+6)
#define ARROW_RIGHT					(ARROW_LEFT+1)

#define HAND_REG1_VALUE				0xFD00
#define HAND_REG2_VALUE				0xFD01
#define HAND_REG3_VALUE				0xFD02
#define HAND_REG4_VALUE				0xFD03
#define HAND_REG5_VALUE				0xFD04
#define HAND_REG6_VALUE				0xFD05
#define HAND_REG7_VALUE				0xFD06
#define HAND_REG8_VALUE				0xFD07
#define HAND_REG9_VALUE				0xFD08
#define HAND_REG10_VALUE			0xFD09

#define HAND_TYPEHZ_VALUE			0xFD0A
#define HAND_TYPEZM_VALUE			0xFD0B
#define HAND_TYPESZ_VALUE			0xFD0C

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void FreeWriteLineBuff();
extern void InitWriteLineBuff();
/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog

//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "stdio.h"

#define	PINYIN_DATA_OFFSET	1664		//拼音数据在拼音表中的偏移地址
#define	PINYIN_TOTAL		416			//拼音总个数

#define	PINYIN_INPUT_TABLE	"\\flashdrv\\res_dat\\pinyin.dat"
#define HZ_FIRST_CHAR_FILE	"\\flashdrv\\res_dat\\firstchar.txt"

char * g_pPinYin;
#define	PINYIN_ADDRESS		g_pPinYin

void InitPinYin()
{
	g_pPinYin = new char[23000]; 
	FILE *fp = fopen(PINYIN_INPUT_TABLE,"rb");
	fread(g_pPinYin,1,23000,fp);
	fclose(fp);

	/*
	//测试手写库
	int ret = HWRE_LoadDict(&g_writedic);

	THWAttribute attr;
	unsigned short Result[MAXCANDNUM];
	int iCodeNum, j;
	
	attr.pRom = g_writedic;
		
	attr.pRam = ( unsigned char* )pRam;
	attr.iCandidateNum = MAXCANDNUM;			//set the number of candidate you want to be returned
	attr.dwRange = CHARSET_CNDEFAULT;			//set range
	
	iCodeNum = HWRecognize(aPenData, &attr, Result);
	*/
}

char * g_pHzFirstCharAddress;
void InitHzFirstChar()
{
	g_pHzFirstCharAddress = new char[6800]; 
	FILE *fp = fopen(HZ_FIRST_CHAR_FILE,"rb");
	fread(g_pHzFirstCharAddress,1,6800,fp);
	fclose(fp);
}
#else

#endif
/*============================================================================*/
//					 拼音输入法函数定义                                       //
/*============================================================================*/
///////////////////////////////////////////////////////////////
//
//	功能	输入拼音，查找汉字
//
//	参数	pinyin	输入拼音字符串，不区分大小写
//			hanzi	输出汉字字符串 buffer
//			size	输出汉字字符串 buffer 的大小，包含结束符
//
//	返回	0		没有查到，例如输入"z"
//			1 -> N	查到 N 个
//			-1		输入拼音字符串为不可能字符串，例如输入"zb"
//
//////////////////////////////////////////////////////////////
extern char *hwstrncpy(char *strDest, const char *strSource, size_t count );
char *hwstrncpy(char *strDest, const char *strSource, size_t count )
{
	strncpy(strDest,  strSource,  count );
	return strDest;
}

int FindPinYin(const char * pinyin, char * hanzi, int size)
{
	int		i, j, k, cmp;
	char	*hzp, lwrpinyin[PINYIN_LEN_MAX + 1];

	// nake input pinyin lower case
	hwstrncpy(lwrpinyin, pinyin, PINYIN_LEN_MAX);
	lwrpinyin[PINYIN_LEN_MAX] = 0x00;
	for (i = 0; i < PINYIN_LEN_MAX; i ++){
		if (*(lwrpinyin + i) == 0x00)
			break;
		if ((*(lwrpinyin + i) >= 'A') && (*(lwrpinyin + i) <= 'Z'))
			*(lwrpinyin + i) += 'a' - 'A';
	}
	// 二分法查找拼音
	for (i = 0, j = PINYIN_TOTAL - 1; i <= j; ){
		k = (i + j) / 2;
	#ifdef WIN32
		unsigned long offset = *((unsigned long *)(g_pPinYin + k * 4));
		unsigned char temp ;
		char *poffset = (char *)&offset;
		temp = *poffset;
		*poffset = *(poffset+3);
		*(poffset+3) = temp;
		
		temp = *(poffset+1);
		*(poffset+1) = *(poffset+2);
		*(poffset+2) = temp;

		hzp = (char *)( offset + PINYIN_ADDRESS + PINYIN_DATA_OFFSET);
	#else
		unsigned long val = *((unsigned long *)(PINYIN_ADDRESS + k * 4));
		unsigned long val0 = (val >> 24) & 0x000000FF;
		unsigned long val1 = (val >> 8) & 0x0000FF00;
		unsigned long val2 = (val << 8)  & 0x00FF0000;
		val = (val << 24) & 0xFF000000;
		val = val | val0 | val1 | val2;
		hzp = (char *)(val + PINYIN_ADDRESS + PINYIN_DATA_OFFSET);
	#endif
		cmp = strcmp(lwrpinyin, hzp);
		if (cmp == 0)
			break;
		else{
			if (cmp < 0)
				j = k - 1;
			else
				i = k + 1;
		}
	}
	// 查到
	if (i <= j){
		// 定位汉字串
		while (*hzp != 0x00)	
			hzp ++;
		hzp ++;
		// 如果输出 buffer 足够大，则输出全部汉字串
		i = (int)strlen(hzp);
		if (i <= size - 1){
			strcpy(hanzi, hzp);
			return i / 2;
		}
		// 否则只输出部分汉字串
		else{
			for (i = 0; i < ((size - 1) / 2) * 2; i += 2){
				*(hanzi + i) = *(hzp + i);
				*(hanzi + i + 1) = *(hzp + i + 1);
			}
			*(hanzi + i) = 0x00;
			return i / 2;
		}
	}
	// 没有查到
	else{
		// 输入拼音在有效拼音字符串范围之外，返回 -1
		if ((i == -1) || (i == PINYIN_TOTAL))
			return -1;
		// 输入拼音是有效字符串的子集，返回 0
		#ifdef WIN32
		unsigned long offset = *((long *)(PINYIN_ADDRESS + i * 4));
		unsigned char temp ;
		char *poffset = (char *)&offset;
		temp = *poffset;
		*poffset = *(poffset+3);
		*(poffset+3) = temp;
		
		temp = *(poffset+1);
		*(poffset+1) = *(poffset+2);
		*(poffset+2) = temp;

		hzp = (char *)( offset + PINYIN_ADDRESS + PINYIN_DATA_OFFSET);
	#else
		unsigned long val = *((unsigned long *)(PINYIN_ADDRESS + i * 4));
		unsigned long val0 = (val >> 24) & 0x000000FF;
		unsigned long val1 = (val >> 8) & 0x0000FF00;
		unsigned long val2 = (val << 8)  & 0x00FF0000;
		val = (val << 24) & 0xFF000000;
		val = val | val0 | val1 | val2;
		hzp = (char *)(val + PINYIN_ADDRESS + PINYIN_DATA_OFFSET);
	#endif

		
		if (strstr(hzp, lwrpinyin))
			return 0;
		// 输入拼音在有效拼音字符串范围之外，返回 -1
		else
			return -1;
	}
}

extern void PutIconPartial(HDC pdc, int x, int y, int iNO, CRect r);
extern void PutIconPartial(CDC *pdc, int x, int y, int iNO, CRect r);

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_chPyShowBuff, 0, sizeof(m_chPyShowBuff));
	memset(m_chSxShowBuff, 0, sizeof(m_chSxShowBuff));
	m_pOwnerEdit = NULL;
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SOFTKEY, OnSoftKey)
	ON_MESSAGE(WM_SOFTKEY_REG, OnSoftKeyReg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

BOOL CInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// this->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//init input
	SetInputType(0);
	CreateCtrl();
	m_CopyString = "";

	VERIFY(m_font.CreateFont(
		28,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体")));                 // lpszFacename
	SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDlg::SetOwnerEdit(CEdit *pEdit)
{
	m_pOwnerEdit = pEdit;
}

void CInputDlg::SetInputType(int y, int style)
{
	m_bIsShow = TRUE;
	m_xPoint = 0;
	m_yPoint = y;
	m_bIsLimit = FALSE;
	m_nRegStyle = 0;
	m_mInputMathStyle = style;

	//拼音
	memset(m_chPYBuff, 0, HANZI_LEN_MAX+1);
	memset(m_chPYInPut, 0, PINYIN_LEN_MAX+1);
	m_nPYInputCount = 0;						//记录输入字母的个数
	m_nPYLocal = 0;							//记录当前显示位置
	m_nPYLen = 0;								//记录返回汉字的个数
	m_bISPY = FALSE;
	m_bISLX = FALSE;

	//联想
	memset(m_chLXBuff, 0, 64+1);
	m_nLXLen =  0;
	m_nLXLocal = 0;
	
	memset(m_strPad,0,128);

	if(style == INPUTMATH_X || style == INPUTMATH_D)
	{
		m_mInputMath = INPUTMATH_ENG;
		m_iUpOrLowCase = style;
	}
	else
	{
		m_mInputMath =  style ;
		m_iUpOrLowCase = INPUTMATH_X;
	}
}

void CInputDlg::CreateCtrl()
{
	
	//拼音软键盘
	//英文键盘
//	int x1Local[] = {13, 62, 111, 159, 208, 256, 304, 353, 401, 450, 497, 546, 594, 642, 692, 740, 788};
//	int y1Local[] = {1, 40, 101, 158, 218};
	int x1Local[] = {7, 56, 105, 154, 203, 252, 301, 350, 399, 448, 497, 546, 595, 644, 693, 742, 791};
	int y1Local[] = {7, 57, 114, 171, 220};
	CRect r2[64];

	CPoint thePoint;

	for(int i = 0; i < 62; i++)
	{
		if(i < 14)
		{
			r2[i] = CRect(x1Local[i+2]+1, y1Local[0]+1, x1Local[i+2+1]-1, y1Local[1]-1);
		}
		else if(i < 30)
		{
			r2[i] = CRect(x1Local[i-14]+1, y1Local[1]+1, x1Local[i-14+1]-1, y1Local[2]-1);
		}
		else if(i < 46)
		{
			r2[i] = CRect(x1Local[i-30]+1, y1Local[2]+1, x1Local[i-30+1]-1, y1Local[3]-1);
		}
		else
		{
			r2[i] = CRect(x1Local[i-46]+1, y1Local[3]+1, x1Local[i-46+1]-1, y1Local[4]-1);
		}
	}

	thePoint.x = m_xPoint;
	thePoint.y = m_yPoint;
	UINT16 vSoftKeyP[] = {HOUXUAN_LOCAL, HOUXUAN_LOCAL+1, HOUXUAN_LOCAL+2, HOUXUAN_LOCAL+3, HOUXUAN_LOCAL+4, ARROW_LEFT, ARROW_RIGHT, VOIPINPUT_DELETE_VALUE, VOIPINPUT_COPY_VALUE, VOIPINPUT_PASTE_VALUE, VOIPINPUT_SPACE_VALUE, VOIPINPUT_ENTER_VALUE, VOIPINPUT_MOVE_VALUE, VOIPINPUT_HIDE_VALUE,\
		'-', '_', '@', '0', '1', '2','3', 'a','b', 'c','d', 'e','f', 'g','h', 'i',\
	'!', '?',',', '.','4', '5','6', 'j','k', 'l','m', 'n','o', 'p','q', 'r',\
	VOIPINPUT_WRITE_VALUE, VOIPINPUT_PINYIN_VALUE, VOIPINPUT_ENG_VALUE, VOIPINPUT_ZIFU_VALUE,'7', '8','9', 's','t', 'u','v', 'w','x', 'y','z', 0};

	if( !(m_oSoftkeyP.Create(Data::g_inputPinyinBMPID[Data::g_skinstyle], thePoint, 62, r2, vSoftKeyP, INPUTSOFTKEYRECT, this, 0, VOIPINPUT_PINYIN_IDC)) )
        return ;
	m_oSoftkeyP.ModifyStyle(0, SS_NOTIFY);

	//ENG INPUT
	for(i = 0; i < 48; i++)
	{
		if(i < 16)
		{
			r2[i] = CRect(x1Local[i]+1+m_xPoint, y1Local[1]+1+m_yPoint, x1Local[i+1]-1+m_xPoint, y1Local[2]-1+m_yPoint);
		}
		else if(i < 32)
		{
			r2[i] = CRect(x1Local[i-16]+1+m_xPoint, y1Local[2]+1+m_yPoint, x1Local[i-16+1]-1+m_xPoint, y1Local[3]-1+m_yPoint);
		}
	
		else
		{
			r2[i] = CRect(x1Local[i-32]+1+m_xPoint, y1Local[3]+1+m_yPoint, x1Local[i-32+1]-1+m_xPoint, y1Local[4]-1+m_yPoint);
		}
	}

	for( i = 0; i < 7; i++)
	{
		r2[i+48] = CRect(x1Local[9+i]+1+m_xPoint, y1Local[0]+1+m_yPoint, x1Local[9+i+1]-1+m_xPoint, y1Local[1]-1+m_yPoint);
	}

	UINT16 vSoftKeyD[] = {'-', '_', '@', '0', '1', '2','3', 'a','b', 'c','d', 'e','f', 'g','h', 'i',
	'!', '?',',', '.','4', '5','6', 'j','k', 'l','m', 'n','o', 'p','q', 'r',
	VOIPINPUT_WRITE_VALUE, VOIPINPUT_PINYIN_VALUE, VOIPINPUT_ENG_VALUE, VOIPINPUT_ZIFU_VALUE,'7', '8','9', 's','t', 'u','v', 'w','x', 'y','z', VOIPINPUT_COPSLOCK_VALUE,
	VOIPINPUT_DELETE_VALUE, VOIPINPUT_COPY_VALUE, VOIPINPUT_PASTE_VALUE, VOIPINPUT_SPACE_VALUE, VOIPINPUT_ENTER_VALUE, VOIPINPUT_MOVE_VALUE, VOIPINPUT_HIDE_VALUE};
	UINT16 vSoftKeyF[] = {'-', '_', '@', '#', '+', '-','*', '/','=', '%','&', '~','^', /*'￥'*/0xA3A4,'$', 0xA1EA,
	'!', '?',',', '.', 0xA1A3, 0xA1A2 ,'<', '>','(', ')','[', ']','{', '}', 0xA1B6, 0xA1B7,
	VOIPINPUT_WRITE_VALUE, VOIPINPUT_PINYIN_VALUE, VOIPINPUT_ENG_VALUE, VOIPINPUT_ZIFU_VALUE,':', ';', 0xA1AD, 0xA6B5, 0xA6C1, 0xA6C2, 0xA6C3, '|', '\'', '\'','"', '"',
	VOIPINPUT_DELETE_VALUE, VOIPINPUT_COPY_VALUE, VOIPINPUT_PASTE_VALUE, VOIPINPUT_SPACE_VALUE, VOIPINPUT_ENTER_VALUE, VOIPINPUT_MOVE_VALUE, VOIPINPUT_HIDE_VALUE};
	if( !(m_oSoftkeyEng.Create(Data::g_inputEngBMPID[Data::g_skinstyle], thePoint, 48+7, r2, vSoftKeyD, INPUTSOFTKEYRECT, this, 0, VOIPINPUT_ENG_IDC)) )
        return ;
	m_oSoftkeyEng.ModifyStyle(0, SS_NOTIFY);
	if( !(m_oSoftkeySymbol.Create(Data::g_inputSysmolBMPID[Data::g_skinstyle], thePoint, 48+7, r2, vSoftKeyF, INPUTSOFTKEYRECT, this, 0, VOIPINPUT_FUHAO_IDC)) )
        return ;
	m_oSoftkeySymbol.ModifyStyle(0, SS_NOTIFY);

	UINT16 vSoftKeyH[] = {HAND_REG1_VALUE, HAND_REG2_VALUE, HAND_REG3_VALUE, HAND_REG4_VALUE,HAND_REG5_VALUE, HAND_REG6_VALUE,HAND_REG7_VALUE, HAND_REG8_VALUE, HAND_REG9_VALUE, VOIPINPUT_DELETE_VALUE, VOIPINPUT_COPY_VALUE, VOIPINPUT_PASTE_VALUE, VOIPINPUT_SPACE_VALUE, VOIPINPUT_ENTER_VALUE, VOIPINPUT_MOVE_VALUE, VOIPINPUT_HIDE_VALUE,\
	'-', '_', '@', '0', '1', '2','3', 
	'!', '?',',', '.','4', '5','6', 
	VOIPINPUT_WRITE_VALUE, VOIPINPUT_PINYIN_VALUE, VOIPINPUT_ENG_VALUE, VOIPINPUT_ZIFU_VALUE,'7', '8','9'
	}; //40
	for(i = 0; i < 37; i++)
	{
		if(i <= 15)
		{
			r2[i] = CRect(x1Local[i]+1, y1Local[0]+1, x1Local[i+1]-1, y1Local[1]-1);
		}
		else if(i <= 22)
		{
			r2[i] = CRect(x1Local[i-16]+1, y1Local[1]+1, x1Local[i-16+1]-1, y1Local[2]-1);
		}
		else if(i <= 29)
		{
			r2[i] = CRect(x1Local[i-23]+1, y1Local[2]+1, x1Local[i-23+1]-1, y1Local[3]-1);
		}
		else 
		{
			r2[i] = CRect(x1Local[i-30]+1, y1Local[3]+1, x1Local[i-30+1]-1, y1Local[4]-1);
		}
	}
	
	if( !(m_oSoftkeyHand.Create(Data::g_inputWriteBMPID[Data::g_skinstyle], thePoint, 37, r2, vSoftKeyH, INPUTSOFTKEYRECT, this, 0, VOIPINPUT_HAND_IDC)) )
        return ;
//	m_oSoftkeyHand.SetHandWriteRect(CRect(361, 48, 560, 211), CRect(578, 48, 779, 211));
	m_oSoftkeyHand.SetHandWriteRect(CRect(355, 63, 355+214, 63+154), CRect(573, 63, 573+214, 64+154));
	m_oSoftkeyHand.ModifyStyle(0, SS_NOTIFY);


	l_SetInputWindow();
	return ;
}

void CInputDlg::hide(BOOL flag)
{
	FreeWriteLineBuff();

	m_bIsShow = FALSE;
	ShowWindow(SW_HIDE);
/*
	if(flag)
		OnPaint();
*/
}

void CInputDlg::show(BOOL flag)
{
	m_bIsShow = TRUE;
	CRect rt;
	GetWindowRect(&rt);
	//ScreenToClient(&rt);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, rt.left, rt.top, rt.Width(), rt.Height(), 0);
	ShowWindow(SW_SHOW);
	/*
	if(flag)
		OnPaint();
	*/
}

void CInputDlg:: hideorshow()
{
	if(m_bIsShow)
		hide();
	else
		show();
}

BOOL CInputDlg::MoveTo(int y)
{
	CRect myRect;
	GetClientRect(&myRect);
//	hide();   //lxz 2007 11 26
	MoveWindow(0, y, myRect.Width(), myRect.Height(), FALSE);

	return TRUE;
}

void CInputDlg::l_PutIcon()
{
	//CDC *pdc = GetDC();

	if(m_mInputMath == INPUTMATH_F)
	{
		m_oSoftkeySymbol.SendMessage(WM_PAINT,0, 0);
	}
	
	else if(m_mInputMath == INPUTMATH_ENG)
	{
		m_oSoftkeyEng.SendMessage(WM_PAINT,0, 0);
	}

	else if(m_mInputMath == INPUTMATH_P)
	{
		m_oSoftkeyP.SendMessage(WM_PAINT,0, 0);
	}
	else if(m_mInputMath == INPUTMATH_P)
	{
		m_oSoftkeyHand.SendMessage(WM_PAINT,0, 0);
	}
//	ReleaseDC(pdc);
}

void CInputDlg::l_SetInputWindow()
{
//	int style;
	m_oSoftkeyEng.ShowWindow(SW_HIDE);
	m_oSoftkeySymbol.ShowWindow(SW_HIDE);
	m_oSoftkeyP.ShowWindow(SW_HIDE);
	m_oSoftkeyHand.ShowWindow(SW_HIDE);

	if(m_mInputMath == INPUTMATH_ENG)
	{
		if(m_iUpOrLowCase == INPUTMATH_D)
			m_oSoftkeyEng.SetParameters(Data::g_inputEng1BMPID[Data::g_skinstyle]);
		else if(m_iUpOrLowCase == INPUTMATH_X)
			m_oSoftkeyEng.SetParameters(Data::g_inputEngBMPID[Data::g_skinstyle]);
		m_oSoftkeyEng.ShowWindow(SW_SHOW);
	}
	else if(m_mInputMath == INPUTMATH_F)
	{
		m_oSoftkeySymbol.SetParameters(Data::g_inputSysmolBMPID[Data::g_skinstyle]);
		m_oSoftkeySymbol.ShowWindow(SW_SHOW);
	}
	else if(m_mInputMath == INPUTMATH_P)
	{
		m_oSoftkeyP.SetParameters(Data::g_inputPinyinBMPID[Data::g_skinstyle]);
		m_oSoftkeyP.ShowWindow(SW_SHOW);
	}
	else if(m_mInputMath == INPUTMATH_H)
	{
		m_oSoftkeyHand.SetParameters(Data::g_inputWriteBMPID[Data::g_skinstyle]);
		m_oSoftkeyHand.ShowWindow(SW_SHOW);
	}
}

void CInputDlg::ChangMode(int iMode,int iRangeOrUpLow)
{
	if(iMode == INPUTMATH_H)
		InitWriteLineBuff();
	else
		FreeWriteLineBuff();

	if(m_mInputMath != INPUTMATH_F)
		m_mOldInputMath = m_mInputMath;	

	if(m_mInputMath != iMode)
	{
		m_mInputMath = iMode;
	}
	else if(m_mInputMath == INPUTMATH_ENG && m_iUpOrLowCase != iRangeOrUpLow)
	{
		m_iUpOrLowCase = iRangeOrUpLow;
	}
	else 
	{
		return;
	}
	
	memset(m_chSxShowBuff, 0, sizeof(m_chSxShowBuff));
	memset(m_chPyShowBuff, 0, sizeof(m_chPyShowBuff));

	l_SetInputWindow();
	l_PutIcon();
}

void CInputDlg::OnSoftKeyP(NMHDR_*hm, LRESULT*)
{
	UINT16 wParam = (UINT16)hm->wParam;
	if(wParam == '-' || wParam == '_' || wParam == '@'|| wParam == '!' \
		|| wParam == '?' || wParam == ',' || wParam == '.' || (wParam >= '0' && wParam <= '9') || wParam == VOIPINPUT_SPACE_VALUE || wParam == VOIPINPUT_ENTER_VALUE)	//立即可以显示的字符
	{
		if(m_bIsLimit)
		{
			if(wParam == '\n')
			{
				return;			//lxz 2005.1.7 \n 直接返回
			}
		}
		if(m_pOwnerEdit)
		{
			char txt[3];
			txt[0] = (char)wParam;
			txt[1] = 0;
			txt[2] = 0;

			wchar_t wch[2];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );
			if(wParam == VK_RETURN || wParam == VK_BACK)
			{
				m_pOwnerEdit->SendMessage(WM_CHAR, wParam, 1);
			}
			else
			{
				m_pOwnerEdit->ReplaceSel(wch);
			}
		}
	}
	
	else if(wParam == VOIPINPUT_DELETE_VALUE)		//删除键
	{
		if(m_bISPY && strlen(m_chPYInPut) > 0)
		{
			//lxz 2005.6.16 
			m_nPYLocal = 0;   //显示位置复位

			--m_nPYInputCount;
			m_chPYInPut[m_nPYInputCount] = '\0';
			//显示一下拼音区, 汉字区
			PinYinHZ(0);
			PinYinInPut(1);
		}
		else
		{
			m_bISPY = FALSE;
		}
		if(!m_bISPY)
		{
			if(m_pOwnerEdit)
				m_pOwnerEdit->SendMessage(WM_CHAR, wParam, 1);
		}
	}

	else if(wParam == VOIPINPUT_ZIFU_VALUE)		//切换到符号输入
	{
		ChangMode(INPUTMATH_F, -1);
	}

	else if(wParam == VOIPINPUT_ENG_VALUE )		//切换到英文
	{
		ChangMode(INPUTMATH_ENG, INPUTMATH_X);
	}

	else if(wParam == VOIPINPUT_WRITE_VALUE )		//切换到手写
	{
		ChangMode(INPUTMATH_H, -1);
	}
	else if(wParam == VOIPINPUT_PINYIN_VALUE )		
	{
		return;
	}

	else if(wParam == VOIPINPUT_HIDE_VALUE )		//隐藏
	{
		hide();
	}

	else if((wParam >= HOUXUAN_LOCAL) && (wParam <= (HOUXUAN_LOCAL+5)))		//汉字区
	{
		if(m_bISLX)
		{
			char *ch = (char *)(m_chLXBuff + (m_nLXLocal + wParam - HOUXUAN_LOCAL)*2);
			if(strlen(ch)>0)
			{
				//UINT16 temp = MAKEWORD(*(ch+1),*ch);
				if(m_pOwnerEdit)
				{
					char txt[3];
					txt[0] = *(ch);
					txt[1] = *(ch+1);
					txt[2] = 0;

					wchar_t wch[2];
					memset(wch, 0, sizeof(wch));
					mbstowcs( wch, txt, strlen(txt) );
					m_pOwnerEdit->ReplaceSel(wch);
				}
//				LianXiang(temp, 1, 0);
			}
		}
		else
		{
			char *ch = (char *)(m_chPYBuff + (m_nPYLocal + wParam - HOUXUAN_LOCAL)*2);
			if(strlen(ch)>0)
			{
			//	UINT16 temp = MAKEWORD(*(ch+1),*ch);
				if(m_pOwnerEdit)
				{
					char txt[3];
					txt[0] = *(ch);
					txt[1] = *(ch+1);
					txt[2] = 0;

					wchar_t wch[2];
					memset(wch, 0, sizeof(wch));
					mbstowcs( wch, txt, strlen(txt) );
					m_pOwnerEdit->ReplaceSel(wch);
				}
//				LianXiang(temp, 1, 0);
			}
		}
		m_bISLX = TRUE;
		m_bISPY = FALSE;
		PinYinInPut(0);
	}
	else if(wParam == ARROW_LEFT)		//左翻页
	{
		if(m_bISLX)
		{
		//	LianXiang(NULL, 1, 1);
		}
		else
		{
			PinYinHZ(1);
		}
	}
	else if(wParam == ARROW_RIGHT)		//右翻页
	{
		if(m_bISLX)
		{
		//	LianXiang(NULL, 1, 2);
		}
		else
		{
			PinYinHZ(2);
		}
	}
	else			//输入的是字符
	{
		m_bISLX = FALSE;
		m_bISPY = TRUE;
		m_chPYInPut[m_nPYInputCount++] = (char)wParam;
		PinYinHZ(0);
		PinYinInPut(1);
	}
}

void CInputDlg::OnSoftKeySym(NMHDR_*hm, LRESULT*index)
{

	//todo lxz

	//2005.4.9 lxz 从符号返回
	//ChangMode(m_mOldInputMath,-1);
	int wParam = hm->wParam;
	if(wParam == VOIPINPUT_ENG_VALUE )
	{
		ChangMode(INPUTMATH_ENG, INPUTMATH_X);
	}
	else if(wParam == VOIPINPUT_PINYIN_VALUE)		//切换到拼音
	{
		ChangMode(INPUTMATH_P, -1);
	}

	else if(wParam == VOIPINPUT_WRITE_VALUE )		//切换到手写
	{
		ChangMode(INPUTMATH_H, -1);
	}
	else if(wParam == VOIPINPUT_ZIFU_VALUE)
	{
		return;
	}
	else if(wParam == VOIPINPUT_HIDE_VALUE )		//隐藏
	{
		hide();
	}

	else
	{
		if(m_pOwnerEdit)
		{
			char txt[3] = {0};
			
			if(wParam > 255)
			{
				txt[1] = wParam&0xFF;
				txt[0] = (wParam>>8);
			}
			else
				txt[0] = wParam;
			

			wchar_t wch[2];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );
			if(wParam == VK_RETURN || wParam == VK_BACK)
			{
				m_pOwnerEdit->SendMessage(WM_CHAR, wParam, 1);
			}
			else
			{
				m_pOwnerEdit->ReplaceSel(wch);
			}
		}
	}
}

void CInputDlg::OnSoftKeyHand(NMHDR_*hm, LRESULT*index)
{
	int wParam = hm->wParam;
	if(wParam ==VOIPINPUT_ZIFU_VALUE)		//change to zifu
	{
		ChangMode(INPUTMATH_F,-1);//-1,不用
	}
	else if(wParam == VOIPINPUT_PINYIN_VALUE )		//切换到拼音
	{
		ChangMode(INPUTMATH_P, -1);
	}
	else if(wParam == VOIPINPUT_ENG_VALUE )		//切换到英文
	{
		ChangMode(INPUTMATH_ENG, -1);
	}
	else if(wParam == VOIPINPUT_WRITE_VALUE)
	{
		return;
	}
	else if(wParam == VOIPINPUT_HIDE_VALUE )		//隐藏
	{
		hide();
	}

	else if(wParam >= HAND_REG1_VALUE && wParam <= HAND_REG10_VALUE)
	{
		if(m_pOwnerEdit)
		{
			char *ch = m_chSxShowBuff+(wParam - HAND_REG1_VALUE)*2;
			char txt[3];
			txt[0] = *ch;
			txt[1] = *(ch+1);
			txt[2] = 0;

			wchar_t wch[2];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt));
			
			int n, n1;
			m_pOwnerEdit->GetSel(n, n1);
			m_pOwnerEdit->SetSel(n-1, n);
			m_pOwnerEdit->ReplaceSel(wch);
		}
	}
	else
	{
		if(m_pOwnerEdit)
		{
			char txt[3];
			txt[0] = wParam;
			txt[1] = 0;
			txt[2] = 0;

			wchar_t wch[2];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );
			if(wParam == VK_RETURN || wParam == VK_BACK)
			{
				m_pOwnerEdit->SendMessage(WM_CHAR, wParam, 1);
			}
			else
			{
				m_pOwnerEdit->ReplaceSel(wch);
			}
		}
	}
}

void CInputDlg::OnSoftKeyEng(NMHDR_*hm, LRESULT*index)
{

	int wParam = hm->wParam;
	if(wParam ==VOIPINPUT_ZIFU_VALUE)		//change to zifu
	{
		ChangMode(INPUTMATH_F,-1);//-1,不用
	}
	else if(wParam == VOIPINPUT_PINYIN_VALUE )		//切换到拼音
	{
		ChangMode(INPUTMATH_P, -1);
	}
	else if(wParam == VOIPINPUT_WRITE_VALUE )		//切换到手写
	{
		ChangMode(INPUTMATH_H, -1);
	}
	else if(wParam == VOIPINPUT_ENG_VALUE )		
	{
		return;
	}

	else if(wParam == VOIPINPUT_HIDE_VALUE )		//隐藏
	{
		hide();
	}

	else if(wParam == VOIPINPUT_COPSLOCK_VALUE) 
	{
		if(m_iUpOrLowCase == INPUTMATH_X)
		{
			m_iUpOrLowCase = INPUTMATH_D;
		}
		else
		{
			m_iUpOrLowCase = INPUTMATH_X;
		}
		l_SetInputWindow();
		l_PutIcon();
	}
	//end 

	else
	{
		if(m_iUpOrLowCase == INPUTMATH_D)
		{
			if(wParam >= 'a' && wParam <= 'z')
			{
				wParam -= 0x20;
			}
		}
		
		if(m_bIsLimit)
		{
			if(wParam == '\n')
			{
				return;			//lxz 2005.1.7 \n 直接返回
			}
		}
		
		if(m_pOwnerEdit)
		{
			char txt[3];
			txt[0] = wParam;
			txt[1] = 0;
			txt[2] = 0;

			wchar_t wch[2];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );
			if(wParam == VK_RETURN || wParam == VK_BACK)
			{
				m_pOwnerEdit->SendMessage(WM_CHAR, wParam, 1);
			}
			else
			{
				m_pOwnerEdit->ReplaceSel(wch);
			}
		}
	}
}

// int style 0		初始化
//			 1		左翻页
//			 2      右翻页
void CInputDlg ::PinYinHZ(int style)
{
	if(style == 0)		//初始化，第一调用
	{
		//lxz 2005.6.16
		m_nPYLocal = 0;	

		if(strlen(m_chPYInPut) == 0)
		{
			memset(m_chPYBuff, 0, HANZI_LEN_MAX+1);						
		}
		int n = FindPinYin(m_chPYInPut, m_chPYBuff, HANZI_LEN_MAX);								
		if(n < 0)
		{
			m_chPYInPut[--m_nPYInputCount] = '\0';
		}
		else
		{
			m_nPYLen = n;
		}
	}
	else if(style == 1)		//左翻页
	{
		m_nPYLocal -= PY_HZ_SHOWLEN;
		m_nPYLocal = MAX(m_nPYLocal, 0);
	}
	else if(style == 2)		//右翻页
	{
		if((m_nPYLocal+PY_HZ_SHOWLEN) > m_nPYLen)
			return;
		m_nPYLocal += PY_HZ_SHOWLEN;
		m_nPYLocal = MIN(m_nPYLocal, m_nPYLen-1);
	}

	memset(m_chPyShowBuff, 0, sizeof(m_chPyShowBuff));
	int n = MIN(PY_HZ_SHOWLEN, (m_nPYLen-m_nPYLocal));
	memcpy(m_chPyShowBuff, m_chPYBuff+(m_nPYLocal*2), sizeof(char)*n*2); 
	Invalidate();
	//SendMessage(WM_PAINT);
}

//int style 0 重新输入，将m_chPYInput[]清零
//			1 显示输入
void CInputDlg ::PinYinInPut(int style)
{
	if(style == 0)
	{
		m_nPYInputCount = 0;
		memset(m_chPYInPut, 0, PINYIN_LEN_MAX+1);
	}

	Invalidate();
}


void CInputDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CInputDlg ::OnSoftKey(WPARAM w, LPARAM l)
{
	NMHDR_ hm = {this, CN_COMMAND, w, (UINT32)0};
	if(w == VOIPINPUT_HIDE_VALUE)   //hide 
	{
		m_CopyString = "";
		hide();
		return;
	}
	else if(w == VOIPINPUT_MOVE_VALUE)		//move
	{
		CRect rt;
		GetWindowRect(&rt);
		if(rt.top != 0)
			MoveTo(TOP_INPUT);
		else
			MoveTo(BOTTOM_INPUT);
		return;		
	}
	else if(w == VOIPINPUT_COPY_VALUE)		//copy
	{
		if(m_pOwnerEdit)
		{
			int n, n1;
			m_pOwnerEdit->GetSel(n, n1);
			if(n != n1)
			{
				CString s;
				m_pOwnerEdit->GetWindowText(s);
				m_CopyString = s.Mid(n, n1-n);
			}
		}
		return;
	}

	else if(w == VOIPINPUT_PASTE_VALUE)		//paste
	{
		if(m_CopyString != "")
		{
			if(m_pOwnerEdit)
			{
				m_pOwnerEdit->ReplaceSel(m_CopyString);
			//	m_CopyString = "";
			}
		}
		return;
	}

	switch(l)
	{
	case VOIPINPUT_ENG_IDC:
		OnSoftKeyEng(&hm, NULL);
		break;
	case VOIPINPUT_FUHAO_IDC:
		OnSoftKeySym(&hm, NULL);
		break;
	case VOIPINPUT_PINYIN_IDC:
		OnSoftKeyP(&hm, NULL);
		break;
	case VOIPINPUT_HAND_IDC:
		OnSoftKeyHand(&hm, NULL);
	default:
		break;
	}
}

void CInputDlg ::OnSoftKeyReg(WPARAM w, LPARAM l)
{ 
	memset(m_chSxShowBuff, 0, sizeof(m_chSxShowBuff));
	/*if(w==0)
	{
		char *str = "一二三四五六七八九十";
		memcpy(m_chSxShowBuff, str, strlen(str)); 
	}
	else
	{
		char *str1 = "北京易度科技有限公司";
		memcpy(m_chSxShowBuff, str1, strlen(str1)); 
	}
	*/
	memcpy(m_chSxShowBuff, (void *)w, l*2);

	if(m_pOwnerEdit)
	{
		char *ch = m_chSxShowBuff;
		char txt[3];
		txt[0] = *ch;
		txt[1] = *(ch+1);
		txt[2] = 0;

		wchar_t wch[2];
		memset(wch, 0, sizeof(wch));
		mbstowcs( wch, txt, strlen(txt));
		m_pOwnerEdit->ReplaceSel(wch);

		m_pOwnerEdit->SetFocus();
		m_pOwnerEdit->Invalidate();
	}
	Invalidate();
}

void CInputDlg::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	CDialog::OnPaint();
    
	if(m_mInputMath == INPUTMATH_P)
	{
		CDC *pDC = m_oSoftkeyP.GetDC();
		CFont *oldFont = pDC->SelectObject(&m_font);
		//	CRect r = CRect(m_oSoftkeyP.m_pKeys[0].sKeyRect.right+1, m_oSoftkeyP.m_pKeys[0].sKeyRect.top+1, m_oSoftkeyP.m_pKeys[1].sKeyRect.left-1, m_oSoftkeyP.m_pKeys[1].sKeyRect.bottom-1);
		CRect rA[] = 
		{
			m_oSoftkeyP.m_pKeys[0].sKeyRect, 	m_oSoftkeyP.m_pKeys[1].sKeyRect,
			m_oSoftkeyP.m_pKeys[2].sKeyRect,	m_oSoftkeyP.m_pKeys[3].sKeyRect, 
			m_oSoftkeyP.m_pKeys[4].sKeyRect, 	m_oSoftkeyP.m_pKeys[5].sKeyRect
		};
		CRect r1 = CRect(m_oSoftkeyP.m_pKeys[0].sKeyRect.left+1, m_oSoftkeyP.m_pKeys[0].sKeyRect.top+1, m_oSoftkeyP.m_pKeys[5].sKeyRect.left-1, m_oSoftkeyP.m_pKeys[5].sKeyRect.bottom-1);
	
		PutIconPartial(pDC, r1.left, r1.top, Data::g_inputPinyinBMPID[Data::g_skinstyle], r1);
		for(int i=0;i<PY_HZ_SHOWLEN;i++)
		{
			char txt[3];
			txt[0] = m_chPyShowBuff[i*2];
			txt[1] = m_chPyShowBuff[i*2+1];
			txt[2] = '\0';

			rA[i].DeflateRect(1,1);
			wchar_t wch[128];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(wch, wcslen(wch), rA[i], DT_WORDBREAK | DT_CENTER);
		}

		//拼音字母输入
        CRect r = CRect(1+1, m_oSoftkeyP.m_pKeys[0].sKeyRect.top-1, m_oSoftkeyP.m_pKeys[0].sKeyRect.left-1, m_oSoftkeyP.m_pKeys[0].sKeyRect.bottom-1);
		PutIconPartial(pDC, r.left, r.top, Data::g_inputPinyinBMPID[Data::g_skinstyle], r);
		if(strlen(m_chPYInPut))
		{
			r.DeflateRect(1,1);
		
			wchar_t wch[128];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, m_chPYInPut, strlen(m_chPYInPut) );
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(wch, wcslen(wch), r, DT_WORDBREAK | DT_CENTER);
		}
		pDC->SelectObject(oldFont);
		ReleaseDC(pDC);
	}

	else if(m_mInputMath == INPUTMATH_H)
	{
		CDC *pDC = m_oSoftkeyHand.GetDC();
		CFont *oldFont = pDC->SelectObject(&m_font);
		CRect r = CRect(m_oSoftkeyHand.m_pKeys[0].sKeyRect.left+1, m_oSoftkeyHand.m_pKeys[0].sKeyRect.top+1, m_oSoftkeyHand.m_pKeys[8].sKeyRect.right-1, m_oSoftkeyHand.m_pKeys[8].sKeyRect.bottom-1);
		PutIconPartial(pDC, r.left,r.top, Data::g_inputEngBMPID[Data::g_skinstyle], r);
		CRect rA[SX_HZ_SHOWLEN];
		for(int i = 0; i < SX_HZ_SHOWLEN; i++)
		{
			rA[i] = m_oSoftkeyHand.m_pKeys[i].sKeyRect;
			rA[i].DeflateRect(1,1);
			char txt[3];
			txt[0] = m_chSxShowBuff[i*2];
			txt[1] = m_chSxShowBuff[i*2+1];;
			txt[2] = '\0';

			wchar_t wch[128];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(wch, rA[i], DT_WORDBREAK | DT_CENTER);	
		}
		pDC->SelectObject(oldFont);
		ReleaseDC(pDC);
	}
	if(m_pOwnerEdit)
		m_pOwnerEdit->SetFocus();
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CInputDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pOwnerEdit)
		m_pOwnerEdit->SetFocus();

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CInputDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL ret = CDialog::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_LBUTTONDOWN)   
	{         
		if(m_pOwnerEdit)
		{
			m_pOwnerEdit->SetFocus();
			m_pOwnerEdit->Invalidate();
		}
	}
	else if(pMsg->message == WM_LBUTTONUP)   
	{         
		if(m_pOwnerEdit)
		{
			m_pOwnerEdit->SetFocus();
			m_pOwnerEdit->Invalidate();
		}
	}
	return ret;
}
