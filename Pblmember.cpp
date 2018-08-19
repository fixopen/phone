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

CString GetFileName(CString pathname) 
{
    int i;
	for(i=pathname.GetLength()-1; i>=0; i-- ) 
	{ 
		if(pathname[i]== '/') 
			break; 
	} 
	return pathname.Mid( i+1 ); 
}

#include <Afxinet.h>
/* *****************************************************************************

 * Description : BASE64 encoding and decoding

 * Date             : 06-08-21 21:00 

 * Author          :  aishen944

 * Copryright   :  

********************************************************************************/
// #include <string.h>
// #include <stdlib.h>
// 
// #define BASE64_PAD64 '='
// 
// char base64_alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
//                           'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
//                           'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a',
//                           'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
//                           'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
//                           't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1',
//                           '2', '3', '4', '5', '6', '7', '8', '9', '+',
//                           '/'};
// 
// char base64_suffix_map[256] = {
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255,  62, 255, 255, 255,  63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
//      255, 255, 255, 255, 255,  0,   1,    2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
//      15,   16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255, 255,  26,  27,  28,
//      29,   30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
//      49,   50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
// 
// static char cmove_bits(unsigned char src, unsigned lnum, unsigned rnum) {
//    src <<= lnum;
//    src >>= rnum;
//    return src;
// }
// 
// char* base64_encode(const char *data) {
//    char *ret, *retpos;
//    int n, m, padnum = 0, retsize, dlen = strlen(data);
// 
//    if(dlen == 0) return NULL;
// 
//    /* Account the result buffer size and alloc the memory for it. */
//    if((dlen % 3) != 0)
//     padnum = 3 - dlen % 3;
//    retsize = (dlen + padnum) + ((dlen + padnum) * 1/3) + 1;
//    if((ret = (char *)malloc(retsize)) == NULL) 
//       return NULL;
//    retpos = ret;
// 
//    /* Starting to convert the originality characters to BASE64 chracaters. 
//       Converting process keep to 4->6 principle. */
//    for(m = 0; m < (dlen + padnum); m += 3) {
//       /* When data is not suffice 24 bits then pad 0 and the empty place pad '='. */
//       *(retpos) = base64_alphabet[cmove_bits(*data, 0, 2)];
//       if(m == dlen + padnum - 3 && padnum != 0) {  /* Whether the last bits-group suffice 24 bits. */
//           if(padnum == 1) {   /* 16bit need pad one '='. */
//               *(retpos + 1) = base64_alphabet[cmove_bits(*data, 6, 2) + cmove_bits(*(data + 1), 0, 4)];
//               *(retpos + 2) = base64_alphabet[cmove_bits(*(data + 1), 4, 2)];
//               *(retpos + 3) = BASE64_PAD64;
//           } else if(padnum == 2) { /* 8bit need pad two'='. */
//               *(retpos + 1) = base64_alphabet[cmove_bits(*data, 6, 2)];
//               *(retpos + 2) = BASE64_PAD64;
//               *(retpos + 3) = BASE64_PAD64;
//           }
//       } else {  /* 24bit normal. */
//          *(retpos + 1) = base64_alphabet[cmove_bits(*data, 6, 2) + cmove_bits(*(data + 1), 0, 4)];
//          *(retpos + 2) = base64_alphabet[cmove_bits(*(data + 1), 4, 2) + cmove_bits(*(data + 2), 0, 6)];
//          *(retpos + 3) = base64_alphabet[*(data + 2) & 0x3f];
//       }
// 
//       retpos += 4;
//       data += 3;
//    }
// 
//    ret[retsize - 1] =0;
// 
//    return ret;
// }
// 
// char* base64_decode(const char *bdata) {
//    char *ret = NULL, *retpos;
//    int n, m, padnum = 0, retsize, bdlen = strlen(bdata);
// 
//    if(bdlen == 0) return NULL;
//    if(bdlen % 4 != 0) return NULL;
// 
//    /* Whether the data have invalid base-64 characters? */
//    for(m = 0; m < bdlen; ++m) {
//       if(bdata[m] != BASE64_PAD64 && base64_suffix_map[bdata[m]] == 255)
//          goto LEND;
//    }
// 
//    /* Account the output size. */
//    if(bdata[bdlen - 1] ==  '=')  padnum = 1;
//    if(bdata[bdlen - 1] == '=' && bdata[bdlen - 2] ==  '=') padnum = 2;
//    retsize = (bdlen - 4) - (bdlen - 4) / 4 + (3 - padnum) + 1;
//    ret = (char *)malloc(retsize);
//    if(ret == NULL) 
//          return NULL;
//    retpos = ret; 
// 
//    /* Begging to decode. */
//    for(m = 0; m < bdlen; m += 4) {
//       *retpos = cmove_bits(base64_suffix_map[*bdata], 2, 0) + cmove_bits(base64_suffix_map[*(bdata + 1)], 0, 4);
//       if(m == bdlen - 4 && padnum != 0) {  /* Only deal with last four bits. */
//          if(padnum == 1)   /* Have one pad characters, only two availability characters. */
//             *(retpos + 1) = cmove_bits(base64_suffix_map[*(bdata + 1)], 4, 0) + cmove_bits(base64_suffix_map[*(bdata + 2)], 0, 2);
//          /*
//          Have two pad characters, only two availability characters.
//          if(padnum == 2) { 
//          }
//          */
//          retpos += 3 - padnum;
//       } else {
//          *(retpos + 1) = cmove_bits(base64_suffix_map[*(bdata + 1)], 4, 0) + cmove_bits(base64_suffix_map[*(bdata + 2)], 0, 2);
//          *(retpos + 2) = cmove_bits(base64_suffix_map[*(bdata + 2)], 6, 0) + base64_suffix_map[*(bdata + 3)];
//          retpos += 3;
//       }
//       bdata += 4;
//    }
// 
//    ret[retsize - 1] = 0;
// 
//    LEND: return ret;
// }

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
	
	//Dprintf(L"解析完成\r\n");
	Dprintf("解析完成\r\n");
	return 0;
}

