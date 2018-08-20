#if !defined(AFX_COPYFILEDLG_H__D3A688B9_0BC7_4342_9F15_56AB71CCDDD7__INCLUDED_)
#define AFX_COPYFILEDLG_H__D3A688B9_0BC7_4342_9F15_56AB71CCDDD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// copyfileDlg.h : header file

#include "../Resource.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Control/CEProcessBar.h"
#include "../Control/MJPGStatic.h"

namespace View {
    using namespace Control;
    //�ļ��ǻָ����Ǳ���    0 �����ļ�  1 �ָ��ļ� 2 ����¼���ļ�
    enum operater_type{backup_file, restore_file, copy_file, 
        mp3_in, mp3_out,
        photo_in, photo_out,
        bell_in, bell_out,
        show_in, show_out};
    //����״̬				0 ��ʼ  1 û�в���U�� 2 U������ͬ���ļ� 3 ���ڿ��� 4 �������
    enum operater_status{start_status, nousb_status, isexitfile_status, copying_status, finish_status};
    /////////////////////////////////////////////////////////////////////////////
    // copyfileDlg dialog
    const PTCHAR ptchLocalDb = _T("");
    //const PTCHAR ptchLocalMp3 = ::musicPath;
    //const PTCHAR ptchLocalPhoto = ::photoPath;
    //const PTCHAR ptchLocalBell = ::ringPath;
    //const PTCHAR ptchLocalShow = ::adPath;
    const PTCHAR ptcPlayList[] = {_T("/UsbDisk/mediaphone/my_music/playlist.pls"), _T("/StorageCard/mediaphone/my_music/playlist.pls")};

    class copyfileDlg : public CCEDialog
    {
    public:
        // 	CCEBmpButton m_btnOk;
        // 	CCEBmpButton m_btnCancel;
        // 
        // 	CCEFramePartStatic m_sticBackground;

        void SetType(operater_type type);
        operater_status progress_status_;		//����״̬				0 ��ʼ  1 û�в���U�� 2 U������ͬ���ļ� 3 ���ڿ��� 4 �������
        CCEStatic m_sticTitle;
        CCEProcessBar m_procbarSound;
        int m_nPos;
        operater_type type_;					//�ļ��ǻָ����Ǳ���    0 �����ļ�  1 �ָ��ļ� 2 ����¼���ļ�

        HANDLE			hCopyFileThread;

        // 	static enum {usb_device, sdcard_device, none_device} m_devStatus;
        // 	static TCHAR m_chExtPath[64];
        static void CopyFileProc(void);
        void StartCopyFile();

        // Construction
    public:
        copyfileDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(copyfileDlg)
        enum { IDD = IDD_DIALOG_COPYFILE };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(copyfileDlg)
    protected:
        virtual BOOL OnInitDialog();
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        void Import(BOOL bOverflow=FALSE);

    protected:
        // Generated message map functions
        //{{AFX_MSG(copyfileDlg)
        // NOTE: the ClassWizard will add member functions here
        afx_msg void OnButtonCancel();
        afx_msg void OnTimer(UINT nIDTimer);
        //}}AFX_MSG
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        DECLARE_MESSAGE_MAP()
    private:
        CMJPGStatic		m_MJPGList;
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_COPYFILEDLG_H__D3A688B9_0BC7_4342_9F15_56AB71CCDDD7__INCLUDED_)
