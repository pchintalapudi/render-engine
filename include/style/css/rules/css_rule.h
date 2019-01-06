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
                UNKNOWN_RULE,
                STYLE_RULE,
                CHARSET_RULE,
                IMPORT_RULE,
                MEDIA_RULE,
                FONT_FACE_RULE,
                PAGE_RULE,
                KEYFRAMES_RULE,
                KEYFRAME_RULE,
                RESERVED_RULE,
                NAMESPACE_RULE,
                COUNTER_STYLE_RULE,
                SUPPORTS_RULE,
                DOCUMENT_RULE,
                FONT_FEATURE_VALUES_RULE,
                VIEWPORT_RULE,
                REGION_STYLE_RULE
            };

            class CSSRule {
            public:

                CSSRule(CSSRuleType type, const StrongPointer<CSSRule> &parentRule,
                        const StrongPointer<CSSStyleSheet> &parentStylesheet)
                        : type(type), parentRule(parentRule), parentStyleSheet(parentStylesheet) {}

                virtual DOMString getCssText() = 0;

                inline StrongPointer<CSSRule> getParentRule() { return parentRule.lock(); }

                inline StrongPointer<CSSStyleSheet> getParentStyleSheet() { return parentStyleSheet.lock(); }

                inline CSSRuleType getType() { return type; }

            private:
                CSSRuleType type;
                WeakPointer<CSSRule> parentRule;
                WeakPointer<CSSStyleSheet> parentStyleSheet;
            };
        }
    }
}

#endif //FEATHER_CSS_RULE_H
