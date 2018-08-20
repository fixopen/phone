#ifndef __UTIL_SCANDIR_H__
#define __UTIL_SCANDIR_H__

namespace Util {
    class ProcessFileProcedure
    {
        friend void ProcessFiles(CString const pattern, ProcessFileProcedure& fileproc);
    public:
        ProcessFileProcedure(CString const directoryName);
        virtual void operator()(WIN32_FIND_DATA const& FindFileData);
    protected:
        CString directoryName_;
    };

    void ProcessFiles(CString const pattern, ProcessFileProcedure& fileproc);
}

#endif //__UTIL_SCANDIR_H__
