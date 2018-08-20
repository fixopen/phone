#include "stdafx.h"

#include "Phone.h"

namespace Logical//scan html text fire follow event
{
    enum TelephoneState
    {
        tsIdle,
        tsHandsfree,
        tsPickup,
        tsDialKey,
        tsConnected,
        tsRecording,
        tsPlaying,
        tsRing,
        tsError,
    };

    enum TelephoneEvent
    {
        teNull,
        tePickup,
        teHandsfree,
        teHangup,
        teRemoteHangup,
        teKey,
        teConnect,
        teRing,
        teCallID,
        teTimeout,
        teStartRecord,
        teStopRecord,
        teStartPlay,
        teStopPlay,
        teError,
    };

    void pickup(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Pickup();
    }

    void handsfree(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Handsfree();
    }

    void ring(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->RingIn(p->GetContactInfo()->telephoneNumber());
    }

    void hangupNotRecord(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Hangup();
    }

    void keyAndStartTimeout(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->KeypadDial(p->KeyCode());
    }

    void dialTimeout(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Timeout();
    }

    void connect(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Connect();
    }

    void remoteHangup(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->RemoteHangup();
    }

    void increamentRingCount(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->IncreamentRingCount();
    }

    void recordCallId(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->RecordCallId();
    }

    void hangup(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Hangup();
        p->RecordContactInfo();
    }

    void subKey(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->SubKey();
    }

    void startRecord(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->StartRecord("filename.mp3");
    }

    void startPlay(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->StartPlay("filename.mp3");
    }

    void communicationError(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Error();
    }

    void stopRecord(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->StopRecord();
    }

    void stopPlay(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->StopPlay();
    }

    void recordError(void* params)
    {
        MyPhone* p = reinterpret_cast<MyPhone*>(params);
        p->Error();
    }

    MyPhone::MyPhone(PickupFunc apickup, HandsfreeFunc ahandsfree, HangupFunc ahangup, KeypadDialFunc akeypadDial, ConnectFunc aconnect, RingInFunc aringIn, RemoteHangupFunc aremoteHangup, TimeoutFunc atimeout, ErrorFunc aerror)
        : Pickup(apickup)
        , Handsfree(ahandsfree)
        , Hangup(ahangup)
        , KeypadDial(akeypadDial)
        , Connect(aconnect)
        , RingIn(aringIn)
        , RemoteHangup(aremoteHangup)
        , Timeout(atimeout)
        , Error(aerror)
        , ringCount_(0)
    {
        //init fsm
        fsm_.registerRule(tsIdle, tePickup, tsPickup, pickup);
        fsm_.registerRule(tsIdle, teHandsfree, tsHandsfree, handsfree);//no Handsfree
        fsm_.registerRule(tsIdle, teRing, tsRing, ring);

        fsm_.registerRule(tsPickup, teHangup, tsIdle, hangupNotRecord);
        fsm_.registerRule(tsPickup, teKey, tsDialKey, keyAndStartTimeout);
        fsm_.registerRule(tsPickup, teTimeout, tsError, dialTimeout);

        fsm_.registerRule(tsHandsfree, teHandsfree, tsIdle, hangupNotRecord);//no Handsfree
        fsm_.registerRule(tsHandsfree, teKey, tsDialKey, keyAndStartTimeout);//no Handsfree
        fsm_.registerRule(tsHandsfree, teTimeout, tsError, dialTimeout);//no Handsfree

        fsm_.registerRule(tsDialKey, teConnect, tsConnected, connect);
        fsm_.registerRule(tsDialKey, teHangup, tsIdle);
        fsm_.registerRule(tsDialKey, teHandsfree, tsIdle);//no Handsfree
        fsm_.registerRule(tsDialKey, teKey, tsDialKey, keyAndStartTimeout);

        fsm_.registerRule(tsRing, tePickup, tsConnected, connect);
        fsm_.registerRule(tsRing, teRemoteHangup, tsIdle, remoteHangup);//no RemoteHangup
        fsm_.registerRule(tsRing, teRing, tsRing, increamentRingCount);
        fsm_.registerRule(tsRing, teCallID, tsRing, recordCallId);

        fsm_.registerRule(tsConnected, teCallID, tsConnected, recordCallId);
        fsm_.registerRule(tsConnected, teHangup, tsIdle, hangup);
        fsm_.registerRule(tsConnected, teKey, tsConnected, subKey);
        fsm_.registerRule(tsConnected, teStartRecord, tsRecording, startRecord);
        fsm_.registerRule(tsConnected, teStartPlay, tsPlaying, startPlay);
        fsm_.registerRule(tsConnected, teError, tsError, communicationError);//Error ?

        fsm_.registerRule(tsRecording, teStopRecord, tsConnected, stopRecord);
        fsm_.registerRule(tsRecording, teTimeout, tsConnected, stopRecord);
        fsm_.registerRule(tsRecording, teError, tsConnected, stopRecord);//Error ?
        fsm_.registerRule(tsRecording, teHangup, tsIdle, stopRecord);//save contactInfo in stopRecord ?
        fsm_.registerRule(tsRecording, teRemoteHangup, tsIdle, stopRecord);//save contactInfo in stopRecord ?
        fsm_.registerRule(tsRecording, teCallID, tsRecording, recordCallId);//?

        fsm_.registerRule(tsPlaying, teStopPlay, tsConnected, stopPlay);
        fsm_.registerRule(tsPlaying, teTimeout, tsConnected, stopPlay);
        fsm_.registerRule(tsPlaying, teError, tsConnected, stopPlay);// ?
        fsm_.registerRule(tsPlaying, teHangup, tsIdle, stopPlay);//save contactInfo in stopPlay ?
        fsm_.registerRule(tsPlaying, teRemoteHangup, tsIdle, stopPlay);//save contactInfo in stopPlay ?
        fsm_.registerRule(tsPlaying, teCallID, tsPlaying, recordCallId);

        fsm_.registerRule(tsError, teHangup, tsIdle, recordError);

        fsm_.setStartState(tsIdle);

        //init record thread and play thread
        recordThread_ = new RecordThread(this);
        recordThread_->start();
        playThread_ = new PlayThread(this);
        playThread_->start();

        contactInfo_ = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo());

