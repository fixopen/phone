#ifndef __UTIL_OBSERVER_H__
#define __UTIL_OBSERVER_H__

#include <string> //for std::string
#include <vector> //for std::vector
#include "Event.h" //for Event

namespace Util {
    class Observable;
    class Observer {
    public:
        virtual ~Observer() = 0 {}
        virtual void Update(Event const& evt) = 0;
        inline void Observe(Observable* const observable) const;
    };
    class Observable {
    public:
        virtual ~Observable() {}
        void RegisterObserver(Observer* const ob) {
            observers_.push_back(ob);
        }
        void Notify(Event const& evt) {
            for (size_t i = 0, e = observers_.size(); i < e; ++i) {
                observers_[i]->Update(evt);
            }
        }
    private:
        std::vector<Observer* const> observers_;
    };
    void Observer::Observe(Observable* const observable) const {
        observable->RegisterObserver(const_cast<Observer * const>(this));
    }
}

#endif //__UTIL_OBSERVER_H__
