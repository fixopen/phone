#include "TimeOp.h"
#include "../Text/StringOp.h" //for Text::StringOp
#include <WinDef.h> //for WinBase.h
#include <WinBase.h> //for TIME_ZONE_INFORMATION

namespace Util {
    namespace Time {
        int const TimeOp::days[mCount] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        std::map<Month, std::wstring> TimeOp::monthName;
        std::vector<std::wstring> TimeOp::DefaultDurationUnits;

        void TimeOp::Init() {
            TIME_ZONE_INFORMATION timeZoneInfo;
            ::GetTimeZoneInformation(&timeZoneInfo);
            timeZoneInfo.Bias = 0 * 60;
            ::SetTimeZoneInformation(&timeZoneInfo);
            monthName.insert(std::make_pair(mJanuary, L"January"));
            monthName.insert(std::make_pair(mFebruary, L"February"));
            monthName.insert(std::make_pair(mMarch, L"March"));
            monthName.insert(std::make_pair(mApril, L"April"));
            monthName.insert(std::make_pair(mMay, L"May"));
            monthName.insert(std::make_pair(mJune, L"June"));
            monthName.insert(std::make_pair(mJuly, L"July"));
            monthName.insert(std::make_pair(mAugust, L"August"));
            monthName.insert(std::make_pair(mSeptember, L"September"));
            monthName.insert(std::make_pair(mOctober, L"October"));
            monthName.insert(std::make_pair(mNovember, L"November"));
            monthName.insert(std::make_pair(mDecember, L"December"));
            DefaultDurationUnits.push_back(L"s");
            DefaultDurationUnits.push_back(L"m");
            DefaultDurationUnits.push_back(L"h");
            DefaultDurationUnits.push_back(L"d");
        }

        std::wstring const TimeOp::ToString(int const month) {
            Month monthId = static_cast<Month>(month);
            return Text::StringOp::ToUpper(monthName[monthId].substr(0, 3));
        }

        std::wstring const TimeOp::ToString(Timestamp const& timestamp, TimeOp::Part const part, std::wstring const& separtors) {
            std::wstring result;
            char buffer[32] = {0};
            std::string month = Text::StringOp::ToUTF8(ToString(timestamp.GetMonth()));

            switch (part) {
                case pDateTime:
                    sprintf(buffer, "%02d%c%s%c%04d%c%02d%c%02d%c%02d",
                            timestamp.GetDay(), separtors[0],
                            month.c_str(), separtors[0],
                            timestamp.GetYear(), separtors[1],
                            timestamp.GetHour(), separtors[2],
                            timestamp.GetMinute(), separtors[2],
                            timestamp.GetSecond());
                    result = Text::StringOp::FromUTF8(buffer);
                    break;
                case pDate:
                    sprintf(buffer, "%02d%c%s%c%04d",
                            timestamp.GetDay(), separtors[0],
                            month.c_str(), separtors[0],
                            timestamp.GetYear());
                    result = Text::StringOp::FromUTF8(buffer);
                    break;
                case pYearMonth:
                    sprintf(buffer, "%s%c%04d", month.c_str(), separtors[0], timestamp.GetYear());
                    result = Text::StringOp::FromUTF8(buffer);
                    break;
                case pMonth:
                    sprintf(buffer, "%s", month.c_str());
                    result = Text::StringOp::FromUTF8(buffer);
                    break;
                case pTime:
                    sprintf(buffer, "%02d%c%02d%c%02d",
                            timestamp.GetHour(), separtors[0],
                            timestamp.GetMinute(), separtors[0],
                            timestamp.GetSecond());
                    result = Text::StringOp::FromUTF8(buffer);
                    break;
                default:
                    break;
            }

            return result;
        }

