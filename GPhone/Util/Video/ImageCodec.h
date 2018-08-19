#ifndef __UTIL_VIDEO_IMAGECODEC_H__
#define __UTIL_VIDEO_IMAGECODEC_H__

namespace Util {
    namespace Video {
        class ImageMetaInfo {
        public:
            virtual ~ImageMetaInfo() = 0 {}
        };
        class ImageCodec {
        public:
            virtual ~ImageCodec() = 0 {}
            virtual ImageMetaInfo* GetMetaInfo() = 0;
        };
        class ImageEncoder {
        };
        class ImageDecoder {
        };
    }
}

#endif // __UTIL_VIDEO_IMAGECODEC_H__
