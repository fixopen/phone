#include "timer.h"

namespace Util {
    using std::set;
    unsigned int Timer::unusedId_ = 1;
    TimerManager* Timer::manager = new TimerManager();

    bool operator<(Timer const& lhs, Timer const& rhs) {
        //return lhs.registerTick_ + lhs.timeout_ > rhs.registerTick_ + rhs.timeout_;
        return lhs.tickPoint_ < rhs.tickPoint_;
    }

    bool operator==(Timer const& lhs, Timer const& rhs) {
        //return lhs.registerTick_ + lhs.timeout_ > rhs.registerTick_ + rhs.timeout_;
        return lhs.tickPoint_ == rhs.tickPoint_;
    }

    TimerManager::TimerManager(void)
        : lock_(0/*semBCreate(SEM_Q_FIFO, SEM_FULL)*/)
        , currentTick_(0)
        , timerThread_(0) {
            timerThread_ = new TimerThread();
            timerThread_->prevValue_ = ::timeGetTime();
            timerThread_->start();
    }

    unsigned int const TimerManager::request(unsigned int const timeout, TimerHandler const delegate, void* const param) {
        return request(timeout, Timer::Other, Timer::tpAutoCancel, delegate, param);
    }

    unsigned int const TimerManager::request(unsigned int const timeout, Timer::Policy const policy, TimerHandler const delegate, void* const param) {
        return request(timeout, Timer::Other, policy, delegate, param);
    }

    unsigned int const TimerManager::request(unsigned int const timeout, Timer::Type const type, TimerHandler const delegate, void* const param) {
        return request(timeout, type, Timer::tpAutoCancel, delegate, param);
    }

    unsigned int const TimerManager::request(unsigned int const timeout, Timer::Type const type, Timer::Policy const policy, TimerHandler const delegate, void* const param) {
        Timer timer;
        timer.id_ = ++Timer::unusedId_;
        timer.type_ = type;
        //timer.registerTick_ = currentTick_;
        timer.timeout_ = timeout;
        timer.tickPoint_ = currentTick_ + timeout;
        timer.policy_ = policy;
        timer.action_ = delegate;
        timer.param_ = param;
        if (true/*semTake(lock_, WAIT_FOREVER) == OK*/) {
            timerQueue_.insert(timer);
            //semGive(lock_);
        }
        return timer.id_;
        //Util::Timer::manager->printTimerQueue();
    }

    unsigned int const TimerManager::requestP(unsigned int const tickPoint, TimerHandler const delegate, void* const param) {
        return requestP(tickPoint, Timer::Other, Timer::tpAutoCancel, delegate, param);
    }

    unsigned int const TimerManager::requestP(unsigned int const tickPoint, Timer::Policy const policy, TimerHandler const delegate, void* const param) {
        return requestP(tickPoint, Timer::Other, policy, delegate, param);
    }

    unsigned int const TimerManager::requestP(unsigned int const tickPoint, Timer::Type const type, TimerHandler const delegate, void* const param) {
        return requestP(tickPoint, type, Timer::tpAutoCancel, delegate, param);
    }

    unsigned int const TimerManager::requestP(unsigned int const tickPoint, Timer::Type const type, Timer::Policy const policy, TimerHandler const delegate, void* const param) {
        Timer timer;
        timer.id_ = ++Timer::unusedId_;
        timer.type_ = type;
        //timer.registerTick_ = currentTick_;
        timer.timeout_ = tickPoint - currentTick_;
        timer.tickPoint_ = tickPoint;
        timer.policy_ = policy;
        timer.action_ = delegate;
        timer.param_ = param;
        if (true/*semTake(lock_, WAIT_FOREVER) == OK*/) {
            timerQueue_.insert(timer);
            //semGive(lock_);
        }
        return timer.id_;
        //Util::Timer::manager->printTimerQueue();
    }

    void TimerManager::release(unsigned int const timerId) {
        for (std::multiset<Timer>::iterator i = timerQueue_.begin(); i != timerQueue_.end(); ++i) {
            if (i->id_ == timerId) {
                timerQueue_.erase(i);
                break;
            }
        }
        //Util::Timer::manager->printTimerQueue();
    }

    void TimerManager::release(Timer::Type const type, unsigned int const startTick, unsigned int const stopTick) {
        for (std::multiset<Timer>::iterator i = timerQueue_.begin(); i != timerQueue_.end(); ++i) {
            if (i->tickPoint_ > startTick && i->tickPoint_ < stopTick) {
                if (i->type_ == type)
                    timerQueue_.erase(i++);
            }
        }
        //Util::Timer::manager->printTimerQueue();
    }

    void TimerManager::nativeTick(int e) {
        if (true/*semTake(lock_, WAIT_FOREVER) == OK*/) {
            currentTick_ += e;
            if (!timerQueue_.empty()) {
                //printf("first element tickPoint is %d\n", timerQueue_.begin()->tickPoint_);
                while (currentTick_ == timerQueue_.begin()->tickPoint_) {
                    switch (timerQueue_.begin()->policy_) {
                    case Timer::tpAutoRestart:
                        {
                            Timer timer;
                            timer.id_ = ++Timer::unusedId_;
                            timer.type_ = timerQueue_.begin()->type_;
                            //timer.registerTick_ = currentTick_;
                            timer.timeout_ = timerQueue_.begin()->timeout_;
                            timer.tickPoint_ = currentTick_ + timerQueue_.begin()->timeout_;
                            timer.policy_ = timerQueue_.begin()->policy_;
                            timer.action_ = timerQueue_.begin()->action_;
                            timer.param_ = timerQueue_.begin()->param_;
                            timerQueue_.insert(timer);
                        }
                        break;
                    default:
                        break;
                    }
                    if (timerQueue_.begin()->action_) {
                        timerQueue_.begin()->action_(timerQueue_.begin()->param_);
                    }
                    timerQueue_.erase(timerQueue_.begin());
                }
            }
            //semGive(lock_);
        }
    }

    /*
    void TimerManager::outerTick(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
    {
    Timer::manager->nativeTick();
    //Timer::manager->printTimerQueue();
    }
    */

    int const TimerManager::TimerThread::run(void) {
        while (!exit_) {
            int now = ::timeGetTime();
            Timer::manager->nativeTick(now - prevValue_);
            prevValue_ = now;
            ::Sleep(10);
        }
        return 0;
    }
}
