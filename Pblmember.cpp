#include "StdAfx.h"
//#include <afx.h>
#pragma warning(disable: 4786)
#include <string>
#pragma warning(disable: 4786)
#include <vector>
#include "./util/HTTPTransport.h"
#include "Pblmember.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "./data/OTAData.h"
#include "./Util/Base64.h"
#include "./util/unzip.h"
#include "./Util/MMSWarp.h"

extern int g_SignalSize;
extern std::string g_messageID;
extern std::string g_recipientAddress;
extern std::string g_senderAddress;

extern BOOL g_isAutoPlay;
extern BOOL g_bReplay;

BOOL FindVideoDlg(BOOL flag)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	BOOL isFind = FALSE;
	if(main->m_pMainDlg->m_mainVideoDlg_->m_IsPlay == 1 || g_isAutoPlay)  //影院 相册
	{
		isFind = TRUE;
		if(flag)
		{
			main->m_pMainDlg->StopVideo(FALSE, 0, 0);
			g_bReplay = TRUE;
		}
	}

	return isFind;
}

//检测路径是否存在
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

void CopyDirectoryFiles(TCHAR *src, TCHAR *des, DWORD &freeBytes, BOOL bOverflow)
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
				DWORD fileBytes = GetFileSize(wideFromName);
				if(freeBytes > fileBytes)
				{
					freeBytes -= fileBytes;
					if (!::CopyFile(wideFromName, wideToName, !bOverflow))
					{
						Dprintf("current error is %d.", GetLastError());
					}
					nFileIndex++;
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(nFileIndex);
				}
				else
				{
					freeBytes = 0;
					return;
				}
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

double GetFileSize(TCHAR *sFile)
{
	double dwSize = 0;
	if(DetectFile(sFile))
	{
		CFile cfile;
		if (cfile.Open(sFile, CFile::modeRead))
		{
			dwSize = cfile.GetLength();
		}
	}
	return dwSize;
}

double GetDirSize(TCHAR *sDir, double &fSize)
{
	WIN32_FIND_DATA  fileInfo;   
	HANDLE           hFind;   
	CString srcPath = sDir;
	CString path = srcPath + "/*.*";
	hFind = FindFirstFile(path, &fileInfo);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		bool finished = false;
		do
		{
			wchar_t pathName[256];
			wsprintf(pathName, _T("%s/%s"), srcPath, (LPCTSTR)fileInfo.cFileName);
			if((fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				GetDirSize(pathName, fSize);
			}
			else
			{
				//fSize += fileInfo.nFileSizeHigh * (MAXDWORD+1) + fileInfo.nFileSizeLow;
				fSize += GetFileSize(pathName);
			}
			
			if (!FindNextFile(hFind, &fileInfo))
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
		}while(!finished);
		FindClose(hFind);
	}
	return fSize;
}

BOOL DeleteFiles(TCHAR* sDir)
{
	g_bAdjustPanel = TRUE;

	int nFileCount = CountFolderSize(sDir);
	int nFileIndex = 0;

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
	g_bAdjustPanel = TRUE;

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
				
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(0);
	g_bAdjustPanel = FALSE;
	return dwSize;
}

CString GetFileName(CString pathname) 
{ 
	for( int i=pathname.GetLength()-1; i>=0; i-- ) 
	{ 
		if(pathname[i]== '/') 
			break; 
	} 
	return pathname.Mid( i+1 ); 
}

#include <Afxinet.h>

