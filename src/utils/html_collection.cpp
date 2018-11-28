//
// Created by prem on 11/28/2018.
//

#include "include/utils/html_collection.h"
#include "include/nodes/element.h"

const std::vector<dom::Element *> *dom::HTMLCollection::compute() const {
    if (watched) {
        for (unsigned long i = 0; i < watched->size(); i++) {
            auto node = watched->get(i);
            if (node->getNodeType() == dom::NodeType::ELEMENT_NODE) backing.push_back(static_cast<Element *>(node));
        }
    } else backing.clear();
    return &backing;
}