#if !defined(AFX_INPUTDLG_H__3E0A0999_75A6_4848_8994_A96B63FF2BD7__INCLUDED_)
#define AFX_INPUTDLG_H__3E0A0999_75A6_4848_8994_A96B63FF2BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog
#include "softkey.h"


#define INPUTMATH_D		0x100
#define INPUTMATH_X		0x101
#define INPUTMATH_ENG	0x102
#define INPUTMATH_F		0x103
#define INPUTMATH_P		0x104
#define INPUTMATH_H		0x105

#define HANZI_LEN_MAX	320
#define PINYIN_LEN_MAX	6

#define PY_HZ_SHOWLEN	5
#define SX_HZ_SHOWLEN	9

#define VOIPINPUT_FRMAE_HEIGHT		112
#define VOIPINPUT_FRMAE_WIDTH		480
#define VOIPINPUT_LCD_HEIGHT		234

#define TOP_INPUT					0
#define BOTTOM_INPUT				(VOIPINPUT_LCD_HEIGHT - VOIPINPUT_FRMAE_HEIGHT + 1)

class CInputDlg : public CDialog
{
// Construction
public:
	CEdit		*m_pOwnerEdit;
	CSoftKey	m_softkey;
	void		SetOwnerEdit(CEdit *pEdit);
public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_INPUTDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSoftKey(WPARAM w, LPARAM l);
	afx_msg void OnSoftKeyReg(WPARAM w, LPARAM l);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG


public:
	//////////////////////////////////////////////////////////////////////////////
	//说明:				创建函数
	//参数:	
	//		int y, 输入框的位置
	//      style,首先显示的输入方式:
	//				INPUTMATH_H 手写,INPUTMATH_D 英文大写,
	//				INPUTMATH_X 英文小写,INPUTMATH_F 符号
	void SetInputType(int y, int style = INPUTMATH_D);
	void SetLimit(BOOL falg = TRUE){m_bIsLimit = falg;}

protected:
	void OnSoftKeyP(NMHDR_*, LRESULT*);
	
	void OnSoftKeySym(NMHDR_*, LRESULT*);
	void OnSoftKeyEng(NMHDR_*, LRESULT*);
	void OnSoftKeyHand(NMHDR_*, LRESULT*);

private:
	void l_SetInputWindow();	//设置输入方式:手写,拼音,英文,符号,但界面不刷新
	void l_PutIcon();			//刷新界面
	//改变输入方式，主要调用上面两个函数，
	void ChangMode(int iMode,int iRangeOrUpLow);
	
	//int nInputmath	0 为手写联想， 1 为拼音联想
	//int style			0 为初始化	1 为左翻页	2 为右翻页

	char m_chLXBuff[64+1];		//保存联想候选字buff
	int	 m_nLXLocal;			//记录联想显示的位置
	int  m_nLXLen;				//记录联想候选字的长度

	//拼音
	char m_chPYBuff[HANZI_LEN_MAX+1];			//记录拼音汉字
	char m_chPYInPut[PINYIN_LEN_MAX+1];			//记录拼音输入
	int  m_nPYInputCount;						//记录输入字母的个数
	int  m_nPYLocal;							//记录当前显示位置
	int  m_nPYLen;								//记录返回汉字的个数
	BOOL m_bISPY;								//记录当前是否是输入拼音状态
	BOOL m_bISLX;								//记录当前是否是联想状态

	char m_chPyShowBuff[32];                    //记录拼音或者手写时,在候选区显示的汉字
	char m_chSxShowBuff[32];                    //记录拼音或者手写时,在候选区显示的汉字

	void LianXiang(short nHz, int nInputmath, int style); //联想汉字并显示 nHz 高位为区码,低位为位码
	void PinYinHZ(int style);				
	void PinYinInPut(int style);

	CString		m_CopyString;

	//2006.11.18 add by lxz
public:
	BOOL MoveTo(int y);
	void hide(BOOL flag = TRUE);
	void show(BOOL flag = TRUE);
	
	void hideorshow();

private:
	void CreateCtrl();
	BOOL m_bIsLimit;			//是否限制特殊字符的输入\n

	int		m_nRegStyle;		//记录识别的范围 0 汉字 1  英文  2 数字
	char *m_pPreText;			//保存输入buff
	char *m_Text;				//用来接收输入的内容
	int  m_mInputMath;			//输入方式
	int  m_mOldInputMath;		//记录前次输入方式  2005.4.9 lxz
	int	 m_mInputMathStyle;		//用来记录是否数字输入
	int  m_iLen;
	BOOL m_bInputPassWord;		//是否是用来输入密码
	int  m_iRecogRange;			//手写识别范围 
	int  m_iUpOrLowCase;		//记录英文输入时是大写还是小写
	
//	CWrite m_oWrite;			//手写控件

	int  m_xPoint, m_yPoint;	//窗口的位置
	BOOL m_bIsShow;

	CSoftKey m_oSoftkeyFun1, m_oSoftkeyFun2;
	CSoftKey m_oSoftkeyH1, m_oSoftkeyH2;	//手写键盘 左右两个
	CSoftKey m_oSoftkeyP;  //拼音键盘
	CSoftKey m_oSoftkeyEng; //英文大小键盘
	CSoftKey m_oSoftkeySymbol;	//符号键盘
	CSoftKey m_oSoftkeyHand;	//手写
	
	char   m_strPad[128];		//剪贴板缓冲,不用但暂时保留 
	unsigned short m_nCheckTxt;  //2005.5.19

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__3E0A0999_75A6_4848_8994_A96B63FF2BD7__INCLUDED_)
