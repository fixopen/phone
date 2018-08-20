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

        BOOL			m_IsError;  //2005.5.19 zmy 设置错误的标志
        CFont m_Font;

    public:
        double  m_number1;    //MS键的数据缓冲区bn
        int     m_keyvalue;   //符号键值,数字输入时用来判断是否刷新
        int     m_keyvalue1;  //符号键值,用来标志数字输入前的符号键
        int     m_keyvalue2;  //加、减、乘、除、等5个符号键值,用来标志数字输入后的符号键
        int     m_keyvalue3;  //加减乘除4个符号键值，用来标志等号前的符号键
        double  m_dtemp0;    //存放运算符中第一个数字
        double  m_dtemp1;     //存放运算结果
        double  m_dtemp2;    //存放运算符中第二个数字
        double	m_dCurtemp;	 //当前显示在输入框的数字

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
