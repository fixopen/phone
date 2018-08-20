#ifndef __HTTPCLIENT_RESPONSE_H__
#define __HTTPCLIENT_RESPONSE_H__

#pragma once

#include <string> //for std::wstring
#include <map> //for std::map

namespace HttpClient {
	class Response {
	public:
		Response();
		~Response();
		static std::wstring version; //= L"HTTP/1.1"
		int statusCode;
		std::wstring statusText;
		std::map<std::wstring, std::wstring> headers;
		std::string body;
	};
}

#endif //__HTTPCLIENT_RESPONSE_H__
