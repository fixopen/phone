#ifndef __UTIL_SCANNER_H__
#define __UTIL_SCANNER_H__

#include <utility> //for std::pair
#include <string> //for std::string
//#include "../Mms/Data/MmsMessage.h" //for ContentType

namespace Util {
    namespace Scanner {
        typedef std::pair<size_t, std::string> OptType;
        size_t const ScanShortInteger(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        size_t const ScanLongInteger(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        size_t const ScanVarInteger(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        size_t const ScanLength(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        size_t const ScanDate(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        size_t const ScanIntegerParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        size_t const ScanDeltaSecondsParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanToken(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanText(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanQuotedString(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanExtensionMedia(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanStringParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanUriParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        std::string const ScanFrom(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        OptType const ScanConstrainedEncoding(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        OptType const ScanConstrainedMedia(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        OptType const ScanVersionParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
        OptType const ScanContentType(unsigned char const* const data, size_t const maxLength, size_t& currentPos);
    }
}

#endif //__UTIL_SCANNER_H__
