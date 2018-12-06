//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_TOKEN_GROUP_H
#define FEATHER_CSS_SELECTOR_TOKEN_GROUP_H

#include <utility>
#include <vector>
#include <deque>
#include "css_selector_relation.h"
#include "css_selector_token.h"

namespace css { class CSSSelectorTokenGroup; }
class css::CSSSelectorTokenGroup {
public:
    CSSSelectorTokenGroup(CSSSelectorToken *end, std::vector<std::pair<CSSSelectorToken *, CSSSelectorRelation >> tokens);

    bool matches(const dom::Element *element) const;

    DOMString toString() const;

    unsigned long getLength() const;

private:
    CSSSelectorToken *end;
    std::deque<std::pair<CSSSelectorToken *, CSSSelectorRelation>> tokens;
};

#endif //FEATHER_CSS_SELECTOR_TOKEN_GROUP_H
