// Calculater.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "CalculaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


double WTOF(CString buffer)
{
	char buf[32];
	memset(buf, 0, 32);
	int n = wcstombs( buf, buffer.GetBuffer(30), 30);
	buf[n] = '\0';
	return atof(buf);
}
//�Զ����ֵ
#define SOFTKEY1_BACK	 400
#define m_keynull        490
#define C_KEYV           500
#define MJ_KEYV          510
#define MS_KEYV          520 
#define MR_KEYV          530 
#define MC_KEYV          540 
#define GENHAO_KEYV      550 
#define DAOSHU_KEYV      560 
#define FUHAO_KEYV       570 
#define CHU_KEYV         580 
#define CHENG_KEYV       590 
#define JIAN_KEYV        600 
#define JIA_KEYV         610 
#define DENG_KEYV        620 
#define BAIFENHAO_KEYV   630 
#define BACKSPACE_KEYV	 640
#define CE_KEYV			 650
/////////////////////////////////////////////////////////////////////////////
// CCalculaterDlg dialog


CCalculaterDlg::CCalculaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalculaterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalculaterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCalculaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalculaterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalculaterDlg, CDialog)
	//{{AFX_MSG_MAP(CCalculaterDlg)
	ON_MESSAGE(WM_SOFTKEY, OnSoftKey)
	ON_BN_CLICKED(IDC_BUTTON_CALUEXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCalculaterDlg::OnExit()
{
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
}
/////////////////////////////////////////////////////////////////////////////
// CCalculaterDlg message handlers

BOOL CCalculaterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//2005.3.30��zmy ��ʼ��
	m_number1 = 0;
	m_dtemp0 = 0;
	m_dtemp1 = 0;
	m_dtemp2 = 0;
	m_IsError = FALSE; //2005.5.19 zmy
	CRect rectA[27];

	UINT16 keyV[] = {BACKSPACE_KEYV, CE_KEYV, C_KEYV,\
	MC_KEYV, '7', '8', '9', CHU_KEYV, GENHAO_KEYV,\
	MR_KEYV, '4', '5', '6', CHENG_KEYV, BAIFENHAO_KEYV,\
	MS_KEYV, '1', '2', '3', JIAN_KEYV, DAOSHU_KEYV,\
	MJ_KEYV, '0', FUHAO_KEYV, '.', JIA_KEYV, DENG_KEYV
	};

	rectA[0] = CRect(51, 3, 96, 23);
	rectA[1] = CRect(100, 3, 145, 23);
	rectA[2] = CRect(150, 3, 195, 23);

	unsigned short x_[] = {7, 36, 51, 75, 81, 106, 110, 135, 140, 165, 169, 194};
	unsigned short y_[] = {33, 58, 63, 88, 93, 118, 123, 148};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 6; j++)
		{
			rectA[i*6+j+3] = CRect(x_[j*2], y_[i*2] , x_[j*2+1], y_[i*2+1]);
		}
	CPoint p = CPoint(0, 0);
	CRect rect1 = CRect(1, 46, 205, 46+154);
	m_SoftKey.Create(IDB_BITMAP_CALCULATOR, p, 27, rectA, keyV, rect1, this, WS_VISIBLE|WS_CHILD, IDC_CALCULATOR_SOFTKEY);

	CRect r(2, 17, 205, 43);
	m_DialNumEdit.Create(WS_CHILD|WS_VISIBLE|ES_RIGHT|ES_NUMBER|ES_MULTILINE, r, this, 0xFFFF);
	VERIFY(m_Font.CreateFont(
	   24,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("����")));                 // lpszFacename
	m_DialNumEdit.SetFont(&m_Font,FALSE);   
	m_DialNumEdit.HideCaret();
	m_DialNumEdit.SetLimitText(16);

