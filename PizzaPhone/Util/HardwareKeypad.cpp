#include "HardwareKeypad.h"
#include "Log.h"

namespace Util {
    HardwareKeypad::HardwareKeypad(unsigned int const portNo) {
        hardware_ = new RS232();
        hardware_->Open(portNo, 0);
        hardware_->SetReadFunc(disptch_);
    }

    HardwareKeypad::~HardwareKeypad() {
        hardware_->Close();
        delete hardware_;
    }

    void HardwareKeypad::disptch_(unsigned char const* const data, unsigned int const length) {
        //Log::Log(reinterpret_cast<char const* const>(data));
        std::string d(reinterpret_cast<char const*>(data), length);
        HardwareKeypad::Instance()->Notify(d);
    }
}
