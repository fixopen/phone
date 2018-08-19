#ifndef __CONTACT_DATA_TELEPHONENUMBER_H__
#define __CONTACT_DATA_TELEPHONENUMBER_H__

#include <string>
#include <vector>

namespace Contact_ {
    namespace Data {
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
            Type const type() const {
                return type_;
            }
            void type(Type const type) {
                type_ = type;
            }
            std::wstring const nationalNumber() const {
                return nationalNumber_;
            }
            void nationalNumber(std::wstring const& nationalNumber) {
                nationalNumber_ = nationalNumber;
            }
            std::wstring const regionNumber() const {
                return regionNumber_;
            }
            void regionNumber(std::wstring const regionNumber) {
                regionNumber_ = regionNumber;
            }
            std::wstring const number() const {
                return number_;
            }
            void number(std::wstring const number) {
                number_ = number;
            }
            std::wstring const subNumber() const {
                return subNumber_;
            }
            void subNumber(std::wstring const subNumber) {
                subNumber_ = subNumber;
            }
        private:
            Type type_;
            std::wstring nationalNumber_;
            std::wstring regionNumber_;
            std::wstring number_;
            std::wstring subNumber_;
        private:
            static std::wstring ipPrefix_;
            static std::wstring outlinePrefix_;
            static std::wstring virtualPrefix_;
        private:
            static std::vector<std::wstring> worldNotionalNumbers_; //所有国家编号
            static std::vector<std::wstring> chinaRegionNumbers_; //中国所有区号
        public:
            static std::wstring thisNationalNumber_;
            static std::wstring thisRegionNumber_;
            static TelephoneNumber ThisTelephoneNumber;
        public:
            TelephoneNumber(); //create
            TelephoneNumber(TelephoneNumber const& rhs);
            TelephoneNumber(std::wstring const& number, bool isWhole = false); //create form single string
            TelephoneNumber(std::wstring const& regionNumber, std::wstring const& number);
            TelephoneNumber(Type type, std::wstring const& nationalNumber, std::wstring const& regionNumber, std::wstring const& number, std::wstring const& subNumber);
            static void ConstructLocalNumber(TelephoneNumber& telephoneNumber, std::wstring const& number);
            static void ConstructRemoteNumber(TelephoneNumber& telephoneNumber, std::wstring const& regionNumber, std::wstring const& number);
            static void ConstructBranchNumber(TelephoneNumber& telephoneNumber, std::wstring const& subNumber);
            static void ConstructLocalBranchNumber(TelephoneNumber& telephoneNumber, std::wstring const& number, std::wstring const& subNumber);
            static void ConstructRemoteBranchNumber(TelephoneNumber& telephoneNumber, std::wstring const& regionNumber, std::wstring const& number, std::wstring const& subNumber);
            static void ConstructLocalLitSmartNumber(TelephoneNumber& telephoneNumber, std::wstring const& number);
            static void ConstructRemoteLitSmartNumber(TelephoneNumber& telephoneNumber, std::wstring const& regionNumber, std::wstring const& number);
            static void ConstructLocalMobileNumber(TelephoneNumber& telephoneNumber, std::wstring const& number);
            static void ConstructRemoteMobileNumber(TelephoneNumber& telephoneNumber, std::wstring const& number);
            ~TelephoneNumber(); //destroy
            bool const IsEquals(std::wstring const& number) const; //compare telephoneNumber is equal or not
            std::wstring const ToString() const; //convert to single string
        };
    }
}

#endif //__CONTACT_DATA_TELEPHONENUMBER_H__
