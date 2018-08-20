// MJPGStatic.cpp : implementation file
//

#include "stdafx.h"
#include <INITGUID.h>
#include "MJPGStatic.h"

#include "Cache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HDC		CImgCache::m_hdcBuffer; //公共后台缓冲
UINT	CImgCache::m_TotalMemSize;//总共占用内存（粗略估计）
HBITMAP CImgCache::m_hOldBmpForBuffer; //创建

std::vector<CImgCache::CacheItem>	CImgCache::m_ImgInfo;

//IImagingFactory * CImgCache::m_pImgFactory;
extern IImagingFactory   *pImgFactory;

BITMAPINFO * Get565BitmapInfo(int width, int height)
{
	static BITMAPINFO * pBmpInfo565 = NULL;
	if (pBmpInfo565 == NULL)
	{
		pBmpInfo565 = (BITMAPINFO*)(new BYTE[sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 2]);
		memset(pBmpInfo565, 0, sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 2);
		
		pBmpInfo565->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		
		pBmpInfo565->bmiHeader.biPlanes = 1;
		pBmpInfo565->bmiHeader.biBitCount = 16;
		pBmpInfo565->bmiHeader.biCompression = BI_BITFIELDS;
		pBmpInfo565->bmiHeader.biSizeImage = 0;
		
		pBmpInfo565->bmiColors[0].rgbBlue		=	0;   
		pBmpInfo565->bmiColors[0].rgbGreen		=	0xF8;   
		pBmpInfo565->bmiColors[0].rgbRed		=	0;   
		pBmpInfo565->bmiColors[0].rgbReserved	=	0;   
		pBmpInfo565->bmiColors[1].rgbBlue		=	0xE0;   
		pBmpInfo565->bmiColors[1].rgbGreen		=	0x07;
		pBmpInfo565->bmiColors[1].rgbRed		=	0;
		pBmpInfo565->bmiColors[1].rgbReserved	=	0;
		pBmpInfo565->bmiColors[2].rgbBlue		=	0x1F;
		pBmpInfo565->bmiColors[2].rgbGreen		=	0;
		pBmpInfo565->bmiColors[2].rgbRed		=	0;
		pBmpInfo565->bmiColors[2].rgbReserved	=	0;
	}
	
	pBmpInfo565->bmiHeader.biWidth = width;
    pBmpInfo565->bmiHeader.biHeight = height;
	
	return pBmpInfo565;
}

void CImgCache::InitCache(HWND hwnd)
{
	HDC hdc = ::GetDC(hwnd);
	m_hdcBuffer = ::CreateCompatibleDC(hdc);
	HBITMAP hBmp = ::CreateCompatibleBitmap(hdc, 800, 480);
	ASSERT (hBmp);
	m_hOldBmpForBuffer = (HBITMAP)::SelectObject(m_hdcBuffer, hBmp);   
	::SetBkMode(m_hdcBuffer, TRANSPARENT);

	CreateCacheItem(hdc, 60, 60, 10);
	CreateCacheItem(hdc, 250, 40, 10);
	CreateCacheItem(hdc, 300, 400, 5);
	CreateCacheItem(hdc, 800, 90, 2);
	CreateCacheItem(hdc, 800, 480, 10);
	
	::ReleaseDC(hwnd, hdc);

//	CoInitializeEx(NULL,   COINIT_MULTITHREADED);
	if (FAILED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory)))
	{
		TRACE(L"Create ImagingFactory failed.\r\n");
	}
}

void CImgCache::CreateCacheItem(HDC hdc, UINT width, UINT height, int cnt)
{
	for (int i = 0; i < cnt; i ++)
	{
		CacheItem ci;
		ci.CacheWidth = width;
		ci.CacheHeight = height;
		ci.PicHeight = 0;
		ci.PicWidth = 0;

		ci.hdc = ::CreateCompatibleDC(m_hdcBuffer);
		
		BITMAPINFO * pBitmapInfo = Get565BitmapInfo(width, height);
		HBITMAP hBmp = ::CreateDIBSection(hdc, pBitmapInfo, DIB_RGB_COLORS, (void**)&ci.pData, NULL, 0);
		
		ci.hOldBmp = (HBITMAP)::SelectObject(ci.hdc, hBmp);
		m_ImgInfo.push_back(ci);
	}
}

