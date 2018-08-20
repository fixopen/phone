#include "StdAfx.h"
//#include <afx.h>
#include "Pblmember.h"

//¼ì²âÂ·¾¶ÊÇ·ñ´æÔÚ
BOOL DetectDIR(TCHAR *sDir)
{
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
	{ 
		flag = TRUE;
	} 
	FindClose(hFind);
	return flag;
}

BOOL DetectFile(TCHAR *sDir)
{
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if (hFind != INVALID_HANDLE_VALUE) 
	{ 
		flag = TRUE;
	} 
	FindClose(hFind);
	return flag;
}

void CopyDirFiles(TCHAR *src, TCHAR *des, BOOL bOverflow)
{
	CString SrcDir = src;
	CString DesDir = des;
	CString findFilename = SrcDir + "/*.*";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
//		Dprintf("not find file\n");
	}
	else
	{
		bool finished = false;
		do
		{
			wchar_t wideToName[256];
			//	wchar_t root[256] = {0};
			//	mbstowcs(root, rootPath.c_str(), rootPath.length());
			wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			//	wideToName =  
			wchar_t wideFromName[256] = {0};
			//	wchar_t usb[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
				//CopyDirFiles(wideFromName, wideToName);
			}
			else
			{
				if (!::CopyFile(wideFromName, wideToName, !bOverflow))
				{
				//	Dprintf("current error is %d.", GetLastError());
				}
				//wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					//Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					//Dprintf("Couldn't find next file.\n");
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
}

int CopyDirFilesEx(LPCTSTR strSrc, LPCTSTR strDes, UINT nNums)
{		
	if (!strSrc || !strDes || nNums<1) return 0;
	UINT nRes = 0;
	
	CString strPath(strSrc);
	strPath += 	_T("\\*.*");
	WIN32_FIND_DATA FindFileData;
	
	HANDLE hFind = FindFirstFile((LPCTSTR)strPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//		Dprintf("not find file\n");
	}
	else
	{
		bool finished = false;
		do
		{
			wchar_t wideToName[256];
			wsprintf(wideToName, _T("%s/%s"), strDes, (LPCTSTR)FindFileData.cFileName); //findFileName
			wchar_t wideFromName[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), strSrc, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
				//CopyDirFiles(wideFromName, wideToName);
			}
			else
			{
				if (::CopyFile(wideFromName, wideToName, false))
				{
					//	Dprintf("current error is %d.", GetLastError());
					if (nRes < nNums) 
						nRes++;
					else
						break;
				}
				//wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					//Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					//Dprintf("Couldn't find next file.\n");
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
	return nRes;
}


BOOL DeleteFiles(TCHAR* sDir)
{
	CString s = sDir;
	s = s.Left(s.GetLength() - 1);
	
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE)) 
	{ 
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			try
			{
				CFile::Remove(s + fd.cFileName);
			}
			catch (CFileException* pEx)
			{
				pEx->Delete();
			}
		}
		
		while(FindNextFile(hFind, &fd))
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				try
				{
					CFile::Remove(s + fd.cFileName);
				}
				catch (CFileException* pEx)
				{
					pEx->Delete();
				}
			}
		}
		flag = TRUE;
	} 
	FindClose(hFind);
	return flag;
}

DWORD CountFolderSize(CString strPath)
{
	strPath += _T("\\*.*");
	DWORD dwSize = 0;
	WIN32_FIND_DATA FindFileData;
	
	HANDLE hFind = FindFirstFile((LPCTSTR)strPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//		Dprintf("not find file\n");
	}
	else
	{
		bool finished = false;
		do
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				dwSize++;
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					//Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					//Dprintf("Couldn't find next file.\n");
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
	return dwSize;
}