//	m_DialNumEdit.SetFontSize(24);
//	m_DialNumEdit.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	CString s;
	s = Data::LanguageResource::Get(Data::RI_MAIN_CALCULATORBTN).c_str();
	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(4, 0, 80, 15), this);
	m_stcTitle.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
	m_btnExit.Create(L"", WS_CHILD|WS_VISIBLE, CRect(192, 0, 207, 15), this, IDC_BUTTON_CALUEXIT);
	m_btnExit.SetIcon(IDI_ICON_EXIT, CSize(12, 12));
	m_backGroudStatic.Create(CRect(0, 0, 240, 234), this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//����̵���Ӧ����
void CCalculaterDlg ::OnSoftKey(WPARAM w, LPARAM l)
{
	if (m_IsError && w != C_KEYV)return; //2005.5.19 zmy ���󣬵���κμ����޷�Ӧ		

	//���ּ�������
	if (w < m_keynull)
	{
		if (m_keyvalue < C_KEYV)
		{	
			WCHAR t_buffer[20];
			//CString s;
			int len = m_DialNumEdit.GetWindowText(t_buffer, 20);
			if((len == 0) && (w == '.'))return;
			if((len == 1) && (t_buffer[0] == '0') && (w == '0'))return;
			for (int i = wcslen(t_buffer); i > 0; i --)
				if ((t_buffer[i-1] == '.') && (w == '.')) return;//С����ֻ�ܵ��һ��
		
			if (len < 9)
			{
				CString s;
				m_DialNumEdit.GetWindowText(s);
				if(s == "0")
					s == "";
				s += (char)w;
				m_DialNumEdit.SetWindowText(s);
			}			
		}
		else
		{
			m_DialNumEdit.SetWindowText(L"");
			if(w == '.')return;
			CString s;
			m_DialNumEdit.GetWindowText(s);
			if(s == "0")
			  s == "";
			s += (char)w;
			m_DialNumEdit.SetWindowText(s);
			m_keyvalue1 = m_keyvalue;
			m_keyvalue = m_keynull;
			m_keyvalue2 = m_keynull;
			m_keyvalue3 = m_keynull;
		}
		if (m_keyvalue1 >= CHU_KEYV)
		{
			WCHAR buffer[20];
			memset(buffer, 0, 40);
			m_DialNumEdit.GetWindowText(buffer, 20);
			m_dtemp2 = WTOF(buffer);
		}
	}
	
	//���ż��������Ӧ����
	else
	{
		//2005.3.30-zmy �������Ϊ��ʱ��������ż�������MS�⣩������ʾ
		WCHAR buff[20];
		int len = m_DialNumEdit.GetWindowText(buff, 20);
		if ((len == 0) && (w != MR_KEYV)) return;
		WCHAR buffer[20];
		WCHAR buffer1[20];
		int i;
		int j;
		double temp1;
		double temp2;
		double dtemp1;
		
		switch ( w ) 
		{
			case BACKSPACE_KEYV:	//delete
				{
					CString s;
					m_DialNumEdit.GetWindowText(s);
					int len = s.GetLength();
					if(len > 0)
					{
						s = s.Left(len-1);
						m_DialNumEdit.SetWindowText(s);
					}
				}
				
				break;
			//�������
			case C_KEYV :
				m_DialNumEdit.SetWindowText(L"");
				m_dtemp0 = 0;
				m_keyvalue = m_keynull;
				m_keyvalue1 = m_keynull;
				m_IsError = FALSE;  //2005.5.19 zmy ���ִ���ʱ��ֻ�ܵ�����������������Ч
				if (m_keyvalue2 == CHU_KEYV || m_keyvalue2 == CHENG_KEYV || m_keyvalue2 == JIAN_KEYV \
					||m_keyvalue2 == JIA_KEYV || m_keyvalue2 == DENG_KEYV)
				{
					m_keyvalue2 = m_keynull;
				}
				break;

			case CE_KEYV :
				//ClearData();
				m_DialNumEdit.SetWindowText(L"");
			//	m_dtemp0 = 0;
				/*
				m_keyvalue = m_keynull;
				m_keyvalue1 = m_keynull;
				m_keyvalue2 = m_keynull;
				*/
				break;

			//��ǰ���ֺʹ洢���е�������Ӳ��浽�洢����
			case MJ_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				dtemp1 = WTOF(buffer);
				m_number1 = m_number1 + dtemp1;
				m_keyvalue = MJ_KEYV;
				break;

			//��ǰ���ִ洢���洢���в��滻��ԭ���洢���е�ֵ
			case MS_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				m_number1 = WTOF(buffer);
				m_keyvalue = MS_KEYV;
				break;

			//���ô洢���е���ֵ
			case MR_KEYV :
				m_DialNumEdit.SetWindowText(L"");
				wsprintf(buffer, _T("%16.16f"), m_number1);
				for (i =  wcslen(buffer); i > 0; i --)
					if (buffer[i-1] != '0')break;
				if (buffer[i-1] == '.')
				{
					i = i - 1;
				}
				memcpy(buffer1, buffer, i*2);
				buffer1[i] = '\0';
				m_DialNumEdit.SetWindowText(buffer1);
				m_keyvalue = MR_KEYV;
				break;

			//�Ѵ洢���е���ֵ�������Ϊ��
			case MC_KEYV :
				m_number1 = 0;
				m_keyvalue = MC_KEYV;
				break;

			//��ǰ����ȡ����ֵ
			case GENHAO_KEYV :
				
				m_DialNumEdit.GetWindowText(buffer, 20);
				temp1 = WTOF(buffer);
				if(temp1 >= 0)
				{
					temp2 = sqrt(temp1) ;		//lxz 2004.9.5			
					wsprintf(buffer, _T("%16.16f"), temp2);				
					for (i =  wcslen(buffer); i > 0; i --)
						if (buffer[i-1] != '0')break;
					if (buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);
				}
				else 
				{
					//m_DialNumEdit.SetWindowText("����������Ч!");
					m_DialNumEdit.SetWindowText(L"Error");
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
				m_keyvalue = GENHAO_KEYV;
				break;

			//��ǰ����ȡ����ֵ
			case DAOSHU_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				temp1 = WTOF(buffer);
				if (temp1 != 0)
				{
					temp2 = 1 / temp1;
					//sprintf_(buffer, "%.12f", temp2);
					wsprintf(buffer, _T("%16.16f"), temp2);
					for (i =  wcslen(buffer); i > 0; i --)
					if (buffer[i-1] != '0')break;
					if (buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);
				}
				else 
				{
					//strcpy(buffer1, "��������Ϊ��");
					m_DialNumEdit.SetWindowText(L"Error");
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
					
				m_keyvalue = DAOSHU_KEYV;
				break;

			//��ǰ����ȡ����
			case FUHAO_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				temp1 = WTOF(buffer);
				temp2 = -temp1 ;
				//sprintf_(buffer, "%.12f", temp2);
				wsprintf(buffer, _T("%16.16f"), temp2);
				for (i =  wcslen(buffer); i > 0; i --)
					if (buffer[i-1] != '0')break;
				if (buffer[i-1] == '.')
				{
					i = i - 1;
				}
				memcpy(buffer1, buffer, i*2);
				buffer1[i] = '\0';	
				m_DialNumEdit.SetWindowText(buffer1);
				if (m_keyvalue != DAOSHU_KEYV && m_keyvalue != GENHAO_KEYV)
				{
					j =  wcslen(buffer1);
					m_keyvalue = m_keynull;
				}
				else m_keyvalue = FUHAO_KEYV;				
				break;

			//���ٷֱȵ���ʽ��ʾ�˻����
			case BAIFENHAO_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);				
				dtemp1 = WTOF(buffer);
				dtemp1 = m_dtemp0 * dtemp1 / 100;
				//sprintf_(buffer, "%.12f", dtemp1);				
				wsprintf(buffer, _T("%16.16f"), dtemp1);				
				for (i =  wcslen(buffer); i > 0; i --)
					if (buffer[i-1] != '0')break;
				if (buffer[i-1] == '.')
				{
					i = i - 1;
				}
				memcpy(buffer1, buffer, i*2);
				buffer1[i] = '\0';
				m_DialNumEdit.SetWindowText(buffer1);
				m_keyvalue = BAIFENHAO_KEYV;
				break;

			//����
			case CHU_KEYV :				
				m_DialNumEdit.GetWindowText(buffer, 20);			
				dtemp1 = WTOF(buffer);
				if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
				{
					if (m_keyvalue2 == m_keynull)
					{
						switch ( m_keyvalue1 )
						{
							case CHU_KEYV :
								dtemp1 = m_dtemp0 / dtemp1;
								break;
							case CHENG_KEYV :
								dtemp1 = m_dtemp0 * dtemp1;
								break;
							case JIAN_KEYV :
								dtemp1 = m_dtemp0 - dtemp1;
								break;
							case JIA_KEYV :
								dtemp1 = m_dtemp0 + dtemp1;
								break;
						}
					}
					//2005.1.24-zmy
					if ( dtemp1 >= 99999999999999999 )
					{
						m_DialNumEdit.SetWindowText(L"Error");
						m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
						m_dtemp0 = 0;
						m_keyvalue = m_keynull;
						m_keyvalue1 = m_keynull;
						m_keyvalue2 = m_keynull;
						return;
					}
					//if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

					wsprintf(buffer, _T("%16.16f"), dtemp1);
					for (i =  wcslen(buffer); i > 0; i --)
						if (buffer[i-1] != '0')break;
					if (buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);		
				}
				else if (m_keyvalue2 == m_keynull)
				{
					//strcpy(buffer1, "��������Ϊ��");
					m_DialNumEdit.SetWindowText(L"Error");
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
				
				m_dtemp0 = dtemp1;
				m_dtemp1 = dtemp1;
				m_keyvalue = CHU_KEYV;
				m_keyvalue2 = CHU_KEYV;
				m_keyvalue3 = CHU_KEYV;
				break;

			//�˺�
			case CHENG_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				dtemp1 = WTOF(buffer);
				if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
				{
					if (m_keyvalue2 == m_keynull)
					{					
						switch ( m_keyvalue1 )
						{
							case CHU_KEYV :
								dtemp1 = m_dtemp0 / dtemp1;
								break;
							case CHENG_KEYV :
								dtemp1 = m_dtemp0 * dtemp1;
								break;
							case JIAN_KEYV :
								dtemp1 = m_dtemp0 - dtemp1;
								break;
							case JIA_KEYV :
								dtemp1 = m_dtemp0 + dtemp1;
								break;
						}
					}
					//2005.1.24-zmy
					if ( dtemp1 >= 99999999999999999 )
					{
						m_DialNumEdit.SetWindowText(L"Error");
						m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
						m_dtemp0 = 0;
						m_keyvalue = m_keynull;
						m_keyvalue1 = m_keynull;
						m_keyvalue2 = m_keynull;
						return;
					}
					//if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

					wsprintf(buffer, _T("%16.16f"), dtemp1);
					for (i =  wcslen(buffer); i > 0; i --)
						if (buffer[i-1] != '0')break;
					if(buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);		
				}
				else if (m_keyvalue2 == m_keynull)
				{
					//strcpy(buffer1, "��������Ϊ��");
					m_DialNumEdit.SetWindowText(L"Error");		
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
				
				m_dtemp0 = dtemp1;
				m_dtemp1 = dtemp1;
				m_keyvalue = CHENG_KEYV;
				m_keyvalue2 = CHENG_KEYV;
				m_keyvalue3 = CHENG_KEYV;
				break;

			//����
			case JIAN_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				dtemp1 = WTOF(buffer);
				if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
				{
					if (m_keyvalue2 == m_keynull)
					{
						switch ( m_keyvalue1 )
						{
							case CHU_KEYV :
								dtemp1 = m_dtemp0 / dtemp1;
								break;
							case CHENG_KEYV :
								dtemp1 = m_dtemp0 * dtemp1;
								break;
							case JIAN_KEYV :
								dtemp1 = m_dtemp0 - dtemp1;
								break;
							case JIA_KEYV :
								dtemp1 = m_dtemp0 + dtemp1;
								break;
						}
					}
					//2005.1.24-zmy
					if ( dtemp1 >= 99999999999999999 )
					{
						m_DialNumEdit.SetWindowText(L"Error");
						m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
						m_dtemp0 = 0;
						m_keyvalue = m_keynull;
						m_keyvalue1 = m_keynull;
						m_keyvalue2 = m_keynull;
						return;
					}
					//if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

					wsprintf(buffer, _T("%16.16f"), dtemp1);
					for (i =  wcslen(buffer); i > 0; i --)
						if (buffer[i-1] != '0')break;
					if (buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);	
				}
				else if (m_keyvalue2 == m_keynull)
				{
					//strcpy(buffer1, "��������Ϊ��");					
					m_DialNumEdit.SetWindowText(L"Error");		
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
				
				m_dtemp0 = dtemp1;
				m_dtemp1 = dtemp1;
				m_keyvalue = JIAN_KEYV;
				m_keyvalue2 = JIAN_KEYV;
				m_keyvalue3 = JIAN_KEYV;
				break;

			//�Ӻ�
			case JIA_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				dtemp1 = WTOF(buffer);
				if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
				{
					if (m_keyvalue2 == m_keynull)
					{
						switch ( m_keyvalue1 )
						{
							case CHU_KEYV :
								dtemp1 = m_dtemp0 / dtemp1;
								break;
							case CHENG_KEYV :
								dtemp1 = m_dtemp0 * dtemp1;
								break;
							case JIAN_KEYV :
								dtemp1 = m_dtemp0 - dtemp1;
								break;
							case JIA_KEYV :
								dtemp1 = m_dtemp0 + dtemp1;
								break;
						}
					}
					//2005.1.24-zmy
					if ( dtemp1 >= 99999999999999999 )
					{
						m_DialNumEdit.SetWindowText(L"Error");
						m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
						m_dtemp0 = 0;
						m_keyvalue = m_keynull;
						m_keyvalue1 = m_keynull;
						m_keyvalue2 = m_keynull;
						return;
					}
					//if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

					wsprintf(buffer, _T("%16.16f"), dtemp1);
					for (i =  wcslen(buffer); i > 0; i --)
						if (buffer[i-1] != '0')break;
					if (buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);	

				}
				else if (m_keyvalue2 == m_keynull)
				{
					//strcpy(buffer1, "��������Ϊ��");
					m_DialNumEdit.SetWindowText(L"Error");		
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
				
				m_dtemp0 = dtemp1;
				m_dtemp1 = dtemp1;
				m_keyvalue = JIA_KEYV;
				m_keyvalue2 = JIA_KEYV;
				m_keyvalue3 = JIA_KEYV;
				break;

			//�Ⱥ�
			case DENG_KEYV :
				m_DialNumEdit.GetWindowText(buffer, 20);
				dtemp1 = WTOF(buffer);
				if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
				{
					switch ( m_keyvalue2 )	
					{
						//������Ⱥ�֮�������������ʱ
						case m_keynull :
							switch ( m_keyvalue1 )
							{
								case m_keynull :
									dtemp1 = dtemp1;
									break;
								case CHU_KEYV :
									dtemp1 = m_dtemp0 / dtemp1;
									break;
								case CHENG_KEYV :
									dtemp1 = m_dtemp0 * dtemp1;
									break;
								case JIAN_KEYV :
									dtemp1 = m_dtemp0 - dtemp1;
									break;
								case JIA_KEYV :
									dtemp1 = m_dtemp0 + dtemp1;
									break;
							}				
							break;
						//�Ⱥ�ǰ���������Ϊ��ʱ
						case DENG_KEYV :
							switch ( m_keyvalue3 )
							{
								case m_keynull :						
									switch ( m_keyvalue1 )
									{
										case m_keynull :
											dtemp1 = dtemp1;
											break;
										case CHU_KEYV :
											dtemp1 = m_dtemp2;
											dtemp1 = m_dtemp0 / dtemp1;
											break;
										case CHENG_KEYV :
											dtemp1 = m_dtemp2;
											dtemp1 = m_dtemp0 * dtemp1;
											break;
										case JIAN_KEYV :
											dtemp1 = m_dtemp2;
											dtemp1 = m_dtemp0 - dtemp1;
											break;
										case JIA_KEYV :
											dtemp1 = m_dtemp2;
											dtemp1 = m_dtemp0 + dtemp1;										
											break;
									}
									break;
								case CHU_KEYV :
									dtemp1 = m_dtemp1;
									dtemp1 = m_dtemp0 / dtemp1;
									break;
								case CHENG_KEYV :
									dtemp1 = m_dtemp1;
									dtemp1 = m_dtemp0 * dtemp1;
									break;
								case JIAN_KEYV :
									dtemp1 = m_dtemp1;
									dtemp1 = m_dtemp0 - dtemp1;
									break;
								case JIA_KEYV :
									dtemp1 = m_dtemp1;
									dtemp1 = m_dtemp0 + dtemp1;
									break;		
							}
							break;
						//�Ⱥ�ǰ���������Ϊ��ʱ
						case CHU_KEYV :
							dtemp1 = m_dtemp0 / dtemp1;
							break;
						//�Ⱥ�ǰ���������Ϊ��ʱ
						case CHENG_KEYV :
							dtemp1 = m_dtemp0 * dtemp1;
							break;
						//�Ⱥ�ǰ���������Ϊ��ʱ
						case JIAN_KEYV :
							dtemp1 = m_dtemp0 - dtemp1;
							break;
						//�Ⱥ�ǰ���������Ϊ�ӵ�ʱ
						case JIA_KEYV :
							dtemp1 = m_dtemp0 + dtemp1;
							break;
					}
					//2005.1.24-zmy
					if ( dtemp1 >= 99999999999999999 )
					{
						m_DialNumEdit.SetWindowText(L"Error");
						m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
						m_dtemp0 = 0;
						m_keyvalue = m_keynull;
						m_keyvalue1 = m_keynull;
						m_keyvalue2 = m_keynull;
						return;
					}
					//if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

					wsprintf(buffer, _T("%16.16f"), dtemp1);
					for (i =  wcslen(buffer); i > 0; i --)
						if (buffer[i-1] != '0') break;
					if (buffer[i-1] == '.')
					{
						i = i - 1;
					}
					memcpy(buffer1, buffer, i*2);
					buffer1[i] = '\0';
					m_DialNumEdit.SetWindowText(buffer1);								
				}
				else if (m_keyvalue2 == m_keynull)
				{
					//strcpy(buffer1, "��������Ϊ��");
					m_DialNumEdit.SetWindowText(L"Error");	
					m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
				}
				
				m_dtemp0 = dtemp1 ;
				m_keyvalue = DENG_KEYV;
				m_keyvalue2 = DENG_KEYV;
				break;
				
		}	
	}
}

void CCalculaterDlg::ClearData()
{
	m_DialNumEdit.SetWindowText(L"");
	m_dtemp0 = 0;
	m_dtemp1 = 0;
	m_dtemp2 = 0;
	m_number1 = 0;
	m_IsError = FALSE;//2005.5.19 zmy 
}
