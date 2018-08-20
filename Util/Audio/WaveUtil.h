#ifndef __UTIL_AUDIO_WAVEUTIL_H__
#define __UTIL_AUDIO_WAVEUTIL_H__

#include "OggCodec.h"

namespace Util {
    namespace Audio {
        enum WAVEDEV_MSG {
            WAV_PLAYBACK_FLAG       = 0x10000000,

            WAV_LOCAL_OUT = 0x10000001,
            WAV_PSTN_OUT = 0x10000002,
            WAV_HANDSET_OUT = 0x10000004,

            WAV_SNDEFFECT_POP		= 0x20000000,
            WAV_SNDEFFECT_3D		= 0x20000001,
            WAV_SNDEFFECT_CLASSIC	= 0x20000002,
            WAV_SNDEFFECT_JAZZ		= 0x20000003,

            WAV_RECORD_FLAG			= 0x30000000,

            WAV_PSTN_IN = 0x30000001,
            WAV_LOCAL_IN = 0x30000002,
            WAV_HANDSET_IN = 0x30000004,

            WAV_SND_SOFTMUTE_ON		= 0x40000003,
            WAV_SND_SOFTMUTE_OFF	= 0x40000004,

            WAV_MIC_GAIN   			= 0x40000001,
        };

        OggCodec* const GetOggCodec();
    }
}

#endif //__UTIL_AUDIO_WAVEUTIL_H__
