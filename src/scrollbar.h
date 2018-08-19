/******************************************************************************/
/*                                                                            */
/*  ��Ȩ����(c)  2001�����Ƽ�                                                 */
/*                                                                            */
/******************************************************************************/
/*  �� �� ��                                                                  */
/*             scrollbar.h                                                    */
/*                                                                            */
/*  �� �� ��                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �����Ƽ�E-PhoneII��������ͷ�ļ�                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �������ؼ���ͷ�ļ�                                             */
/*                                                                            */
/*  ��    ��                                                                  */
/*             ����                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             2001-07-02                                                     */
/*                                                                            */
/*  ��ʷ��¼                                                                  */
/******************************************************************************/
#ifndef __E201_SCROLLBAR_H_
#define __E201_SCROLLBAR_H_
#include "hwfcl.h" 

////////////////////////////////////////////////////////////////////////////////
//
#define SB_WIDTH           16        //�������Ŀ��
#define SB_SLED_MINHEIGHT  14        //�������С�߶�
#define MOVE_FILTER_NUM    5        // hqf 2002/1/7 ���ƶ�ʱ��ÿ���ٸ����һ��
/*============================================================================*/
//  CScrollBar��Ķ���                                                        //
/*============================================================================*/
class CScrollBar : public CCtrl
{
protected:
    UINT8   m_iActive;              //�ϴ�ѡ�е�
    CCtrl * m_pBuddyCtrl;           //���������صĿؼ�
    UINT16  m_iRows;                //������
	UINT16  m_iCurRow;              //��ǰ��ʾ���к�
    UINT16  m_iScreenRows;          //��ؿؼ���Ļ����
    CRect   m_sUpRect;              //�ϼ�ͷ��ť����
    CRect   m_sDownRect;            //�¼�ͷ��ť����
    CRect   m_sSledRect;            //��������
    UINT16  m_iLastPenY;            

////////////////////////////////////////////////////////////////////////////////
// ˽�й���
protected:
	void DoPenDown(CPoint &pt);
    void DoPenMove(CPoint &pt);
    void DoPenUp();
	    
    void DoUpActive();
    void DoDownActive();
    void DoOtherUpActive();
    void DoOtherDownActive();
    void DoSledMove(UINT16 iPenY);

    void PaintUpDownBut(CDC &dc, CRect &scrRect, BOOL bUp=TRUE);
    void PaintSled(CDC &dc);

    void CalcSledPos();
    void NotifyBuddy();

////////////////////////////////////////////////////////////////////////////////
// �麯��
public:
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);

////////////////////////////////////////////////////////////////////////////////
// �ӿں���
// ������ʹ�÷���˵��
//     1.��������һ���ؼ���������ĳ����CFrameWnd��������Ϊ������
//     2.�������������Զ���ɹ�������ʾ����
//     3.������ʹ��CM_SCROOLTO��Ϣ֪ͨ�����ڻ��������
// ʹ�þ���
//     ��ĳ��CFrameWnd�����Ĵ������ж����Ա����CScrollBar m_oSb;
//     �ڴ�������麯��OnCreate�д���������
//     m_oSb.Create(ptLeftTop, iHeight, this, pBuddyCtrl, ID_SCRBAR);
//     ������ز���
//     m_oSb.SetPerameter(100, 10, 0);
// ˵��
//     ���������������Ҫ�ڸ������д��������Ϣ���ڴ���ʱ�����pBuddyCtrl����ָ��ΪNULL
//     �ڸ����ڵ���Ϣӳ���������
//     ON_MESSAGE(id, CN_SCROLLTO, OnScroll)
//     ��������Ϣ������ void OnScroll(UINT32, UINT32, UINT32);
public:
    ////////////////////////////////////////////////////////////////////////////
	//����
	//      ����������
	//����
	//       ptLeftTop             ���������Ͻ�����
	//       iHeight               �������ĸ߶�
	//       pParent               ������
	//       pBuddyCtrl            ���������صĿؼ�
	//       iScrollId             �ؼ�ID
	//����
	//       �����Ƿ�ɹ�
	//˵��
	//       ����������CN_SCROLLTO��Ϣ�����pBuddyCtrlΪ�գ���Ѵ���Ϣ���͵�������
	//       �����͵�pBuddyCtrl��
    virtual BOOL Create(CPoint &ptLeftTop, UINT16 iHeight, CFrameWnd * pParent, CCtrl *pBuddyCtrl=NULL, UINT32 iScrollId=0);


	////////////////////////////////////////////////////////////////////////////
	//����
	//       ���ù���������
	//����
	//       iRows                ���������ܹ������ܵ�����
	//       iScreenRows          �����������Ķ�����Ļ������ʾ������
	//       iCurRow              ���������鵱ǰλ�ñ�ʾ���к�
	//       bReDraw              ����������Ƿ��ػ������
	//˵��
	//       �����������Ķ�������������������������������Ĵ�С��λ��Ҳ��������������
	//       �������ĸ߶�ȷ�������л���Ĵ�СΪ iScreenRows/iRows * �������Ŀͻ����߶�
	//       �����λ��Ϊ (�������ͻ����߶�-����߶�)*iCurRow/iRows
    void SetPerameter(UINT16 iRows, UINT16 iScreenRows, UINT16 iCurRow, BOOL bReDraw = FALSE);

	////////////////////////////////////////////////////////////////////////////
	//����
	//       ��ù�������صĲ���
	void GetPerameter(UINT16 &iRows, UINT16 &iScreenRows, UINT16 &iCurRow);

	void Moveto(INT16 x, INT16 y);
};
#endif
////////////////////////////////////////////////////////////////////////////////
// end menu.h 