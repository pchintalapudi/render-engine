//
// Created by binhe on 12/1/2018.
//

#ifndef FEATHER_CSS_STYLE_RULE_H
#define FEATHER_CSS_STYLE_RULE_H

#include "typedefs.h"
#include "css_style_declaration.h"

namespace feather {
    namespace css {
        namespace rules {
            class CSSStyleRule : public CSSRule {
            private:
                DOMString selectorText;
                StrongPointer<CSSStyleDeclaration> styleDeclaration;
            };
        }
    }
}
#endif //FEATHER_CSS_STYLE_RULE_H
