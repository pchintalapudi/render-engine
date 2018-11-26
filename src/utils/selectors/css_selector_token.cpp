//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector_token.h"
#include "include/nodes/element.h"

css::CSSSelectorToken::CSSSelectorToken(
        DOMString type, DOMString id, std::vector<DOMString> &classes,
        std::vector<std::pair<std::function<bool(dom::Element *)>, DOMString>> &weirdFunctions)
        : type(type), id(id) {
    this->classes.insert(this->classes.end(), classes.begin(), classes.end());
    this->attributesAndPseudoclasses.insert(this->attributesAndPseudoclasses.end(),
                                            weirdFunctions.begin(), weirdFunctions.end());
}

bool css::CSSSelectorToken::matches(dom::Element *element) const {
    if (type != "*" && type != element->getTagName()) {
        return false;
    }
    if (!id.empty() && id != element->getId()) {
        return false;
    }
    if (!std::all_of(classes.begin(), classes.end(),
                     [element](auto clazz) { return element->getClassList().contains(clazz); })) {
        return false;
    }
    return std::all_of(attributesAndPseudoclasses.begin(), attributesAndPseudoclasses.end(),
                       [element](auto test) { return test.first(element); });
}

DOMString css::CSSSelectorToken::toString() const {
    DOMString str = "";
    str += type;
    if (!id.empty()) (str += "#") += id;
    for (auto clazz : classes) str += ("." + clazz);
    for (auto func : attributesAndPseudoclasses) str += func.second;
    return str;
}