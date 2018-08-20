#ifndef PUBLIC_MEMBER_
#define PUBLIC_MEMBER_

//#include "StdAfx.h"
#include <WinDef.h>

extern BOOL DetectDIR(TCHAR *sDir);
extern BOOL DetectFile(TCHAR *sDir);
extern void CopyDirFiles(TCHAR *src, TCHAR *des, BOOL bOverflow=TRUE);
extern int CopyDirFilesEx(LPCTSTR strSrc, LPCTSTR strDes, UINT nNums=1);
extern BOOL DeleteFiles(TCHAR* sDir);
extern DWORD CountFolderSize(CString strPath);

#endif