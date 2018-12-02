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

class dom::ClassAttr : public Attr {
public:

    ClassAttr(Element *owner, DOMTokenList &classes) : Attr("class", owner), classes(classes) {
        classes.addInvalidator(this, invalidator);
    }

    DOMString getValue() const override {
        if (this->isValid()) return cached;
        DOMString val = classes.getValue();
        this->validate();
        return val;
    }

    void setValue(DOMString value) override { classes.setValue(value); }

    DOMTokenList &getClassList() { return classes; }

    ~ClassAttr() { classes.clear(); }

private:
    DOMTokenList &classes;
    mutable DOMString cached;
};

#endif //FEATHER_CLASS_ATTR_H