//int ype   0 文件下载   1 天气预报 2 菜单维护  3 xxxx 4 注册回应
int dowith_Parse(char *ptr, int type)
{
	std::string str = (char *)ptr;
	if(str.find("utf-8")>0 || str.find("UTF-8")>0)
	{
		str = Util::StringOp::FromUTF8(str);
	}
	TCHAR *root_dir[] = {L"/flashdrv/3g/file/", L"/flashdrv/3g/weather/", L"/flashdrv/3g/menu/"};
	char *root_dir_[] = {"/flashdrv/3g/file/", "/flashdrv/3g/weather/", "/flashdrv/3g/menu/"};
	int iDir = type;
	if(type == 0)      //
	{
		std::vector<Data::MultimediaDownload> result = Data::MultimediaDownload::Parse(str);
		for(int i = 0; i < result.size(); i++)
		{
			CString savePath = root_dir[iDir];
			SYSTEMTIME curtime;
			GetLocalTime(&curtime);
			char txt[32];
			sprintf(txt, "%04d%02d%02d%02d%02d%02d/", curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
			savePath += txt;
			std::string r = Util::Base64::Decode(result[i].multimediaInfos.content);
			
			FILE *file = fopen("/flashdrv/3g/temp.zip", "w");
			if(file)
			{
				fwrite(r.c_str(), sizeof(char), r.length(), file);
				fclose(file);
			}
			
			//解压
			HZIP hz;  
			TRACE(L"start unzip!\r\n");
			char *pChar = (char *)r.c_str();
			hz = OpenZip((void *)(r.c_str()), r.length(), 0);
			char *filename = (char *)result[i].multimediaInfos.filename.c_str();
			TCHAR tFileName[128] = {0};
			mbstowcs(tFileName, filename, strlen(filename));
			BOOL flag = FALSE;
			SetUnzipBaseDir(hz, /*root_dir[iDir]*/ savePath);
			ZIPENTRY ze;
			GetZipItem(hz,-1,&ze);
			int numitems=ze.index;
			for (int zi=0; zi<numitems; zi++)
			{ 
				GetZipItem(hz,zi,&ze);
				if(wcsstr(ze.name, tFileName))
				{
					flag = TRUE;
					wsprintf(tFileName, L"%s%s", /*root_dir[iDir]*/ savePath, ze.name);
				}
				UnzipItem(hz,zi,ze.name);
				::Sleep(0);
				Dprintf("continue unzip!\r\n");
			}
			CloseZip(hz);
			if(flag)    //找到文件保存到数据库中，否则不保存
			{
				char fname[128] = {0};
				wcstombs(fname, tFileName, wcslen(tFileName)*2);
				result[i].multimediaInfos.filename = fname;
				result[i].multimediaInfos.content = "";
				result[i].Insert();
			}
			Dprintf("end unzip!\r\n");
		}
	}
	else if(type == 1)		//天气预报
	{
		std::vector<Data::Weather> result = Data::Weather::Parse(str);
		for(int i = 0 ; i < result.size(); i++ )
		{
			std::string filter = "area = '";
			filter += result[i].weatherInfos.area;
			filter += "' AND datetime = '";
			filter += Util::StringOp::FromTimestamp(result[i].weatherInfos.datetime);
			filter += "'";
			std::vector<boost::shared_ptr<Data::Weather> > m_result = Data::Weather::GetFromDatabase(filter); 
			if(m_result.size() > 0)
			{
				
				m_result[0]->weatherInfos.wind = result[i].weatherInfos.wind; //Wind
                m_result[0]->weatherInfos.temperature = result[i].weatherInfos.temperature; //TemperaTure
                m_result[0]->weatherInfos.code = result[i].weatherInfos.code; //WeatherCode
				m_result[0]->weatherInfos.description = result[i].weatherInfos.description; //OtherInfo
				m_result[0]->Update();
			}
			else
				result[i].Insert();
		}
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SetWeather();
	}
	else if(type == 2)   //menumanage
	{
		std::vector<Data::MenuManager> result = Data::MenuManager::Parse(str);
		for(int i = 0; i < result.size(); i++)
		{
			std::string r = Util::Base64::Decode(result[i].menuInfos.imageContent);
			HZIP hz;  
			TRACE(L"start unzip!\r\n");
			char *pChar = (char *)r.c_str();
			hz = OpenZip((void *)(r.c_str()), r.length(), 0);
			char *filename = (char *)result[i].menuInfos.imageName.c_str();
			TCHAR tFileName[128] = {0};
			mbstowcs(tFileName, filename, strlen(filename));
			BOOL flag = FALSE;
			SetUnzipBaseDir(hz, root_dir[iDir]);
			ZIPENTRY ze;
			GetZipItem(hz,-1,&ze);
			int numitems=ze.index;
			for (int zi=0; zi<numitems; zi++)
			{ 
				GetZipItem(hz,zi,&ze);
				if(wcsstr(ze.name, tFileName))
				{
					flag = TRUE;
					wsprintf(tFileName, L"%s%s", root_dir[iDir], ze.name);
				}
				UnzipItem(hz,zi,ze.name);
				::Sleep(0);
				Dprintf("continue unzip!\r\n");
			}
			CloseZip(hz);

			r = Util::Base64::Decode(result[i].menuInfos.specImageContent);
			HZIP hz1;  
			TRACE(L"start unzip!\r\n");
			char *pChar1 = (char *)r.c_str();
			hz1 = OpenZip((void *)(r.c_str()), r.length(), 0);
			char *filename1 = (char *)result[i].menuInfos.specImageName.c_str();
			TCHAR tFileName1[128] = {0};
			mbstowcs(tFileName1, filename1, strlen(filename1));
			BOOL flag1 = FALSE;
			SetUnzipBaseDir(hz1, root_dir[iDir]);
			ZIPENTRY ze1;
			GetZipItem(hz1,-1,&ze1);
			int numitems1=ze1.index;
			for (int zi1=0; zi1<numitems1; zi1++)
			{ 
				GetZipItem(hz1,zi1,&ze1);
				if(wcsstr(ze1.name, tFileName1))
				{
					flag1 = TRUE;
					wsprintf(tFileName1, L"%s%s", root_dir[iDir], ze1.name);
				}
				UnzipItem(hz1,zi1,ze1.name);
				::Sleep(0);
				Dprintf("continue unzip!\r\n");
			}
			CloseZip(hz1);

			if(flag)    //找到文件保存到数据库中，否则不保存
			{
				char fname[128] = {0};
				wcstombs(fname, tFileName, wcslen(tFileName)*2);
				char fname1[128] = {0};
				wcstombs(fname1, tFileName1, wcslen(tFileName1)*2);
				result[i].menuInfos.imageName = fname;
				result[i].menuInfos.imageContent = "";
				result[i].menuInfos.specImageName = fname1;
				result[i].menuInfos.specImageContent = "";
				result[i].Insert();
			}
			Dprintf("end unzip!\r\n");
			/*
			char txtFileName[128];
			sprintf(txtFileName, "%s%s", root_dir_[iDir], result[i].menuInfos.imageName.c_str());
			FILE *file = fopen(txtFileName, "w");
			if(file)
			{
				fwrite(r.c_str(), sizeof(char), r.length(), file);
				fclose(file);
			}
			
			r = Util::Base64::Decode(result[i].menuInfos.specImageContent);
			char txtFileName1[128];
			sprintf(txtFileName1, "%s%s", root_dir_[iDir], result[i].menuInfos.specImageName.c_str());
			file = fopen(txtFileName1, "w");
			if(file)
			{
				fwrite(r.c_str(), sizeof(char), r.length(), file);
				fclose(file);
			}

			//解压
			result[i].menuInfos.imageName = txtFileName;
			result[i].menuInfos.specImageName  = txtFileName1;
			result[i].menuInfos.imageContent = "";
			result[i].menuInfos.specImageContent  = "";
			result[i].Insert();

			*/
		}
	}
	else if(type == 4)   //注册回应
	{

		Data::RegisterReply result = Data::RegisterReply::Parse(str);
		if(result.result == 0)
		{

			std::vector<boost::shared_ptr<Data::RegisterReply> > m_result = Data::RegisterReply::GetFromDatabase("");
			if(m_result.size() == 0 || m_result[0]->areaCode != result.areaCode)
			{
				Data::RegisterReply::Remove("");
				result.Insert();
			}
		}
		return result.result;
	}
	
	Dprintf(L"解析完成\r\n");
	return 0;
}

BOOL FindHttpEnd(char *pBuf, int &status, int pLen, int &nOffLen)
{
	static int zLength = 0;
	int len = 0;
	if(status == 0)
	{
		char *ptr = strstr(pBuf, "\r\n\r\n");
		
		int len = 0;
		if(ptr)
		{
			char *lPtr;

			lPtr = strstr(pBuf, "Content-Length:");
			len = strlen("Content-Length:");
			if(!lPtr)
			{
				lPtr = strstr(pBuf, "content-length:");
				len = strlen("content-length:");
				if(!lPtr)
				{
					lPtr = strstr(pBuf, "Content-length:");
					len = strlen("Content-length:");
					if(!ptr)
					{
						lPtr = strstr(pBuf, "Content-Length :");
						len = strlen("Content-Length :");
						if(!lPtr)
						{
							lPtr = strstr(pBuf, "content-length :");
							len = strlen("content-length :");
							if(!lPtr)
							{
								lPtr = strstr(pBuf, "Content-length :");
								len = strlen("Content-length :");
							}
						}
					}
				}	
			}

			if(lPtr)
			{
				char *ePtr = strstr(lPtr+len, "\r\n");
				if(ePtr)
				{
					char txt[12] = {0};
					memcpy(txt, lPtr+len, (ePtr - lPtr - len));
					CString s = txt;
					s.TrimLeft();
					s.TrimRight();
					int length1 = Util::StringOp::ToInt(s);
					zLength = length1 + ptr - pBuf + 4;
					status = 1;
					nOffLen = ptr - pBuf + 4;

					if(pLen >= zLength)
						return TRUE;
				}
			}
		}
	}
	else if(status == 1)
	{
		if(pLen >= zLength)
			return TRUE;
	}
	return FALSE;
}

//release dialed   20090619
int HttpProcesse(void *pParam)
{
	extern VOID WriteMyLog_(char *ptr, int size);
	WriteMyLog_("HttpProcesse",strlen("HttpProcesse"));

	Dprintf("HttpProcesse:\r\n");
	int ret = -1;
	HttpParam *pHttpParam = (HttpParam *)pParam;
	CString strSentence;
	std::wstring strFileName = Util::StringOp::ToCString(pHttpParam->url);
 // CString strFileName = pHttpParam->url;
	
	char log[100] = {0};
	sprintf(log,"url:%s",pHttpParam->url);
	WriteMyLog_(log,strlen(log));

//	Dprintf(pHttpParam->url);
//	Dprintf("\r\n");

	CString strFormData = "";    // 需要提交的数据
    CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); //请求头

	Util::HTTPTransport Transport;
	Dprintf("HttpProcesse:0\r\n");
	WriteMyLog_("HttpProcesse:0",strlen("HttpProcesse:0"));

	Transport.SetAPN((Util::DIAL_APN)pHttpParam->apn_type, pHttpParam->apn_proxy);
	Dprintf("HttpProcesse:1\r\n");
	WriteMyLog_("HttpProcesse:1",strlen("HttpProcesse:1"));

	if(pHttpParam->type == 2 || pHttpParam->type == 11 || pHttpParam->type == 12)    //发送彩信
	{
		
		if(pHttpParam->type == 2)
		{
			Transport.message.headers.insert(std::make_pair(std::wstring(L"Content-Type"), std::wstring(L"application/vnd.wap.mms-message")));
			
			Transport.message.headers.insert(std::make_pair(std::wstring(L"Accept"), std::wstring(L"application/vnd.wap.mms-message,text/plain,*/*")));
			Transport.message.headers.insert(std::make_pair(std::wstring(L"User-Agent"), std::wstring(L"CeHttp")));
		}

		TCHAR sLen[32];
		wsprintf(sLen, L"%d", pHttpParam->dataLentg);
		std::wstring sLen_ = sLen;
		Transport.message.headers.insert(std::make_pair(std::wstring(L"CONTENT-LENGTH"), sLen_));
		Transport.message.body = std::string(pHttpParam->pData, pHttpParam->dataLentg);
		Transport.SetPost(pHttpParam->pData, pHttpParam->dataLentg);
		
		Dprintf("Post start:\r\n");
		WriteMyLog_("Post start",strlen("Post start"));

	//	DWORD r_ = Transport.Post((LPCTSTR)strFileName);
		//DWORD r_ = Transport.Post(strFileName);
		DWORD r_ = Transport.PostMms(strFileName, (UINT8*)pHttpParam->pData, pHttpParam->dataLentg);
		
		Dprintf("Post mms error %d\r\n", r_);

		sprintf(log,"Post mms error: %d",r_);
		WriteMyLog_(log,strlen(log));

		if(r_ == 0 )
		{	
			int status = 0;
			int nOffLength = 0;
			DWORD start = GetTickCount();

			int size = 0;
			BYTE pBuf[1024*4+1] = {0};
			int nCount = 1;
			while(nCount != 0)
			{
// 				nCount = Transport.Recv(pBuf+size, 128);
// 				size += nCount;
				Dprintf("Recive\r\n");
				nCount = Transport.Recv(pBuf+size, 128);
				Dprintf("Recive_End\r\n");
				size += nCount;

			/*	if( (nCount < 128 && nCount != 0))
				{
					Dprintf("nCount: %d",nCount);
					break;
				}
				*/
				
// 				DWORD end = GetTickCount() - start;
// 				if(end >= 300000)
// 				{
// 					sprintf(log, "Get MMS Error The SignalSize is %d\r\n", g_SignalSize);
// 					WriteMyLog_(log, strlen(log));
// 					
// 					Dprintf("Get TimeOUT");
// 					size = 0;
// 					break;
// 				}
								
				if(FindHttpEnd((char *)pBuf, status, size, nOffLength))
				{
					Dprintf("Get Over\r\n");
					break;
				}
				else if(size > (1024*4-128 + 1))
				{	
					Dprintf("Get Out\r\n");
					break;
				}
				Sleep(10);

			} 
			Dprintf((char *)pBuf);
			
			memset(log,0,100);
			sprintf(log,"size:%d",size);
			WriteMyLog_(log,strlen(log));

			WriteMyLog_("Recv Data:",strlen("Recv Data:"));
			WriteMyLog_((char*)pBuf,strlen((char*)pBuf));

			if(pHttpParam->type == 2)
				ret = MMS::MMSWarp::GetMMSWarp()->DecodeSubmitResponse(pBuf, size);
			else if(pHttpParam->type == 12)
				ret = MMS::MMSWarp::GetMMSWarp()->DecodeReadOriginatorRequest(pBuf, size);
			else
				ret = 0;

			char retLog[30] = {0};
			sprintf(retLog,"ret_:%d",ret);
			WriteMyLog_(retLog,strlen(retLog));
		}
				
	}
	else	//get
	{
		Dprintf("Get start:\r\n");
		WriteMyLog_("Get start:",strlen("Get start:"));
				
	//	DWORD r_ = Transport.Get_((LPCTSTR)strFileName);
		DWORD r_ = Transport.Get_(strFileName);
		Dprintf("Get error %d\r\n", r_);
		sprintf(log,"Get error: %d",r_);
		WriteMyLog_(log,strlen(log));

		if(r_ == 0 )
		{
			Dprintf("Get Start\r\n");
			BYTE  *pBuf = new BYTE[1024*1024];
			memset(pBuf, 0, 1024*1024);
			int size = 0;
			int nCount = 1;

			int status = 0;
			int nOffLength = 0;
			DWORD start = GetTickCount();
			while(nCount != 0)
			{	
				Dprintf("Get Recv\r\n");
				Sleep(10);
				nCount = Transport.Recv(pBuf+size, 512*1024);
				size += nCount;

			/*	if( nCount < 128 && nCount != 0)
				{
					Dprintf("nCount: %d",nCount);
					break;
				}
				*/
								
				DWORD end = GetTickCount() - start;
				if(end >= 300000)
				{
					sprintf(log, "Get MMS Error The SignalSize is %d\r\n", g_SignalSize);
					WriteMyLog_(log, strlen(log));

					Dprintf("Get TimeOUT");
					memset(pBuf, 0, 1024*1024);
					size = 0;
					break;
				}
				

				if(FindHttpEnd((char *)pBuf, status, size, nOffLength))
				{
					Dprintf("Get Over\r\n");
					break;
				}
				else if(size > (1024*1204-1024))
				{	
					Dprintf("Get Out\r\n");
					break;
				}

			} 
			Dprintf("Get FindHttpEnd \r\n");

			if(pHttpParam->dwType < 10)
				ret = dowith_Parse((char *)(pBuf+nOffLength), pHttpParam->dwType);
			else if(pHttpParam->dwType == 10) //彩信
			{
				CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

				CFile file;
				static int filecount = 10;
				WCHAR text[30];
				swprintf(text,L"FlashDrv/%d.txt",filecount++);
				if(file.Open(text, CFile::modeCreate|CFile::modeWrite))
				{
					file.Write(pBuf, size);
					file.Close();
				}

				size_t pos = 0;
				char *data = (char*)pBuf;
				char *subStr = NULL;
				bool flag = false;
				std::string str;
				subStr = strstr(data, "\r\n");
				if(subStr != NULL)
				{
					pos = subStr - data;
				}
				if(pos > 0)
				{ 
					//找到\r\n后将Http头第一行赋给subStr,只查找Http头的第一行
					str += std::string(data, pos);
					subStr = (char*)str.c_str();
					if(strstr(subStr, "200") != NULL && strstr(subStr, "OK") != NULL)
					{
						flag = true;
						str = "";
						str += std::string(data, size);
						pos = str.find("\r\n\r\n", 0);
						pos += 4;
					}
					else
					{
						flag = false;
					}
				}
				
				data = NULL;
				subStr = NULL;
				
				if(flag)
				{
					int maxSize = main->m_pMMSDlg->GetMMSMaxSize();
					
					if((size - pos)/1024 > maxSize) //彩信大于设置的最大值则不接收
					{
						FindVideoDlg(TRUE);
						main->m_pWarningNoFlashDlg->SetTitle(L"彩信息过大！无法接收！");
						main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
						ret = -1;
					}
					else
					{
						Data::MMSData* pMMsData = MMS::MMSWarp::GetMMSWarp()->DecodeMessage(pBuf+pos, size);
						Dprintf("DecodeMessage Over\r\n");
						//pMMsData->Insert();
						main->m_pMMSDlg->SaveMmsData(pMMsData);
						ret = pMMsData->id();

						::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
						delete pMMsData;
						pMMsData = NULL;
					}
				}
				else
				{
					main->m_pWarningNoFlashDlg->SetTitle(L"彩信接收过程错误!");
					main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				}
			}
			delete []pBuf;
			pBuf = NULL;
		}
	}
    Transport.Close();
	delete pHttpParam;
	return ret;
}

