#ifndef PUBLIC_MEMBER_
#define PUBLIC_MEMBER_

//#include "StdAfx.h"
#include <WinDef.h>
struct HttpParam {
    int apn_type;
    char* apn_proxy;
    int type;    //post or get
    int dwType;  //0 mediadowload   1 weather  2 menumanager
    char* url;
    char* lfilename;
    char* cIMEI;
    char* cTerminalId;
    char* pData;
    int dataLentg;
};

extern BOOL DetectDIR(TCHAR* sDir);
extern BOOL DetectFile(TCHAR* sDir);
extern void CopyDirFiles(const TCHAR* src, const TCHAR* des, BOOL bOverflow = TRUE);
extern void CopyDirectoryFiles(TCHAR* src, TCHAR* des, DWORD& freeBytes, BOOL bOverflow = TRUE);
extern int CopyDirFilesEx(LPCTSTR strSrc, LPCTSTR strDes, UINT nNums = 1);
extern BOOL DeleteFiles(TCHAR* sDir);
extern DWORD CountFolderSize(CString strPath);
extern DWORD GetFileSize(TCHAR* sFile);
extern DWORD GetDirSize(TCHAR* sDir, DWORD& fSize);

#endif