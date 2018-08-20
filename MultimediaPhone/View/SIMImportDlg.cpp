// SIMImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIMImportDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIMImportDlg dialog

namespace View {
    CSIMImportDlg::CSIMImportDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CSIMImportDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CSIMImportDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_bIsSIM = FALSE; 
    }


    void CSIMImportDlg::DoDataExchange(CDataExchange* pDX)
    {
        CCEDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSIMImportDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CSIMImportDlg, CCEDialog)
        //{{AFX_MSG_MAP(CSIMImportDlg)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    LRESULT CSIMImportDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 2:
            if(!m_bIsSIM)
            {
                m_MJPGList.SetUnitIsShow(3, FALSE);
                m_MJPGList.SetUnitIsShow(0, TRUE);

                m_MJPGList.SetUnitIsDisable(1, TRUE);
                m_MJPGList.SetUnitIsDownStatus(1, TRUE);
                m_MJPGList.SetUnitIsShow(1, TRUE);

                m_bIsSIM = TRUE;
                InitSIM();
            }
            break;
        case 1:
            break;
        case 3:
            gCmdIndex = 1000;
            KillTimer(101);
            KillTimer(102);

            if(m_bIsSIM)
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->simdevice_->SetExitTread();
                if(!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->simdevice_->Reset())
                    m_MJPGList.SetUnitText(2, _T("出错或卡不存在"), TRUE);
            }

            m_procbar.SetPos(0);
            ShowWindow(SW_HIDE);
            m_MJPGList.SetUnitText(2, _T(""), FALSE);
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
            }
            m_MJPGList.SetUnitIsDisable(1, FALSE);
            m_MJPGList.SetUnitIsDownStatus(1, FALSE);
            m_MJPGList.SetUnitIsShow(1, TRUE);
            m_bIsSIM = FALSE;
            break;
        case 4:
            OnClickMJPG(3, 0);
            m_MJPGList.SetUnitIsShow(3, FALSE);
            m_MJPGList.SetUnitIsShow(0, TRUE);
            break;
        }
        return result;
    }
    /////////////////////////////////////////////////////////////////////////////
    // CSIMImportDlg message handlers
