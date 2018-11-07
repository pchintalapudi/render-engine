//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
#define FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H

#include "ielement.h"

class INonDocumentTypeChildNode {
public:
    const virtual IElement *previousElementSibling() = 0;

    const virtual IElement *nextElementSibling() = 0;

    virtual ~INonDocumentTypeChildNode() = default;
};

#endif //FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
