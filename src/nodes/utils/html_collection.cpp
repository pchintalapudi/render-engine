//
// Created by prem on 12/13/2018.
//

#include "nodes/utils/html_collection.h"
#include "nodes/elements/element.h"

using namespace feather::dom;

std::vector<feather::WeakPointer<feather::dom::Element>> &HTMLCollection::getVector() const {
    if (isValid()) return cached;
    cached.clear();
    UInt count = 0;
    for (UInt i = 0; i < childNodes->size(); i++)
        if (childNodes->get(i)->getNodeTypeInternal() == NodeType::ELEMENT_NODE) count++;
    cached.reserve(count);
    for (UInt i = 0; i < childNodes->size(); i++) {
        auto child = childNodes->get(i);
        if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE)
            cached.push_back(std::static_pointer_cast<Element>(child));
    }
    validate();
    return cached;
}

feather::StrongPointer<Element> HTMLCollection::getNamedItem(feather::DOMString name) const {
    auto vec = getVector();
    StrongPointer <Element> matchedName = nullptr;
    for (const auto &el : vec) {
        if (el.lock()->getId() == name) return el.lock();
        else if (!matchedName && el.lock()->getNodeName() == name) matchedName = el.lock();
    }
    return matchedName;
}