#define IDC_STATIC_STATUS			1120
#define IDC_EDIT1					1121
    BOOL CSIMImportDlg::OnInitDialog() 
    {
        CCEDialog::OnInitDialog();

        // TODO: Add extra initialization here

        m_Edit1.Create(WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL, CRect(0, 250, 800 * 125 / 100, 400), this, IDC_EDIT1);
        m_Static1.Create(L"", WS_CHILD, CRect(180, 240 , 200+50, 270), this, IDC_STATIC_STATUS);
        //m_Static1.SetFontSize(18);
        m_nSIMID = -1;
        /*
        std::vector<Util::shared_ptr<Data::ContactGroup> > result;
        if (Data::ContactGroup::GetDataCount("") > 0)
        {
        result = Data::ContactGroup::GetFromDatabase("");
        for (int i = 0; i < result.size(); ++i)
        {
        if(CString(result[i]->name().c_str()) == L"SIM卡")
        {
        m_nSIMID = result[i]->id();
        break;
        }
        }
        }
        */

        /*	m_cmbGroup.Create(WS_CHILD|WS_VISIBLE, CRect(54+206, 62+114, 260+254, 176+40), this, IDC_COMBOBOX_CONTACTNEW_GROUP);

        m_cmbGroup.ResetContent();
        std::vector<Util::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("");
        for (int i = 0; i < result.size(); ++i)
        {
        m_cmbGroup.AddString(CString(result[i]->name().c_str()));
        m_cmbGroup.SetItemData(i, result[i]->id());
        }
        m_cmbGroup.SetCurSel(0);
        */

        m_procbar.Create(WS_CHILD|WS_VISIBLE, CRect(54+203, 62+180 , 54+459, 62+205), this, IDC_PBAR_SOUNDDLG_PROCESS);
        m_procbar.ShowWindow(SW_HIDE);
        //	m_procbarSound.SetParam(0, 0, 50, 1);
        m_procbar.SetPos(0);

        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\导入到类别.xml"));
        m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));
        m_MJPGList.SetUnitText(2, L"", FALSE);
        m_MJPGList.SetUnitIsShow(3, FALSE);

        m_pContact = Util::shared_ptr<Data::Contact>(new Data::Contact);	

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    LRESULT CSIMImportDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        if(message == WM_SIMDATA_OK)
        {
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->CancelBalckLightSaveTimer();

            //Dprintf("Recv OK\n");
            KillTimer(101);

            CString ss;
            ss.Format(_T("No %03d: "), gIndex);
            gEditList += ss;
            int len = wParam;
            if(len == 0x1B)
            {
                int i;
                i++;
            }
            BYTE *ptr = (BYTE *)lParam;
            for(int i = 0; i < len; i++)
            {
                CString ss;
                ss.Format(_T("%02x "), *(ptr+i));
                gEditList += ss;
            }
            gEditList += "\r\n";
            SetDlgItemText(IDC_EDIT1, gEditList);
            CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
            pEdit->LineScroll(pEdit->GetLineCount());

            //判断卡是否被锁住

            if(gCmdIndex == 4)
            {
                /*	UINT8 chstatus = *(ptr+19);
                if(chstatus & 0x80)
                {
                m_MJPGList.SetUnitText(2, "卡被锁住", TRUE);
                //	return CCEDialog::WindowProc(message, wParam, lParam);
                }
                */
            }


            //计算有多少个记录
            if(gCmdIndex == 6)
            {
                //	int cc = (*(ptr+2) << 8 | *(ptr+3));
                int cc = (*(ptr+3) << 8 | *(ptr+4));

                int rr = *(ptr+15);
                m_nRecordeSize = rr;
                if(rr != 0)
                    gRecordCount = cc/rr;
                m_procbar.SetParam(0, 0, gRecordCount, 1);
            }
            //解析电话号码本
            if(gCmdIndex > 6)
            {
                if(len <= 2)
                {
                    m_MJPGList.SetUnitText(2, _T("卡无法读取"), TRUE);
                    return CCEDialog::WindowProc(message, wParam, lParam);
                }

                WCHAR name[32];
                memset(name, 0, 64);
                BYTE *pName = (BYTE *)name;
                char cName[64];
                memset(cName, 0, 64);

                BYTE *pB;
                pB = ptr+2;
                int nn = 0;

                if((*(ptr+1) == 0x80))  //UCS  
                {
                    while(*pB != 0xFF)
                    {
                        *pName++ = *(pB+1);
                        *pName++ = *pB;
                        pB += 2;
                        nn += 2;
                    }
                    WideCharToMultiByte(CP_ACP, NULL, name, nn/2, cName, nn, NULL, NULL);
                }
                else if(*(ptr+1) == 0x81)
                {
                    int length = *pB;
                    pB += 1;
                    unsigned short base = ((*pB << 7) & 0x7F80);
                    pB += 1;

                    for(int i = 0; i < length; i++)
                    {
                        unsigned short code = (base | (*pB & 0x7F));
                        name[i] = code;
                    }
                    WideCharToMultiByte(CP_ACP, NULL, name, length, cName, length*2, NULL, NULL);
                }
                else if(*(ptr+1) == 0x82)
                {
                    int length = *pB;
                    pB += 1;
                    unsigned short base = (*pB << 8) | (*(pB+1));
                    pB += 2;

                    for(int i = 0; i < length; i++)
                    {
                        unsigned short code = (base | (*pB & 0x7F));
                        name[i] = code;
                    }
                    WideCharToMultiByte(CP_ACP, NULL, name, length, cName, length*2, NULL, NULL);
                }
                else
                {
                    while(*pB != 0xFF)
                    {
                        cName[nn++] = *pB;
                        pB += 1;
                    }
                }
                gEditList += cName;

                gEditList += "     ";

                int telLength = *(ptr+15);  //
                char telcode[32];
                memset(telcode, 0, 32);
                pB= ptr+17;                 //
                for(int i = 0; i < telLength*2; i++)
                {
                    BYTE c = *(pB+i/2);
                    if(i%2 == 0)
                    {
                        c = (c&0x0F);
                        if(c < 10)
                            telcode[i] = c + 0x30;
                        else
                            break;
                    }
                    else
                    {
                        c = ((c>>4)&0x0F);
                        if(c < 10)
                            telcode[i] = c + 0x30;
                        else
                            break;
                    }
                }
                gEditList += telcode;
                gEditList += "\r\n";

                static int ggcount = 0;
                ggcount++;
                if(strlen(telcode) > 0)
                {
                    TRACE(L"%d ", ggcount);

                    CString sName_ = Util::StringOp::ToCString(cName);
                    CString sCode_ = Util::StringOp::ToCString(telcode);
                    WriteSIMTelPhoneBOOK(sName_, sCode_);
                }
                else
                {
                    TRACE(L"%d ", ggcount);
                }
            }

            m_procbar.SetPos(gIndex);

            CString s_;
            if(gIndex <= gRecordCount || gRecordCount == 0)
                s_.Format(_T("%d/%d %d"), gIndex, gRecordCount, gValidRecordCount);
            else
            {
                OnClickMJPG(3, 0);
                return 0;
            }

            m_MJPGList.SetUnitText(2, s_, TRUE);

            //SendCMDSIM(gCmdIndex++);
            //PostMessage(WM_SENDSIMCMD, 0, 0);
            SetTimer(102, 30, NULL);
        }
        else if(message == WM_SIMDATA_ERROR)
        {
            //Dprintf("WM_SIMDATA_ERROR\r\n");
            KillTimer(101);
            KillTimer(102);
            gCmdIndex = 1000;

            m_MJPGList.SetUnitText(2, _T("传输出错"), TRUE);

            /*
            gEditList += "crc error: ";
            int len = wParam;
            BYTE *ptr = (BYTE *)lParam;
            for(int i = 0; i < len; i++)
            {
            CString ss;
            ss.Format(_T("%02x "), *(ptr+i));
            gEditList += ss;
            }
            gEditList += "\r\n";
            SetDlgItemText(IDC_EDIT1, gEditList);
            CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
            pEdit->LineScroll(pEdit->GetLineCount());
            */
        }
        else if(message == WM_SENDSIMCMD)
        {
            //	SendCMDSIM(gCmdIndex++);
        }
        return CCEDialog::WindowProc(message, wParam, lParam);
    }

    static int gReCount = 0;
    void CSIMImportDlg::InitSIM() 
    {
        // TODO: Add your control notification handler code here

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->KillScreenSaveTimer();    //取消屏保

        m_nSIMID = -1;
        std::vector<Util::shared_ptr<Data::ContactGroup> > result;
        if (Data::ContactGroup::GetDataCount("") > 0)
        {
            result = Data::ContactGroup::GetFromDatabase("");
            for (std::size_t i = 0; i < result.size(); ++i)
            {
                if (CString(result[i]->name().c_str()) == L"SIM卡")
                {
                    m_nSIMID = result[i]->id();
                    break;
                }
            }
        }

        if(m_nSIMID == -1)
        {
            //创建SIM卡类别
            //MessageBox(L"缺少SIM卡类别，请建一个SIM卡类别!");
            //return;
            Util::shared_ptr<Data::ContactGroup> m_pContactGroup = Util::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
            m_pContactGroup->name("SIM卡");
            m_pContactGroup->ringName("默认");
            m_pContactGroup->tipName("默认");
            m_pContactGroup->Insert();
            m_nSIMID = m_pContactGroup->id();
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
        }

        gReCount = 0;
        std::vector<Util::shared_ptr<Data::Contact> > result1 = Data::Contact::GetFromDatabase("groupId = " + Util::StringOp::FromInt(m_nSIMID));
        if (!result1.empty())
        {
            for (std::size_t i = 0; i < result1.size(); ++i)
                result1[i]->Remove();
        }

        m_procbar.SetPos(0);
        m_procbar.ShowWindow(SW_SHOW);

        //	m_Static1.ShowWindow(SW_SHOW);
        m_MJPGList.SetUnitText(2, _T("开始导入"), TRUE);

        gEditList = "";
        gIndex = 0;
        gCmdIndex = 0;
        gRecordCount = 0;
        gValidRecordCount = 0;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->simdevice_->SetOwner(this);
        SetTimer(103, 20, NULL);
    }


    BYTE gSimSelectDF[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x7F, 0x10};
    BYTE gSimSelectEF[] = {0xA0, 0xA4, 0x00, 0x00, 0x02, 0x6F, 0x3A};
    BYTE gSimResponse[] = {0xA0, 0xC0, 0x00, 0x00};
    BYTE gSimReadRecorde[] = {0xA0, 0xB2, 0x0, 0x02, 0x1C};

    unsigned char cData[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    void convertdata(BYTE *des, BYTE *Src, int len, BOOL MSB = FALSE)
    {
        /*
        BYTE *ptr = des;
        for(int i = 0; i < len; i++)
        {
        *ptr++ = cData[(Src[i]>>4)&0xF];
        *ptr++ = cData[Src[i]&0xF];
        }
        */
    }

    int SIMWrite(BYTE *des, BYTE *src, int len)
    {
        /*
        BYTE ptr[128];
        ptr[0] = 0x55;
        ptr[1] = 3+len+1;
        ptr[2] = 0x41;
        ptr[3] = 0X00;
        ptr[4] = 0x00;
        memcpy(ptr+5, src, len);
        int ret = 0;
        int i = 0;
        for(i = 1; i <= ptr[1]; i++)
        ret += ptr[i];
        ptr[i] = ret%256;

        convertdata(des, ptr, ptr[1] +2);
        return ptr[1] +2;
        */

        memcpy(des, src, len);
        return len;
    }

    void CSIMImportDlg::SendCMDSIM(int nIndex)
    {
        //	if(!IsWindowVisible())
        //		return;
        if(nIndex >= 1000)
            return;

        //Dprintf("SendCMDSIM %d\r\n", nIndex);

        BYTE ptr[64];
        memset(ptr, 0, 64);
        int crclen;
        BYTE des[128];
        memset(des, 0, 128);
        int len = 0;
        int nRecLen;
        switch(nIndex)
        {
        case 0:				//设置卡座
            {
                /*
                ptr[0] = 0x55;
                ptr[1] = 0x03;
                ptr[2] = 0x54;
                ptr[3] = 0x0C;
                crclen = 3;
                int ret = 0;
                int i = 0;
                for(i = 1; i <= crclen; i++)
                ret += ptr[i];
                ptr[i] = ret%256;
                convertdata(des, ptr, i+1, TRUE);
                */
                if(!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->simdevice_->Reset())
                {
                    m_MJPGList.SetUnitIsDisable(1, FALSE);
                    m_MJPGList.SetUnitIsDownStatus(1, FALSE);
                    m_MJPGList.SetUnitIsShow(1, TRUE);
                    m_bIsSIM = FALSE;

                    m_MJPGList.SetUnitText(2, _T("出错或卡不存在"), TRUE);
                    gCmdIndex = 1000;
                }
                return;
            }
            break;
        case 1:			   //复位卡
            {
                /*
                ptr[0] = 0x55;
                ptr[1] = 0x03;
                ptr[2] = 0x43;
                ptr[3] = 0x00;
                crclen = 3;
                int ret = 0;
                int i = 0;
                for(i = 1; i <= crclen; i++)
                ret += ptr[i];
                ptr[i] = ret%256;
                convertdata(des, ptr, i+1, TRUE);
                */
                return;
            }
            break;
        case 2:				//读DF
            {
                len = 7;
                nRecLen = 2;
                SIMWrite(des, gSimSelectDF, 7);
                break;
            }
        case 3:				//DF EF response
        case 5:
            {
                BYTE cmd[10];
                memcpy(cmd, gSimResponse, 4);
                cmd[4] = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->simdevice_->mLength;
                len = 5;
                nRecLen = cmd[4] + 3;
                SIMWrite(des, cmd, 5);
                break;
            }
        case 4:				//EF
            {
                len = 7;
                nRecLen = 2;
                SIMWrite(des, gSimSelectEF, 7);
                break;
            }

        default:
            {
                if(gIndex >= gRecordCount)
                {
                    char txt[24];
                    sprintf(txt, "导入完成 %d张", gValidRecordCount);
                    m_MJPGList.SetUnitText(2, Util::StringOp::ToCString(txt), TRUE);

                    m_MJPGList.SetUnitIsShow(3, TRUE);
                    m_MJPGList.SetUnitIsShow(0, FALSE);

                    m_MJPGList.SetUnitIsDisable(1, FALSE);
                    m_MJPGList.SetUnitIsDownStatus(1, FALSE);
                    m_MJPGList.SetUnitIsShow(1, TRUE);

                    return;
                }

                BYTE cmd[10];
                memcpy(cmd, gSimReadRecorde, 5);
                cmd[2] = gIndex++;
                cmd[4] = m_nRecordeSize;
                len = 5;
                nRecLen = 3 + m_nRecordeSize;
                SIMWrite(des, cmd, 5);
                break;
            }
        }

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->simdevice_->WritePort(des, len, nRecLen);//发送数据  
        SetTimer(101, 4000, NULL);
    }

    void CSIMImportDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(nIDEvent == 101)
        {
            KillTimer(101);

            if(gReCount++ < 5)
            {
                //	MessageBox(L"通讯超时，请检查是否连接好!");
                gCmdIndex--;
                SendCMDSIM(gCmdIndex++);
            }
            else
            {
                m_MJPGList.SetUnitText(2, _T("导入失败"), TRUE);
            }
        }
        else if(nIDEvent == 102)
        {
            KillTimer(102);
            SendCMDSIM(gCmdIndex++);
        }

        else if(nIDEvent == 103)
        {
            KillTimer(103);

            SendCMDSIM(gCmdIndex++);
            SendCMDSIM(gCmdIndex++);
            SendCMDSIM(gCmdIndex++);
        }

        CCEDialog::OnTimer(nIDEvent);
    }

    std::string  GetAlias(std::string name)
    {
        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex == NULL)
        {
            return "";
        }

        std::string alias;
        for (std::size_t i = 0; i < name.size(); ++i)
        {
            if ((unsigned char)name[i] > (unsigned char)127)
            {
                unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i+1] - 0xa1);

                if(index >= 0 && index < 6768)
                {
                    alias += ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex[index];	
                }
                else
                {
                }
                ++i;
            }
            else
            {
                if (name[i] >= 'A' && name[i] <= 'Z')
                {
                    alias += char(name[i] + 32);
                }
                else
                {
                    alias += name[i];
                }
            }
        }
        return alias;
    }

    void CSIMImportDlg::WriteSIMTelPhoneBOOK(CString name, CString telcode)
    {
        gValidRecordCount++;
        m_pContact->name(Util::StringOp::FromCString(name));
        m_pContact->alias(GetAlias(m_pContact->name()));
        m_pContact->mobilesTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
        m_pContact->worksTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));

        if(telcode.Find(L"1", 0) == 0 || telcode.Find(L"861", 0) == 0)
            m_pContact->mobilesTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(telcode)));
        else
            m_pContact->worksTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(telcode)));

        m_pContact->mobilesTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
        m_pContact->worksTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
        m_pContact->email(Util::StringOp::FromCString(L""));
        m_pContact->homesite(Util::StringOp::FromCString(L""));
        m_pContact->company(Util::StringOp::FromCString(L""));
        m_pContact->companyAddress(Util::StringOp::FromCString(L""));
        m_pContact->companyPostCode(Util::StringOp::FromCString(L""));
        m_pContact->department(Util::StringOp::FromCString(L""));
        m_pContact->duty(Util::StringOp::FromCString(L""));
        m_pContact->memo(Util::StringOp::FromCString(L""));
        m_pContact->type(Data::ctNormal);
        m_pContact->groupId(m_nSIMID);
        m_pContact->Insert();

        CString s = name;
        s += " ";
        s += telcode;
        s += "\n";
        TRACE(s);
    }
}
