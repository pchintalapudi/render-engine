//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_CSS_SELECTOR_H
#define FEATHER_CSS_SELECTOR_H

#include "typedefs.h"

namespace feather {
    namespace dom {
        class Element;
        namespace selector {
            enum class CSSRelation {
                DESCENDANT, SIBLING, IMMEDIATE_DESCENDANT, IMMEDIATE_SIBLING
            };

            enum class CSSAttributeType {
                PRESENT, EQUALS, LIST, HYPHEN, PREFIX, SUFFIX, CONTAINS
            };

            class CSSAttributeSelector {
            public:

                CSSAttributeSelector(DOMString attr, DOMString val, CSSAttributeType type, TriValue caseSensitive);

                DOMString toString() const;

                bool matches(StrongPointer<const Element> element) const;

            private:
                const DOMString attr;
                const CSSAttributeType type;
                const TriValue caseSensitive;
                const DOMString val;
            };

            class CSSPseudoclassSelector {
            public:

                virtual DOMString toString() const = 0;

                virtual bool matches(StrongPointer<const Element> element) const = 0;

                virtual ~CSSPseudoclassSelector() = default;
            };

            class CSSToken {
            public:
                CSSToken(DOMString tagName, DOMString id, Vector<DOMString> classes,
                         Vector<CSSAttributeSelector> attributes,
                         Vector<CSSPseudoclassSelector *> pseudoclasses)
                        : tagName(tagName), id(id), classes(classes), attributes(attributes),
                          pseudoclasses(pseudoclasses) {}

                DOMString toString() const;

                bool matches(StrongPointer<const Element> element) const;

                ~CSSToken() { for (auto pclass : pseudoclasses) delete pclass; }

            private:
                DOMString tagName;
                DOMString id;
                Vector<DOMString> classes;
                Vector<CSSAttributeSelector> attributes;
                Vector<CSSPseudoclassSelector *> pseudoclasses;
            };

            class CSSSelector {
            public:

                CSSSelector(CSSToken end, Vector<Pair<CSSRelation, CSSToken>> extra)
                        : end(end), extra(extra) {}

                bool matches(StrongPointer<const Element> element) const;

                DOMString toString() const;

                static inline CSSSelector parse(DOMString string, StrongPointer<const Element> scope) {
                    return parseDelegate(string.begin(), string.end(), scope);
                }

                static inline Vector<CSSSelector> parseSelectorList(DOMString string,
                                                                    StrongPointer<const Element> scope) {
                    return parseDelegateList(string.begin(), string.end(), scope);
                }

            private:
                CSSToken end;
                Vector<Pair<CSSRelation, CSSToken>> extra;

                static CSSSelector parseDelegate(DOMString::iterator begin,
                                                 DOMString::iterator end,
                                                 StrongPointer<const Element> scope);

                static Vector<CSSSelector> parseDelegateList(DOMString::iterator begin,
                                                             DOMString::iterator end,
                                                             StrongPointer<const Element> scope);
            };
        }
    }
}
#endif //FEATHER_CSS_SELECTOR_H
