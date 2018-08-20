#ifndef __UTIL_DOCSOURCE_H__
#define __UTIL_DOCSOURCE_H__

#include <string>
#include <cstdio>

namespace Util {
    class DocSource {
    public:
        DocSource(std::string const& filename);
        ~DocSource();
        int GetChar();
        int GetPosition();
        std::wstring const GetContent(int const start, int const stop);
        bool const IsEnd() const;
    private:
        std::FILE* file_;
    };
}

#endif //__UTIL_DOCSOURCE_H__
