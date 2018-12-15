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
            enum class Relation {
                DESCENDANT, SIBLING, IMMEDIATE_DESCENDANT, IMMEDIATE_SIBLING
            };

            enum class AttributeType {
                PRESENT, EQUALS, LIST, HYPHEN, PREFIX, SUFFIX, CONTAINS
            };

            class AttributeSelector {
            public:
                DOMString toString();

                bool matches(StrongPointer<Element> element);

            private:
                DOMString attr;
                DOMString val;
                AttributeType type;
                TriValue caseSensitive;
            };

            class PseudoclassSelector {
            public:
                DOMString toString();

                virtual bool matches(StrongPointer<Element> element);

                virtual ~PseudoclassSelector() = default;
            };

            class Token {
            public:
                Token(DOMString tagName, DOMString id, Vector<DOMString> classes, Vector<AttributeSelector> attributes,
                      Vector<PseudoclassSelector> pseudoclasses);

                DOMString toString();

                bool matches(StrongPointer<Element> element);

            private:
                DOMString tagName;
                DOMString id;
                Vector<DOMString> classes;
                Vector<AttributeSelector> attributes;
                Vector<PseudoclassSelector> pseudoclasses;
            };

            class Selector {
            public:

                Selector(Token end, Vector<Pair<Relation, Token>> extra);

                bool matches(StrongPointer<Element> element);

                DOMString toString();

            private:
                Token end;
                Vector<Pair<Relation, Token>> extra;
            };
        }
    }
}
#endif //FEATHER_CSS_SELECTOR_H
