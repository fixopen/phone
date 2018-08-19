#include "ATCommand.h"
#include "../Text/StringOp.h"
#include "../Log.h"

namespace Util {
    namespace AT {
        CREGResponse::CREGResponse(std::string const & content) {
            size_t startPos = content.find("+CREG:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                //full format
                state = static_cast<State>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    locationAreaCode = content.substr(startPos, stopPos - startPos);
                    startPos = stopPos + 1;
                    cellId = content.substr(startPos);
                }
            } else {
                  //only state format
                  state = static_cast<State>(Util::Text::StringOp::ToInt(content.substr(startPos)));
            }
        }

        CIEV::CIEV(std::string const & content) {
            size_t startPos = content.find("+CIEV:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                indication = static_cast<Indication>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
                value = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
            }
        }

        ServiceType const ParseServiceType(std::string const & typeString) {
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

        DVTCODECResponse::DVTCODECResponse(std::string const & content) {
            size_t startPos = content.find("^DVTCODEC:", 0);
            startPos += 10;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                action = static_cast<Action>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            } else {
                  action = static_cast<Action>(Util::Text::StringOp::ToInt(content.substr(startPos)));
            }
            if (action == aConsultation) {
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    decodeFormat = static_cast<Format>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    mp4PostFilter = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    encodeFormat = static_cast<Format>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    width = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    height = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    targetBitrate = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    vopTime = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    timeIncreamentResolution = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    timeIncSize = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    tstampclr = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    quantType = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 64; ++j) {
                        stopPos = content.find(",", startPos);
                        if (stopPos != std::string::npos) {
                            quantMatrix[i][j] = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                            startPos = stopPos + 1;
                        }
                    }
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    qFactor = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    IFrameInterval = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    acPredictFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    halfMoveFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    fourMoveFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    variableBitrateFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    resyncFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    dataPartFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    reverseVLCflag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                    shortVideoHeaderFlag = Util::Text::StringOp::ToInt(content.substr(startPos));
                }
            }
        }

        Incoming::Incoming(std::string const & content) {
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
                type = static_cast<NumberType>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddr = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddrType = static_cast<NumberType>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                alphaId = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
                cliValidity = static_cast<Incoming::CLIValidity>(Util::Text::StringOp::ToInt(content.substr(startPos)));
            }
        }

        DSCIResponse::DSCIResponse(std::string const & content) {
            //^DSCI:id,direction,state,mode,conference,"number",type,serviceType,cause,transactionId,"subaddr",subAddrType
            //^DSCI: 1,1,4,0,0,"13693607335",161,,,0,"",0
            //^DSCI: 1,1,6,0,0,"13693607335",161,,16,0,"",0
            //^DSCI: 1,1,4,0,0,"01062985517",161,,,0,"",0
            size_t startPos = content.find("^DSCI:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                id = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                direction = static_cast<Direction>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                state = static_cast<State>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                mode = static_cast<Mode>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                conference = static_cast<Conference>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                number = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<NumberType>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                serviceType = ParseServiceType(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                cause = static_cast<DSCIResponse::Cause>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                transactionId = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddr = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
                subAddrType = static_cast<NumberType>(Util::Text::StringOp::ToInt(content.substr(startPos)));
            }
        }

        DCPIResponse::DCPIResponse(std::string const & content) {
            //^DCPI:id,type,bandTone,TCH
            size_t startPos = content.find("^DCPI:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                id = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<DCPIResponse::Type>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                bandTone = static_cast<DCPIResponse::BandTone>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
                tch = static_cast<DCPIResponse::TCH>(Util::Text::StringOp::ToInt(content.substr(startPos)));
            }
        }

        CMTIndicator::CMTIndicator(std::string const & content) {
            size_t startPos = content.find("+CMT:", 0);
            startPos += 5;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                alpha = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD\xA", startPos);
            if (stopPos != std::string::npos) {
                length = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 2;
                pdu = content.substr(startPos);
                if (length != pdu.length()) {
                    //error.
                }
            }
        }

        DSQU::DSQU(std::string const & content) {
            size_t startPos = content.find("^DSQU:", 0);
            startPos += 6;
            value = Util::Text::StringOp::ToInt(content.substr(startPos));
        }

        DACTI::DACTI(std::string const & content) {
            size_t startPos = content.find("^DACTI:", 0);
            startPos += 7;
            mode = static_cast<Mode>(Util::Text::StringOp::ToInt(content.substr(startPos)));
        }

        void Dispatcher::Write(std::string const & data, std::string const & finallyResponse, RS232* const port) {
            for (; ;) {
                if (actionState_ == sIdle) {
                    RS232* p = serialPort_;
                    if (port) {
                        p = port;
                    }
                    //p->Write(reinterpret_cast<unsigned char const* const>(data.data()), data.length());
                    p->Write(data + "\xD");
                    finallyResponse_ = finallyResponse;
                    Log::Log(data);
                    actionState_ = sBusy;
                    if (finallyResponse == NullResponse()) {
                        actionState_ = sIdle;
                    }
                    break;
                } else {
                    ::Sleep(100);
                }
            }
        }

        void Dispatcher::disptch_(unsigned char const * const data, unsigned int const length) {
            Log::Log(reinterpret_cast<char const * const>(data));
            std::string d(reinterpret_cast<char const *>(data), length);
            Dispatcher::Instance()->Notify(d);
            if (Dispatcher::Instance()->finallyResponse_ == "") {
                //ok or error
                if (d.find("OK\xD\xA") != std::string::npos) {
                    //0
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else if (d.find("CONNECT") != std::string::npos) {
                    //1
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else if (d.find("RING") != std::string::npos) {
                    //2
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else if (d.find("NO CARRIER") != std::string::npos) {
                    //3
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else if (d.find("ERROR") != std::string::npos) {
                    //4
                    Dispatcher::Instance()->actionState_ = sIdle;
                    //ERROR:
                    //+CME ERROR:
                    //+CMS ERROR:
                    //+HTC ERROR:
                    //Error e(d);
                } else if (d.find("BUSY") != std::string::npos) {
                    //7
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else if (d.find("NO DIALTONE") != std::string::npos) {
                    //??
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else {
                    //do nothing, wait ok or error
                }
            } else {
                  //spec response
                if (d.find(Dispatcher::Instance()->finallyResponse_) != std::string::npos) {
                    Dispatcher::Instance()->actionState_ = sIdle;
                } else {
                    //do nothing, wait spec response
                }
            }
        }

        Dispatcher::Dispatcher(size_t const portNo) {
            serialPort_ = new Util::RS232();
            serialPort_->Open(portNo, 0);
            serialPort_->SetReadFunc(disptch_);
        }
    }
}
