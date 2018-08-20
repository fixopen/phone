#ifndef __UTIL_ATCOMMANDWARP_H__
#define __UTIL_ATCOMMANDWARP_H__

#include "../Logical/serialPort.h"
#include "Observer.h"
#include <string>
#include <vector>
#include <Ras.h>
#include <Wininet.h>
#include <Raserror.h>

/*
艺术源于灵感，而设计源于动机。
艺术为人诠释，而设计为人理解。
艺术是一种品味，而设计则是一种主张。
艺术来自天赋，而设计来自技艺。
艺术为不同的人带来不同的信息，而设计为不同的人带来相同的信息。
*/

namespace Util {
    namespace AT {
        struct CREGResponse { //register
            //+CREG:state[,"locationAreaCode","cellId"]
            CREGResponse(std::string const& content);
            enum State {
                sUnregisterNotSearch = 0,
                wRegisterHome = 1,
                sUnregisterAndSearch = 2,
                sRegisterDenied = 3,
                sUnknown = 4,
                sRegisterRoaming = 5,
            };
            State state;
            std::string locationAreaCode;
            std::string cellId;
        };

        struct CIEV { //Mobile termination Event Report
            //+CIEV:indication, value //Indicator EVent report
            CIEV(std::string const& content);
            enum Indication {
                iBatteryChargeLevel = 1, //0-5::0 20 40 60 80 100
                iSignalQulity = 2, //0-5
                iServiceLevel = 3, //0 - 1 Service availability
                iSounderActivity = 4, //0 tone stop, 1 incoming call ringing, waiting call tone, call process tone, busy tone
                iUnreadedShortMessage = 5, //0 1
                iCallInProgress = 6, //0 1
                iRoamingStatus = 7, //0 1
                iSMSStorageFull = 8, //0 available 1 full
            };
            Indication indication;
            int value;
        };

        //OK 0
        //CONNECT 1
        //RING 2
        //NO CARRIER 3
        //ERROR 4
        //BUSY 7
        //+CME ERROR
        //NO DIAL TONE
        //NO DIALTONE

        struct DUSIMUResponse {
            //^DUSIMU:state
            DUSIMUResponse(std::string const& content);
            enum State {
                sAbsent = 0,
                sPresent = 1,
            };
            State state;
        };

        enum NumberType {
            tUnknown = 129, //IDSN
            tNational = 161, //IDSN
            tInternational = 145, //ISDN
            tNetworkSpecific = 177, //ISDN
            tSub = 255, //SubNumber
            tNonSub = 0, //SubNumberNon
        };

        enum Direction {
            dOriginated = 0, //主叫
            dTerminated = 1, //被叫
        };

        enum State {
            sActive = 0,
            sHeld = 1,
            sDialing4O = 2,
            sAlerting4O = 3,
            sIncoming4T = 4,
            sWaiting4T = 5,
            sTerminated = 6,
        };

        enum Mode {
            mVoice = 0,
            mData = 1,
            mFax = 2,
            mVoiceAndData1 = 3,
            mVoiceOrData1 = 4,
            mVoiceOrFax1 = 5,
            mVoiceAndData2 = 6,
            mVoiceOrData2 = 7,
            mVoiceOrFax2 = 8,
            mUnknown = 9,
        };

        enum Conference {
            cNoMultiparty = 0,
            cMultiparty = 1,
        };

        enum ServiceType {
            tAsync, //ASYNC
            tSync, //SYNC
            tRelAsync, //REL ASYNC
            tRelSync, //REL SYNC
            tFax, //FAX
            tVoice, //VOICE
            tGprs, //GPRS PDP_type, PDP_addr // GPRS "IP","x.y.z.w"
            tError, //error
        };

        ServiceType const ParseServiceType(std::string const& typeString);
        std::string const ServiceTypeToString(ServiceType const serviceType);

