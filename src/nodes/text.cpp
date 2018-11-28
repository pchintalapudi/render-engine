//
// Created by prem on 11/10/2018.
//
#include <sstream>
#include "include/nodes/text.h"

DOMString dom::Text::getWholeText() const {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        unsigned long idx = ~0;
        for (unsigned long i = 0; i < children.size(); i++) {
            auto child = children.get(i);
            if (!~idx && child->getNodeType() == NodeType::TEXT_NODE) idx = i;
            if (child == this) break;
        }
        DOMString str;
        for (; idx < children.size(); idx++) str += *children.get(idx)->getNodeValue();
        return str;
    } else return getData();
}

dom::Text *dom::Text::cloneNode() {
    return new Text(getBaseURI(), getOwner(), getParentNode(), getData());
}

bool dom::Text::isEqualNode(const dom::Node *other) const {
    return other->getNodeType() == NodeType::TEXT_NODE && other->getNodeValue() == getNodeValue();
}