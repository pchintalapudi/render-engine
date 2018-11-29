//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_CSS_RULE_H
#define FEATHER_CSS_RULE_H

#include "include/utils/observable/invalidatable.h"
#include "css_rule_type.h"
#include "include/typedefs.h"

namespace css {
    class CSSRule;

    class CSSStyleSheet;
}

class css::CSSRule : public observable::Invalidatable {
public:
    CSSRule(CSSRuleType type) : CSSRule(type, nullptr, nullptr) {}

    CSSRule(CSSRuleType type, CSSRule *parent) : CSSRule(type, parent, nullptr) {}

    CSSRule(CSSRuleType type, CSSStyleSheet *owner) : CSSRule(type, nullptr, owner) {}

    CSSRule(CSSRuleType type, CSSRule *parent, CSSStyleSheet *owner) : type(type), parent(parent), owner(owner) {}

    inline CSSRuleType getType() { return type; }

    inline CSSRule *getParent() { return parent; }

    inline CSSStyleSheet *getOwner() { return owner; }

    DOMString getCSSText() {
        if (isValid()) return cached;
        cached = compute();
        validate();
        return cached;
    }

protected:
    virtual DOMString compute() = 0;

private:
    const CSSRuleType type;
    CSSRule *parent;
    CSSStyleSheet *owner;
    DOMString cached;
};

#endif //FEATHER_CSS_RULE_H
