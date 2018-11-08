//
// Created by prem on 11/8/2018.
//

#ifndef FEATHER_C_DATA_SECTION_H
#define FEATHER_C_DATA_SECTION_H

#include "text.h"

class CDataSection : public Text {
public:
    CDataSection(std::string baseURI, Document *owner, Node *parent) : Text(baseURI, owner, parent) {}

    CDataSection(Document *owner, Node &parent) : Text(owner, parent) {}

    CDataSection(std::string baseURI, Document *owner, Node *parent, std::string data) : Text(baseURI, owner, parent,
                                                                                              data) {}

    CDataSection(Document *owner, Node &parent, std::string data) : Text(owner, parent, data) {}
};

#endif //FEATHER_C_DATA_SECTION_H
