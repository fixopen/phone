// GPhoneDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>

// CGPhoneDlg �Ի���
class CGPhoneDlg : public CDialog
{
// ����
public:
	CGPhoneDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GPHONE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
    std::vector<int> logIds_;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDestroy();
};
