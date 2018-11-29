//
// Created by prem on 11/29/2018.
//

#ifndef FEATHER_CLASS_ATTR_H
#define FEATHER_CLASS_ATTR_H

#include "include/utils/dom_token_list.h"
#include "../attr.h"

namespace dom {
    class ClassAttr;
}

class dom::ClassAttr : Attr {
public:

    ClassAttr(DOMString name, Element *owner) : Attr(name, owner), classes(nullptr) {
        classes.addInvalidator(this, [this]() { this->invalidate(); });
    }

    DOMString getValue() const override {
        if (this->isValid()) return cached;
        this->validate();
    }

    void setValue(DOMString value) override { classes.setValue(value); }

    DOMTokenList &getClassList() { return classes; }

private:
    DOMTokenList classes;
    mutable DOMString cached;
};

#endif //FEATHER_CLASS_ATTR_H
