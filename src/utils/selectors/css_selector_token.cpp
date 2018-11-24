//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector_token.h"
#include "include/nodes/element.h"

bool css::CSSSelectorToken::matches(dom::Element *element) const {
    if (!type.empty() && type != element->getTagName()) {
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
    std::stringstream stream;
    if (!type.empty()) {
        stream << type;
    }
    if (!id.empty()) {
        stream << "#" << type;
    }
    std::for_each(classes.begin(), classes.end(), [&stream](auto clazz) { stream << "." << clazz; });
    std::for_each(attributesAndPseudoclasses.begin(), attributesAndPseudoclasses.end(),
                  [&stream](auto test) { stream << test.second; });
    return stream.str();
}