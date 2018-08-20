#include <stdlib.h> //for memcpy
#include "H264CodecHard.h"

namespace Util {
    namespace Video {
        H264Encoder::H264Encoder()
        : VideoEncoder()
        , parameters_(VideoParameters())
        , isParametersSet_(false)
        , encoderBuffer_(0)
        , encoderBufferIn_(0)
        , encoderBufferOut_(0) {
        }

        void H264Encoder::SetParameters(VideoParameters const & p) {
            parameters_ = p;
            //parameters_.width = p.width;
            //parameters_.height = p.height;
            if ((parameters_.width == 176) && (parameters_.height == 144)) {
                //parameters_.bitrate_kbps = p.bitrate_kbps;
                //parameters_.framerate = p.framerate;
                //parameters_.gopNum = p.gopNum;
                //parameters_.quantInter = p.quantInter;
                //parameters_.quantIntra = p.quantIntra;
                isParametersSet_ = true;
            }
        }

        bool H264Encoder::Init() {
            bool result = false;
            if (isParametersSet_) {
                // Initialize table for RGB to YUV conversion
                // InitLookupTable();
                // Initialize the compressor
                encoderBuffer_ = (unsigned char*)SsbSipH264EncodeInit(parameters_.width, parameters_.height, parameters_.framerate, parameters_.bitrate_kbps, parameters_.gopNum);
                if (encoderBuffer_) {
                    if (!SsbSipH264EncodeExe(encoderBuffer_)) {
                        encoderBufferIn_ = (unsigned char*)SsbSipH264EncodeGetInBuf(encoderBuffer_, 80000);
                        if (encoderBufferIn_) {
                            result = true;
                        }
                    } else {
                        SsbSipH264EncodeDeInit(encoderBuffer_);
                        encoderBuffer_ = 0;
                    }
                }
            }
            return result;
        }

        bool H264Encoder::Encode(unsigned char const * const yuv420Data, unsigned char* const outBuffer, size_t const outBufferSize, size_t& encodedSize) {
            bool result = false;
            //Compress the data...to h264
            memcpy(encoderBufferIn_, yuv420Data, parameters_.width * parameters_.height + parameters_.width * parameters_.height / 2);
            int r = SsbSipH264EncodeExe(encoderBuffer_);
            if (r == SSBSIP_H264_ENC_RET_OK) {
                long size = 0;
                encoderBufferOut_ = (unsigned char*)SsbSipH264EncodeGetOutBuf(encoderBuffer_, &size);
                if (encoderBufferOut_) {
                    if ((size_t)size <= outBufferSize) {
                        memcpy(outBuffer, encoderBufferOut_, size);
                        encodedSize = size;
                        result = true;
                    }
                }
            }
            return result;
        }

        void H264Encoder::Uninit() {
            if (isParametersSet_) {
                if (encoderBuffer_) {
                    SsbSipH264EncodeDeInit(encoderBuffer_);
                    encoderBuffer_ = 0;
                }
                isParametersSet_ = false;
            }
        }

        H264Decoder::H264Decoder()
        : VideoDecoder()
        , parameters_(VideoParameters())
        , isParametersProbe_(false)
        , decoderBuffer_(0)
        , decoderBufferIn_(0)
        , decoderBufferOut_(0) {
        }

        bool H264Decoder::Init() {
            bool result = false;
            // Initialize conversion table for YUV420 to RGB
            // InitConvertTable();
            // Initialize decompressor
            decoderBuffer_ = (unsigned char*)SsbSipH264DecodeInit();
            if (decoderBuffer_) {
                decoderBufferIn_ = (unsigned char*)SsbSipH264DecodeGetInBuf(decoderBuffer_, 80000);
                if (decoderBufferIn_) {
                    result = true;
                } else {
                    SsbSipH264DecodeDeInit(decoderBuffer_);
                    decoderBuffer_ = 0;
                }
            }
            return result;
        }

        bool H264Decoder::Decode(unsigned char const * const h264Data, size_t const h264DataSize, unsigned char* const outYuv420Data, size_t& decodedLength) {
            bool result = false;
            memcpy(decoderBufferIn_, h264Data, h264DataSize);
            if (!isParametersProbe_) {
                int r = SsbSipH264DecodeExe(decoderBuffer_, h264DataSize);
                if (!r) {
                    SSBSIP_H264_STREAM_INFO streamInfo;
                    r = SsbSipH264DecodeGetConfig(decoderBuffer_, H264_DEC_GETCONF_STREAMINFO, &streamInfo);
                    if (!r) {
                        parameters_.width = streamInfo.width;
                        parameters_.height = streamInfo.height;
                        result = true;
                        isParametersProbe_ = true;
                    }
                }
            } else {
                int r = SsbSipH264DecodeExe(decoderBuffer_, h264DataSize);        
                if (!r) {
                    long size = 0;
                    decoderBufferOut_ = (unsigned char*)SsbSipH264DecodeGetOutBuf(decoderBuffer_, &size);
                    if (decoderBufferOut_) {
                        if ((size_t)size <= decodedLength) {
                            memcpy(outYuv420Data, decoderBufferOut_, size);             
                            decodedLength = size;
                            result = true;
                        }
                    }
                }
            }
            return result;
        }

        void H264Decoder::GetParameters(VideoParameters& p) {
            if (isParametersProbe_) {
                p = parameters_;
                //p.width = parameters_.width;
                //p.height = parameters_.height;
                //p.bitrate_kbps = parameters_.bitrate_kbps;
                //p.framerate = parameters_.framerate;
                //p.gopNum = parameters_.gopNum;
                //p.quantInter = parameters_.quantInter;
                //p.quantIntra = parameters_.quantIntra;
            }
        }

        void H264Decoder::Uninit() {
            if (isParametersProbe_) {
                // close decoder ......
                if (decoderBuffer_) {
                    SsbSipH264DecodeDeInit(decoderBuffer_);
                    decoderBuffer_ = 0;
                }
            }
            isParametersProbe_ = false;
        }
    }
}
