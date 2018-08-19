#include "DocType.h"

namespace Xml {
    std::wstring DocType::name_ = L"doctype";

    DocType::DocType(Node const * const parent)
    : Node(parent) {
    }

    std::wstring const & DocType::name() const {
        return DocType::name_;
    }

    std::wstring const DocType::ToString() const {
        return L"<!DOCTYPE " + value() + L">";
    }

    Node::NodeType const DocType::getNodeType() const {
        return DOCUMENT_TYPE_NODE;
    }
}
