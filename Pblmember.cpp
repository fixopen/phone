#include "StdAfx.h"
//#include <afx.h>
#include "Pblmember.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"

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
extern BOOL g_bAdjustPanel;
void CopyDirFiles(TCHAR *src, TCHAR *des, BOOL bOverflow)
{
	g_bAdjustPanel = TRUE;
	int nFileCount = CountFolderSize(src);
	int nFileIndex = 0;

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetParam(0, 0, nFileCount, 1);

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
					Dprintf("current error is %d.", GetLastError());
				}
				nFileIndex++;
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(nFileIndex);
			//	wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				DWORD d = GetLastError();
				if (d == ERROR_NO_MORE_FILES)
				{
					Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					Dprintf("Couldn't find next file.\n");
				}
			}
			::Sleep(10);
		} while (!finished);
		FindClose(hFind);
	}
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(nFileCount);
	g_bAdjustPanel = FALSE;
}

int CopyDirFilesEx(LPCTSTR strSrc, LPCTSTR strDes, UINT nNums)
{	
	g_bAdjustPanel = TRUE;
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
					Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					Dprintf("Couldn't find next file.\n");
				}
			}
			::Sleep(10);
		} while (!finished);
		FindClose(hFind);
	}
	g_bAdjustPanel = FALSE;
	return nRes;
}


BOOL DeleteFiles(TCHAR* sDir)
{
	g_bAdjustPanel = TRUE;

	int nFileCount = CountFolderSize(sDir);
	int nFileIndex = 0;
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->m_procbarSound.SetParam(0, 0, nFileCount, 1);

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
				nFileIndex++;
				if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->IsWindowVisible())
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->m_procbarSound.SetPos(nFileIndex);
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
					nFileIndex++;
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->IsWindowVisible())
						((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->m_procbarSound.SetPos(nFileIndex);
				}
				catch (CFileException* pEx)
				{
					pEx->Delete();
				}
			}
			Sleep(10);
		}
		flag = TRUE;
	} 
	FindClose(hFind);
	g_bAdjustPanel = FALSE;
	return flag;
}

DWORD CountFolderSize(CString strPath)
{
//	g_bAdjustPanel = TRUE;
	
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->m_procbarSound.SetParam(0, 0, 4, 1);
	
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetParam(0, 0, 4, 1);

	if(strPath.Find(L"*", 0) == -1)
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
				if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->IsWindowVisible())
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->m_procbarSound.SetPos(dwSize%5);
				
				if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(dwSize%5);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					Dprintf("Couldn't find next file.\n");
				}
			}
			Sleep(10);
		} while (!finished);
		FindClose(hFind);
	}
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->m_procbarSound.SetPos(0);
				
				if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(0);
//	g_bAdjustPanel = FALSE;
	return dwSize;
}