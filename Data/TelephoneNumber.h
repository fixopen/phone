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
        enum Type
        {
            Null,
            LocalTelephone, //'' 'nnnnnnnn' ''
            RemoteTelephone, //'rrrr' 'nnnnnnnn' ''
            BranchTelephone, //'' '' 'sss'
            LocalBranchTelephone, //'' 'nnnnnnnn' 'sss'
            RemoteBranchTelephone, //'rrrr' 'nnnnnnnn' 'sss'
            LocalLitSmartPhone, //106'' 'nnnnnnnn' ''
            RemoteLitSmartPhone, //106'rrrr' 'nnnnnnnn' ''
            LocalMobilePhone, //'' '1nnnnnnnnn' ''
            RemoteMobilePhone, //'0' '1nnnnnnnnn' ''
            LocalSpecService, //'' 110 112 ... 10086 ''
            RemoteSpecService, //'rrrr' 110 112 ... 10086 ''
            LocalCalleePay, //'' 400 800 ''
            RemoteCalleePay, //'rrrr' 400 800 ''
            Other,
        };
    public:
        std::string const number() const
        {
            return number_;
        }
        void number(std::string const number)
        {
            number_ = number;
        }
	private:
		std::string number_;
	private:
		static std::string ipPrefix_;
		static std::string outlinePrefix_;
		static std::string virtualPrefix_;
	private:
		static std::vector<std::string> worldNotionalNumbers_; //所有国家编号
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
