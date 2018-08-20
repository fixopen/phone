#ifndef __UTIL_STATICMEDIALIST_H__
#define __UTIL_STATICMEDIALIST_H__

#pragma once

#include "MediaList.h"

namespace Util {
    class StaticMediaList : public MediaList {
    public:
        virtual ~StaticMediaList();
    };
}

#endif //__UTIL_STATICMEDIALIST_H__
