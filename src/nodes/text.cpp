//
// Created by prem on 11/10/2018.
//
#include <sstream>
#include "include/nodes/text.h"

DOMString Text::getWholeText() const {
    if (getParentNode()) {
        std::stringstream stream;
        auto children = getParentNode()->getChildNodes();
        auto idx = std::find(children.begin(), children.end(), this);
        unsigned long i = idx - children.begin() - 1;
        while (--i > -1 && children[i]->getNodeType() == NodeType::TEXT_NODE) {}
        while (++i < children.size() && children[i]->getNodeType() == NodeType::TEXT_NODE) {
            stream << children[i]->getNodeValue();
        }
        return stream.str();
    } else return getData();
}