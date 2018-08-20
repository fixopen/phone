#include "Folder.h"
#include "File.h"
#include <numeric>

namespace Util {
    void Folder::CreateFolder(std::wstring const& path, std::wstring const name) {
        ::CreateDirectory((path + L"/" + name).c_str(), 0);
    }

    bool const Folder::HasFolder(std::wstring const& folderName) {
        bool result = false;
        WIN32_FIND_DATA fd; 
        HANDLE hFind = ::FindFirstFile(folderName.c_str(), &fd); 
        if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            result = true;
        } 
        ::FindClose(hFind);
        return result;
    }

    Folder::Folder(std::wstring const& directoryName, std::wstring const& pattern)
    : ProcessFileProcedure(directoryName) {
        ProcessFiles(pattern, *this);
    }

    void Folder::operator()(WIN32_FIND_DATA const& FindFileData) {
        files_.push_back(File(directoryName_, FindFileData));
    }

    namespace {
        size_t const addSize(File const& left, File const& right) {
            return left.GetSize() + right.GetSize();
        }
    }

    size_t const Folder::GetSize() const {
        //return std::accumulate(files_.begin(), files_.end(), 0, addSize);
        size_t result = 0;
        for (size_t i = 0; i < files_.size(); ++i) {
            result += files_[i].GetSize();
        }
        return result;
    }

    void Folder::DeleteFiles() {
        for (size_t i = 0; i < files_.size(); ++i) {
            files_[i].Delete();
        }
        files_.clear();
    }

    void Folder::CopyTo(std::wstring const& newFolderName) const {
        for (size_t i = 0; i < files_.size(); ++i) {
            files_[i].CopyTo(newFolderName);
        }
    }

    bool const Folder::HasFile(std::wstring const& filename) const {
        bool result = false;
        for (size_t i = 0; i < files_.size(); ++i) {
            if (files_[i].GetName() == filename) {
                result = true;
                break;
            }
        }
        return result;
    }

    File const Folder::GetFile(std::wstring const& filename) const {
        File result;
        for (size_t i = 0; i < files_.size(); ++i) {
            if (files_[i].GetName() == filename) {
                result = files_[i];
                break;
            }
        }
        return result;
    }

    std::vector<File> const& Folder::GetFileList() const {
        return files_;
    }
}
