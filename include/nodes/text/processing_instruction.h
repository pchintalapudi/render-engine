//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_PROCESSING_INSTRUCTION_H
#define FEATHER_PROCESSING_INSTRUCTION_H

#include "character_data.h"

namespace feather {
    namespace dom {
        class ProcessingInstruction : public CharacterData {
        public:

            ProcessingInstruction(DOMString baseURI, const DOMString &target, const StrongPointer<Node> &parent)
                    : CharacterData(std::move(baseURI), target, NodeType::PROCESSING_INSTRUCTION_NODE, parent,
                                    target) {}

            ProcessingInstruction(DOMString baseURI, const DOMString &target, const StrongPointer<Node> &parent,
                                  DOMString val)
                    : CharacterData(std::move(baseURI), target, NodeType::PROCESSING_INSTRUCTION_NODE, parent, target) {
                setRealData(std::move(val));
            }

            inline DOMString getRealTarget() { return getData(); }

            inline void setRealTarget(DOMString target) { setData(std::move(target)); }

            inline DOMString getRealData() { return data; }

            inline void setRealData(DOMString data) { this->data = std::move(data); }

        private:
            DOMString data = "";
        };
    }
}
#endif //FEATHER_PROCESSING_INSTRUCTION_H
