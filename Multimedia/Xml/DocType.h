#pragma once
#include "Node.h"

namespace Xml {
    class DocType : public Node {
    public:
        DocType(Node const* const parent = nullParent);
        virtual std::wstring const& name() const;
        virtual std::wstring const ToString() const;
        virtual NodeType const getNodeType() const;
    private:
        static std::wstring name_;
    };
}
