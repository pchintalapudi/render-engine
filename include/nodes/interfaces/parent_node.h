//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_PARENT_NODE_H
#define FEATHER_PARENT_NODE_H

#include "../../utils/html_collection.h"
#include "../node.h"

namespace dom {
    class Element;
    class ParentNode;
}

class dom::ParentNode {
    unsigned long getChildElementCount() const { return getChildren().getLength(); }

    virtual HTMLCollection &getChildren() = 0;

    virtual HTMLCollection &getChildren() const = 0;

    Element *getFirstElementChild() { return getChildElementCount() ? getChildren().getItem(0) : nullptr; }

    Element *getLastElementChild() {
        return getChildElementCount() ? getChildren().getItem(getChildElementCount() - 1) : nullptr;
    }

    virtual void append(std::vector<Node *> children) = 0;

    virtual void prepend(std::vector<Node *> children) = 0;

    virtual Element *querySelector(DOMString selector);
};

#endif //FEATHER_PARENT_NODE_H
