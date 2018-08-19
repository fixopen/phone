#ifndef __FSM_H__
#define __FSM_H__

#pragma warning(disable: 4786)

#include <map>

namespace Util
{
    class FSM {
    public:
        typedef void (*EventHandle)(void* param);
        void registerRule(int const startState, int const event, int const stopState, EventHandle const action = 0);
        void fireEvent(int const event, void* const param);
        void setStartState(int const startState);
        int const getCurrentState(void) const;
    private:
        int currentState_;
        std::map<int, std::map<int, std::pair<int, EventHandle> > > rules_;
    };
}

#endif //__FSM_H__