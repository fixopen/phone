#ifndef __UTIL_RECORDER_H__
#define __UTIL_RECORDER_H__

#pragma once

#include "Stream.h"
#include "MediaContainer.h"
#include "Encoder.h"
#include "Capture.h"

namespace Util {
    class Recorder {
    public:
        Recorder(Stream const* const stream, MediaContainer const* const container, Encoder const* const encoder, Capture const* const capture);
        ~Recorder();
    private:
        Stream const* const stream_;
        MediaContainer const* const container_;
        Encoder const* const encoder_;
        Capture const* const capture_;
    };
}

#endif //__UTIL_RECORDER_H__