//release dialed   20090619
int HttpProcesse(void *pParam)
{
	Dprintf("HttpProcesse:\r\n");
	int ret = -1;
	HttpParam *pHttpParam = (HttpParam *)pParam;
	CString strSentence;
    CString strFileName = pHttpParam->url;
	
	Dprintf(pHttpParam->url);
	Dprintf("\r\n");

	CString strFormData = "";    // 需要提交的数据
    CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头

	Util::HTTPTransport Transport;
	Dprintf("HttpProcesse:0\r\n");
	Transport.SetAPN((Util::DIAL_APN)pHttpParam->apn_type, pHttpParam->apn_proxy);
	Dprintf("HttpProcesse:1\r\n");
    char *regist[] = 
	{
	"<?xml version=\"1.0\" encoding=\"GBK\" ?>\r\n\
		<JTXX>\r\n\
		<Header>\r\n\
		<OperCode>REGISTER</OperCode>\r\n\
		<Version>1.0.0</Version>\r\n\
		<TimeStamp>",
		"</TimeStamp>\r\n\
		</Header>\r\n\
		<Body>\r\n\
		<TerminalVersion>1.0.0</TerminalVersion>\r\n\
		<IMEI>",
		"</IMEI>\r\n\
		<TerminalId>",
		"</TerminalId>\r\n\
		</Body>\r\n\
		</JTXX>\r\n"
	};
    if(pHttpParam->type == 1)    //post
	{
		
		Dprintf("HttpProcesse:2\r\n");
		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
		char datetime[32];
		sprintf(datetime, "%04d%02d%02d%02d%02d%02d", curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
		Dprintf("HttpProcesse:3\r\n");
		
		char sData[1024] = {0};
		memset(sData, 0, 1024);

		strcpy(sData, regist[0]);
		strcat(sData, datetime);
		strcat(sData, regist[1]);
		if(pHttpParam->cIMEI)
		strcat(sData, pHttpParam->cIMEI);
		strcat(sData, regist[2]);
		if(pHttpParam->cTerminalId)
		strcat(sData, pHttpParam->cTerminalId);
		strcat(sData, regist[3]);
		Dprintf("HttpProcesse:4\r\n");

		Transport.message.headers.insert(std::make_pair(std::wstring(L"Content-Type"), std::wstring(L"application/x-www-form-urlencoded")));
		
		TCHAR sLen[32];
		wsprintf(sLen, L"%d", strlen(sData));
		std::wstring sLen_ = sLen;
		Transport.message.headers.insert(std::make_pair(std::wstring(L"CONTENT-LENGTH"), sLen_));

		Transport.message.body = sData;
		Dprintf("HttpProcesse5: %d\r\n", strlen(sData));
		Transport.SetPost(sData, strlen(sData));
		Dprintf("HttpProcesse:6\r\n");
		DWORD r_ = Transport.Post_((LPCTSTR)strFileName);
		Dprintf("Post1 error %d\r\n", r_);
		if(r_ == 0 )
		{
			int size = 0;
			BYTE pBuf[1024*2+1] = {0};
			int nCount = 1;
			while(nCount != 0)
			{
				nCount = Transport.Recv(pBuf+size, 256);
				size += nCount;
			} 
			Dprintf((char *)pBuf);
			ret = dowith_Parse((char *)pBuf, 4);
		}
	}
	
	else if(pHttpParam->type == 2 || pHttpParam->type == 11)    //发送彩信
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
		extern VOID WriteLog_(char *ptr, int size);
		WriteLog_(pHttpParam->pData, pHttpParam->dataLentg);
		Dprintf("Post start:\r\n");
		DWORD r_ = Transport.Post((LPCTSTR)strFileName);
		Dprintf("Post mms error %d\r\n", r_);
		if(r_ == 0 )
		{
			int size = 0;
			BYTE pBuf[1024*2+1] = {0};
			int nCount = 1;
			while(nCount != 0)
			{
				nCount = Transport.Recv(pBuf+size, 256);
				size += nCount;
			} 
			Dprintf((char *)pBuf);

			//extern VOID WriteLog_(char *ptr, int size);
			//WriteLog_((char *)pBuf, size);
			if(pHttpParam->type == 2)
				ret = MMS::MMSWarp::GetMMSWarp()->DecodeSubmitResponse(pBuf, size);
			else
				ret = 0;
		}
				
	}

	else	//get
	{
		Dprintf("Get start:\r\n");
// 	
// 		Transport.message.headers.insert(std::make_pair(std::wstring(L"Accept-Language"), std::wstring(L"utf-8")));
// 		Transport.message.headers.insert(std::make_pair(std::wstring(L"Accept"), std::wstring(L"text/plain,*/*")));
// 		Transport.message.headers.insert(std::make_pair(std::wstring(L"User-Agent"), std::wstring(L"CeHttp")));
// 		Transport.message.headers.insert(std::make_pair(std::wstring(L"Connection"), std::wstring(L"Keep-Alive")));
				
		DWORD r_ = Transport.Get_((LPCTSTR)strFileName);
		Dprintf("Get error %d\r\n", r_);
		if(r_ == 0 )
		{
			BYTE  *pBuf = new BYTE[1024*1024*2];
			memset(pBuf, 0, 1024*1024*2);
			int size = 0;
			int nCount = 1;
		
			while(nCount != 0)
			{
				nCount = Transport.Recv(pBuf+size, 128);
				size += nCount;
				if(size > (1024*1204*2-256))
					break;
			} 
			
	//		Dprintf((char *)pBuf);
			if(pHttpParam->dwType < 10)
				ret = dowith_Parse((char *)pBuf, pHttpParam->dwType);
			else if(pHttpParam->dwType == 10)			//彩信
			{
				extern VOID WriteLog_(char *ptr, int size);
			//	WriteLog_((char *)pBuf, size);

				Data::MMSData* pMMsData = MMS::MMSWarp::GetMMSWarp()->DecodeMessage(pBuf, size);
				pMMsData->Insert();
				ret = pMMsData->id();
				::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
				delete pMMsData;
			}
			delete []pBuf;
		}
	}
    Transport.Close();
	delete pHttpParam;
	return ret;
}

