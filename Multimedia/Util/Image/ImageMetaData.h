#ifndef __UTIL_IMAGE_IMAGEMETADATA_H__
#define __UTIL_IMAGE_IMAGEMETADATA_H__

#pragma once

#include "../MetaData.h"

namespace Util {
    namespace Image {
        class ImageMetaData : public MetaData {
        public:
            virtual ~ImageMetaData();
        };
    }
}

#endif //__UTIL_IMAGE_IMAGEMETADATA_H__
