#include "Attr.h"

namespace Xml {
    Attr::Attr(Node const * const parent)
    : Node(parent) {
    }

    std::wstring const Attr::ToString() const {
        return name() + L"=\"" + value() + L"\"";
    }

    Node::NodeType const Attr::getNodeType() const {
        return ATTRIBUTE_NODE;
    }
}
