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

                Property() = default;

                Property(DOMString key, DOMString val, bool priority) : key(key), val(val), priority(priority) {}

                UnaryPair<DOMString> getProp() const { return {key, val}; }

                DOMString toString() const { return key + ": " + val + (priority ? " !important" : ""); }

                Property operator+(const Property &other) const {
                    return Property(other.key, priority ? val : other.val, priority | other.priority);
                }
            };

            Map<DOMString, UInt> indeces{};
            Vector<Property> props{};
            WeakPointer<rules::CSSRule> parentRule{};
            mutable DOMString styleString{};

            void mergeBefore(const CSSStyleDeclaration &other);

            void mergeAfter(const CSSStyleDeclaration &other);

        public:
            void setProperty(const DOMString &property, DOMString value, bool priority);

            inline bool hasProperty(const DOMString &property) const {
                return indeces.find(property) != indeces.end();
            }

            DOMString getPropertyPriority(const DOMString &) const;

            DOMString getPropertyValue(const DOMString &) const;

            DOMString removeProperty(const DOMString &);

            Tuple<DOMString, DOMString, bool> item(UInt) const;

            const DOMString &getCssText() const;

            void setCssText(const DOMString &cssText);

            inline UInt getLength() const { return size(); }

            inline UInt size() const { return props.size(); }

            inline StrongPointer<rules::CSSRule> getParentRule() const { return parentRule.lock(); }

            inline void markValid() const { validate(); }

            //Internal implementation methods
            template<bool after>
            inline void merge(const CSSStyleDeclaration &other) {
                if (after) mergeAfter(other); else mergeBefore(other);
            }
        };
    }
}

#endif //FEATHER_CSS_STYLE_DECLARATION_H
