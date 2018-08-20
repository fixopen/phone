//////////////////////////////////////////////////
//
// csplayer.cxx
//
//
//
//
//////////////////////////////////////////////////
#include <windows.h>
#include "csplayer.h"

#define _T TEXT

// 宏定义
#define SAFE_CLOSE_HANDLE(x)		if(x){CloseHandle(x);x=NULL;}
#define MSG_BASE					WM_APP + 60
#define MSG_HEARTBEAT				MSG_BASE
#define MSG_ASSOCIATE_HANDLE		MSG_BASE + 1
#define MSG_OPEN					MSG_BASE + 2
#define MSG_EXIT					MSG_BASE + 4
#define MSG_PLAY					MSG_BASE + 5
#define MSG_STOP					MSG_BASE + 6
#define MSG_FORWARD					MSG_BASE + 7
#define MSG_SET_WND_POS				MSG_BASE + 8
#define MSG_SET_ROTATE				MSG_BASE + 9
#define MSG_SET_POS					MSG_BASE + 10
#define MSG_SET_ZOOM				MSG_BASE + 11
#define MSG_SET_FULLSCREEN			MSG_BASE + 12
#define MSG_SET_REPEAT				MSG_BASE + 13
#define MSG_SET_SHUFFLE				MSG_BASE + 14
#define MSG_SET_AUTO_PREROTATE		MSG_BASE + 15
#define MSG_SET_VID_OUTPUT			MSG_BASE + 16
#define MSG_SET_AUD_OUTPUT			MSG_BASE + 17
#define MSG_SET_MUTE				MSG_BASE + 18
#define MSG_SET_VOLUME				MSG_BASE + 19
#define MSG_SET_ASPECT				MSG_BASE + 20
#define MSG_SET_STEREO				MSG_BASE + 21
#define MSG_CAP_BMP					MSG_BASE + 22
#define MSG_SUBTITLE_TEXT			MSG_BASE + 23
#define MSG_ERROR					MSG_BASE + 24
#define MSG_GET_OUT_RECT			MSG_BASE + 25
#define MSG_OUTPUT_RECT				MSG_BASE + 26
#define MSG_MEDIA_INFO				MSG_BASE + 27

// 全局变量
LPCTSTR g_strMutexName = _T("csplayer mutex object");
LPCTSTR g_strAppName = _T("D:\\csplayer\\csplayer.exe");
LPCTSTR g_strWndName = _T("csplayer window");
LPCTSTR g_strClsName = _T("csplayer_win");
const TCHAR szRunKey[] = _T("SOFTWARE\\Cyansoft\\CSplayer");
const TCHAR szRun[] = _T("playfile");
const TCHAR szSubtitle[] = _T("subtitle");
const TCHAR szSubtitleColor[] = _T("subcolor");
const TCHAR szFileName[] = _T("filename");
const TCHAR szTick[] = _T("tick");
const TCHAR szDuration[] = _T("duration");
const TCHAR szSize[] = _T("size");
const TCHAR szVidName[] = _T("video");
const TCHAR szVidWidth[] = _T("vidwidth");
const TCHAR szVidHeight[] = _T("vidheight");
const TCHAR szVidFR[] = _T("framerate");
const TCHAR szVidBR[] = _T("vidbitrate");
const TCHAR szAudName[] = _T("audio");
const TCHAR szSR[] = _T("samplerate");
const TCHAR szChs[] = _T("channels");
const TCHAR szAudBR[] = _T("audbyterate");

static HANDLE g_hMutex = NULL;
static HWND g_hWnd = NULL;

// 函数实现
int plyCreate(int x, int y, int width, int height)
{
	BOOL	fResult = FALSE;

	if (g_hMutex) return -1;

	// 创建全局互斥体, 以确保进程唯一
	g_hMutex = CreateMutex(NULL, TRUE, g_strMutexName);
	if (g_hMutex)
	{
		if (ERROR_ALREADY_EXISTS == GetLastError())
		{
			CloseHandle(g_hMutex);
			g_hMutex = NULL;
			return -1;
		}
	}

	// 创建播放器进程
	static TCHAR szCmdLine[32] = {0};
	
	//wsprintf(szCmdLine, _T("%d %d %d %d"), x, y, width, height);
	//if (CreateProcess(g_strAppName, szCmdLine, NULL, NULL, FALSE, NULL, NULL, NULL, NULL, NULL) == FALSE)
	//	goto finish;
	WinExec(("D:\\csplayer\\csplayer.exe 0 0 0 0"), SW_SHOW);
	fResult = TRUE;

finish:
	if (fResult == FALSE)
	{
		if (g_hMutex)
		{
			ReleaseMutex(g_hMutex);
			CloseHandle(g_hMutex);
			g_hMutex = NULL;
		}

		return -1;
	}
	else
		return 0;
}

