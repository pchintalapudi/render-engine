//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_HTML_ELEMENT_H
#define FEATHER_HTML_ELEMENT_H

#include "utils/named_node_map.h"
#include "node.h"

class Element : public Node {
public:
    virtual const NamedNodeMap attributes() = 0;

    virtual ~Element() {};
};

#endif //FEATHER_HTML_ELEMENT_H
