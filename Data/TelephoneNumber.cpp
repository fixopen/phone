#include "../stdafx.h"
#include "TelephoneNumber.h"

namespace Data
{
    std::string TelephoneNumber::ipPrefix_;
    std::string TelephoneNumber::outlinePrefix_;
    std::string TelephoneNumber::virtualPrefix_;
    std::vector<std::string> TelephoneNumber::worldNotionalNumbers_; //���й��ұ��
	std::vector<std::string> TelephoneNumber::chinaRegionNumbers_; //�й���������
    TelephoneNumber TelephoneNumber::ThisTelephoneNumber;

    TelephoneNumber::TelephoneNumber()
    {
    }

    TelephoneNumber::TelephoneNumber(std::string number, bool isWhole)
    : number_(number)
    {
    }

    TelephoneNumber::~TelephoneNumber()
    {
        //do nothing
    }

    bool const TelephoneNumber::IsEquals(std::string const number) const
    {
        bool result = false;
        if (number == number_)
        {
            result = true;
        }
        return result;
    }

    std::string const TelephoneNumber::ToString() const
    {
        std::string result = number_;
        return result;
    }
}
