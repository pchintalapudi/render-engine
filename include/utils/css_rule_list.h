//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSS_RULE_LIST_H
#define FEATHER_CSS_RULE_LIST_H

#include <vector>
#include <numeric>
#include "include/css/css_rules/css_rule.h"

namespace css {
    class CSSRuleList;
}

class css::CSSRuleList {
public:
    inline unsigned long getLength() const { return rules.size(); }

    inline CSSRule *getItem(unsigned long index) const { return rules[index]; }

    inline std::vector<CSSRule *> &backing() { return rules; }

    unsigned long getChecksum() {
        unsigned long checksum = 0;
        for (auto rule : rules) { checksum += rule->getChecksum(); }
        return checksum;
    }

private:
    std::vector<CSSRule *> rules;
};

#endif //FEATHER_CSS_RULE_LIST_H
