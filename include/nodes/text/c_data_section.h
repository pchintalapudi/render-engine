//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_C_DATA_SECTION_H
#define FEATHER_C_DATA_SECTION_H

#include "text.h"

namespace feather {
    namespace dom {
        class CDATASection : public Text {
            CDATASection(DOMString baseURI, const StrongPointer<Node> &parent)
                    : Text(std::move(baseURI), "#cdata-section", NodeType::CDATA_SECTION_NODE, parent) {}

            CDATASection(DOMString baseURI, const StrongPointer<Node> &parent, DOMString init)
                    : Text(std::move(baseURI), "#cdata-section", NodeType::CDATA_SECTION_NODE, parent,
                           std::move(init)) {}
        };
    }
}
#endif //FEATHER_C_DATA_SECTION_H
