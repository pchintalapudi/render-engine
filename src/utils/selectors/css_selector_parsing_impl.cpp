//
// Created by prem on 11/24/2018.
//
#include <iostream>
#include <sstream>
#include "include/utils/selectors/css_selector_parsing_impl.h"
#include "include/nodes/element.h"
#include "include/nodes/document.h"

struct NthSel {
    NthSel() : A(0), b(0) {}

    NthSel(long A, long b) : A(A), b(b) {}

    long A;
    long b;
};
namespace {
    const NthSel odd = NthSel(2, 1);
    const NthSel even = NthSel(2, 0);
}

css::CSSSelector getSubselector(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1, openCount = 1;
    std::stringstream output;
    for (; k < j; output << selector[k++]) {
        switch (selector[k]) {
            case '(':
                openCount++;
                break;
            case ')':
                openCount--;
                if (!openCount) {
                    *i = k;
                    return css::parse(output.str());
                }
            default:
                break;
        }
    }
    *i = k;
    return css::parse(output.str());
}

std::vector<css::CSSSelector>
getSubselectors(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1, openCount = 1;
    for (; k < j && openCount; k++) {
        switch (selector[k]) {
            case '(':
                openCount++;
                break;
            case ')':
                openCount--;
                if (!openCount) {
                    *i = k;
                    return css::parseList(selector.substr(*i + 1, k - *i + 2));
                }
                break;
            default:
                break;
        }
    }
    *i = k;
    return css::parseList(selector.substr(*i + 1, k - *i + 2));
}

const NthSel parseFormula(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1;
    NthSel ret;
    std::stringstream stream;
    while (k < j && selector[k] != 'n' && selector[k] != ')') if (!isspace(selector[k++])) stream << selector[k - 1];
    if (selector[k] == 'n') {
        if (stream.str() == "eve") return even;
        else stream >> ret.A;
    } else {
        auto str = stream.str();
        if (str == "odd") return odd;
        stream >> ret.b;
        *i = k;
        return ret;
    }
    k++;
    stream.str("");
    stream.clear();
    while (k < j && selector[k] != ')') if (selector[k] != '+' && !isspace(selector[k++])) stream << selector[k - 1];
    if (!stream.str().empty())
        stream >> ret.b;
    *i = k + 1;
    return ret;
}

enum class AttrSelectorType {
    MATCH, CONTAIN, END, LIST, HYPHEN, BEGIN
};

bool equalsIgnoreCase(DOMString s1, DOMString s2) {
    if (s1.length() != s2.length()) return false;
    for (unsigned long i = 0; i < s1.length(); i++) if (std::tolower(s1[i]) != std::tolower(s2[i])) return false;
    return true;
}

bool containsIgnoreCase(DOMString s1, DOMString s2) {
    std::stringstream stream;
    for (unsigned long i = 0; i < s1.length(); stream << s1[i++]);
    s1 = stream.str();
    stream.str("");
    stream.clear();
    for (unsigned long i = 0; i < s2.length(); stream << s2[i++]);
    s2 = stream.str();
    return s1.find(s2) != DOMString::npos;
}

bool inList(DOMString list, DOMString val) {
    std::stringstream stream;
    for (auto character : list) {
        if (character == ' ') {
            auto str = stream.str();
            if (str == val) return true;
            stream.str("");
            stream.clear();
        } else {
            stream << character;
        }
    }
    return stream.str() == val;
}

bool inListIgnoreCase(DOMString list, DOMString val) {
    std::stringstream stream;
    for (auto character : list) {
        if (character == ' ') {
            auto str = stream.str();
            if (equalsIgnoreCase(str, val)) return true;
            stream.str("");
            stream.clear();
        } else {
            stream << character;
        }
    }
    return stream.str() == val;
}

