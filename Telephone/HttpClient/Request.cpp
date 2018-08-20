#include "Request.h"

namespace HttpClient {
	std::wstring Request::version = L"HTTP/1.1";

	Request::Request() : method(L"GET") {
	}

	Request::~Request() {
	}
}
