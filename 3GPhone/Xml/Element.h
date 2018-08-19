#pragma once
#include "Node.h"
#include "Attr.h"

namespace Xml {
    class Element :
        public Node
    {
    public:
        Element(Node const* const parent = nullParent);
        ~Element();
        Node const& operator[](std::wstring const& name) const;
        void appendAttr(Attr* const attr);
    private:
        std::vector<Util::shared_ptr<Attr> > attributes_;
    };
}
