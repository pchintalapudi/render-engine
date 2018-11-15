//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_CSS_STYLE_RULE_H
#define FEATHER_CSS_STYLE_RULE_H

#include "include/utils/css_style_declaration.h"
#include "css_rule.h"

namespace css {
    class CSSStyleRule;
}

class css::CSSStyleRule : CSSRule {
private:

    CSSStyleDeclaration styling;
};

#endif //FEATHER_CSS_STYLE_RULE_H
