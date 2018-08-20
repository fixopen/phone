#include "NetworkDial.h"
#include "../Text/StringOp.h"
#include "../Reg.h"

namespace Util {
    namespace AT {
        APN ApnList[] = {
            APN(L"AT+CGDCONT=1,\"IP\",\"CMWAP\",\"\",0,0<cr>", L"AT+CGQREQ=1<cr>", L"*98*1#", L"ip:port", L""),
            APN(L"AT+CGDCONT=1,\"IP\",\"CMNET\",\"\",0,0<cr>", L"AT+CGEQREQ=1,2,128,2048,0,0,0,0,\"0E0\",\"0E0\",,0,0<cr>", L"*98*1#", L"ip:port", L""),
            APN(L"AT+CGDCONT=1,\"IP\",\"labwap3\",\"\",0,0<cr>", L"AT+CGEQREQ=1,2,128,2048,0,0,0,0,\"0E0\",\"0E0\",,0,0<cr>", L"*98*1#", L"ip:port", L"")
        };

        APNType ApnType;

        std::wstring dialInfos[] = {
            L"�򿪶˿�",
            L"�˿��Ѵ�",
            L"�����豸",
            L"�豸������",
            L"�����豸������",
            L"��֤",
            L"��֤֪ͨ",
            L"��֤����",
            L"��֤�ص�",
            L"��֤�����Ѹı�",
            L"��֤����",
            L"��֤��������",
            L"��֤��Ӧ",
            L"������֤",
            L"��֤��ͨ��",
            L"׼����Ӧ",
            L"�ȴ�modem��λ",
            L"�ȴ���Ӧ",
            L"����",
            L"��ͣ",
            L"������֤",
            L"���еȴ���Ӧ",
            L"�������",
            L"������",
            L"���ӶϿ�",
            L"δ֪����"
        };

        std::pair<int, std::wstring> const OnRasStateChanged(WPARAM w, LPARAM l) {
            int state = w;
            if (state >= RASCS_Interactive && state <= RASCS_PasswordExpired) {
                state = state - RASCS_Interactive + 19;
            } else if (state >= RASCS_Connected && state <= RASCS_Disconnected) {
                state = state - RASCS_Connected + 23;
            }
            if (state > 25) {
                state = 25;
            }
            //if (state == 23) { //connected
            //} else if (state == 24) { //unconnected
            //}
            return std::make_pair(state, dialInfos[state]);
        }

        DWORD const InitDial(APNType const apnType) {
            ApnType = apnType;
            Reg r(HKEY_CURRENT_USER, L"ControlPanel\\Dial");
            r.Write(L"CurrentLoc", 1);
            Reg reg(HKEY_LOCAL_MACHINE, L"ExtModems\\3GMODEM\\Init");
            reg.Write(ApnList[apnType].dialInfo.second.name.c_str(), ApnList[apnType].dialInfo.second.value.c_str());
            reg.Write(ApnList[apnType].dialInfo.third.name.c_str(), ApnList[apnType].dialInfo.third.value.c_str());
            RASENTRY entry;
            memset(&entry, 0, sizeof(RASENTRY));
            entry.dwSize = sizeof(RASENTRY);
            //entry.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_ModemLights | RASEO_IpHeaderCompression | RASEO_SwCompression | RASEO_NetworkLogon;
            //entry.dwfOptions = 0x38010210;
            entry.dwfOptions = 0x400208;
            entry.dwfNetProtocols = RASNP_Ip /*| RASNP_NetBEUI | RASNP_Ipx*/;
            entry.dwFramingProtocol = RASFP_Ppp;
            wsprintf(entry.szDeviceType, L"modem");
            wsprintf(entry.szDeviceName, L"Serial Cable on VCOM3:(PS)");
            wsprintf(entry.szLocalPhoneNumber, ApnList[apnType].telephoneNumber.c_str());
            entry.dwCountryID = 0;
            entry.dwCountryCode = 86;
            wsprintf(entry.szAreaCode, L"10");
            return ::RasSetEntryProperties(NULL, // pointer to full path and filename of phone-book file
                L"890", // pointer to an entry name
                &entry, // buffer that contains entry information
                sizeof(RASENTRY), //size, in bytes, of the lpRasEntry buffer
                NULL, //buffer that contains device-specific 
                0);	
        }

        DWORD const Dial(HWND const notifyWindow) {
            RASDIALPARAMS dialParams;
            memset(&dialParams, 0, sizeof(dialParams));
            // Configure the RASDIALPARAMS structure. 
            dialParams.dwSize = sizeof (RASDIALPARAMS);
            wsprintf(dialParams.szPhoneNumber, ApnList[ApnType].telephoneNumber.c_str());
            wsprintf(dialParams.szCallbackNumber, L"");
            wsprintf(dialParams.szDomain, L"");
            wsprintf(dialParams.szEntryName, L"890");
            //strcpy(dialParams.szUserName, "100001644588");
            //strcpy(dialParams.szPassword, "m2y8g4k9");
            wsprintf(dialParams.szUserName, ApnList[ApnType].userName.c_str());
            wsprintf(dialParams.szPassword, ApnList[ApnType].password.c_str());
            wsprintf(dialParams.szDomain, L"");
            return ::RasDial(NULL, NULL, &dialParams, 0xFFFFFFFF, notifyWindow, &rasConn_);
        }
    }
}
