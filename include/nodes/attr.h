//
// Created by prem on 11/12/2018.
//

#ifndef FEATHER_ATTR_H
#define FEATHER_ATTR_H

#include "include/typedefs.h"

class Element;
class Attr {
public:
    inline DOMString getName() {
        return prefix + localName;
    }
    inline DOMString getNamespaceURI() {
        return namespaceURI;
    }
    inline DOMString getLocalName() {
        return localName;
    }
    inline DOMString getPrefix() {
        return prefix;
    }
    inline Element *getOwnerElement() {
        return ownerElement;
    }
    inline DOMString getValue() {
        return value;
    }
    inline void setValue(DOMString value) {
        this->value = value;
    }
private:
    DOMString namespaceURI;
    DOMString localName;
    DOMString prefix;
    Element *ownerElement;
    DOMString value;
};
#endif //FEATHER_ATTR_H
