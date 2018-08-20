#ifndef __UTIL_FSM_H__
#define __UTIL_FSM_H__

#include <map>
#include <vector>

#include "Log.h"
#include "Text/StringOp.h"

namespace Util {
    template<typename T>
    class FSM {
    public:
        typedef void (T::*EventHandle)(void* param);

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
            std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > >::iterator i = rules_.find(currentState_);
		    if (i != rules_.end()) { //state machine is ok
				Log::Log(L"currentState is " + Text::StringOp::FromInt(currentState_) + L", event is " + Text::StringOp::FromInt(event));
			    std::map<int, std::pair<int, std::pair<T* const, EventHandle> > >::iterator j = i->second.find(event);
			    if (j != i->second.end()) { //has valid jump rule
				    std::pair<int, std::pair<T*, EventHandle> > jumpTable = j->second;
				    currentState_ = jumpTable.first;
				    if (jumpTable.second.second) { //has action
					    (jumpTable.second.first->*jumpTable.second.second)(param);
				    }
			    }
		    }
        }

        void setStartState(int const startState) {
            currentState_ = startState;
        }

        void setEndStates(std::vector<int>& endStates) {
            endStates_ = endStates;
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

        int const getCurrentState() const {
            return currentState_;
        }
    private:
        int currentState_;
        std::vector<int> endStates_;
        std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > > rules_;
    };
}
#if 0
		//FSM() : child_(0), parent_(0), errorState_(-1) {}
		    if (child_) {
			    child_->fireEvent(event, param);
		    } else {
		    }
        //void addActiveChildRule(int const state, FSM* child) {
        //    children_.insert(std::make_pair(state, child));
        //}

        //void setEndNotify(std::vector<int> const& endStates, EventHandle const endHandle = 0) {
        //    endStates_ = endStates;
        //    notifyEnd_ = endHandle;
        //}

        //void setErrorNotify(int const errorState, EventHandle const errorHandle = 0) {
        //    errorState_ = errorState;
        //    notifyError_ = errorHandle;
        //}

        //void activeChild(FSM* const child) {
        //    currentChild_ = child;
        //    currentChild_->parent_ = this;
        //}

        //void clearChild() {
        //    currentChild_->parent_ = 0;
        //    currentChild_ = 0;
        //}

                        //if (currentState_ in children_) {
                        //    activeChild(*children_[currentState_]);
                        //}
					    //if (isEnd()) {
						   // if (parent_ && parent_->notifyEnd_) {
							  //  //if (notifyEnd_) {
								 //  // //notifyEnd_(param);
							  //  //}
						   // }
					    //}
					    //if (currentState_ == errorState_) {
						   // if (parent_) {
							  //  if (notifyError_) {
								 //   //notifyError_(param);
							  //  }
						   // }
					    //}
		//std::vector<int> endStates_;
        //FSM* parent_;
        //FSM* currentChild_;
        //std::map<int, FSM*> children_;
		//int errorState_;
		//EventHandle notifyEnd_;
		//EventHandle notifyError_;
#endif
#endif //__UTIL_FSM_H__