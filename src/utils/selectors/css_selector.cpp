//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector.h"
#include "include/utils/selectors/css_selector_parsing_impl.h"
#include "include/nodes/element.h"

//Useful function declarations


bool css::CSSSelector::matches(dom::Element *element) const {
    dom::Element *focused = element;
    for (long long i = (long long) groups.size(); focused && i > -1; i--) {
        while (focused) {
            if (groups[i].matches(focused)) {
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
    if (groups.size()) stream << groups[1].toString();
    if (groups.size() > 1)
        std::for_each(groups.begin() + 1, groups.end(), [&stream](auto group) { stream << " " << group.toString(); });
    return stream.str();
}