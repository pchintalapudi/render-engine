//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_CHILD_NODE_H
#define FEATHER_CHILD_NODE_H

#include <vector>
#include "node.h"

class IChildNode {
public:
    virtual void remove() = 0;

    virtual void before(std::vector<Node *> toInsert) = 0;

    virtual void after(std::vector<Node *> toInsert) = 0;

    virtual void replaceWith(std::vector<Node *> toInsert) = 0;

    virtual ~IChildNode() {};
};

#endif //FEATHER_CHILD_NODE_H
