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
            inline DOMString getRealTarget() { return getData(); }

            inline void setRealTarget(DOMString target) { setData(target); }

            inline DOMString getRealData() { return data; }

            inline void setRealData(DOMString data) { this->data = data; }

        private:
            DOMString data = "";
        };
    }
}
#endif //FEATHER_PROCESSING_INSTRUCTION_H
