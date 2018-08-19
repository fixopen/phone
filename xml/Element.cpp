#include "Element.h"

namespace Xml {
    Element::Element(Node const* const parent) 
        : Node(parent) {
    }

    Element::~Element() {
        for (size_t i = 0; i < attributes_.size(); ++i) {
            delete attributes_[i];
        }
    }

    Node const& Element::operator[](std::wstring const& name) const {
        return operator()(name, 0);
    }

    Node const& Element::operator()(std::wstring const& name, int const index) const {
        Node* result = 0;
        if (name[0] == L'@') {
            for (size_t i = 0; i < attributes_.size(); ++i) {
                if (attributes_[i]->name() == name.substr(1)) {
                    result = attributes_[i];
                    break;
                }
            }
        } else {
            result = const_cast<Node*>(&Node::operator ()(name, index));
        }
        return *result;
    }

    void Element::appendAttr(Attr* const attr) {
        attributes_.push_back(attr);
    }

    bool const Element::hasAttr(std::wstring const& attrName) const {
        bool result = false;
        for (size_t i = 0; i < attributes_.size(); ++i) {
            if (attributes_[i]->name() == attrName) {
                result = true;
                break;
            }
        }
        return result;
    }

    std::wstring const Element::ToString() const {
        std::wstring result = L"<" + name();
        for (size_t i = 0; i < attributes_.size(); ++i) {
            result += L" " + attributes_[i]->ToString();
        }
        if (children_.empty()) {
            result += L"/>";
        } else {
            for (size_t i = 0; i < children_.size(); ++i) {
                result += children_[i]->ToString();
            }
            result += L"</" + name() + L">";
        }
        return result;
    }

    Node::NodeType const Element::getNodeType() const {
        return ELEMENT_NODE;
    }
}
