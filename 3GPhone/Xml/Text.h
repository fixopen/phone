#pragma once
#include "Node.h"

namespace Xml {
    class Text :
        public Node
    {
    public:
        Text(Node const* const parent = nullParent);
        ~Text();
    };
}
