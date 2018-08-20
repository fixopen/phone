#ifndef __DATA_TELEPHONENUMBER_H__
#define __DATA_TELEPHONENUMBER_H__

#pragma warning(disable: 4786)
#include <string>
#include <vector>

namespace Data
{
	class TelephoneNumber
	{
    public:
        std::string const nationalNumber(void) const
        {
            return nationalNumber_;
        }
        void nationalNumber(std::string const nationalNumber)
        {
            nationalNumber_ = nationalNumber;
        }
        std::string const regionNumber(void) const
        {
            return regionNumber_;
        }
        void regionNumber(std::string const regionNumber)
        {
            regionNumber_ = regionNumber;
        }
        std::string const number(void) const
        {
            return number_;
        }
        void number(std::string const number)
        {
            number_ = number;
        }
        std::string const subNumber(void) const
        {
            return subNumber_;
        }
        void subNumber(std::string const subNumber)
        {
            subNumber_ = subNumber;
        }
	private:
		std::string nationalNumber_;
		std::string regionNumber_;
		std::string number_;
		std::string subNumber_;
	private:
		static std::string ipPrefix_;
		static std::string outlinePrefix_;
		static std::string virtualPrefix_;
	private:
		static std::string thisNationalNumber_;
		static std::vector<std::string> worldNotionalNumbers_; //所有国家编号
		static std::string thisRegionNumber_;
		static std::vector<std::string> chinaRegionNumbers_; //中国所有区号
	public:
		static TelephoneNumber ThisTelephoneNumber;
	public:
		TelephoneNumber(); //create
		TelephoneNumber(std::string number, bool isWhole = false); //create form single string
		~TelephoneNumber(); //destroy
		bool const IsEquals(std::string const number) const; //compare telephoneNumber is equal or not
		std::string const ToString() const; //convert to single string
	};
}

#endif //__DATA_TELEPHONENUMBER_H__
