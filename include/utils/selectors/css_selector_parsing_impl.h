//
// Created by prem on 11/24/2018.
//

#ifndef FEATHER_CSS_SELECTOR_PARSING_IMPL_H
#define FEATHER_CSS_SELECTOR_PARSING_IMPL_H

#include "css_selector.h"

namespace css {

    css::CSSSelector parse(DOMString selector);

    std::vector<css::CSSSelector> parseList(DOMString selector);
}

#endif //FEATHER_CSS_SELECTOR_PARSING_IMPL_H
