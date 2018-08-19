#ifndef __THREAD_H__
#define __THREAD_H__

#include <windef.h> //for HANDLE

namespace Util {
    class Thread {
    public:
        enum Status {
            tsNull,
            tsCreated,
            tsRunning,
            tsSleeping,
            tsExit,
            tsZomb,
            tsOther
        };
        //typedef int HANDLE;

        Thread();
        virtual ~Thread();
        virtual int const run();
        void start(int const pri = 251);
        void stop();
        void state(enum Status const state);
        int const id() const;
        void id(int const id);
        HANDLE const handle() const;
        void handle(HANDLE const handle);
    private:
        HANDLE handle_;
        int id_;
        enum Status state_;
    };
}

#endif //__THREAD_H__
