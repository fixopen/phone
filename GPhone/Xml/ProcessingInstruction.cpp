#include "ProcessingInstruction.h"

namespace Xml {
    ProcessingInstruction::ProcessingInstruction(Node const * const parent)
    : Node(parent) {
    }

    std::wstring const ProcessingInstruction::ToString() const {
        return L"<?" + name() + L" " + value() + L"?>";
    }

    Node::NodeType const ProcessingInstruction::getNodeType() const {
        return PROCESSING_INSTRUCTION_NODE;
    }
}
