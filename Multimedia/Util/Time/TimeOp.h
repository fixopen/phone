#ifndef __UTIL_TIME_TIMEOP_H__
#define __UTIL_TIME_TIMEOP_H__

#include <map> //for std::map
#include <string> //for std::wstring
#include <vector> //for std::vector
#include "DateTime.h" //for Timestamp, TimeDuration

namespace Util {
    namespace Time {
        class Timestamp;
        class TimeDuration;

        enum Month {
            mJanuary,
            mFebruary,
            mMarch,
            mApril,
            mMay,
            mJune,
            mJuly,
            mAugust,
            mSeptember,
            mOctober,
            mNovember,
            mDecember,
            mCount
        };

        class TimeOp {
        public:
            static int const days[12];
            static std::map<Month, std::wstring> monthName;
            static void Init();
            static std::vector<std::wstring> DefaultDurationUnits;
            enum Part {
                pDateTime,
                pDate,
                pYearMonth,
                pMonth,
                pTime,
            };
            static std::wstring const ToString(int const month);
            static std::wstring const ToString(Timestamp const& timestamp, Part const part = pDateTime, std::wstring const& separtors = L" \n:");
            static std::wstring const ToString(TimeDuration const& duration, std::vector<std::wstring> const& durationUnits = DefaultDurationUnits);
            static Timestamp const FromString(std::wstring const& timestamp, Part const part = pDateTime, std::wstring const& separtors = L"/ :");
            static TimeDuration const FromString(std::wstring const& duration, std::vector<std::wstring> const& durationUnits = DefaultDurationUnits);
        };
    }
}

#endif //__UTIL_TIME_TIMEOP_H__