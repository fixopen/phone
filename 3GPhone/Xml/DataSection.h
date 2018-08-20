#pragma once
#include "Node.h"

namespace Xml {
    class CDataSection :
        public Node
    {
    public:
        CDataSection(Node const* const parent = nullParent);
        ~CDataSection();
    };
}