        //open the telephone hareware file
        telephoneHandle_ = ::CreateFile(_T("SCL1:"), // Pointer to the name of the port
            GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
            0,            // Share mode
            NULL,         // Pointer to the security attribute
            OPEN_EXISTING,// How to open the serial port
            0,            // Port attributes
            NULL);
        if (telephoneHandle_ == INVALID_HANDLE_VALUE)
        {
            //throw TelephoneHardwareError;
        }

        //init audio control interface
        ::CoInitialize(NULL);
        ::CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&mediaPipe_);
        mediaPipe_->QueryInterface(IID_IMediaControl, (void**)&mediaControl_);
        mediaPipe_->QueryInterface(IID_IBasicAudio, (void**)&basicAudio_);
        mediaPipe_->QueryInterface(IID_IMediaEvent, (void**)&mediaEvent_);
    }

    MyPhone::~MyPhone()
    {
        //destory audio control interface
        mediaEvent_->Release();
        basicAudio_->Release();
        mediaControl_->Release();
        mediaPipe_->Release();
        ::CoUninitialize();

        //the telephone hareware file
        ::CloseHandle(telephoneHandle_);

        //destory record thread and play thread
        playThread_->willStop();
        recordThread_->willStop();
        ::Sleep(1000);
        playThread_->stop();
        recordThread_->stop();
        //destory fsm
    }

    void MyPhone::Dial(Data::TelephoneNumber number)
    {
        //contactInfo_ = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo());
        contactInfo_->telephoneNumber(number);
        //reallyDial_();
    }

    void MyPhone::SetSoundSource(std::set<SoundSource> soundSources)
    {
        soundSources_ = soundSources;
    }

    void MyPhone::SetSoundDestination(std::set<SoundDestination> soundDestinations)
    {
        soundDestinations_ = soundDestinations;
    }

    void MyPhone::AppendSoundSource(SoundSource const& soundSource)
    {
        soundSources_.insert(soundSource);
    }

    void MyPhone::AppendSoundDestination(SoundDestination const& soundDestination)
    {
        soundDestinations_.insert(soundDestination);
    }

    void MyPhone::RemoveSoundSource(SoundSource const& soundSource)
    {
        soundSources_.erase(soundSource);
    }

    void MyPhone::RemoveSoundDestination(SoundDestination const& soundDestination)
    {
        soundDestinations_.erase(soundDestination);
    }

    void MyPhone::StartRecord(std::string const& filename)
    {
        fsm_.fireEvent(teStartRecord, 0);
        /*
        Util::shared_ptr<SoundSegment> result;
        result->filename(filename);
        soundFile_ = ::fopen(filename.c_str(), "wb+");
        AppendSoundDestination(sdStorage);
        return result;
        */
    }

    void MyPhone::StopRecord()
    {
        fsm_.fireEvent(teStopRecord, 0);
        //soundMedia->duration(xxs);
        //::fclose(soundFile_);
    }

    void MyPhone::StartPlay(std::string const& filename)
    {
        fsm_.fireEvent(teStartPlay, 0);
        /*
        switch (soundDestinations_)
        {
        playHandle_ = ::playSound(sound->filename());
        }
        */
    }

    void MyPhone::StopPlay()
    {
        fsm_.fireEvent(teStopPlay, 0);
        /*
        ::playStop(playHandle_);
        */
    }

    Util::shared_ptr<Data::ContactInfo> MyPhone::GetContactInfo() const
    {
        return contactInfo_;
    }

    unsigned char const* const telephoneCode = reinterpret_cast<unsigned char const* const>("01234567890*#ABCD");
    unsigned char const* const DTMFCode = reinterpret_cast<unsigned char const* const>("D1234567890*#ABC");

    int const MyPhone::run()
    {
        size_t const dataLength = 256;
        unsigned char data[dataLength] = {0};
        DWORD size;
        while (!quit_)
        {
            //readTelephoneFile;
            /*size_t count = */::ReadFile(telephoneHandle_, data, dataLength, &size, NULL);
            TelephoneEvent event = teNull;
            //parse it; constructe the event and fill the eventArgs to this object;
            CallIdInfo* callIdInfo = new CallIdInfo();
            unsigned char* parsed = (unsigned char*)malloc(size + 1);
            memset(parsed, 0, size + 1);
            for (int i = 0; i < size; ++i)
            {
                unsigned char c = data[i];
                switch (c)
                {
                case 0x84: //RING
                    event = teRing;
                    break;
                case 0x82: //Pickup
                    event = tePickup;
                    break;
                case 0x83: //Hangup
                    event = teHangup;
                    break;
                case 0x8F: //FSK END
                    DecodeFSKCallId_(data, dataLength, callIdInfo);
                    event = teCallID;
                    break;
                case 0x8E: //DTMF END
                    DecodeDTMFCallId_(parsed, data, dataLength);
                    event = teCallID;
                    break;
                case 0xA5: //PAGEUP
                    keybd_event('U', 0, 0, 0);
                    break;  
                case 0xA6: //PAGEDOWN
                    keybd_event('D', 0, 0, 0);
                    break;
                case 0x30:
                case 0x31:
                case 0x32:
                case 0x33:
                case 0x34:
                case 0x35:
                case 0x36:
                case 0x37:
                case 0x38:
                case 0x39:
                case 0x3A:
                case 0x3B:
                case 0x3C:
                case 0x3D:
                case 0x3E:
                case 0x3F:
                    //do nothing, the data in data buffer
                    break;
                case 0x40:
                case 0x41:
                case 0x42:
                case 0x43:
                case 0x44:
                case 0x45:
                case 0x46:
                case 0x47:
                case 0x48:
                case 0x49:
                case 0x4A:
                case 0x4B:
                case 0x4C:
                case 0x4D:
                case 0x4E:
                case 0x4F:
                    //do nothing, the data in data buffer
                    break;
                case 0xB0:
                case 0xB1:
                case 0xB2:
                case 0xB3:
                case 0xB4:
                case 0xB5:
                case 0xB6:
                case 0xB7:
                case 0xB8:
                case 0xB9:
                case 0xBA:
                case 0xBB:
                case 0xBC:
                case 0xBD:
                case 0xBE:
                case 0xBF:
                    keyCode_ = telephoneCode[c - 0xB0];
                    event = teKey;
                    //do nothing, the data in data buffer
                    break;
                default:
                    break;
                }
            }
            fsm_.fireEvent(event, this);
            ::Sleep(1);
        }
        return 0;
    }

    void MyPhone::SendCommandToTelephone_(TelephoneCommand command, unsigned char const* const code)
    {
        //0xD0 : open ring
        //0xD1 : close ring
        //0x00 : handsfree, 0x01 : pickup
        //0x03 : hungup
        unsigned char* commandValue = 0;
        size_t length = 0;
        switch (command)
        {
        case tcPickup:
            length = strlen((char*)code);
            commandValue = (unsigned char*)malloc(length + 2);
            commandValue[0] = 0x00;
            commandValue[1] = 0x01;
            memcpy(commandValue + 2, code, length);
            length += 2;
            break;
        case tcHungup:
            commandValue = (unsigned char*)malloc(1);
            commandValue[0] = 0x02;
            length = 1;
            break;
        case tcStartRing:
            commandValue = (unsigned char*)malloc(1);
            commandValue[0] = 0xD0;
            length = 1;
            break;
        case tcStopRing:
            commandValue = (unsigned char*)malloc(1);
            commandValue[0] = 0xD1;
            length = 1;
            break;
        }
        unsigned long totalWritedLength = 0;
        while (totalWritedLength < length)
        {
            unsigned long writedLength = 0;
            ::WriteFile(telephoneHandle_, commandValue + totalWritedLength, length - totalWritedLength, &writedLength, NULL);
            totalWritedLength += writedLength;
            ::Sleep(200);
        }
    }

    /*
    void MyPhone::ConvertASCIItoDTMF_(unsigned char* const dst, unsigned char const* const src, unsigned int const length)
    {
    for (int i = 0; i < length; ++i)
    {
    dst[i] = src[i];
    }
    }
    */

    void MyPhone::DecodeDTMFCallId_(unsigned char* const dst, unsigned char const* const src, unsigned int const length)
    {
        for (int i = 0; (i < length) && (src[i] != 0x8E); ++i)
        {
            dst[i] = DTMFCode[src[i] - 0x30]; //DTMF channel start
        }
    }

    /*
    void MyPhone::ConvertASCIItoFSK_(unsigned char* const dst, unsigned char const* const src, unsigned int const length)
    {
    }
    */

    void MyPhone::DecodeFSKCallId_(unsigned char const* const src, unsigned int const length, CallIdInfo* callIdInfo)
    {
        //assemble fsk packet
        size_t reallySrcLength = length / 2 + 2;
        unsigned char* reallySrc = (unsigned char*)malloc(reallySrcLength);
        memset(reallySrc, 0, reallySrcLength);
        for (int srcIndex = 0; (srcIndex < length) || (src[srcIndex] != 0x8F); ++srcIndex)
        {
            if (srcIndex % 2 == 0)
            {
                reallySrc[srcIndex / 2] = (src[srcIndex] << 4) & 0xF0;
            }
            else
            {
                reallySrc[srcIndex / 2] |= (src[srcIndex] << 0) & 0xF;
            }
        }
        //find fsk packet head
        size_t findHeadPosition = -1;
        for (int reallySrcIndex = 0; reallySrcIndex < reallySrcLength; ++reallySrcIndex)
        {
            if ((reallySrc[reallySrcIndex] == 0x04) || (reallySrc[reallySrcIndex] == 0x80)) //simple or complex
            {
                findHeadPosition = reallySrcIndex;
                reallySrcLength -= findHeadPosition;
                break;
            }
        }
        if (findHeadPosition == -1)
        {
            return;
        }
        //check fsk packet length
        if (reallySrc[findHeadPosition + 1] + 3 != reallySrcLength)
        {
            return;
        }
        //check fsk packet sum
        unsigned char sum = 0;
        for (int reallySrcIndex = 0; reallySrcIndex < reallySrcLength; ++reallySrcIndex)
        {
            sum += reallySrc[findHeadPosition + reallySrcIndex];
        }
        if (sum)
        {
            return;
        }
        unsigned char* param = reallySrc + findHeadPosition + 2;
        switch (reallySrc[findHeadPosition])
        {
        case 0x04:
            DecodeFSKSimpleParam_(param, reallySrc[findHeadPosition + 1], callIdInfo);
            break;
        case 0x80:
            while (param < reallySrc + reallySrc[findHeadPosition + 1] + 2)
            {
                DecodeFSKComplexParam_(param, callIdInfo);
                param += param[1] + 2;
            }
            break;
        }
        return;
    }

    void MyPhone::DecodeFSKSimpleParam_(unsigned char* param, int length, CallIdInfo* callIdInfo)
    {
        //param length check
        if (length < 8 + 1)
        {
            return;
        }
        //decode time
        callIdInfo->month = (*(param + 0) - '0') * 10 + (*(param + 1) - '0');
        callIdInfo->day = (*(param + 2) - '0') * 10 + (*(param + 3) - '0');
        callIdInfo->hour = (*(param + 4) - '0') * 10 + (*(param + 5) - '0');
        callIdInfo->minute = (*(param + 6) - '0') * 10 + (*(param + 7) - '0');
        callIdInfo->timeStatus = 0; //CallId ok

        //decode number
        unsigned char* number = param + 8;
        int numberLength = length - 8;
        //step1: discard odd-even check info
        for (int i = 0; i < numberLength; ++i)
        {
            number[i] &= 0x7F;
        }
        //step2: really decode
        switch (number[0])
        {
        case 'O': //not get == 3
            break;
        case 'P': //forbid == 2
            break;
        default: //normal
            for (int i = 0; (i < numberLength) && (i < 32 - 1); ++i) //32 : callId number length
            {
                number[i] &= 0xF;
                number[i] |= 0x30;
            }
            strncpy(callIdInfo->number, (char*)number, numberLength);
            callIdInfo->numberStatus = 0; //callId ok
            break;
        }
    }

    void MyPhone::DecodeFSKComplexParam_(unsigned char* param, CallIdInfo* callIdInfo)
    {
        size_t dataLength = param[1];
        unsigned char* data = param + 2;
        //step1: discard odd-even check info
        for (int i = 0; i < dataLength; ++i)
        {
            data[i] &= 0x7F;
        }

        switch (*param)
        {
        case 0x01: //CALLID_PARAM_TIME:
            //data time length error
            if (dataLength != 8)
                return;
            //decode time
            callIdInfo->month = (*(data + 0) - '0') * 10 + (*(data + 1) - '0');
            callIdInfo->day = (*(data + 2) - '0') * 10 + (*(data + 3) - '0');
            callIdInfo->hour = (*(data + 4) - '0') * 10 + (*(data + 5) - '0');
            callIdInfo->minute = (*(data + 6) - '0') * 10 + (*(data + 7) - '0');
            callIdInfo->timeStatus = 0; //CallId ok
            break;
        case 0x02: //CALLID_PARAM_NUMBER:
            //decode number
            for (int i = 0; (i < dataLength) && (i < 32 - 1); ++i) //32 : callId number length
            {
                data[i] &= 0xF;
                data[i] |= 0x30;
            }
            strncpy(callIdInfo->number, (char*)data, dataLength);
            callIdInfo->numberStatus = 0; //callId ok
            break;
        case 0x04: //CALLID_PARAM_NONUMBER:
            //set 'no number'
            switch (data[0])
            {
            case 'O': //not get == 3
                callIdInfo->numberStatus = 3; //callId not get
                break;
            case 'P': //forbid == 2
                callIdInfo->numberStatus = 2; //callId forbid
                break;
            default: //error, same as forbid
                callIdInfo->numberStatus = 2; //callId forbid
                break;
            }
            break;
        case 0x07: //CALLID_PARAM_NAME:
            //decode name
            strncpy(callIdInfo->name, (char*)data, dataLength);
            callIdInfo->nameStatus = 0; //callId ok
            break;
        case 0x08: //CALLID_PARAM_NONAME:
            //set 'no name'
            switch (data[0])
            {
            case 'O': //not get == 3
                callIdInfo->nameStatus = 3; //callId not get
                break;
            case 'P': //forbid == 2
                callIdInfo->nameStatus = 2; //callId forbid
                break;
            default: //error, same as forbid
                callIdInfo->nameStatus = 2; //callId forbid
                break;
            }
            break;
        }
    }

    int const RecordThread::run()
    {
        while (!quit_)
        {
            if (passive_)
            {
                ::Sleep(1000);
            }
            else
            {
                //do something
            }
        }
        return 0;
    }

    int const PlayThread::run()
    {
        while (!quit_)
        {
            if (passive_)
            {
                if (owner_->mediaControl_)
                    owner_->mediaControl_->Stop();
                ::Sleep(1000);
            }
            else
            {
                if (owner_->mediaPipe_ && owner_->mediaControl_)
                {
                    owner_->mediaPipe_->RenderFile((LPCTSTR)Util::StringOp::ToCString(ringFilename_), NULL);
                    owner_->mediaControl_->Run();
                }
                if (isRepeat_)
                {
                    long event = 0;
                    owner_->mediaEvent_->WaitForCompletion(5, &event);
                    if (event != 0)
                    {
                        passive_ = false;
                    }
                    else
                    {
                        passive_ = true;
                    }
                }
                else
                {
                    passive_ = true;
                }
            }
        }
        return 0;
    }
}
