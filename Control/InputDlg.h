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
	//˵��:				��������
	//����:	
	//		int y, ������λ��
	//      style,������ʾ�����뷽ʽ:
	//				INPUTMATH_H ��д,INPUTMATH_D Ӣ�Ĵ�д,
	//				INPUTMATH_X Ӣ��Сд,INPUTMATH_F ����
	void SetInputType(int y, int style = INPUTMATH_D);
	void SetLimit(BOOL falg = TRUE){m_bIsLimit = falg;}

protected:
	void OnSoftKeyP(NMHDR_*, LRESULT*);
	
	void OnSoftKeySym(NMHDR_*, LRESULT*);
	void OnSoftKeyEng(NMHDR_*, LRESULT*);
	void OnSoftKeyHand(NMHDR_*, LRESULT*);

private:
	void l_SetInputWindow();	//�������뷽ʽ:��д,ƴ��,Ӣ��,����,�����治ˢ��
	void l_PutIcon();			//ˢ�½���
	//�ı����뷽ʽ����Ҫ������������������
	void ChangMode(int iMode,int iRangeOrUpLow);
	
	//int nInputmath	0 Ϊ��д���룬 1 Ϊƴ������
	//int style			0 Ϊ��ʼ��	1 Ϊ��ҳ	2 Ϊ�ҷ�ҳ

	char m_chLXBuff[64+1];		//���������ѡ��buff
	int	 m_nLXLocal;			//��¼������ʾ��λ��
	int  m_nLXLen;				//��¼�����ѡ�ֵĳ���

	//ƴ��
	char m_chPYBuff[HANZI_LEN_MAX+1];			//��¼ƴ������
	char m_chPYInPut[PINYIN_LEN_MAX+1];			//��¼ƴ������
	int  m_nPYInputCount;						//��¼������ĸ�ĸ���
	int  m_nPYLocal;							//��¼��ǰ��ʾλ��
	int  m_nPYLen;								//��¼���غ��ֵĸ���
	BOOL m_bISPY;								//��¼��ǰ�Ƿ�������ƴ��״̬
	BOOL m_bISLX;								//��¼��ǰ�Ƿ�������״̬

	char m_chPyShowBuff[32];                    //��¼ƴ��������дʱ,�ں�ѡ����ʾ�ĺ���
	char m_chSxShowBuff[32];                    //��¼ƴ��������дʱ,�ں�ѡ����ʾ�ĺ���

	void LianXiang(short nHz, int nInputmath, int style); //���뺺�ֲ���ʾ nHz ��λΪ����,��λΪλ��
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
	BOOL m_bIsLimit;			//�Ƿ����������ַ�������\n

	int		m_nRegStyle;		//��¼ʶ��ķ�Χ 0 ���� 1  Ӣ��  2 ����
	char *m_pPreText;			//��������buff
	char *m_Text;				//�����������������
	int  m_mInputMath;			//���뷽ʽ
	int  m_mOldInputMath;		//��¼ǰ�����뷽ʽ  2005.4.9 lxz
	int	 m_mInputMathStyle;		//������¼�Ƿ���������
	int  m_iLen;
	BOOL m_bInputPassWord;		//�Ƿ���������������
	int  m_iRecogRange;			//��дʶ��Χ 
	int  m_iUpOrLowCase;		//��¼Ӣ������ʱ�Ǵ�д����Сд
	
//	CWrite m_oWrite;			//��д�ؼ�

	int  m_xPoint, m_yPoint;	//���ڵ�λ��
	BOOL m_bIsShow;

	CSoftKey m_oSoftkeyFun1, m_oSoftkeyFun2;
	CSoftKey m_oSoftkeyH1, m_oSoftkeyH2;	//��д���� ��������
	CSoftKey m_oSoftkeyP;  //ƴ������
	CSoftKey m_oSoftkeyEng; //Ӣ�Ĵ�С����
	CSoftKey m_oSoftkeySymbol;	//���ż���
	CSoftKey m_oSoftkeyHand;	//��д
	
	char   m_strPad[128];		//�����建��,���õ���ʱ���� 
	unsigned short m_nCheckTxt;  //2005.5.19

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__3E0A0999_75A6_4848_8994_A96B63FF2BD7__INCLUDED_)
