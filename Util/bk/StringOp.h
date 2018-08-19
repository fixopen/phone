#ifndef __UTIL_STRINGOP_H__
#define __UTIL_STRINGOP_H__

#include "Symbols.h"
#include "DateTime.h"

#include <string>
#include <vector>

namespace Util {
    //if unicode project
    //  std::wstring::c_str() ==> CString
    //  static_cast<LPCTSTR>(CString) ==> std::wstring
    //else
    //  std::string::c_str() ==> CString
    //  static_cast<LPCTSTR>(CString) ==> std::string

    class SYMBOL StringOp {
    public: //native charset interface
        static int const ToInt(std::string const& content, size_t const mode = 10);
        static double const ToDouble(std::string const& content);
        static CRect const ToRectangle(std::string const& content);//[(x0,y0)(x1,y1)]
        static Timestamp const ToTimestamp(std::string const& content);
        static TimeSpan const ToTimeSpan(std::string const& content);
    public: //unicode charset interface
        static int const ToInt(std::wstring const& content, size_t const mode = 10);
        static double const ToDouble(std::wstring const& content);
        static CRect const ToRectangle(std::wstring const& content);//[(x0,y0)(x1,y1)]
        static Timestamp const ToTimestamp(std::wstring const& content);
        static TimeSpan const ToTimeSpan(std::wstring const& content);

        static std::wstring const FromInt(int const value);
        static std::wstring const FromDouble(double const dbl);
        static std::wstring const FromRectangle(CRect const& rect);
        static std::wstring const FromTimestamp(Timestamp const& timestamp);
        static std::wstring const FromTimeSpan(TimeSpan const& timeSpan);
    public: //string split merge and trim
        static std::vector<std::wstring> const Split(std::wstring const& value, std::wstring const& delimeter);
        static std::wstring const Merge(std::vector<std::wstring> const& value, std::wstring const& delimeter);
        static std::vector<std::string> const Split(std::string const& value, std::string const& delimeter);
        static std::string const Merge(std::vector<std::string> const& value, std::string const& delimeter);
        static std::wstring const Trim(std::wstring const& value, std::wstring const& spaceChars = L" ");
        static std::string const Trim(std::string const& value, std::string const& spaceChars = " ");
    public: //native <=> unicode
        static std::wstring const ToUnicode(std::string const& content);
        static std::string const FromUnicode(std::wstring const& unicode);
    public: //unicode <=> utf8
        static std::string const ToUTF8(std::wstring const& unicode);
        static std::wstring const FromUTF8(std::string const& utf8);
    public: //base16 <=> binary
        static std::string const ToBase16(std::string const& binary);
        static std::string const ToBinary(std::string const& base16);
        static std::string const Base16ToArray(std::string const& v);
    //public: //base64 <=> binary
    //    static std::string const ToBase64(std::string const& binary);
    //    static std::string const ToBinary(std::string const& base64);
    };
}

#endif //__UTIL_STRINGOP_H__