//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_PROCESSING_INSTRUCTION_H
#define FEATHER_PROCESSING_INSTRUCTION_H

#include "character_data.h"

namespace dom {
    class ProcessingInstruction;
}

class dom::ProcessingInstruction : public CharacterData {
public:

    ProcessingInstruction(DOMString baseURI, Document *owner, Node *parent, DOMString target)
            : CharacterData(baseURI, target, NodeType::PROCESSING_INSTRUCTION_NODE, owner, parent), target(target) {}

    ProcessingInstruction(Node &parent, DOMString target)
            : CharacterData(target, NodeType::PROCESSING_INSTRUCTION_NODE, parent), target(target) {}

    inline DOMString getTarget() const { return target; }

private:
    const DOMString target;
};

#endif //FEATHER_PROCESSING_INSTRUCTION_H