HttpParam    gHttpParam;
//int type 
//   = 0	get
//   = 1    post
//int dwtype
//	= 0    mediafiledownload
//	= 1	   weather
//  = 2	   menumanager
//  = 10   MMS
void StartHttp(char *url, int type = 0, int dwtype = 0, char *lfilename = NULL, char *cIMEI = NULL, char *cTerminalId = NULL)
{

	HttpParam    *pHttpParam = new HttpParam;
	pHttpParam->type = type;
	pHttpParam->url = url;
	pHttpParam->lfilename = lfilename;
	pHttpParam->cIMEI = cIMEI;
	pHttpParam->cTerminalId = cTerminalId;
	pHttpParam->dwType = dwtype;

	DWORD httpThreadID = 0;
	HANDLE m_pThread1 = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)HttpProcesse, pHttpParam, 0, &httpThreadID );
}

void GeHttptFile(TCHAR   *url)   
{
	CString szuser = "";
    CString szpass = "";

    CString str_url= url;
    CString str_savepath=_T("\\temp\\Test.xml");

    DWORD dwServiceType;
    CString strServerName;
    CString strFilePath;    

    INTERNET_PORT nPort;
    CInternetSession comm;
    CHttpConnection* pServer   = NULL;
    CHttpFile*         pHttpFile = NULL;

    INTERNET_PROXY_INFO Proxy;

       AfxParseURL(str_url, dwServiceType, strServerName, strFilePath, nPort);

       Proxy.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
       CString strProxyServer;
       strProxyServer.Format(_T("%s:%d"), "10.0.0.172", 80);
       Proxy.lpszProxy = strProxyServer;
       Proxy.lpszProxyBypass = NULL;

      pServer = comm.GetHttpConnection(strServerName, nPort);

    comm.SetOption (INTERNET_OPTION_CONNECT_TIMEOUT, 30 * 1000) ;

    comm.SetOption (INTERNET_OPTION_CONNECT_BACKOFF, 1000) ;

    comm.SetOption (INTERNET_OPTION_CONNECT_RETRIES, 3) ;

    CFile wrifile;
    if (!wrifile.Open(str_savepath,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary)) 
    {        
      //  AfxMessageBox(L"文件创建失败！","提示！");    
        comm.Close();
        return;
    }

    try
    {
		pServer->SetOption(INTERNET_OPEN_TYPE_PROXY, (LPVOID)&Proxy, sizeof(INTERNET_PROXY_INFO));

		try
		{
			pHttpFile = (CHttpFile*)comm.OpenURL(str_url,1, INTERNET_FLAG_TRANSFER_BINARY);
				/*, INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD*/
		}
		catch(CInternetException* e)
		{
			if (e->m_dwError == 12002)
			{
				pHttpFile = NULL; 
				e-> Delete();
				Sleep(1000); 
			}
			else
			{
				pHttpFile = 0;
				throw;
			}
			
		}    
        
		pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME, (LPVOID)(LPCTSTR)/*szuser*/L"", /*lstrlen(szuser)+*/1);//此处没有添加
		pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)(LPCTSTR)/*szpass*/L"", /*lstrlen(szpass)+*/1);//用户名和密码验证
           
        CString      str ;
        pHttpFile->QueryInfo (HTTP_QUERY_STATUS_CODE, str) ;
        if (str == _T("407"))
        {
         //   AfxMessageBox(L"需要用户名和密码验证!!!");

            //pHttpFile->SetOption(INTERNET_OPTION_PROXY, (LPVOID)&Proxy, sizeof(INTERNET_PROXY_INFO));
            pServer->SetOption(INTERNET_OPEN_TYPE_PROXY, (LPVOID)&Proxy, sizeof(INTERNET_PROXY_INFO));
            pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME, (LPVOID)(LPCTSTR)szuser, lstrlen(szuser)+1);//此处没有添加
            pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)(LPCTSTR)szpass, lstrlen(szpass)+1);//用户名和密码验证
            pHttpFile->SendRequest(NULL);

            pHttpFile->QueryInfo (HTTP_QUERY_STATUS_CODE, str) ;
            if (str != _T("200"))
            {
             //   AfxMessageBox(L"用户名或密码验证失败！");
                pHttpFile->Close() ;
                delete pHttpFile ;
                pHttpFile = NULL;
                return ;
            }

            }
            else
            {
                pServer->SetOption(INTERNET_OPTION_PROXY, (LPVOID)&Proxy, sizeof(INTERNET_PROXY_INFO));
                 //pHttpFile->SetOption(INTERNET_OPTION_PROXY, (LPVOID)&Proxy, sizeof(INTERNET_PROXY_INFO));
            }            

        }
        catch(CInternetException * m_pException)
        {
            pHttpFile = NULL;

            m_pException->m_dwError;

            m_pException->Delete();

            comm.Close();

         //   AfxMessageBox(L"连接失败！","错误！");
            return;
        }

        DWORD uRead=0;
        BYTE buffer[2048];
        do 
        {
                    try
            {     
                memset((void*)buffer,0,2048);
                uRead=pHttpFile->Read(buffer,2048);        

            }
            catch (CInternetException* e)
            {
            //    AfxMessageBox(L"读取数据失败！","错误！");
                e->Delete();
                continue;
            }

            if(uRead<=0)    
            {
                break;
            }
            // 如果读到数据就写入文件
            wrifile.Write(buffer,uRead);

        } 
        while(true);

        wrifile.Flush();
        wrifile.Close();// 接收完毕关闭文件

        pHttpFile->Close();// 接收完毕关闭连接

        delete pHttpFile;
        pHttpFile = NULL;

        comm.Close();        
}

// add by qi 获得一个目录下的文件大小
ULONGLONG GetDirAllFileSize(CString path)
{	
	CString allPath = path + L"*" ;
	CFile File;
	ULONGLONG filesize = 0;
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(allPath,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes !=FILE_ATTRIBUTE_DIRECTORY)
			{ 
				//如果是文件
				CString strfl = path+fd.cFileName ;
				if (File.Open(strfl,CFile::modeRead,NULL))
				{
					filesize += File.GetLength();
					File.Close();
				}	
			}
	
		}
		while(FindNextFile(hfind,&fd));
	}
	return filesize ;
}

//获得 Flashdrv磁盘目录下的剩余空间 和 总空间
void GetDiskStatus(ULONGLONG &free,ULONGLONG &total,ULONGLONG &uleft)
{
	ULARGE_INTEGER freeBytes;
	ULARGE_INTEGER totalBytes;
	ULARGE_INTEGER totalFreeBytes;
	GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
	free  = totalFreeBytes.QuadPart;
	total = totalBytes.QuadPart;
	
	//可用空间
	uleft = 0;
	if (free > 50*1024*1024)
	{
		uleft = free - 50*1024*1024;
	}
} 