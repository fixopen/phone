#ifndef __FSM_H__
#define __FSM_H__

#include <map>
#include <vector>

namespace Util {
    template<typename T>
    class FSM {
    public:
        typedef void (T::*EventHandle)(void* param);

		FSM() : child_(0), parent_(0), errorState_(-1) {}
        virtual ~FSM() {}

        void registerRule(int const startState, int const event, int const stopState, EventHandle const action = 0) {
            std::pair<T* const, EventHandle> process = std::make_pair(static_cast<T* const>(this), action); //WinCE not support dynamic_cast, use static_cast instead of it
            std::pair<int, std::pair<T* const, EventHandle> > jumpTable = std::make_pair(stopState, process);
            std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > >::iterator i = rules_.find(startState);
            if (i != rules_.end()) {
                std::map<int, std::pair<int, std::pair<T* const, EventHandle> > >::iterator j = i->second.find(event);
                if (j != i->second.end()) {
                    //error
                } else {
                    i->second.insert(std::make_pair(event, jumpTable));
                }
            } else {
                std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > firstEventRules;
                firstEventRules.insert(std::make_pair(event, jumpTable));
                rules_.insert(std::make_pair(startState, firstEventRules));
            }
            //rules_[startState][event] = jumpTable;
        }

        void fireEvent(int const event, void* const param) {
		    if (child_) {
			    child_->fireEvent(event, param);
		    } else {
                std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > >::iterator i = rules_.find(currentState_);
			    if (i != rules_.end()) {//state machine is ok
				    std::map<int, std::pair<int, std::pair<T* const, EventHandle> > >::iterator j = i->second.find(event);
				    if (j != i->second.end()) {//has valid jump rule
					    std::pair<int, std::pair<T*, EventHandle> > jumpTable = j->second;
					    if (jumpTable.second.second) {
						    (jumpTable.second.first->*jumpTable.second.second)(param);
					    }
					    if (isEnd()) {
						    if (parent_) {
							    if (notifyEnd_) {
								    //notifyEnd_(param);
							    }
						    }
					    }
					    if (currentState_ == errorState_) {
						    if (parent_) {
							    if (notifyError_) {
								    //notifyError_(param);
							    }
						    }
					    }
					    currentState_ = jumpTable.first;
				    }
			    }
		    }
        }

        void setStartState(int const startState) {
            currentState_ = startState;
        }

		bool const isEnd() const {
		    bool isEndState = false;
		    for (size_t i = 0; i < endStates_.size(); ++i) {
			    if (endStates_[i] == currentState_) {
				    isEndState = true;
				    break;
			    }
		    }
		    return isEndState;
	    }

		void setEndNotify(std::vector<int> const& endStates, EventHandle const endHandle = 0) {
		    endStates_ = endStates;
		    notifyEnd_ = endHandle;
	    }

		void setErrorNotify(int const errorState, EventHandle const errorHandle = 0) {
		    errorState_ = errorState;
		    notifyError_ = errorHandle;
	    }

		void activeChild(FSM* const child) {
		    child_ = child;
		    child_->parent_ = this;
	    }

		void clearChild() {
		    child_->parent_ = 0;
		    child_ = 0;
	    }

        int const getCurrentState() const {
            return currentState_;
        }
    private:
        int currentState_;
		FSM* child_;
		FSM* parent_;
		std::vector<int> endStates_;
		int errorState_;
		EventHandle notifyEnd_;
		EventHandle notifyError_;
        std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > > rules_;
    };
}

#endif //__FSM_H__