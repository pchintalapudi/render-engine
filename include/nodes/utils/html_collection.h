//
// Created by prem on 11/8/2018.
//

#ifndef FEATHER_HTML_COLLECTION_H
#define FEATHER_HTML_COLLECTION_H

#include <vector>
#include "../node.h"
#include "typedefs.h"

class HTMLCollection {
public:
    Node *getItem(unsigned long index) {
        return index < collection.size() ? collection[index] : nullptr;
    }

    Node *getNamedItem(DOMString name) {
        Node *fallback = nullptr;
        for (unsigned long i = 0; i < collection.size(); i++) {
            Node *item = collection[i];
            if (item->getNodeType() == ELEMENT_NODE && dynamic_cast<Element *>(item)->getId() == name) {
                return item;
            } else if (!fallback && item->getNodeName() == name) {
                fallback = item;
            }
        }
        return fallback;
    }

private:
    std::vector<Node *> collection;
};

#endif //FEATHER_HTML_COLLECTION_H
