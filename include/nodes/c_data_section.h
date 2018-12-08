//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_C_DATA_SECTION_H
#define FEATHER_C_DATA_SECTION_H

#include "text.h"

namespace dom {
    class CDataSection;
}

class dom::CDataSection : public Text {
public:
    CDataSection(DOMString baseURI, Document *owner, Node *parent)
            : Text(baseURI, "#cdata-section", NodeType::CDATA_SECTION_NODE, parent) {}

    explicit CDataSection(Node &parent) : Text("#cdata-section", NodeType::CDATA_SECTION_NODE, parent) {}

    CDataSection(DOMString baseURI, Document *owner, Node *parent, DOMString text)
            : CDataSection(baseURI, owner, parent) { setData(text); }

    CDataSection(Node &parent, DOMString text) : CDataSection(parent) { setData(text); }
};

#endif //FEATHER_C_DATA_SECTION_H
