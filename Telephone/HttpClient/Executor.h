#ifndef __HTTPCLIENT_EXECUTOR_H__
#define __HTTPCLIENT_EXECUTOR_H__

#pragma once

#include "Request.h"
#include "Response.h"

namespace HttpClient {
	class Executor {
	public:
		Executor();
		~Executor();
	};

	class WinInetExecutor {
	public:
		static Response const Do(Request const& req);
	};

	class SocketExecutor {
	public:
		static Response const Do(Request const& req);
	};

	class AsioExecutor {
	public:
		static Response const Do(Request const& req);
	};
}

#endif //__HTTPCLIENT_EXECUTOR_H__
