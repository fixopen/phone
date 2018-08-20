#ifndef __UTIL_IMAGE_SCANNER_H__
#define __UTIL_IMAGE_SCANNER_H__

#pragma once

#include "ImageCapture.h" //for ImageCapture
#include "ImageMetaData.h" //for ImageMetaData

namespace Util {
    namespace Image {
        class ScannerParameters : public ImageMetaData {
        };
        class Scanner : public ImageCapture {
        public:
            Scanner(ScannerParameters const& params);
            virtual ~Scanner();
            virtual void Start();
            virtual void Stop();
        };
    }
}

#endif //__UTIL_IMAGE_SCANNER_H__
