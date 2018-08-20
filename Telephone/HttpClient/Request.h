#ifndef __HTTPCLIENT_REQUEST_H__
#define __HTTPCLIENT_REQUEST_H__

#pragma once

#include <string> //for std::wstring
#include <map> //for std::map

namespace HttpClient {
	class Request {
	public:
		Request();
		~Request();
		std::wstring method;
		std::wstring hostName;
		unsigned short port;
		std::wstring uri;
		static std::wstring version; //= L"HTTP/1.1"
		std::map<std::wstring, std::wstring> headers;
		std::string body;
	};
}

#endif //__HTTPCLIENT_REQUEST_H__
