#include "TelephoneNumber.h"
namespace Contact_{
	namespace Data
	{
		std::wstring TelephoneNumber::ipPrefix_;
		std::wstring TelephoneNumber::outlinePrefix_;
		std::wstring TelephoneNumber::virtualPrefix_;
		std::vector<std::wstring> TelephoneNumber::worldNotionalNumbers_; //所有国家编号
		std::vector<std::wstring> TelephoneNumber::chinaRegionNumbers_; //中国所有区号
		//std::string TelephoneNumber::thisNationalNumber_ = "86";
		//std::string TelephoneNumber::thisRegionNumber_ = "10";
		TelephoneNumber TelephoneNumber::ThisTelephoneNumber;

		TelephoneNumber::TelephoneNumber()
		//: nationalNumber_(std::string())
		//, regionNumber_(std::string())
		//, number_(std::string())
		//, subNumber_(std::string())
		{
		}

		TelephoneNumber::TelephoneNumber(std::wstring const& number, bool isWhole)
		//: nationalNumber_(thisNationalNumber_)
		//, regionNumber_(thisRegionNumber_)
		: number_(number) 
		//, subNumber_(std::string())
		{
			if (isWhole)
			{
				//nationalNumber_ = GetNationalNumber_(number_);
				//regionNumber_ = GetRegionNumber_(number_);
				//subNumber_ = GetSubNumber_(number_);
				//number_ = GetNumber_(number_);
			}
			/*
			if (number startwith "13 || 14 || 15")
			{
	            
			}
			*/
			/*
			else
			{
				nationalNumber_ = thisNationalNumber_;
				regionNumber_ = regionNumber_;
				number_ = number;
				subNumber_ = "";
			}
			*/
		}
		/*
		TelephoneNumber::TelephoneNumber(std::string regionNumber, std::string number)
		: nationalNumber_(thisNationalNumber_)
		, regionNumber_(regionNumber)
		, number_(number)
		//, subNumber_(std::string())
		{
		}

		TelephoneNumber::TelephoneNumber(Type type, std::string nationalNumber, std::string regionNumber, std::string number, std::string subNumber)
		{
			switch (type)
			{
			case LocalTelephone:
				nationalNumber_ = nationalNumber == "" ? thisNationalNumber_ : nationalNumber;
				regionNumber_ = regionNumber == "" ? regionNumber_ : regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case RemoteTelephone:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case BranchTelephone:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case LocalLitSmartPhone:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case RemoteLitSmartPhone:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case LocalMobilePhone:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case RemoteMobilePhone:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case SpecService:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			case CalleePay:
				nationalNumber_ = nationalNumber;
				regionNumber_ = regionNumber;
				number_ = number;
				subNumber_ = subNumber;
				break;
			}
		}

		void TelephoneNumber::ConstructLocalNumber(TelephoneNumber& telephoneNumber, std::string number)
		{
			telephoneNumber.type(LocalTelephone);
			telephoneNumber.nationalNumber(thisNationalNumber_);
			telephoneNumber.regionNumber(thisRegionNumber_);
			telephoneNumber.number(number);
			//telephoneNumber.subNumber(subNumber);
		}

		void TelephoneNumber::ConstructRemoteNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number)
		{
			telephoneNumber.type(RemoteTelephone);
			telephoneNumber.nationalNumber(thisNationalNumber_);
			telephoneNumber.regionNumber(regionNumber);
			telephoneNumber.number(number);
			//telephoneNumber.subNumber(subNumber);
		}

		void TelephoneNumber::ConstructBranchNumber(TelephoneNumber& telephoneNumber, std::string subNumber)
		{
			telephoneNumber.type(BranchTelephone);
			telephoneNumber.nationalNumber(thisNationalNumber_);
			telephoneNumber.regionNumber(thisRegionNumber_);
			telephoneNumber.number(ThisTelephoneNumber.number());
			telephoneNumber.subNumber(subNumber);
		}

		void TelephoneNumber::ConstructLocalLitSmartNumber(TelephoneNumber& telephoneNumber, std::string number)
		{
			telephoneNumber.type(LocalLitSmartPhone);
			telephoneNumber.nationalNumber(thisNationalNumber_);
			telephoneNumber.regionNumber(thisRegionNumber_);
			telephoneNumber.number(number);
			//telephoneNumber.subNumber(subNumber);
		}

		void TelephoneNumber::ConstructRemoteLitSmartNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number)
		{
			telephoneNumber.type(RemoteLitSmartPhone);
			telephoneNumber.nationalNumber(thisNationalNumber_);
			telephoneNumber.regionNumber(regionNumber);
			telephoneNumber.number(number);
			//telephoneNumber.subNumber(subNumber);
		}

		void TelephoneNumber::ConstructLocalMobileNumber(TelephoneNumber& telephoneNumber, std::string number)
		{
			telephoneNumber.type(LocalMobilePhone);
			//telephoneNumber.nationalNumber(thisNationalNumber_);
			//telephoneNumber.regionNumber(thisRegionNumber_);
			telephoneNumber.number(number);
			//telephoneNumber.subNumber(subNumber);
		}

		void TelephoneNumber::ConstructRemoteMobileNumber(TelephoneNumber& telephoneNumber, std::string number)
		{
			telephoneNumber.type(RemoteMobilePhone);
			//telephoneNumber.nationalNumber(thisNationalNumber_);
			//telephoneNumber.regionNumber(thisRegionNumber_);
			telephoneNumber.number(number);
			//telephoneNumber.subNumber(subNumber);
		}
		*/
		TelephoneNumber::~TelephoneNumber()
		{
			//do nothing
		}

		bool const TelephoneNumber::IsEquals(std::wstring const& number) const
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
			if (number == number_)
			{
				result = true;
			}
			return result;
		}

		std::wstring const TelephoneNumber::ToString() const
		{
			/*
			std::string result = "00";
			result += nationalNumber_;
			result += "0";
			result += regionNumber_;
			result += number_;
			result += subNumber_;
			return result;
			*/
			std::wstring result = number_;
			return result;
		}
	}
}
