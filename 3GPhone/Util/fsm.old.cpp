#include "fsm.h"

using std::map;

namespace Util {
    void FSM::registerRule(int const startState, int const event, int const stopState, EventHandle const action) {
        rules_[startState][event] = std::make_pair(stopState, action);
    }

    void FSM::fireEvent(int const event, void* const param) {
		std::map<int, std::map<int, std::pair<int, EventHandle> > >::iterator i = rules_.find(currentState_);
        if (i != rules_.end()) {//state machine is ok
			std::map<int, std::pair<int, EventHandle> >::iterator j = i->second.find(event);
            if (j != i->second.end()) {//has valid jump rule
				std::pair<int, EventHandle> jumpTable = j->second;
				currentState_ = jumpTable.first;
				if (jumpTable.second)
					(jumpTable.second)(param);
			}
		}
    }

    void FSM::setStartState(int const startState) {
        currentState_ = startState;
    }

    int const FSM::getCurrentState(void) const {
        return currentState_;
    }
}
