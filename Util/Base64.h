#ifndef __UTIL_BASE64_H__
#define __UTIL_BASE64_H__

#include <string>

namespace Util
{
    class Base64 {
    public:
        union Convert {
            struct SixBits {
                unsigned fourth : 6;
                unsigned third : 6;
                unsigned second : 6;
                unsigned first : 6;
            } sixBits;
            char octetBits[4];
            unsigned int whole;
        };
        static std::string const Encode(std::string const & content);
        static std::string const Decode(std::string const & content);
        static char* ConvertTable/*[64]*/;
    private:
        static int const revConvert_(char const c);
        static int eqCount_;
    };
}

#endif //__UTIL_BASE64_H__
