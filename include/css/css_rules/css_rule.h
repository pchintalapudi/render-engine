//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSSRULE_H
#define FEATHER_CSSRULE_H

#include "include/typedefs.h"
#include "css_rule_type.h"

namespace css {
    class CSSStyleSheet;

    class CSSRule;
}
class css::CSSRule {
public:

    CSSRule(CSSRule *parent, CSSStyleSheet *parentStyleSheet, CSSRuleType type, bool *dirty)
            : parent(parent), parentStyleSheet(parentStyleSheet), type(type), dirty(dirty), checksum(0) {}

    virtual DOMString getCSSText() = 0;

    inline CSSRule *getParent() { return parent; }

    inline CSSStyleSheet *getParentStylesheet() { return parentStyleSheet; }

    inline CSSRuleType getType() { return type; }

    inline unsigned long getChecksum() { return checksum; }

    inline bool isDirty() { return *dirty; }

    inline void clean() { *dirty = false; }

private:
    CSSRule *parent;
    CSSStyleSheet *parentStyleSheet;
    CSSRuleType type;
    unsigned long checksum;
    bool *dirty;
};

#endif //FEATHER_CSSRULE_H
