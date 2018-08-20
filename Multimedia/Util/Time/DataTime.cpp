#include "DateTime.h"

namespace Util {
    namespace Time {
        Timestamp const Timestamp::Now() {
            Timestamp result;
            return result;
        }

        Timestamp const Timestamp::Parse(std::wstring const& content) {
            Timestamp result;
            return result;
        }

        std::wstring const Timestamp::ToString() const {
            std::wstring result;
            return result;
        }

        int Timestamp::GetYear() const {
            int result = 0;
            return result;
        }

        int Timestamp::GetMonth() const {
            int result = 0;
            return result;
        }

        int Timestamp::GetDay() const {
            int result = 0;
            return result;
        }

        int Timestamp::GetWeekDay() const {
            int result = 0;
            return result;
        }

        int Timestamp::GetHour() const {
            int result = 0;
            return result;
        }

        int Timestamp::GetMinute() const {
            int result = 0;
            return result;
        }

        int Timestamp::GetSecond() const {
            int result = 0;
            return result;
        }

        TimeDuration const TimeDuration::Parse(std::wstring const& content) {
            TimeDuration result(0, 0, 0, 0);
            return result;
        }

        TimeDuration::TimeDuration(int const day, int const hour, int const minute, int const second) {
        }

        std::wstring const TimeDuration::ToString() const {
            std::wstring result;
            return result;
        }

        int TimeDuration::GetDays() const {
            int result = 0;
            return result;
        }

        int TimeDuration::GetHours() const {
            int result = 0;
            return result;
        }

        int TimeDuration::GetMinutes() const {
            int result = 0;
            return result;
        }

        int TimeDuration::GetSeconds() const {
            int result = 0;
            return result;
        }
    }
}

//Timestamp const StringOp::ToTimestamp(std::wstring const& content) {
//    Timestamp result;
//    if (content.length() != 14) {
//        return result;
//    }
//    //YYYYMMDDHHMMSS format
//    int year = ToInt(content.substr(0, 4));
//    if (year < 1900)
//        year = 1900;
//    int month = ToInt(content.substr(4, 2));
//    if (month < 1)
//        month = 1;
//    if (month > 12)
//        month = 12;
//    int day = ToInt(content.substr(6, 2));
//    if (day < 1)
//        day = 1;
//    if (day > 31)
//        day = 31;
//    int hour = ToInt(content.substr(8, 2));
//    if (hour < 0)
//        hour = 0;
//    if (hour > 23)
//        hour = 23;
//    int minute = ToInt(content.substr(10, 2));
//    if (minute < 0)
//        minute = 0;
//    if (minute > 59)
//        minute = 59;
//    int second = ToInt(content.substr(12, 2));
//    if (second < 0)
//        second = 0;
//    if (second > 59)
//        second = 59;
//    result = Timestamp(year, month, day, hour, minute, second);
//    return result;
//}

//TimeSpan const StringOp::ToTimeSpan(std::wstring const& content) {
//    int time = ToInt(content, 10);
//    TimeSpan result(time);
//    return result;
//}

//std::wstring const StringOp::FromTimestamp(Timestamp const& timestamp) {
//    char buffer[256] = {0};
//    //sprintf_s(buffer, 256, "%04d%02d%02d%02d%02d%02d", timestamp.GetYear(), timestamp.GetMonth(), timestamp.GetDay(), timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond());
//    sprintf(buffer, "%04d%02d%02d%02d%02d%02d", timestamp.GetYear(), timestamp.GetMonth(), timestamp.GetDay(), timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond());
//    std::wstring result = FromUTF8((char*)buffer);
//    return result;
//}

//std::wstring const StringOp::FromTimeSpan(TimeSpan const& timeSpan) {
//    return FromInt((int const)timeSpan.GetTotalSeconds());
//}
