#include "fsm.h"

using std::map;

namespace Util {
    //FSM::FSM() : child_(0), parent_(0), errorState_(-1) {}

    //void FSM::registerRule(int const startState, int const event, int const stopState, EventHandle const action) {
    //    rules_[startState][event] = std::make_pair(stopState, action);
    //}

    //void FSM::fireEvent(int const event, void* const param) {
	   // if (child_) {
		  //  child_->fireEvent(event, param);
	   // } else {
		  //  std::map<int, std::map<int, std::pair<int, EventHandle> > >::iterator i = rules_.find(currentState_);
		  //  if (i != rules_.end()) {//state machine is ok
			 //   std::map<int, std::pair<int, EventHandle> >::iterator j = i->second.find(event);
			 //   if (j != i->second.end()) {//has valid jump rule
				//    std::pair<int, EventHandle> jumpTable = j->second;
				//    currentState_ = jumpTable.first;
				//    if (jumpTable.second) {
				//	    (jumpTable.second)(param);
				//    }
				//    if (isEnd()) {
				//	    if (parent_) {
				//		    if (notifyEnd_) {
				//			    notifyEnd_(param);
				//		    }
				//	    }
				//    }
				//    if (currentState_ == errorState_) {
				//	    if (parent_) {
				//		    if (notifyError_) {
				//			    notifyError_(param);
				//		    }
				//	    }
				//    }
			 //   }
		  //  }
	   // }
    //}

    //void FSM::setStartState(int const startState) {
    //    currentState_ = startState;
    //}

    //bool const FSM::isEnd() const {
	   // bool isEndState = false;
	   // for (size_t i = 0; i < endStates_.size(); ++i) {
		  //  if (endStates_[i] == currentState_) {
			 //   isEndState = true;
			 //   break;
		  //  }
	   // }
	   // return isEndState;
    //}

    //void FSM::setEndNotify(std::vector<int> const& endStates, EventHandle endHandle) {
	   // endStates_ = endStates;
	   // notifyEnd_ = endHandle;
    //}

    //void FSM::setErrorNotify(int const errorState, EventHandle errorHandle) {
	   // errorState_ = errorState;
	   // notifyError_ = errorHandle;
    //}

    //void FSM::activeChild(FSM* const child) {
	   // child_ = child;
	   // child_->parent_ = this;
    //}

    //void FSM::clearChild() {
	   // child_->parent_ = 0;
	   // child_ = 0;
    //}

    //int const FSM::getCurrentState() const {
    //    return currentState_;
    //}
}
