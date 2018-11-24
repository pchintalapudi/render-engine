//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_TOKEN_H
#define FEATHER_CSS_SELECTOR_TOKEN_H

#include "include/typedefs.h"
#include <vector>
#include <functional>
#include <utility>

namespace dom { class Element; }
namespace css { class CSSSelectorToken; }
class css::CSSSelectorToken {
private:
    DOMString type;
    DOMString id;
    std::vector<DOMString> classes;
    std::vector<std::pair<std::function<bool(dom::Element *)>, DOMString>> attributesAndPseudoclasses;
public:
    bool matches(dom::Element *element) const;

    DOMString toString() const;
};

#endif //FEATHER_CSS_SELECTOR_TOKEN_H
