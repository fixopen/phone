#ifndef __DATA_TELEPHONENUMBER_H__
#define __DATA_TELEPHONENUMBER_H__

#pragma warning(disable: 4786)
#include <string>
#include <vector>

namespace Data
{
    class TelephoneNumber {
    public:
        enum Type {
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
        /*
        Type const type() const
        {
        return type_;
        }
        void type(Type const type)
        {
        type_ = type;
        }
        std::string const nationalNumber() const
        {
        return nationalNumber_;
        }
        void nationalNumber(std::string const nationalNumber)
        {
        nationalNumber_ = nationalNumber;
        }
        std::string const regionNumber() const
        {
        return regionNumber_;
        }
        void regionNumber(std::string const regionNumber)
        {
        regionNumber_ = regionNumber;
        }
        */
        std::string const number() const {
            return number_;
        }
        void number(std::string const number) {
            number_ = number;
        }
        /*
        std::string const subNumber() const
        {
        return subNumber_;
        }
        void subNumber(std::string const subNumber)
        {
        subNumber_ = subNumber;
        }
        */
    private:
        //Type type_;
        //std::string nationalNumber_;
        //std::string regionNumber_;
        std::string number_;
        //std::string subNumber_;
    private:
        static std::string ipPrefix_;
        static std::string outlinePrefix_;
        static std::string virtualPrefix_;
    private:
        static std::vector<std::string> worldNotionalNumbers_; //所有国家编号
        static std::vector<std::string> chinaRegionNumbers_; //中国所有区号
    public:
        //static std::string thisNationalNumber_;
        //static std::string thisRegionNumber_;
        static TelephoneNumber ThisTelephoneNumber;
    public:
        TelephoneNumber(); //create
        TelephoneNumber(std::string number, bool isWhole = false); //create form single string
        /*
        TelephoneNumber(std::string regionNumber, std::string number);
        TelephoneNumber(Type type, std::string nationalNumber, std::string regionNumber, std::string number, std::string subNumber);
        static void ConstructLocalNumber(TelephoneNumber& telephoneNumber, std::string number);
        static void ConstructRemoteNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number);
        static void ConstructBranchNumber(TelephoneNumber& telephoneNumber, std::string subNumber);
        static void ConstructLocalBranchNumber(TelephoneNumber& telephoneNumber, std::string number, std::string subNumber);
        static void ConstructRemoteBranchNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number, std::string subNumber);
        static void ConstructLocalLitSmartNumber(TelephoneNumber& telephoneNumber, std::string number);
        static void ConstructRemoteLitSmartNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number);
        static void ConstructLocalMobileNumber(TelephoneNumber& telephoneNumber, std::string number);
        static void ConstructRemoteMobileNumber(TelephoneNumber& telephoneNumber, std::string number);
        static void ConstructLocalNumber(TelephoneNumber& telephoneNumber, std::string number);
        static void ConstructRemoteNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number);
        static void ConstructLocalNumber(TelephoneNumber& telephoneNumber, std::string number);
        static void ConstructRemoteNumber(TelephoneNumber& telephoneNumber, std::string regionNumber, std::string number);
        */
        ~TelephoneNumber(); //destroy
        bool const IsEquals(std::string const number) const; //compare telephoneNumber is equal or not
        std::string const ToString() const; //convert to single string
    };
}

#endif //__DATA_TELEPHONENUMBER_H__
