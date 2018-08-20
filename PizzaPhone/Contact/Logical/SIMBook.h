#ifndef __CONTACT_LOGICAL_SIMBOOK_H__
#define __CONTACT_LOGICAL_SIMBOOK_H__

#include <string>
#include <map>

namespace Contact {
    namespace Logical {
        class SIMBook {
        public:
            //IRA GSM UCS2 HEX PCCP PCDN 8859_1
            //CPBF CPBS CPBR CPBW
            //AT+COPS?\xD //get operator
            //AT+WOPN\xD
            //AT+AT+CPOL\xD
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
            std::pair<std::string, std::string> const ReadEntry(size_t const index);
            void WriteEntry(size_t const index, std::string const & name, std::string const & number);
        };
    }
}

#endif //__CONTACT_LOGICAL_SIMBOOK_H__
