//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_H
#define FEATHER_CSS_SELECTOR_H

#include <vector>
#include "css_selector_token_group.h"

namespace css {
    class CSSSelector;
}
class css::CSSSelector {
public:
    bool matches(dom::Element *element);
    DOMString toString();
    static CSSSelector parse(DOMString selector);
    static std::vector<CSSSelector> parseList(DOMString selectorList);
private:
    std::vector<CSSSelectorTokenGroup> groups;
};

#endif //FEATHER_CSS_SELECTOR_H
