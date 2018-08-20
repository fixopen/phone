#include "SIMBook.h"

namespace Contact {
    namespace Logical {
        void SIMBook::Lock(std::string const & password) {
            //AT+CLCK=\"SC\",1,\"%s\"\xD //lock sim with password
        }

        void SIMBook::Unlock(std::string const & password) {
            //AT+CLCK=\"SC\",2\xD //get sim lock status
            //AT+CLCK=\"SC\",0,\"%s\"\xD //unlock sim with password
        }

        void SIMBook::InputPIN(std::string const & pin) {
            //AT+CPIN=\"%s\"\xD //input pin
            //std::string CPIN = "AT+CPIN=\"" + pin + "\""; //input the pin
            //Util::ATCommand::Instance()->Write(CPIN);
            //currentAction_ = aInputPIN;
        }

        void SIMBook::InputPIN2(std::string const & pin) {
            //AT+CPIN2=\"%s\"\xD //input pin2
        }

        void SIMBook::InputPUK(std::string const & pin) {
            //AT+CPIN=\"%s\",\"%s\"\xD //input puk, newpin
        }

        void SIMBook::InputPUK2(std::string const & pin) {
            //AT+CPIN2=\"%s\",\"%s\"\xD //input puk2, newpin
        }

        void SIMBook::ChangePIN(std::string const & oldPin, std::string const & newPin) {
            //AT+CPWD=\"SC\",\"%s\",\"%s\"\xD //change pin to newpin
        }

        void SIMBook::ChangePIN2(std::string const & oldPin, std::string const & newPin) {
            //AT+PASSWORD=\"P2\",\"%s\",\"%s\"\xD //change pin2 to newpin2
        }

        void SIMBook::ChangePUK(std::string const & oldPin, std::string const & newPin) {
        }

        void SIMBook::ChangePUK2(std::string const & oldPin, std::string const & newPin) {
        }

        void SIMBook::GetStorageStatus() {
            //AT+CPBS=\"SM\"\xD //select sim storage
            //AT+CPBS?\xD //get sim entry count
        }

        std::pair<std::string, std::string> const SIMBook::ReadEntry(size_t const index) {
            //AT+CPBR=0,%d\xD //get sim entries
            std::pair<std::string, std::string> result;
            return result;
        }

        void SIMBook::WriteEntry(size_t const index, std::string const & name, std::string const & number) {
            //AT+CPBW=%d,\"%s\",129,\"%s\"\xD //0, number, name
        }
    }
}
