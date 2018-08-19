#pragma once
#include "Node.h"
#include "Attr.h"

namespace Xml {
    class Element : public Node {
    public:
        Element(Node const* const parent = nullParent);
        ~Element();
        Node const& operator[](std::wstring const& name) const;
        Node const& operator()(std::wstring const& name, int const index = 0) const;
        void appendAttr(Attr* const attr);
        bool const hasAttr(std::wstring const& attrName) const;
        virtual std::wstring const ToString() const;
        virtual NodeType const getNodeType() const;
    private:
        std::vector<Attr*> attributes_;
    };
}
