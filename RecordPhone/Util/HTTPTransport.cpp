#include "HTTPTransport.h"

#include "StringOp.h"

namespace Util {
    namespace {
        enum Verbs {
            vGet,
            vPost,
        };
        std::wstring const HttpVerbs[2] = {        
            L"GET",
            L"POST"
        };
        std::wstring const HttpAcceptType = L"*/*";
        std::wstring const HttpAccept = L"Accept: */*\r\n";
        int const BufferSize = 1024;
    }
    std::wstring const AgentName = L"3GPHONE(0.9/;p)";

    URI::URI(std::wstring const& uri) {
        size_t startPos = 0;
        size_t stopPos = 0;
        stopPos = uri.find(L"://", startPos);
        if (stopPos != std::wstring::npos) { //find protocol
            protocol = uri.substr(startPos, stopPos - startPos);
            startPos = stopPos + 1;
        }
        stopPos = uri.find(L"/", startPos);
        if (stopPos != std::wstring::npos) { //find host
            host = uri.substr(startPos, stopPos - startPos);
            startPos = stopPos + 1;
            size_t userPartStopPos = host.find(L"@");
            if (userPartStopPos != std::wstring::npos) { //find user
                username = host.substr(0, userPartStopPos);
                size_t usernameStopPos = username.find(L":");
                if (usernameStopPos != std::wstring::npos) { //find password
                    password = username.substr(usernameStopPos + 1);
                    username = username.substr(0, usernameStopPos);
                }
                host = host.substr(userPartStopPos);
            }
            size_t portStartPos = host.find(L":");
            if (portStartPos != std::wstring::npos) { //find port
                port = StringOp::ToInt(host.substr(portStartPos + 1));
                host = host.substr(0, portStartPos);
            }
        } else { //end
            host = uri.substr(startPos);
        }
        resource = uri.substr(startPos);
        bool findFragment = false;
        stopPos = resource.find(L"#", startPos);
        if (stopPos != std::wstring::npos) { //find fragment
            resource = resource.substr(0, stopPos);
            fragment = resource.substr(stopPos + 1);
            startPos = stopPos + 1;
            findFragment = true;
        }
        std::wstring rc = resource;
        if (findFragment) {
            rc = fragment;
        }
        stopPos = rc.find(L"?", startPos);
        if (stopPos != std::wstring::npos) { //find quests
            if (findFragment) {
                fragment = rc.substr(0, stopPos);
            } else {
                resource = rc.substr(0, stopPos);
            }
            rc = rc.substr(stopPos + 1);
            startPos = 0;
            while ((stopPos = rc.find(L"&", startPos)) != std::wstring::npos) {
                std::wstring p = uri.substr(startPos, stopPos - startPos);
                size_t nameStopPos = p.find(L"=");
                std::wstring name = p.substr(0, nameStopPos);
                std::wstring value = p.substr(nameStopPos + 1);
                quests.insert(std::make_pair(name, value));
                startPos = stopPos + 1;
            }
            std::wstring p = rc.substr(startPos, stopPos - startPos);
            size_t nameStopPos = p.find(L"=");
            std::wstring name = p.substr(0, nameStopPos);
            std::wstring value = p.substr(nameStopPos + 1);
            quests.insert(std::make_pair(name, value));
            startPos = stopPos + 1;
        }
    }

    HTTPTransport::HTTPTransport()
    : responseHandler_(0)
    , thread_(new Transport(this)) {
    }

    DWORD HTTPTransport::Get(std::wstring const& uriV, std::wstring const& agentName) {
        DWORD error = 0;
        HINTERNET HTTPOpen = ::InternetOpen(agentName.c_str(),
            INTERNET_OPEN_TYPE_PRECONFIG, // proxy option
            L"", // proxy
            L"", // proxy bypass
            0); // flags

        if (!HTTPOpen) {
            error = ::GetLastError();
        } else {
            URI uri(uriV);

            HINTERNET HTTPConnection = ::InternetConnect(HTTPOpen, // internet opened handle
                uri.host.c_str(),
                uri.port,
                uri.username.c_str(),
                uri.password.c_str(),
                INTERNET_SERVICE_HTTP, // service type
                INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, // service option
                0);	// context call-back option

            if (!HTTPConnection) {
                error = ::GetLastError();
                ::CloseHandle(HTTPOpen);
            } else {
                if (::InternetAttemptConnect(NULL) != ERROR_SUCCESS) {
                    error = ::GetLastError();
                    ::CloseHandle(HTTPConnection);
                    ::CloseHandle(HTTPOpen);
                } else {
                    wchar_t const* acceptType = HttpAcceptType.c_str();
                    HINTERNET HTTPRequest = ::HttpOpenRequest(HTTPConnection,
                        HttpVerbs[vGet].c_str(), // HTTP Verb
                        uriV.c_str(), // Object Name
                        HTTP_VERSION, // Version
                        L"", // Reference
                        &acceptType, // Accept Type
                        INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,
                        0); // context call-back point

                    if (!HTTPRequest) {
                        error = ::GetLastError();
                        return error;
                    } else {
                        // set accept header
                        if (!::HttpAddRequestHeaders(HTTPRequest, HttpAccept.c_str(), HttpAccept.length(), HTTP_ADDREQ_FLAG_REPLACE)) {
                            error = ::GetLastError();
                            return error;
                        } else {
                            //set other headers
                            std::wstring headers;
                            for (std::map<std::wstring, std::wstring>::iterator i = message.headers.begin(); i != message.headers.end(); ++i) {
                                headers += i->first + L": " + i->second + L"\x13\x10";
                            }
                            if (!::HttpAddRequestHeaders(HTTPRequest, headers.c_str(), headers.length(), HTTP_ADDREQ_FLAG_ADD)) {
                                error = ::GetLastError();
                            } else {
                                //send request
                                if (!::HttpSendRequest(HTTPRequest, // handle by returned HttpOpenRequest
                                    NULL, // additional HTTP header
                                    0, // additional HTTP header length
                                    NULL, // additional data in HTTP Post or HTTP Put
                                    0)) { // additional data length
                                        error = ::GetLastError();
                                } else {
                                    thread_->setHttpRequest(HTTPRequest);
                                    thread_->startReceive();
                                }
                            }
                        }
                    }
                }
            }
        }
        return error;
    }

