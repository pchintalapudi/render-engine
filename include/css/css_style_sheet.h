//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSS_STYLE_SHEET_H
#define FEATHER_CSS_STYLE_SHEET_H

#include "include/utils/css_rule_list.h"
#include "css_rules/css_import_rule.h"
#include "style_sheet.h"

class CSSStyleSheet : StyleSheet {
public:
    inline const CSSRuleList &getCssRules() const { return rules; }

    inline CSSImportRule *getOwner() const { return owner; }

    inline void insertRule(CSSRule *cssRule) {
        rules.backing().push_back(cssRule);
        dirty = true;
    }

    inline CSSRule *deleteRule(unsigned long index) {
        auto rule = rules.getItem(index);
        rules.backing().erase(rules.backing().begin() + index);
        dirty = true;
        return rule;
    }

    inline void clean() { dirty = false; }

    inline bool isDirty() { return dirty; }

private:
    CSSRuleList rules;
    CSSImportRule *owner;
    bool dirty;
    unsigned long checksum;
};

#endif //FEATHER_CSS_STYLE_SHEET_H
