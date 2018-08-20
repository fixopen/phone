#include "Node.h"

namespace Xml {
    Node const* const Node::nullParent = 0;

    Node::Node(Node const* const parent) : parent_(const_cast<Node*>(parent)) {
    }

    Node::~Node() {
        for (size_t i = 0; i < children_.size(); ++i) {
            delete children_[i];
        }
    }

    bool const Node::hasChild(std::wstring const& name) const {
        return hasChild(name, 0);
    }

    bool const Node::hasChild(std::wstring const& name, int const index) const {
        int pos = 0;
        bool result = false;

        for (size_t i = 0; i < children_.size(); ++i) {
            if (children_[i]->name() == name) {
                if (pos == index) {
                    result = true;
                    break;
                }

                ++pos;
            }
        }

        return result;
    }

    Node const& Node::operator[](std::wstring const& name) const {
        return operator()(name, 0);
    }

    Node const& Node::operator()(std::wstring const& name, int const index) const {
        int pos = 0;
        Node* result = 0;

        for (size_t i = 0; i < children_.size(); ++i) {
            if (children_[i]->name() == name) {
                if (pos == index) {
                    result = children_[i];
                    break;
                }

                ++pos;
            }
        }

        return *result;
    }

    std::wstring const Node::ToString() const {
        return value();
    }

    std::wstring const& Node::name() const {
        return nodeName_;
    }

    void Node::name(std::wstring const& name) {
        nodeName_ = name;
    }

    std::wstring const& Node::value() const {
        return nodeValue_;
    }

    void Node::value(std::wstring const& value) {
        nodeValue_ = value;
    }

    Node::NodeType const Node::getNodeType() const {
        return UNKNOWN_NODE;
    }

    Node* const Node::Parent() const {
        return parent_;
    }

    void Node::appendChild(Node* const child) {
        child->parent_ = this;
        children_.push_back(child); //Util::shared_ptr<Node>()
    }

    void Node::removeChild(Node const* const child) {
        for (std::vector<Node*>::iterator i = children_.begin(); i != children_.end(); ++i) {
            if (*i == child) {
                children_.erase(i);
                break;
            }
        }
    }
}
