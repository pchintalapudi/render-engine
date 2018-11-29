//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSS_RULE_LIST_H
#define FEATHER_CSS_RULE_LIST_H

#include "include/css/css_rules/css_rule.h"
#include "observable/smart_observable_list.h"

namespace css {
    class CSSRuleList;
}

class css::CSSRuleList : public observable::SmartObservableList<CSSRule *> {
};

#endif //FEATHER_CSS_RULE_LIST_H
