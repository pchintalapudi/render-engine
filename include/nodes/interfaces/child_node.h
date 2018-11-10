//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_CHILD_NODE_H
#define FEATHER_CHILD_NODE_H

#include <vector>
#include "../node.h"

class ChildNode {
    virtual void remove() = 0;

    virtual void before(std::vector<Node *> &nodes) = 0;

    virtual void before(Node *node);

    virtual void after(std::vector<Node *> &nodes) = 0;

    virtual void after(Node *node);

    virtual void replaceWith(std::vector<Node *> &nodes) = 0;

    virtual void replaceWith(Node *nodes);
};

#endif //FEATHER_CHILD_NODE_H
