#include <winsock2.h>
#include <Wininet.h>
#include <windows.h>

#include "Executor.h"
#include "../Util/Text/StringOp.h"

namespace HttpClient {
	Executor::Executor() {
	}

	Executor::~Executor() {
	}

	Response const WinInetExecutor::Do(Request const& req) {
		Response result;
		//⊙ InternetOpen 初始化 WinInet.dll
		//⊙ InternetOpenUrl 打开 Url，读取数据
		//⊙ InternetAttemptConnect 尝试建立到 Internet 的连接
		//⊙ InternetConnect 建立 Internet 的连接
		//⊙ InternetCheckConnection 检查 Internet 的连接是否能够建立
		//⊙ InternetSetOption 设置一个 Internet 选项
		//⊙ InternetSetStausCallback 安装一个回调函数，供 API 函数调用
		//⊙ InternetQueryOption 查询在一个指定句柄上的 Internet 选项
		//⊙ InternetQueryDataAvailable 查询可用数据的数量
		//⊙ InternetReadFile(Ex) 从一个打开的句柄读取数据
		//⊙ InternetFindNextFile 继续文件搜寻
		//⊙ InetrnetSetFilePointer 为 InternetReadFile 设置一个文件位置
		//⊙ InternetWriteFile 将数据写到一个打开的 Internet 文件
		//⊙ InternetLockRequestFile 允许用户为正在使用的文件加锁
		//⊙ InternetUnlockRequestFile 解锁被锁定的文件
		//⊙ InternetTimeFromSystemTime 根据指定的 RFC 格式格式化日期和时间
		//⊙ InternetTimeToSystemTime 将一个 HTTP 时间/日期字串格式化为 SystemTime 结构对象
		//⊙ InternetConfirmZoneCrossing 检查在安全 URL 和非安全 URL 间的变化
		//⊙ InternetCloseHandle 关闭一个单一的 Internet 句柄
		//⊙ InternetErrorDlg 显示错误信息对话框
		//⊙ InternetGetLastResponesInfo 获取最近发送的 API函数的错误

		//⊙ HttpOpenRequest 打开一个 HTTP 请求的句柄
		//⊙ HttpSendRequert(Ex) 向 HTTP 服务器发送指定的请求
		//⊙ HttpQueryInfo 查询有关一次 HTTP 请求的信息
		//⊙ HttpEndRequest 结束一个 HTTP 请求
		//⊙ HttpAddRequestHeaders 添加一个或多个 HTTP 请求报头到 HTTP请求句柄

		//⊙ FtpCreateDirectory 在 Ftp 服务器新建一个目录
		//⊙ FtpDelectFile 删除存储在 Ftp 服务器上的文件
		//⊙ FtpFindFirstFile 查找给定 Ftp 会话中的指定目录
		//⊙ FtpGetCurrentDirectory 为指定 Ftp 会话获取当前目录
		//⊙ FtpGetFile 从 Ftp 服务器下载文件
		//⊙ FtpOpenFile 访问一个远程文件以对其进行读写
		//⊙ FtpPutFile 向 Ftp 服务器上传文件
		//⊙ FtpRemoveDirectory 在 Ftp 服务器删除指定的文件
		//⊙ FtpRenameFile 为 Ftp 服务器上的指定文件改名
		//⊙ FtpSetCurrentDirectory 更改在 Ftp 服务器上正在使用的目录

		//Function					Description
		//InternetCanonicalizeUrl 	Canonicalizes the URL.
		//InternetCombineUrl 		Combines base and relative URLs.
		//InternetCrackUrl 			Parses a URL string into components.
		//InternetCreateUrl 		Creates a URL string from components.
		//InternetOpenUrl 			Begins retrieving an FTP, HTTP, or HTTPS resource.

		//InternetAutodial
		//InternetAutodialHangup
		//InternetDial
		//InternetGetConnectedState
		//InternetGetConnectedStateEx
		//InternetHangUp
		//InternetGoOnline

		HINTERNET winInetHandle = InternetOpen(L"Microsoft Internet Explorer", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		HINTERNET sessionHandle = InternetConnect(winInetHandle, req.hostName.c_str(), req.port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		HINTERNET requestHandle = HttpOpenRequest(sessionHandle, req.method.c_str(), req.uri.c_str(), NULL, L"", NULL, 0, 0);
		//BOOL b = InternetSetOption(connectionHandle, option, buffer, bufferSize);
		std::wstring reqHeaders;
		for (std::map<std::wstring, std::wstring>::const_iterator i = req.headers.begin(); i != req.headers.end(); ++i) {
			reqHeaders += i->first + L":" + i->second + L"\r\n";
		}
		BOOL b = HttpAddRequestHeaders(requestHandle, reqHeaders.c_str(), reqHeaders.length(), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
		b = HttpSendRequest(requestHandle, NULL, 0, reinterpret_cast<void* const>(const_cast<char* const>(req.body.c_str())), req.body.length());
		//wchar_t version[16] = {0};
		//DWORD requestSize = 16 * sizeof(wchar_t);
		//b = HttpQueryInfo(requestHandle, HTTP_QUERY_VERSION, version, &requestSize, 0);
		//Response::version = std::wstring(version);
		//int statusCode = 0;
		//requestSize = sizeof statusCode;
		//b = HttpQueryInfo(requestHandle, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &requestSize, NULL);
		//if (b) {
		//	result.statusCode = statusCode;
		//}
		//b = HttpQueryInfo(requestHandle, HTTP_QUERY_STATUS_TEXT, NULL, &requestSize, 0);
		//wchar_t* statusText = (wchar_t*)malloc(requestSize + 1);
		//b = HttpQueryInfo(requestHandle, HTTP_QUERY_STATUS_TEXT, statusText, &requestSize, 0);
		//if (b) {
		//	result.statusText = statusText;
		//}
		//free(statusText);
		wchar_t* headers = 0;
		DWORD requestSize = 0;
		b = HttpQueryInfo(requestHandle, HTTP_QUERY_RAW_HEADERS_CRLF, headers, &requestSize, 0);
		headers = (wchar_t*)malloc(requestSize);
		b = HttpQueryInfo(requestHandle, HTTP_QUERY_RAW_HEADERS_CRLF, headers, &requestSize, 0);
		using namespace Util::Text::StringOp;
		std::vector<std::wstring> h = Split(headers, L"\r\n");
		size_t index = 0;
		for (std::vector<std::wstring>::iterator i = h.begin(); i != h.end(); ++i) {
			if (!i->empty()) {
				if (index == 0) { //first line
					//parse the version code text
					size_t pos = i->find(L" ");
					if (pos != std::wstring::npos) {
						Response::version = i->substr(0, pos);
						size_t offset = pos + 1;
						pos = i->find(L" ", offset);
						if (pos != std::wstring::npos) {
							result.statusCode = ToInt(i->substr(offset, pos - offset));
							result.statusText = i->substr(pos + 1);
						}
					}
				} else { //other
					//parse the name: value
					size_t pos = i->find(L":");
					if (pos != std::wstring::npos) {
						std::wstring const name = i->substr(0, pos);
						std::wstring const value = i->substr(pos + 1);
						result.headers.insert(std::make_pair(name, value));
					}
				}
			}
			++index;
		}
		free(headers);
		//b = HttpSendRequestEx(requestHandle, &bufferIn, NULL, NULL, 0);
		DWORD bufferLength = 0;
		DWORD bufferLengthLen = sizeof bufferLength;
		b = HttpQueryInfo(requestHandle, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &bufferLength, &bufferLengthLen, 0);
		char* buffer = (char*)malloc(bufferLength);
		//memset(buffer, 0, bufferLength);
		DWORD actualLength = 0;
		b = InternetReadFile(requestHandle, buffer, bufferLength, &actualLength);
		result.body = std::string(buffer, bufferLength);
		//b = InternetReadFileEx(requestHandle, bufferOut, flags, context);
		//b = InternetWriteFile(requestHandle, buffer, 1024, &writtenBytes);
		//b = InternetQueryDataAvailable(fileHandle, numberOfBytesAvailable, flags, context);
		//DWORD r = InternetSetFilePointer(fileHandle, distanceToMove, reserved, moveMethod, context);
		//r = InternetErrorDlg(windowHandle, requestHandle, error, flags, data);
		//b = HttpEndRequest(requestHandle, NULL, 0, 0);
		//b = InternetCloseHandle(requestHandle);
		return result;
	}

	Response const SocketExecutor::Do(Request const& req) {
		Response result;
		return result;
	}

	Response const AsioExecutor::Do(Request const& req) {
		Response result;
		return result;
	}
}
