#ifndef __UTIL_TIME_DATETIME_H__
#define __UTIL_TIME_DATETIME_H__

#include <string> //for std::wstring
//#include <winbase.h> //for next include file
//#include <atltime.h> //for CTime, CTimeSpan

namespace Util {
    namespace Time {
        //typedef CTime Timestamp;
        class Timestamp {
        public:
            static Timestamp const Now();
            static Timestamp const Parse(std::wstring const& content);
            std::wstring const ToString() const;
            int GetYear() const;
            int GetMonth() const;
            int GetDay() const;
            int GetWeekDay() const;
            int GetHour() const;
            int GetMinute() const;
            int GetSecond() const;
        };
        //typedef CTimeSpan TimeSpan;
        class TimeDuration {
        public:
            static TimeDuration const Parse(std::wstring const& content);
            TimeDuration(int const day, int const hour, int const minute, int const second);
            std::wstring const ToString() const;
            int GetDays() const;
            int GetHours() const;
            int GetMinutes() const;
            int GetSeconds() const;
        };
    }
}

#endif //__UTIL_TIME_DATETIME_H__