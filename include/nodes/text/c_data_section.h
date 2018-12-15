//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_C_DATA_SECTION_H
#define FEATHER_C_DATA_SECTION_H

#include "text.h"

namespace feather {
    namespace dom {
        class CDataSection : public Text {
            CDataSection(DOMString baseURI, StrongPointer<Node> parent)
                    : Text(baseURI, "#cdata-section", NodeType::CDATA_SECTION_NODE, parent) {}

            CDataSection(DOMString baseURI, StrongPointer<Node> parent, DOMString init)
                    : Text(baseURI, "#cdata-section", NodeType::CDATA_SECTION_NODE, parent, init) {}
        };
    }
}
#endif //FEATHER_C_DATA_SECTION_H
