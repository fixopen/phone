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
		//�� InternetOpen ��ʼ�� WinInet.dll
		//�� InternetOpenUrl �� Url����ȡ����
		//�� InternetAttemptConnect ���Խ����� Internet ������
		//�� InternetConnect ���� Internet ������
		//�� InternetCheckConnection ��� Internet �������Ƿ��ܹ�����
		//�� InternetSetOption ����һ�� Internet ѡ��
		//�� InternetSetStausCallback ��װһ���ص��������� API ��������
		//�� InternetQueryOption ��ѯ��һ��ָ������ϵ� Internet ѡ��
		//�� InternetQueryDataAvailable ��ѯ�������ݵ�����
		//�� InternetReadFile(Ex) ��һ���򿪵ľ����ȡ����
		//�� InternetFindNextFile �����ļ���Ѱ
		//�� InetrnetSetFilePointer Ϊ InternetReadFile ����һ���ļ�λ��
		//�� InternetWriteFile ������д��һ���򿪵� Internet �ļ�
		//�� InternetLockRequestFile �����û�Ϊ����ʹ�õ��ļ�����
		//�� InternetUnlockRequestFile �������������ļ�
		//�� InternetTimeFromSystemTime ����ָ���� RFC ��ʽ��ʽ�����ں�ʱ��
		//�� InternetTimeToSystemTime ��һ�� HTTP ʱ��/�����ִ���ʽ��Ϊ SystemTime �ṹ����
		//�� InternetConfirmZoneCrossing ����ڰ�ȫ URL �ͷǰ�ȫ URL ��ı仯
		//�� InternetCloseHandle �ر�һ����һ�� Internet ���
		//�� InternetErrorDlg ��ʾ������Ϣ�Ի���
		//�� InternetGetLastResponesInfo ��ȡ������͵� API�����Ĵ���

		//�� HttpOpenRequest ��һ�� HTTP ����ľ��
		//�� HttpSendRequert(Ex) �� HTTP ����������ָ��������
		//�� HttpQueryInfo ��ѯ�й�һ�� HTTP �������Ϣ
		//�� HttpEndRequest ����һ�� HTTP ����
		//�� HttpAddRequestHeaders ���һ������ HTTP ����ͷ�� HTTP������

		//�� FtpCreateDirectory �� Ftp �������½�һ��Ŀ¼
		//�� FtpDelectFile ɾ���洢�� Ftp �������ϵ��ļ�
		//�� FtpFindFirstFile ���Ҹ��� Ftp �Ự�е�ָ��Ŀ¼
		//�� FtpGetCurrentDirectory Ϊָ�� Ftp �Ự��ȡ��ǰĿ¼
		//�� FtpGetFile �� Ftp �����������ļ�
		//�� FtpOpenFile ����һ��Զ���ļ��Զ�����ж�д
		//�� FtpPutFile �� Ftp �������ϴ��ļ�
		//�� FtpRemoveDirectory �� Ftp ������ɾ��ָ�����ļ�
		//�� FtpRenameFile Ϊ Ftp �������ϵ�ָ���ļ�����
		//�� FtpSetCurrentDirectory ������ Ftp ������������ʹ�õ�Ŀ¼

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
