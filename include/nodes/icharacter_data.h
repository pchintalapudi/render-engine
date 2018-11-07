//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_CHARACTER_DATA_H
#define FEATHER_CHARACTER_DATA_H

#include "utils/typedefs.h"
#include "inode.h"
#include "ichild_node.h"
#include "inon_document_type_child_node.h"

class ICharacterData : INode, IChildNode, INonDocumentTypeChildNode {
public:
    virtual DOMString getData() = 0;

    virtual DOMString setData(std::string data) = 0;

    const unsigned long getLength() {
        return this->getData().length();
    }

    inline void appendData(std::string toAppend) {
        this->setData(this->getData() + toAppend);
    }


};

#endif //FEATHER_CHARACTER_DATA_H
