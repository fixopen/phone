#include "DataSection.h"

namespace Xml {
    std::wstring CDataSection::name_ = L"CDATA";

    CDataSection::CDataSection(Node const* const parent) : Node(parent) {
    }

    std::wstring const& CDataSection::name() const {
        return CDataSection::name_;
    }

    std::wstring const CDataSection::ToString() const {
        return L"<![CDATA[" + value() + L"]]>";
    }

    Node::NodeType const CDataSection::getNodeType() const {
        return CDATA_SECTION_NODE;
    }
}