        struct CLCCResponse { //list current call
            //+CLCC:index,direct,state,mode,conference[,"number",type]<CR><LF>
            CLCCResponse(std::string const& content);
            int index; //=1, normal
            Direction direct;
            State state;
            Mode mode;
            Conference conference;
            std::string number;
            NumberType type;
        };

        struct Incoming { //ring
            Incoming(std::string const& content);
            enum AlphaId {
                //TODO: not understand it
            };
            enum CLIValidity {
                vValid = 0,
                OriginatorWithheld = 1,
                NotAvailable = 2,
            };
            ServiceType serviceType;
            std::string number;
            NumberType type;
            std::string subAddr; //default null
            NumberType subAddrType; //default null
            //AlphaId alphaId; //default null
            int alphaId;
            CLIValidity cliValidity;
        };

        struct DVTCODECResponse {
            DVTCODECResponse(std::string const& content);
            //^DVTCODEC:{1:3:2}[,...]
            enum Action {
                aStart = 1,
                aConsultation = 3,
                aStop = 2,
            };
            Action action;
            enum Format {
                fH323 = 1,
                fMP4 = 2,
            };
            Format decodeFormat;
            int mp4PostFilter;
            Format encodeFormat;
            int width;
            int height;
            int targetBitrate;
            int vopTime;
            int timeIncreamentResolution;
            int timeIncSize;
            int tstampclr; //??
            int quantType;
            int quantMatrix[2][64];
            int qFactor;
            int IFrameInterval;
            int acPredictFlag;
            int halfMoveFlag;
            int fourMoveFlag;
            int variableBitrateFlag;
            int resyncFlag;
            int dataPartFlag;
            int reverseVLCflag;
            int shortVideoHeaderFlag;
            //int sliceCount;
        };

        struct CPASResponse { //phone activity status of MT
            //+CPAS:state
            CPASResponse(std::string const& content);
            enum State {
                sReady = 0,
                sUnavailable = 1,
                sUnknown = 2,
                sRinging = 3,
                sProgressOrHold = 4,
                sSleep = 5,
            };
            State state;
        };

        struct DSCIResponse { //State of Call Indicator
            DSCIResponse(std::string const& content);
            int id;
            Direction direction;
            State state;
            Mode mode;
            Conference conference;
            std::string number;
            NumberType type;
            ServiceType serviceType; //when mode is 3-8, this field valid
            enum Cause {
                cUnassignedNumber = 0, //unallocated number
                cNoRouteToDestination = 3,
                cChannelUnacceptable = 6,
                cOperatorDeterminedBarring = 8,
                cNormalCallClearing = 16,
                cUserBusy = 17,
                cNoUserResponding = 18,
                cUserAlertingNotAnswer = 19,
                cCallRejected = 21,
                cNumberChanged = 22,
                cPreemption = 25,
                cNonSelectedUserClearing = 26,
                cDestinationOutOfOrder = 27,
                cInvalidNumberFormat = 28, //incomplete number
                cFacilityRejected = 29,
                cResponseToStatusEnquiry = 30,
                cNormalUnspecified = 31,
                cNoCircuitChannelAvailable = 34,
                cNetworkOutOfOrder = 38,
                cTemporaryFailure = 41,
                cSwitchingEquipmentCongestion = 42,
                cAccessInformagtionDiscarded = 43,
                cRequestedCircuitChannelUnavailable = 44,
                cResourceUnavailableUnspecified = 47,
                cQualityOfServiceUnavailable = 49,
                cRequestedFacilityNotSubscribed = 50,
                cIncomingCallsBarredWithinTheCUG = 55,
                cBearerCapabilityNotAuthorized = 57,
                cBearerCapabilityNotPresentlyAvailable = 58,
                cServiceOrOptionNotAvailableUnspecified = 63,
                cBearerServiceNotImplemented = 65,
                cACMEqualToOrGreaterThanACMmax = 68,
                cRequestedFacilityNotImplementesd = 69,
                cOnlyRestrictedDigitalInfoBearerCapabilityIsAvailable = 70,
                cServiceOrOptionNotImplementedUnspecified = 79,
                cInvalidTransactionIdentifierValue = 81,
                cUserNotMemberOfCUG = 87,
                cIncompatibleDestination = 88,
                cInvalidTransitNetworkSelection = 91,
                cSemanticallyIncorrectMessage = 95,
                cInvalidMandatoryInformation = 96,
                cMessageTypeNonexistentOrNotImplemented = 97,
                cMessageTypeNotCompatibleWithProtocolState = 98,
                cInformationElementNonexistentOrNotImplemented = 99,
                cConditionalIEError = 100,
                cMessageNotCompatibleWithProtocolState = 101,
                cRecoveryOnTimerExpiry = 102,
                cProtocolErrorUnspecified = 111,
                cInterworkingUnspecified = 127,
            };
            Cause cause; //only disconnected valid
            int transactionId;
            std::string subAddr;
            NumberType subAddrType;
        };

