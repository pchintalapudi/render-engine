//
// Created by prem on 11/10/2018.
//
#include <sstream>
#include "include/nodes/text.h"

DOMString dom::Text::getWholeText() const {
    if (getParentNode()) {
        std::stringstream stream;
        auto children = getParentNode()->getChildNodes();
        long long idx = children.indexOf(this) - 1;
        while (--idx > -1 && children.get(idx)->getNodeType() == NodeType::TEXT_NODE);
        for (; ++idx < (long long) children.size() && children.get(idx)->getNodeType() == NodeType::TEXT_NODE;
               stream << children.get(idx)->getNodeValue());
        return stream.str();
    } else return getData();
}

dom::Text *dom::Text::cloneNode() {
    return new Text(getBaseURI(), getOwner(), getParentNode(), getData());
}

bool dom::Text::isEqualNode(const dom::Node *other) const {
    return other->getNodeType() == NodeType::TEXT_NODE && other->getNodeValue() == getNodeValue();
}