        std::wstring const TimeOp::ToString(TimeDuration const& duration, std::vector<std::wstring> const& durationUnits) {
            size_t seconds = duration.GetSeconds();
            std::wstring result = Text::StringOp::FromInt(seconds) + durationUnits[0];
            size_t minutes = duration.GetMinutes();
            result = Text::StringOp::FromInt(minutes) + durationUnits[1] + result;
            size_t hours = duration.GetHours();
            result = Text::StringOp::FromInt(hours) + durationUnits[2] + result;
            size_t days = static_cast<size_t>(duration.GetDays());
            result = Text::StringOp::FromInt(days) + durationUnits[3] + result;
            return result;
        }

        Timestamp const TimeOp::FromString(std::wstring const& timestamp, TimeOp::Part const part, std::wstring const& separtors) {
            Timestamp result;

            switch (part) {
                case pDateTime: {
                        std::wstring separtor;
                        separtor += separtors[1];
                        std::vector<std::wstring> dateAndTime = Text::StringOp::Split(timestamp, separtor);
                        separtor = L"";
                        separtor += separtors[0];
                        std::vector<std::wstring> dates = Text::StringOp::Split(dateAndTime[0], separtor);
                        tm t;
                        t.tm_year = Text::StringOp::ToInt(dates[0]) - 1900;
                        t.tm_mon = Text::StringOp::ToInt(dates[1]) - 1;
                        t.tm_mday = Text::StringOp::ToInt(dates[2]);
                        separtor = L"";
                        separtor += separtors[2];
                        std::vector<std::wstring> times = Text::StringOp::Split(dateAndTime[1], separtor);
                        t.tm_hour = Text::StringOp::ToInt(times[0]);
                        t.tm_min = Text::StringOp::ToInt(times[1]);
                        t.tm_sec = Text::StringOp::ToInt(times[2]);
                        Timestamp result; // = _mktime64(&t);
                        return result;
                    }
                    break;
                case pDate: {
                        std::vector<std::wstring> dates = Text::StringOp::Split(timestamp, separtors);
                        tm t;
                        t.tm_year = Text::StringOp::ToInt(dates[0]) - 1900;
                        t.tm_mon = Text::StringOp::ToInt(dates[1]) - 1;
                        t.tm_mday = Text::StringOp::ToInt(dates[2]);
                        t.tm_hour = 0;
                        t.tm_min = 0;
                        t.tm_sec = 0;
                        Timestamp result; // = _mktime64(&t);
                        return result;
                    }
                    break;
                case pTime: {
                        Timestamp result = Timestamp::Now();
                        tm t;
                        t.tm_year = result.GetYear() - 1900;
                        t.tm_mon = result.GetMonth() - 1;
                        t.tm_mday = result.GetDay();
                        std::vector<std::wstring> times = Text::StringOp::Split(timestamp, separtors);
                        t.tm_hour = Text::StringOp::ToInt(times[0]);
                        t.tm_min = Text::StringOp::ToInt(times[1]);
                        t.tm_sec = Text::StringOp::ToInt(times[2]);
                        //result = _mktime64(&t);
                        return result;
                    }
                    break;
                default:
                    break;
            }

            return result;
        }

        TimeDuration const TimeOp::FromString(std::wstring const& duration, std::vector<std::wstring> const& durationUnits) {
            int day = 0, hour = 0, minute = 0, second = 0;
            size_t startPos = 0;
            size_t stopPos = duration.find(durationUnits[3], startPos);

            if (stopPos != std::wstring::npos) {
                day = Text::StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
                startPos += durationUnits[3].length() + 1;
            }

            stopPos = duration.find(durationUnits[2], startPos);

            if (stopPos != std::wstring::npos) {
                hour = Text::StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
                startPos += durationUnits[2].length() + 1;
            }

            stopPos = duration.find(durationUnits[1], startPos);

            if (stopPos != std::wstring::npos) {
                minute = Text::StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
                startPos += durationUnits[1].length() + 1;
            }

            stopPos = duration.find(durationUnits[0], startPos);

            if (stopPos != std::wstring::npos) {
                second = Text::StringOp::ToInt(duration.substr(startPos, stopPos - startPos));
                //startPos += durationUnits[0].length() + 1;
            }

            TimeDuration result = TimeDuration(day, hour, minute, second);
            return result;
        }
    }
}
