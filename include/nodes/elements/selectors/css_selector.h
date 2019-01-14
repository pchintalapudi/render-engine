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

                bool matches(const StrongPointer<const Element> &element) const;

            private:
                DOMString attr{};
                CSSAttributeType type;
                TriValue caseSensitive;
                DOMString val{};
            };

            class CSSPseudoclassSelector {
            public:

                virtual DOMString toString() const = 0;

                virtual bool matches(const StrongPointer<const Element> &element) const = 0;

                virtual ~CSSPseudoclassSelector() = default;
            };

            class CSSToken {
            public:
                CSSToken(DOMString tagName, DOMString id, Vector<DOMString> classes,
                         Vector<CSSAttributeSelector> attributes, Vector<CSSPseudoclassSelector *> pseudoclasses,
                         CSSRelation relation) : tagName(std::move(tagName)), id(std::move(id)),
                                                 classes(std::move(classes)), attributes(std::move(attributes)),
                                                 pseudoclasses(std::move(pseudoclasses)), relation(relation) {}

                CSSToken(const CSSToken &other) = delete;

                auto operator=(const CSSToken &other) = delete;

                CSSToken(CSSToken &&other) = default;

                CSSToken &operator=(CSSToken &&other) = default;

                DOMString toString() const;

                bool matches(const StrongPointer<const Element> &element) const;

                inline CSSRelation getRelation() const { return relation; }

                inline void setRelation(CSSRelation relation) { this->relation = relation; }

                ~CSSToken() { for (auto pclass : pseudoclasses) delete pclass; }

            private:
                DOMString tagName{};
                DOMString id{};
                Vector<DOMString> classes{};
                Vector<CSSAttributeSelector> attributes{};
                Vector<CSSPseudoclassSelector *> pseudoclasses{};
                CSSRelation relation;
            };

            class CSSDescendantToken {
            public:

                explicit CSSDescendantToken(Vector<CSSToken> group);

                DOMString toString() const;

                bool matches(const StrongPointer<const Element> &element) const;

                inline UInt size() const { return length; }

            private:
                Vector<CSSToken> group{};
                UInt length = 0;
            };

            class CSSSelector {
            public:

                explicit CSSSelector(Vector<CSSDescendantToken> descendants)
                        : descendants(std::move(descendants)) {}

                bool matches(const StrongPointer<const Element> &element) const;

                inline StrongPointer<Element> querySelector(const StrongPointer<const Element> &scope) const {
                    return querySelectorInternal(preprocess(scope), scope);
                }

                inline Vector<StrongPointer<Element>>
                querySelectorAll(const StrongPointer<const Element> &scope) const {
                    Vector<StrongPointer<Element>> vec;
                    querySelectorAllInternal(preprocess(scope), scope, vec);
                    return vec;
                }

                DOMString toString() const;

                static inline CSSSelector parse(DOMString string, StrongPointer<const Element> scope) {
                    return parseDelegate(string.begin(), string.end(), std::move(scope));
                }

                static inline Vector<CSSSelector> parseSelectorList(DOMString string,
                                                                    StrongPointer<const Element> scope) {
                    return parseDelegateList(string.begin(), string.end(), std::move(scope));
                }

                static CSSSelector parseDelegate(DOMString::const_iterator begin,
                                                 DOMString::const_iterator end,
                                                 StrongPointer<const Element> scope);

                static Vector<CSSSelector> parseDelegateList(DOMString::const_iterator begin,
                                                             DOMString::const_iterator end,
                                                             StrongPointer<const Element> scope);

            private:
                Vector<CSSDescendantToken> descendants{};

                Vector<CSSDescendantToken>::const_iterator
                preprocess(StrongPointer<const Element> scope) const;

                StrongPointer<Element> querySelectorInternal(Vector<CSSDescendantToken>::const_iterator begin,
                                                             StrongPointer<const Element> scope) const;

                void querySelectorAllInternal(Vector<CSSDescendantToken>::const_iterator begin,
                                              StrongPointer<const Element> scope,
                                              Vector<StrongPointer<Element>> &ref) const;
            };
        }
    }
}
#endif //FEATHER_CSS_SELECTOR_H
