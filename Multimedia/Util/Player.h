#ifndef __UTIL_PLAYER_H__
#define __UTIL_PLAYER_H__

#pragma once

#include "Stream.h"
#include "MediaContainer.h"
#include "Decoder.h"
#include "Render.h"

namespace Util {
    class Player {
    public:
        Player(Stream const* const stream, MediaContainer const* const container, Decoder const* const decoder, Render const* const render);
        ~Player();
    private:
        Stream const* const stream_;
        MediaContainer const* const container_;
        Decoder const* const decoder_;
        Render const* const render_;
    };
}

#endif //__UTIL_PLAYER_H__
