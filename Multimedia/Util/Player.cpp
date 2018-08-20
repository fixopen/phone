#include "Player.h"

namespace Util {
    Player::Player(Stream const* const stream, MediaContainer const* const container, Decoder const* const decoder, Render const* const render)
        : stream_(stream)
        , container_(container)
        , decoder_(decoder)
        , render_(render) {
    }


    Player::~Player() {
    }
}