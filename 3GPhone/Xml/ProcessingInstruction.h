#pragma once
#include "Node.h"

namespace Xml {
    class ProcessingInstruction :
        public Node
    {
    public:
        ProcessingInstruction(Node const* const parent = nullParent);
        ~ProcessingInstruction();
    };
}
