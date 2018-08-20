#ifndef __UTIL_HTTPTRANSPORT_H__
#define __UTIL_HTTPTRANSPORT_H__

#include "Thread.h"
#include "SmartPtr.h"
#include <map>
#include <wininet.h>

namespace Util {
    struct URI {
        URI()
        : protocol(L"http")
        , port(80) {
        }
        URI(std::wstring const& v);
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

        HTTPTransport();

        Message message;
        typedef void (*ResponseHandler)(Message const& message);
        void SetResponseHandler(ResponseHandler responseHandler) {
            responseHandler_ = responseHandler;
        }
        DWORD Get(std::wstring const& uri, std::wstring const& agentName = AgentName);
        DWORD Post(std::wstring const& uri, std::wstring const& agentName = AgentName);
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
        ResponseHandler responseHandler_;
        shared_ptr<URI> parseUri_(std::wstring const& uri);
    };
}

#endif // __UTIL_HTTPTRANSPORT_H__
