#include "WaveUtil.h"

OggCodec* const GetOggCodec() {
    static OggCodec* result = new OggCodec();
    return result;
}
