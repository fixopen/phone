#pragma once

#include <string>
#include <vector>
#include "../Util/SmartPtr.h"

namespace Xml {
    class Node
    {
        friend class xmlLoad;
    public:
        Node(Node const* const parent = nullParent);
        virtual ~Node();
        bool const hasChild(std::wstring const& name) const;
        bool const hasChild(std::wstring const& name, int const index) const;
        virtual Node const& operator[](std::wstring const& name) const;
        Node const& operator()(std::wstring const& name, int const index) const;
        virtual std::wstring const ToString() const;
        std::wstring const& name() const;
        void name(std::wstring const& name);
        std::wstring const& value() const;
        void value(std::wstring const& value);
        Node* const Parent() const;
        void appendChild(Node* const child);
        void removeChild(Node const* const child);
        void* ext;
        static Node const* const nullParent;
    private:
        std::wstring nodeName_;
        std::wstring nodeValue_;
        Node* parent_;
        std::vector<Util::shared_ptr<Node> > children_;
    };
}
