//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
#define FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H


class Element;
class INonDocumentTypeChildNode {
public:
    virtual Element *previousElementSibling() const = 0;

    virtual Element *nextElementSibling() const = 0;

    virtual ~INonDocumentTypeChildNode() {};
};

#endif //FEATHER_NON_DOCUMENT_TYPE_CHILD_NODE_H
