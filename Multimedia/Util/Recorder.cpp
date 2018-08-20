#include "Recorder.h"

namespace Util {
    Recorder::Recorder(Stream const* const stream, MediaContainer const* const container, Encoder const* const encoder, Capture const* const capture)
        : stream_(stream)
        , container_(container)
        , encoder_(encoder)
        , capture_(capture) {
    }


    Recorder::~Recorder() {
    }
}
