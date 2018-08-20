#ifndef __UTIL_PLAYER_H__
#define __UTIL_PLAYER_H__

#include <vector>
#include <string>
#include "DateTime.h"

namespace Util {
    class Player {
    public:
        typedef void (*EndProc)();
        Player();
        virtual ~Player() = 0 {}
        void SetPlayList(std::vector<std::wstring> const& filelist);
        void SetPlayFile(std::wstring const& filename);
        void SetPlayEndProc(EndProc const endProc);
        std::wstring const& GetCurrentMediaName() const;
        virtual void Start() = 0;
        virtual void Previous() = 0;
        virtual void Next() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(size_t const volume) = 0;
        virtual void SetLightness(size_t const lightness) = 0;
        virtual void SetContract(size_t const contract) = 0;
        virtual TimeSpan const GetCurrentMediaDuration() const = 0;
        virtual size_t const GetCurrentMediaPlayPrecent() const = 0;
    private:
        std::vector<std::wstring> filelist_;
        size_t currentIndex_;
        EndProc endProc_;
    };
}

#endif //__UTIL_PLAYER_H__
