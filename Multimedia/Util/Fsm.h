#ifndef __UTIL_FSM_H__
#define __UTIL_FSM_H__

#include <map> //for std::map, std::pair
#include <vector> //for std::vector
#include <set> //for std::set

namespace Util {
    template<typename T>
    class Dfa {
    public:
        typedef void (T::*EventHandle)(void* param);

        virtual ~Dfa() {}

        void RegisterRule(int const startState, int const event, int const stopState, EventHandle const action = 0) {
            std::pair<T* const, EventHandle> process = std::make_pair(static_cast<T * const>(this), action); //WinCE not support dynamic_cast, use static_cast instead it
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
                std::map<int, std::pair<int, std::pair<T * const, EventHandle> > > firstEventRules;
                firstEventRules.insert(std::make_pair(event, jumpTable));
                rules_.insert(std::make_pair(startState, firstEventRules));
            }

            //rules_[startState][event] = jumpTable;
        }

        void FireEvent(int const event, void* const param) {
            std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > >::iterator i = rules_.find(currentState_);

            if (i != rules_.end()) { //state machine is ok
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

        void SetStartState(int const startState) {
            currentState_ = startState;
        }

        void SetEndStates(std::vector<int>& endStates) {
            endStates_ = endStates;
        }

        bool const IsEnd() const {
            bool isEndState = false;

            for (size_t i = 0, e = endStates_.size(); i < e; ++i) {
                if (endStates_[i] == currentState_) {
                    isEndState = true;
                    break;
                }
            }

            return isEndState;
        }

        int const GetCurrentState() const {
            return currentState_;
        }
    private:
        int currentState_;
        std::vector<int> endStates_;
        std::map<int, std::map<int, std::pair<int, std::pair<T* const, EventHandle> > > > rules_;
    };

    template<typename T>
    class Nfa {
    public:
        typedef void (T::*EventHandle)(void* param);

        virtual ~Nfa() {}

        void RegisterRule(int const startState, int const event, std::map<int, EventHandle> const& stopStateAndAction) {
            std::map<int, std::pair<T * const, EventHandle> > jumpTables;

            for (std::map<int, EventHandle>::iterator i = stopStateAndAction.begin(), e = stopStateAndAction.end(); i != e; ++i) {
                std::pair<T* const, EventHandle> process = std::make_pair(static_cast<T * const>(this), i->second); //WinCE not support dynamic_cast, use static_cast instead it
                std::pair<int, std::pair<T* const, EventHandle> > jumpTable = std::make_pair(stopState, process);
                jumpTables.insert(jumpTable);
            }

            std::map<int, std::map<int, std::map<int, std::pair<T* const, EventHandle> > > >::iterator i = rules_.find(startState);

            if (i != rules_.end()) {
                std::map<int, std::map<int, std::pair<T* const, EventHandle> > >::iterator j = i->second.find(event);

                if (j != i->second.end()) {
                    //error
                } else {
                    i->second.insert(std::make_pair(event, jumpTables));
                }
            } else {
                std::map<int, std::map<int, std::pair<T * const, EventHandle> > > firstEventRules;
                firstEventRules.insert(std::make_pair(event, jumpTables));
                rules_.insert(std::make_pair(startState, firstEventRules));
            }

            //rules_[startState][event] = jumpTable;
        }

        void FireEvent(int const event, void* const param) {
            std::set<int> states;

            for (size_t csi = 0, csc = currentStates_.size(); csi < csc; ++csi) {
                std::map<int, std::map<int, std::map<int, std::pair<T* const, EventHandle> > > >::iterator i = rules_.find(currentStates_[csi]);

                if (i != rules_.end()) { //state machine is ok
                    std::map<int, std::map<int, std::pair<T* const, EventHandle> > >::iterator j = i->second.find(event);

                    if (j != i->second.end()) { //has valid jump rule
                        std::map<int, std::pair<T*, EventHandle> > jumpTables = j->second;

                        for (std::map<int, std::pair<T*, EventHandle> >::iterator k = jumpTables.begin(), e = jumpTables.end(); k != e; ++k) {
                            states.insert(k->first);

                            if (k->second.second) { //has action
                                (k->second.first->*k->second.second)(param);
                            }
                        }
                    }
                }
            }

            currentStates_.clear();
            currentStates_.insert(currentStates_.end(), states.begin(), states.end());
        }

        void SetStartState(int const startState) {
            currentStates_.clear();
            currentStates.push_back(startState);
        }

        void SetEndStates(std::vector<int>& endStates) {
            endStates_ = endStates;
        }

        bool const IsEnd() const {
            bool result = false;

            for (size_t i = 0, ie = endStates_.size(); i < ie; ++i) {
                for (size_t j = 0, je < currentStates_.size(); j < je; ++j) {
                    if (endStates_[i] == currentStates_[j]) {
                        result = true;
                        goto Outer;
                    }
                }
            }

Outer:
            return result;
        }

        std::vector<int> const GetCurrentStates() const {
            return currentStates_;
        }
    private:
        std::vector<int> currentStates_;
        std::vector<int> endStates_;
        std::map<int, std::map<int, std::map<int, std::pair<T* const, EventHandle> > > > rules_;
    };
}

#endif //__UTIL_FSM_H__
