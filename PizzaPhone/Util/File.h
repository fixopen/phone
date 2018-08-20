#ifndef __UTIL_FILE_H__
#define __UTIL_FILE_H__

#include <string>
#include <winbase.h>

namespace Util {
    class File {
    public:
        static bool const HasFile(std::wstring const& fileName);
        File();
        File(std::wstring const& filespec);
        File(std::wstring const& dir, WIN32_FIND_DATA const& FindFileData);
        size_t const GetSize() const;
        std::wstring const GetPath() const;
        std::wstring const GetName() const;
        void CopyTo(std::wstring const& destinationDirectory) const;
        void Delete();
    private:
        size_t size_;
        std::wstring directory_;
        std::wstring filename_;
        std::wstring fullname_;
    };
}

#endif //__UTIL_FILE_H__
