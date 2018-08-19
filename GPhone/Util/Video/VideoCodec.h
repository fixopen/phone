#ifndef __UTIL_VIDEO_VIDEOCODEC_H__
#define __UTIL_VIDEO_VIDEOCODEC_H__

namespace Util {
    namespace Video {
        enum ImageFormat {
            RGB565,
            RGB888,
            YUV420P,
            YVU420P,
            YUV420,
            YVU420,
            YUV422P,
            YVU422P,
            YUV422,
            YVU422,
            YUV444P,
            YVU444P,
            YUV444,
            YVU444,
        };

        struct VideoParameters {
            VideoParameters()
            : width(0)
            , height(0)
            , framerate(0)
            , bitrate_kbps(0)
            , quantIntra(0)
            , quantInter(0)
            , gopNum(0) {
            }
            unsigned int width;
            unsigned int height;
            unsigned int framerate;
            unsigned int bitrate_kbps;
            unsigned int quantIntra;
            unsigned int quantInter;
            unsigned int gopNum;
        };

        class YUVConvert {
        public:
            static void YVU420PtoYUV420P(unsigned char const * const yuv420PData, size_t const width, size_t const height, unsigned char* const yvu420PData);
        };

        class VideoEncoder {
        public:
            virtual ~VideoEncoder() {
            };
            virtual void SetParameters(VideoParameters const & p) = 0;
            virtual bool Init() = 0;
            virtual bool Encode(unsigned char const * const yuv420Data, unsigned char* const outBuffer, size_t const outBufferSize, size_t& encodedSize) = 0;
            virtual void Uninit() = 0;
        };

        class VideoDecoder {
        public:
            virtual ~VideoDecoder() {
            };
            virtual bool Init() = 0;
            virtual bool Decode(unsigned char const * const data, size_t const dataSize, unsigned char* const outYuv420Data, size_t& decodedLength) = 0;
            virtual void GetParameters(VideoParameters& p) = 0;
            virtual void Uninit() = 0;
        };
    }
}

#endif //__UTIL_VIDEO_VIDEOCODEC_H__
