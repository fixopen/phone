#ifndef __TIMER_H__
#define __TIMER_H__

#pragma warning(disable: 4786)

//#include <wtypes.h> //type define
//#define AFXAPI __stdcall
#include <set>

#include "thread.h"

namespace Util
{
    class Timer;
    class TimerManager;
    typedef void (*TimerHandler)(void* param);
    bool operator<(Timer const & lhs, Timer const & rhs);
    bool operator==(Timer const & lhs, Timer const & rhs);
    class Timer {
        friend TimerManager;
        friend bool operator<(Timer const & lhs, Timer const & rhs);
        friend bool operator==(Timer const & lhs, Timer const & rhs);
    public:
        static TimerManager* manager;
        enum Type {
            PlayerVideo,
            PlayerImage,
            PlayerText,
            FTP,
            Manager,
            Device,
            Main,
            Other,
        };
        enum Policy {
            tpNull,
            tpAutoCancel,
            /*超时后自动销毁自身，缺省*/
            tpAutoRestart,
            /*超时后自动重起，也就是所谓的周期性重复时钟*/
            tpStiff,
            /*超时候变成僵尸*/
            tpOther
        };
    public:
        Timer(void) //constructor the null timer

        : id_(0)
        , type_(Other)
            //, registerTick_(0)

        , timeout_(0)
        , tickPoint_(0)
        , policy_(tpNull)
        , action_(0) {
        }
    private:
        unsigned int id_;
        Type type_;
        //unsigned int registerTick_;
        unsigned int timeout_;
        unsigned int tickPoint_;
        Policy policy_;
        TimerHandler action_;
        void* param_;
        static unsigned int unusedId_;
    };

    class TimerManager {
    public:
        TimerManager(void);

        void SetCurrentTick(unsigned int const currentTick) {
            currentTick_ = currentTick;
            std::multiset<Timer>::iterator i = timerQueue_.begin();
            for (; i != timerQueue_.end(); ++i) {
                if (i->tickPoint_ > currentTick_) {
                    break;
                }
            }
            timerQueue_.erase(timerQueue_.begin(), i);
        }

        void printTimerQueue() {
            printf("[[[\n");
            for (std::multiset<Timer>::iterator i = timerQueue_.begin(); i != timerQueue_.end(); ++i) {
                printf("id is %d, type is %d, tickPoint is %d, timeout is %d, action is %x\n", i->id_, i->type_, i->tickPoint_, i->timeout_, i->action_);
            }
            printf("]]]\n");
        }

        unsigned int const request(unsigned int const timeout, TimerHandler const delegate, void* const param);
        unsigned int const request(unsigned int const timeout, Timer::Policy const policy, TimerHandler const delegate, void* const param);
        unsigned int const request(unsigned int const timeout, Timer::Type const type, TimerHandler const delegate, void* const param);
        unsigned int const request(unsigned int const timeout, Timer::Type const type, Timer::Policy const policy, TimerHandler const delegate, void* const param);
        unsigned int const requestP(unsigned int const tickPoint, TimerHandler const delegate, void* const param);
        unsigned int const requestP(unsigned int const tickPoint, Timer::Policy const policy, TimerHandler const delegate, void* const param);
        unsigned int const requestP(unsigned int const tickPoint, Timer::Type const type, TimerHandler const delegate, void* const param);
        unsigned int const requestP(unsigned int const tickPoint, Timer::Type const type, Timer::Policy const policy, TimerHandler const delegate, void* const param);
        void release(unsigned int const timerId);
        void release(Timer::Type const type, unsigned int const startTick, unsigned int const stopTick);
        void nativeTick(int e);
        //static void outerTick(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
    private:
        typedef int Lock;
        std::multiset<Timer> timerQueue_;
        unsigned int currentTick_;
        Lock lock_;
        class TimerThread : public Thread {
            friend TimerManager;
        public:
            TimerThread()
            : Thread() {
            }
            virtual int const run(void);

            void willStop() {
                exit_ = true;
            }
        private:
            volatile bool exit_;
            int prevValue_;
        };
        TimerThread* timerThread_;
    };
    //bool operator<(TimerManager::ForMassTimer const& lhs, TimerManager::ForMassTimer const& rhs);
}

#endif //__TIMER_H__
