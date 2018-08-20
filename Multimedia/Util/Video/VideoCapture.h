#ifndef __UTIL_VIDEO_VIDEOCAPTURE_H__
#define __UTIL_VIDEO_VIDEOCAPTURE_H__

#pragma once

#include "../Capture.h"

namespace Util {
    namespace Video {
        class VideoCapture : public Capture {
        public:
            virtual ~VideoCapture();
            virtual void Start();
            virtual void Stop();
        };
    }
}

#endif //__UTIL_VIDEO_VIDEOCAPTURE_H__
