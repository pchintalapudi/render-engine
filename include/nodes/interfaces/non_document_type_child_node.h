//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
#define FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H

class Element;

class NonDocumentTypeChildNode {

    virtual Element *getPreviousElementSibling() const = 0;

    virtual Element *getNextElementSibling() const = 0;
};

#endif //FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