std::pair<std::function<bool(const dom::Element *)>, DOMString> *
parseAttrSelector(unsigned long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1;
    std::stringstream stream, combined;
    for (; k < j && selector[k] != '=' && selector[k] != ']'; stream << selector[k++]);
    if (selector[k] == '=' && k > 0) {
        AttrSelectorType type;
        DOMString attr;
        switch (selector[k - 1]) {
            case '*':
                type = AttrSelectorType::CONTAIN;
                attr = stream.str().substr(0, k - 1);
                break;
            case '$':
                type = AttrSelectorType::END;
                attr = stream.str().substr(0, k - 1);
                break;
            case '^':
                type = AttrSelectorType::BEGIN;
                attr = stream.str().substr(0, k - 1);
                break;
            case '~':
                type = AttrSelectorType::LIST;
                attr = stream.str().substr(0, k - 1);
                break;
            case '|':
                type = AttrSelectorType::HYPHEN;
                attr = stream.str().substr(0, k - 1);
                break;
            default:
                type = AttrSelectorType::MATCH;
                attr = stream.str().substr(0, k);
        }
        combined << '[' << attr << '=' << '"';
        while (isspace(selector[k++]));
        for (; selector[k] != '"' && selector[k] != ']'; stream << selector[k++]);
        auto val = stream.str();
        combined << val << '"';
        stream.str("");
        stream.clear();
        while (isspace(selector[k++]));
        k--;
        bool caseSensitive;
        if ((caseSensitive = (selector[k] == 'i' || selector[k] == 'I'))) combined << ' ' << 'i';
        combined << ']';
        while (selector[k++] != ']');
        *i = k;
        switch (type) {
            case AttrSelectorType::MATCH:
                if (!caseSensitive)
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && attribute->getValue() == val;
                            }, combined.str());
                else
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && equalsIgnoreCase(attribute->getValue(), val);
                            }, combined.str());
            case AttrSelectorType::CONTAIN:
                if (!caseSensitive)
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && attribute->getValue().find(val) != DOMString::npos;
                            }, combined.str());
                else
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && containsIgnoreCase(attribute->getValue(), val);
                            }, combined.str());
            case AttrSelectorType::END:
                if (!caseSensitive)
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() &&
                                       value.substr(value.length() - val.length(), val.length()) == val;
                            }, combined.str());
                else
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() &&
                                       equalsIgnoreCase(value.substr(value.length() - val.length(), val.length()), val);
                            }, combined.str());
            case AttrSelectorType::BEGIN:
                if (!caseSensitive)
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute)return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() && value.substr(0, val.length()) == val;
                            }, combined.str());
                else
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute)return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() &&
                                       equalsIgnoreCase(value.substr(0, val.length()), val);
                            }, combined.str());
            case AttrSelectorType::HYPHEN:
                if (!caseSensitive)
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return (value == val) ||
                                       (value.length() > val.length() &&
                                        (val + "-") == value.substr(0, val.length() + 1));
                            }, combined.str());
                else
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute)return false;
                                auto value = attribute->getValue();
                                return equalsIgnoreCase(value, val)
                                       || (value.length() > val.length()
                                           && equalsIgnoreCase(value.substr(0, val.length() + 1), val + "-"));
                            }, combined.str());
            case AttrSelectorType::LIST:
                if (!caseSensitive)
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return inList(value, val);
                            }, combined.str());
                else
                    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                            [attr, val](const dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return inListIgnoreCase(value, val);
                            }, combined.str());
        }
    } else {
        auto attr = stream.str();
        combined << '[' << attr << ']';
        *i = k;
        return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(
                [attr](const dom::Element *element) { return element->getAttributes().getNamedItem(attr) != nullptr; },
                combined.str());
    }
}

template<typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

bool isNthChild(NthSel &formula, const dom::Element *element) {
    if (element->getParentNode()) {
        long long idx = 0;
        if (element->getParentElement()) {
            auto children = element->getParentElement()->getChildren();
            while (idx < children.size() && children.get(idx++) != element);
            idx--;
        } else {
            for (unsigned long i = 0; i < element->getParentNode()->getChildNodes().size(); i++) {
                auto child = element->getParentNode()->getChildNodes().get(i);
                if (child == element) break;
                if (child->getNodeType() == dom::NodeType::ELEMENT_NODE) idx++;
            }
        }
        idx -= formula.b;
        return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
    } else return formula.b == 0;
}

bool isNthLastChild(NthSel &formula, const dom::Element *element) {
    if (element->getParentNode()) {
        long long idx = 0;
        if (element->getParentElement()) {
            auto children = element->getParentElement()->getChildren();
            auto size = children.size();
            while (idx < children.size() && children.get(size - idx++ - 1) != element);
            idx--;
        } else {
            for (unsigned long i = element->getParentNode()->getChildNodes().size(); i-- > 0;) {
                auto child = element->getParentNode()->getChildNodes().get(i);
                if (child == element) break;
                if (child->getNodeType() == dom::NodeType::ELEMENT_NODE) idx++;
            }
        }
        idx -= formula.b;
        return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
    } else return formula.b == 0;
}

