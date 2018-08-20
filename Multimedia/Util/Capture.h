#ifndef __UTIL_CAPTURE_H__
#define __UTIL_CAPTURE_H__

#pragma once

namespace Util {
    class Capture {
    public:
        virtual ~Capture();
        virtual void Start();
        virtual void Stop();
    };
}

#endif //__UTIL_CAPTURE_H__
