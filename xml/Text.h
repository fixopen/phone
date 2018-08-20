#pragma once
#include "Node.h"

namespace Xml {
    class Text : public Node {
    public:
        Text(Node const* const parent = nullParent);
        virtual std::wstring const& name() const;
        virtual NodeType const getNodeType() const;
    private:
        static std::wstring name_;
    };
}
