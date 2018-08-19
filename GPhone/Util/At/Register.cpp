#include "Register.h"

#include "ATCommand.h"
#include "../Text/StringOp.h"

namespace Util {
    namespace AT {
        void Register::Start() {
            //"ATE0\r"; //ECHO OFF  
            //"AT+CMGF=0\r"; //PDU模式
            //"AT+CSCS=\"UCS2\"\r";
            //"AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
            //"AT+CNMI=2,1,0,0,0\r";
            //"ATV1\r"; //1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR> "ATV0\r";返回“0\r”（ATV指令决定返回result code是数字格式还是字符格式，ATV0 返回为数字格式，其中0 表示OK，1 表示CONNECT，2 表示RING，3 表示NO CARRIER，4 表示ERROR
            //"AT^DSLP=0,0\r";
            //"AT^DAUDSCS?\r";
            //startTime_ = ::GetTickCount();
            ::GetSystemTime(&startTime_);
            test_();
            reset3GModule_();
            //setPowerOn_();
            controlEP_(1, true);
            controlEP_(2, true);
            controlEP_(3, true);
            controlEP_(4, true);
            //controlEP_(5, true);
            //controlEP_(6, true);
            setGPIOWakeupMode_();
            enableNetworkSignalIndicator_();
            enableIncomingExtendIndicator_();
            enableProgressIndicator_();
            enableCallProgressIndication_();
            enableRegisterStateReport_();
            enableProtocolStackIndicator_();
            enableAccessTechChangeIndication_();
            enableSIMreport_();
            enableUSIM_();
            configH324MCodec_();
            enableH324M_();
            enableProtocolStack_();
            enableCellCapabilityReport_();
            queryReversion_();
            //getCOPSlist_();
            forceNetwork_(ntTds);
            searchAndRegister_();
            queryAccessTech_();
        }

        void Register::Stop() {
            unregister_();
            disableProtocolStack_();
            disableUSIM_();
            softwarePowerOff_();
        }

        void Register::Update(std::string const & data) {
            if (data.find("+CREG") != std::string::npos) {
                CREGResponse r(data);
                switch (r.state) {
                case CREGResponse::sUnregisterNotSearch:
                    isRegistered_ = false;
                    break;
                case CREGResponse::wRegisterHome:
                    isRegistered_ = true;
                    break;
                case CREGResponse::sUnregisterAndSearch:
                    isRegistered_ = false;
                    break;
                case CREGResponse::sRegisterDenied:
                    isRegistered_ = false;
                    break;
                case CREGResponse::sUnknown:
                    isRegistered_ = false;
                    break;
                case CREGResponse::sRegisterRoaming:
                    isRegistered_ = true;
                    break;
                default:
                    break;
                }
            } else if (data.find("+CIEV") != std::string::npos) {
                CIEV e(data);
                switch (e.indication) {
                case CIEV::iBatteryChargeLevel:
                    break;
                case CIEV::iSignalQulity:
                    signalLevel_ = e.value;
                    break;
                case CIEV::iServiceLevel:
                    break;
                case CIEV::iSounderActivity:
                    break;
                case CIEV::iUnreadedShortMessage:
                    unreadedSmsCount_ = e.value;
                    break;
                case CIEV::iCallInProgress:
                    break;
                case CIEV::iRoamingStatus:
                    break;
                case CIEV::iSMSStorageFull:
                    isSmsFull_ = !!e.value;
                    break;
                default:
                    break;
                }
            } else if (data.find("^DSQU") != std::string::npos) {
                DSQU s(data);
                signalValue_ = s.value;
            } else if (data.find("^DACTI") != std::string::npos) {
                DACTI at(data);
                network_ = at.mode;
            } else if (data.find("+CGMR") != std::string::npos) {
                size_t pos = data.find("+CGMR:");
                version_ = data.substr(pos + 6);
            } else {
                //other
                switch (state_) {
                case sQueryVersion:
                    version_ = data;
                    state_ = sNull;
                    break;
                default:
                    break;
                }
            }
        }

        Register::Register()
        : isRegistered_(false)
        , signalLevel_(0)
        , signalValue_(0)
        , network_(DACTI::mUnknown) {
            Dispatcher::Instance()->RegisterObserver(this);
        }

        void Register::test_() {
            std::string command = "AT";
            Dispatcher::Instance()->Write(command);
        }

        void Register::reset3GModule_() {
            std::string command = "ATZ"; //reset the at command device
            Dispatcher::Instance()->Write(command);
        }