    DWORD HTTPTransport::Post(std::wstring const& uriV, std::wstring const& agentName) {
        DWORD error = 0;
        HINTERNET HTTPOpen = ::InternetOpen(agentName.c_str(),
            INTERNET_OPEN_TYPE_PRECONFIG, // proxy option
            L"", // proxy
            L"", // proxy bypass
            0); // flags

        if (!HTTPOpen) {
            error = ::GetLastError();
        } else {
            URI uri(uriV);

            HINTERNET HTTPConnection = ::InternetConnect(HTTPOpen, // internet opened handle
                uri.host.c_str(), // server name
                uri.port, // ports
                uri.username.c_str(), // user name
                uri.password.c_str(), // password
                INTERNET_SERVICE_HTTP, // service type
                INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, // service option
                0);	// context call-back option

            if (!HTTPConnection) {
                error = ::GetLastError();
                ::CloseHandle(HTTPOpen);
            } else {
                if (::InternetAttemptConnect(NULL) != ERROR_SUCCESS) {
                    error = ::GetLastError();
                    ::CloseHandle(HTTPConnection);
                    ::CloseHandle(HTTPOpen);
                } else {
                    wchar_t const* acceptType = HttpAcceptType.c_str(); //const_cast<wchar_t*>();
                    HINTERNET HTTPRequest = ::HttpOpenRequest(HTTPConnection,
                        HttpVerbs[vPost].c_str(), // HTTP Verb
                        uriV.c_str(), // Object Name
                        HTTP_VERSION, // Version
                        L"", // Reference
                        &acceptType, // Accept Type reinterpret_cast<LPCWSTR*>()
                        INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,
                        0); // context call-back point

                    if (!HTTPRequest) {
                        error = ::GetLastError();
                        return error;
                    } else {
                        // set accept header
                        if (!::HttpAddRequestHeaders(HTTPRequest, HttpAccept.c_str(), HttpAccept.length(), HTTP_ADDREQ_FLAG_REPLACE)) {
                            error = ::GetLastError();
                            return error;
                        } else {
                            //set other headers
                            std::wstring headers;
                            for (std::map<std::wstring, std::wstring>::iterator i = message.headers.begin(); i != message.headers.end(); ++i) {
                                headers += i->first + L": " + i->second + L"\x13\x10";
                            }
                            if (!::HttpAddRequestHeaders(HTTPRequest, headers.c_str(), headers.length(), HTTP_ADDREQ_FLAG_ADD)) {
                                error = ::GetLastError();
                            } else {
                                //send request
                                if (!::HttpSendRequest(HTTPRequest, // handle by returned HttpOpenRequest
                                    NULL, // additional HTTP header
                                    0, // additional HTTP header length
                                    const_cast<void*>(reinterpret_cast<void const*>(message.body.c_str())), // additional data in HTTP Post or HTTP Put
                                    message.body.length())) { // additional data length
                                        error = ::GetLastError();
                                } else {
                                    thread_->setHttpRequest(HTTPRequest);
                                    thread_->startReceive();
                                }
                            }
                        }
                    }
                }
            }
        }
        return error;
    }


    int const HTTPTransport::Transport::run() {
        while (!quit_) {
            if (active_) {
                if (owner_->responseHandler_) {
                    //send get or post
                    //query response info
                    Message message;
                    //receive headers
                    DWORD index = 0;
                    for (;;) {
                        char buffer[BufferSize] = {0};
                        DWORD bufferLength = BufferSize;
                        if (::HttpQueryInfo(HTTPRequest_, HTTP_QUERY_RAW_HEADERS_CRLF, buffer, &bufferLength, &index) == ERROR_HTTP_HEADER_NOT_FOUND) {
                            break;
                        }
                        //if (!::HttpQueryInfo(HTTPRequest_, HTTP_QUERY_RAW_HEADERS_CRLF, pHeaderBuffer, &dwHeaderBufferLength, NULL)) {
                        //    error = ::GetLastError();
                        //}
                        //message.headers.insert(std::make_pair(name, value));
                    }
                    //receive body
                    for (;;) {
                        char buffer[BufferSize] = {0};
                        DWORD actSize = 0;
                        if (!::InternetReadFile(HTTPRequest_,
                            buffer,
                            BufferSize,
                            &actSize)) {
                            //error = ::GetLastError();
                        } else {
                            message.body += buffer;
                        }
                    }
                    //if finally
                    //call back customer
                    owner_->responseHandler_(message);
                }
                active_ = false;
            } else {
                ::Sleep(3 * 1000);
            }
        }
        return 0;
    }
}
