//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_PARENT_NODE_H
#define FEATHER_PARENT_NODE_H

#include "include/utils/html_collection.h"
#include "../node.h"

namespace dom {
    class Element;

    class ParentNode;
}

class dom::ParentNode {
    unsigned long getChildElementCount() const;

    virtual HTMLCollection &getChildren() = 0;

    virtual const HTMLCollection &getChildren() const = 0;

    Element *getFirstElementChild();

    Element *getLastElementChild();

    virtual void append(std::vector<Node *> children) = 0;

    virtual void prepend(std::vector<Node *> children) = 0;

    virtual Element *querySelector(DOMString selector) = 0;

    virtual NodeList &querySelectorAll(DOMString selector) = 0;
};

#endif //FEATHER_PARENT_NODE_H