        struct DCPIResponse { //call progress indication
            DCPIResponse(std::string const& content);
            enum Type {
                tSetup = 0, //sent out
                tDisconnect = 1,
                tAlerting = 2,
                tCallProceeding = 3,
                tChannelAssignment = 4,
                tProgressDescription = 5,
                tConnect = 6,
            };
            enum BandTone {
                bNoInBandTone = 0,
                bInBandTone = 1,
            };
            enum TCH {
                cNotAssigned = 0,
                cAssigned = 1,
            };
            int id;
            Type type;
            BandTone bandTone;
            TCH tch;
        };

		struct CMTIndicator { //SMS indicator
			//+CMT:[alpha],length<CR><LF>pdu
			//+CMT:oa,[alpha],scts[,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>
			//+CBM: <length><CR><LF><pdu>
			//+CBM: <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data>
			//+CDS: <length><CR><LF><pdu>
			//+CDS: <fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>
			//+CDSI: <mem>,<index>
			CMTIndicator(std::string const& content);
			std::string alpha;
			int length;
			std::string pdu;

		};

        struct Error {
            //+CME ERROR:error
            Error(std::string const& content);
            enum ErrorCode {
                ePhoneFailure = 0,
                eNoConnectionToPhone = 1,
                ePhoneAdaptorLinkReserved = 2,
                eOperationNotAllowed = 3,
                eOperationNotSupported = 4,
                ePHSIMPINRequired = 5,
                ePHFSIMPINRequired = 6,
                ePHFSIMPUKRequired = 7,
                eSIMNotInserted = 10,
                eSIMPINRequired = 11,
                eSIMPUKRequired = 12,
                eSIMFailure = 13,
                eSIMBusy = 14,
                eSIMWrong = 15,
                eIncorrectPassword = 16,
                eSIMPIN2Required = 17,
                eSIMPUK2Required = 18,
                eMemoryFull = 20,
                eInvalidIndex = 21,
                eNotFound = 22,
                eMemoryFailure = 23,
                eTextStringTooLength = 24,
                eInvalidCharactersInTextString = 25,
                eDialStringTooLength = 26,
                eInvalidCharactersInDialString = 27,
                eNoNetworkService = 30,
                eNetworkTimeout = 31,
                eNetworkNotAllowed = 32, //emergency calls only
                eNetworkPersonalizationPINRequired = 40,
                eNetworkPersonalizationPUKRequired = 41,
                eNetworkSubsetPersonalizationPINRequired = 42,
                eNetworkSubsetPersonalizationPUKRequired = 43,
                eServiceProviderPersonalizationPINRequired = 44,
                eServiceProviderPersonalizationPUKRequired = 45,
                eCorporatePersonalizationPINRequired = 46,
                eCorporatePersonalizationPUKRequired = 47,
                eHiddenKeyRequired = 48, //this key is required when accessing hidden phonebook entries
                eUnkown = 100,
            };
            ErrorCode error;
        };
    }

