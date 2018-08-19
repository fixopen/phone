#pragma once
#include "Node.h"

namespace Xml {
    class ProcessingInstruction : public Node {
    public:
        ProcessingInstruction(Node const * const parent = nullParent);
        virtual std::wstring const ToString() const;
        virtual NodeType const getNodeType() const;
    };
}
