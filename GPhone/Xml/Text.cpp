#include "Text.h"

namespace Xml {
    std::wstring Text::name_ = L"text";

    Text::Text(Node const* const parent) : Node(parent) {
    }

    std::wstring const& Text::name() const {
        return Text::name_;
    }

    Node::NodeType const Text::getNodeType() const {
        return TEXT_NODE;
    }
}
