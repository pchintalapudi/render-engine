//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSS_STYLE_SHEET_H
#define FEATHER_CSS_STYLE_SHEET_H

#include "include/utils/css_rule_list.h"
#include "style_sheet.h"

namespace css {
    class CSSStyleSheet;
    class CSSRule;
    class CSSImportRule;
}

class css::CSSStyleSheet : css::StyleSheet {
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

    inline void clean() {
        dirty = false;
        checksum = rules.getChecksum();
    }

    inline bool isDirty() { return dirty || checksum != rules.getChecksum(); }

private:
    CSSRuleList rules;
    CSSImportRule *owner;
    bool dirty;
    unsigned long checksum;
};

#endif //FEATHER_CSS_STYLE_SHEET_H
