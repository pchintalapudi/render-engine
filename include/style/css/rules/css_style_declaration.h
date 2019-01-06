//
// Created by binhe on 12/2/2018.
//

#ifndef FEATHER_CSS_STYLE_DECLARATION_H
#define FEATHER_CSS_STYLE_DECLARATION_H

#include "observable/invalidatable.h"
#include "css_rule.h"

namespace feather {
    namespace css {

        class CSSStyleDeclaration : public observable::Invalidatable {
        private:

            struct Property {
                DOMString key;
                DOMString val;
                bool priority;

                UnaryPair<DOMString> getProp() const { return {key, val}; }

                DOMString toString() const { return key + ": " + val + (priority ? " !important" : ""); }
            };

            Map<DOMString, UInt> indeces{};
            Vector<Property> props{};
            WeakPointer<rules::CSSRule> parentRule{};
        public:
            void setProperty(const DOMString &property, DOMString value, bool priority);

            inline bool hasProperty(const DOMString &property) const {
                return std::find(indeces.begin(), indeces.end(), property) != indeces.end();
            }

            DOMString getPropertyPriority(const DOMString &) const;

            DOMString getPropertyValue(const DOMString &) const;

            DOMString removeProperty(const DOMString &);

            UnaryPair<DOMString> item(UInt) const;

            DOMString getCssText() const;

            void setCssText(DOMString cssText);

            inline UInt getLength() const { return props.size(); }

            inline StrongPointer<rules::CSSRule> getParentRule() const { return parentRule.lock(); }
        };
    }
}

#endif //FEATHER_CSS_STYLE_DECLARATION_H
