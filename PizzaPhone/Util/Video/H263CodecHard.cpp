#include <stdlib.h> //for memcpy
#include "H263CodecHard.h"

namespace Util {
    namespace Video {
        H263Encoder::H263Encoder()
        : VideoEncoder()
        , parameters_(VideoParameters())
        , isParametersSet_(false)
        , encoderBuffer_(0)
        , encoderBufferIn_(0)
        , encoderBufferOut_(0) {
        }

        void H263Encoder::SetParameters(VideoParameters const & p) {
            parameters_ = p;
            if ((parameters_.width == 176) && (parameters_.height == 144)) {
                isParametersSet_ = true;
            }
        }

        bool H263Encoder::Init() {
            bool result = false;
            if (isParametersSet_) {
                // Initialize table for RGB to YUV conversion
                // InitLookupTable();
                // Initialize the compressor
                encoderBuffer_ = (unsigned char*)SsbSipMPEG4EncodeInit(SSBSIPMFCENC_H263, parameters_.width, parameters_.height, parameters_.framerate, parameters_.bitrate_kbps, parameters_.gopNum);
                if (encoderBuffer_) {
                    if (!SsbSipMPEG4EncodeExe(encoderBuffer_)) {
                        encoderBufferIn_ = (unsigned char*)SsbSipMPEG4EncodeGetInBuf(encoderBuffer_, 80000);
                        if (encoderBufferIn_) {
                            result = true;
                        }
                    } else {
                        SsbSipMPEG4EncodeDeInit(encoderBuffer_);
                        encoderBuffer_ = 0;
                    }
                }
            }
            return result;
        }

        bool H263Encoder::Encode(unsigned char const * const yuv420Data, unsigned char* const outBuffer, size_t const outBufferSize, size_t& encodedSize) {
            bool result = false;
            //Compress the data...to h263
            memcpy(encoderBufferIn_, yuv420Data, parameters_.width * parameters_.height + parameters_.width * parameters_.height / 2);
            int r = SsbSipMPEG4EncodeExe(encoderBuffer_);
            if (r == SSBSIP_MPEG4_ENC_RET_OK) {
                long size = 0;
                encoderBufferOut_ = (unsigned char*)SsbSipMPEG4EncodeGetOutBuf(encoderBuffer_, &size);
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

        void H263Encoder::Uninit() {
            if (isParametersSet_) {
                if (encoderBuffer_) {
                    SsbSipMPEG4EncodeDeInit(encoderBuffer_);
                    encoderBuffer_ = 0;
                }
                isParametersSet_ = false;
            }
        }

        H263Decoder::H263Decoder()
        : VideoDecoder()
        , parameters_(VideoParameters())
        , isParametersProbe_(false)
        , decoderBuffer_(0)
        , decoderBufferIn_(0)
        , decoderBufferOut_(0) {
        }

        bool H263Decoder::Init() {
            bool result = false;
            // Initialize conversion table for YUV420 to RGB
            // InitConvertTable();
            // Initialize decompressor
            decoderBuffer_ = (unsigned char*)SsbSipMPEG4DecodeInit();
            if (decoderBuffer_) {
                decoderBufferIn_ = (unsigned char*)SsbSipMPEG4DecodeGetInBuf(decoderBuffer_, 80000);
                if (decoderBufferIn_) {
                    result = true;
                } else {
                    SsbSipMPEG4DecodeDeInit(decoderBuffer_);
                    decoderBuffer_ = 0;
                }
            }
            return result;
        }

        bool H263Decoder::Decode(unsigned char const * const h263Data, size_t const h263DataSize, unsigned char* const outYuv420Data, size_t& decodedLength) {
            bool result = false;
            memcpy(decoderBufferIn_, h263Data, h263DataSize);
            if (!isParametersProbe_) {
                int r = SsbSipMPEG4DecodeExe(decoderBuffer_, h263DataSize);
                if (!r) {
                    SSBSIP_MPEG4_STREAM_INFO streamInfo;
                    r = SsbSipMPEG4DecodeGetConfig(decoderBuffer_, MPEG4_DEC_GETCONF_STREAMINFO, &streamInfo);
                    if (!r) {
                        parameters_.width = streamInfo.width;
                        parameters_.height = streamInfo.height;
                        result = true;
                        isParametersProbe_ = true;
                    }
                }
            } else {
                int r = SsbSipMPEG4DecodeExe(decoderBuffer_, h263DataSize);        
                if (!r) {
                    long size = 0;
                    decoderBufferOut_ = (unsigned char*)SsbSipMPEG4DecodeGetOutBuf(decoderBuffer_, &size);
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

        void H263Decoder::GetParameters(VideoParameters& p) {
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

        void H263Decoder::Uninit() {
            if (isParametersProbe_) {
                // close decoder ......
                if (decoderBuffer_) {
                    SsbSipMPEG4DecodeDeInit(decoderBuffer_);
                    decoderBuffer_ = 0;
                }
            }
            isParametersProbe_ = false;
        }
    }
}
