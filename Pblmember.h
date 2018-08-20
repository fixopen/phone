#ifndef PUBLIC_MEMBER_
#define PUBLIC_MEMBER_

//#include "StdAfx.h"
#include <WinDef.h>
struct HttpParam
{
	int apn_type;
	char *apn_proxy;
	int type;    //post or get
	int dwType;  //0 mediadowload   1 weather  2 menumanager
	char *url;
	char *lfilename;
	char *cIMEI;
	char *cTerminalId;
	char *pData;
	int  dataLentg;
};

extern BOOL DetectDIR(TCHAR *sDir);
extern BOOL DetectFile(TCHAR *sDir);
extern void CopyDirFiles(TCHAR *src, TCHAR *des, BOOL bOverflow=TRUE);
extern void CopyDirectoryFiles(TCHAR *src, TCHAR *des, DWORD &freeBytes, BOOL bOverflow=TRUE);
extern int CopyDirFilesEx(LPCTSTR strSrc, LPCTSTR strDes, UINT nNums=1);
extern BOOL DeleteFiles(TCHAR* sDir);
extern DWORD CountFolderSize(CString strPath);
extern double GetFileSize(TCHAR *sFile);
extern double GetDirSize(TCHAR *sDir, float &fSize);

extern ULONGLONG GetDirAllFileSize(CString path);//获得一个目录下文件的大小
extern void GetDiskStatus(ULONGLONG &free,ULONGLONG &total,ULONGLONG &uleft);//获得 Flashdrv磁盘目录下的剩余空间 和 总空间

#endif