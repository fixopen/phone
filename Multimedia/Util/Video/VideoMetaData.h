#ifndef __UTIL_VIDEO_VIDEOMETADATA_H__
#define __UTIL_VIDEO_VIDEOMETADATA_H__

#pragma once

#include "../MetaData.h"

namespace Util {
    namespace Video {
        class VideoMetaData : public MetaData {
        public:
            virtual ~VideoMetaData();
        };
    }
}

#endif //__UTIL_VIDEO_VIDEOMETADATA_H__