        void Register::setPowerOn_() {
            std::string command = "AT^DUSBPOWER=1"; //reset the at command device
            Dispatcher::Instance()->Write(command);
        }

        void Register::controlEP_(int const number, bool const enable) {
            std::string command = "AT^DUSBDEG=" + Text::StringOp::ToUTF8(Text::StringOp::FromInt(number)) + "," + Text::StringOp::ToUTF8(Text::StringOp::FromInt(enable)) + ""; //setup the serial port
            Dispatcher::Instance()->Write(command);
        }

        void Register::setGPIOWakeupMode_() {
            std::string command = "AT^DGPIOSM=1"; //set GPIO wakeup mode
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableNetworkSignalIndicator_() {
            std::string command = "AT^DSQ=1"; //startup network signal indicator
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableIncomingExtendIndicator_() {
            std::string command = "AT+CRC=1"; //incoming call extend indicator
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableProgressIndicator_() {
            std::string command = "AT^DSCI=1"; //progress indicator
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableCallProgressIndication_() {
            std::string command = "AT^DCPI=1"; //call progress indication
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableRegisterStateReport_() {
            std::string command = "AT+CREG=1"; //setup network register state report
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableProtocolStackIndicator_() {
            std::string command = "AT+CMER=2,0,0,2"; //setup high level protocol stack event report : CIEV
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableAccessTechChangeIndication_() {
            std::string command = "AT^DACTI=1"; //setup Access Tech Change Indication : ^DACTI
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableSIMreport_() {
            std::string command = "AT^DUSIMR=1"; //setup SIM state report : ^DUSIMR
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableUSIM_() {
            std::string command = "AT+CFUN=5"; //active USIM
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableH324M_() {
            std::string command = "AT^DVTTYPE=3"; //init h324m mode
            Dispatcher::Instance()->Write(command);
        }

        void Register::configH324MCodec_() {
            //std::string command = "AT^DVTABLE=2,2,1"; //config codec for mp4
            std::string command = "AT^DVTABLE=1,1,1"; //config codec for h263
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableProtocolStack_() {
            std::string command = "AT+CFUN=1"; //active all protocol stack
            Dispatcher::Instance()->Write(command);
        }

        void Register::enableCellCapabilityReport_() {
            std::string command = "AT^DCINFO=1"; //enable cell capability report
            Dispatcher::Instance()->Write(command);
        }

        void Register::forceNetwork_(NetworkType const networkType) {
            std::string accessTech;
            switch (networkType) {
            case ntGsm:
                //accessTech = "AT^DTMS=0";
                //accessTech = "AT^DSTM=1,1,0";
                accessTech = "AT^DDTM=1,0";
                break;
            case ntTds:
                //accessTech = "AT^DTMS=2";
                //accessTech = "AT^DSTM=1,1,2";
                accessTech = "AT^DDTM=1,2";
                break;
            default:
                break;
            }
            Dispatcher::Instance()->Write(accessTech);
        }

        void Register::queryReversion_() {
            std::string command = "AT+CGMR"; //get network list
            Dispatcher::Instance()->Write(command);
            state_ = sQueryVersion;
        }

        void Register::getCOPSlist_() {
            std::string command = "AT+COPS=?"; //get network list
            Dispatcher::Instance()->Write(command);
        }

        void Register::searchAndRegister_() {
            //"AT+COPS=1,0,\"UTRAN\",2"; //search network and auto register
            std::string command = "AT+COPS=0"; //search network and auto register
            Dispatcher::Instance()->Write(command);
        }

        void Register::queryAccessTech_() {
            std::string command = "AT^DSTM?"; //get network type
            Dispatcher::Instance()->Write(command);
        }

        void Register::getAccessTech_() {
            std::string command = "AT+COPS?"; //get network type
            Dispatcher::Instance()->Write(command);
        }

        void Register::unregister_() {
            std::string command = "AT+COPS=2"; //unregister from network
            Dispatcher::Instance()->Write(command);
        }

        void Register::disableProtocolStack_() {
            std::string command = "AT+CFUN=0"; //power off the protocol stack
            Dispatcher::Instance()->Write(command);
        }

        void Register::disableUSIM_() {
            std::string command = "AT+CFUN=6"; //deactive USIM
            Dispatcher::Instance()->Write(command);
        }

        void Register::softwarePowerOff_() {
            std::string command = "AT^DSOFF"; //software power off
            Dispatcher::Instance()->Write(command);
        }
    }
}
