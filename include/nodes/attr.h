//
// Created by prem on 11/12/2018.
//

#ifndef FEATHER_ATTR_H
#define FEATHER_ATTR_H

#include "include/typedefs.h"
#include "include/utils/observable/invalidatable.h"

namespace dom {
    class Element;

    class Attr;
}

class dom::Attr : public observable::Invalidatable {
public:

    Attr(DOMString name, Element *owner) : namespaceURI(""), localName(name), prefix(""), ownerElement(owner) {}

    inline DOMString getName() const { return prefix + localName; }

    inline DOMString getNamespaceURI() const { return namespaceURI; }

    inline DOMString getLocalName() const { return localName; }

    inline DOMString getPrefix() const { return prefix; }

    inline Element *getOwnerElement() const { return ownerElement; }

    virtual DOMString getValue() const = 0;

    virtual void setValue(DOMString val) = 0;

private:
    DOMString namespaceURI;
    DOMString localName;
    DOMString prefix;
    Element *ownerElement;
};

#endif //FEATHER_ATTR_H
