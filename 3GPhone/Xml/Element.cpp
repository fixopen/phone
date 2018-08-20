#include "Element.h"

namespace Xml {
    Element::Element(Node const* const parent) 
        : Node(parent) {
    }

    Element::~Element() {
    }

    Node const& Element::operator[](std::wstring const& name) const {
        Node* result = 0;
        if (name[0] == L'@') {
            for (size_t i = 0; i < attributes_.size(); ++i) {
                if (attributes_[i]->name() == name.substr(1)) {
                    result = attributes_[i].get();
                    break;
                }
            }
        } else {
            result = const_cast<Node*>(&Node::operator [](name));
        }
        return *result;
    }

    void Element::appendAttr(Attr* const attr) {
        attributes_.push_back(Util::shared_ptr<Attr>(attr));
    }
}
