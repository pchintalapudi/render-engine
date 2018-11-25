//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_H
#define FEATHER_CSS_SELECTOR_H

#include <deque>
#include "css_selector_token_group.h"

namespace css {
    class CSSSelector;
}
class css::CSSSelector {
public:

    CSSSelector(std::vector<CSSSelectorTokenGroup *> groups);

    bool matches(dom::Element *element) const;

    DOMString toString() const;

private:
    std::deque<CSSSelectorTokenGroup *> groups;
};

#endif //FEATHER_CSS_SELECTOR_H
