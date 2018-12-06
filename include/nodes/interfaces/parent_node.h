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
    inline unsigned long getChildElementCount() const { return getChildren().size(); }

    virtual HTMLCollection &getChildren() = 0;

    virtual const HTMLCollection &getChildren() const = 0;

    inline Element *getFirstElementChild() { return getChildren().get(0); }

    inline Element *getLastElementChild() { return getChildren().get(getChildElementCount() - 1); }

    virtual void append(std::vector<Node *> &children) = 0;

    virtual void prepend(std::vector<Node *> &children) = 0;

    virtual Element *querySelector(DOMString selector) const = 0;

    virtual std::vector<Element *> querySelectorAll(DOMString selector) const = 0;
};

#endif //FEATHER_PARENT_NODE_H
