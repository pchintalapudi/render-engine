//
// Created by prem on 12/12/2018.
//

#include "nodes/utils/named_node_map.h"

using namespace feather::dom;

feather::StrongPointer<feather::dom::Attr> NamedNodeMap::getItem(feather::UInt idx) const {
    auto it = insertionOrder.begin();
    while (idx-- > 0) it++;
    return backing.at(*it);
}

void NamedNodeMap::setNamedItem(feather::StrongPointer<feather::dom::Attr> attr) {
    if (attr) {
        auto name = attr->getName();
        backing[name] = attr;
        insertionOrder.remove(name);
        insertionOrder.push_back(name);
    }
}

feather::StrongPointer<feather::dom::Attr> NamedNodeMap::removeNamedItem(feather::DOMString name) {
    if (backing.find(name) != backing.end()) {
        auto ptr = getNamedItem(name);
        backing.erase(name);
        return ptr;
    }
    return nullptr;
}

feather::Vector<feather::DOMString> NamedNodeMap::getKeys() const {
    Vector <DOMString> v;
    for (const auto &pair : backing) v.push_back(pair.first);
    return v;
}

//Leaves extra space in front
feather::DOMString NamedNodeMap::toHTML() const {
    DOMString html;
    UInt reserve = 0;
    for (const auto &pair : backing) reserve += pair.first.length() + 3 + pair.second->getValue().length();
    html.reserve(reserve);
    for (const auto &pair : backing) ((((html += " ") += pair.first) += "='") += pair.second->getValue()) += "'";
    return html;
}