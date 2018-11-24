//
// Created by prem on 11/12/2018.
//

#include "include/nodes/node.h"
#include "include/nodes/element.h"
#include "include/utils/html_collection.h"

std::vector<dom::Element *> dom::HTMLCollection::_get_cacheable_element_vector() const {
    std::vector<Element *> elements;
    for (unsigned long i = 0; i < watched.size(); i++) {
        Node *node = watched.get(i);
        if (node->getNodeType() == NodeType::ELEMENT_NODE) {
            elements.push_back(static_cast<Element *>(node));
        }
    }
    return elements;
}

std::vector<dom::Element *> dom::HTMLCollection::getElementVector() const {
    if (checksum == watched.getChecksum()) return cached;
    cached = _get_cacheable_element_vector();
    checksum = watched.getChecksum();
    return cached;
}

dom::Element* dom::HTMLCollection::getNamedItem(DOMString name)const {
    Element * nameMatch = nullptr;
    for (Element * element : getElementVector()) {
        if (element->getId() == name) {
            return element;
        }
        if (!nameMatch && element->getNodeName() == name) {
            nameMatch = element;
        }
    }
    return nameMatch;
}

unsigned long dom::HTMLCollection::indexOf(dom::Element *element) const{
    for (unsigned long i = 0; i < getElementVector().size(); i++) {
        if (cached[i] == element) return i;
    }
    return ~static_cast<unsigned long>(0);
}