//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_TOKEN_GROUP_H
#define FEATHER_CSS_SELECTOR_TOKEN_GROUP_H

#include <utility>
#include <vector>
#include "css_selector_relation.h"
#include "css_selector_token.h"

namespace css { class CSSSelectorTokenGroup; }
class css::CSSSelectorTokenGroup {
public:
    bool matches(dom::Element *element);

    DOMString toString();

    unsigned long getLength();

private:
    CSSSelectorToken end;
    std::vector<std::pair<CSSSelectorToken, CSSSelectorRelation>> tokens;
};

#endif //FEATHER_CSS_SELECTOR_TOKEN_GROUP_H
