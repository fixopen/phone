#include "File.h"
#include <stdio.h>
#include "Text/StringOp.h"

namespace Util {
    bool const File::HasFile(std::wstring const& folderName) {
        bool result = false;
        WIN32_FIND_DATA fd; 
        HANDLE hFind = ::FindFirstFile(folderName.c_str(), &fd); 
        if (hFind != INVALID_HANDLE_VALUE) {
            result = true;
        } 
        ::FindClose(hFind);
        return result;
    }

    File::File()
    : size_(0) {
    }

    File::File(std::wstring const& filespec)
    : size_(0)
    , fullname_(filespec) {
        size_t pos = fullname_.find_last_of(L'/');
        if (pos != std::wstring::npos) {
            filename_ = fullname_.substr(pos);
            directory_ = fullname_.substr(0, fullname_.length() - pos);
        } else {
            filename_ = fullname_;
        }
        //FILE* file = 0;
        //fopen_s(&file, Text::StringOp::ToUTF8(fullname_).c_str(), "a");
        FILE* file = fopen(Text::StringOp::ToUTF8(fullname_).c_str(), "a");
        if (file) {
            size_ = ftell(file);
            fclose(file);
        }
    }

    File::File(std::wstring const& dir, WIN32_FIND_DATA const& FindFileData)
    : size_(FindFileData.nFileSizeLow)
    , directory_(dir)
    , filename_(FindFileData.cFileName) {
        fullname_ = directory_ + L"/" + filename_;
    }

    size_t const File::GetSize() const {
        return size_;
    }

    std::wstring const File::GetPath() const {
        return directory_;
    }

    std::wstring const File::GetName() const {
        return filename_;
    }

    void File::CopyTo(std::wstring const& destinationDirectory) const {
        ::CopyFile(fullname_.c_str(), (destinationDirectory + L"/" + filename_).c_str(), FALSE);
    }

    void File::Delete() {
        ::DeleteFile(fullname_.c_str());
    }
}
