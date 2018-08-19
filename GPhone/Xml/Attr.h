#pragma once
#include "Node.h"

namespace Xml {
    class Attr : public Node {
    public:
        Attr(Node const * const parent = nullParent);
        virtual std::wstring const ToString() const;
        virtual NodeType const getNodeType() const;
    };
}
