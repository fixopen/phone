#ifndef __UTIL_VIDEO_VIDEORENDER_H__
#define __UTIL_VIDEO_VIDEORENDER_H__

#pragma once

#include "../Render.h"

namespace Util {
    namespace Video {
        class VideoRender : public Render {
        public:
            virtual ~VideoRender();
            virtual void Show();
        };
    }
}

#endif //__UTIL_VIDEO_VIDEORENDER_H__
