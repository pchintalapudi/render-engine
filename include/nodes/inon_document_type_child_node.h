//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
#define FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H

#include "element.h"

class INonDocumentTypeChildNode {
public:
    virtual Element *const previousElementSibling() const = 0;

    virtual Element *const nextElementSibling() const = 0;

    virtual ~INonDocumentTypeChildNode() {};
};

#endif //FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
