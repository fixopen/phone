#ifndef __UTIL_VIDEO_DXRENDER_H__
#define __UTIL_VIDEO_DXRENDER_H__

#include <windef.h> //for HWND, RECT
#include <ddraw.h> //for LPDIRECTDRAW, LPDIRECTDRAWSURFACE
#include "VideoRender.h" //for VideoRender

#define SURFACE_TYPE_YV12        (0)
#define SURFACE_TYPE_RGB565      (4)

#define IMAGE_TYPE_YV12          (0)
#define IMAGE_TYPE_YUV420        (9)
#define IMAGE_TYPE_RGB565        (4)

namespace Util {
    namespace Video {
        struct RenderOverlay {
            LPDIRECTDRAWSURFACE directDrawSurface;
            int x, y;
            int srcWidth, srcHeight;
            int dstWidth, dstHeight;
            int surfaceType;
        };

        class Surface;
        class DxManager {
            friend class Surface;
        public:
            DxManager(HWND const windowHandle, DWORD const videoWidth, DWORD const videoHeight, int const surfaceType);
            ~DxManager();
            Surface* const CreateSurface(int const surfaceType, int const x, int const y, int const srcWidth, int const srcHeight, int const dstWidth, int const dstHeight);
            void DestroySurface(Surface* const surface);
        private:
            LPDIRECTDRAW directDraw_;
            LPDIRECTDRAWSURFACE directDrawPrimarySurface_; // Primary Surface.
            bool inited_;
            bool finaled_;
        };
        class Surface {
            friend class DxManager;
        public:
            Surface(DxManager* const owner, int const surfaceType, int const x, int const y, int const srcWidth, int const srcHeight, int const dstWidth, int const dstHeight);
            ~Surface();
            void Do(unsigned char const* const image, int const width, int const height, int const imageType);
        public:
            unsigned int left;
            unsigned int top;
            RECT bounds;
        private:
            DxManager* owner_;
            RenderOverlay* offlineScreen_;
            RenderOverlay* handle_;
        };
        class DxRender : public VideoRender {
        public:
            static bool const Init(HWND const windowHandle, DWORD const videoWidth, DWORD const videoHeight, int const surfaceType);
            static void Finally();
            void* const CreateSurface(int const surfaceType, int const x, int const y, int const srcWidth, int const srcHeight, int const dstWidth, int const dstHeight);
            void DeleteSurface(void* renderHandle);
            void Do(void* const renderHandle, unsigned char const* const image, int const width, int const height, int const imageType);
        private:
            static LPDIRECTDRAW s_DirectDraw_;
            static LPDIRECTDRAWSURFACE s_DirectDrawPrimarySurface_; // Primary Surface.
            static bool s_Inited_;
            static bool s_Finaled_;
            static void s_ReleaseAllObjects_();
        public:
            unsigned int left;
            unsigned int top;
            RECT bounds;
        private:
            RenderOverlay* offlineScreen_;
        };
    }
}

#endif //__UTIL_VIDEO_DXRENDER_H__
