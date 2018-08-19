
/******************************************************************************/
/*                                                                            */
/*  ��Ȩ����(c)  2001�����Ƽ�                                                 */
/*                                                                            */
/******************************************************************************/
/*  �� �� ��                                                                  */
/*             cscrollwnd.h                                                   */
/*                                                                            */
/*  �� �� ��                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �����Ƽ�E-PhoneII��������Դ�ļ�                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  ��    ��                                                                  */
/*             ���������ڿؼ���ͷ�ļ�                                         */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �ʻ�                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  ��ʷ��¼                                                                  */
/******************************************************************************/
#ifndef __E401_MYCTRL_H_
#define __E401_MYCTRL_H_

#include "hwfcl.h"
#include "string.h"
#include "dc.h"    


class CScrollWnd  : public CCtrl  
{
private:
	UINT32 m_count;
	//
	UINT32				m_currentScrollPos;					//����������ǰ����������λ��
	UINT32				m_scrollHeight;						//����������ǰ����������߶�
	INT32				m_scrollRage;						//��������������Χ
	INT32				m_linesPerPage;						//��������ÿҳ����
	INT32				m_currentRagePos;					//����������ǰ����λ��

	INT32				BARWIDTH;							//�����������
	INT32				m_mouseOnSlider;					//������������Ƿ���ֻ���
	INT32				m_mouseOnArrow;						//������������Ƿ���ֻ���

	UINT32				m_b_currentScrollPos;				//���������ǰ����������λ��
	UINT32				m_b_scrollHeight;					//���������ǰ����������߶�
	INT32				m_b_scrollRage;						//�������������Χ
	INT32				m_b_linesPerPage;					//�������ÿҳ����
	INT32				m_b_currentRagePos;					//���������ǰ����λ��

	INT32				b_BARWIDTH;							//����������
	INT32				m_b_mouseOnSlider;					//�����������Ƿ���ֻ���
	INT32				m_b_mouseOnArrow;					//����������quyubiaozhi
    int					m_iMoveCount;
	INT32				BARMINHEIGHT;						//��������С���

public:
	BOOL Create(char *strName, const CRect wndRect, 
				   CWnd * pParent, 
				   UINT32 dwStyle, UINT32 nCtrlId, 
				   UINT32 uiBarWidth, 
				   int iV, int iH);

public:
	virtual void	WindowProcess(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//������
	virtual void	OnScrollPosChanged(UINT32 uiNewPos){};	//�϶����鴦���������û����أ�
	virtual void	OnClientEvent(UINT32 nMessage, 
							UINT32 x, UINT32 y){};			//����ͻ��������������û����أ�			
	virtual void	OnPaint(CDC &dc	);						//�ػ�����

//���µ�iSelect��ʾ�Ƕ����������������Ǻ������������
//���Ƕ�������������,���ǶԺ����������
	virtual void	PageDown(INT32 iSelect);				//�·�ҳ������
	virtual void	PageUp(INT32 iSelect);					//�Ϸ�ҳ������
	virtual void	ScrollDown(INT32 iSelect);				//�·�һ�д�����
	virtual void	ScrollUp(INT32 iSelect);				//�Ϸ�һ�д�����

	//���º�������������
public:
	void	SetScrollPos(INT32 iPos, INT32 iSelect);		//���ù���λ��
	INT32	GetScrollPos(INT32 iSelect);					//ȡ�ù���λ��
	void	SetScrollRage(INT32 iRage, INT32 iSelect);		//���ù�����Χ
	INT32	GetScrollRage(INT32 iSelect);					//ȡ�ù�����Χ
	void	SetRowsPerPage(INT32 iRage, INT32 iSelect);		//����ÿҳ������Χ
	INT32	GetRowsPerPage(INT32 iSelect);					//ȡ��ÿҳ������Χ
	INT32	GetBarWid(INT32 iSelect);						//ȡ�ù���λ��
	void		DrawScroll();								//��������//ygr 11-27

private:
	void	DoPtInSlider(INT32 x, INT32 y);					//�϶����鴦����
	void	DoPenDown(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//�����������
	void	DoPenMove(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//����ƶ�������
	void	DoPenOther(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//��������¼�������
	

private:
	void	SetSliderWid(INT32 iSelect);					//���û���߶�
	UINT32	GetSliderWid(INT32 iSelect);					//ȡ�û���߶�
	void	SetSliderOff(INT32 iSelect);					//���û���ƫ��
	UINT32	GetSliderOff(INT32 iSelect);					//ȡ�û���ƫ��

private:
	INT32		ResetData();								//��������
	INT32		Init();										//��ʼ��
	
};
//*/




#endif