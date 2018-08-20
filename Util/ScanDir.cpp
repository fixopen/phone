#include "ScanDir.h"
#include "Log.h"

namespace Util {
    ProcessFileProcedure::ProcessFileProcedure(std::wstring const& directoryName)
    : directoryName_(directoryName) {
    }

    void ProcessFileProcedure::operator()(WIN32_FIND_DATA const& FindFileData) {
        //do nothing
    }

    void ProcessFiles(std::wstring const& pattern, ProcessFileProcedure& fileproc) {
        std::wstring findFilename = fileproc.directoryName_ + L"/" + pattern;
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = ::FindFirstFile(findFilename.c_str(), &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            printf("not find file\n");
        } else {
            bool finished = false;
            do {
                if (false/*FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY*/) {
                    std::wstring orgDir = fileproc.directoryName_;
                    fileproc.directoryName_ += std::wstring(L"/") + FindFileData.cFileName;
                    ProcessFiles(pattern, fileproc);
                    fileproc.directoryName_ = orgDir;
                } else {
                    fileproc(FindFileData);
                    if (!::FindNextFile(hFind, &FindFileData)) {
                        if (::GetLastError() == ERROR_NO_MORE_FILES) {
                            //printf("find end.\n");
                            finished = true;
                        } else {
                            Log::Log("Couldn't find next file.");
                        }
                    }
                }
            } while (!finished);
            ::FindClose(hFind);
        }
    }
}
