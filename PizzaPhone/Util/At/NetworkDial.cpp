#include "NetworkDial.h"
#include "../Text/StringOp.h"
#include "../Reg.h"
#include "../ControlCode.h"

#define   FSCTL_NDISUIO_BASE FILE_DEVICE_NETWORK 

#define _NDISUIO_CTL_CODE(_Function, _Method, _Access) CTL_CODE(FSCTL_NDISUIO_BASE, _Function, _Method, _Access)

#define IOCTL_NDISUIO_QUERY_BINDING _NDISUIO_CTL_CODE(0x203, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NDISUIO_OPEN_DEVICE _NDISUIO_CTL_CODE(0x200, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NDISUIO_QUERY_OID_VALUE _NDISUIO_CTL_CODE(0x201, METHOD_BUFFERED, FILE_ANY_ACCESS)

#include <Ntddndis.h>

namespace Util {
    namespace AT {
        Dialer::APN Dialer::ApnList[] = {
            APN(L"AT+CGDCONT=1,\"IP\",\"CMWAP\",\"\",0,0<cr>", L"AT+CGQREQ=1<cr>", L"*98*1#", L"ip:port", L""), APN(L"AT+CGDCONT=1,\"IP\",\"CMNET\",\"\",0,0<cr>", L"AT+CGEQREQ=1,2,128,2048,0,0,0,0,\"0E0\",\"0E0\",,0,0<cr>", L"*98*1#", L"ip:port", L""), APN(L"AT+CGDCONT=1,\"IP\",\"labwap3\",\"\",0,0<cr>", L"AT+CGEQREQ=1,2,128,2048,0,0,0,0,\"0E0\",\"0E0\",,0,0<cr>", L"*98*1#", L"ip:port", L"")
        };

        Dialer::APNType Dialer::ApnType;

        std::wstring Dialer::dialInfos[] = {
            L"打开端口", L"端口已打开", L"连接设备", L"设备已连接", L"所有设备已连接", L"验证", L"验证通知", L"验证重试", L"验证回调", L"验证密码已改变", L"验证方案", L"验证连接速率", L"验证回应", L"重新验证", L"验证已通过", L"准备回应", L"等待modem复位", L"等待回应", L"方案", L"暂停", L"重新验证", L"呼叫等待回应", L"密码过期", L"已连接", L"连接断开", L"未知错误"
        };

        Dialer::LinkMode Dialer::linkMode;

        struct NDISUIO_QUERY_BINDING {
            ULONG BindingIndex; //0-based binding number
            ULONG DeviceNameOffset; //from start of this struct
            ULONG DeviceNameLength; //in bytes
            ULONG DeviceDescrOffset; //from start of this struct
            ULONG DeviceDescrLength; //in bytes
        };
        typedef NDISUIO_QUERY_BINDING* PNDISUIO_QUERY_BINDING;
        typedef ULONG NDIS_OID, * PNDIS_OID;
        struct NDISUIO_QUERY_OID {
            NDIS_OID Oid;
            wchar_t DeviceName;
            UCHAR Data[sizeof(ULONG)];
        };
        typedef NDISUIO_QUERY_OID* PNDISUIO_QUERY_OID;

