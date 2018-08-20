#ifndef __UTIL_AT_NETWORKDIAL_H__
#define __UTIL_AT_NETWORKDIAL_H__

#include <utility>
#include <string>
#include <Ras.h>
#include <Wininet.h>
#include <Raserror.h>

namespace Util {
    namespace AT {
        class Dialer {
        public:
            enum LinkMode {
                lmDirect,
                lmDial,
            };

            enum APNType {
                tCMWAP,
                tCMNET,
                tLABWAP3,
            };
            struct RegItem {
                RegItem(std::wstring const & key, std::wstring const & v) {
                    name = key;
                    value = v;
                }
                std::wstring name;
                std::wstring value;
            };

            struct DialInfo {
                DialInfo(std::wstring const & secondValue, std::wstring const & thirdValue)
                : second(L"2", secondValue)
                , third(L"3", thirdValue) {
                }
                RegItem second;
                RegItem third;
            };

            struct APN {
                APN(std::wstring const & secondValue, std::wstring const & thirdValue, std::wstring const & number, std::wstring const & proxyValue, std::wstring const & httpValue, std::wstring const & username = L"", std::wstring const & passwd = L"")
                : dialInfo(secondValue, thirdValue)
                , telephoneNumber(number)
                , userName(username)
                , password(passwd)
                , proxy(proxyValue)
                , http(httpValue) {
                }
                DialInfo dialInfo;
                std::wstring telephoneNumber;
                std::wstring userName;
                std::wstring password;
                std::wstring proxy;
                std::wstring http;
            };

        public:
            static Dialer* const Instance() {
                static Dialer* result = new Dialer();
                return result;
            }
            DWORD const CheckDirectNetworkStatus() const;
            DWORD const CheckDialNetworkStatus() const;
            DWORD const CheckNetworkStatus() const;
            DWORD const InitDial(APNType const apnType);
            DWORD const Dial(HWND const notifyWindow);
            std::pair<int, std::wstring> const OnRasStateChanged(WPARAM w, LPARAM l);
            DWORD const Hangup();
            std::wstring const GetDynamicIp() const;
        private:
            Dialer();
            HRASCONN rasConn_;
            static APN ApnList[];
            static APNType ApnType;
            static std::wstring dialInfos[];
            static LinkMode linkMode;
        };
    }
}

#endif //__UTIL_AT_NETWORKDIAL_H__
