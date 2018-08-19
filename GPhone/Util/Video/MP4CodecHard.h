#ifndef __UTIL_VIDEO_MP4CODECHARD_H__
#define __UTIL_VIDEO_MP4CODECHARD_H__

#include "H264_MP4_Decoder_encoder/MfcDrvParams.h"
#include "H264_MP4_Decoder_encoder/SsbSipMpeg4Encode.h"
#include "H264_MP4_Decoder_encoder/SsbSipMpeg4Decode.h"
#include "VideoCodec.h"

namespace Util {
    namespace Video {
        class MP4Encoder : public VideoEncoder {
        public:
            MP4Encoder();
            virtual void SetParameters(VideoParameters const & p);
            virtual bool Init();
            virtual bool Encode(unsigned char const * const yuv420Data, unsigned char* const outBuffer, size_t const outBufferSize, size_t& encodedSize);
            virtual void Uninit();
            bool const IsIFrame() const;
        private:
            VideoParameters parameters_;
            bool isParametersSet_;
            unsigned char* encoderBuffer_;
            unsigned char* encoderBufferIn_;
            unsigned char* encoderBufferOut_;
            bool isIFrame_;
            size_t frameNo_;
        };

        class MP4Decoder : public VideoDecoder {
        public:
            MP4Decoder();
            virtual bool Init();
            virtual bool Decode(unsigned char const * const mp4Data, size_t const mp4DataSize, unsigned char* const outYuv420Data/*, size_t const outYuv420DataSize*/, size_t& decodedLength);
            virtual void GetParameters(VideoParameters& p);
            virtual void Uninit();
        private:
            VideoParameters parameters_;
            bool isParametersProbe_;
            unsigned char* decoderBuffer_;
            unsigned char* decoderBufferIn_;
            unsigned char* decoderBufferOut_;
        };
    }
}

#endif //__UTIL_VIDEO_MP4CODECHARD_H__
