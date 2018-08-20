#include "ScanDir.h"
#include "Log.h"

namespace Util {
    ProcessFileProcedure::ProcessFileProcedure(CString const& directoryName)
        : directoryName_(directoryName) {
    }

    void ProcessFileProcedure::operator()(WIN32_FIND_DATA const& FindFileData) {
        //do nothing
    }

    void ProcessFiles(CString const& pattern, ProcessFileProcedure& fileproc) {
        //CString findFilename = fileproc.directoryName_ + _T("/") + pattern;
        CString findFilename = fileproc.directoryName_;
        findFilename += CString(_T("\\"));
        findFilename += pattern;
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            printf("not find file\n");
        } else {
            bool finished = false;
            do {
                if (false/*FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY*/) {
                    CString orgDir = fileproc.directoryName_;
                    fileproc.directoryName_ += CString(_T("/")) + FindFileData.cFileName;
                    ProcessFiles(pattern, fileproc);
                    fileproc.directoryName_ = orgDir;
                } else {
                    fileproc(FindFileData);
                    if (!FindNextFile(hFind, &FindFileData)) {
                        if (GetLastError() == ERROR_NO_MORE_FILES) {
                            //printf("find end.\n");
                            finished = true;
                        } else {
                            Log::Log("Couldn't find next file.");
                        }
                    }
                }
            } while (!finished);
            FindClose(hFind);
        }
    }
}