/*
int HttpProcesse(void *pParam)
{
	int ret = -1;
	HttpParam *pHttpParam = (HttpParam *)pParam;
	CString strSentence;
    CString strFileName = pHttpParam->url;

	CString strFormData = "";    // 需要提交的数据
    CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头
	
	INTERNET_PROXY_INFO Proxy;
	Proxy.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
	CString strProxyServer;
	strProxyServer.Format(_T("%s:%d"), "10.0.0.172", 80);
	Proxy.lpszProxy = strProxyServer;
	Proxy.lpszProxyBypass = NULL;

    CInternetSession sess;
    CHttpFile* fileGet;
    try
    {
        fileGet=(CHttpFile*)sess.OpenURL(strFileName, 0, INTERNET_FLAG_TRANSFER_BINARY);
	}
    catch(CInternetException* e)
    {
		if (e->m_dwError == 12002)
		{
			fileGet = NULL; 
			e-> Delete();
			Sleep(1000); 
		}
		else
		{
			fileGet = 0;
			throw;
		}
    }    
	
    if(fileGet)
    {
        DWORD dwStatus;
        DWORD dwBuffLen = sizeof(dwStatus);
        BOOL bSuccess = fileGet->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
		
		char *regist[] = 
		{
		"<?xml version=\"1.0\" encoding=\"GBK\" ?>\r\n\
			<JTXX>\r\n\
			<Header>\r\n\
			<OperCode>REGISTER</OperCode>\r\n\
			<Version>1.0.0</Version>\r\n\
			<TimeStamp>",
			"</TimeStamp>\r\n\
			</Header>\r\n\
			<Body>\r\n\
			<TerminalVersion>1.0.0</TerminalVersion>\r\n\
			<IMEI>",
			"</IMEI>\r\n\
			<TerminalId>",
			"</TerminalId>\r\n\
			</Body>\r\n\
			</JTXX>\r\n"
		};
        if( bSuccess && dwStatus>= 200 && dwStatus<300 ) 
        { 
			if(pHttpParam->type == 1)    //post
			{
				SYSTEMTIME curtime;
				GetLocalTime(&curtime);
				char datetime[32];
				sprintf(datetime, "%04d%02d%02d%02d%02d%02d", curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
				char sData[1024] = {0};
				strcpy(sData, regist[0]);
				strcat(sData, datetime);
				strcat(sData, regist[1]);
				if(pHttpParam->cIMEI)
					strcat(sData, pHttpParam->cIMEI);
				strcat(sData, regist[2]);
				if(pHttpParam->cTerminalId)
					strcat(sData, pHttpParam->cTerminalId);
				strcat(sData, regist[3]);

				CString strSentence, strGetSentence = "";
				DWORD dwStatus;
				DWORD dwBuffLen = sizeof(dwStatus);
				BOOL bSuccess = fileGet->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
				if( bSuccess && dwStatus>= 200 &&dwStatus<300 )
				{ 
					BOOL result = fileGet->SendRequest(strHeaders, (LPVOID)(LPCTSTR)sData, strlen(sData));
					char pBuf[1024*2+1] = {0};
					//从缓冲区循环读取文数据
					int bytesread = fileGet->Read((BYTE *)pBuf,1024*2);
					if(bytesread > 0)
					{
						ret = dowith_Parse(pBuf, 4);
					}
					
					Dprintf(pBuf); // 显示返回网页内容
					Dprintf("\r\n");
				}
				else 
				{
					strSentence.Format(L"POST出错，错误码：%d", dwStatus);
					Dprintf(strSentence);
				}
			}
			else	//get
			{
				CStdioFile fileWrite; 
				
				
			//	if(fileWrite.Open(sLocaleFileName, CFile::modeWrite|CFile::modeCreate))
				{ 
					BYTE  *pBuf = new BYTE[1024*1024];
					memset(pBuf, 0, 1024*1024);
					//从缓冲区循环读取文数据
					int ncount = 0;
					while (int bytesread = fileGet->Read(pBuf+ncount,256))
					{
						//指针移到文件结尾
				//		fileWrite.SeekToEnd();
						//将读取的缓冲区数据写入本地文件
				//		fileWrite.Write(pBuf+ncount, bytesread);
						ncount += bytesread;
						if(ncount >= (1024*1024-256))
							break;
					}
				//	fileWrite.Close();
					Dprintf(L"下载完毕\r\n");
					if(pHttpParam->dwType < 10)
						ret = dowith_Parse((char *)pBuf, pHttpParam->dwType);
					else if(pHttpParam->dwType == 10)
					{
						extern VOID WriteLog(char *ptr);
						WriteLog((char *)pBuf);
					}

					delete []pBuf;
				}
// 				else
// 				{
// 					Dprintf("本地文件"+sLocaleFileName+"打开出错."); 
// 				}
			}
        }
        else 
        {
            strSentence.Format(L"打开网页文件出错，错误码：%d", dwStatus);
            Dprintf(strSentence);
        }
        fileGet->Close();
        delete fileGet;
    }
    else
        Dprintf(L"不能找到网页文件！");
	
	sess.Close();
	delete pHttpParam;
	return ret;
}
*/

