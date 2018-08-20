#include "ATCommandWarp.h"
#include "Log.h"
#include "StringOp.h"

namespace Util {
    namespace AT {
        struct CFUN { //set device FUNctionaliity
            //AT+CFUN=funLevel[,reset]
            CFUN(std::string const& content);
            enum FunctionLevel {
                fMinimum = 0, //power off, deregisteration
                fFull = 1, //power on, registeration
                fFly = 4, //disable transmit and receice RF circuits
                fSimUsimActivate = 5,
                fSimUsimDeactivate = 6,
            };
            enum Reset {
                rImmediately = 0, //default
                rManual = 1,
            };
            FunctionLevel functionLevel;
            Reset reset;
        };

        struct COPS { //set operator selection mode
            //AT+COPS=[mode[,format[,operator[,accessTech]]]]
            COPS(std::string const& content);
            enum Mode {
                mAutomatic = 0, //operator ignored
                mManual = 1, //operator must has
                mManualDeregister = 2,
                mSetFormat4ReadCommandPlusCOPSQuest = 3,
                mIfManualFailSwitchAutomatic = 4,
            };
            enum Format {
                fLongAlphaNumeric = 0,
                fShortAlphaNumeric = 1,
                fNumeric = 2,
            };
            enum State {
                sUnknown = 0,
                sAvailable = 1,
                sCurrent = 2,
                sForbidden = 3,
            };
            enum AccessTech {
                aGSM = 0,
                aGSMCompact = 1,
                aUTRAN  = 2,
                aEGPRS = 3,
                aGPRSwithDTM = 16,
                aEGPRSwithDTM = 19,
            };
        };

        struct COPSResponse {
            //struct Item {
            //    Mode mode;
            //    Format format;
            //    State state;
            //    AccessTech accessTech;
            //};
            //+COPS:(2,"D2","XX","26202"),(3,"E-Plus","YY","26203"),(3,"TD1",,"26201"),(3,"Interkom","ZZ","26207"),,(0-4),(0,2)
            //OK
            COPSResponse(std::string const content) {
            }
            //std::vector<Item> Networks;
        };

        struct DSQ { //Signal Quality
            //AT^DSQ=[mode]
            DSQ(std::string const& content);
            enum Mode {
                mDisableIntermediateResult = 0,
                mEnableIntermediateResult = 1,
                mQueryCurrentSignalQuality = 2, //^DSQ:value //0-91
            };
            Mode mode;
        };

        struct CRC { //Cellular Result Code
            //AT+CRC=[mode]
            CRC(std::string const& content);
            enum Mode {
                mDisableExtend = 0, //RING
                mEnableExtend = 1, //+CRING
            };
            Mode mode;
        };

        ServiceType const ParseServiceType(std::string const& typeString) {
            ServiceType result = tError;
            if (typeString == "ASYNC") {
                result = tAsync;
            } else if (typeString == "SYNC") {
                result = tSync;
            } else if (typeString == "REL ASYNC") {
                result = tRelAsync;
            } else if (typeString == "REL SYNC") {
                result = tRelSync;
            } else if (typeString == "FAX") {
                result = tFax;
            } else if (typeString == "VOICE") {
                result = tVoice;
            } else if (typeString == "GPRS") {
                result = tGprs;
            } else {
                result = tError; //not support
            }
            return result;
        }

        std::string const ServiceTypeToString(ServiceType const serviceType) {
            std::string result;
            switch (serviceType) {
                case tVoice:
                    result = "VOICE";
                    break;
                case tFax:
                    result = "FAX";
                    break;
                case tAsync:
                    result = "ASYNC";
                    break;
                case tSync:
                    result = "SYNC";
                    break;
                case tRelAsync:
                    result = "REL ASYNC";
                    break;
                case tRelSync:
                    result = "REL SYNC";
                    break;
                case tGprs:
                    result = "GPRS";
                    break;
                default:
                    break;
            }
            return result;
        }

