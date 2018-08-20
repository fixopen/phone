#include "Comment.h"

namespace Xml {
    std::wstring Comment::name_ = L"comment";

    Comment::Comment(Node const* const parent) : Node(parent) {
    }

    std::wstring const& Comment::name() const {
        return Comment::name_;
    }

    std::wstring const Comment::ToString() const {
        return L"<!--" + value() + L"-->";
    }

    Node::NodeType const Comment::getNodeType() const {
        return COMMENT_NODE;
    }
}
