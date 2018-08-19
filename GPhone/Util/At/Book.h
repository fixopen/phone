#ifndef __UTIL_AT_BOOK_H__
#define __UTIL_AT_BOOK_H__

#include "../Observer.h"
#include "ATCommand.h"
#include <string>

namespace Util {
    namespace AT {
        class Book : public Observer {
        public:
            struct Entry {
                size_t seqNo;
                std::string number;
                NumberType numberType;
                std::string name;
                std::string state;
            };
            static Book* const Instance() {
                Book* result = new Book();
                return result;
            }
            virtual void Update(std::string const & data);
            //IRA GSM UCS2 HEX PCCP PCDN 8859_1
            //CPBF CPBS CPBR CPBW
            //AT+COPS?\xD //get operator
            //AT+WOPN\xD
            //AT+CPOL\xD
            //AT+CSCS=\"GSM\"\xD //set charset to 7bit

            void Lock(std::string const & password);
            void Unlock(std::string const & password);
            void InputPIN(std::string const & pin = "");
            void InputPIN2(std::string const & pin = "");
            void InputPUK(std::string const & pin = "");
            void InputPUK2(std::string const & pin = "");
            void ChangePIN(std::string const & oldPin, std::string const & newPin);
            void ChangePIN2(std::string const & oldPin, std::string const & newPin);
            void ChangePUK(std::string const & oldPin, std::string const & newPin);
            void ChangePUK2(std::string const & oldPin, std::string const & newPin);
            void GetStorageStatus();
            Entry const ReadEntry(size_t const index) const;
            std::vector<Entry> const & GetEntries() const;
            void WriteEntry(size_t const index, std::string const & name, std::string const & number);
        private:
            Book();
        private:
            std::vector<Entry> entries_;
        };
    }
}

#endif //__UTIL_AT_BOOK_H__
