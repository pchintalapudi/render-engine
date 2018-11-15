//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_H
#define FEATHER_CSS_SELECTOR_H

#include <functional>
#include <utility>
#include <vector>
#include "include/typedefs.h"

class Element;

namespace css {
    class Token;

    enum class Relation;

    class TokenGroup;

    class CSSSelector;
}

struct css::Token {
    DOMString tag;
    DOMString id;
    std::vector<DOMString> classes;
    std::vector<std::function<bool(Element &)>> pseudoclassesAndAttributes;
};

enum class css::Relation {
    DESCENDANT, ADJACENT, POSTERIOR;
};

class css::TokenGroup {
    Token end;
    std::vector<std::pair<css::Token, Relation>> additional;
};

class css::CSSSelector {
};

#endif //FEATHER_CSS_SELECTOR_H
