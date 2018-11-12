//
// Created by prem on 11/10/2018.
//
#include <sstream>
#include "include/nodes/text.h"

DOMString Text::getWholeText() const {
    if (getParentNode()) {
        std::stringstream stream;
        auto children = getParentNode()->getChildNodes();
        auto idx = children.indexOf(this) - 1;
        while (--idx > -1 && children.get(idx)->getNodeType() == NodeType::TEXT_NODE);
        for (; ++idx < children.size() && children.get(idx)->getNodeType() == NodeType::TEXT_NODE;
               stream << children.get(idx)->getNodeValue());
        return stream.str();
    } else return getData();
}

Text *Text::cloneNode() {
    return new Text(getBaseURI(), getOwner(), getParentNode(), getData());
}

bool Text::isEqualNode(const Node *other) const {
    return other->getNodeType() == NodeType::TEXT_NODE && other->getNodeValue() == getNodeValue();
}