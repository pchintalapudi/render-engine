//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_NAMED_NODE_MAP_H
#define FEATHER_NAMED_NODE_MAP_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "dom_token_list.h"
#include "include/nodes/attr.h"
#include "include/typedefs.h"

namespace dom {
    class NamedNodeMap;
}
class dom::NamedNodeMap {
public:

    NamedNodeMap(Element *owner, DOMTokenList *classList) : owner(owner), classList(classList) {}

    inline unsigned long getLength() const { return insertionOrder.size(); }

    inline Attr *getNamedItem(DOMString name) const {
        return name == "class" ? getClassAttr() : dataMap.at(name);
    }

    void setNamedItem(Attr &data);

    inline Attr *removeNamedItem(DOMString name) {
        Attr *val = dataMap[name];
        dataMap.erase(name);
        insertionOrder.erase(std::find(insertionOrder.begin(), insertionOrder.end(), name));
        return val;
    }

    inline Attr *getItem(unsigned long index) const {
        return dataMap.at(insertionOrder[index]);
    }

    ~NamedNodeMap() {
        for (auto pair : dataMap) {
            delete pair.second;
        }
        delete classAttr;
    }

    //TODO: Add the methods relating to namespaces that idk about
private:
    std::unordered_map<DOMString, Attr *> dataMap;
    std::vector<DOMString> insertionOrder;
    Element *const owner;
    DOMTokenList *const classList;
    mutable Attr *classAttr;
    mutable unsigned long checksum;

    Attr *getClassAttr() const;

    void setClassAttr(Attr *classAttr);
};

#endif //FEATHER_NAMED_NODE_MAP_H
