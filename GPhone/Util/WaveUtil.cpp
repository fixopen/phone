#include "WaveUtil.h"

namespace Util {
    OggCodec* const GetOggCodec() {
        static OggCodec* result = new OggCodec();
        return result;
    }
}
