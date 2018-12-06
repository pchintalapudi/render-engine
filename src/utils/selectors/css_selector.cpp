//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector.h"
#include "include/nodes/element.h"

//Useful function declarations

css::CSSSelector::CSSSelector(std::vector<css::CSSSelectorTokenGroup *> groups) {
    this->groups.insert(this->groups.end(), groups.begin(), groups.end());
}

bool css::CSSSelector::matches(const dom::Element *element) const {
    auto focused = element;
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

css::CSSSelectorTokenGroup *css::CSSSelector::process(const dom::Element *element) {
    if (groups.front()->matches(element)) {
        auto group = groups.front();
        groups.pop_front();
        return group;
    }
    return nullptr;
}

void css::CSSSelector::preprocess(const dom::Element *element) {
    auto dispatchChain = element->buildDispatchChain();
    dispatchChain.pop_back();
    for (auto el : dispatchChain) {
        if (groups.size() - 1) {
            if (el->getNodeType() == dom::NodeType::ELEMENT_NODE
                && groups.front()->matches(static_cast<dom::Element *>(el)))
                groups.pop_front();
        } else break;
    }
}

DOMString css::CSSSelector::toString() const {
    DOMString string;
    switch (groups.size()) {
        case 0:
            break;
        case 1:
            string += groups.front()->toString();
            break;
        default:
            for (auto group : groups) (string += " ") += group->toString();
            break;
    }
    return string;
}

css::CSSSelector::~CSSSelector() {
    for (auto group : groups) delete group;
}