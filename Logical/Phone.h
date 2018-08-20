#ifndef __LOGICAL_PHONE_H__
#define __LOGICAL_PHONE_H__

#include "../Data/ContactInfo.h"
#include "../Data/SoundSegment.h"
#include "../Util/SmartPtr.h"
#include "../Util/FSM.h"
#include "../Util/Thread.h"

#pragma warning(disable : 4786)
#include <set>

#include <dshow.h>

namespace Logical
{
    enum SoundDestination
    {
        sdNull,
        sdSpeaker,
        sdHandsetSpeaker,
        sdStorage,
        sdLine,
        sdError,
    };

    enum SoundSource
    {
        ssNull,
        ssMic,
        ssHandsetMic,
        ssStorage,
        ssLine,
        ssError,
    };

    enum TelephoneCommand
    {
        tcPickup,
        tcHungup,
        tcStartRing,
        tcStopRing,
    };

// 	int const CALLID_NUM_LEN = 32;
// 	int const CALLID_NAME_LEN = 32;
    struct CallIdInfo
    {
        CallIdInfo()
        {
            ::memset(reinterpret_cast<void*>(this), 0, sizeof(CallIdInfo));
        }

        unsigned char type;  // CALLID_TYPE_SIMPLE / CALLID_TYPE_COMPLEX

        unsigned char timeStatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
        unsigned char month;  // 1 - 12
        unsigned char day;  // 0 - 31
        unsigned char hour;  // 0 - 23
        unsigned char minute;  // 0 - 59

        unsigned char numberStatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
//         char number[CALLID_NUM_LEN];  // string ending with '\0'
        char number[32];  // string ending with '\0'

        unsigned char nameStatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
//         char name[CALLID_NAME_LEN];  // string ending with '\0'
        char name[32];  // string ending with '\0'
    };

    class RecordThread;
    class PlayThread;
    class Phone : public Util::Thread
    {
        friend RecordThread;
        friend PlayThread;
    public:
        typedef void (*PickupFunc)();
        typedef void (*HandsfreeFunc)();
        typedef void (*HangupFunc)();
        typedef void (*KeypadDialFunc)(unsigned char const& keyCode);
        typedef void (*ConnectFunc)();
        typedef void (*RingInFunc)(Data::TelephoneNumber number);
        typedef void (*RemoteHangupFunc)();
        typedef void (*TimeoutFunc)();
        typedef void (*ErrorFunc)();
    public:
        Phone(PickupFunc pickup, HandsfreeFunc handsfree, HangupFunc hangup, KeypadDialFunc keypadDial, ConnectFunc connect, RingInFunc ringIn, RemoteHangupFunc remoteHangup, TimeoutFunc timeout, ErrorFunc error); //init phone system
        ~Phone(); //destory phone system
        void Dial(Data::TelephoneNumber number);
        boost::shared_ptr<Data::ContactInfo> GetContactInfo() const;
		unsigned char KeyCode() const
		{
			return keyCode_;
		}
        void SetSoundSource(std::set<SoundSource> soundSources);
        void SetSoundDestination(std::set<SoundDestination> soundDestinations);
        void AppendSoundSource(SoundSource const& soundSource);
        void AppendSoundDestination(SoundDestination const& soundDestination);
        void RemoveSoundSource(SoundSource const& soundSource);
        void RemoveSoundDestination(SoundDestination const& soundDestination);
        void StartRecord(std::string const& filename);
        void StopRecord();
        void StartPlay(std::string const& filename);
        void StopPlay();
		//void Error();
        PickupFunc Pickup;
        HandsfreeFunc Handsfree;
        HangupFunc Hangup;
        KeypadDialFunc KeypadDial;
        ConnectFunc Connect;
        RingInFunc RingIn;
        RemoteHangupFunc RemoteHangup;
        TimeoutFunc Timeout;
        ErrorFunc Error;
        void willStop()
        {
            quit_ = true;
        }
        virtual int const run();
        void IncreamentRingCount()
        {
            ++ringCount_;
        }
        void RecordCallId()
        {
            //callIdInfo_ = callIdInfo;
        }
		void RecordContactInfo()
		{
		}
		void SubKey()
		{
		}
    private:
        void SendCommandToTelephone_(TelephoneCommand command, unsigned char const* const code);
        //void ConvertASCIItoDTMF_(unsigned char* const dst, unsigned char const* const src, unsigned int const length);
        void DecodeDTMFCallId_(unsigned char* const dst, unsigned char const* const src, unsigned int const length);
        //void ConvertASCIItoFSK_(unsigned char* const dst, unsigned char const* const src, unsigned int const length);
        void DecodeFSKCallId_(unsigned char const* const src, unsigned int const length, CallIdInfo* callIdInfo);
		void DecodeFSKSimpleParam_(unsigned char* param, int length, CallIdInfo* callIdInfo);
		void DecodeFSKComplexParam_(unsigned char* param, CallIdInfo* callIdInfo);
    private:
        volatile bool quit_;
        HANDLE telephoneHandle_;
        std::set<SoundDestination> soundDestinations_;
        std::set<SoundSource> soundSources_;
        boost::shared_ptr<Data::ContactInfo> contactInfo_;
		unsigned char keyCode_;
        size_t ringCount_;
        CallIdInfo* callIdInfo_;
        RecordThread* recordThread_;
        PlayThread* playThread_;
        typedef int TimerId;
        TimerId timerId_;
        Util::FSM fsm_;

        IGraphBuilder* mediaPipe_;
        IMediaControl* mediaControl_;
        IBasicAudio* basicAudio_;
        IMediaEvent* mediaEvent_;
    };

    class RecordThread : public Util::Thread
    {
    public:
        RecordThread(Phone* owner)
        : owner_(owner)
        , quit_(false)
        , passive_(true)
        {
        }
        virtual int const run();
        void Start()
        {
            passive_ = false;
        }
        void Stop()
        {
            passive_ = true;
        }
        void willStop()
        {
            quit_ = true;
        }
    private:
        volatile bool quit_;
        volatile bool passive_;
        Phone* owner_;
        FILE* soundFile_;
    };

    class PlayThread : public Util::Thread
    {
    public:
        PlayThread(Phone* owner)
        : owner_(owner)
        , quit_(false)
        , passive_(true)
        {
        }
        virtual int const run();
        void Start(std::string filename)
        {
            ringFilename_ = filename;
            passive_ = false;
        }
        void Stop()
        {
            ringFilename_ = "";
            passive_ = true;
        }
        void SetVolume(int volume)
        {
            if (owner_->basicAudio_)
            {
                owner_->basicAudio_->put_Volume(volume);
            }
        }
        void willStop()
        {
            quit_ = true;
        }
    private:
        Phone* owner_;
        volatile bool quit_;
        volatile bool passive_;
        volatile bool isRepeat_;
        std::string ringFilename_;
        HANDLE playHandle_;
    };
}

#endif //__LOGICAL_PHONE_H__
