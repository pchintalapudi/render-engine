//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_NAMED_NODE_MAP_H
#define FEATHER_NAMED_NODE_MAP_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "include/nodes/attr.h"
#include "include/typedefs.h"

class NamedNodeMap {
public:
    inline Attr *getNamedItem(DOMString name) const {
        return dataMap.at(name);
    }

    inline void setNamedItem(Attr &data) {
        dataMap[data.getName()] = &data;
        insertionOrder.push_back(data.getName());
    }

    inline Attr *removeNamedItem(DOMString name) {
        Attr *val = dataMap[name];
        dataMap.erase(name);
        insertionOrder.erase(std::find(insertionOrder.begin(), insertionOrder.end(), name));
        return val;
    }

    inline Attr *getItem(unsigned long index) const {
        return dataMap.at(insertionOrder[index]);
    }

    //TODO: Add the methods relating to namespaces that idk about
private:
    std::unordered_map<DOMString, Attr *> dataMap;
    std::vector<DOMString> insertionOrder;
};

#endif //FEATHER_NAMED_NODE_MAP_H
