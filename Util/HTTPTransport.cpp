#include "stdafx.h"
#pragma warning(disable: 4786)
#include <string>
#pragma warning(disable: 4786)
#include <vector>
#include "HTTPTransport.h"
#include "../MultimediaPhoneDlg.h"
#include "../Multimediaphone.h"

#include "StringOp.h"
extern int g_SignalSize;
extern VOID WriteMyLog_(char *ptr, int size);

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
		WCHAR const *HttpAcceptType_;
        int const BufferSize = 1024;
    }
    std::wstring const AgentName = L"3GPHONE(0.9/;p)";

    URI::URI(std::wstring const& uri):port(80) {
        size_t startPos = 0;
        size_t stopPos = 0;
        stopPos = uri.find(L"://", startPos);
        if (stopPos != std::wstring::npos) { //find protocol
            protocol = uri.substr(startPos, stopPos - startPos);
            startPos = stopPos + 3;
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
                port = StringOp::ToInt(host.substr(portStartPos + 1).c_str());
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
  /*  : responseHandler_(0)
    , thread_(new Transport(this))*/ 
	{
		wsprintf(apn_proxyIP, L"");
		bNewSocket = FALSE;
		HTTPRequest = NULL;
	//	pNewSocket = NULL;
		pNewSocket.Create();
    }
	
	void HTTPTransport::SetAPN(DIAL_APN apn, char *proxy)
	{
		m_APN = apn;
		mbstowcs(apn_proxyIP, proxy, strlen(proxy) + 1);
	}

    DWORD HTTPTransport::Get(std::wstring const& uriV, std::wstring const& agentName) {
		Dprintf("Get HTTPTransport 0\r\n");
        DWORD error = 0;
        HINTERNET HTTPOpen = ::InternetOpen(agentName.c_str(),
            INTERNET_OPEN_TYPE_PROXY /*INTERNET_OPEN_TYPE_PRECONFIG*/, // proxy option
            apn_proxyIP, // proxy
            NULL, // proxy bypass
            0); // flags
		Dprintf("Get HTTPTransport 1\r\n");
        if (!HTTPOpen) {
            error = ::GetLastError();
        } else {
			Dprintf("Get HTTPTransport 2\r\n");
            URI uri(uriV);
			Dprintf("Get HTTPTransport 3\r\n");

            HINTERNET HTTPConnection = ::InternetConnect(HTTPOpen, // internet opened handle
                uri.host.c_str(),
                uri.port,
                uri.username.c_str(),
                uri.password.c_str(),
                INTERNET_SERVICE_HTTP, // service type
                INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, // service option
                0);	// context call-back option
			Dprintf("Get HTTPTransport 4\r\n");
            if (!HTTPConnection) {
                error = ::GetLastError();
                ::CloseHandle(HTTPOpen);
            } else {
				Dprintf("Get HTTPTransport 5\r\n");
                if (::InternetAttemptConnect(NULL) != ERROR_SUCCESS) {
                    error = ::GetLastError();
                    ::CloseHandle(HTTPConnection);
                    ::CloseHandle(HTTPOpen);
                } else {
					Dprintf("Get HTTPTransport 6\r\n");
					wchar_t data[256];
					wcscpy(data,  HttpAcceptType.c_str());
                //  wchar_t const* acceptType = HttpAcceptType.c_str();
					wchar_t const* acceptType = &data[0];

					LPCTSTR ppszAcceptTypes[2];
					ppszAcceptTypes[0] = _T("*/*"); //We support accepting any mime file type since this is a simple download of a file
					ppszAcceptTypes[1] = NULL;
					/*
					m_hHttpFile = HttpOpenRequest(m_hHttpConnection, NULL, (LPCTSTR)m_sObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, (DWORD) this);
					if (m_hHttpFile == NULL)
					{
						HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
						return;
					}
					*/
				       HTTPRequest = ::HttpOpenRequest(HTTPConnection,
                        HttpVerbs[vGet].c_str(), // HTTP Verb
                        uriV.c_str(), // Object Name
                        HTTP_VERSION, // Version
                        NULL, // Reference
                        /*&acceptType*/ppszAcceptTypes, // Accept Type
                        INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION,
                        0); // context call-back point
					Dprintf("Get HTTPTransport 62\r\n");
                    if (!HTTPRequest) {
                        error = ::GetLastError();
						Dprintf("Get HTTPTransport 63 = %d\r\n", error);
                        return error;
                    } else {
                        // set accept header
						Dprintf("Get HTTPTransport 7\r\n");
                        if (!::HttpAddRequestHeaders(HTTPRequest, HttpAccept.c_str(), HttpAccept.length(), HTTP_ADDREQ_FLAG_REPLACE)) {
                            error = ::GetLastError();
                            return error;
                        } else {
                            //set other headers
							Dprintf("Get HTTPTransport 8\r\n");
                            std::wstring headers;
                            for (std::map<std::wstring, std::wstring>::iterator i = message.headers.begin(); i != message.headers.end(); ++i) {
                                headers += i->first + L": " + i->second + L"\x0d\x0a";
                            }
                            if(headers.length() != 0)
							{
								Dprintf("Get HTTPTransport 9\r\n");
								if (!::HttpAddRequestHeaders(HTTPRequest, headers.c_str(), headers.length(), HTTP_ADDREQ_FLAG_ADD)) {
									error = ::GetLastError();
								}
                            } 
							{
								Dprintf("Get HTTPTransport 10\r\n");
                                //send request
                                if (!::HttpSendRequest(HTTPRequest, // handle by returned HttpOpenRequest
                                    NULL, // additional HTTP header
                                    0, // additional HTTP header length
                                    NULL, // additional data in HTTP Post or HTTP Put
                                    0)) { // additional data length
                                        error = ::GetLastError();
                                } else {
                                   // thread_->startReceive();
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
		
		WriteMyLog_("Post_in",strlen("Post_in"));
		char log[100];
		memset(log,0,100);

		DWORD error = 0;
        HINTERNET  HTTPOpen = ::InternetOpen(agentName.c_str(),
            INTERNET_OPEN_TYPE_PROXY,//INTERNET_OPEN_TYPE_PRECONFIG, // proxy option
            apn_proxyIP, // proxy
            NULL, // proxy bypass
            0); // flags

        if (!HTTPOpen) {
            error = ::GetLastError();
			sprintf(log,"InternetOpen_error:%d",error);
			WriteMyLog_(log,strlen(log));
        } else {
            URI uri(uriV);

            HINTERNET  HTTPConnection = ::InternetConnect(HTTPOpen, // internet opened handle
                uri.host.c_str(), // server name
                uri.port, // ports
                uri.username.c_str(), // user name
                uri.password.c_str(), // password
                INTERNET_SERVICE_HTTP, // service type
                INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, // service option
                0);	// context call-back option

            if (!HTTPConnection) {
                error = ::GetLastError();
                //::CloseHandle(HTTPOpen);
				InternetCloseHandle(HTTPOpen);
				memset(log,0,100);
				sprintf(log,"HTTPConnection_error:%d",error);
				WriteMyLog_(log,strlen(log));

            } else {
                if (::InternetAttemptConnect(NULL) != ERROR_SUCCESS) {
                    error = ::GetLastError();
                    //::CloseHandle(HTTPConnection);
                    //::CloseHandle(HTTPOpen);
					InternetCloseHandle(HTTPConnection);
					InternetCloseHandle(HTTPOpen);
					memset(log,0,100);
					sprintf(log,"InternetAttemptConnect_error:%d",error);
					WriteMyLog_(log,strlen(log));

                } else {
                    wchar_t const* acceptType = HttpAcceptType.c_str(); //const_cast<wchar_t*>();
					LPCTSTR ppszAcceptTypes[2];
					ppszAcceptTypes[0] = _T("*/*"); //We support accepting any mime file type since this is a simple download of a file
					ppszAcceptTypes[1] = NULL;

                    HTTPRequest = ::HttpOpenRequest(HTTPConnection,
                        HttpVerbs[vPost].c_str(), // HTTP Verb
                        uriV.c_str(), // Object Name
                        HTTP_VERSION, // Version
                        NULL, // Reference
                        /*&acceptType*/ppszAcceptTypes, // Accept Type reinterpret_cast<LPCWSTR*>()
                        INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,
                        0); // context call-back point

                    if (!HTTPRequest) {
                        error = ::GetLastError();
						memset(log,0,100);
						sprintf(log,"HTTPRequest_error:%d",error);
						WriteMyLog_(log,strlen(log));
                        return error;
                    } else {
                        // set accept header
                        if (!::HttpAddRequestHeaders(HTTPRequest, HttpAccept.c_str(), HttpAccept.length(), HTTP_ADDREQ_FLAG_REPLACE)) {
                            error = ::GetLastError();
							memset(log,0,100);
							sprintf(log,"HttpAddRequestHeaders_error:%d",error);
							WriteMyLog_(log,strlen(log));
                            return error;
                        } else {
                            //set other headers
                            std::wstring headers;
                            for (std::map<std::wstring, std::wstring>::iterator i = message.headers.begin(); i != message.headers.end(); ++i) {
                                headers += i->first + L": " + i->second + L"\xd\xa";
                            }
							if(headers.length() != 0)
							{
								if (!::HttpAddRequestHeaders(HTTPRequest, headers.c_str(), headers.length(), HTTP_ADDREQ_FLAG_ADD)) {
									error = ::GetLastError();
									memset(log,0,100);
									sprintf(log,"HttpAddRequestHeaders_error:%d",error);
									WriteMyLog_(log,strlen(log));
								}
                            } 
							{
                                //send request
                                if (!::HttpSendRequest(HTTPRequest, // handle by returned HttpOpenRequest
                                    NULL, // additional HTTP header
                                    0, // additional HTTP header length
                                    pPostData,
		                            nPostDataLenth/*(void *)message.body.c_str(),*/ // additional data in HTTP Post or HTTP Put
                                    /*message.body.length()*/)) { // additional data length
                                        error = ::GetLastError();
									memset(log,0,100);
									sprintf(log,"HttpSendRequest_error:%d",error);
									WriteMyLog_(log,strlen(log));

                                } 
								else {
                                 //   thread_->setHttpRequest(HTTPRequest);
                                 //   thread_->startReceive();
                                }
                            }
                        }
                    }
                }
            }
        }
        return error;
    }

	int HTTPTransport::Recv(UINT8 * pBuf, int length)
	{
		int actSize = 0;
		DWORD size = 0;
		if(HTTPRequest)
		{
			if (!::InternetReadFile(HTTPRequest, pBuf, length, &size))
			{
				actSize = 0;
			}       
		}
		else if(bNewSocket)
		{
// 			actSize = pNewSocket.Receive(pBuf, length, MSG_PEEK);
// 			if(actSize > 0)
// 			{
				actSize = pNewSocket.Receive(pBuf, length);
// 			}
// 			else if(actSize == SOCKET_ERROR)
// 			{
// 				int err = GetLastError();
// 				actSize = SOCKET_ERROR;
// 			}
		}
		return actSize;
	}

	void HTTPTransport::Close()
	{
		if(HTTPRequest)
		{
			::InternetCloseHandle(HTTPRequest);
		//	::InternetCloseHandle(HTTPOpen);
		//	::InternetCloseHandle(HTTPConnection);
			HTTPRequest = NULL;
		//	HTTPOpen = NULL;
		//	HTTPConnection = NULL;
		}
		if(bNewSocket)
		{
//			pNewSocket.Close();    //不主动断开
//			delete pNewSocket;
//			pNewSocket = NULL;
			bNewSocket = FALSE;
		}
	}

	DWORD HTTPTransport::Get_(std::wstring const& uriV) {
		bNewSocket = FALSE;
	//	if(pNewSocket == NULL)
		{
		//	pNewSocket = new CNewSocket();
		//	pNewSocket.Create();
		}
		Dprintf("Get_ HTTPTransport 0\r\n");
        DWORD error = 0;
		URI uri(uriV);
		CString sIp = apn_proxyIP;
		int port = 80;
        if(wcslen(apn_proxyIP) > 0)
		{
			CString s = apn_proxyIP;
			int n = s.Find(L":");
			if(n >= 0)
			{
				sIp = s.Mid(0, n);
				CString sPort = s.Right(s.GetLength() - n - 1);
				port = Util::StringOp::ToInt(sPort);
			}
		}
		else
		{
			sIp = uri.host.c_str();
			port = uri.port;
		}

		if(pNewSocket.Connect(sIp, port))
		{
			bNewSocket = TRUE;
			char BUF_[1024] = {0};
			char url_[256] = {0};
			wcstombs(url_, uriV.c_str(), uriV.length());
			sprintf(BUF_, "GET %s HTTP/1.1\r\nAccept:*/*\r\nAccept-Language:zh-cn\r\nUser-Agent:3GPHONE(0.9/;p)\r\nConnection:Keep-Alive\r\n\r\n", url_);
			int ret = pNewSocket.Send(BUF_, strlen(BUF_));
			if(ret != strlen(BUF_))
				error = GetLastError();
		}
		else
			error = GetLastError();	
        return error;
    }

	DWORD HTTPTransport::Post_(std::wstring const& uriV) {
		bNewSocket = FALSE;
	//	if(pNewSocket == NULL)
		{
		//	pNewSocket = new CNewSocket();
		//	pNewSocket.Create();
		}
		Dprintf("Post_ HTTPTransport 0\r\n");
        DWORD error = 0;
		URI uri(uriV);
		CString sIp = apn_proxyIP;
		int port = 80;
        if(wcslen(apn_proxyIP) > 0)
		{
			CString s = apn_proxyIP;
			int n = s.Find(L":");
			if(n >= 0)
			{
				sIp = s.Mid(0, n);
				CString sPort = s.Right(s.GetLength() - n -1);
				port = Util::StringOp::ToInt(sPort);
			}

		}
		else
		{
			sIp = uri.host.c_str();
			port = uri.port;
		}
		if(pNewSocket.Connect(sIp, port))
		{
			bNewSocket = TRUE;
			char BUF_[1024] = {0};
			char url_[256] = {0};
			wcstombs(url_, uriV.c_str(), uriV.length());
			char *pBuf[3] = {"POST ", " HTTP/1.1\r\nContent-type: application/x-www-form-urlencoded\r\nAccept: text/plain,*/*\r\nUser-Agent: ceHttp\r\nContent-Length: ", "\r\n\r\n"};
	
			sprintf(BUF_, "%s%s%s%d%s", pBuf[0], url_, pBuf[1], nPostDataLenth, pBuf[2]);
			strcat(BUF_, pPostData);
			int ret = pNewSocket.Send(BUF_, strlen(BUF_));
			if(ret != strlen(BUF_))
				error = GetLastError();
		}
		else
			error = GetLastError();	
        return error;
    }

	DWORD HTTPTransport::PostMms(std::wstring const uriV, UINT8 *pData, int nDatalen)
	{
		bNewSocket = FALSE;

        DWORD error = 0;
		URI uri(uriV);
		CString sIp = apn_proxyIP;
		int port = 80;
        if(wcslen(apn_proxyIP) > 0)
		{
			CString s = apn_proxyIP;
			int n = s.Find(L":");
			if(n >= 0)
			{
				sIp = s.Mid(0, n);
				CString sPort = s.Right(s.GetLength() - n -1);
				port = Util::StringOp::ToInt(sPort);
			}

		}
		else
		{
			sIp = uri.host.c_str();
			port = uri.port;
		}
		
		Dprintf("sIp:%s",Util::StringOp::FromCString(sIp).c_str());
		Dprintf("port:%d",port);
		if(pNewSocket.Connect(sIp, port))
		{
			bNewSocket = TRUE;
			char BUF_[1024] = {0};
			char url_[256] = {0};
			wcstombs(url_, uriV.c_str(), uriV.length());


			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			CString mmsc(url_);  //wangzhenxing20100803
			mmsc.MakeLower();
			if(main->m_pSettingDlg->m_pSetting->mmsType_ == 0)//现网判断，不是现网放过
			{
				if(mmsc != L"http://mmsc.monternet.com" && mmsc != L"http://mmsc.monternet.com/")
				{
					error = -1;
					return error;
				}
			}

			char cLen[12];
			sprintf(cLen, "%d", nDatalen);
			char *pBuf[3] = {"POST ", " HTTP/1.1\r\nContent-type: application/vnd.wap.mms-message\r\nAccept: application/vnd.wap.mms-message,text/plain,*/*\r\nUser-Agent: ceHttp\r\nContent-Length: ", "\r\n\r\n"};
	
			TCHAR sLen[32];
		//	char BUF_[1024];
			sprintf(BUF_, "%s%s%s%d%s", pBuf[0], url_, pBuf[1], nDatalen, pBuf[2]);
			int ret = pNewSocket.Send(BUF_, strlen(BUF_));
			if(ret != strlen(BUF_))
				error = GetLastError();
			else
			{
				int nPlen = nDatalen;
				UINT8 *ptr = pData;

				int ret = pNewSocket.Send(ptr, nPlen);
				if(ret != nPlen)
				{
					error = GetLastError();
					return error ;
				}

// 				while(nPlen > 0)
// 				{
// 					Sleep(10);
// 					int count = min(nPlen, 1024);
// 					ret = pNewSocket.Send(ptr, count);
// 					Dprintf("count:%d",count);
// 					Dprintf("ret:%d",ret);
// 
// 					if(ret != count)
// 					{
// 						error = GetLastError();
// 						break;
// 					}
// 					nPlen -= count;
// 					ptr += count;
// 				}
			}
		}
		else
			error = GetLastError();	
        return error;
	}

	/*
	int HTTPTransport::Recv()
	{
		int size = 0;
		if(HTTPRequest)
		{
			
// 			for (;;) {
// 				char buffer[BufferSize] = {0};
// 				DWORD bufferLength = BufferSize;
// 				if (::HttpQueryInfo(HTTPRequest, HTTP_QUERY_RAW_HEADERS_CRLF, buffer, &bufferLength, &index) == ERROR_HTTP_HEADER_NOT_FOUND) {
// 					break;
// 				}
// 				//if (!::HttpQueryInfo(HTTPRequest_, HTTP_QUERY_RAW_HEADERS_CRLF, pHeaderBuffer, &dwHeaderBufferLength, NULL)) {
// 				//    error = ::GetLastError();
// 				//}
// 				//message.headers.insert(std::make_pair(name, value));
// 			}
			
			//receive body
			message.body = "";
			for (;;) {
				char buffer[BufferSize] = {0};
				DWORD actSize = 0;
				if (!::InternetReadFile(HTTPRequest,
					buffer,
					BufferSize,
					&actSize)) {
					break;
					//error = ::GetLastError();
				} else {
					if(actSize == 0)
						break;
					message.body += buffer;
					size += actSize;
				}
				if(actSize == 0)
					break;
			}             
		}
		return size;
		   //
	}
	*/
/*
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
	*/
}
