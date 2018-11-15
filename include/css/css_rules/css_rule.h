//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSSRULE_H
#define FEATHER_CSSRULE_H

#include "include/typedefs.h"
#include "css_rule_type.h"

class CSSStyleSheet;

class CSSRule {
public:

    CSSRule(CSSRule *parent, CSSStyleSheet *parentStyleSheet, CSSRuleType type)
            : parent(parent), parentStyleSheet(parentStyleSheet), type(type) {}

    inline DOMString getCssText() { return cssText; };

    inline CSSRule *getParent() { return parent; }

    inline CSSStyleSheet *getParentStylesheet() { return parentStyleSheet; }

    inline CSSRuleType getType() { return type; }

private:
    CSSRule *parent;
    CSSStyleSheet *parentStyleSheet;
    CSSRuleType type;
    DOMString cssText;
};

#endif //FEATHER_CSSRULE_H
