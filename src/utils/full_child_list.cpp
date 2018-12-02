//
// Created by prem on 12/1/2018.
//

#include "include/utils/full_child_list.h"
#include "include/nodes/element.h"

bool dom::FullChildList::handle(long long bitField) const {
    if (observable::present(bitField, observable::EventType::LIST_CHANGE))return true;
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child && !child->getAllChildren().isValid()) return true;
    }
    return false;
}

std::vector<dom::Element *> *dom::FullChildList::compute() const {
    unsigned long netSize = children.size();
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child) netSize += child->getAllChildren().size();
    }
    backing.reserve(netSize);
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        backing.push_back(child);
        if (child) {
            auto childs = child->getAllChildren();
            for (unsigned long j = 0; j < childs.size(); j++) backing.push_back(childs.get(j));
        }
    }
    return &backing;
}