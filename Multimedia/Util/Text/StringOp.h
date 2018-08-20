#ifndef __UTIL_TEXT_STRINGOP_H__
#define __UTIL_TEXT_STRINGOP_H__

#include <string>
#include <vector>

namespace Util {
    namespace Text {
        //charset, encoding, format, layout

        //if unicode project
        //  std::wstring::c_str() ==> CString
        //  static_cast<LPCTSTR>(CString) ==> std::wstring
        //else
        //  std::string::c_str() ==> CString
        //  static_cast<LPCTSTR>(CString) ==> std::string

        class StringOp {
        public: //native charset interface
            static int const ToInt(std::string const& content, size_t const mode = 10);
            static double const ToDouble(std::string const& content);
        public: //unicode charset interface
            static std::wstring const ToUpper(std::wstring const& v);
            static std::wstring const ToLower(std::wstring const& v);
        public: //unicode charset interface
            static int const ToInt(std::wstring const& content, size_t const mode = 10);
            static double const ToDouble(std::wstring const& content);

            static std::wstring const FromInt(int const value);
            static std::wstring const FromDouble(double const dbl, size_t const prec = 2);
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
}

#endif //__UTIL_TEXT_STRINGOP_H__