bool isNthOfType(NthSel &formula, DOMString type, const dom::Element *element) {
    if (type == "*") return isNthChild(formula, element);
    long long idx = 0;
    if (element->getParentElement()) {
        auto parent = element->getParentElement();
        for (unsigned long i = 0; i < parent->getChildren().size(); i++) {
            auto child = parent->getChildren().get(i);
            if (child == element) break;
            if (child->getTagName() == type) idx++;
        }
    } else if (element->getParentNode()) {
        auto parent = element->getParentNode();
        for (unsigned long i = 0; i < parent->getChildNodes().size(); i++) {
            auto child = parent->getChildNodes().get(i);
            if (child == element) break;
            if (child->getNodeType() == dom::NodeType::ELEMENT_NODE
                && static_cast<dom::Element *>(child)->getTagName() == type)
                idx++;
        }
    } else return formula.b == 0;
    idx -= formula.b;
    return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
}

bool isNthLastOfType(NthSel &formula, DOMString type, const dom::Element *element) {
    if (type == "*") return isNthLastChild(formula, element);
    long long idx = 0;
    if (element->getParentElement()) {
        auto parent = element->getParentElement();
        for (unsigned long i = parent->getChildren().size(); i-- > 0;) {
            auto child = parent->getChildren().get(i);
            if (child == element) break;
            if (child->getTagName() == type) idx++;
        }
    } else if (element->getParentNode()) {
        auto parent = element->getParentNode();
        for (unsigned long i = parent->getChildNodes().size(); i-- > 0;) {
            auto child = parent->getChildNodes().get(i);
            if (child == element) break;
            if (child->getNodeType() == dom::NodeType::ELEMENT_NODE &&
                static_cast<dom::Element *>(child)->getTagName() == type)
                idx++;
        }
    } else return formula.b == 0;
    idx -= formula.b;
    return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
}

bool isActive(const dom::Element *element) {
    return element->getOwner() ? element->getOwner()->getPseudoclassManager().isActive(element) : false;
}

bool isAnyLink(const dom::Element *element) {
    DOMString tag = element->getTagName();
    dom::Attr *attr = element->getAttributes().getNamedItem("href");
    return (tag == "a" || tag == "area" || tag == "link") && (attr && !attr->getValue().empty());
}

bool isFirstChild(const dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        if (children.get(i) == element) return true;
        else if (children.get(i)->getNodeType() == dom::NodeType::ELEMENT_NODE) break;
    }
    return false;
}

bool isFirstOfType(const dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        if (children.get(i) == element) return true;
        else if (children.get(i)->getNodeType() == dom::NodeType::ELEMENT_NODE
                 && static_cast<dom::Element *>(children.get(i))->getTagName() == element->getTagName())
            break;
    }
    return false;
}

bool isFocus(const dom::Element *element) {
    return element->getOwner() && element->getOwner()->getPseudoclassManager().isFocused(element);
}

bool containsFocus(const dom::Element *element) {
    return element->getOwner() && element->getOwner()->getPseudoclassManager().hasFocus(element);
}

bool isHover(const dom::Element *element) {
    return element->getOwner() && element->getOwner()->getPseudoclassManager().isHover(element);
}

bool isLastChild(const dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = children.size(); i > 0; i--) {
        auto child = children.get(i - 1);
        if (child == element) return true;
        if (child->getNodeType() == dom::NodeType::ELEMENT_NODE) break;
    }
    return false;
}

bool isLastOfType(const dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = children.size(); i-- > 0;) {
        auto child = children.get(i);
        if (child == element) return true;
        if (child->getNodeType() == dom::NodeType::ELEMENT_NODE
            && static_cast<dom::Element *>(child)->getTagName() == element->getTagName())
            break;
    }
    return false;
}

bool isOnlyChild(const dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child != element && child->getNodeType() == dom::NodeType::ELEMENT_NODE) return false;
    }
    return true;
}

bool isOnlyOfType(const dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child != element && child->getNodeType() == dom::NodeType::ELEMENT_NODE
            && static_cast<dom::Element *>(child)->getTagName() == element->getTagName())
            return false;
    }
    return true;
}

bool isRoot(const dom::Element *element) {
    return element->getOwner() && element->getOwner()->getDocumentElement() == element;
}

bool isTarget(const dom::Element *element) {
    return false;//TODO: somehow determine current url fragment from the element; maybe it's a property of Document
}

std::pair<std::function<bool(const dom::Element *)>, DOMString> *
generatePair(std::function<bool(const dom::Element *)> func, DOMString type) {
    return new std::pair<std::function<bool(const dom::Element *)>, DOMString>(func, type);
}

