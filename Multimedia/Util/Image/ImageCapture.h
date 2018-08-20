#ifndef __UTIL_IMAGE_IMAGECAPTURE_H__
#define __UTIL_IMAGE_IMAGECAPTURE_H__

#pragma once

#include "../Capture.h"

namespace Util {
    namespace Image {
        class ImageCapture : public Capture {
        public:
            virtual ~ImageCapture();
        };
    }
}

#endif //__UTIL_IMAGE_IMAGECAPTURE_H__
