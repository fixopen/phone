#include "Book.h"
#include "ATCommand.h"
#include "../Text/StringOp.h"

namespace Util {
    namespace AT {
        void Book::Update(std::string const & data) {
            if (data.find("+CREG") != std::string::npos) {
            } else {
                //other
            }
        }

        void Book::Lock(std::string const & password) {
            //AT+CLCK=\"SC\",1,\"%s\"\xD //lock sim with password
            std::string command = "AT+CLCK=\"SC\",1,\"" + password + "\"";
            Dispatcher::Instance()->Write(command);
        }

        void Book::Unlock(std::string const & password) {
            //AT+CLCK=\"SC\",2\xD //get sim lock status
            //AT+CLCK=\"SC\",0,\"%s\"\xD //unlock sim with password
            std::string command = "AT+CLCK=\"SC\",0,\"" + password + "\"";
            Dispatcher::Instance()->Write(command);
        }

        void Book::InputPIN(std::string const & pin) {
            //AT+CPIN=\"%s\"\xD //input pin
            std::string command = "AT+CPIN=\"" + pin + "\"";
            Dispatcher::Instance()->Write(command);
        }

        void Book::InputPIN2(std::string const & pin) {
            //AT+CPIN2=\"%s\"\xD //input pin2
            std::string command = "AT+CPIN2=\"" + pin + "\"";
            Dispatcher::Instance()->Write(command);
        }

        void Book::InputPUK(std::string const & puk) {
            //AT+CPIN=\"%s\",\"%s\"\xD //input puk, newpin
            //std::string command = "AT+CPIN=\"" + puk + "\",\"" + newpin + "\"";
            //Dispatcher::Instance()->Write(command);
        }

        void Book::InputPUK2(std::string const & puk) {
            //AT+CPIN2=\"%s\",\"%s\"\xD //input puk2, newpin
            //std::string command = "AT+CPIN2=\"" + puk + "\",\"" + newpin + "\"";
            //Dispatcher::Instance()->Write(command);
        }

        void Book::ChangePIN(std::string const & oldPin, std::string const & newPin) {
            //AT+CPWD=\"SC\",\"%s\",\"%s\"\xD //change pin to newpin
            std::string command = "AT+CPWD=\"SC\",\"" + oldPin + "\",\"" + newPin + "\"";
            Dispatcher::Instance()->Write(command);
        }

        void Book::ChangePIN2(std::string const & oldPin, std::string const & newPin) {
            //AT+PASSWORD=\"P2\",\"%s\",\"%s\"\xD //change pin2 to newpin2
            std::string command = "AT+PASSWORD=\"P2\",\"" + oldPin + "\",\"" + newPin + "\"";
            Dispatcher::Instance()->Write(command);
        }

        void Book::ChangePUK(std::string const & oldPin, std::string const & newPin) {
        }

        void Book::ChangePUK2(std::string const & oldPin, std::string const & newPin) {
        }

        void Book::GetStorageStatus() {
            //AT+CPBS=\"SM\"\xD //select sim storage
            //AT+CPBS?\xD //get sim entry count
            std::string command = "AT+CPBS=\"SM\"";
            Dispatcher::Instance()->Write(command);
            command = "AT+CPBS?";
            Dispatcher::Instance()->Write(command);
        }

        Book::Entry const Book::ReadEntry(size_t const index) const {
            //AT+CPBR=0,%d\xD //get sim entries
            Book::Entry result;
            return result;
        }

        std::vector<Book::Entry> const & Book::GetEntries() const {
            return entries_;
        }

        void Book::WriteEntry(size_t const index, std::string const & name, std::string const & number) {
            //AT+CPBW=%d,\"%s\",129,\"%s\"\xD //0, number, name
            std::string command = "AT+CPBW=" + Text::StringOp::ToUTF8(Text::StringOp::FromInt(index)) + ",\"" + number + "\",129,\"" + name + "\"";
            Dispatcher::Instance()->Write(command);
        }

        Book::Book() {
            Dispatcher::Instance()->RegisterObserver(this);
        }
    }
}