int CImgCache::PerpareCache(UINT width, UINT height)
{
	for (int i = 0; i < m_ImgInfo.size(); i ++)
	{
		CacheItem & ci = m_ImgInfo.at(i);
		if (ci.CacheWidth >= width && ci.CacheHeight >= height)
		{
			return BringTopPriority(i);
		}
	}
	ASSERT (FALSE);
	return -1;
}
void CImgCache::Dump()
{
	for (int i = 0; i < m_ImgInfo.size(); i ++)
	{
		CacheItem & ci = m_ImgInfo.at(i);
		TRACE (L"Cache %d : %d * %d, %s\r\n", i, ci.PicWidth, ci.PicHeight, (LPCTSTR)ci.FileName);
	}
}
int CImgCache::BringTopPriority(int i)
{
	CacheItem ciOrg = m_ImgInfo.at(i);

	for (int j = i + 1; j < m_ImgInfo.size(); j ++)
	{
		CacheItem & ci = m_ImgInfo.at(j);
		if (ci.CacheWidth != ciOrg.CacheWidth || ci.CacheHeight != ciOrg.CacheHeight)
		{
			break;
		}
	}
	for (; i < j - 1; i ++)
	{
		m_ImgInfo[i] = m_ImgInfo[i + 1];
	}
	m_ImgInfo[i] = ciOrg;
	return i;
}

HDC CImgCache::GetHDC(CString fileName, UINT * pWidth, UINT * pHeight)
{
	ASSERT (!fileName.IsEmpty());
		
	BOOL bUseLastBuffer = FALSE;

	int index;
	for (index = 0; index < m_ImgInfo.size(); index ++)
	{
		if (m_ImgInfo.at(index).FileName == fileName)
			break;
	}
	if (index != m_ImgInfo.size())
	{
		index = BringTopPriority(index);
	}else
	{
		if(fileName.Find(L".bmp") == -1 && fileName.Find(L".BMP") == -1)
		{
 			index = LoadCommonImage(fileName);
 			ASSERT (index >= 0);
 			if (index == -1)
				return NULL;
		}else
		{
			index = LoadBmp(fileName);
			ASSERT (index >= 0);
			if (index == -1)
				return NULL;
		}
	}

	if (pWidth)
		*pWidth = m_ImgInfo.at(index).PicWidth;
	if (pHeight)
		*pHeight = m_ImgInfo.at(index).PicHeight;
	
	return m_ImgInfo.at(index).hdc;
}

int CImgCache::LoadBmp(CString fileName)
{
	CString strCacheFileName = fileName; //用于缓冲的文件名

	fileName = CMJPGStatic::ToFileDir(fileName);

	CFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		// 如果之前执行向565格式转换时出错，有可能导致原始文件已删除，但新文件
		// 尚未重命名的情况发生。故文件打不开的话先尝试恢复。
		CFile::Rename(fileName + "_565", fileName);
		if (!file.Open(fileName, CFile::modeRead))
		{
			TRACE (L"Load Bmp %s failed.\r\n", (LPCTSTR)fileName);
			return FALSE;
		}
	}

	// 读取文件头部信息
	BITMAPFILEHEADER   bfhHeader;
	LPBITMAPINFO lpBitmapInfo;

	if (!LoadBmpHeader(file, &bfhHeader, &lpBitmapInfo))
		return FALSE;
	
	if (lpBitmapInfo->bmiHeader.biBitCount != 16 || lpBitmapInfo->bmiHeader.biCompression != BI_BITFIELDS)
	{		
		// 不是565格式的,先关闭,转换为565格式,再重新打开

		TRACE (L"File %s is not 565 format. Convertion starts.\r\n", (LPCTSTR)fileName);

		delete [] (BYTE*)lpBitmapInfo;
		file.Close();

		if (!TransformTo565(fileName))
		{
			TRACE (L"Convertion failed.\r\n");
			return FALSE;;
		}
		if (!file.Open(fileName, CFile::modeRead))
			return FALSE;
		
		if (!LoadBmpHeader(file, &bfhHeader, &lpBitmapInfo))
			return FALSE;
	}

	int width = lpBitmapInfo->bmiHeader.biWidth;
	int height = lpBitmapInfo->bmiHeader.biHeight;

	int index = PerpareCache(width, height);

	
	CacheItem & ci = m_ImgInfo.at(index);
	ci.PicWidth = width;
	ci.PicHeight = height;
	ci.FileName = strCacheFileName;

	// 计算文件中一行所占的字节数
	int sizeLine = (((width * lpBitmapInfo->bmiHeader.biBitCount)+31)>>5)<<2;

	// 计算缓存中一行所占的字节数
	int sizeCacheLine = (((ci.CacheWidth * lpBitmapInfo->bmiHeader.biBitCount)+31)>>5)<<2;

	//HGDIOBJ hBmp = ::SelectObject(ci.hdc, ci.hOldBmp);

	//读取数据
	memset(ci.pData, 0xff, (ci.CacheHeight - height) * sizeCacheLine);

	BYTE * pData = ci.pData + (ci.CacheHeight - height) * sizeCacheLine;
	
	if (sizeLine == sizeCacheLine)
	{	
		file.Read(pData, sizeLine * height);
	}
	else
	{
		int sizeDataLine = width * 2;
		for (int i = 0; i < height; i ++)
		{
			file.Read(pData, sizeLine);
			memset(pData + sizeDataLine, 0xff, sizeCacheLine - sizeDataLine);
			pData += sizeCacheLine;
		}
	}
	file.Close();
	
	//ci.hOldBmp = (HBITMAP)::SelectObject(ci.hdc, hBmp);

	delete [](BYTE*)lpBitmapInfo;

	return index;
}

