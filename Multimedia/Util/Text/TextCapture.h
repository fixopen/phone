#ifndef __UTIL_TEXT_TEXTCAPTURE_H__
#define __UTIL_TEXT_TEXTCAPTURE_H__

#pragma once

#include "../Capture.h"

namespace Util {
    namespace Text {
        class TextCapture : public Capture {
        public:
            virtual ~TextCapture();
        };
    }
}

#endif //__UTIL_TEXT_TEXTCAPTURE_H__
