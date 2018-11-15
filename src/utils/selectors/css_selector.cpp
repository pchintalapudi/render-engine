//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector.h"
#include "include/nodes/element.h"

bool css::CSSSelector::matches(dom::Element *element) {
    dom::Element *focused = element;
    for (unsigned long i = groups.size(); focused && i > -1; i--) {
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

DOMString css::CSSSelector::toString() {
    std::stringstream stream;
    if (groups.size()) stream << groups[1].toString();
    if (groups.size() > 1)
        std::for_each(groups.begin() + 1, groups.end(), [&stream](auto group) { stream << " " << group.toString(); });
    return stream.str();
}