#ifndef __UTIL_MEDIACONTAINER_H__
#define __UTIL_MEDIACONTAINER_H__

#pragma once

namespace Util {
    class MediaContainer {
    public:
        virtual ~MediaContainer();
        virtual void Split();
        virtual void Mix();
    };
}

#endif //__UTIL_MEDIACONTAINER_H__
