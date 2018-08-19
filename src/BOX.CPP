/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             messagebox.cpp                                                 */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             CMessageBox类定义                                              */
/*             MessageBox函数定义                                             */
/*             CInputBox类定义                                                */
/*             InputBox函数定义                                               */
/*             CNumInputBox类定义                                             */
/*             NumInputBox函数定义                                            */
/*             DateInputbox函数定义                                           */
/*             TimeInputBox函数定义                                           */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-16                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-08-07    完善CInputBox                                    */
/*                           增加NumInputBox                                  */
/*                           增加白亚伟提供的拼音输入法函数                   */
/*                                                                            */
/*             2001-08-08    完善电话号码输入框                               */
/*                           增加CPinYin类                                    */
/*                                                                            */
/*             2001-08-09    完成输入框(拼音,手写,符号,英文,大小转换)         */
/*                                                                            */
/*             2001-10-15    增加日期输入框类及函数                           */
/*                           增加时间输入框类及函数                           */
/*                                                                            */
/*             2001-11-1     因为界面改变修改很多地方                         */
/*             2001-11-6     增加了取汉字拼音缩写的函数（程晓东）             */
/*             2001-11-15    郝庆丰修改时间输入界面的大部分                   */
/******************************************************************************/
#include "messagebox.h"
#include "edit.h"
#include "softkey.h"
#include "writepad.h"
#include "menu.h"
#include "pinyin.h"
#include "phoneapp.h"
#include "sysSetup.h"
#include "pendrv.h"

#define MARGIN          3
#define BUTTON_DISTANCE 7
#define BUTTON_WIDTH    40
#define BUTTON_HEIGHT   18


////////////////////////////////////////////////////////////////////////////////
//
class CMessageBox : public CDialog
{
private:
	CButton    m_obutYesNoOkCancel[4];
	UINT32     m_nType;
	UINT32     m_nIcon;
	char       m_strText[128];

private:
	void       PaintTextAndIcon(CDC &dc);

public:
	BOOL     Create(char * strText, char *strCaption, UINT32 nType);
	virtual  void PaintTitle(CDC &dc);
	UINT32   GetMessageBoxType(){return m_nType;}
	virtual  BOOL OnCreate();
	virtual  void OnPaint(CDC &dc);
};
////////////////////////////////////////////////////////////////////////////////
//
BOOL CMessageBox::Create(char * strText, char *strCaption, UINT32 nType)
{
	m_nType = nType;	
	if(strText)
		hwstrncpy(m_strText, strText, 127);
	else 
		m_strText[0]= 0;

	if(!CFrameWnd::Create(strCaption, WS_CAPTION|WS_VISIBLE|WS_BORDER, PHONE_WND_RECT, 0))
		return FALSE;
//	::SetCurrentApp();
	return TRUE;
	
}

