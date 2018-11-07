//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_HTML_ELEMENT_H
#define FEATHER_HTML_ELEMENT_H

#include "utils/named_node_map.h"
#include "inode.h"

class IElement : INode {
public:
    virtual const NamedNodeMap attributes() = 0;

    virtual ~IElement() {};
};

#endif //FEATHER_HTML_ELEMENT_H
