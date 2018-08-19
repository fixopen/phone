#if !defined(AFX_CACHE_H__)
#define AFX_CACHE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <imaging.h>
#include <list>
#include "./cximageinc/ximage.h"

// CImgCache������õ�����ڴ�����Release�����ڲ���Ҫ���ص����ó��򣬿��ÿռ����һЩ��
#ifdef _DEBUG
  #define CACHE_RELEASE_START_SIZE		(10 * 1024 * 1024)
  #define MIN_CACHE_SIZE				(5 * 1024 * 1024)
#else
  #define CACHE_RELEASE_START_SIZE		(15 * 1024 * 1024)
  #define MIN_CACHE_SIZE				(10 * 1024 * 1024)
#endif

// ���һ��565��ɫ��ʽ��BITMAPINFO�ṹ������ָ�벻��Ҫ�ͷţ�ֻ��֤���´ε��ñ�����ǰ��Ч��
BITMAPINFO * Get565BitmapInfo(int width, int height);

// ͼ�λ����࣬���ڻ���������õ���ͼ�Ρ�
class CImgCache
{
public:

	// ��ʼ�����棬hwndΪһ�Ϸ����ھ�������ڴ�������DC��
	static void InitCache(HWND hwnd);

	// ��ȡװ����fileName��ָ����λͼ��HDC. 
	// pWidth��pHeight���ڻ�ȡλͼ�Ŀ�͸ߡ�
	// ���������������Զ�����������NULL��ʾʧ��.
	// ���ص�HDC������ɾ�����ͷţ������´ε���GetHDC()ǰ��֤��Ч��
	static HDC GetHDC(CString fileName, UINT * pWidth = NULL, UINT * pHeight = NULL);
	
	// ��ȡһ������Ļһ�����DC���ں�̨����, ����ģʽ�ѱ���ΪTRANSPARENT��
	static HDC GetBackBuffer() { return m_hdcBuffer; }	

	// ��ָ���ļ�ת��Ϊ565ģʽ��16λɫλͼ��(WinCE��׼��ʽ)
	static BOOL TransformTo565(CString fileName);
	static void Dump();
private:

	// ����һ��������
	struct CacheItem
	{
		CString FileName;
		HDC hdc;
		BYTE * pData;
		
		UINT CacheWidth;
		UINT CacheHeight;
		
		UINT PicWidth;
		UINT PicHeight;

		HBITMAP hOldBmp; //����hdcʱ�Դ���HBITMAP���ͷ�ʱ��Ҫ����ѡ��hdc
	};

	static int PerpareCache(UINT width, UINT height);

	static int BringTopPriority(int i);

	
	static void CreateCacheItem(HDC hdc, UINT width, UINT height, int cnt);

	static int LoadBmp(CString fileName);
	static int LoadCommonImage(CString fileName);

	// �ͷ�һ�����ȼ���͵Ļ����(Ŀǰʵ��Ϊ�ͷ����δ������)
	static void ReleaseOneItem();

	// ����Bmp�ļ���ͷ���ṹ
	static BOOL LoadBmpHeader(CFile & file, BITMAPFILEHEADER * bfhHeader, BITMAPINFO ** ppBitmapInfo); 
	
private:

	//IImagingFactory �ӿڣ�������jpg��һ��ͼ���ļ���
	//static IImagingFactory * m_pImgFactory;

	static HDC m_hdcBuffer;	//��̨����HDC
	static HBITMAP m_hOldBmpForBuffer;  // ��̨���崴��ʱ�Դ���HBITMAP������ͷ�ʱѡ����(Ŀǰʵ���У�ֱ�ӵȲ���ϵͳ����)
	static UINT m_TotalMemSize;		//�ܹ�ռ���ڴ�(��ʵ��ֵ��С)

	// �������ݡ�Խ�������ȼ�Խ�ߡ�
	static std::vector<CacheItem> m_ImgInfo;
};

// ��໺�����������
#define CACHE_MAX_TEXT_COUNT 30
// ׼����������������Χ���С
#define CACHE_MAX_TEXTBOX_SIZE (600 * 60)

// ��ʾһ�����ֻ�����ļ�ֵ
struct CTextCacheKey
{
	CString Text; // �ı�ֵ

	UINT BoxWidth;	// �������ֵ�Unit�Ŀ��
	UINT BoxHeight; // �������ֵ�Unit�ĸ߶�

	UINT FontSize;
	UINT FontHeight;
	UINT FontWeights;
	BOOL FontItalic;
	BOOL FontUnderLine;
	int FontLanguage;
	CString FontName;

	UINT FontAlign;

	COLORREF FontColor; //�ı���ɫ��ֻ�к�ɫ�ı�����
};

bool operator == (const CTextCacheKey & key1, const CTextCacheKey & key2);

class CTextCache
{
public:
	
	// ��ȡ������key��ָ�����ֵ�λͼ��HDC. 
	// �������������򷵻�NULL
	// ���ص�HDC������ɾ�����ͷţ������´ε���GetHDC()ǰ��֤��Ч��
	static HDC GetCachedHDC(CTextCacheKey & key);
	
	// ����һ���µĻ�������ض�Ӧ��HDC(λͼ�ѽ��ã���δ��������)
	// hdcRef���ڴ���memDC�����������в�����
	// ���������Բ����������򷵻�NULL��
	static HDC CreateEmptyHDC(CTextCacheKey & key, HDC hdcRef);

private:

	// �ͷ�һ�����ȼ���͵Ļ����(Ŀǰʵ��Ϊ�ͷ����δ������)
	static void ReleaseOneItem();

	// Ϊ�µĻ�����׼���ռ䣬��Ҫʱ�ͷ������
	static void PrepareCache();

private:

	// ������
	struct TextCacheItem
	{
		CTextCacheKey Key;
		HDC hdc;
		HBITMAP hOldBmp; //����hdcʱ�Դ���HBITMAP���ͷ�ʱ��Ҫ����ѡ��hdc
	};
	// �������ݡ�Խ�������ȼ�Խ�ߡ�
	static std::list<TextCacheItem> m_CacheList;
};


#endif // !defined(AFX_CACHE_H__)