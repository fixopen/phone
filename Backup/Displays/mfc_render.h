#ifndef __SAMSUNG_SYSLSI_APDEV_SAMPLE_MFC_RENDER_H__
#define __SAMSUNG_SYSLSI_APDEV_SAMPLE_MFC_RENDER_H__



#define MFC_RENDER_SURFACE_TYPE_YV12		(0)
#define MFC_RENDER_SURFACE_TYPE_RGB565		(4)


#define MFC_RENDER_IMAGE_TYPE_YV12			(0)
#define MFC_RENDER_IMAGE_TYPE_YUV420		(9)
#define MFC_RENDER_IMAGE_TYPE_RGB565		(4)


#include <windows.h>	// Because of HWND
#include <ddraw.h>

typedef struct tagMFC_RENDER_OVERLAY
{
	LPDIRECTDRAWSURFACE   pDD;
	int   x, y;
	int   src_wd, src_hi;
	int   dst_wd, dst_hi;
	int   surface_type;
} _MFC_RENDER_OVERLAY;

class Dx_Render
{
public:
	static BOOL mfc_render_init(HWND hWnd,DWORD CodeWidth,DWORD CodeHeight,int surface_type);
	static void mfc_render_final();

	void *mfc_render_create_surface(int surface_type,
		int x, int y,
		int src_wd, int src_hi,
		int dst_wd, int dst_hi);
	void mfc_render_do(void *mfc_render_handle, unsigned char *pImg, int width, int height, int img_type);
	void mfc_render_delete_surface(void *mfc_render_handle);
private:
	static LPDIRECTDRAW          g_pDD;
	static LPDIRECTDRAWSURFACE   g_pDDSPrimary;		// Primary Surface.
	//static LPDIRECTDRAWSURFACE   g_pDDSBack; // DirectDraw back surface

	static bool inited;
	static bool deinited;
	static void ReleaseAllObjects(void);
public:
	unsigned int WindowLeft;
	unsigned int WindowTop;
public:
	RECT	win_rc;
private:
	_MFC_RENDER_OVERLAY *pOffScreen;
};



#endif /* __SAMSUNG_SYSLSI_APDEV_SAMPLE_MFC_RENDER_H__ */
