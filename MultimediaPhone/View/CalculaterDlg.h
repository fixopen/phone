#if !defined(AFX_CALCULATER_H__F77690C8_F769_4312_B097_00AAA31C8B78__INCLUDED_)
#define AFX_CALCULATER_H__F77690C8_F769_4312_B097_00AAA31C8B78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Calculater.h : header file
//

#include "../Control/CeBtnST.h"
#include "../Control/CEStatic.h"
#include "../Control/SoftKey.h"
#include "../Control/CELineEdit.h"
#include "../Control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CCalculaterDlg dialog
namespace View {
    using namespace Control;
    class CCalculaterDlg : public CDialog
    {
    private:

        CMJPGStatic		m_MJPGList;
        /*
        CCEStatic		m_stcTitle;
        CCEBmpButton	m_btnExit;
        CSoftKey        m_SoftKey;
        CCEFrameStatic  m_backGroudStatic;
        */
        CCERectEdit		m_DialNumEdit;

        BOOL			m_IsError;  //2005.5.19 zmy ���ô���ı�־
        CFont m_Font;

    public:
        double  m_number1;    //MS�������ݻ�����bn
        int     m_keyvalue;   //���ż�ֵ,��������ʱ�����ж��Ƿ�ˢ��
        int     m_keyvalue1;  //���ż�ֵ,������־��������ǰ�ķ��ż�
        int     m_keyvalue2;  //�ӡ������ˡ�������5�����ż�ֵ,������־���������ķ��ż�
        int     m_keyvalue3;  //�Ӽ��˳�4�����ż�ֵ��������־�Ⱥ�ǰ�ķ��ż�
        double  m_dtemp0;    //���������е�һ������
        double  m_dtemp1;     //���������
        double  m_dtemp2;    //���������еڶ�������
        double	m_dCurtemp;	 //��ǰ��ʾ������������

        void ClearData();
        void OnExit_(){OnExit();}

        // Construction
    public:
        CCalculaterDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CCalculaterDlg)
        enum { IDD = IDD_DIALOG_CACULATER };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCalculaterDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        afx_msg void OnPaint();
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CCalculaterDlg)
        virtual BOOL OnInitDialog();
        afx_msg void OnSoftKey(WPARAM w, LPARAM l);
        afx_msg void OnExit();
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CALCULATER_H__F77690C8_F769_4312_B097_00AAA31C8B78__INCLUDED_)
