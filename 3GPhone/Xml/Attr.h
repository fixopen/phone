#pragma once
#include "Node.h"

namespace Xml {
    class Attr :
        public Node
    {
    public:
        Attr(Node const* const parent = nullParent);
        ~Attr();
    };
}
