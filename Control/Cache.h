#if !defined(AFX_CACHE_H__)
#define AFX_CACHE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <imaging.h>
#include <list>
#include "./cximageinc/ximage.h"

// CImgCache缓存可用的最大内存量，Release版由于不需要加载调试用程序，可用空间更大一些。
#ifdef _DEBUG
  #define CACHE_RELEASE_START_SIZE		(10 * 1024 * 1024)
  #define MIN_CACHE_SIZE				(5 * 1024 * 1024)
#else
  #define CACHE_RELEASE_START_SIZE		(15 * 1024 * 1024)
  #define MIN_CACHE_SIZE				(10 * 1024 * 1024)
#endif

// 获得一个565颜色格式的BITMAPINFO结构，返回指针不需要释放，只保证在下次调用本函数前有效。
BITMAPINFO * Get565BitmapInfo(int width, int height);

// 图形缓存类，用于缓存界面中用到的图形。
class CImgCache
{
public:

	// 初始化缓存，hwnd为一合法窗口句柄，用于创建兼容DC。
	static void InitCache(HWND hwnd);

	// 获取装载了fileName所指定的位图的HDC. 
	// pWidth与pHeight用于获取位图的宽和高。
	// 不存在已有项则自动创建，返回NULL表示失败.
	// 返回的HDC不可以删除或释放，仅在下次调用GetHDC()前保证有效。
	static HDC GetHDC(CString fileName, UINT * pWidth = NULL, UINT * pHeight = NULL);
	
	// 获取一个与屏幕一样大的DC用于后台缓冲, 背景模式已被设为TRANSPARENT。
	static HDC GetBackBuffer() { return m_hdcBuffer; }	

	// 将指定文件转换为565模式的16位色位图。(WinCE标准格式)
	static BOOL TransformTo565(CString fileName);
	static void Dump();
private:

	// 储存一个缓存项
	struct CacheItem
	{
		CString FileName;
		HDC hdc;
		BYTE * pData;
		
		UINT CacheWidth;
		UINT CacheHeight;
		
		UINT PicWidth;
		UINT PicHeight;

		HBITMAP hOldBmp; //创建hdc时自带的HBITMAP，释放时需要将其选回hdc
	};

	static int PerpareCache(UINT width, UINT height);

	static int BringTopPriority(int i);

	
	static void CreateCacheItem(HDC hdc, UINT width, UINT height, int cnt);

	static int LoadBmp(CString fileName);
	static int LoadCommonImage(CString fileName);

	// 释放一个优先级最低的缓存项。(目前实现为释放最久未引用项)
	static void ReleaseOneItem();

	// 加载Bmp文件的头部结构
	static BOOL LoadBmpHeader(CFile & file, BITMAPFILEHEADER * bfhHeader, BITMAPINFO ** ppBitmapInfo); 
	
private:

	//IImagingFactory 接口，供加载jpg等一般图形文件用
	//static IImagingFactory * m_pImgFactory;

	static HDC m_hdcBuffer;	//后台缓冲HDC
	static HBITMAP m_hOldBmpForBuffer;  // 后台缓冲创建时自带的HBITMAP，最后释放时选回用(目前实现中，直接等操作系统回收)
	static UINT m_TotalMemSize;		//总共占用内存(比实际值略小)

	// 缓存数据。越往后优先级越高。
	static std::vector<CacheItem> m_ImgInfo;
};

// 最多缓存的文字条数
#define CACHE_MAX_TEXT_COUNT 30
// 准许缓冲的文字项的最大包围框大小
#define CACHE_MAX_TEXTBOX_SIZE (600 * 60)

// 表示一个文字缓存项的键值
struct CTextCacheKey
{
	CString Text; // 文本值

	UINT BoxWidth;	// 包含文字的Unit的宽度
	UINT BoxHeight; // 包含文字的Unit的高度

	UINT FontSize;
	UINT FontHeight;
	UINT FontWeights;
	BOOL FontItalic;
	BOOL FontUnderLine;
	int FontLanguage;
	CString FontName;

	UINT FontAlign;

	COLORREF FontColor; //文本颜色，只有黑色的被缓存
};

bool operator == (const CTextCacheKey & key1, const CTextCacheKey & key2);

class CTextCache
{
public:
	
	// 获取绘制了key所指定文字的位图的HDC. 
	// 不存在已有项则返回NULL
	// 返回的HDC不可以删除或释放，仅在下次调用GetHDC()前保证有效。
	static HDC GetCachedHDC(CTextCacheKey & key);
	
	// 创建一个新的缓冲项，返回对应的HDC(位图已建好，但未绘制文字)
	// hdcRef用于创建memDC，不会对其进行操作。
	// 如果缓冲策略不允许缓冲该项，则返回NULL。
	static HDC CreateEmptyHDC(CTextCacheKey & key, HDC hdcRef);

private:

	// 释放一个优先级最低的缓存项。(目前实现为释放最久未引用项)
	static void ReleaseOneItem();

	// 为新的缓冲项准备空间，必要时释放已有项。
	static void PrepareCache();

private:

	// 缓存项
	struct TextCacheItem
	{
		CTextCacheKey Key;
		HDC hdc;
		HBITMAP hOldBmp; //创建hdc时自带的HBITMAP，释放时需要将其选回hdc
	};
	// 缓存数据。越往后优先级越高。
	static std::list<TextCacheItem> m_CacheList;
};


#endif // !defined(AFX_CACHE_H__)