        DWORD const Dialer::CheckDirectNetworkStatus() const {
            DWORD result = 0;
            HANDLE hDevice = ::CreateFile(TEXT("UIO1:"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
            if (hDevice != INVALID_HANDLE_VALUE) {
                size_t const bindSize = sizeof(NDISUIO_QUERY_BINDING);
                char bindBuffer[bindSize] = {
                    0
                };
                PNDISUIO_QUERY_BINDING bindInfo = reinterpret_cast<PNDISUIO_QUERY_BINDING>(bindBuffer);
                bindInfo->BindingIndex = 0;
                DWORD outByteCount = 0; 
                //Get Device Name
                BOOL success = ::DeviceIoControl(hDevice, IOCTL_NDISUIO_QUERY_BINDING, bindInfo, bindSize, NULL, 0, &outByteCount, NULL);   
                if (success) {
                    wchar_t* deviceName = (wchar_t*)((char*)bindInfo + bindInfo->DeviceNameOffset);   
                    //Open Device
                    success = ::DeviceIoControl(hDevice, IOCTL_NDISUIO_OPEN_DEVICE, deviceName, wcslen(deviceName) * sizeof(wchar_t), NULL, 0, &outByteCount, NULL);   
                    if (success) {
                        size_t const querySize = sizeof(NDISUIO_QUERY_OID) + sizeof(DWORD);
                        char queryBuffer[querySize] = {
                            0
                        };
                        PNDISUIO_QUERY_OID queryOid = reinterpret_cast<PNDISUIO_QUERY_OID>(queryBuffer);
                        queryOid->Oid = OID_GEN_MEDIA_CONNECT_STATUS; //Custom OID
                        queryOid->DeviceName = NULL; //You dont need this once you have done a OPEN Device
                        //Query State
                        success = ::DeviceIoControl(hDevice, IOCTL_NDISUIO_QUERY_OID_VALUE, (LPVOID)queryOid, querySize, (LPVOID)queryOid, querySize, &outByteCount, NULL);
                        if (success) {
                            ULONG state = *(ULONG*)(queryOid->Data);
                            switch (state) {
                            case NdisMediaStateConnected:
                                // Connected!
                                result = 1;
                                break;
                            case NdisMediaStateDisconnected:
                                //do nothing
                                break;
                            default:
                                break;
                            }
                        } else {
                            //query state failure
                        }
                    } else {
                        //open device failure
                    }
                } else {
                    //get device name failure
                }
                ::CloseHandle(hDevice);
            } else {
                //get device handler failure
            }
            return result;
        }

        DWORD const Dialer::CheckDialNetworkStatus() const {
            DWORD result = 0;
            RASCONNSTATUS state;
            DWORD failure = ::RasGetConnectStatus(rasConn_, &state);
            /*RASCONNSTATE */int stateCode = state.rasconnstate;
            if (stateCode >= RASCS_Interactive && stateCode <= RASCS_PasswordExpired) {
                stateCode = stateCode - RASCS_Interactive + 19;
            } else if (stateCode >= RASCS_Connected && stateCode <= RASCS_Disconnected) {
                  stateCode = stateCode - RASCS_Connected + 23;
            }
            if (RASCS_Connected == stateCode) {
                result = 1;
            } else {
                result = 0;
            }
            return result;
        }

        DWORD const Dialer::CheckNetworkStatus() const {
            DWORD result = 0;
            switch (linkMode) {
            case lmDirect:
                result = CheckDirectNetworkStatus();
                break;
            case lmDial:
                result = CheckDialNetworkStatus();
                break;
            default:
                break;
            }
            return result;
        }

        std::pair<int, std::wstring> const Dialer::OnRasStateChanged(WPARAM w, LPARAM l) {
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

        DWORD const Dialer::InitDial(APNType const apnType) {
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

        DWORD const Dialer::Dial(HWND const notifyWindow) {
            RASDIALPARAMS dialParams;
            memset(&dialParams, 0, sizeof(dialParams));
            // Configure the RASDIALPARAMS structure. 
            dialParams.dwSize = sizeof(RASDIALPARAMS);
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

        DWORD const Dialer::Hangup() {
            return ::RasHangUp(rasConn_);
            bool result = false;
            if (::RasHangUp(rasConn_) == 0) {
                result = true;
            }
            ::Sleep(2000);
            return result;
        }

        std::wstring const Dialer::GetDynamicIp() const {
            Reg reg(HKEY_LOCAL_MACHINE, L"Comm\\Serial Cable on VCOM3:(PS)\\Parms\\TcpIp");
            wchar_t ipAddress[256] = {
                0
            };
            reg.Read(L"IpAddress", ipAddress);
            return ipAddress;
        }

        Dialer::Dialer() {
        }
    }
}
