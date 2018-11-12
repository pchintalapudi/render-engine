//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_PARENT_NODE_H
#define FEATHER_PARENT_NODE_H

#include "../../utils/html_collection.h"
#include "../node.h"

class Element;

class ParentNode {
    virtual unsigned long getChildElementCount() const = 0;

    virtual HTMLCollection &getChildren() = 0;

    virtual Element *getFirstElementChild();

    virtual Element *getLastElementChild();

    virtual void append(std::vector<Node *> children);

    virtual void prepend(std::vector<Node *> children);


};

#endif //FEATHER_PARENT_NODE_H
