//
// Created by prem on 12/12/2018.
//

#include "nodes/utils/named_node_map.h"

using namespace feather::dom;

feather::StrongPointer<feather::dom::Attr<void>> NamedNodeMap::getItem(feather::UInt idx) const {
    auto it = insertionOrder.begin();
    while (idx-- > 0) it++;
    return backing.at(*it);
}

void NamedNodeMap::setNamedItem(feather::StrongPointer<feather::dom::Attr<void>> attr) {
    if (attr) {
        auto name = attr->getName();
        backing[name] = attr;
        insertionOrder.remove(name);
        insertionOrder.push_back(name);
    }
}

feather::StrongPointer<feather::dom::Attr<void>> NamedNodeMap::removeNamedItem(feather::DOMString name) {
    if (backing.find(name) != backing.end()) {
        auto ptr = getNamedItem(name);
        backing.erase(name);
        return ptr;
    }
    return nullptr;
}