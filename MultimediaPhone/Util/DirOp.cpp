#include "stdafx.h"

#include "ScanDir.h"

#include <string>
#include <vector>

#include "StringOp.h"

class FileLister : public Util::ProcessFileProcedure
{
public:
    FileLister(CString const directoryName, std::string& result)
    : ProcessFileProcedure(directoryName)
    , result_(result)
    {
    }

    void operator()(WIN32_FIND_DATA const& FindFileData)
    {
        //std::string findedFilename = Util::StringOp::FromCString(CString(FindFileData.cFileName));
        //result_ += "<ITEM>\n\t<FILENAME>";
        //result_ += findedFilename;
        //result_ += "</FILENAME>\n\t<FILESIZE>";
        //result_ += Util::StringOp::FromInt((FindFileData.nFileSizeHigh << 32) | FindFileData.nFileSizeLow);
        //result_ += "</FILESIZE>\n</ITEM>";
    }
private:
    std::string& result_;
};

std::string const ListFiles(std::string directoryName)
{
    std::string result;
    FileLister fl(Util::StringOp::ToCString(directoryName), result);
    ProcessFiles(_T("*.*"), fl); //const_cast<FileLister>
    return result;
}


class FileDeleter : public Util::ProcessFileProcedure
{
public:
    FileDeleter(CString const directoryName)
    : ProcessFileProcedure(directoryName)
    {
    }

    void operator()(WIN32_FIND_DATA const& FindFileData)
    {
        CString name = directoryName_ + "/" + CString(FindFileData.cFileName);
        if (!::DeleteFile((LPCTSTR)name))
        {
            printf("delete file error is %d.", GetLastError());
        }
    }
};

void DeleteFiles(CString const directoryName)
{
    ProcessFiles(_T("*.*"), FileDeleter(directoryName));
}

class FileCopyer : public Util::ProcessFileProcedure
{
public:
    FileCopyer(CString const fromDirectoryName, CString const toDirectoryName)
    : ProcessFileProcedure(fromDirectoryName)
    , toDirectoryName_(toDirectoryName)
    {
    }

    void operator()(WIN32_FIND_DATA const& FindFileData)
    {
        CString toName = toDirectoryName_ + "/" + CString(FindFileData.cFileName);
        CString fromName = directoryName_ + "/" + CString(FindFileData.cFileName);
        if (!::CopyFile(fromName, toName, false))
        {
            printf("copy file error is %d.", GetLastError());
        }
    }
private:
    CString toDirectoryName_;
};

void CopyFiles(CString const fromDirectoryName, CString const toDirectoryName)
{
    ProcessFiles(_T("*.*"), FileCopyer(fromDirectoryName, toDirectoryName));
}

class BillFinder : public Util::ProcessFileProcedure
{
public:
    BillFinder(CString const directoryName, std::vector<std::string>& findedFiles)
    : ProcessFileProcedure(directoryName)
    , findedFiles_(findedFiles)
    {
    }

    void operator()(WIN32_FIND_DATA const& FindFileData)
    {
        //findedFiles_.push_back(Util::StringOp::FromCString(CString(FindFileData.cFileName)));
    }
private:
    std::vector<std::string>& findedFiles_;
};

std::vector<std::string> FindBills(CString const directoryName, Util::Timestamp const& date)
{
    wchar_t pattern[256] = {0};
    wsprintf(pattern, _T("/Log/%04d%02d%02d*.bil"), date.GetYear(), date.GetMonth(), date.GetDay());
    //, std::string pattern,
    std::vector<std::string> result;
    ProcessFiles(pattern, BillFinder(directoryName, result));
    return result;
}
