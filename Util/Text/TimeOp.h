#ifndef __UTIL_TEXT_TIMEOP_H__
#define __UTIL_TEXT_TIMEOP_H__

#include <string>
#include <vector>
#include "../DateTime.h"

namespace Util {
    namespace Text {
        class TimeOp {
        public:
            static void Init();
            static std::vector<std::wstring> DefaultDurationUnits;
            enum Part {
                pDateTime,
                pDate,
                pTime,
            };
            static std::wstring const ToString(Timestamp const& timestamp, Part const part = pDateTime, std::wstring const& separtors = L"/ :");
            static std::wstring const ToString(TimeSpan const& duration, std::vector<std::wstring> const& durationUnits = DefaultDurationUnits);
            static Timestamp const FromString(std::wstring const& timestamp, Part const part = pDateTime, std::wstring const& separtors = L"/ :");
            static TimeSpan const FromString(std::wstring const& duration, std::vector<std::wstring> const& durationUnits = DefaultDurationUnits);
        };
    }
}

#endif //__UTIL_TEXT_TIMEOP_H__