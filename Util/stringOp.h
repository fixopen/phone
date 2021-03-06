#ifndef __UTIL_STRINGOP_H__
#define __UTIL_STRINGOP_H__

#include "DateTime.h"

#include <string>

namespace Util
{
	class StringOp
	{
	public:
		static int UnicodeToUTF_8(char* des, wchar_t* src, int size_d, int size_s);
		static CString utf82Cstring(char *pBuff);
		// UTF-8的unicode表示方法到unicode的值转换函数
		static int utf82unicode(unsigned char  *byte, int index, int count, WCHAR &unicode);
		static int FindStringArryIndex(CString *sArry, int ncount, std::string s);
		static int ToInt(std::string& content, size_t const mode = 10);
		static void ToRectangle(std::string& content, CRect &rect);//[(x0,y0)(x1,y1)]
		static int FindStringArryIndex(CString *sArry, int ncount, CString s);
		static int ToInt(CString content, size_t mode = 10);
		static void GetIPFromCString(CString ip, int *nIp);	
		static void ToRectangle(CString& content, CRect &rect); //[(x0,y0)(x1,y1)]
		static Timestamp ToTimestamp(std::string const& content);
		static TimeSpan ToTimeSpan(std::string &content);
		static CString ToCString(std::string const& content);
		static std::string FromInt(int const value);
		static std::string FromDouble(double const value);
		static std::string FromTimestamp(Timestamp const& timestamp);
		static std::string FromTimeSpan(TimeSpan const& timeSpan);
		static std::string FromCString(CString const& value);
		static std::string const FromUTF8(std::string const& value);
		static std::string const ToUTF8(std::string const& content);
		//zzw写
		static void unicodeToUTF8(const std::wstring& src,std::string& result);  
		static  void unicodeToGB2312(const std::wstring& wstr,std::string& result);
		static void utf8ToUnicode(const std::string&src,std::wstring& result);
		static void gb2312ToUnicode(const std::string&src,std::wstring& result);
		static void gb2312TOutf8(const std::string &src,std::string &result);
	    static void utf8Togb2312(const std::string& src, std::string& result);
	};
}

#endif //__UTIL_STRINGOP_H__