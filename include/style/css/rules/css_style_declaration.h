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
                DOMString key{};
                DOMString val{};
                bool priority{false};

                Property() = default;

                Property(DOMString key, DOMString val, bool priority) : key(std::move(key)), val(std::move(val)),
                                                                        priority(priority) {}

                UnaryPair<DOMString> getProp() const { return {key, val}; }

                DOMString toString() const { return key + ": " + val + (priority ? " !important" : ""); }

                Property operator+(const Property &other) const {
                    return Property(other.key, priority && priority ^ other.priority ? val : other.val,
                                    priority | other.priority);
                }

                Property &operator+=(const Property &other) {
                    val = priority && priority ^ other.priority ? val : other.val;
                    priority |= other.priority;
                    return *this;
                }

                Property &operator+=(Property &&other) {
                    priority |= other.priority;
                    return *this;
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

            void setCssText(DOMString cssText);

            inline UInt getLength() const { return size(); }

            inline UInt size() const { return props.size(); }

            inline StrongPointer<rules::CSSRule> getParentRule() const { return parentRule.lock(); }

            inline void markValid() const { validate(); }

            //Internal implementation methods
            template<bool after>
            inline void merge(const CSSStyleDeclaration &other) {
                if (after) mergeAfter(other); else mergeBefore(other);
            }

            auto begin() const { return props.begin(); }

            auto begin() { return props.begin(); }

            auto end() const { return props.end(); }

            auto end() { return props.end(); }
        };
    }
}

#endif //FEATHER_CSS_STYLE_DECLARATION_H
