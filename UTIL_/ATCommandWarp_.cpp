#include "../Util/StringOp.h"
#include "ATCommandWarp_.h"
//#include "Log.h"
#include "Reg.h"

//#include "../GPhone.h"
#include "../MultimediaPhoneDlg.h"

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
                action = static_cast<Action>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            if (action == aConsultation) {
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    decodeFormat = static_cast<Format>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    mp4PostFilter = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    encodeFormat = static_cast<Format>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    width = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    height = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    targetBitrate = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    vopTime = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    timeIncreamentResolution = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    timeIncSize = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    quantType = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 64; ++j) {
                        stopPos = content.find(",", startPos);
                        if (stopPos != std::string::npos) {
                            quantMatrix[i][j] = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                            startPos = stopPos + 1;
                        }
                    }
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    qFactor = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    IFrameInterval = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    acPredictFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    halfMoveFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    fourMoveFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    variableBitrateFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    resyncFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    dataPartFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find(",", startPos);
                if (stopPos != std::string::npos) {
                    reverseVLCflag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                    startPos = stopPos + 1;
                }
                stopPos = content.find("\xD", startPos);
                if (stopPos != std::string::npos) {
                    shortVideoHeaderFlag = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
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
                type = static_cast<NumberType>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddr = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddrType = static_cast<NumberType>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                alphaId = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                cliValidity = static_cast<Incoming::CLIValidity>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
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
                id = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                direction = static_cast<Direction>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                state = static_cast<State>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                mode = static_cast<Mode>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                conference = static_cast<Conference>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                number = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<NumberType>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                serviceType = ParseServiceType(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                cause = static_cast<DSCIResponse::Cause>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                transactionId = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                subAddr = content.substr(startPos, stopPos - startPos);
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                subAddrType = static_cast<NumberType>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
        }

        DCPIResponse::DCPIResponse(std::string const& content) {
            //^DCPI:id,type,bandTone,TCH
            size_t startPos = content.find("^DCPI:", 0);
            startPos += 6;
            size_t stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                id = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                type = static_cast<DCPIResponse::Type>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find(",", startPos);
            if (stopPos != std::string::npos) {
                bandTone = static_cast<DCPIResponse::BandTone>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
                startPos = stopPos + 1;
            }
            stopPos = content.find("\xD", startPos);
            if (stopPos != std::string::npos) {
                tch = static_cast<DCPIResponse::TCH>(Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos)));
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
                length = Util::StringOp::ToInt(content.substr(startPos, stopPos - startPos));
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

    void ATCommand::Write(std::string const& data,std::string const& finallyResponse, RS232* const port) {
        
		DWORD Dstart = GetTickCount();
		for (;;) {
            if (actionState_ == sIdle) {
                RS232* p = serialPort_;
                if (port) {
                    p = port;
                }				
			//	Dprintf("Write:" );
			//	Dprintf(data.c_str());
			//	Dprintf("\r\n");
                p->WritePort(reinterpret_cast<unsigned char const* const>(data.data()), data.length());
				finallyResponse_ = finallyResponse;
				//Log::Log(data);
                actionState_ = sBusy;
                break;
            } else {
			//	Dprintf("Write BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\r\n");
				if (finallyResponse == "")
				{
					if ( (GetTickCount() - Dstart) > 1000)//超过一秒后就可以写
					{	
			//			Dprintf("Write CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\r\n");
						actionState_ = sIdle	;
					}
				}
                ::Sleep(100);

            }

        }
    }

    void ATCommand::Start() {
        //"AT\r"; //测试GSM-MODEM的存在性
        //"ATE0\r"; //ECHO OFF  
        //"AT+CGMR\r";
        //"AT+CMGF=0\r"; //PDU模式
        //"AT+CSCS=\"UCS2\"\r";
        //"AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
        //"AT+CNMI=2,1,0,0,0\r";
        //"ATV1\r"; //1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\r";返回“0\r”（ATV指令决定返回result code是数字格式还是字符格式，ATV0 返回为数字格式，其中0 表示OK，1 表示CONNECT，2 表示RING，3 表示NO CARRIER，4 表示ERROR
        //"AT^DSLP=0,0\r";
        //"AT^DAUDSCS?\r";
		//startTime_ = ::GetTickCount();
		::GetSystemTime(&startTime_);
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
		enableAccessTechChangeIndication();
        enableUSIM();
        enableH324M();
        configH324MCodec();
        enableProtocolStack();
		//forceNetwork(ntTds);
		//getCOPSlist();
        searchAndRegister();
    }

    struct RegItem {
        RegItem(std::wstring const& key, std::wstring const& v) {
            name = key;
            value = v;
        }
        std::wstring name;
        std::wstring value;
    };

    struct DialInfo {
        DialInfo(std::wstring const& secondValue, std::wstring const& thirdValue)
            : second(L"2", secondValue)
            , third(L"3", thirdValue) {
        }
        RegItem second;
        RegItem third;
    };

    struct APN {
        APN(std::wstring const& secondValue, std::wstring const& thirdValue, std::wstring const& number, std::wstring const& proxyValue, std::wstring const& httpValue, std::wstring const& username = L"", std::wstring const& passwd = L"")
            : dialInfo(secondValue, thirdValue)
            , telephoneNumber(number)
            , userName(username)
            , password(passwd)
            , proxy(proxyValue)
            , http(httpValue) {
        }
        DialInfo dialInfo;
        std::wstring telephoneNumber;
        std::wstring userName;
        std::wstring password;
        std::wstring proxy;
        std::wstring http;
    };

    APN ApnList[] = {
        APN(L"AT+CGDCONT=1,\"IP\",\"CMWAP\",\"\",0,0<cr>", L"AT+CGQREQ=1<cr>", L"*98*1#", L"ip:port", L""),
        APN(L"AT+CGDCONT=1,\"IP\",\"CMNET\",\"\",0,0<cr>", L"AT+CGEQREQ=1,2,128,2048,0,0,0,0,\"0E0\",\"0E0\",,0,0<cr>", L"*98*1#", L"ip:port", L""),
        APN(L"AT+CGDCONT=1,\"IP\",\"labwap3\",\"\",0,0<cr>", L"AT+CGEQREQ=1,2,128,2048,0,0,0,0,\"0E0\",\"0E0\",,0,0<cr>", L"*98*1#", L"ip:port", L"")
    };

    APNType ApnType;

    std::wstring dialInfos[] = {
        L"打开端口",
        L"端口已打开",
        L"连接设备",
        L"设备已连接",
        L"所有设备已连接",
        L"验证",
        L"验证通知",
        L"验证重试",
        L"验证回调",
        L"验证密码已改变",
        L"验证方案",
        L"验证连接速率",
        L"验证回应",
        L"重新验证",
        L"验证已通过",
        L"准备回应",
        L"等待modem复位",
        L"等待回应",
        L"方案",
        L"暂停",
        L"重新验证",
        L"呼叫等待回应",
        L"密码过期",
        L"已连接",
        L"连接断开",
        L"未知错误"
    };

    std::pair<int, std::wstring> const ATCommand::OnRasStateChanged(WPARAM w, LPARAM l) {
        int state = w;
        if (state >= RASCS_Interactive && state <= RASCS_PasswordExpired) {
            state = state - RASCS_Interactive + 19;
        } else if (state >= RASCS_Connected && state <= RASCS_Disconnected) {
            state = state - RASCS_Connected + 23;
        }
        if (state > 25) {
            state = 25;
        }
        //if (state == 23) { //connected
        //} else if (state == 24) { //unconnected
        //}
        return std::make_pair(state, dialInfos[state]);
    }

    //0xCCCD
    //std::wstring const NotifyWindow::OnRasErrorCode(WPARAM w, LPARAM l) {
    //    int result = w;
    //    if (result >= RASCS_Interactive && result <= RASCS_PasswordExpired) {
    //        result = result - RASCS_Interactive + 19;
    //    } else if (result >= RASCS_Connected && result <= RASCS_Disconnected) {
    //        result = result - RASCS_Connected + 23;
    //    }
    //    if (result > 25) {
    //        result = 25;
    //    }
    //    if (result == 23) { //connected
    //    } else if (result == 24) { //unconnected
    //    }
    //    return dialInfos[result];

    //    RASCONNSTATUS result;
    //    DWORD failure = ::RasGetConnectStatus(rasConn_, &result);
    //    int statusCode = result.rasconnstate;
    //    if (result.rasconnstate >= RASCS_Interactive && result.rasconnstate <= RASCS_PasswordExpired) {
    //        statusCode = result.rasconnstate - RASCS_Interactive + 19;
    //    } else if (result.rasconnstate >= RASCS_Connected && result.rasconnstate <= RASCS_Disconnected) {
    //        statusCode = result.rasconnstate - RASCS_Connected + 23;
    //    }
    //    if (RASCS_Connected == result.rasconnstate) {
    //    }
    //}

    DWORD const ATCommand::InitDial(APNType const apnType) {
        ApnType = apnType;
        Reg r(HKEY_CURRENT_USER, L"ControlPanel\\Dial");
        r.Write(L"CurrentLoc", 1);
        Reg reg(HKEY_LOCAL_MACHINE, L"ExtModems\\3GMODEM\\Init");
        reg.Write(ApnList[apnType].dialInfo.second.name.c_str(), ApnList[apnType].dialInfo.second.value.c_str());
        reg.Write(ApnList[apnType].dialInfo.third.name.c_str(), ApnList[apnType].dialInfo.third.value.c_str());
        RASENTRY entry;
        memset(&entry, 0, sizeof(RASENTRY));
        entry.dwSize = sizeof(RASENTRY);
        //entry.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_ModemLights | RASEO_IpHeaderCompression | RASEO_SwCompression | RASEO_NetworkLogon;
        //entry.dwfOptions = 0x38010210;
        entry.dwfOptions = 0x400208;
        entry.dwfNetProtocols = RASNP_Ip /*| RASNP_NetBEUI | RASNP_Ipx*/;
        entry.dwFramingProtocol = RASFP_Ppp;
        wsprintf(entry.szDeviceType, L"modem");
        wsprintf(entry.szDeviceName, L"Serial Cable on VCOM3:(PS)");
        wsprintf(entry.szLocalPhoneNumber, ApnList[apnType].telephoneNumber.c_str());
        entry.dwCountryID = 0;
        entry.dwCountryCode = 86;
        wsprintf(entry.szAreaCode, L"10");
        return ::RasSetEntryProperties(NULL, // pointer to full path and filename of phone-book file
            L"890", // pointer to an entry name
            &entry, // buffer that contains entry information
            sizeof(RASENTRY), //size, in bytes, of the lpRasEntry buffer
            NULL, //buffer that contains device-specific 
            0);	
    }

    DWORD const ATCommand::Dial(HWND const notifyWindow) {
        RASDIALPARAMS dialParams;
        memset(&dialParams, 0, sizeof(dialParams));
        // Configure the RASDIALPARAMS structure. 
        dialParams.dwSize = sizeof (RASDIALPARAMS);
        wsprintf(dialParams.szPhoneNumber, ApnList[ApnType].telephoneNumber.c_str());
        wsprintf(dialParams.szCallbackNumber, L"");
        wsprintf(dialParams.szDomain, L"");
        wsprintf(dialParams.szEntryName, L"890");
        //strcpy(dialParams.szUserName, "100001644588");
        //strcpy(dialParams.szPassword, "m2y8g4k9");
        wsprintf(dialParams.szUserName, ApnList[ApnType].userName.c_str());
        wsprintf(dialParams.szPassword, ApnList[ApnType].password.c_str());
        wsprintf(dialParams.szDomain, L"");
        return ::RasDial(NULL, NULL, &dialParams, 0xFFFFFFFF, notifyWindow, &rasConn_);
    }

    std::wstring const ATCommand::GetDynamicIP() const {
        Reg reg(HKEY_LOCAL_MACHINE, L"Comm\\Serial Cable on VCOM3:(PS)\\Parms\\TcpIp");
        wchar_t ipAddress[256] = {0};
        reg.Read(L"IpAddress", ipAddress);
        return ipAddress;
    }

    bool const ATCommand::Hangup() {
        bool result = false;
        if (::RasHangUp(rasConn_) == 0) {
            result = true;

        }
        ::Sleep(2000);
        return result;
    }

    void ATCommand::Stop() {
        unregister();
        disableProtocolStack();
        disableUSIM();
        softwarePowerOff();
    }

    ATCommand::ATCommand(unsigned int const port)
        : rasConn_(0)
        , actionState_(sIdle)
        , currentAction_(aNull)
        , event_(eNull)
        , isRegister_(false)
        , serialPort_(0) {
            initSerialPort_(port, 0);
    }

    ATCommand::~ATCommand() {
        finallySerialPort_();
    }

    void ATCommand::reset3GModule() {
        std::string ATZ = "ATZ\xD"; //reset the at command device
        Write(ATZ);
        actionState_ = sIdle;
        currentAction_ = aReset;
    }

    void ATCommand::controlEP(int const number, bool const enable) {
        std::string DUSBDEG = "AT^DUSBDEG=" + Util::StringOp::ToUTF8(Util::StringOp::FromInt(number))
            + "," + Util::StringOp::ToUTF8(Util::StringOp::FromInt(enable)) + "\xD"; //setup the serial port
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

	void ATCommand::enableAccessTechChangeIndication() {
        std::string accessTech = "AT^DACTI=1\xD"; //setup Access Tech Change Indication : ^DACTI
        Write(accessTech);
        currentAction_ = aSetupAccessTechChangeIndication;
	}

    void ATCommand::enableUSIM() {
        //std::string DUSIMU = "AT^DUSIMU=1\xD"; //setup USIM state report
        //Write(DUSIMU);
        std::string CFUN5 = "AT+CFUN=5\xD"; //active USIM
        Write(CFUN5);
        currentAction_ = aActiveUSIM;
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

	void ATCommand::forceNetwork(NetworkType const networkType) {
		std::string accessTech;
		switch (networkType) {
			case ntGsm:
				//accessTech = "AT^DTMS=0";
				accessTech = "AT^DSTM=1,1,0";
				break;
			case ntTds:
				//accessTech = "AT^DTMS=2";
				accessTech = "AT^DSTM=1,1,2";
				break;
			default:
				break;
		}
        Write(accessTech);
        currentAction_ = aSwitchNetwork;
	}
	void ATCommand::getCOPSlist() {
        std::string COPS = "AT+COPS=?\xD"; //search network and auto register
        Write(COPS);
        currentAction_ = aSearchNetworkAndAutoRegister;
	}

    void ATCommand::searchAndRegister() {
        //std::string COPS = "AT+COPS=1,0,\"UTRAN\",2\xD"; //search network and auto register
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
//      Log::Log(reinterpret_cast<char const* const>(data));
       std::string d(reinterpret_cast<char const*>(data), length);
//	   Dprintf("Read:");
//	   Dprintf(d.c_str());
//	   Dprintf("\r\n");
	   ATCommand::Instance()->Notify(d);
	   
	   if (ATCommand::Instance()->finallyResponse_ == "") { //ok or error
		   if (d.find("OK\xD\xA") != std::string::npos) {
			   ATCommand::Instance()->actionState_ = sIdle;
		   } else if (d.find("ERROR\xD\xA") != std::string::npos) {
			   ATCommand::Instance()->actionState_ = sIdle;
			   //Error e(d);
			   //process e;
		   }
	   } else { //spec response
		   if (d.find(ATCommand::Instance()->finallyResponse_) != std::string::npos) {
			   ATCommand::Instance()->actionState_ = sIdle;
		   }
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
        //^DSQU //signal
        //+CSQ
        //+CGMR
        //^DAUDSCS
        //^DUSIMU
        //^DPROFI
        //+CREG
        //^DCALLPI
        //^DCINFO
        //+HTCERROR
        //+CIEV
        //^DACTI
        //+CGEV
        //+PCD
        //+CPMS
        //+CSCA
		//^HTCDIS: 10
		//+CGEV
    }

    void ATCommand::errorProcess_() {
    }

    void ATCommand::initSerialPort_(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits) {     
		serialPort_ = new Util::RS232();
        serialPort_->OpenPort(portNo, baud, parity, databits, stopbits);
        serialPort_->SetReadFunc(disptch_);
    }

    void ATCommand::finallySerialPort_() {
        serialPort_->ClosePort();
        delete serialPort_;
    }

}
