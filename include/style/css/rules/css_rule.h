//
// Created by binhe on 12/17/2018.
//

#ifndef FEATHER_CSS_RULE_H
#define FEATHER_CSS_RULE_H

#include "typedefs.h"
#include "style/css/css_style_sheet.h"

namespace feather {
    namespace css {
        namespace rules {
            enum CSSRuleType {
                STYLE_RULE,
                IMPORT_RULE,
                MEDIA_RULE,
                FONT_FACE_RULE,
                PAGE_RULE,
                KEYFRAMES_RULE
            };

            class CSSRule {
                virtual DOMString getCssText();

                virtual std::shared_ptr<DOMString> getParentRule();

                virtual CSSStyleSheet getParentStyleSheet();

                virtual CSSRuleType getType();
            };
        }
    }
}

#endif //FEATHER_CSS_RULE_H
