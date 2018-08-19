#include "Player.h"

namespace Util {
    Player::Player()
    : currentIndex_(0)
    , endProc_(0) {
    }

    void Player::SetPlayList(std::vector<std::wstring> const& filelist) {
        filelist_ = filelist;
        currentIndex_ = 0;
    }

    void Player::SetPlayFile(std::wstring const& filename) {
        filelist_.clear();
        filelist_.push_back(filename);
        currentIndex_ = 0;
    }

    void Player::SetPlayEndProc(EndProc const endProc) {
        endProc_ = endProc;
    }

    std::wstring const& Player::GetCurrentMediaName() const {
        if (!filelist_.empty()) {
            return filelist_[currentIndex_];
        }
        return L"";
    }
}