int plyInit(HWND hWnd)
{
	// 查找播放器窗口句柄
	g_hWnd = FindWindow(g_strClsName, g_strWndName);
	if (g_hWnd == NULL)
	{
		//RETAILMSG(1, (_T("could not to find window : %d\r\n"), GetLastError()));
		return -1;
	}

	// 关联心跳窗口句柄
	if (0 == SendMessage(g_hWnd, MSG_ASSOCIATE_HANDLE, (WPARAM) hWnd, NULL))
	{
		//RETAILMSG(1, (_T("associate handle ok\r\n")));
		return 0;
	}

	return -1;
}

BOOL AddRegistry(const TCHAR *pFile)
{
	BOOL bResult = FALSE;
	HKEY hKey = NULL;	
	DWORD dwDisposition = 0;

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szRunKey, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(hKey, szRun, 0, REG_SZ, (LPBYTE)pFile, wcslen(pFile) * 2) == ERROR_SUCCESS) bResult = TRUE;

		RegCloseKey(hKey);
	}

	return bResult;
}

int plyOpen(LPTSTR pszFile)
{
	if (g_hWnd == NULL || pszFile == NULL) 
		return -1;

	if (AddRegistry(pszFile))
	{
		if (0 == SendMessage(g_hWnd, MSG_OPEN, NULL, NULL))
		{
			return 0;
		}
	}

	return -1;
}

int plyExit(void)
{
	if (g_hMutex)
	{
		ReleaseMutex(g_hMutex);
		CloseHandle(g_hMutex);
		g_hMutex = NULL;
	}

	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_EXIT, NULL, NULL))
			return 0;
	}

	return -1;	
}

int plyPlay(BOOL fStart)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_PLAY, (WPARAM)fStart, NULL))
			return 0;
	}

	return -1;
}

int plyStop(void)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_STOP, NULL, NULL))
			return 0;
	}

	return -1;
}

int plySetFullScreen(BOOL isFullScreen)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_FULLSCREEN, (WPARAM) isFullScreen, NULL))
			return 0;
	}

	return -1;
}

int plySetWndPos(WORD x, WORD y, WORD width, WORD height)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_WND_POS, MAKEWPARAM(x, y), MAKELPARAM(width, height)))
			return 0;
	}

	return -1;
}

int plySetPos(int percent)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_POS, percent, NULL))
			return 0;
	}

	return -1;
}

int plyForward(void)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_FORWARD, NULL, NULL))
			return 0;
	}

	return -1;	
}

int plySetRotate(int rotate)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_ROTATE, (WPARAM) rotate, NULL))
			return 0;
	}

	return -1;	
}

int plySetZoom(int zoom)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_ZOOM, (WPARAM) zoom, NULL))
			return 0;
	}

	return -1;
}

