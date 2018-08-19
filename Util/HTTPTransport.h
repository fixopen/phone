#ifndef __UTIL_HTTPTRANSPORT_H__
#define __UTIL_HTTPTRANSPORT_H__

//#include "Thread.h"

//#include "SmartPtr.h"
#include <string>
#include <vector>
#include <map>
#include <Wininet.h>
#include "NewSocket.h"

typedef unsigned long DWORD;
namespace Util
{
    enum DIAL_APN {
        CM_WAP,
        CM_NET
    };
    struct URI {
        URI()
        : protocol(L"http")
        , port(80) {
        }
        URI(std::wstring const & v);
        std::wstring protocol;
        std::wstring username;
        std::wstring password;
        std::wstring host;
        unsigned short port;
        std::wstring resource;
        std::wstring fragment;
        std::map<std::wstring, std::wstring> quests;
    };
    extern std::wstring const AgentName;
    class HTTPTransport {
        friend class Transport;
    public:
        struct Message {
            std::map<std::wstring, std::wstring> headers;
            std::string body;
        };
        char* pPostData;
        int nPostDataLenth;
        void SetPost(char* ptr, int len) {
            pPostData = ptr; nPostDataLenth = len;
        }
        DIAL_APN m_APN;
        TCHAR apn_proxyIP[32];
        void SetAPN(DIAL_APN apn, char* proxy);
        HTTPTransport();
        typedef void (*ResponseHandler)(Message const & message);
        ResponseHandler responseHandler_;
        Message message;
        HINTERNET HTTPRequest;
        void SetResponseHandler(ResponseHandler responseHandler) {
            responseHandler_ = responseHandler;
        }
        DWORD Get(std::wstring const & uri, std::wstring const & agentName = AgentName);
        DWORD Post(std::wstring const & uri, std::wstring const & agentName = AgentName);

        CNewSocket pNewSocket;
        BOOL bNewSocket;
        DWORD Get_(std::wstring const & uri);
        DWORD Post_(std::wstring const & uri);

        int Recv(UINT8* pBuf, int length);
        void Close();
        /*
        private:
        class Transport : public Thread {
        public:
        Transport(HTTPTransport* owner)
        : Thread()
        , owner_(owner)
        , quit_(false)
        , active_(false) {
        }
        void setHttpRequest(HINTERNET const& HTTPRequest) {
        HTTPRequest_ = HTTPRequest;
        }
        void willStop() {
        quit_ = true;
        }
        void startReceive() {
        active_ = true;
        }
        virtual int const run();
        private:
        HTTPTransport* owner_;
        HINTERNET HTTPRequest_;
        volatile bool quit_;
        volatile bool active_;
        };
        Transport* thread_;
        */
        //      shared_ptr<URI> parseUri_(std::wstring const& uri);
    };
}

#endif // __UTIL_HTTPTRANSPORT_H__