        Incoming::Incoming(std::string const& content) {
            //+CRING: type
            //+CRING: VOICE\xD\xA+CLIP:"number",type,"subaddr",subAddrType,"alphaId",CLIValidity\xD\xA
            size_t startPos = content.find(":", 0);
            ++startPos;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                number = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<NumberType>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddr = StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddrType = static_cast<NumberType>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                alphaId = StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                cliValidity = static_cast<Incoming::CLIValidity>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
        }

        DSCIResponse::DSCIResponse(std::string const& content) {
            //^DSCI:id,direction,state,mode,conference,"number",type,serviceType,cause,transactionId,"subaddr",subAddrType
            //^DSCI: 1,1,4,0,0,"13693607335",161,,,0,"",0
            //^DSCI: 1,1,6,0,0,"13693607335",161,,16,0,"",0
            //^DSCI: 1,1,4,0,0,"01062985517",161,,,0,"",0
            size_t startPos = content.find("^DSCI:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                id = StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                direction = static_cast<Direction>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                state = static_cast<State>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                mode = static_cast<Mode>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                conference = static_cast<Conference>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                number = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<NumberType>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                serviceType = ParseServiceType(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                cause = static_cast<DSCIResponse::Cause>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                transactionId = StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddr = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                subAddrType = static_cast<NumberType>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
        }

        DCPIResponse::DCPIResponse(std::string const& content) {
            //^DCPI:id,type,bandTone,TCH
            size_t startPos = content.find("^DCPI:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                id = StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<DCPIResponse::Type>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                bandTone = static_cast<DCPIResponse::BandTone>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                tch = static_cast<DCPIResponse::TCH>(StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
        }
    }

    void ATCommand::Write(std::string const& data, RS232* const port) {
        for (;;) {
            if (actionState_ == sIdle) {
                RS232* p = serialPort_;
                if (port) {
                    p = port;
                }
                p->Write(reinterpret_cast<unsigned char const* const>(data.data()), data.length());
                Util::Log::Log(data);
                actionState_ = sBusy;
                break;
            } else {
                ::Sleep(100);
            }
        }
    }

    void ATCommand::Start() {
        reset3GModule();
        controlEP(1, true);
        controlEP(2, true);
        controlEP(3, true);
        controlEP(4, true);
        setGPIOWakeupMode();
        enableNetworkSignalIndicator();
        enableIncomingExtendIndicator();
        enableProgressIndicator();
        enableCallProgressIndication();
        enableRegisterStateReport();
        enableProtocolStackIndicator();
        enableUSIM();
        setVideoChannel(3);
        enableH324M();
        configH324MCodec();
        enableProtocolStack();
        searchAndRegister();
    }

    void ATCommand::Stop() {
        unregister();
        disableProtocolStack();
        disableUSIM();
        softwarePowerOff();
    }

    ATCommand::ATCommand(unsigned int const port)
    : actionState_(sIdle)
    , currentAction_(aNull)
    , event_(eNull)
    , isRegister_(false)
    , serialPort_(0)
    , videoPort_(0) {
        initSerialPort_(port, 0);
    }

    ATCommand::~ATCommand() {
        if (videoPort_) {
            videoPort_->Close();
            delete videoPort_;
        }
        finallySerialPort_();
    }

    void ATCommand::reset3GModule() {
        std::string ATZ = "ATZ\xD"; //reset the at command device
        Write(ATZ);
        actionState_ = sIdle;
        currentAction_ = aReset;
	}

    void ATCommand::controlEP(int const number, bool const enable) {
        std::string DUSBDEG = "AT^DUSBDEG=" + StringOp::ToUTF8(StringOp::FromInt(number)) + "," + StringOp::ToUTF8(StringOp::FromInt(enable)) + "\xD"; //setup the serial port
        Write(DUSBDEG);
        currentAction_ = aSetupSerialPort;
	}

    void ATCommand::setGPIOWakeupMode() {
        std::string DGPIOSM = "AT^DGPIOSM=1\xD"; //set GPIO wakeup mode
        Write(DGPIOSM);
        currentAction_ = aSetupWakeupMode;
    }

    void ATCommand::enableNetworkSignalIndicator() {
        std::string DSQ = "AT^DSQ=1\xD"; //startup network signal indicator
        Write(DSQ);
        currentAction_ = aSetupNetworkSignalIndicator;
    }

    void ATCommand::enableIncomingExtendIndicator() {
        std::string CRC = "AT+CRC=1\xD"; //incoming call extend indicator
        Write(CRC);
        currentAction_ = aSetupIncomingExtendIndicator;
    }

    void ATCommand::enableProgressIndicator() {
        std::string DSCI = "AT^DSCI=1\xD"; //progress indicator
        Write(DSCI);
        currentAction_ = aSetupProgressIndicator;
    }

    void ATCommand::enableCallProgressIndication() {
        std::string DCPI = "AT^DCPI=1\xD"; //call progress indication
        Write(DCPI);
        currentAction_ = aSetupCallProgressIndication;
	}

    void ATCommand::enableRegisterStateReport() {
        std::string CREG = "AT+CREG=1\xD"; //setup network register state report
        Write(CREG);
        currentAction_ = aSetupNetworkRegisterStateReport;
    }

    void ATCommand::enableProtocolStackIndicator() {
        std::string CMER = "AT+CMER=2,0,0,2\xD"; //setup high level protocol stack event report : CIEV
        Write(CMER);
        currentAction_ = aSetupHighLevelProtocolEventReport;
	}

    void ATCommand::enableUSIM() {
        //std::string DUSIMU = "AT^DUSIMU=1\xD"; //setup USIM state report
        //Write(DUSIMU);
        std::string CFUN5 = "AT+CFUN=5\xD"; //active USIM
        Write(CFUN5);
        currentAction_ = aActiveUSIM;
    }

    void ATCommand::inputPIN(std::string const& pin) {
        std::string CPIN = "AT+CPIN=\"" + pin + "\"\xD"; //input the pin
        Write(CPIN);
        currentAction_ = aInputPIN;
	}

    namespace {
        void videoDataProcess(unsigned char const* const data, unsigned int const length) {
            //decode video data and show it
        }
    }

    void ATCommand::setVideoChannel(int const channelNumber) {
        videoPort_ = new Util::RS232();
        videoPort_->Open(channelNumber, 0, 0, 0, 0);
        videoPort_->SetReadFunc(videoDataProcess);
        std::string SELCH = "AT^DVTCHL=1" + StringOp::ToUTF8(StringOp::FromInt(channelNumber)) + "\xD"; //set video channel
        Write(SELCH, videoPort_);
        currentAction_ = aSetVideoChannel;
        actionState_ = sIdle;
    }

    void ATCommand::enableH324M() {
        //use mode[type]
        std::string INIT = "AT^DVTTYPE=3\xD"; //init h324m mode
        Write(INIT);
        currentAction_ = aEnableH324M;
    }

    void ATCommand::configH324MCodec() {
        //codec cap;
        std::string CC = "AT^DVTABLE=2,2,1\xD"; //config codec
        Write(CC);
        currentAction_ = aConfigH324MCodec;
    }

    void ATCommand::enableProtocolStack() {
        std::string CFUN1 = "AT+CFUN=1\xD"; //active all protocol stack
        Write(CFUN1);
        currentAction_ = aActiveFullProtocolStack;
    }

    void ATCommand::searchAndRegister() {
        std::string COPS = "AT+COPS=0\xD"; //search network and auto register
        Write(COPS);
        currentAction_ = aSearchNetworkAndAutoRegister;
	}

    /*std::vector<int> const*/void ATCommand::getAccessTech() {
        //std::vector<int> result;
        std::string COPS = "AT+COPS?\xD"; //get network type
        Write(COPS);
        currentAction_ = aGetAccessTech;
        //return result;
    }

    void ATCommand::unregister() {
        std::string COPS = "AT+COPS=2\xD"; //unregister from network
        Write(COPS);
        currentAction_ = aUnregisterFromNetwork;
	}

    void ATCommand::disableProtocolStack() {
        std::string CFUN0 = "AT+CFUN=0\xD"; //power off the protocol stack
        Write(CFUN0);
        currentAction_ = aDeactiveFullProtocolStack;
	}

    void ATCommand::disableUSIM() {
        std::string CFUN6 = "AT+CFUN=6\xD"; //deactive USIM
        Write(CFUN6);
        currentAction_ = aDeactiveUSIM;
	}

    void ATCommand::softwarePowerOff() {
        std::string DSOFF = "AT^DSOFF\xD"; //software power off
        Write(DSOFF);
        currentAction_ = aSoftwarePowerOff;
	}

    void ATCommand::disptch_(unsigned char const* const data, unsigned int const length) {
        Log::Log(reinterpret_cast<char const* const>(data));
        std::string d(reinterpret_cast<char const*>(data), length);
        ATCommand::Instance()->Notify(d);
        //COPSResponse* d = 0;
        switch (ATCommand::Instance()->CurrentAction()) {
            case ATCommand::aSearchNetworkAndAutoRegister:
                //OK
                //^DACTI: 2
                //+CREG: 2
                //+CREG: 1,"0012",""0044"
                //+CIEV: 2,3
                //+CIEV: 8,0
                if (d.find("+CREG: 1") != std::string::npos) { //or 5 is roam
                    ATCommand::Instance()->ActionEnd();
                    ATCommand::Instance()->ClearAction();
                }
                break;
            case ATCommand::aGetAccessTech:
                //d = new COPSResponse(d);
                //for (size_t i = 0; i < d->Networks.size(); ++i) {
                //}
                break;
            case ATCommand::aActiveUSIM:
                //^DSUIMU: 1
                //+CME ERROR: 11
                //or
                //OK
                if (d.find("^DSUIMU: 1") != std::string::npos) {
                    //continue
                }
                if (d.find("+CME ERROR: 11") != std::string::npos) {
                    //need pin
                    ATCommand::Instance()->ActionEnd();
                    ATCommand::Instance()->inputPIN();
                    ATCommand::Instance()->ClearAction();
                } else if (d.find("OK") != std::string::npos) {
                    //not need pin
                    ATCommand::Instance()->ActionEnd();
                    ATCommand::Instance()->ClearAction();
                }
                break;
            case ATCommand::aUnregisterFromNetwork:
                //+CREG: 0
                //OK
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aInputPIN:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aReset:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupSerialPort:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupWakeupMode:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupNetworkSignalIndicator:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupIncomingExtendIndicator:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupProgressIndicator:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupCallProgressIndication:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupNetworkRegisterStateReport:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetupHighLevelProtocolEventReport:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSetVideoChannel:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aEnableH324M:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aConfigH324MCodec:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aActiveFullProtocolStack:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aDeactiveFullProtocolStack:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aDeactiveUSIM:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            case ATCommand::aSoftwarePowerOff:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
            default:
                ATCommand::Instance()->CommonResponseProcess(d);
                break;
        }
    }

    void ATCommand::CommonResponseProcess(std::string const& data) {
        if (data.find("OK") != std::string::npos) {
            actionState_ = sIdle;
            currentAction_ = aNull;
        } else if (data.find("ERROR") != std::string::npos) {
            actionState_ = sIdle;
            errorProcess_();
            currentAction_ = aNull;
        } else {
            actionState_ = sIdle;
            //currentAction_ = aNull;
        }
    }

    void ATCommand::errorProcess_() {
    }

    void ATCommand::initSerialPort_(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits) {
        serialPort_ = new Util::RS232();
        serialPort_->Open(portNo, baud, parity, databits, stopbits);
        serialPort_->SetReadFunc(disptch_);
    }

    void ATCommand::finallySerialPort_() {
        serialPort_->Close();
        delete serialPort_;
    }
}
