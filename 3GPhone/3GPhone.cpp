// 3GPhone.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "3GPhone.h"
#include "3GPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy3GPhoneApp

BEGIN_MESSAGE_MAP(CMy3GPhoneApp, CWinApp)
END_MESSAGE_MAP()


// CMy3GPhoneApp ����
CMy3GPhoneApp::CMy3GPhoneApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMy3GPhoneApp ����
CMy3GPhoneApp theApp;

// CMy3GPhoneApp ��ʼ��

BOOL CMy3GPhoneApp::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMy3GPhoneDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
