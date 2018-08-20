#include "TimeOp.h"
#include "StringOp.h"

namespace Util {
    std::vector<std::wstring> TimeOp::DefaultDurationUnits;
    void TimeOp::Init() {
        DefaultDurationUnits.push_back(L"s");
        DefaultDurationUnits.push_back(L"m");
        DefaultDurationUnits.push_back(L"h");
        DefaultDurationUnits.push_back(L"d");
    }
    std::wstring const TimeOp::ToString(Timestamp const& timestamp, TimeOp::Part const part, std::wstring const& separtors) {
        std::wstring result;
        switch (part) {
            case pDateTime:
                result = Util::StringOp::FromInt(timestamp.GetYear()) + separtors[0]
                    + Util::StringOp::FromInt(timestamp.GetMonth()) + separtors[0]
                    + Util::StringOp::FromInt(timestamp.GetDay()) + separtors[1]
                    + Util::StringOp::FromInt(timestamp.GetHour()) + separtors[2]
                    + Util::StringOp::FromInt(timestamp.GetMinute()) + separtors[2]
                    + Util::StringOp::FromInt(timestamp.GetSecond());
                break;
            case pDate:
                result = Util::StringOp::FromInt(timestamp.GetYear()) + separtors[0]
                    + Util::StringOp::FromInt(timestamp.GetMonth()) + separtors[0]
                    + Util::StringOp::FromInt(timestamp.GetDay());
                break;
            case pTime:
                result = Util::StringOp::FromInt(timestamp.GetHour()) + separtors[0]
                    + Util::StringOp::FromInt(timestamp.GetMinute()) + separtors[0]
                    + Util::StringOp::FromInt(timestamp.GetSecond());
                break;
            default:
                break;
        }
        return result;
    }

    std::wstring const TimeOp::ToString(TimeSpan const& duration, std::vector<std::wstring> const& durationUnits) {
        size_t seconds = duration.GetSeconds();
        std::wstring result = Util::StringOp::FromInt(seconds) + durationUnits[0];
        size_t minutes = duration.GetMinutes();
        result = Util::StringOp::FromInt(minutes) + durationUnits[1] + result;
        size_t hours = duration.GetHours();
        result = Util::StringOp::FromInt(hours) + durationUnits[2] + result;
        size_t days = duration.GetDays();
        result = Util::StringOp::FromInt(days) + durationUnits[3] + result;
        return result;
    }

    Timestamp const TimeOp::FromString(std::wstring const& timestamp, TimeOp::Part const part, std::wstring const& separtors) {
        Timestamp result;
        switch (part) {
            case pDateTime:
                {
                std::wstring separtor;
                separtor += separtors[1];
                std::vector<std::wstring> dateAndTime = Util::StringOp::Split(timestamp, separtor);
                separtor = L"";
                separtor += separtors[0];
                std::vector<std::wstring> dates = Util::StringOp::Split(dateAndTime[0], separtor);
                tm t;
                t.tm_year = Util::StringOp::ToInt(dates[0]) - 1900;
                t.tm_mon = Util::StringOp::ToInt(dates[1]) - 1;
                t.tm_mday = Util::StringOp::ToInt(dates[2]);
                separtor = L"";
                separtor += separtors[2];
                std::vector<std::wstring> times = Util::StringOp::Split(dateAndTime[1], separtor);
                t.tm_hour = Util::StringOp::ToInt(times[0]);
                t.tm_min = Util::StringOp::ToInt(times[1]);
                t.tm_sec = Util::StringOp::ToInt(times[2]);
                Timestamp result = _mktime64(&t);
                return result;
                }
                break;
            case pDate:
                {
                std::vector<std::wstring> dates = Util::StringOp::Split(timestamp, separtors);
                tm t;
                t.tm_year = Util::StringOp::ToInt(dates[0]) - 1900;
                t.tm_mon = Util::StringOp::ToInt(dates[1]) - 1;
                t.tm_mday = Util::StringOp::ToInt(dates[2]);
                t.tm_hour = 0;
                t.tm_min = 0;
                t.tm_sec = 0;
                Timestamp result = _mktime64(&t);
                return result;
                }
                break;
            case pTime:
                {
                Timestamp result = Timestamp::GetCurrentTime();
                tm t;
                t.tm_year = result.GetYear() - 1900;
                t.tm_mon = result.GetMonth() - 1;
                t.tm_mday = result.GetDay();
                std::vector<std::wstring> times = Util::StringOp::Split(timestamp, separtors);
                t.tm_hour = Util::StringOp::ToInt(times[0]);
                t.tm_min = Util::StringOp::ToInt(times[1]);
                t.tm_sec = Util::StringOp::ToInt(times[2]);
                result = _mktime64(&t);
                return result;
                }
                break;
            default:
                break;
        }
        return result;
    }

    TimeSpan const TimeOp::FromString(std::wstring const& duration, std::vector<std::wstring> const& durationUnits) {
        int day = 0, hour = 0, minute = 0, second = 0;
        size_t startPos = 0;
        size_t stopPos = duration.find(durationUnits[3], startPos);
        if (stopPos != std::wstring::npos) {
            day = StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
            startPos += durationUnits[3].length() + 1;
        }
        stopPos = duration.find(durationUnits[2], startPos);
        if (stopPos != std::wstring::npos) {
            hour = StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
            startPos += durationUnits[2].length() + 1;
        }
        stopPos = duration.find(durationUnits[1], startPos);
        if (stopPos != std::wstring::npos) {
            minute = StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
            startPos += durationUnits[1].length() + 1;
        }
        stopPos = duration.find(durationUnits[0], startPos);
        if (stopPos != std::wstring::npos) {
            second = StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
            //startPos += durationUnits[0].length() + 1;
        }
        TimeSpan result = TimeSpan(day, hour, minute, second);
        return result;
    }
}
