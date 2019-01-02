//
// Created by prem on 12/13/2018.
//

#include "nodes/utils/html_collection.h"
#include "nodes/elements/element.h"

using namespace feather::dom;

feather::Pair<bool, feather::StrongPointer<Element>>
feather::dom::nodeToElement(const feather::StrongPointer<feather::dom::Node> &p) {
    return p && p->getNodeTypeInternal() == NodeType::ELEMENT_NODE
           ? std::make_pair(true, std::static_pointer_cast<Element>(p))
           : std::make_pair(false, StrongPointer<Element>());
}

feather::StrongPointer<Element> HTMLCollection::getNamedItem(feather::DOMString name) const {
    StrongPointer <Element> e{};
    for (UInt i = 0; i < size(); i++) {
        auto child = get(i);
        if (child->getId() == name) return child;
        else if (!e && child->getTagName() == name) e = child;
    }
    return e;
}