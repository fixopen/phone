#ifndef __UTIL_CONTAINERCOMPONENT_H__
#define __UTIL_CONTAINERCOMPONENT_H__

#pragma once

#include "Component.h"

namespace Util {
    class ContainerComponent : public Component {
    public:
        ContainerComponent();
        virtual ~ContainerComponent();
    private:
        ContainerComponent* parent_;
        std::vector<Component*> children_;
    };
}

#endif //__UTIL_CONTAINERCOMPONENT_H__