BOOL CImgCache::LoadBmpHeader(CFile & file, BITMAPFILEHEADER * pHeader, BITMAPINFO ** ppBitmapInfo)
{
	*ppBitmapInfo = NULL;
	
	//首先读取BITMAPFILEHEADER头
	file.Read(pHeader, sizeof(BITMAPFILEHEADER));   
	
	if(pHeader->bfType != ((WORD)('M'<<8)|'B') || pHeader->bfSize!=file.GetLength())
		return FALSE;
	
	//计算接下来的LPBITMAPINFO结构的尺寸
	UINT uBmpInfoLen = (UINT)pHeader->bfOffBits - sizeof(BITMAPFILEHEADER);
	
	//读取接下来的BITMAPINFO结构
	*ppBitmapInfo = (LPBITMAPINFO) new BYTE[uBmpInfoLen];  
	file.Read((LPVOID)(*ppBitmapInfo), uBmpInfoLen);
	
	if((*ppBitmapInfo)->bmiHeader.biSize != sizeof(BITMAPINFOHEADER))     
	{   
		delete [] (BYTE*)(*ppBitmapInfo);
		*ppBitmapInfo = NULL;
		return FALSE;
	}
	return TRUE;
}

int CImgCache::LoadCommonImage(CString fileName)
{
	CString strCacheFileName = fileName; //用于缓冲的文件名
	
	fileName = CMJPGStatic::ToFileDir(fileName);

	IImage   *pImage   =   NULL; 
	if (FAILED(pImgFactory->CreateImageFromFile(fileName, &pImage))) 
		return -1;

	ImageInfo imageInfo; 
	pImage->GetImageInfo(&imageInfo);

	RECT rect;
	rect.left = 0;
	rect.right = imageInfo.Width;
	rect.top = 0;
	rect.bottom = imageInfo.Height;

	int index = PerpareCache(imageInfo.Width, imageInfo.Height);

	ASSERT(index >= 0);
	
	CacheItem & ci = m_ImgInfo.at(index);
	ci.PicWidth = imageInfo.Width;
	ci.PicHeight = imageInfo.Height;
	ci.FileName = strCacheFileName;

	pImage->Draw(ci.hdc, &rect, NULL);
	pImage->Release(); 
	
	return index;
}

BOOL CImgCache::TransformTo565(CString fileName)
{
	// 首先，读取源位图文件
	CFile file;
	if (!file.Open(fileName, CFile::modeRead))
		return FALSE;
	
	BITMAPFILEHEADER   bfhHeader;
	LPBITMAPINFO lpBitmapInfo;
	
	if (!LoadBmpHeader(file, &bfhHeader, &lpBitmapInfo))
		return FALSE;

	int Width = lpBitmapInfo->bmiHeader.biWidth;
	int Height = lpBitmapInfo->bmiHeader.biHeight;
	LPVOID   lpBits = NULL;
	HBITMAP hBmpOrigin = ::CreateDIBSection(m_hdcBuffer, lpBitmapInfo, DIB_RGB_COLORS, &lpBits, NULL, 0);
	if(lpBits == NULL)
	{
		TRACE (L"No memory to create bmp.\n");
		delete [] (BYTE*)lpBitmapInfo;
		return FALSE;
	}

	//计算一行所占的字节数
	int sizeLine = (((Width * lpBitmapInfo->bmiHeader.biBitCount)+31)>>5)<<2;
	//总的字节数
	int TotalSize = sizeLine * Height;

	file.Read(lpBits, TotalSize);
	file.Close();
	
	// 下面开始创建565位图
	BITMAPINFO * pBmpInfo565 = Get565BitmapInfo(Width, Height); // 565格式的BITMAPINFO
	HBITMAP hDIB565 = ::CreateDIBSection(m_hdcBuffer, pBmpInfo565, DIB_RGB_COLORS, &lpBits, NULL, 0);
	if (lpBits == NULL)
	{
		TRACE (L"No memory to create bmp.\n");
		delete [] (BYTE*)lpBitmapInfo;
		::DeleteObject(hBmpOrigin);
		return FALSE;
	}

	HDC hdcBuffer = GetBackBuffer();	//用于存放565格式的bmp，使用后须将原来的位图还原

	HDC hdcOrigin = ::CreateCompatibleDC(m_hdcBuffer); //用于存放原bmp图
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hdcOrigin, hBmpOrigin);
	HBITMAP hOldBmpBuffer = (HBITMAP)::SelectObject(hdcBuffer, hDIB565);

	::BitBlt(hdcBuffer, 0, 0, Width, Height, hdcOrigin, 0, 0, SRCCOPY);

	hBmpOrigin = (HBITMAP)::SelectObject(hdcOrigin, hOldBmp);
	hDIB565 = (HBITMAP)::SelectObject(hdcBuffer, hOldBmpBuffer);  //恢复hdcBuffer中的位图

	//原始位图及相应的DC已经不需要，可以释放，hdcBuffer不需要释放，hDIB565在文件写入后释放
	::DeleteObject(hBmpOrigin);
	::DeleteDC(hdcOrigin);
	
	
	//计算一行所占的字节数
	sizeLine = (((Width * pBmpInfo565->bmiHeader.biBitCount)+31)>>5)<<2;
	//总的字节数
	TotalSize = sizeLine * Height;

	//重设BITMAPFILEHEADER的字段
	bfhHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFO) +  sizeof(RGBQUAD) * 2;
	bfhHeader.bfSize = bfhHeader.bfOffBits + TotalSize;
	
	//BITMAPINFO 直接沿用创建位图时的值即可。

	//开始写文件
	CFile fileWrite;
	fileWrite.Open(fileName + L"_565", CFile::modeWrite | CFile::modeCreate);
	fileWrite.Write(&bfhHeader, sizeof(BITMAPFILEHEADER));
	fileWrite.Write(pBmpInfo565, sizeof(BITMAPINFO) +  sizeof(RGBQUAD) * 2);

	fileWrite.Write(lpBits, TotalSize);
	fileWrite.Close();

	//释放剩余资源
	DeleteObject(hDIB565);

	CFile::Remove(fileName);
	CFile::Rename(fileName + L"_565", fileName);
	return TRUE;
}

