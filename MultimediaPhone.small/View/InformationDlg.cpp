// InformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InformationDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"

//#include "windows.h"
//#include <winsock.h>
//#include <iphlpapi.h>
//#include <icmpapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    //检测网络是否连接  news.sina.com.cn  202.108.33.32
    /*BOOL DetectInternet(char *sIP)
    {  
        //return InternetGetConnectedState(&flag,0);  
        // 构造ping数据包
        HANDLE pingHandle_;
        pingHandle_ = IcmpCreateFile();
        if(pingHandle_ == 0)
        {
            return FALSE;
        }

        char acPingBuffer[64];
        memset(acPingBuffer, '*', sizeof(acPingBuffer));
        PICMP_ECHO_REPLY pIpe = (PICMP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(ICMP_ECHO_REPLY) + sizeof(acPingBuffer)); 
        if (pIpe == 0)
        {
            IcmpCloseHandle(pingHandle_);
            return FALSE;
        }

        //将目标字符串IP地址转换成IPAddr结构
        typedef unsigned int IPAddr;
        IPAddr ipAddr = inet_addr((const char *)sIP);      
        if (ipAddr == INADDR_NONE) 
        {
            IcmpCloseHandle(pingHandle_);
            return FALSE;
        }

        // 发送ping数据包
        //	std::string ssIP;
        char ssIP[128];
        sprintf(ssIP, "ping %s with 64 Bytes", sIP);
        std::string m_recv = ssIP;
        for (int i = 0; i < 4; ++i)
        {
            //发送ping服务包，等待接收时间为1秒
            DWORD dwStatus = IcmpSendEcho(pingHandle_, ipAddr, acPingBuffer, sizeof(acPingBuffer), NULL, pIpe, sizeof(ICMP_ECHO_REPLY) + sizeof(acPingBuffer), 1000);
            //当dwStatus不等于0，代表接收到回应
            if (dwStatus != 0)
            {
                IcmpCloseHandle(pingHandle_);
                return TRUE;
            }
        }
        IcmpCloseHandle(pingHandle_);
        return FALSE;
    }*/

    /////////////////////////////////////////////////////////////////////////////
    // CInformationDlg dialog


    CInformationDlg::CInformationDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CInformationDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CInformationDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CInformationDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CInformationDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CInformationDlg, CDialog)
        //{{AFX_MSG_MAP(CInformationDlg)
        //}}AFX_MSG_MAP
        ON_WM_PAINT()
        ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonNoteClose)
        ON_WM_TIMER()
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CInformationDlg message handlers

    BOOL CInformationDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
        str = strTemp.c_str();
        m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 177, 471, 197), this, IDC_BUTTON_NOTE_CLOSE);
        m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        m_sticBack.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(8, 27, 415, 197), this);
        m_sticBack.SetAlign(DT_CENTER);
        m_sticBack.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

        m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
        m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

        //	runstocks();
        m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);
        ts[0].txtRect = CRect(10, 4, 160, 24);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFO).c_str(), Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFO).length());

        m_sticBackground.SetTextStruct(ts, 1, FALSE);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CInformationDlg::OnButtonNoteClose()
    {
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
        stopstocks();
    }

    void CInformationDlg::runstocks()
    {
        std::string strTemp;
        CString str;
        strTemp = Data::LanguageResource::Get(Data::RI_STOCKS_NETCONNECT);
        str = strTemp.c_str();
        m_sticBack.SetWindowText(str);
        ShowWindow(SW_SHOW);
        SetTimer(IDT_SHOWSTOCKS_TIMER, 10, NULL);
    }

    void CInformationDlg::stopstocks()
    {
        HWND stockWnd = ::FindWindow(NULL, _T("HexinName"));
        if(/*m_bStockIsRun*/ stockWnd)
        {
            ::ShowWindow(stockWnd, SW_HIDE);
            CloseHandle(m_stocksprocessinfo.hThread); 
            CloseHandle(m_stocksprocessinfo.hProcess);
            m_bStockIsRun = !m_bStockIsRun;
        } 
    }

    void CInformationDlg ::ShowWindow_(int nCmdShow)
    {
        if(nCmdShow == SW_SHOW)
            runstocks();

        //	HWND stockWnd = ::FindWindow(NULL, _T("HexinName"));
        ShowWindow(nCmdShow);
        //	if(stockWnd)
        //		::ShowWindow(stockWnd, nCmdShow);
        if(nCmdShow == SW_HIDE)
        {
            stopstocks();
        }
    }

    BOOL CInformationDlg ::PreTranslateMessage(MSG* pMsg)
    {
        return CDialog::PreTranslateMessage(pMsg);
    }


    void CInformationDlg::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting
        // TODO: Add your message handler code here

        // Do not call CDialog::OnPaint() for painting messages
    }

    void CInformationDlg::OnTimer(UINT nId)
    {
        if(nId == IDT_SHOWSTOCKS_TIMER)
        {	
            KillTimer(IDT_SHOWSTOCKS_TIMER);
            BOOL isNetConnect = TRUE; //DetectInternet("202.108.33.32");
            if(isNetConnect)
            {
                //Dprintf("net connect!\n");
                HWND stockWnd = ::FindWindow(NULL, _T("HexinName"));
                if(/*(!m_bStockIsRun) && */stockWnd == NULL)
                {	
                    LPCTSTR   filename=_T("C:/flashdrv/my_stocks/HexinCE.exe"); 

                    //打开新进程 
                    BOOL ret = CreateProcess(filename,0,0,0,0,CREATE_NEW_CONSOLE,0,0,0, &m_stocksprocessinfo); 
                    if(ret)
                        m_bStockIsRun = !m_bStockIsRun;
                    stockWnd = ::FindWindow(NULL, _T("HexinName"));
                }
                ::ShowWindow(stockWnd, SW_SHOW);
            }
            else
            {
                //Dprintf("no net connect!\n");
                stopstocks();
                std::string strTemp;
                CString str;
                strTemp = Data::LanguageResource::Get(Data::RI_STOCKS_NETERROR);
                str = strTemp.c_str();
                m_sticBack.SetWindowText(str);
            }	
        }
        CDialog::OnTimer(nId);
    }
}
