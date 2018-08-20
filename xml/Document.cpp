#include "Document.h"

namespace Xml {
    Document::Document()
        : Node(nullParent) {
    }

    Document::~Document() {
    }

    //void Document::SetRootElement(Element* const rootElement) {
    //    appendChild(rootElement);
    //    rootElement_ = rootElement;
    //}

    void Document::version(std::wstring const& version) {
        version_ = version;
    }

    void Document::encoding(std::wstring const& encoding) {
        encoding_ = encoding;
    }

    void Document::standalone(bool const standalone) {
        standalone_ = standalone;
    }

    std::wstring const Document::ToString() const {
        std::wstring result = L"<?xml version=\"" + version_ + L"\" encoding=\"" + (encoding_ == L"" ? L"UTF-8" : encoding_) + L"\"?>\n";
        for (size_t i = 0; i < children_.size(); ++i) {
            result += children_[i]->ToString();
        }
        return result;
    }

    Node::NodeType const Document::getNodeType() const {
        return DOCUMENT_NODE;
    }
}
