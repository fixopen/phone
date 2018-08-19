#pragma once

#include <string>
#include <vector>
#include "../Util/SmartPtr.h"
//#include "../Util/XMLParser.h"

namespace Xml {
    class Node {
        //friend class Util::XMLParser;
    public:
        enum NodeType {
            UNKNOWN_NODE = 0,
            ELEMENT_NODE = 1,
            ATTRIBUTE_NODE = 2,
            TEXT_NODE = 3,
            CDATA_SECTION_NODE = 4,
            ENTITY_REFERENCE_NODE = 5,
            ENTITY_NODE = 6,
            PROCESSING_INSTRUCTION_NODE = 7,
            COMMENT_NODE = 8,
            DOCUMENT_NODE = 9,
            DOCUMENT_TYPE_NODE = 10,
            DOCUMENT_FRAGMENT_NODE = 11,
            NOTATION_NODE = 12,
        };
        Node(Node const * const parent = nullParent);
        virtual ~Node();
        bool const hasChild(std::wstring const & name) const;
        bool const hasChild(std::wstring const & name, int const index) const;
        virtual Node const & operator[](std::wstring const & name) const;
        Node const & operator()(std::wstring const & name, int const index = 0) const;
        virtual std::wstring const ToString() const;
        virtual std::wstring const & name() const;
        void name(std::wstring const & name);
        std::wstring const & value() const;
        void value(std::wstring const & value);
        virtual NodeType const getNodeType() const;
        Node* const Parent() const;
        void appendChild(Node* const child);
        void removeChild(Node const * const child);
        //typedef void (*ChildProcess)(Node* const child);
        template <typename ChildProcess>
        void ForeachChild(ChildProcess childProcess, void* params) {
            for (size_t i = 0; i < children_.size(); ++i) {
                childProcess(*this, *children_[i], params);
            }
        }
        void* ext;
        static Node const * const nullParent;
    protected:
        std::wstring nodeName_;
        std::wstring nodeValue_;
        Node* parent_;
        std::vector<Node*> children_;
    };
}
