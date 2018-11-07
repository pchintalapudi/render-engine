//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
#define FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H

#include "element.h"

class INonDocumentTypeChildNode {
public:
    const virtual Element *previousElementSibling() = 0;

    const virtual Element *nextElementSibling() = 0;

    virtual ~INonDocumentTypeChildNode() {};
};

#endif //FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
