#ifndef __UTIL_VIDEO_H264CODECHARD_H__
#define __UTIL_VIDEO_H264CODECHARD_H__

//#include "H264_MP4_Decoder_encoder/SsbSipLogMsg.h"
#include "H264_MP4_Decoder_encoder/MfcDrvParams.h"
#include "H264_MP4_Decoder_encoder/SsbSipH264Encode.h"
#include "H264_MP4_Decoder_encoder/SsbSipH264Decode.h"
#include "VideoCodec.h"

namespace Util {
    namespace Video {
        class H264Encoder : public VideoEncoder {
        public:
            H264Encoder();
            virtual void SetParameters(VideoParameters const & p);
            virtual bool Init();
            virtual bool Encode(unsigned char const * const yuv420Data, unsigned char* const outBuffer, size_t const outBufferSize, size_t& encodedSize);
            virtual void Uninit();
        private:
            VideoParameters parameters_;
            bool isParametersSet_;
            unsigned char* encoderBuffer_;
            unsigned char* encoderBufferIn_;
            unsigned char* encoderBufferOut_;
        };

        class H264Decoder : public VideoDecoder {
        public:
            H264Decoder();
            virtual bool Init();
            virtual bool Decode(unsigned char const * const h264Data, size_t const h264DataSize, unsigned char* const outYuv420Data/*, size_t const outYuv420DataSize*/, size_t& decodedLength);
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

#endif //__UTIL_VIDEO_H264CODECHARD_H__
