//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_NAMED_NODE_MAP_H
#define FEATHER_NAMED_NODE_MAP_H

#include <algorithm>
#include <list>
#include <map>
#include <include/utils/observable/event/event_type.h>
#include "dom_token_list.h"
#include "include/nodes/attr.h"
#include "include/typedefs.h"
#include "observable/invalidatable.h"

namespace dom {
    class NamedNodeMap;
}
class dom::NamedNodeMap : public observable::Invalidatable {
public:

    inline bool contains(DOMString attr) const {
        return attrMap.find(attr) != attrMap.end();
    }

    inline Attr *getNamedItem(DOMString key) const {
        return attrMap.find(key) != attrMap.end() ? attrMap.at(key) : nullptr;
    }

    void setNamedItem(Attr *attr) {
        DOMString name = attr->getName();
        if (attrMap.find(name) != attrMap.end()) delete attrMap[name];
        attrMap[name] = attr;
        insertionOrder.remove(name);
        insertionOrder.push_front(name);
        this->invalidate(observable::generate(observable::EventType::MAP_CHANGE));
    }

    Attr *removeNamedItem(DOMString key) {
        auto val = getNamedItem(key);
        if (val) {
            attrMap.erase(key);
            insertionOrder.remove(key);
        }
        return val;
    }

    std::vector<DOMString> keys() const {
        std::vector<DOMString> keys;
        for (auto it : attrMap) keys.push_back(it.first);
        return keys;
    }

    Attr *getItem(unsigned long index) const { return attrMap.at(*std::next(insertionOrder.begin(), index)); }

    inline unsigned long size() const { return attrMap.size(); }

    ~NamedNodeMap() {
        for (auto attr : attrMap) delete attr.second;
    }

private:
    Element *owner;
    std::map<DOMString, Attr *> attrMap;
    mutable std::list<DOMString> insertionOrder;
};

#endif //FEATHER_NAMED_NODE_MAP_H
