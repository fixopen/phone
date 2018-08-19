#pragma once
#include "Node.h"
#include "Element.h"

namespace Xml {
    class Document : public Node {
    public:
        Document();
        ~Document();
        //void SetRootElement(Element* const rootElement);
        void version(std::wstring const & version);
        void encoding(std::wstring const & encoding);
        void standalone(bool const standalone);
        virtual std::wstring const ToString() const;
        virtual NodeType const getNodeType() const;
    private:
        std::wstring version_;
        std::wstring encoding_;
        bool standalone_;
        //Element* rootElement_;
        static Util::shared_ptr<Node> nullParent_;
    private:
        static std::wstring name_;
    };
}