/*
int HttpProcesse(void *pParam)
{
	int ret = -1;
	HttpParam *pHttpParam = (HttpParam *)pParam;
	CString strSentence;
    CString strFileName = pHttpParam->url;

	CString strFormData = "";    // 需要提交的数据
    CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头
	
	INTERNET_PROXY_INFO Proxy;
	Proxy.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
	CString strProxyServer;
	strProxyServer.Format(_T("%s:%d"), "10.0.0.172", 80);
	Proxy.lpszProxy = strProxyServer;
	Proxy.lpszProxyBypass = NULL;

    CInternetSession sess;
	sess.SetOption (INTERNET_OPTION_CONNECT_TIMEOUT, 30 * 1000) ;
	sess.SetOption (INTERNET_OPTION_CONNECT_BACKOFF, 1000) ;
    sess.SetOption (INTERNET_OPTION_CONNECT_RETRIES, 3) ;

	DWORD dwSerType;
	CString szServer;
	CString szObject;
	unsigned short nPort;
	AfxParseURL(strFileName, dwSerType, szServer, szObject, nPort); 
	
	CHttpConnection *pConnec = sess.GetHttpConnection(szServer,nPort,NULL,NULL); 


    CHttpFile* fileGet;
	
    if(pConnec)
    {
        DWORD dwStatus;
        DWORD dwBuffLen = sizeof(dwStatus);
		if(pHttpParam->type == 1)    //post
			fileGet = pConnec->OpenRequest(CHttpConnection::HTTP_VERB_POST, L""); 
		else
			fileGet = pConnec->OpenRequest(CHttpConnection::HTTP_VERB_GET, szObject); 
		if(!fileGet)
			return ret;
		
        BOOL bSuccess = fileGet->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
		
		char *regist[] = 
		{
		"<?xml version=\"1.0\" encoding=\"GBK\" ?>\r\n\
			<JTXX>\r\n\
			<Header>\r\n\
			<OperCode>REGISTER</OperCode>\r\n\
			<Version>1.0.0</Version>\r\n\
			<TimeStamp>",
			"</TimeStamp>\r\n\
			</Header>\r\n\
			<Body>\r\n\
			<TerminalVersion>1.0.0</TerminalVersion>\r\n\
			<IMEI>",
			"</IMEI>\r\n\
			<TerminalId>",
			"</TerminalId>\r\n\
			</Body>\r\n\
			</JTXX>\r\n"
		};
        if( bSuccess ) 
        { 
			if(pHttpParam->type == 1)    //post
			{
				SYSTEMTIME curtime;
				GetLocalTime(&curtime);
				char datetime[32];
				sprintf(datetime, "%04d%02d%02d%02d%02d%02d", curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
				char sData[1024] = {0};
				strcpy(sData, regist[0]);
				strcat(sData, datetime);
				strcat(sData, regist[1]);
				if(pHttpParam->cIMEI)
					strcat(sData, pHttpParam->cIMEI);
				strcat(sData, regist[2]);
				if(pHttpParam->cTerminalId)
					strcat(sData, pHttpParam->cTerminalId);
				strcat(sData, regist[3]);

				CString strSentence, strGetSentence = "";
				DWORD dwStatus;
				DWORD dwBuffLen = sizeof(dwStatus);
				BOOL bSuccess = fileGet->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
				if( bSuccess && dwStatus>= 200 &&dwStatus<300 )
				{ 
					BOOL result = fileGet->SendRequest(strHeaders, (LPVOID)(LPCTSTR)sData, strlen(sData));
					char pBuf[1024*2+1] = {0};
					//从缓冲区循环读取文数据
					int bytesread = fileGet->Read((BYTE *)pBuf,1024*2);
					if(bytesread > 0)
					{
						ret = dowith_Parse(pBuf, 4);
					}
					
					Dprintf(pBuf); // 显示返回网页内容
					Dprintf("\r\n");
				}
				else 
				{
					strSentence.Format(L"POST出错，错误码：%d", dwStatus);
					Dprintf(strSentence);
				}
			}
			else	//get
			{
				CStdioFile fileWrite; 
				int t_ = fileGet->SendRequest(); 
				
				

			//	if(fileWrite.Open(sLocaleFileName, CFile::modeWrite|CFile::modeCreate))
				{ 
					BYTE  *pBuf = new BYTE[1024*1024];
					memset(pBuf, 0, 1024*1024);
					//从缓冲区循环读取文数据
					int ncount = 0;
					while (int bytesread = fileGet->Read(pBuf+ncount,256))
					{
						//指针移到文件结尾
				//		fileWrite.SeekToEnd();
						//将读取的缓冲区数据写入本地文件
				//		fileWrite.Write(pBuf+ncount, bytesread);
						ncount += bytesread;
						if(ncount >= (1024*1024-256))
							break;
					}
				//	fileWrite.Close();
					Dprintf(L"下载完毕\r\n");
					if(pHttpParam->dwType < 10)
						ret = dowith_Parse((char *)pBuf, pHttpParam->dwType);
					else if(pHttpParam->dwType == 10)
					{
						extern VOID WriteLog(char *ptr);
						WriteLog((char *)pBuf);
					}

					delete []pBuf;
				}
// 				else
// 				{
// 					Dprintf("本地文件"+sLocaleFileName+"打开出错."); 
// 				}
			}
        }
        else 
        {
            strSentence.Format(L"打开网页文件出错，错误码：%d", dwStatus);
            Dprintf(strSentence);
        }
        fileGet->Close();
        delete fileGet;
    }
    else
        Dprintf(L"不能找到网页文件！");
	
	sess.Close();
	delete pHttpParam;
	return ret;
}
*/

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
	/*
	CString   szContent;   
	TCHAR   strProxyList[MAX_PATH],   strUsername[64],   strPassword[64];   
	//in   this   case   "proxya"   is   the   proxy   server   name,   "8080"   is   its   port   
	wcscpy(strProxyList,   L"10.0.0.172:80");   
	wcscpy(strUsername,   L"");   
	wcscpy(strPassword,   L"");   
	DWORD   dwServiceType   =   AFX_INET_SERVICE_HTTP;   
	CString   szServer,   szObject;   
	INTERNET_PORT   nPort;   
	AfxParseURL(url,   dwServiceType,   szServer,   szObject,   nPort);   
	CInternetSession   mysession;   
	CHttpConnection*   pConnection;   
	CHttpFile*   pHttpFile;   
	pConnection   =   mysession.GetHttpConnection(szServer,   
		INTERNET_FLAG_KEEP_CONNECTION,   
		INTERNET_INVALID_PORT_NUMBER,   
		NULL,   NULL);   
	pHttpFile   =   pConnection->OpenRequest(L"GET",   szObject,   
		NULL,   0,   NULL,   NULL,   
		INTERNET_FLAG_KEEP_CONNECTION);   
	//here   for   proxy   
	INTERNET_PROXY_INFO   proxyinfo;   
	proxyinfo.dwAccessType   =   INTERNET_OPEN_TYPE_PROXY;   
	proxyinfo.lpszProxy   =   strProxyList;   
	proxyinfo.lpszProxyBypass   =   NULL;   
	mysession.SetOption(INTERNET_OPTION_PROXY,   (LPVOID)&proxyinfo,   sizeof(INTERNET_PROXY_INFO));   
	pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,   strUsername,   wcslen(strUsername)+1);   
	pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,   strPassword,   wcslen(strPassword)+1);   
    
	pHttpFile->SendRequest(NULL);   
	DWORD   nFileSize   =   pHttpFile->GetLength();   
	TCHAR *rbuf   =   szContent.GetBuffer(nFileSize);   
	UINT   uBytesRead   =   pHttpFile->Read(rbuf,   nFileSize);   
	szContent.ReleaseBuffer();   
	pHttpFile->Close();   
	delete   pHttpFile;   
	pConnection->Close();   
	delete   pConnection;   
	mysession.Close();   
	return   szContent;   
	*/

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