    enum APNType {
        tCMWAP,
        tCMNET,
        tLABWAP3,
    };

    class ATCommand : public Observable {
    public:
        static ATCommand* const Instance() {
            static ATCommand* result = new ATCommand(8);
            return result;
        }
    public:
        enum ActionState {
            sIdle,
            sBusy,
        };
        enum Action {
            aNull,
            aReset,
            aSetupSerialPort,
            aSetupWakeupMode,
            aSetupNetworkSignalIndicator,
            aSetupIncomingExtendIndicator,
            aSetupProgressIndicator,
            aSetupCallProgressIndication,
            //sInited,
            aSetupNetworkRegisterStateReport,
            aSetupHighLevelProtocolEventReport,
			aSetupAccessTechChangeIndication,
            aActiveUSIM,
            aInputPIN,
            aSetVideoChannel,
            aEnableH324M,
            aConfigH324MCodec,
            aActiveFullProtocolStack,
            aSearchNetworkAndAutoRegister,
			aSwitchNetwork,
            aGetAccessTech,
            //sRegistered,
            aUnregisterFromNetwork,
            aDeactiveFullProtocolStack,
            aDeactiveUSIM,
            aSoftwarePowerOff,
            //aError,
        };
        enum Event {
            eNull,
            eInit,
            eActive,
            eRequestPIN,
            eInputPIN,
            eRegister,
            eRegisterSuccess,
            eUnregister,
            eFinally,
            eError,
        };

		enum NetworkType {
			ntGsm,
			ntTds,
		};

        void ActionEnd() {
            actionState_ = sIdle;
        }
        void ClearAction() {
            currentAction_ = aNull;
        }
        Action const CurrentAction()  const {
            return currentAction_;
        }
        bool const Register() const {
            return isRegister_;
        }
        void Registered() {
            isRegister_ = true;
        }
        void Write(std::string const& data,std::string const& finallyResponse = "", RS232* const port = 0);
        void Start();
        DWORD const InitDial(APNType const apnType);
        DWORD const Dial(HWND const notifyWindow);
        static std::pair<int, std::wstring> const OnRasStateChanged(WPARAM w, LPARAM l);
        std::wstring const GetDynamicIP() const;
        bool const Hangup();
        void Stop();
        void CommonResponseProcess(std::string const& data);
    protected:
        ATCommand(unsigned int const portNo);
        ~ATCommand();

        void reset3GModule();
        void controlEP(int const number, bool const enable);
        void setGPIOWakeupMode();
        void enableNetworkSignalIndicator();
        void enableIncomingExtendIndicator();
        void enableProgressIndicator();
        void enableCallProgressIndication();
        void enableRegisterStateReport();
        void enableProtocolStackIndicator();
		void enableAccessTechChangeIndication();
        void enableUSIM();
        void enableH324M();
        void configH324MCodec();
        void enableProtocolStack();
		void forceNetwork(NetworkType const networkType);
		void getCOPSlist();
        void searchAndRegister();
        /*std::vector<int> const*/void getAccessTech();
        void unregister();
        void disableProtocolStack();
        void disableUSIM();
        void softwarePowerOff();

    private:
        void errorProcess_();
        void initSerialPort_(unsigned int const portNo = 1, unsigned int const baud = 9600, unsigned int const parity = NOPARITY, unsigned int const databits = 8, unsigned int const stopbits = 1);
        void finallySerialPort_();
        static void disptch_(unsigned char const* const data, unsigned int const length);
        Util::RS232* serialPort_; // = 0;

        HRASCONN rasConn_;
        //int rasConnState_;

        ActionState actionState_;
        Action currentAction_;
        Event event_;
        bool isRegister_;

		std::string  finallyResponse_;
		//DWORD startTime_;
		SYSTEMTIME startTime_;
    };
}

#endif // __UTIL_ATCOMMANDWARP_H__
