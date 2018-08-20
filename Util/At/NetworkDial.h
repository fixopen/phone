#ifndef __UTIL_AT_NETWORKDIAL_H__
#define __UTIL_AT_NETWORKDIAL_H__

#include <string>
#include <Ras.h>
#include <Wininet.h>
#include <Raserror.h>

namespace Util {
    namespace AT {
        class Dial {
        public:
            enum APNType {
                tCMWAP,
                tCMNET,
                tLABWAP3,
            };
            struct RegItem {
                RegItem(std::wstring const& key, std::wstring const& v) {
                    name = key;
                    value = v;
                }
                std::wstring name;
                std::wstring value;
            };

            struct DialInfo {
                DialInfo(std::wstring const& secondValue, std::wstring const& thirdValue)
                    : second(L"2", secondValue)
                    , third(L"3", thirdValue) {
                }
                RegItem second;
                RegItem third;
            };

            struct APN {
                APN(std::wstring const& secondValue, std::wstring const& thirdValue, std::wstring const& number, std::wstring const& proxyValue, std::wstring const& httpValue, std::wstring const& username = L"", std::wstring const& passwd = L"")
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
            DWORD const InitDial(APNType const apnType);
            DWORD const Dial(HWND const notifyWindow);
            std::pair<int, std::wstring> const OnRasStateChanged(WPARAM w, LPARAM l);
        private:
            APNType ApnType;
            HRASCONN rasConn_;
        };
    }
}

#endif //__UTIL_AT_NETWORKDIAL_H__
