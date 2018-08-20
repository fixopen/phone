#ifndef __THREAD_H__
#define __THREAD_H__

#include <windows.h>

namespace Util
{
    class Thread
    {
    public:
        enum Status
        {
            tsNull,
            tsCreated,
            tsRunning,
            tsSleeping,
            tsExit,
            tsZomb,
            tsOther
        };
        //typedef int HANDLE;

        Thread(void);
        virtual ~Thread(void);
        virtual int const run(void);
        void start(void);
        void stop(void);
        void state(enum Status const state);
        int const id(void) const;
        void id(int const id);
        HANDLE const handle(void) const;
        void handle(HANDLE const handle);

		 HANDLE handle_;
    private:
        int id_;
        enum Status state_;
    };
}

#endif //__THREAD_H__