void interpretBuffer(DOMString *tagName, DOMString *id, std::vector<DOMString> &classes,
                     std::vector<std::pair<std::function<bool(const dom::Element *)>, DOMString> *> &weirdFuncs,
                     DOMString str) {
    if (!str.empty())
        switch (str[0]) {
            default:
                *tagName = str;
                break;
            case '#':
                *id = str.substr(1, str.length() - 1);
                break;
            case '.':
                classes.push_back(str.substr(1, str.length() - 1));
                break;
            case ':':
                str = str.length() > 2 ? str[1] == ':' ? str.substr(2, str.length() - 2)
                                                       : str.substr(1, str.length() - 1) : str.substr(1,
                                                                                                      str.length() - 1);
                if (str == "active") {
                    weirdFuncs.push_back(generatePair(isActive, ":active"));
                } else if (str == "any-link" || str == "link") {
                    weirdFuncs.push_back(generatePair(isAnyLink, ":link"));
                } else if (str == "first-child") {
                    weirdFuncs.push_back(generatePair(isFirstChild, ":first-child"));
                } else if (str == "first-of-type") {
                    weirdFuncs.push_back(generatePair(isFirstOfType, ":first-of-type"));
                } else if (str == "focus") {
                    weirdFuncs.push_back(generatePair(isFocus, ":focus"));
                } else if (str == "focus-within") {
                    weirdFuncs.push_back(generatePair(containsFocus, ":focus-within"));
                } else if (str == "hover") {
                    weirdFuncs.push_back(generatePair(isHover, ":hover"));
                } else if (str == "last-child") {
                    weirdFuncs.push_back(generatePair(isLastChild, ":last-child"));
                } else if (str == "last-of-type") {
                    weirdFuncs.push_back(generatePair(isLastOfType, ":last-of-type"));
                } else if (str == "only-child") {
                    weirdFuncs.push_back(generatePair(isOnlyChild, ":only-child"));
                } else if (str == "only-of-type") {
                    weirdFuncs.push_back(generatePair(isOnlyOfType, ":only-of-type"));
                } else if (str == "root") {
                    weirdFuncs.push_back(generatePair(isRoot, ":root"));
                } else if (str == "target") {
                    weirdFuncs.push_back(generatePair(isTarget, ":target"));
                }
                break;
        }
}

void handleSwitch(bool *spaceFound, css::CSSSelectorRelation *relation, DOMString *type, DOMString *id,
                  std::vector<DOMString> &classes,
                  std::vector<std::pair<std::function<bool(const dom::Element *)>, DOMString> *> &weirdFuncs,
                  std::vector<std::pair<css::CSSSelectorToken *, css::CSSSelectorRelation> *> &relations,
                  std::vector<css::CSSSelectorTokenGroup *> &selVector) {
    if (*spaceFound) {
        if (*relation == css::CSSSelectorRelation::NONE) {
            css::CSSSelectorToken *token = new css::CSSSelectorToken(*type, *id, classes, weirdFuncs);
            *type = "*";
            *id = "";
            classes.clear();
            weirdFuncs.clear();
            css::CSSSelectorTokenGroup *tokenGroup = new css::CSSSelectorTokenGroup(token, relations);
            selVector.push_back(tokenGroup);
        }
        *spaceFound = false;
    }
    if (*relation != css::CSSSelectorRelation::NONE) {
        css::CSSSelectorToken *token = new css::CSSSelectorToken(*type, *id, classes, weirdFuncs);
        *type = "*";
        *id = "";
        classes.clear();
        weirdFuncs.clear();
        relations.push_back(new std::pair<css::CSSSelectorToken *, css::CSSSelectorRelation>(token, *relation));
        *relation = css::CSSSelectorRelation::NONE;
    }
}

