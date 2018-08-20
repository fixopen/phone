#include "ATCommand.h"
#include "../Text/StringOp.h"

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

        DVTCODECResponse::DVTCODECResponse(std::string const& content) {
            size_t startPos = content.find("^DVTCODEC:", 0);
            startPos += 10;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                action = static_cast<Action>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
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
                }
                stopPos = content.find("\xD", startPos);
                if (stopPos != std::string::npos) {
                    shortVideoHeaderFlag = Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
            }
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
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                cliValidity = static_cast<Incoming::CLIValidity>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
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
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                subAddrType = static_cast<NumberType>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
        }

        DCPIResponse::DCPIResponse(std::string const& content) {
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
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                tch = static_cast<DCPIResponse::TCH>(Util::Text::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
        }

        CMTIndicator::CMTIndicator(std::string const& content) {
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
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                pdu = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
			if (length != pdu.length()) {
				//error.
			}
		}
    }

}
