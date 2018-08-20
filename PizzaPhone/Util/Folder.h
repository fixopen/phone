#ifndef __UTIL_FOLDER_H__
#define __UTIL_FOLDER_H__

#include <string>
#include <vector>
#include "ScanDir.h"

namespace Util {
    class File;
    class Folder : public ProcessFileProcedure {
    public:
        static void CreateFolder(std::wstring const& path, std::wstring const name);
        static bool const HasFolder(std::wstring const& folderName);
        Folder(std::wstring const& directoryName, std::wstring const& pattern = L"*.*");
        size_t const GetSize() const;
        void DeleteFiles();
        void CopyTo(std::wstring const& newFolderName) const;
        bool const HasFile(std::wstring const& filename) const;
        File const GetFile(std::wstring const& filename) const;
        std::vector<File> const& GetFileList() const;
    private:
        virtual void operator()(WIN32_FIND_DATA const& FindFileData);
    private:
        std::vector<File> files_;
    };
}

#endif //__UTIL_FOLDER_H__
