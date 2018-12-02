//
// Created by prem on 11/29/2018.
//

#ifndef FEATHER_STANDARD_ATTR_H
#define FEATHER_STANDARD_ATTR_H

#include "../attr.h"
#include "include/utils/observable/event/event_type.h"

namespace dom {
    class StandardAttr;
}

class dom::StandardAttr : public Attr {
public:

    StandardAttr(DOMString name, Element *owner) : Attr(name, owner) {}

    StandardAttr(DOMString name, Element *owner, DOMString value) : Attr(name, owner), value(value) {}

    DOMString getValue() const override { return value; }

    void setValue(DOMString value) override {
        this->value = value;
        this->invalidate(observable::generate(observable::EventType::INTERNAL_CHANGE));
    }

private:
    DOMString value;
};

#endif //FEATHER_STANDARD_ATTR_H
