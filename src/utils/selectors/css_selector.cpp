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

css::CSSSelector getSubselector(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1, openCount = 1;
    for (; k < j; k++) {
        switch (selector[k]) {
            case '(':
                openCount++;
                break;
            case ')':
                openCount--;
                if (!openCount) {
                    goto closed;
                }
        }
    }
    closed:
    *i = k;
    return css::CSSSelector::parse(selector.substr(*i + 1, k - *i + 2));
}

std::vector<css::CSSSelector> getSubselectors(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1, openCount = 1;
    for (; k < j && openCount; k++) {
        switch (selector[k]) {
            case '(':
                openCount++;
                break;
            case ')':
                openCount--;
                break;
        }
    }
    *i = k;
    return css::CSSSelector::parseList(selector.substr(*i + 1, k - *i + 2));
}

struct NthSel {
    long A;
    long b;
};

NthSel parseFormula(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1;
    NthSel ret;
    std::stringstream stream;
    while (k < j && selector[k] != 'n') if (!isspace(selector[k++])) stream << selector[k - 1];
    stream >> ret.A;
    k++;
    while (k < j && selector[k] != ')') if (!isspace(selector[k++])) stream << selector[k - 1];
    stream >> ret.b;
    *i = k;
    return ret;
}

enum class AttrSelectorType {
    MATCH, CONTAIN, END, LIST, HYPHEN, BEGIN
};

std::function<bool(dom::Element *)> parseAttrSelector(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1;
    std::stringstream stream;
    for (; k < j && selector[k] != '=' && selector[k] != ']'; stream << selector[k++]);
    if (selector[k] == '=' && k > 0) {
        AttrSelectorType type;
        switch (selector[k - 1]) {
            case '*':
                type = AttrSelectorType::CONTAIN;
                break;
            case '$':
                type = AttrSelectorType::END;
                break;
            case '^':
                type = AttrSelectorType::BEGIN;
                break;
            case '~':
                type = AttrSelectorType::LIST;
                break;
            case '|':
                type = AttrSelectorType::HYPHEN;
                break;
            default:
                type = AttrSelectorType::MATCH;
        }
        auto attr = stream.str();
        stream.clear();
        while (selector[k++] != '"');
        for (; selector[k] != '"'; stream << selector[k++]);
        *i = k;
        auto val = stream.str();
        stream.clear();
        while (selector[k] != ']') {
            if (!isspace(selector[k++])) {
                stream << selector[k - 1];
            }
        }
        auto remainder = stream.str();
        bool caseSensitive = remainder == "i" || remainder == "I";
        val = val.substr(1, val.length() - 2);
        switch (type) {
            case AttrSelectorType::MATCH:
                return [attr, val](dom::Element *element) {
                    auto attribute = element->getAttributes().getNamedItem(attr);
                    return attribute && attribute->getValue() == val;
                };
            case AttrSelectorType::CONTAIN:
                return [attr, val](dom::Element *element) {
                    auto attribute = element->getAttributes().getNamedItem(attr);
                    return attribute && attribute->getValue().find(val) != DOMString::npos;
                };
            case AttrSelectorType::END:
                return [attr, val](dom::Element *element) {
                    auto attribute = element->getAttributes().getNamedItem(attr);
                    if (!attribute) return false;
                    auto value = attribute->getValue();
                    return value.length() > val.length() &&
                           value.substr(value.length() - val.length(), val.length()) == val;
                };
        }
    } else {
        auto attr = stream.str();
        return [attr](dom::Element *element) { return element->getAttributes().getNamedItem(attr) != nullptr; };
    }
}

css::CSSSelector css::CSSSelector::parse(DOMString selector) {
    //Plan of action:
    //Iterate over the selector one char at a time
    //If a special character is encountered stop and interpret

    //Set up start and end indeces to skip over arbitrary whitespace
    unsigned long j = selector.length(), i = -1;
    while (j > -1 && isspace(selector[--j]));
    while (i < j && isspace(++i));
    j++;
    //Set up char buffer
    std::stringstream buffer;
    //Set up selector vector
    std::vector<CSSSelectorTokenGroup> selVector;
    //Set up token group relations
    std::vector<std::pair<CSSSelectorToken, CSSSelectorRelation>> relations;
    //Set up token
    DOMString tagName;
    DOMString id;
    std::vector<DOMString> classes;
    std::vector<std::function<bool(dom::Element *)>> weirdFuncs;
    //Monitor spaces
    CSSSelectorRelation relation = CSSSelectorRelation::NONE;
    bool spaceFound = false;
    for (; i < j; i++) {
        switch (selector[i]) {
            default:
                if (spaceFound) {
                    if (relation == CSSSelectorRelation::NONE) {
                        //Clear up the entire token group; this is a valid space delimiter
                    }
                    spaceFound = false;
                }
                if (relation != CSSSelectorRelation::NONE) {
                    //We are done with the token; we should clean up the token alone
                }
                buffer << selector[i];
                continue;
            case '(': {
                DOMString prev = buffer.str();
                if (prev == ":dir") {
                } else if (prev == ":host-context") {
                    auto sub = getSubselector(&i, j, selector);
                } else if (prev == ":is" || prev == ":matches") {
                    auto subList = getSubselectors(&i, j, selector);
                } else if (prev == ":lang") {
                } else if (prev == ":not") {
                    auto sub = getSubselector(&i, j, selector);
                } else if (prev == ":nth-child") {
                    auto formula = parseFormula(&i, j, selector);
                } else if (prev == ":nth-last-child") {
                    auto formula = parseFormula(&i, j, selector);
                } else if (prev == ":nth-last-of-type") {
                    auto formula = parseFormula(&i, j, selector);
                } else if (prev == ":nth-of-type") {
                    auto formula = parseFormula(&i, j, selector);
                }
                buffer.clear();
                break;
            }
            case ')':
                //There's been an error; the css has failed somehow
                //To try to save the file, we ignore this parentheses and proceed as if none was present.
                break;
            case '\n':
            case '\t':
            case '\r':
            case '\v':
            case '\f':
            case ' ':
                //There's some weird stuff that happens with whitespace; people might whitespace between combinators,
                //so we need to handle that case. That will require significant effort, and will also require much testing.
                spaceFound = true;
                break;
            case ':': {
                DOMString str = buffer.str();
                if (str.empty() || str == ":") {
                    buffer << ":";
                } else {
                    //Interpret the buffer
                    buffer.clear();
                }
            }
            case '.': {
                DOMString str = buffer.str();
                //Interpret the buffer
                buffer.clear();
            }
            case '[': {
                //Skip ahead to the close bracket; that's where the attribute selector ends
                //We can parse the attribute selector based on that substring between open and close
                weirdFuncs.push_back(parseAttrSelector(&i, j, selector));
            }
            case ']':
                //This is an error; this shouldn't happen
                //We'll silently ignore it to best-effort parse the file
                break;
        }
    }
}

std::vector<css::CSSSelector> css::CSSSelector::parseList(DOMString selectorList) {
    std::vector<css::CSSSelector> selectors;
    unsigned long lastI = 0;
    for (unsigned long i = 0; i < selectorList.length(); i++) {
        if (selectorList[i] == ',') {
            selectors.push_back(parse(selectorList.substr(lastI, i + lastI)));
            lastI = i + 1;
        }
    }
    selectors.push_back(parse(selectorList.substr(lastI, selectorList.length() - lastI)));
    return selectors;
}