////////////////////////////////////////////////////////////////////////////////
//
void CMessageBox::PaintTitle(CDC &dc)
{
   CRect r(m_sRect);
   r.bottom = r.top + WND_TITLE_HEIGHT-1;
   dc.SetBackColor(BLACK);
   dc.EraseRect(r); 
   dc.SetPenColor(WHITE); 
   dc.DrawText(r.left +2, r.top + 1, m_strName);
   /*
   if(m_dwStyle & WS_CONTROLBOX)
   {
	   r.left = r.right - WND_TITLE_HEIGHT-1;
	   r.DeflateRect(1,1); 
	   dc.Draw3dButton(r, NULL); 
   }
   */
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CMessageBox::OnCreate()
{
	if(!CDialog::OnCreate())
		return FALSE;

	if(m_nType&(MB_ICONSTOP))
        m_nIcon  = ICON_STOP;
    else  if(m_nType&(MB_ICONQUESTION))
        m_nIcon  = ICON_QUESTION;
    else  if(m_nType&(MB_ICONEXCLAMATION))
        m_nIcon  = ICON_EXCLAMATION;
    else if(m_nType&(MB_ICONINFORMATION))
        m_nIcon  = ICON_INFORMATION;
    else 
		m_nIcon = NULL; 
   
	int butnum = 0;
    for(int i=0; i<BUTTONTYPE; i++) 
        if(m_nType & (1<<i)) 
            butnum++;
    
    for(int j=0, i=0; i<butnum; i++,j++) 
    {
        int left = m_sRect.left + (m_sRect.right-m_sRect.left- 
                     BUTTON_WIDTH*butnum-BUTTON_DISTANCE*(butnum-1))/2 +
                     (BUTTON_WIDTH+BUTTON_DISTANCE)*i;

        CRect r(left, m_sRect.bottom-MARGIN-BUTTON_HEIGHT - 6,
                left+BUTTON_WIDTH, m_sRect.bottom-MARGIN - 6);

        while(!(m_nType & (1<<j))) 
            j++;

        char *txt[] = {"确定", "是","否","取消"};
        UINT32 butid[]={IDOK, IDYES, IDNO, IDCANCEL};
		m_obutYesNoOkCancel[i].Create(txt[j], r, this, butid[j]);
    }   
  return TRUE;  
}
////////////////////////////////////////////////////////////////////////////////
//
void CMessageBox::OnPaint(CDC &dc)
{
   dc.SetBackColor(LGRAY); 
   dc.EraseRect(m_sRect);
   dc.Draw3dRect(m_sRect); 
   PaintTitle(dc);
   PaintTextAndIcon(dc);
   PaintChildWnd();
}
////////////////////////////////////////////////////////////////////////////////
//
void CMessageBox::PaintTextAndIcon(CDC &dc)
{
	CRect	r(m_sRect.left + MARGIN, m_sRect.top + MARGIN + WND_TITLE_HEIGHT, m_sRect.right - MARGIN, m_sRect.bottom - MARGIN);
	INT16	strl;
	
	dc.SetBackColor(LGRAY);
	dc.SetPenColor(); 
	if(m_nIcon){
		if (r.right != r.left)
			if (dc.GetTextExtent(m_strText) / (r.right - r.left - gICON_WIDTH(m_nIcon) - 2) == 0)
				r.top += CHAR_HEIGHT/ 2;
		dc.PutIcon(r.left, r.top, m_nIcon);
		r.left += gICON_WIDTH(m_nIcon) + 3;
		r.top += gICON_HEIGHT(m_nIcon) / 2 - CHAR_HEIGHT / 2;
	}
	else
		r.DeflateRect(3, 3);
	
	dc.DrawText(r, m_strText); 
}
////////////////////////////////////////////////////////////////////////////////
//
#undef MARGIN
#undef BUTTON_DISTANCE
#undef BUTTON_WIDTH
#undef BUTTON_HEIGHT

/*============================================================================*/
//  CPinYin类定义                                                             //
/*============================================================================*/
#define SELECT_COUNT  5
class CPinYin : public CCtrl
{
protected:
	char       m_pInput[PINYIN_LEN_MAX+1];
    char       m_pOutput[HANZI_LEN_MAX+1];
	UINT32     m_nInputCount;
	CCtrl    * m_pOverCtrl;
	INT32      m_nSelNumber;
	CRect      m_sSelRect;
	CRect      m_sButRect;
	INT32      m_nOutputCount;
	INT32      m_nCurOut;
	ScreenBuf *m_pBackScreen;
protected:
	void    DoPenDown(CPoint &pt);
	void    DoPenUp();
	void    DoDactive(CCtrl * wParam);
public:
    BOOL    Create(CFrameWnd *pParent, CCtrl * pOverCtrl, UINT32 nCtrlId);
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void Destroy();
	virtual void OnPaint(CDC &dc);
public:
    void    InPut(char ch);
	void    BackSpace();
	void    PopUp();
	void    Hide();
};
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::DoPenDown(CPoint &pt) 
{
   CRect r(m_sRect);
   r.left  = m_sRect.left +2 + (PINYIN_LEN_MAX-1) * ASC_WIDTH;
   r.right -= 16;
   if(m_nSelNumber>=0)
   {
	   CDC::InvertRect(m_sSelRect);
	   m_nSelNumber = -1;
   }
   if(pt.InRect(r))
   {
	   INT32 nSel = (pt.x - r.left)/HZ_WIDTH;
	   if(nSel != m_nSelNumber && nSel < m_nOutputCount - m_nCurOut)
	   {
	    m_nSelNumber = nSel;
	    m_sSelRect.SetRect(r.left + m_nSelNumber * HZ_WIDTH, r.top+2, r.left + (m_nSelNumber+1) * HZ_WIDTH, r.bottom-1);
		CDC::InvertRect(m_sSelRect);
	   }
	   
   }
   else if(pt.InRect(m_sButRect))
   {
	   m_nSelNumber = SELECT_COUNT+1;
	   m_sSelRect = m_sButRect;
	   CDC::InvertRect(m_sSelRect);
   }

}
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::DoPenUp()
{
	if(m_nSelNumber >=0)
	{
		CDC::InvertRect(m_sSelRect);
		if(m_nSelNumber<=SELECT_COUNT)
		{
		 int pos = (m_nCurOut+m_nSelNumber)<<1;
		 UINT32 lParam = MAKEWORD(UINT8(m_pOutput[pos+1]), UINT8(m_pOutput[pos]));
		 Hide();
		 SendNotifyMessage(WM_KEY, lParam); 
		}
		else 
		{
			if(m_nCurOut + SELECT_COUNT < m_nOutputCount)
				m_nCurOut += SELECT_COUNT;
		    else
				m_nCurOut = 0;
			if(m_nOutputCount>SELECT_COUNT)
				OnPaint(CDC());
			m_nSelNumber  = -1;
		}
		
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::DoDactive(CCtrl * wParam)
{
  if(wParam == m_pOverCtrl)
	  Hide();
}
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
   switch(nMessage)
   {
   case WM_PENDOWN:
   case WM_PENMOVE:
	   DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
	   break;
   case WM_PENUP:
	   DoPenUp();
	   break;
   case WM_DACTIVE:
       DoDactive((CCtrl *)wParam);
	   break;
   }
}
////////////////////////////////////////////////////////////////////////////////
//
#define YPOS1  18
#define YPOS2  58
void CPinYin::PopUp()
{
	INT16 cx, cy;
	if(!GetCursorStatus())
		return;
    GetCursorXY(&cx, &cy);
    if(cy>40)
		m_sRect.SetRect(1, YPOS1, 159, YPOS1+20); 
	else
		m_sRect.SetRect(1, YPOS2, 159, YPOS2+20); 
	m_sButRect.SetRect(m_sRect.right-16, m_sRect.top+2, m_sRect.right-2, m_sRect.bottom-2); 
	m_nCurOut = 0;
    m_pBackScreen = CDC::SaveScreen(m_sRect);
	m_dwStyle |= WS_VISIBLE;
	((CFrameWnd *)m_pParent)->m_pCurCtrl = this;  
	((CFrameWnd *)m_pParent)->DeleteChild(this); 
	((CFrameWnd *)m_pParent)->AddChild(this); 
	Paint();
} 
#undef YPOS1 
#undef YPOS2
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::Hide()
{
	if((m_dwStyle & WS_VISIBLE) && m_pBackScreen)
	{
	 CDC::PutScreen(m_pBackScreen); 
     m_dwStyle &= ~WS_VISIBLE;
	 ((CFrameWnd *)m_pParent)->m_pCurCtrl = NULL;
	 m_nInputCount = 0;
	 m_pInput[0]   = 0;
	 m_pOutput[0]  = 0;
	 m_nOutputCount= 0; 
	 m_nSelNumber  = -1;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CPinYin::Create(CFrameWnd *pParent, CCtrl * pOverCtrl, UINT32 nCtrlId)
{
	m_pBackScreen  = NULL;
	m_nInputCount  = 0;
	m_pOverCtrl    = pOverCtrl;
	m_nSelNumber   = -1;
    return CCtrl::Create(NULL, CRect(1,1,1,1), pParent, WS_CHILD, nCtrlId); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::Destroy()
{
	if(m_pBackScreen)
		CDC::PutScreen(m_pBackScreen); 
	CCtrl::Destroy(); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::OnPaint(CDC &dc)
{
	CRect r1(m_sRect.left, m_sRect.top, m_sRect.right-1, m_sRect.bottom-1);
	CRect r(m_sRect.left+1, m_sRect.top+1, m_sRect.right, m_sRect.bottom);
	dc.SetBackColor(LGRAY); 
	dc.EraseRect(m_sRect);
	dc.Draw3dRect(r1); 
	dc.Draw3dRect(r, GRAY, WHITE);
	INT16 x = m_sRect.left+2, y=m_sRect.top+2;
	dc.TextOut(x, y, m_pInput);
	x += (PINYIN_LEN_MAX-2) * ASC_WIDTH;
	dc.TextOut(x, y, ":");  
	x += ASC_WIDTH;
	dc.TextOut(x, y, m_pOutput+(m_nCurOut<<1), (SELECT_COUNT<<1)); 
	dc.PutIcon(m_sButRect.left, m_sButRect.top, ICON_RIGHTARROW);
	dc.Draw3dRect(m_sButRect); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::InPut(char ch)
{
	((CFrameWnd *)m_pParent)->m_pCurCtrl = this;  
	if(m_nInputCount >= PINYIN_LEN_MAX)
		return;
	m_pInput[m_nInputCount++]=ch;
	m_pInput[m_nInputCount]  =0;
	int outcount = FindPinYin(m_pInput, m_pOutput, HANZI_LEN_MAX+1);
    if(outcount <0)
	{
      m_pInput[--m_nInputCount]  =0;
	  return;
	}
	m_nOutputCount = outcount;
	m_nCurOut = 0;
	if(m_nOutputCount ==0)
	  m_pOutput[0]=0;

	if(m_nInputCount ==1)
    	PopUp(); 
	else
        Paint(); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CPinYin::BackSpace()
{
    if(m_nInputCount>0)
	{
	 m_pInput[--m_nInputCount]  =0;
     m_nOutputCount = FindPinYin(m_pInput, m_pOutput, HANZI_LEN_MAX+1);
     if(m_nOutputCount ==0)
	   m_pOutput[0]=0;
     m_nCurOut = 0;
	 Paint();
	 if(m_nInputCount ==0)
		 Hide();
	}
}
/*============================================================================*/
//  给Rect数组赋值的一个全局函数                                              //
/*============================================================================*/
static void SetRectValue(CRect *pRectArray, const INT16  pValueArray[], INT16 nCount)
{
	for(int i=0, k=0; i < nCount; i++)
	{
		pRectArray->left   = pValueArray[k++];
		pRectArray->top    = pValueArray[k++];
		pRectArray->right  = pValueArray[k++];
		pRectArray->bottom = pValueArray[k++];
		pRectArray++;
	} 
}

/*============================================================================*/
//  CInputbox类定义                                                           //
/*============================================================================*/
class CNumInputBox : public CDialog
{
protected:
	    CLineEdit    m_oNumberDisp;
		CIconButton  m_oButOk;
		CSoftKey     m_oKeyPad;
		char      *  m_pUserText; 
		INT32        m_nBufLen;
public:
		virtual BOOL OnCreate();
		void         SetParmaters(char *strText, INT32 nBufLen);
		virtual void OnOk();
public:
		DECLARE_MESSAGE_MAP(CNumInputBox)
        MESSAGE_HANDLE void OnKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};
////////////////////////////////////////////////////////////////////////////////
//
void CNumInputBox::OnOk()
{
	m_oNumberDisp.GetInputText(m_pUserText, m_nBufLen); 
	CDialog::OnOk(); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CNumInputBox::OnKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	switch(char(lParam))
	{
	case '\b':
       m_oNumberDisp.BackSpace();
	   break;
	case '\n':
       m_oNumberDisp.Clear();
	   break;
	default:
	   m_oNumberDisp.AppendChar(char(lParam)); 
	   break;
	}
}


////////////////////////////////////////////////////////////////////////////////
//
void CNumInputBox::SetParmaters(char *strText, INT32 nBufLen)
{
	m_pUserText = strText;
	m_nBufLen   = nBufLen;
}

////////////////////////////////////////////////////////////////////////////////
//
#define ID_NUMKEYPAD     10002
#define NUM_KEYCOUNT     18

BEGIN_MESSAGE_MAP(CNumInputBox, CDialog)
  ON_MESSAGE(ID_NUMKEYPAD, WM_KEY, OnKey)
END_MESSAGE_MAP 

const INT16 arRectValue[NUM_KEYCOUNT*4] ={ 5, 49,  29, 71,  36, 49, 60,  71,  67, 49, 91, 71, 
	                                    98, 49, 153, 71,   5, 76, 29,  98,  36, 76, 60, 98,  
	                                    67, 76, 91,  98,  98, 76,153,  98,   5,103, 29,125,  
										36,103, 60,  125, 67,103, 91, 125,  98,103,122,125, 
	                                   129,103,153,  125, 5, 130, 29, 152,  36,130, 60,152, 
	                                    67,130, 91,  152,98, 130,122, 152, 129,130,153,152};

////////////////////////////////////////////////////////////////////////////////
//
BOOL CNumInputBox::OnCreate()
{
	if(!CDialog::OnCreate())
		return FALSE;

	m_oNumberDisp.Create(m_pUserText, m_nBufLen, CRect(5,26,153,43), this); 

    CRect arRect[NUM_KEYCOUNT];
	SetRectValue(arRect, arRectValue, NUM_KEYCOUNT);

    UINT16 arValue[NUM_KEYCOUNT] = {'1', '2', '3', '\b', '4', '5', '6', '\n', '7','8','9',',','-','*','0','#','(',')'};
	if(!m_oKeyPad.Create(ICON_NUMKEY, NUM_KEYCOUNT, arRect, arValue, this, ID_NUMKEYPAD))
		return FALSE;

	CRect ctrRect = m_butCancel.GetWindowRect();
	ctrRect.right -= (WND_TITLE_HEIGHT+1);
	ctrRect.left  -= (WND_TITLE_HEIGHT+1);
	if(!m_oButOk.Create(NULL, ICON_OK, ctrRect, this, IDOK))
		return FALSE;
	return TRUE;
}
#undef ID_NUMKEYPAD
#undef NUM_KEYCOUNT 

    
/*============================================================================*/
//  CInputbox类定义                                                           //
/*============================================================================*/
#define ID_WRITENUMBER      0xFFE00
#define ID_WRITEKEY         0xFFE01
#define ID_SIMPLEKEY        0xFFE02
#define ID_CTRLKEY          0xFFE03
#define ID_SYMBOLKEY        0xFFE04
#define ID_SWITCHTOWRITE    0xFFE05
#define ID_SWITCHTOPY       0xFFE06
#define ID_SWITCHTOENG      0xFFE07
#define ID_SWITCHTOSYMBOL   0xFFE08
#define ID_PINYIN           0xFFE09

#define    COPY_BUF_LEN     128
char       g_pCopyBuf[COPY_BUF_LEN];   // hqf
class CInputBox: public CDialog
{
protected:
	 CEdit        m_oEdit;
	 CWritePad    m_oWrite;
	 CMenu        m_oStatusSelect;

	 CSoftKey     m_oWriteKey;
	 CSoftKey     m_oSimpleKey;
	 CSoftKey     m_oSymbolKey;
	 CSoftKey     m_oCtrlKey;
	 CSoftKey     m_oWriteNumberKey;
	 CPinYin      m_oPinYin;
	 UINT32       m_iStatus;

	 CIconButton  m_oButOk;
	 char       * m_pTextBuf;
	 INT32        m_nTextBufLen;

	 BOOL         m_bShift;
	 UINT8        m_nInitStatus;
	 BOOL         m_bCanEnter;
protected:
	 BOOL CreateSoftKey();
	 BOOL CreateSimpleKey();
	 BOOL CreateSymbolKey();
	 BOOL SwitchTo(UINT32 nSrcId);
public:
     virtual BOOL OnCreate();
	 virtual void OnActive();
	 virtual void OnDactive();
     void    SetParmaters(char *strText, INT32 nBufLen, UINT8 nInitStatus, BOOL bCanEnter = FALSE);
	 virtual void OnOk(); 
	 virtual BOOL OnDestroy();
	 virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
public:
	 DECLARE_MESSAGE_MAP(CInputBox)
     MESSAGE_HANDLE void OnWriteKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	 MESSAGE_HANDLE void OnSwitch(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
     MESSAGE_HANDLE void OnCtrlKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	 MESSAGE_HANDLE void OnSimpleKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	 MESSAGE_HANDLE void OnSymbolKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
};

////////////////////////////////////////////////////////////////////////////////
//
void CInputBox::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	switch(nMessage)
	{
	case  WM_WRITEREG_OVER:
//		CDialog::OnDactive();
		if(m_pCurCtrl && m_pCurCtrl != (CCtrl *)&m_oWrite)
			m_pCurCtrl->SendMessage(WM_DACTIVE , 0,0);
		break;
	case WM_DACTIVE:
		if(m_iStatus == ID_SWITCHTOWRITE)
			m_oWrite.OnPaint(CDC());
		OnDactive();
		break;
	default:
		CDialog::WindowProcess(nMessage, wParam, lParam);
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CInputBox::OnSymbolKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	if(lParam == '\n' && !m_bCanEnter)
		return;
	m_oEdit.SendMessage(WM_KEY, lParam, 0);
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CInputBox::OnDestroy()
{
//	if(m_pCopyBuf)
  //  	delete []m_pCopyBuf;
	OnDactive();
	return CDialog::OnDestroy(); 
}

void CInputBox::OnActive()
{
	::SetTouchPaneSampleRate(70);
	m_oEdit.DrawCursor();
	CDialog::OnActive();
}

void CInputBox::OnDactive()
{
	::SetTouchPaneSampleRate();
	::CloseCursor();
	CDialog::OnDactive();
}

////////////////////////////////////////////////////////////////////////////////
//
UINT16 aSimpleValueHig[]={'~', '!','`', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
                          'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\\',
						  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '\n',
						  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', KEY_SHIFT
};
UINT16 aSimpleValueLow[]={'@', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
                          'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '|',
                  		  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\n',
						  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_SHIFT
} ;
void CInputBox::OnSimpleKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	switch(UINT16(lParam))
	{
	case KEY_SHIFT:
        if(m_bShift)
          m_oSimpleKey.SetParameters(ICON_LOWKEY, aSimpleValueLow, TRUE); 
		else
		  m_oSimpleKey.SetParameters(ICON_UPKEY, aSimpleValueHig, TRUE); 
		m_bShift = !m_bShift;
		break;
	default:
		if(m_iStatus == ID_SWITCHTOENG)
		{
			if(lParam == '\n' && !m_bCanEnter)
				return;
			else
				m_oEdit.SendMessage(WM_KEY, lParam, 0);
		}			
		else if(char(lParam) >= 'a' && char(lParam) <= 'z')
		     m_oPinYin.InPut(char(lParam)); 
		else 
		{
			m_oPinYin.Hide();
			m_oEdit.SendMessage(WM_KEY, lParam, 0);
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CInputBox::OnCtrlKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	switch(UINT16(lParam))
	{
	case '\n':
		if(m_bCanEnter)
			m_oEdit.SendMessage(WM_KEY, lParam, 0);
		break;
	case '\b':
		if(m_oPinYin.GetWindowStyle() & WS_VISIBLE)
			m_oPinYin.BackSpace(); 
		else
            m_oEdit.SendMessage(WM_KEY, lParam, 0);
		break;
	case ' ':
		if(!(m_oPinYin.GetWindowStyle() & WS_VISIBLE))
		    m_oEdit.SendMessage(WM_KEY, lParam, 0);
		break;
    case KEY_COPY:
		if(!(m_oPinYin.GetWindowStyle() & WS_VISIBLE))
		   m_oEdit.Copy(g_pCopyBuf , COPY_BUF_LEN);
		 break;
	case KEY_PASTE:
		if(!(m_oPinYin.GetWindowStyle() & WS_VISIBLE))
		   m_oEdit.Paste(g_pCopyBuf); 
		break;
	case KEY_STATUS:
		m_oPinYin.Hide(); 
        m_oStatusSelect.PopupMenu(1, 72); 
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CInputBox::OnWriteKey(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
  switch(UINT16(lParam))
  {
   case '\n':
	    if(m_bCanEnter)
			m_oEdit.SendMessage(WM_KEY, lParam, 0);  
		break;
   case '\b':case ' ':
 	    m_oEdit.SendMessage(WM_KEY, lParam, 0);   
        break;
   case KEY_COPY:
	   m_oEdit.Copy(g_pCopyBuf, COPY_BUF_LEN);
	   break;
   case KEY_PASTE:
	   m_oEdit.Paste(g_pCopyBuf);
	   break;
   case KEY_STATUS:
	   m_oPinYin.Hide(); 
	   m_oStatusSelect.PopupMenu(1, 72); 
	   break;
  }
}
////////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CInputBox, CDialog)
  //{{ Declare CInputBox Message Map
  ON_MESSAGE(ID_WRITEKEY,    WM_KEY, OnWriteKey)
  ON_MESSAGE(ID_CTRLKEY,     WM_KEY, OnCtrlKey)
  ON_MESSAGE(ID_SIMPLEKEY,   WM_KEY, OnSimpleKey)
  ON_MESSAGE(ID_SYMBOLKEY,   WM_KEY, OnSymbolKey)
  ON_MESSAGE(ID_PINYIN,      WM_KEY, OnSymbolKey)
  ON_MESSAGE(ID_WRITENUMBER, WM_KEY, OnSymbolKey)
  ON_MESSAGE_RANGE(ID_SWITCHTOWRITE, ID_SWITCHTOSYMBOL, CN_COMMAND, OnSwitch)
  //}}
END_MESSAGE_MAP 
////////////////////////////////////////////////////////////////////////////////
// 
BOOL CInputBox::SwitchTo(UINT32 nSrcId)
{

	if(m_iStatus == nSrcId)
		return FALSE;
	m_oStatusSelect.CheckItem(m_iStatus, FALSE);
	m_iStatus = nSrcId;
	m_oStatusSelect.CheckItem(m_iStatus, TRUE);
	if(m_iStatus == ID_SWITCHTOENG || m_iStatus == ID_SWITCHTOPY)
	{
	  m_oWriteNumberKey.Visible(FALSE);
	  m_oWrite.Visible(FALSE);
	  m_oWriteKey.Visible(FALSE);
	  m_oSymbolKey.Visible(FALSE);
	  m_oSimpleKey.Visible(TRUE);
	  m_oCtrlKey.Visible(TRUE);
	  if(m_iStatus == ID_SWITCHTOENG)
		  m_oCtrlKey.SetParameters(ICON_CTRLKEY);
	  else
		  m_oCtrlKey.SetParameters(ICON_PYCTRLKEY);
	}
	else if(m_iStatus == ID_SWITCHTOWRITE)
	{
   		m_oSimpleKey.Visible(FALSE);
	    m_oCtrlKey.Visible(FALSE);
		m_oSymbolKey.Visible(FALSE);
		m_oWrite.Visible(TRUE);
		m_oWriteKey.Visible(TRUE);
		m_oWriteNumberKey.Visible(TRUE);
	}
	else if(m_iStatus == ID_SWITCHTOSYMBOL)
	{
		m_oWriteNumberKey.Visible(FALSE);
     	m_oSimpleKey.Visible(FALSE);
	    m_oCtrlKey.Visible(TRUE);
		m_oSymbolKey.Visible(TRUE);
		m_oWrite.Visible(FALSE);
		m_oWriteKey.Visible(FALSE);

      m_oCtrlKey.SetParameters(ICON_SYCTRLKEY);
	}
	return TRUE;
}
void CInputBox::OnSwitch(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	if(SwitchTo(nSrcId))
	   OnPaint(CDC());
}

////////////////////////////////////////////////////////////////////////////////
//
void CInputBox::OnOk()
{
    m_oEdit.GetWindowText(m_pTextBuf, m_nTextBufLen);
	CDialog::OnOk(); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CInputBox::SetParmaters(char *strText, INT32 nBufLen, UINT8 nInitStatus, BOOL bCanEnter)
{
	m_pTextBuf = strText;
	m_nTextBufLen = nBufLen;
	m_nInitStatus = nInitStatus;
	m_bCanEnter = bCanEnter;
}

////////////////////////////////////////////////////////////////////////////////
//
#define LEFT   0
#define TOP    66 
#define WIDTH  11
#define HEIGHT 18 
BOOL CInputBox::CreateSimpleKey()
{
	CRect arRect[52];
	CRect r(LEFT, TOP, LEFT+WIDTH-1, TOP+HEIGHT-1);
    for(int k=0, i=0; i<4; i++)
	{
		for(int j=0; j<13; j++) 
		{
		  arRect[k++] = r;
		  r.left  = r.right + 2;
		  r.right = r.left + WIDTH - 1;
		  if(i==2 && j==10)
		  {
			  r.right += WIDTH-1;
			  j = 11;
		  }
		  else if( i==3 && j==9)
		  {
			  r.right += WIDTH + WIDTH-1;
			  j = 11;
		  }
		  if(j==12)
             arRect[k-1].right = m_sRect.right;
		}
		r.left   = LEFT;
		r.right  = LEFT+WIDTH-1;
		r.top    = r.bottom +2;
		r.bottom = r.top + HEIGHT-1;
	}
    if(!m_oSimpleKey.Create(ICON_LOWKEY, 49, arRect, aSimpleValueLow, this, ID_SIMPLEKEY))
		  return FALSE;
	m_oSimpleKey.Visible(FALSE); 
	
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CInputBox::CreateSymbolKey()
{
	char   * text = "，。、；：？！‘’“”〖〗《》…≈∽≡≠＝≤≥＜＞√＋－×÷°℃￥￡№§※★☆◆◇■□↑↓←→αβγ\0\n";     //符号
	CRect    arRect[52];
	UINT16   aSymbolKeyValue[52];
    CRect    sRec(LEFT, TOP, LEFT+WIDTH-1, TOP+HEIGHT-1);
	memcpy(aSymbolKeyValue, text, 52*2);
	for(int k=0, i=0; i<4; i++)
	{
		for(int j=0; j<13; j++)
		{
		  if(k == 50)
		  {
			  sRec.right  = m_sRect.right;
			  arRect[k++] = sRec;
			  break;
		  }
		  arRect[k++] = sRec;
		  sRec.left   = sRec.right +2;
		  sRec.right  = sRec.left + WIDTH-1;
		  if(j == 12)
			  arRect[k-1].right = m_sRect.right;
		}
	  sRec.left   = LEFT;
	  sRec.right  = LEFT+WIDTH;
	  sRec.top    = sRec.bottom +2;
	  sRec.bottom = sRec.top + HEIGHT-1;
	}
    if(!m_oSymbolKey.Create(ICON_SYMBOKEY, 51, arRect, aSymbolKeyValue, this, ID_SYMBOLKEY))
		return FALSE;
	m_oSymbolKey.Visible(FALSE); 

	return TRUE;
}
#undef  TOP
#undef  WIDTH
#undef  HEIGHT

////////////////////////////////////////////////////////////////////////////////
//
BOOL CInputBox::CreateSoftKey() 
{ 
	
	UINT16  aCtrlKeyValue[] = {KEY_STATUS, KEY_COPY, KEY_PASTE,' ', '\b'};
	UINT16  aWriteKeyValue[]={'\b', ' ', '\n', KEY_COPY, KEY_PASTE,KEY_STATUS};
	INT16   aCtrlKeyRect[]  ={0,142,31,159,33,142,62,159,64,142,94,159,96,142,126,159,128,142,159,159};
	INT16   aWriteKeyRect[]={0,79,32,99,0,101,15,119,17,101,32,119,0,121,15,139,17,121,32,139,0,140,32,159};
	CRect   arRect[6]; 
    UINT16  aWriteNumberValue[16]={'1','2','3','4','5','6','7','8','9','0','，','。','、',':','!','?'};
	memcpy(&aWriteNumberValue[10], "，", 2);
	memcpy(&aWriteNumberValue[11], "。", 2);
	memcpy(&aWriteNumberValue[12], "、", 2);
	CRect   arWriteRect[16];
	for(int i=0, left=0; i<12; i++,left+=13)
	{
		arWriteRect[i].top = 66;
		arWriteRect[i].bottom = 79;
		arWriteRect[i].left =left;
		arWriteRect[i].right = left+12;
	}
	arWriteRect[10].right += 2;
	arWriteRect[11].left = arWriteRect[10].right+1;
	arWriteRect[11].right = 159;
	if(!m_oWriteNumberKey.Create(ICON_WRITENUMBER, 12, arWriteRect, aWriteNumberValue, this, ID_WRITENUMBER))
		return FALSE;
  	SetRectValue(arRect, aWriteKeyRect, 6);
	if(!m_oWriteKey.Create(ICON_WRITEKEY, 6, arRect, aWriteKeyValue, this, ID_WRITEKEY))
		return FALSE;

    SetRectValue(arRect, aCtrlKeyRect, 5);
    if(!m_oCtrlKey.Create(ICON_CTRLKEY, 5, arRect, aCtrlKeyValue, this, ID_CTRLKEY))
		return FALSE; 
    m_oCtrlKey.SetWindowStyle(m_oCtrlKey.GetWindowStyle()&~WS_VISIBLE); 

	if(!CreateSimpleKey())
		return FALSE;

	if(!CreateSymbolKey())
		return FALSE;
     return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CInputBox::OnCreate()
{
	if(!CDialog::OnCreate())
		return FALSE; 
	if(!m_oEdit.Create(m_pTextBuf, CRect(0, WND_TITLE_HEIGHT, 159, 66), this, WS_CHILD|WS_VISIBLE, 0, m_nTextBufLen, NULL , &m_oWrite))
		return FALSE;

	if(!m_oWrite.Create(NULL, CRect(32, 79, 159, 159), this, WS_CHILD|WS_VISIBLE, 0))
		return FALSE; 
	m_oWrite.CennectCtrl(&m_oEdit); 

	m_iStatus = ID_SWITCHTOWRITE;
	if(!CreateSoftKey())
		return FALSE;

	m_oStatusSelect.Create(this);
	m_oStatusSelect.AppendItem("手写", ID_SWITCHTOWRITE, ICON_CHECK);
	m_oStatusSelect.AppendItem("拼音", ID_SWITCHTOPY,ICON_UNCHECK);
	m_oStatusSelect.AppendItem("英文", ID_SWITCHTOENG,ICON_UNCHECK);
	m_oStatusSelect.AppendItem("符号", ID_SWITCHTOSYMBOL,ICON_UNCHECK);

	if(!m_oPinYin.Create(this, &m_oEdit, ID_PINYIN))
		return FALSE; 

	CRect ctrRect = m_butCancel.GetWindowRect();
	ctrRect.right -= (WND_TITLE_HEIGHT+1);
	ctrRect.left  -= (WND_TITLE_HEIGHT+1);
	if(!m_oButOk.Create(NULL, ICON_OK, ctrRect, this, IDOK))
		return FALSE;

//	m_pCopyBuf  = new char[128];
//	*m_pCopyBuf = 0;
	m_bShift    = FALSE;

	switch(m_nInitStatus)
	{
	case INIT_WRITE:
		SwitchTo(ID_SWITCHTOWRITE);
		break;
	case INIT_PY:
		SwitchTo(ID_SWITCHTOPY);
		break;
	case INIT_ENG:
		SwitchTo(ID_SWITCHTOENG);
		break;
	case INIT_SYMBO:
		SwitchTo(ID_SWITCHTOSYMBOL);
		break;
	}
    return TRUE;
}
#undef ID_WRITENUMBER
#undef ID_WRITEKEY
#undef ID_SIMPLEKEY
#undef ID_CTRLKEY
#undef ID_SYMBOLKEY
#undef ID_SWITCHTOWRITE
#undef ID_SWITCHTOPY
#undef ID_SWITCHTOENG
#undef ID_SWITCHTOSYMBOL
#undef ID_PINYIN
/*============================================================================*/
//  三个接口函数定义                                                          //
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//
UINT32 MessageBox(char * strText, char * strCaption, UINT32 uType)
{
   CMessageBox  msgbox;
   msgbox.Create(strText, strCaption, uType);
   return  msgbox.DoModal(); 
}

////////////////////////////////////////////////////////////////////////////////
//
UINT32 InputBox(char * strCaption, char * strInputBuf, INT32 nBufLen, UINT8 nInitStatus , BOOL bCanEnter)
{
	CInputBox iptbox;
	iptbox.SetParmaters(strInputBuf, nBufLen, nInitStatus , bCanEnter);
	iptbox.Create(strCaption, WS_VISIBLE|WS_CAPTION, DESK_TOP_RECT, TRUE, HELP_INPUT);
	return iptbox.DoModal();
}

////////////////////////////////////////////////////////////////////////////////
//
UINT32 NumInputBox(char * strCaption, char * strInputBuf, INT32 nBufLen)
{
	CNumInputBox iptbox;
	iptbox.SetParmaters(strInputBuf, nBufLen);
	iptbox.Create(strCaption, WS_VISIBLE|WS_CAPTION, DESK_TOP_RECT, TRUE, HELP_INPUT_TELNUM); 
	return iptbox.DoModal();
}
/*============================================================================*/
//  拼音输入法函数定义                                                        //
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
		hzp = (char *)(*((long *)(PINYIN_ADDRESS + k * 4)) + PINYIN_ADDRESS + PINYIN_DATA_OFFSET);
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
		hzp = (char *)(*((long *)(PINYIN_ADDRESS + i * 4)) + PINYIN_ADDRESS + PINYIN_DATA_OFFSET);
		if (strstr(hzp, lwrpinyin))
			return 0;
		// 输入拼音在有效拼音字符串范围之外，返回 -1
		else
			return -1;
	}
}

//取一个汉字拼音缩写
char GetHanziPinyin(const char * Hanzi)
{
	int Hhanzi ,Lhanzi;
	char cPinyin,*PinyinList;
	PinyinList=(char*)PINYINABBR_ADDR;
	Hhanzi=(BYTE)Hanzi[0];
	Lhanzi=(BYTE)Hanzi[1];
	cPinyin=PinyinList[(Hhanzi-0xb0)*94+(Lhanzi-0xa1)];
	return cPinyin;
}
//取汉字串拼音缩写
int GetAbbreviation(const char * hanzi, char * pinyin)
{
	int StrLength,PinyinLen=0;

	StrLength=strlen(hanzi);
	for(int i=0;i<StrLength;)
	{
		if(hanzi[i] >= 0xb0 && hanzi[i] <= 0xf7)
		{	
			pinyin[PinyinLen]=GetHanziPinyin(&(hanzi[i]));
			i+=2;
			PinyinLen++;
		}
		else if (hanzi[i] & 0x80)
			i += 2;
		else
			i++;
	}
	pinyin[PinyinLen]=0x00;
	return PinyinLen;
}

#include "combox.h"
////////////////////////////////////////////////////////////////////////////////
//
#define ID_COMYEARSEL  110
#define ID_COMMONTHSEL 111
#define ID_CALDAYSEL   112

class CDateInputBox:public CDialog
{
private:
	CCalendar m_oCalendar;
	CCombox   m_oYearSelect, m_oMonthSelect;
	CIconButton m_obutOk;
public:
	virtual BOOL OnCreate();
	void    SetParmaters(UINT16 nYear, UINT16 nMonth, UINT16 nDay);
	void    GetDate(INT16 &nYear, INT16 &nMonth, INT16 &nDay);
	DECLARE_MESSAGE_MAP(CDateInputBox)
	MESSAGE_HANDLE void OnYearSel();
	MESSAGE_HANDLE void OnMonthSel();
};
////////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDateInputBox, CDialog)
//{{
  ON_MESSAGE(ID_COMYEARSEL, CN_COMMAND, OnYearSel)
  ON_MESSAGE(ID_COMMONTHSEL, CN_COMMAND, OnMonthSel)
//}}
END_MESSAGE_MAP
/*============================================================================*/
//  CDateInputBox类成员函数定义                                               //
/*============================================================================*/
void CDateInputBox::SetParmaters(UINT16 nYear, UINT16 nMonth, UINT16 nDay)
{
	if(TESTDAY(nYear, nMonth, nDay))
	{
	 m_oCalendar.SetDate(nYear, nMonth, nDay);
	 m_oYearSelect.SetCurSel(m_oCalendar.GetYear() - START_YEAR);  
	 m_oMonthSelect.SetCurSel(m_oCalendar.GetMonth() - 1);  
	}
}
 
///////////////////////////////////////////////////////////////////////////////
//
void CDateInputBox::GetDate(INT16 &nYear, INT16 &nMonth, INT16 &nDay)
{
	m_oCalendar.GetDate(nYear, nMonth, nDay); 
}

///////////////////////////////////////////////////////////////////////////////
//
void CDateInputBox::OnYearSel()
{
	INT16 nYear = m_oYearSelect.GetCurSel()+START_YEAR;
	INT16 nOldYear, nMonth, nDay;
	m_oCalendar.GetDate(nOldYear, nMonth, nDay);
	if(nYear != nOldYear)
	{
		if(nDay > CDate::MonthDays(nYear, nMonth))
			 nDay = CDate::MonthDays(nYear, nMonth); 
		m_oCalendar.SetDate(nYear, nMonth, nDay);
		m_oCalendar.OnPaint(CDC());
	}
}

///////////////////////////////////////////////////////////////////////////////
//
void CDateInputBox::OnMonthSel()
{
	INT16 nMonth = m_oMonthSelect.GetCurSel()+1;
	INT16 nYear, nOldMonth, nDay;
	m_oCalendar.GetDate(nYear, nOldMonth, nDay);
	if(nMonth != nOldMonth)
	{
		if(nDay > CDate::MonthDays(nYear, nMonth))
			 nDay = CDate::MonthDays(nYear, nMonth); 
		m_oCalendar.SetDate(nYear, nMonth, nDay);
		m_oCalendar.OnPaint(CDC());
	}
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL CDateInputBox::OnCreate()
{
	if(!m_oCalendar.Create(CPoint(1,40), this, ID_CALDAYSEL))
		return FALSE;

	CRect ctrRect(m_sRect.left+1, m_sRect.top+WND_TITLE_HEIGHT, m_sRect.left+79, m_sRect.top+WND_TITLE_HEIGHT+20); 
	if(!m_oYearSelect.Create(NULL, ctrRect, 5*19, this, ID_COMYEARSEL)) 
		return FALSE;

	ctrRect.left = ctrRect.right +1;
	ctrRect.right = m_sRect.right-1;
	if(!m_oMonthSelect.Create(NULL, ctrRect, 5*19, this, ID_COMMONTHSEL))
		return FALSE;

	if(!CDialog::OnCreate())
		return FALSE;
	ctrRect = m_butCancel.GetWindowRect();
	ctrRect.right -= (WND_TITLE_HEIGHT+1);
	ctrRect.left  -= (WND_TITLE_HEIGHT+1);
    if(!m_obutOk.Create(NULL, ICON_OK, ctrRect, this, IDOK))
		return FALSE; 

	char text[50];
    for(UINT16 i = START_YEAR; i<=END_YEAR; i++)
	{ 
        sprintf(text, "%4d年", i);
		m_oYearSelect.AppendItem(text, 0); 
	}
	m_oYearSelect.SetCurSel(m_oCalendar.GetYear() - START_YEAR);  
	char *szText[]={"一月","二月","三月","四月","五月",
                   "六月","七月","八月","九月","十月","十一月","十二月"};
	for(UINT16 i=0; i<12; i++)
		m_oMonthSelect.AppendItem(szText[i], 0); 
	m_oMonthSelect.SetCurSel(m_oCalendar.GetMonth() - 1);  
	return TRUE;
}
#undef ID_COMYEARSEL  
#undef ID_COMMONTHSEL
#undef ID_CALDAYSEL  
////////////////////////////////////////////////////////////////////////////////
//描述
//    显示一个日期输入窗口
//参数
//    strCaption   输入框标题
//    nYear
//    nMonth
//    nDay         入口时初始化输入年月日，出口时返回用户选择的年月日
//返回
//    IDOK         用户按下"确定"按钮
//    IDCANCEL     用户按下"取消"按钮
//
UINT32 DateInputBox(char * strCaption, INT16 &nYear, INT16 &nMonth, INT16 &nDay, UINT32 nHelpID)
{
	CDateInputBox iptbox;
	iptbox.Create(strCaption, WS_VISIBLE|WS_CAPTION, DESK_TOP_RECT, TRUE , HELP_SETUP_DATE); 
	iptbox.SetParmaters(nYear, nMonth, nDay);
	UINT32 rcode = iptbox.DoModal();
	if(rcode == IDOK)
		iptbox.GetDate(nYear, nMonth, nDay);
	return rcode;
}
////////////////////////////////////////////////////////////////////////////////
//
/*#define ID_BUTUPHOUR   1002
#define ID_BUTDOWNHOUR 1003
#define ID_BUTUPMIN    1004
#define ID_BUTDOWNMIN    1005*/
#define IDC_NUMKEY       1006
#define IDC_BTNHOUR      1007
#define IDC_RADTIMETYPE  1008
#define IDC_BTNTIMEFLAG  1009
#define IDC_BTNMINU      1010
#define IDC_BTNDATE      1011

BOOL CIcon::Create(UINT32 nIconID , CRect &ctrlRect , CFrameWnd *pParent , UINT32 nCtrlID )
{
	return CIconButton::Create(NULL , nIconID , ctrlRect , pParent ,nCtrlID);
}
		

class CHourMinuBtn : public CButton
{
public:
	CHourMinuBtn();
	~CHourMinuBtn();
	virtual void OnPaint(CDC &dc);
	void SetSel(BOOL bSel = TRUE);
	void SetWindowText(const char *strText, BOOL bShowCursor = FALSE, BOOL bRedraw = FALSE)
	{
		hwstrncpy(m_strName, strText, WND_TITLE_LENGTH - 1);
		if(bRedraw)
		{
			OnPaint(CDC());
			if(bShowCursor)
			{
				//			if(!::GetCursorStatus())
				::OpenCursor();
				::SetCursorAttr(16, 2);
				::SetCursorXY(m_sRect.right, m_sRect.top+18);
			}
			else
				::CloseCursor();
		}
	}
protected:
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	BOOL m_bSelected;
};

class CTimeInputBox : public CDialog
{
//protected:
private:
	CClock      m_oClock;
	CIconButton m_obutOk;
	CIcon       m_oBtnType;
	CSoftKey    m_oNumKey;
	CHourMinuBtn  m_oBtnHour, m_oBtnMinu;
	CHourMinuBtn  *m_pCurInput;
	CRadioButton m_oRadTimeType;
//	CButton     m_oBtnDate;
	
	char        m_strHour[5], m_strMinu[5];
	char        m_cHourPos, m_cMinuPos;
	short       m_sTimeType; // 0-- 12 hours or 1--24 hours
	short       m_sTimeFlag; //0 = AM or 1 = PM
	INT16       m_iSec; //保存的秒值
//	CUpDownButton m_obutHourUp, m_obutHourDown;
//    CUpDownButton m_obutMinUp,  m_obutMinDown;
private:
	void PaintDigClock(CDC &dc);//函数已不用
	void PaintOther(CDC &dc);
	void DoHourMsg(short ch);
	void DoMinuMsg(short ch);
	void UpdateClock();
public:
//	virtual BOOL C
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual void OnPaint(CDC &dc);
//	virtual void OnOk();
public:
	void SetParmaters(INT16 nHour, INT16 nMinute);
	void GetTime(INT16 &nHour, INT16&nMinute);//所得时间类型均为24小时制
	void GetTimeType(INT16 &nTimeType){nTimeType =m_sTimeType;}
	DECLARE_MESSAGE_MAP(CClockApp)
/*	MESSAGE_HANDLE void OnHourUp();
	MESSAGE_HANDLE void OnHourDown();
	MESSAGE_HANDLE void OnMinUp();
	MESSAGE_HANDLE void OnMinDown();*/
	MESSAGE_HANDLE void OnNumKey(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnAMPM(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnTimeType(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	MESSAGE_HANDLE void OnInputSwitch(UINT32 nMessage, UINT32 wParam, UINT32 lParam);	
//	MESSAGE_HANDLE void OnDateInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};

void CHourMinuBtn::CHourMinuBtn()
{
	m_bSelected = FALSE;
}

void CHourMinuBtn::~CHourMinuBtn()
{
}

void DrawNumString(CRect rectArea, char *strNum , UINT8 uAlign = HALIGN_LEFT) 
{
	UINT32  index[] = {ICON_TIME_0 , ICON_TIME_1 , ICON_TIME_2 , ICON_TIME_3 , 
							ICON_TIME_4 , ICON_TIME_5 , ICON_TIME_6 , ICON_TIME_7 , 
							ICON_TIME_8 , ICON_TIME_9 , ICON_COLON};
	INT16   nWidth = gICON_WIDTH(ICON_TIME_0);
	CDC dc;
	INT16 x = rectArea.left , y = rectArea.top ;
	dc.EraseRect(rectArea);
	if(uAlign == HALIGN_LEFT)
	{
		  while(*strNum && x < rectArea.right )
		  {
			 if(*strNum >= '0' && *strNum <= '9')
				  dc.PutIcon(x, y, index[((*strNum) - '0')]); 
			 else if(*strNum == ':')
				  dc.PutIcon(x, y, index[10]);
			 x += nWidth;
			 strNum ++;
		  }
	}
	else
	{
		INT16 iLen = strlen(strNum);
		if(nWidth * iLen > rectArea.Width())
			DrawNumString(rectArea , strNum , HALIGN_LEFT);
		else
		{
			rectArea.left = rectArea.right - nWidth * iLen;
			DrawNumString(rectArea , strNum , HALIGN_LEFT);
		}
	}
}
		
void CHourMinuBtn::OnPaint(CDC &dc)
{
	DrawNumString(m_sRect,m_strName , HALIGN_RIGHT);
//	if(m_bSelected)
//		dc.InvertRect(m_sRect);
}
void CHourMinuBtn::SetSel(BOOL bSel)
{
	CDC dc;
	if(bSel)
	{
		if(!m_bSelected)
		{
			dc.InvertRect(m_sRect);
			m_bSelected = TRUE;
		}
	}
	else
	{
		if(m_bSelected)
		{
			dc.InvertRect(m_sRect);
			m_bSelected = FALSE;
		}
	}
}

void CHourMinuBtn::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	CDC dc;
	switch(nMessage)
	{
	case WM_PENDOWN : case WM_PENMOVEIN:
		{
//		if(!::GetCursorStatus())
		::OpenCursor();
		::SetCursorAttr(16, 2 );
		::SetCursorXY(m_sRect.right , m_sRect.top+18 );
		SendNotifyMessage(CN_COMMAND , 0);
		break;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CTimeInputBox, CDialog)
//{{
/*  ON_MESSAGE(ID_BUTUPHOUR, CN_COMMAND, OnHourUp)
  ON_MESSAGE(ID_BUTDOWNHOUR, CN_COMMAND, OnHourDown)
  ON_MESSAGE(ID_BUTUPMIN, CN_COMMAND, OnMinUp)
  ON_MESSAGE(ID_BUTDOWNMIN, CN_COMMAND, OnMinDown) */
   ON_MESSAGE(IDC_NUMKEY, WM_KEY, OnNumKey) 
   ON_MESSAGE(IDC_BTNTIMEFLAG, CN_COMMAND, OnAMPM) 
   ON_MESSAGE(IDC_RADTIMETYPE, CN_COMMAND, OnTimeType) 
   ON_MESSAGE(IDC_BTNHOUR, CN_COMMAND, OnInputSwitch) 
   ON_MESSAGE(IDC_BTNMINU, CN_COMMAND, OnInputSwitch) 
//}}
END_MESSAGE_MAP
////////////////////////////////////////////////////////////////////////////////
//

void CTimeInputBox::PaintDigClock(CDC &dc)
{
/*	char text[30];
	sprintf(text, "%02d:%02d", m_oClock.GetHour(), m_oClock.GetMinute());
	dc.TextOut(m_obutHourUp.GetWindowRect().left, m_obutHourUp.GetWindowRect().bottom+2, text);*/
/*	sprintf(text, "%02d:%02d", m_oClock.GetHour(), m_oClock.GetMinute());
	CPoint pt(100 , 83);
	DrawNumString(pt.x , pt.y, text);
	if(m_oClock.GetHour() >= 12)
		dc.PutIcon(pt.x +32 ,pt.y ,ICON_TIME_PM);
	else
		dc.PutIcon(pt.x +32 ,pt.y ,ICON_TIME_AM);
	sprintf(text, "%02d" , m_oClock.GetSecond());
	DrawSecondString(pt.x +32 ,pt.y + 9, text);*/
}
////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::PaintOther(CDC &dc)
{
/*#define L_TOP 104
  DateTime dt;
  dc.MoveTo(2, L_TOP);
  dc.LineTo(159, L_TOP);
  ::GetSysTime(&dt);
  char text[50];
  sprintf(text, "今天:%4d年%d月%d日", dt.year, dt.month, dt.day);
  dc.TextOut(2, L_TOP+3, text);
  sprintf(text, "现在:%02d时%02d分", dt.hour, dt.minute);
  dc.TextOut(2, L_TOP+20, text);
#undef L_TOP*/

  
  char strSec[6];
  sprintf(strSec, "%02d" , m_iSec);
  ::DrawSecondString(m_sRect.left+ 139,m_sRect.top + 96 , strSec);//画秒数
  sprintf(strSec, "%s" , ":");
  ::DrawNumString(m_sRect.left + 114 , m_sRect.top + 85, strSec); //画小时分钟之间的":"
  CRect r(m_sRect.left + 94, m_sRect.top + 81, m_sRect.left +159 ,m_sRect.top + 108);
  dc.FrameRect(r);
  r.DeflateRect(1, 1);
  dc.Draw3dRect(r, GRAY , LGRAY);  //画外面的三维框
  r.DeflateRect(1, 1);
  dc.SetPenColor(GRAY);
  dc.FrameRect(r);
}

BOOL CTimeInputBox::OnDestroy()
{
	::CloseCursor();
	return CDialog::OnDestroy();
}
////////////////////////////////////////////////////////////////////////////////
//
//class CSystemSetup;
BOOL CTimeInputBox::OnCreate()
{
	if(!m_oClock.Create(CPoint(2, 21), this, 0))
		return FALSE;
	m_oClock.SetClockStyle(0); 
	
/*#define B_LEFT 100
#define B_TOP  34
    CRect ctrRect(B_LEFT, B_TOP, B_LEFT+16, B_TOP+16);
    if(!m_obutHourUp.Create(NULL, ICON_UPARROW, ctrRect, this, ID_BUTUPHOUR))
		return FALSE;
	ctrRect.top = ctrRect.bottom+16; ctrRect.bottom = ctrRect.top+16;
	if(!m_obutHourDown.Create(NULL, ICON_DOWNARROW, ctrRect, this, ID_BUTDOWNHOUR))
		return FALSE;
	ctrRect.left = ctrRect.right + 8; ctrRect.right = ctrRect.left + 16;
	if(!m_obutMinUp.Create(NULL, ICON_DOWNARROW, ctrRect, this, ID_BUTDOWNMIN))
		return FALSE;
	ctrRect.top = B_TOP; ctrRect.bottom = B_TOP+16;
	if(!m_obutMinDown.Create(NULL, ICON_UPARROW, ctrRect, this, ID_BUTUPMIN))
		return FALSE;
#undef B_LEFT
#undef B_TOP*/
	if(!CDialog::OnCreate())
		return FALSE;
	//下面创建数字键盘输入控件
   	CRect pRectArray[10];
	UINT16  pKeyValue[]={ '0' , '1' , '2' , '3' , '4' , 
						'5' , '6' , '7' , '8', '9'
	};
	CRect ctrRect(m_sRect.left+13, m_sRect.bottom - 30 , m_sRect.right-6, m_sRect.bottom-10);
	extern BOOL CalcuRectValue(CRect *pRectArray , int iRows , int iCols , CRect rectTotal , 
					short xInterval , short yInterval);
	CalcuRectValue(pRectArray, 1, 10, ctrRect, 1, 0);
	m_oNumKey.Create(ICON_TIME_NUMKEY, 10 , pRectArray,pKeyValue, this, IDC_NUMKEY);
	//下面创建12小时、24小时制选择控件
	ctrRect.SetRect(m_sRect.left +88 , m_sRect.top +33 , m_sRect.left +159 , m_sRect.top +74);
	if(!m_oRadTimeType.Create(ctrRect, WS_VISIBLE , this , IDC_RADTIMETYPE))
		return FALSE;
	m_oRadTimeType.AddButton("12小时");
	m_oRadTimeType.AddButton("24小时");
	m_oRadTimeType.SetAlign(VIRTICAL);
	extern	SYSTEM_OPTION g_SystemOption;
	m_sTimeType = g_SystemOption.sTimeType;
	m_oRadTimeType.SetSel(g_SystemOption.sTimeType);

	//得到当前秒数
	m_iSec = 0;		//m_oClock.GetSecond();
	
	//下面对小时、分钟的数据初始化
	m_cHourPos = m_cMinuPos = 0;

	//创建小时输入控件
	ctrRect.SetRect(m_sRect.left +98 , m_sRect.top +85, m_sRect.left +113 , m_sRect.top +104);
	if(!m_oBtnHour.Create(NULL, ctrRect , this, IDC_BTNHOUR)) 
		return FALSE;
//	m_oBtnHour.SendMessage(WM_PENDOWN , 0 , 0);
	m_pCurInput = &m_oBtnHour;
	
	//创建分钟输入控件
	ctrRect.left = ctrRect.right + 7 , ctrRect.right = ctrRect.left + 16;
	if(!m_oBtnMinu.Create(NULL, ctrRect , this, IDC_BTNMINU)) 
		return FALSE;
	ctrRect.left = ctrRect.right +2, ctrRect.right = ctrRect.left + 17;
	ctrRect.bottom = ctrRect.top + 8;

	//创建AM/PM显示（切换）控件
/*	UINT32 nIconID;
    nIconID = (m_oClock.GetHour()<12 ? ICON_TIME_AM :ICON_TIME_PM);
	m_sTimeFlag = nIconID - ICON_TIME_AM;*/
	m_oBtnType.Create(0, ctrRect , this, IDC_BTNTIMEFLAG);
	if(m_sTimeType !=0) // 24 小时制
		m_oBtnType.SetWindowStyle(m_oBtnType.GetWindowStyle() & ~WS_VISIBLE);
    ctrRect = m_butCancel.GetWindowRect();
	ctrRect.right -= (WND_TITLE_HEIGHT+1);
	ctrRect.left  -= (WND_TITLE_HEIGHT+1);
    if(!m_obutOk.Create(NULL, ICON_OK, ctrRect, this, IDOK))
		return FALSE;
	ctrRect.right = ctrRect.left -1;
	ctrRect.left = ctrRect.right -34;
//	if(!m_oBtnDate.Create("日期" , ctrRect , this , IDC_BTN_DATE))
//		return FALSE;
//    m_cCurPos = 0;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::OnPaint(CDC &dc)
{
	if(!(m_dwStyle & WS_TRANSPARENT))
	   dc.EraseRect(m_sRect);
   
   if(m_dwStyle & WS_BORDER)
   	     dc.FrameRect(m_sRect);

   if(m_dwStyle & WS_CAPTION)
	     PaintTitle(dc);
   dc.Reset(); 
// PaintDigClock(dc);
   PaintChildWnd();
   PaintOther(dc);
}

////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::SetParmaters(INT16 nHour, INT16 nMinute)
{
	m_oClock.SetTime(nHour, nMinute, 0); 
	nHour = m_oClock.GetHour();
	UINT32 nIconID;
    nIconID = (nHour<12 ? ICON_TIME_AM :ICON_TIME_PM);
	m_oBtnType.SetIcon(nIconID);
	m_sTimeFlag = nIconID - ICON_TIME_AM;
	if(m_sTimeType==0) //12小时制
	{
		if(m_sTimeFlag&& nHour!=12) //下午
			nHour -=12;
		else if(nHour==0)
			nHour =12;
	}
	sprintf(m_strHour, "%02d" , nHour);
	sprintf(m_strMinu, "%02d", m_oClock.GetMinute());
	m_oBtnMinu.SetWindowText(m_strMinu);
	m_oBtnHour.SetWindowText(m_strHour);
	m_oBtnHour.SendMessage(WM_PENDOWN , 0, 0);
}
////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::GetTime(INT16 &nHour, INT16 &nMinute)
{
	INT16 sec;
	m_oClock.GetTime(nHour, nMinute, sec); 
}

void CTimeInputBox::UpdateClock()
{
	extern double  ConvStrtonum(char *pStr);
	INT16 iHour = (INT16)ConvStrtonum(m_strHour);
	INT16 iMinu = (INT16)ConvStrtonum(m_strMinu);
	//下面把12小时转换成24小时
	if(m_sTimeType==0)
	{
		if(m_sTimeFlag==0 && iHour==12)
			iHour=0;
		else if(m_sTimeFlag ==1 &&iHour<12)
			iHour +=12;
	}
	else
	{
		if(m_sTimeFlag==1 && iHour <12)//???
			iHour+= 12;
	}
	m_oClock.SetTime(iHour, iMinu,m_iSec);
	m_oClock.OnPaint(CDC());
}
//用户输入小时时的处理函数
void CTimeInputBox::DoHourMsg(short ch)
{
	m_cHourPos %= 2;
	if(m_cHourPos==0)//第一位
	{
		if(m_sTimeType == 0)//12小时制
		{
			if(ch > '1' )
			{
				m_strHour[m_cHourPos++] = '0';
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
			}
			else
			{
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
			}
			m_oBtnHour.SetWindowText(m_strHour,TRUE,TRUE);
		}
		else  //24小时制
		{
			if(ch > '2')
			{
				m_strHour[m_cHourPos++] = '0';
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
				/*if(m_sTimeFlag)//是PM 
				{
					m_oBtnType.SetIcon(ICON_TIME_AM);
					m_oBtnType.OnPaint(CDC());
					
				}*/

			}
			else 
			{
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
			}
			m_sTimeFlag = 0;
			m_oBtnHour.SetWindowText(m_strHour, TRUE, TRUE);
		}
		UpdateClock();		
	}
	else //是第二位
	{
		if(m_sTimeType==0)//12小时制
		{
			if(m_strHour[0] =='0')
			{
				if(ch=='0')
					strcpy(m_strHour, "12");
				else
					m_strHour[m_cHourPos++] = ch;
				m_cHourPos =2;
				m_strHour[m_cHourPos]= '\0';
			}
			else 
			{
				if(ch>'2')
					return;
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
			}
			m_oBtnHour.SetWindowText(m_strHour, TRUE,TRUE);
			UpdateClock();
		}
		else //24小时制
		{
			if(m_strHour[0]== '0')
			{
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
				/*if(m_sTimeFlag)//是PM 
				{
					m_oBtnType.SetIcon(ICON_TIME_AM);
					m_oBtnType.OnPaint(CDC());
				
				}*/
				m_sTimeFlag = 0;
			}
			else if(m_strHour[0]=='1')
			{
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
				/*if(ch>='2' && m_sTimeFlag==0)//显示AM
				{
					m_oBtnType.SetIcon(ICON_TIME_PM);
					m_oBtnType.OnPaint(CDC());

				}*/
				if(ch>='2' && m_sTimeFlag==0)
					m_sTimeFlag = 1;
				else 
					m_sTimeFlag = 0;
			}
			else
			{
				if(ch >='4')
					return;
				m_strHour[m_cHourPos++] = ch;
				m_strHour[m_cHourPos] = '\0';
				/*if(m_sTimeFlag==0)//显示AM
				{
					m_oBtnType.SetIcon(ICON_TIME_PM);
					m_oBtnType.OnPaint(CDC());
					m_sTimeFlag = 1;
				}*/
				m_sTimeFlag = 1;
			}
			m_oBtnHour.SetWindowText(m_strHour, TRUE,TRUE);
			UpdateClock();
		}
	}				
}

//用户输入分钟时的处理函数
void CTimeInputBox::DoMinuMsg(short ch)
{
	m_cMinuPos %=2;
	if(m_cMinuPos==0)
	{
		if(ch>='6')
		{
			m_strMinu[0] = '0', m_strMinu[1] = ch;
			m_strMinu[2] ='\0';
			m_cMinuPos = 2;
		}
		else
			m_strMinu[m_cMinuPos++]= ch,m_strMinu[m_cMinuPos]= '\0';
		m_oBtnMinu.SetWindowText(m_strMinu,TRUE,TRUE);
		UpdateClock();
	}
	else
	{
		m_strMinu[m_cMinuPos++]= ch;
		m_strMinu[m_cMinuPos]= '\0';
		m_oBtnMinu.SetWindowText(m_strMinu,TRUE,TRUE);
		UpdateClock();
	}
}

void CTimeInputBox::OnNumKey(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_pCurInput == &m_oBtnHour)
	    DoHourMsg(lParam);
	else if(m_pCurInput== &m_oBtnMinu)
		DoMinuMsg(lParam);
}
	
void CTimeInputBox::OnInputSwitch(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(wParam == IDC_BTNHOUR)
		m_pCurInput = &m_oBtnHour;
	else
		m_pCurInput = &m_oBtnMinu;
}

void CTimeInputBox::OnAMPM(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
//	INT16 iHour, iMinu, iSec;
/*	m_cHourPos = m_cMinuPos = 0;
	m_pCurInput = NULL;*/
	if(m_sTimeType == 1)
		return;
	if(m_sTimeFlag==0)//现在是AM
	{
/*		if(m_sTimeType==1)
		{
			m_oClock.GetTime(iHour, iMinu, iSec);
			iHour += 12;
			sprintf(m_strHour,"%02d" , iHour);
			m_oBtnHour.SetWindowText(m_strHour, FALSE , TRUE);
			m_cHourPos = m_cMinuPos = 0;
			m_pCurInput = NULL;
		}
		*/
		m_sTimeFlag = 1;
		UpdateClock();
	}
	else
	{
		/*if(m_sTimeType==1)
		{
			m_oClock.GetTime(iHour, iMinu,iSec);
			iHour -= 12;
			sprintf(m_strHour, "%02d" , iHour);
			m_oBtnHour.SetWindowText(m_strHour, FALSE, TRUE);
			m_cHourPos = m_cMinuPos = 0;
			m_pCurInput = NULL;
		}*/
		m_sTimeFlag = 0;
		UpdateClock();
	}
	m_oBtnType.SetIcon(m_sTimeFlag ? ICON_TIME_PM: ICON_TIME_AM);
	m_oBtnType.OnPaint(CDC());
}

void CTimeInputBox::OnTimeType(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	INT16 iHour;
	if(m_sTimeType== lParam)
		return;
	m_sTimeType = lParam;
	if(lParam==0)//24 -> 12小时
	{
		iHour = (INT16)::ConvStrtonum(m_strHour);
		if(m_sTimeFlag)//PM
		{
			if(iHour>12)
			{
				iHour -= 12;
				sprintf(m_strHour, "%02d" , iHour);
				m_oBtnHour.SetWindowText(m_strHour, FALSE, TRUE);
				m_cHourPos = m_cMinuPos = 0;
				m_pCurInput = NULL;
			}
		}
		else if(iHour==0) 
		{
			iHour=12;
			sprintf(m_strHour, "%02d" , iHour);
			m_oBtnHour.SetWindowText(m_strHour,FALSE , TRUE);
			m_cHourPos = m_cMinuPos = 0;
			m_pCurInput = NULL;
		}
		UpdateClock();
		if(!(m_oBtnType.GetWindowStyle() & WS_VISIBLE))//显示AM控件
		{
			m_oBtnType.SetWindowStyle(m_oBtnType.GetWindowStyle() | WS_VISIBLE);
			m_oBtnType.SetIcon(m_sTimeFlag? ICON_TIME_PM : ICON_TIME_AM);
			m_oBtnType.OnPaint(CDC());
		}
	}
	else   //12 -> 24小时
	{
		iHour = ::ConvStrtonum(m_strHour);
		if(m_sTimeFlag)//现在是PM
		{
			if(iHour!=12)
			{
				iHour += 12;
				sprintf(m_strHour, "%02d" , iHour);
				m_oBtnHour.SetWindowText(m_strHour,FALSE, TRUE);
				m_cHourPos = m_cMinuPos = 0;
				m_pCurInput = NULL;
			}
		}
		else if(iHour==12)
		{
			iHour=0;
			sprintf(m_strHour, "%02d" , iHour);
			m_oBtnHour.SetWindowText(m_strHour,FALSE,TRUE);
			m_cHourPos = m_cMinuPos = 0;
			m_pCurInput = NULL;
		}
		UpdateClock();
		if(m_oBtnType.GetWindowStyle() & WS_VISIBLE)//插去AM控件
		{
			CRect r = m_oBtnType.GetWindowRect();
			CDC dc;
			dc.EraseRect(r);
			m_oBtnType.SetWindowStyle(m_oBtnType.GetWindowStyle() &~WS_VISIBLE);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//
/*void CTimeInputBox::OnHourUp()
{
	m_oClock.StepHour(); 
	PaintDigClock(CDC());
}

////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::OnHourDown()
{
	m_oClock.StepHour(FALSE); 
	PaintDigClock(CDC()); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::OnMinUp()
{
	m_oClock.StepMinute();
	PaintDigClock(CDC()); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CTimeInputBox::OnMinDown()
{
	m_oClock.StepMinute(FALSE); 
	PaintDigClock(CDC()); 
}
#undef ID_BUTUPHOUR
#undef ID_BUTDOWNHOUR
#undef ID_BUTUPMIN
#undef ID_BUTDOWNMIN*/
////////////////////////////////////////////////////////////////////////////////
//描述
//    显示一个时间输入窗口
//参数
//    strCaption   输入框标题
//    nHour        
//    nMinute      入口时初始化输入时分，出口时返回用户选择的时分,其值均为24小时制!!!
//    nTimeType    时间的类型:为0表示12小时制，为1表示24小时制
//返回
//    IDOK         用户按下"确定"按钮
//    IDCANCEL     用户按下"取消"按钮
//
#define        TIME
UINT32 TimeInputBox(char * strCaption, INT16 &nHour, INT16 &nMinute, INT16 &nTimeType,UINT32 nHelpID)
{
	CTimeInputBox iptbox;
	iptbox.Create(strCaption, WS_VISIBLE|WS_CAPTION, DESK_TOP_RECT, TRUE,HELP_SETUP_TIME); 
	iptbox.SetParmaters(nHour, nMinute);
	UINT32 rcode = iptbox.DoModal();
	if(rcode == IDOK)
	{
		iptbox.GetTime(nHour, nMinute);
		iptbox.GetTimeType(nTimeType);
	}
	return rcode;
}
////////////////////////////////////////////////////////////////////////////////
//end messagebox.cpp
 