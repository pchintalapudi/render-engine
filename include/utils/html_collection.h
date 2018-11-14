//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_HTML_COLLECTION_H
#define FEATHER_HTML_COLLECTION_H

#include "include/typedefs.h"
#include "node_list.h"

class Element;

class HTMLCollection {
public:
    HTMLCollection(NodeList &watched) : watched(watched), checksum(watched.getChecksum()), cached(getElementVector()) {}

    inline Element *getItem(unsigned long index) const {
        return index < getLength() ? getElementVector()[index] : nullptr;
    }

    inline unsigned long getLength() const { return getElementVector().size(); }

    Element *getNamedItem(DOMString name);

private:
    NodeList &watched;
    mutable unsigned long checksum;
    mutable std::vector<Element *> cached;

    std::vector<Element *> _get_cacheable_element_vector() const;

    std::vector<Element *> getElementVector() const;
};

#endif //FEATHER_HTML_COLLECTION_H