inline bool operator == (const CTextCacheKey & key1, const CTextCacheKey & key2)
{
	return key1.Text			== key2.Text
	
		&& key1.BoxWidth		== key2.BoxWidth
		&& key1.BoxHeight		== key2.BoxHeight

		&& key1.FontSize		== key2.FontSize
		&& key1.FontHeight		== key2.FontHeight
		&& key1.FontWeights		== key2.FontWeights
		&& key1.FontItalic		== key2.FontItalic
		&& key1.FontUnderLine	== key2.FontUnderLine
		&& key1.FontLanguage	== key2.FontLanguage
		&& key1.FontName		== key2.FontName
		&& key1.FontColor		== key2.FontColor
		&& key1.FontAlign		== key2.FontAlign;
}

std::list<CTextCache::TextCacheItem> CTextCache::m_CacheList;

HDC CTextCache::GetCachedHDC(CTextCacheKey & key)
{
	std::list<TextCacheItem>::iterator it;
	for (it = m_CacheList.begin(); it != m_CacheList.end(); it ++)
		if (it->Key == key)
			break;
	if (it != m_CacheList.end())
	{	//找到匹配项
		TextCacheItem tci = * it;
		m_CacheList.erase(it);
		m_CacheList.push_back(tci);

		return m_CacheList.back().hdc;
	}
	
	//没有找到匹配项
	return NULL;	
}

void CTextCache::ReleaseOneItem()
{
	TextCacheItem & tci = m_CacheList.front();
	
	HGDIOBJ hbmp = ::SelectObject(tci.hdc, tci.hOldBmp);
	::DeleteObject(hbmp);
	::DeleteDC(tci.hdc);

	m_CacheList.erase(m_CacheList.begin());
}

void CTextCache::PrepareCache()
{
	if (m_CacheList.size() >= CACHE_MAX_TEXT_COUNT)
		ReleaseOneItem();
}

HDC CTextCache::CreateEmptyHDC(CTextCacheKey & key, HDC hdcRef)
{
	// 禁止缓冲过大的及非黑色的文字项
	if (key.BoxWidth * key.BoxHeight > CACHE_MAX_TEXTBOX_SIZE || key.FontColor != RGB(0,0,0))
		return NULL;

	PrepareCache();
	
	TextCacheItem tci;
	tci.Key = key;
	tci.hdc = ::CreateCompatibleDC(hdcRef);
	BITMAPINFO * pBmpInfo = Get565BitmapInfo(key.BoxWidth, key.BoxHeight);
	HBITMAP hbmp = ::CreateDIBSection(hdcRef, pBmpInfo, DIB_RGB_COLORS, NULL, NULL, 0);
	tci.hOldBmp = (HBITMAP)::SelectObject(tci.hdc, hbmp);
	
	::BitBlt(tci.hdc, 0, 0, key.BoxWidth, key.BoxHeight, NULL, 0, 0, WHITENESS);

	m_CacheList.push_back(tci);

	return tci.hdc;
}