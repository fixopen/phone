#ifndef __CONTROL_INPUTDLG_H__
#define __CONTROL_INPUTDLG_H__

#include "../Resource.h"

#include "SoftKey.h"

namespace Control {
    int const INPUTMATH_D = 0x100;
    int const INPUTMATH_X = 0x101;
    int const INPUTMATH_ENG = 0x102;
    int const INPUTMATH_F = 0x103;
    int const INPUTMATH_P = 0x104;
    int const INPUTMATH_H = 0x105;

    int const HANZI_LEN_MAX = 320;
    int const PINYIN_LEN_MAX = 6;

    int const PY_HZ_SHOWLEN = 5;
    int const SX_HZ_SHOWLEN = 9;

    int const VOIPINPUT_FRMAE_HEIGHT = 219;
    int const VOIPINPUT_FRMAE_WIDTH = 800;
    int const VOIPINPUT_LCD_HEIGHT = (480 - 22);

    int const TOP_INPUT = 0;
    int const BOTTOM_INPUT = (VOIPINPUT_LCD_HEIGHT - VOIPINPUT_FRMAE_HEIGHT + 1);

    class CInputDlg : public CDialog
    {
        // Construction
    public:
        CFont		m_font;
        CEdit		*m_pOwnerEdit;
        CSoftKey	m_softkey;
        void		SetOwnerEdit(CEdit *pEdit);
    public:
        CInputDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CInputDlg)
        enum { IDD = 10000/*IDD_INPUTDIALOG*/ };
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
        afx_msg LRESULT OnSoftKey(WPARAM w, LPARAM l);
        afx_msg LRESULT OnSoftKeyReg(WPARAM w, LPARAM l);
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
}

#endif //__CONTROL_INPUTDLG_H__
