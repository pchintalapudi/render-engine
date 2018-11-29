//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSS_STYLE_SHEET_H
#define FEATHER_CSS_STYLE_SHEET_H

#include "include/utils/css_rule_list.h"
#include "style_sheet.h"
#include "css_rules/css_import_rule.h"

namespace css {
    class CSSStyleSheet;
}

class css::CSSStyleSheet : public css::StyleSheet, public observable::Invalidatable {
public:
    CSSStyleSheet() : CSSStyleSheet(nullptr) {}

    CSSStyleSheet(CSSImportRule *owner) : owner(owner) {
        rules.addInvalidator(this, [this]() { this->invalidate(); });
    }

    inline const CSSRuleList &getCssRules() const { return rules; }

    inline CSSImportRule *getOwner() const { return owner; }

    inline void insertRule(CSSRule *cssRule) { rules.add(cssRule); }

    inline CSSRule *deleteRule(unsigned long index) { return rules.remove(index); }

private:
    CSSRuleList rules;
    CSSImportRule *owner;
};

#endif //FEATHER_CSS_STYLE_SHEET_H
