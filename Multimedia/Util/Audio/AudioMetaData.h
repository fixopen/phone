#ifndef __UTIL_AUDIO_AUDIOMETADATA_H__
#define __UTIL_AUDIO_AUDIOMETADATA_H__

#pragma once

#include "../MetaData.h"

namespace Util {
    namespace Audio {
        class AudioMetaData : public MetaData {
        public:
            virtual ~AudioMetaData();
        };
    }
}

#endif //__UTIL_AUDIO_AUDIOMETADATA_H__
