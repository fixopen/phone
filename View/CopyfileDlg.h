#if !defined(AFX_COPYFILEDLG_H__D3A688B9_0BC7_4342_9F15_56AB71CCDDD7__INCLUDED_)
#define AFX_COPYFILEDLG_H__D3A688B9_0BC7_4342_9F15_56AB71CCDDD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// copyfileDlg.h : header file
//

#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Control/CEProcessBar.h"

//文件是恢复还是备份    0 备份文件  1 恢复文件 2 拷贝录音文件
enum operater_type{backup_file, restore_file, copy_file};
//操作状态				0 开始  1 没有插入U盘 2 U盘有相同的文件 3 正在拷贝 4 拷贝完成
enum operater_status{start_status, nousb_status, isexitfile_status, copying_status, finish_status};
/////////////////////////////////////////////////////////////////////////////
// copyfileDlg dialog

//#define WM_CHANGELANGUAGE (WM_USER+300)
class copyfileDlg : public CCEDialog
{
public:
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;

	CCEFramePartStatic m_sticBackground;

	void SetType(operater_type type);
	operater_status progress_status_;		//操作状态				0 开始  1 没有插入U盘 2 U盘有相同的文件 3 正在拷贝 4 拷贝完成
	CCEStatic m_sticTitle;
	CCEProcessBar m_procbarSound;
	int m_nPos;
	operater_type type_;					//文件是恢复还是备份    0 备份文件  1 恢复文件 2 拷贝录音文件

	HANDLE			hCopyFileThread;
	static void CopyFileProc(void);
	void StartCopyFile();
    void ChangeLanguage();

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

	// Generated message map functions
	//{{AFX_MSG(copyfileDlg)
	// NOTE: the ClassWizard will add member functions here

	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancel();
	afx_msg void OnTimer(UINT nIDTimer);
    afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYFILEDLG_H__D3A688B9_0BC7_4342_9F15_56AB71CCDDD7__INCLUDED_)