/*
BOOL httpdownload(char *url, int type, char *lfilename)
{
	BOOL ret = FALSE;
	CInternetSession *m_pInetSession=new CInternetSession(NULL, 1, INTERNET_OPEN_TYPE_PRECONFIG,	NULL, NULL, 0);
	//设置参数
	
	m_pInetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000);
	m_pInetSession->SetOption(INTERNET_OPTION_CONNECT_BACKOFF,1000);
	m_pInetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES,1);
	
	CString sURL = url;
	//打开URL文件，返回CStdioFile
	CStdioFile *pSFile = m_pInetSession->OpenURL(sURL);
	if(pSFile)
	{
		//缓冲区
		BYTE pBuf[1024] = {NULL};
		CFile pCFile = NULL;
		
		//创建本地文件
		char *local_dir[] = {"/Flashdrv/my_adv/", "/Flashdrv/my_web/", "/Flashdrv/my_monitor/"};
		
		CString sFileName = GetFileName(sURL);
		CString sLocaleFileName = local_dir[type];
		sLocaleFileName += sFileName;
		
		int i = wcstombs( lfilename, sLocaleFileName.GetBuffer(128), 128);
		lfilename[i] = '\0';
		
		pCFile.Open(sLocaleFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		if(pCFile != -1)
		{
			//从缓冲区循环读取文数据
			while (int bytesread = pSFile->Read(pBuf,1024))
			{
				//指针移到文件结尾
				pCFile.SeekToEnd();
				//将读取的缓冲区数据写入本地文件
				pCFile.Write(pBuf,bytesread);
			}
			ret = TRUE;
		}
		
		//关闭本地文件
		pCFile.Close();
		//关闭CStdioFile
		pSFile->Close();
	}
	delete m_pInetSession;
	return ret;
}
*/