css::CSSSelector css::parse(DOMString selector) {
    //Plan of action:
    //Iterate over the selector one char at a time
    //If a special character is encountered stop and interpret

    //Set up start and end indeces to skip over arbitrary whitespace
    unsigned long j = selector.length(), i = 0;
    while (j-- > 0 && isspace(selector[j]));
    j++;
    while (i < j && isspace(selector[i++]));
    i--;
    //Set up char buffer
    std::stringstream buffer;
    //Set up selector vector
    std::vector<CSSSelectorTokenGroup *> selVector;
    //Set up token group relations
    std::vector<std::pair<CSSSelectorToken *, CSSSelectorRelation> *> relations;
    //Set up token
    DOMString tagName = "*";
    DOMString id;
    std::vector<DOMString> classes;
    std::vector<std::pair<std::function<bool(const dom::Element *)>, DOMString> *> weirdFuncs;
    //Monitor spaces
    CSSSelectorRelation relation = CSSSelectorRelation::NONE;
    bool spaceFound = false;
    std::cout << i << " " << j << std::endl;
    for (; i < j; i++) {
        switch (selector[i]) {
            default:
                handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
                buffer << selector[i];
                break;
            case '(': {
                DOMString prev = buffer.str();
                if (prev == ":dir") {
                } else if (prev == ":host-context") {
                    auto sub = getSubselector(&i, j, selector);
                    auto func = [sub](
                            const dom::Element *element) { return false;/*we don't support shadow roots yet*/ };
                    weirdFuncs.push_back(generatePair(func, ":host-context(" + sub.toString() + ")"));
                } else if (prev == ":is" || prev == ":matches" || prev == ":where") {
                    auto subList = getSubselectors(&i, j, selector);
                    auto func = [subList](const dom::Element *element) {
                        for (const auto sub : subList) if (sub.matches(element)) return true;
                        return false;
                    };
                    DOMString output;
                    (output += ":is(") += subList[0].toString();
                    for (unsigned long k = 1; k < subList.size(); k++) (output += ", ") += subList[k].toString();
                    output += ')';
                    weirdFuncs.push_back(generatePair(func, output));
                } else if (prev == ":lang") {
                } else if (prev == ":not") {
                    auto sub = getSubselector(&i, j, selector);
                    auto func = [sub](const dom::Element *element) { return !sub.matches(element); };
                    weirdFuncs.push_back(generatePair(func, ":not(" + sub.toString() + ")"));
                } else if (prev == ":nth-child") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(generatePair(
                            [&formula](const dom::Element *element) { return isNthChild(formula, element); },
                            ":nth-child(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) + ")"));
                } else if (prev == ":nth-last-child") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(generatePair(
                            [&formula](const dom::Element *element) { return isNthLastChild(formula, element); },
                            ":nth-last-child(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) + ")"));
                } else if (prev == ":nth-last-of-type") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(generatePair(
                            [&formula, tagName](const dom::Element *element) {
                                return isNthLastOfType(formula, tagName, element);
                            },
                            ":nth-last-of-type(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) +
                            ")"));
                } else if (prev == ":nth-of-type") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(generatePair(
                            [&formula, tagName](const dom::Element *element) {
                                return isNthOfType(formula, tagName, element);
                            },
                            ":nth-of-type(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) + ")"));
                }
                buffer.str("");
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
                interpretBuffer(&tagName, &id, classes, weirdFuncs, buffer.str());
                buffer.str("");
                buffer.clear();
                spaceFound = true;
                break;
            case ':': {
                handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
                DOMString str = buffer.str();
                if (str != ":") {
                    interpretBuffer(&tagName, &id, classes, weirdFuncs, str);
                    buffer.str("");
                    buffer.clear();
                }
                buffer << ":";
                break;
            }
            case '.': {
                handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
                DOMString str = buffer.str();
                interpretBuffer(&tagName, &id, classes, weirdFuncs, str);
                buffer.str("");
                buffer.clear();
                buffer << ".";
                break;
            }
            case '#': {
                handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
                DOMString str = buffer.str();
                interpretBuffer(&tagName, &id, classes, weirdFuncs, str);
                buffer.str("");
                buffer.clear();
                buffer << "#";
                break;
            }
            case '*': {
                handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
                //The only valid place where this can appear is in front of a group of selectors
                //Therefore, since the default value of tagName is a "*", this does not need to be handled.
                break;
            }
            case '[': {
                handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
                //Skip ahead to the close bracket; that's where the attribute selector ends
                //We can parse the attribute selector based on that substring between open and close
                weirdFuncs.push_back(parseAttrSelector(&i, j, selector));
            }
            case ']':
                //This is an error; this shouldn't happen
                //We'll silently ignore it to best-effort parse the file
                break;
            case '~':
                relation = CSSSelectorRelation::SIBLING;
                interpretBuffer(&tagName, &id, classes, weirdFuncs, buffer.str());
                buffer.str("");
                buffer.clear();
                break;
            case '+':
                relation = CSSSelectorRelation::IMMEDIATE_SIBLING;
                interpretBuffer(&tagName, &id, classes, weirdFuncs, buffer.str());
                buffer.str("");
                buffer.clear();
                break;
            case '>':
                relation = CSSSelectorRelation::DESCENDANT;
                interpretBuffer(&tagName, &id, classes, weirdFuncs, buffer.str());
                buffer.str("");
                buffer.clear();
                break;
        }
    }
    interpretBuffer(&tagName, &id, classes, weirdFuncs, buffer.str());
    spaceFound = true;
    handleSwitch(&spaceFound, &relation, &tagName, &id, classes, weirdFuncs, relations, selVector);
    return css::CSSSelector(selVector);
}

std::vector<css::CSSSelector> css::parseList(DOMString selectorList) {
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