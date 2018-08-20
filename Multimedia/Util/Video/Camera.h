#ifndef __UTIL_VIDEO_CAMERA_H__
#define __UTIL_VIDEO_CAMERA_H__

#pragma once

#include "VideoCapture.h" //for VideoCapture
#include "VideoMetaData.h" //for VideoMetaData

namespace Util {
    namespace Video {
        class CameraParameters : public VideoMetaData {
        };
        class Camera : public VideoCapture {
        public:
            Camera(CameraParameters const& params);
            virtual ~Camera();
            virtual void Start();
            virtual void Stop();
        };
    }
}

#endif //__UTIL_VIDEO_CAMERA_H__
