#ifndef __UTIL_COMPONENT_H__
#define __UTIL_COMPONENT_H__

#pragma once

#include "Observer.h" //for Observable, Observer
//#include "Event.h" //for Event

namespace Util {
    class Event;
    class Component : public Observable, public Observer {
    public:
        Component();
        virtual ~Component();
        virtual void Update(Event const& evt) {
        }
        void Fire(Event const* const evt);
    };
}

#endif //__UTIL_COMPONENT_H__