int getMediaInfo(const LPMEDIA_INFO info)
{
	HKEY hKey = NULL;
	TCHAR path[MAX_PATH] = {0};
	BOOL bRtn = FALSE;
	DWORD dwType = 0, dwSize = sizeof (DWORD);
	
	if (NULL == info) return -1;	
		
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szRunKey, 0, 0, &hKey) == ERROR_SUCCESS)
	{
		dwSize = sizeof(path);
		memset(path, 0, dwSize);
		if (RegQueryValueEx(hKey, szFileName, NULL, &dwType, (LPBYTE)path, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ && dwSize > 0)
			{
				memcpy(info->szFileName, path, dwSize);				
				bRtn = TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szTick, NULL, &dwType, (LPBYTE)&info->nTick, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD)
			{
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szDuration, NULL, &dwType, (LPBYTE)&info->nDuration, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szSize, NULL, &dwType, (LPBYTE)&info->nFileSize, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szVidWidth, NULL, &dwType, (LPBYTE)&info->nVidWidth, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szVidHeight, NULL, &dwType, (LPBYTE)&info->nVidHeight, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szVidFR, NULL, &dwType, (LPBYTE)&info->nVidFrameRate, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szVidBR, NULL, &dwType, (LPBYTE)&info->nVidBitRate, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szSR, NULL, &dwType, (LPBYTE)&info->nAudSampleRate, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szChs, NULL, &dwType, (LPBYTE)&info->nAudChannels, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}
		if (RegQueryValueEx(hKey, szAudBR, NULL, &dwType, (LPBYTE)&info->nAudByteRate, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwSize > 0)
			{						
				bRtn &= TRUE;
			}
		}		
		dwSize = sizeof(path);
		memset(path, 0, dwSize);
		if (RegQueryValueEx(hKey, szVidName, NULL, &dwType, (LPBYTE)path, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ && dwSize > 0)
			{
				memcpy(info->szVidStreamName, path, dwSize);				
				bRtn &= TRUE;
			}
		}
		dwSize = sizeof(path);
		memset(path, 0, dwSize);
		if (RegQueryValueEx(hKey, szAudName, NULL, &dwType, (LPBYTE)path, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ && dwSize > 0)
			{
				memcpy(info->szAudStreamName, path, dwSize);				
				bRtn &= TRUE;
			}
		}
		RegCloseKey(hKey);
	}
	
	if (bRtn)
		return 0;

	return -1;
}

int plyGetMediaInfo(LPMEDIA_INFO info)
{
	if (NULL == info) return -1;

	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_MEDIA_INFO, NULL, NULL))
			return getMediaInfo(info);
	}
	
	return 0;
}

int plySetRepeat(BOOL repeat)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_REPEAT, (WPARAM) repeat, NULL))
			return 0;
	}

	return -1;
}

int plySetShuffle(BOOL shuffle)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_SHUFFLE, (WPARAM) shuffle, NULL))
			return 0;
	}

	return -1;
}

int plySetAutoPreRotate(BOOL rotate)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_AUTO_PREROTATE, (WPARAM) rotate, NULL))
			return 0;
	}

	return -1;
}

int plySetVideoOutput(BOOL vid)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_VID_OUTPUT, (WPARAM) vid, NULL))
			return 0;
	}

	return -1;
}

int plySetAudioOutput(BOOL aud)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_AUD_OUTPUT, (WPARAM) aud, NULL))
			return 0;
	}

	return -1;
}

int plySetMute(BOOL mute)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_MUTE, (WPARAM) mute, NULL))
			return 0;
	}

	return -1;
}

int plySetVolume(int vol)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_VOLUME, (WPARAM) vol, NULL))
			return 0;
	}

	return -1;
}

int plySetAspect(int aspect)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_ASPECT, (WPARAM) aspect, NULL))
			return 0;
	}

	return -1;
}

int plySetStereo(int stereo)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_SET_STEREO, (WPARAM) stereo, NULL))
			return 0;
	}

	return -1;
}

int plyCaptureBmp(LPTSTR bmpFile)
{
	if (g_hWnd)
	{
		if (0 == SendMessage(g_hWnd, MSG_CAP_BMP, NULL, NULL))
			return 0;
	}

	return -1;
}

BOOL AddSubtitle(const TCHAR *sub, const DWORD color)
{
	BOOL bResult = FALSE;
	HKEY hKey = NULL;	
	DWORD dwDisposition = 0;

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szRunKey, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(hKey, szSubtitle, 0, REG_SZ, (LPBYTE)sub, wcslen(sub) * 2) == ERROR_SUCCESS)
			bResult = TRUE;

		if (RegSetValueEx(hKey, szSubtitleColor, 0, REG_DWORD, (LPBYTE)&color, sizeof (DWORD)) == ERROR_SUCCESS)
		{
			bResult &= TRUE;
		}

		RegCloseKey(hKey);
	}

	return bResult;
}

int plySubtitleText(int x, int y, LPCTSTR strText, DWORD color)
{
	if (AddSubtitle(strText, color))
	{
		if (g_hWnd)
		{
			if (0 == SendMessage(g_hWnd, MSG_SUBTITLE_TEXT, x, y))
				return 0;
		}
	}
	
	return -1;
}

int plyGetRect()
{
	if (g_hWnd)
		return SendMessage(g_hWnd, MSG_GET_OUT_RECT, NULL, NULL);

	return -1;
}


// End of file
//////////////////////////////////////////////////////////////
