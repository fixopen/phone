#pragma once
#include "Node.h"

namespace Xml {
    class Comment :
        public Node
    {
    public:
        Comment(Node const* const parent = nullParent);
        ~Comment();
    };
}
