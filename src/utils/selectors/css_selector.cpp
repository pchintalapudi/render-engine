//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector.h"
#include "include/utils/selectors/css_selector_parsing_impl.h"
#include "include/nodes/element.h"

//Useful function declarations

css::CSSSelector::CSSSelector(std::vector<css::CSSSelectorTokenGroup *> groups) {
    this->groups.insert(this->groups.end(), groups.begin(), groups.end());
}

bool css::CSSSelector::matches(dom::Element *element) const {
    dom::Element *focused = element;
    for (unsigned long i = groups.size(); focused && i-- > 0;) {
        while (focused) {
            if ((*groups[i]).matches(focused)) {
                auto size = groups.size() - 1;
                i -= size;
                while (size-- > 0) {
                    focused = focused->getParentElement();
                }
                break;
            }
            focused = focused->getParentElement();
        }
    }
    return focused != nullptr;
}

DOMString css::CSSSelector::toString() const {
    std::stringstream stream;
    switch (groups.size()) {
        case 0:
            break;
        case 1:
            stream << (*groups[0]).toString();
            break;
        default:
            for (auto group : groups) stream << " " << (*group).toString();
            break;
    }
    return stream.str();
}