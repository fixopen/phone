/******************************************************************************/
/*                                                                            */
/*  ��Ȩ����(c)  2001�����Ƽ�                                                 */
/*                                                                            */
/******************************************************************************/
/*  �� �� ��                                                                  */
/*             messagebox.h                                                   */
/*                                                                            */
/*  �� �� ��                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �����Ƽ�E-PhoneII��������ͷ�ļ�                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  ��    ��                                                                  */
/*             MessageBox��������                                             */
/*             InputBox��������                                               */
/*             NumInputBox��������                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             ����                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             2001-07-16                                                     */
/*                                                                            */
/*  ��ʷ��¼                                                                  */
/*             2001-08-07   ����NumInputBox                                   */
/*                                                                            */
/*             2001-10-15    �����������뺯��                                 */
/*                           ����ʱ�����뺯��                                 */
/******************************************************************************/
#ifndef _HWFCL_MESSAGEBOX_H__
#define _HWFCL_MESSAGEBOX_H__
#include "hwfcl.h"
#include "window.h"
#include "datadrv.h"
#define BUTTONTYPE          4

////////////////////////////////////////////////////////////////////////////////
//���Ͷ���
#define MB_OK               1               //"ȷ��"��ť
#define MB_YES              2               //"��"��ť
#define MB_NO               4               //"��"��ť
#define MB_CANCEL           8               //"ȡ��"��ť
#define MB_ICONSTOP         16              //��ʾSTOPͼ��
#define MB_ICONQUESTION     32              //��ʾQUESTIONͼ��
#define MB_ICONEXCLAMATION  64              //��ʾEXCLAMATIONͼ��
#define MB_ICONINFORMATION  128             //��ʾINFORMATIONͼ��

#define MB_OKCANCEL         (MB_OK+MB_CANCEL)
#define MB_YESNO            (MB_YES+MB_NO)
#define MB_YESNOCANCEL      (MB_YES+MB_NO+MB_CANCEL)

////////////////////////////////////////////////////////////////////////////////
//����
//     ��ʾһ����ʾ����
//����
//    strText      ��ʾ������Ϣ 
//    strCaption   ��ʾ�����
//    uType        ��ʾ�������ͼ�����Ķ���
//����
//    IDOK         �û�����"ȷ��"��ť
//    IDYES        �û�����"��"��ť
//    IDNO         �û�����"��"��ť
//    IDCANCEL     �û�����"ȡ��"��ť�����ϽǵĴ��ڹرհ�ť
//
UINT32 MessageBox(char * strText, char * strCaption = NULL, UINT32 uType = MB_OK);


#define    INIT_WRITE   0
#define    INIT_PY      1
#define    INIT_ENG     2
#define    INIT_SYMBO   3
////////////////////////////////////////////////////////////////////////////////
//����
//    ��ʾһ�����봰��
//����
//    strCaption   ��������
//    strInputBuf  ����û��������ݻ�����
//    nBufLen      ����������
//    nInitStatus  ��ʼ״̬  INIT_WRITE ��д INIT_PY ƴ�� INIT_ENGӢ�� INIT_SYMBO����
//����
//    IDOK         �û�����"ȷ��"��ť
//    IDCANCEL     �û�����"ȡ��"��ť
//
UINT32 InputBox(char * strCaption, char * strInputBuf, INT32 nBufLen, UINT8 nInitStatus = INIT_WRITE , BOOL bCanEnter = FALSE);

////////////////////////////////////////////////////////////////////////////////
//����
//    ��ʾһ���绰�����������봰��
//����
//    strCaption   ��������
//    strInputBuf  ����û��������ݻ�����
//    nBufLen      ����������
//����
//    IDOK         �û�����"ȷ��"��ť
//    IDCANCEL     �û�����"ȡ��"��ť
//
UINT32 NumInputBox(char * strCaption, char * strInputBuf, INT32 nBufLen);

////////////////////////////////////////////////////////////////////////////////
//����
//    ��ʾһ���������봰��
//����
//    strCaption   ��������
//    nYear
//    nMonth
//    nDay         ���ʱ��ʼ�����������գ�����ʱ�����û�ѡ���������
//����
//    IDOK         �û�����"ȷ��"��ť
//    IDCANCEL     �û�����"ȡ��"��ť
//
UINT32 DateInputBox(char * strCaption, INT16 &nYear, INT16 &nMonth, INT16 &nDay,UINT32 nHelpID = 0);

////////////////////////////////////////////////////////////////////////////////
//����
//    ��ʾһ��ʱ�����봰��
//����
//    strCaption   ��������
//    nHour
//    nMinute      ���ʱ��ʼ������ʱ�֣�����ʱ�����û�ѡ���ʱ��
//����
//    IDOK         �û�����"ȷ��"��ť
//    IDCANCEL     �û�����"ȡ��"��ť
//
UINT32 TimeInputBox(char * strCaption, INT16 &nHour, INT16 &nMinute, INT16 &nTimeType, UINT32 nHelpID =0);

class CIcon :public CIconButton
{
public:
	virtual BOOL Create(UINT32 nIconID , CRect &ctrlRect , CFrameWnd *pParent , UINT32 nCtrlID =0);
	virtual void OnPaint(CDC&dc)
	{
		dc.SetBackColor();
		dc.EraseRect(m_sRect);
		INT16 x = (m_sRect.left + m_sRect.right)/2 - gICON_WIDTH(m_nIconId)/2; 
		INT16 y = (m_sRect.top + m_sRect.bottom)/2 - gICON_HEIGHT(m_nIconId)/2; 
		dc.PutIcon(x , y, m_nIconId);
	}
};


#endif
////////////////////////////////////////////////////////////////////////////////
//end messagebox.h