//
// Created by prem on 11/15/2018.
//

#include <algorithm>
#include <sstream>
#include "include/utils/selectors/css_selector_token_group.h"
#include "include/utils/selectors/css_selector_relation.h"
#include "include/nodes/element.h"

bool css::CSSSelectorTokenGroup::matches(dom::Element *element) {
    if (!end.matches(element)) return false;
    dom::Element *focused = element;
    for (long long i = tokens.size(); i > -1; i--) {
        auto token = tokens[i];
        switch (token.second) {
            case CSSSelectorRelation::DESCENDANT:
                if (focused->getParentElement() && token.first.matches(focused->getParentElement())) {
                    focused = focused->getParentElement();
                    break;
                } else return false;
            case CSSSelectorRelation::IMMEDIATE_SIBLING: {
                auto sibling = focused->getPreviousSibling();
                while (sibling && sibling->getNodeType() != dom::NodeType::ELEMENT_NODE)
                    sibling = sibling->getPreviousSibling();
                if (sibling && token.first.matches(
                        static_cast<dom::Element *>(sibling))) {
                    focused = static_cast<dom::Element *>(sibling);
                    break;
                } else return false;
            }
            case CSSSelectorRelation::SIBLING: {
                auto sibling = focused->getPreviousSibling();
                while (sibling) {
                    if (sibling->getNodeType() == dom::NodeType::ELEMENT_NODE) {
                        if (token.first.matches(static_cast<dom::Element *>(sibling))) {
                            focused = static_cast<dom::Element *>(sibling);
                            break;
                        }
                    }
                    sibling = sibling->getPreviousSibling();
                }
                if (!sibling) return false;
                else break;
            }
            default:
                return false;
        }

    }
    return true;
}

DOMString css::CSSSelectorTokenGroup::toString() {
    std::stringstream stream;
    std::for_each(tokens.begin(), tokens.end(), [&stream](auto token) {
        stream << token.first.toString();
        switch (token.second) {
            case CSSSelectorRelation::DESCENDANT:
                stream << " > ";
                break;
            case CSSSelectorRelation::SIBLING:
                stream << " ~ ";
                break;
            case CSSSelectorRelation::IMMEDIATE_SIBLING:
                stream << " + ";
                break;
            default:
                break;
        }
    });
    stream << end.toString();
    return stream.str();
}

unsigned long css::CSSSelectorTokenGroup::getLength() {
    unsigned long length = 1;
    for (unsigned long i = 0; i < tokens.size(); i++) if (tokens[i].second == CSSSelectorRelation::DESCENDANT) length++;
    return length;
}