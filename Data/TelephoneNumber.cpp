#include "stdafx.h"
#include "TelephoneNumber.h"

namespace Data
{
    std::string TelephoneNumber::ipPrefix_;
    std::string TelephoneNumber::outlinePrefix_;
    std::string TelephoneNumber::virtualPrefix_;
    std::string TelephoneNumber::thisNationalNumber_ = "86";
    std::vector<std::string> TelephoneNumber::worldNotionalNumbers_; //所有国家编号
    std::string TelephoneNumber::thisRegionNumber_ = "10";
	std::vector<std::string> TelephoneNumber::chinaRegionNumbers_; //中国所有区号
    TelephoneNumber TelephoneNumber::ThisTelephoneNumber;

    TelephoneNumber::TelephoneNumber()
    : nationalNumber_(std::string())
    , regionNumber_(std::string())
    , number_(std::string())
    , subNumber_(std::string())
    {
    }

    TelephoneNumber::TelephoneNumber(std::string number, bool isWhole)
    : nationalNumber_(thisNationalNumber_)
    , regionNumber_(thisRegionNumber_)
    , number_(number)
    , subNumber_(std::string())
    {
        if (isWhole)
        {
            //nationalNumber_ = GetNationalNumber_(number_);
            //regionNumber_ = GetRegionNumber_(number_);
            //subNumber_ = GetSubNumber_(number_);
            //number_ = GetNumber_(number_);
        }
    }

    TelephoneNumber::~TelephoneNumber()
    {
        //do nothing
    }

    bool const TelephoneNumber::IsEquals(std::string const number) const
    {
        bool result = false;
        //std::string nationalNumber = GetNationalNumber_(number);
        //std::string regionNumber = GetRegionNumber_(number);
        //std::string subNumber = GetSubNumber_(number);
        //std::string number = GetNumber_(number);
        //if ((nationalNumber_ == nationalNumber)
        //	&& (regionNumber_ == regionNumber)
        //	&& (number_ == number)
        //	&& (subNumber_ == subNumber))
        result = true;
        return result;
    }

    std::string const TelephoneNumber::ToString() const
    {
        std::string result = "00";
        result += nationalNumber_;
        result += "0";
        result += regionNumber_;
        result += number_;
        result += subNumber_;
        return result;
    }
}
