#pragma once
#include "Node.h"

namespace Xml {
    class DocType :
        public Node
    {
    public:
        DocType(Node const* const parent = nullParent);
        ~DocType();
    };
}
