#include "thread.h"

#include <winbase.h> //for CreateThread TerminateThread

namespace Util {
    Thread::Thread()
        : handle_(0)
        , id_(0)
        , state_(tsNull) {
    }

    Thread::~Thread() {
    }

    int const Thread::run() {
        return 0;
    }

    static unsigned int __stdcall threadEntry_(void* param) {
        Thread* thread = (Thread*)param;
        thread->state(Thread::tsRunning);
        thread->run();
        //::ExitThread(0);
        thread->state(Thread::tsExit);
        return 0;
    }

    void Thread::start(int const pri) {
		//::AfxBeginThread(threadEntry_, this, priority, stackSize, createFlags, securityAttributes);

		//unsigned long AFXAPI _beginthreadex(void *security, unsigned stack_size, unsigned (__stdcall *start_address)(void *), void *arglist, unsigned initflag, unsigned *thrdaddr);
		//handle_ = (HANDLE)wce__beginthreadex(0, 0, threadEntry_, this, CREATE_SUSPENDED, (UINT*)&id_);
		//handle_ = (HANDLE)_beginthreadex(0, 0, threadEntry_, this, CREATE_SUSPENDED, (UINT*)&id_);
		//uintptr_t _beginthread(void(__cdecl *start_address)(void*), unsigned stack_size, void *arglist);
		//uintptr_t _beginthreadex(void *security, unsigned stack_size, unsigned (__stdcall *start_address)(void*), void *arglist, unsigned initflag, unsigned *thrdaddr);

        handle_ = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadEntry_, this, 0, (unsigned long*)&id_); //CREATE_SUSPENDED
        //::CreateThread(threadAttributes, stackSize, (LPTHREAD_START_ROUTINE)threadEntry_, this, creationFlags, threadId);
        //BOOL WINAPI SetThreadPriority(__in HANDLE hThread, __in int nPriority);
        ::SetThreadPriority(handle_, pri);
        state_ = tsCreated;
    }

    void Thread::stop() {
        if (state_ != tsExit) {
            ::TerminateThread(handle_, 0);
            //::CloseHandle(handle_);
        }
        delete this;
    }

    void Thread::state(enum Status const state) {
        state_ = state;
    }

    int const Thread::id() const {
        return id_;
    }

    void Thread::id(int const id) {
        id_ = id;
    }

    HANDLE const Thread::handle() const {
        return handle_;
    }

    void Thread::handle(HANDLE const handle) {
        handle_ = handle;
    }
}
