#ifndef __UTIL_AT_REGISTER_H__
#define __UTIL_AT_REGISTER_H__

#include "../Observer.h"
#include <winbase.h> //for SYSTEMTIME
#include "ATCommand.h" //for DACTI

namespace Util {
    namespace AT {
        class Register : public Observer {
        public:
            enum State {
                sNull,
                sQueryVersion,
            };

            enum NetworkType {
                ntGsm,
                ntTds,
            };
            static Register* const Instance() {
                static Register* result = new Register();
                return result;
            }
            bool const IsRegistered() const {
                return isRegistered_;
            }

            int const SignalLevel() const {
                return signalLevel_;
            }

            int const SignalValue() const {
                return signalValue_;
            }

            DACTI::Mode const Network() const {
                return network_;
            }

            std::string const & Version() const {
                return version_;
            }

            int const UnreadedSmsCount() const {
                return unreadedSmsCount_;
            }

            bool const IsSmsFull() const {
                return isSmsFull_;
            }

            virtual void Update(std::string const & data);
            void Open();
            void Start();
            void Enable();
            void Disable();
            void Stop();
            void Close();
        private:
            Register();
            void test_();
            void reset3GModule_();
            void setPowerOn_();
            void controlEP_(int const number, bool const enable);
            void setGPIOWakeupMode_();
            void enableNetworkSignalIndicator_();
            void enableIncomingExtendIndicator_();
            void enableProgressIndicator_();
            void enableCallProgressIndication_();
            void enableRegisterStateReport_();
            void enableProtocolStackIndicator_();
            void enableAccessTechChangeIndication_();
            void enableSIMreport_();
            void enableUSIM_();
            void enableH324M_();
            void configH324MCodec_();
            void enableProtocolStack_();
            void enableCellCapabilityReport_();
            void forceNetwork_(NetworkType const networkType);
            void queryReversion_();
            void getCOPSlist_();
            void searchAndRegister_();
            void queryAccessTech_();
            void getAccessTech_();
            void unregister_();
            void disableProtocolStack_();
            void disableUSIM_();
            void softwarePowerOff_();
        private:
            bool isRegistered_;
            int signalLevel_;
            int signalValue_;
            DACTI::Mode network_;
            std::string version_;
            State state_;
            int unreadedSmsCount_;
            bool isSmsFull_;
        private: //@@pref monitor
            SYSTEMTIME startTime_;
        };
    }
}

#endif //__UTIL_AT_REGISTER_H__
