#include "Document.h"

namespace Xml {
    Document::Document()
        : Node(nullParent) {
    }

    Document::~Document() {
    }

    void Document::SetRootElement(Element* const rootElement) {
        appendChild(rootElement);
        rootElement_ = rootElement;
    }

    void Document::version(std::wstring const& version) {
        version_ = version;
    }

    void Document::encoding(std::wstring const& encoding) {
        encoding_ = encoding;
    }

    void Document::standalone(bool const standalone) {
        standalone_ = standalone;
    }
}
